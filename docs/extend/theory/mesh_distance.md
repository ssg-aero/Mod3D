# MeshDistance — distance approchée maillée, accélérée par BVH

> Module : `extend/extrema/` · Binding Python : `mod3d.BRepExtrema.MeshDistance`
> Helper associé : `mod3d.periodic.make_periodic_compound`
> Branche : `feat/mesh-distance`

## 1. Contexte et motivation

### Le cas d'usage cible

Tester **la distance minimale d'un endoscope déformable** vis-à-vis d'une
**rangée d'aubes de turbomachine**, à l'intérieur d'une boucle d'optimisation.

Caractéristiques qui déterminent toute la conception :

| Acteur | Rôle | Comportement dans la boucle |
|--------|------|------------------------------|
| Endoscope | *query* (forme mobile) | **change de forme à chaque itération** (déformation / points de contrôle) |
| Rangée d'aubes | *reference* (cible fixe) | **strictement identique sur toute la boucle** |
| Rangée d'aubes | structure | **périodique en rotation** — une seule aube à mailler, le reste est déduit par rotation |

Besoins :

- seulement le **minimum global** de distance (pas tout le champ) ;
- une **précision approchée (maillage) acceptable** — pas besoin de l'exactitude NURBS ;
- la boucle externe est **séquentielle par nature** (non parallélisable).

### Pourquoi `BRepExtrema_DistShapeShape` ne suffit pas

`DistShapeShape` calcule l'extremum **exact** sur les NURBS paramétriques. C'est
précis mais coûteux, et **chaque appel repart de zéro** : aucune structure n'est
réutilisée d'une itération à l'autre. Pour des milliers d'appels contre la *même*
référence, on paie en boucle un travail qui pourrait être amorti.

> **Le maillage ne « rend pas `DistShapeShape` plus rapide ».** `DistShapeShape`
> travaille sur les NURBS, pas sur des triangles. Le gain ne vient pas de
> mailler — il vient d'**amortir la construction d'une structure d'accélération
> (BVH) sur une référence fixe**, puis de ne faire qu'une descente d'arbre par
> requête.

### Pourquoi pas `BRepExtrema_ShapeProximity`

Piste envisagée puis **écartée** : `ShapeProximity` calcule une **distance de
Hausdorff dirigée** (max-min), pas la distance minimale (min-min). C'est la
mauvaise métrique pour une clairance/jeu. L'avoir identifié tôt a évité une
implémentation inutile.

## 2. Qu'est-ce qu'un BVH ?

**BVH = Bounding Volume Hierarchy** (hiérarchie de volumes englobants).

C'est un **arbre** où chaque nœud porte une **boîte englobante** (axis-aligned
bounding box, AABB) qui contient toute la géométrie de son sous-arbre :

- les **feuilles** contiennent les primitives réelles — ici, les **triangles**
  du maillage de la référence ;
- chaque **nœud interne** contient une boîte qui englobe les boîtes de ses
  enfants ;
- la **racine** englobe donc toute la référence.

```
                  ┌───────────────┐
                  │  AABB racine  │   englobe toute la référence
                  └───────┬───────┘
              ┌───────────┴───────────┐
        ┌─────┴─────┐           ┌─────┴─────┐
        │   AABB    │           │   AABB    │     nœuds internes
        └─────┬─────┘           └─────┬─────┘
        ┌─────┴─────┐           ┌─────┴─────┐
      △ △ △ △      △ △ △ △     △ △ ... △     feuilles = triangles
```

**À quoi ça sert pour une distance point→surface.** Sans BVH, trouver le triangle
le plus proche d'un point demande de tester **tous** les triangles — coût
O(nombre de triangles) par point. Avec un BVH, on descend l'arbre en
**élaguant** : si la boîte d'un nœud est déjà plus loin que le meilleur triangle
trouvé jusque-là, **tout son sous-arbre est ignoré** sans tester un seul de ses
triangles. Le coût moyen tombe à ≈ O(log n).

Deux opérations géométriques suffisent à cette descente :

1. **distance² point → boîte** (`PointBoxSquareDistance`) — borne inférieure pour
   décider d'élaguer un nœud ;
2. **distance² point → triangle** (`PointTriangleProjection`) — la vraie distance
   au niveau des feuilles.

OCCT fournit cette machinerie : `BRepExtrema_TriangleSet` (qui dérive de
`BVH_PrimitiveSet3d`) construit le BVH à partir des triangulations de faces, et
`BVH_Distance<…>` fournit le squelette de descente simple-arbre. On ne
réimplémente donc pas l'arbre — on branche notre logique dessus (§ 3).

## 3. Conception

### Idée centrale

```
Construction (une fois)          Requête (à chaque itération)
─────────────────────            ────────────────────────────
mesh(reference)                  mesh(query)  [si nécessaire]
   │                                 │
build BVH over reference         pour chaque sommet du maillage query :
triangles                            descente BVH point→triangle
   │                                     │
BRepExtrema_TriangleSet          min courant des distances²
(persistant dans l'objet)            │
                                 sqrt(min) → distance approchée
```

La référence est maillée et son BVH construit **une seule fois** (au constructeur
ou via `set_reference`). Chaque `distance_to` ne fait qu'une descente d'arbre par
sommet du maillage de la requête.

### Brique d'accélération : descente BVH simple-arbre

`PointToTriangleSetDistance` dérive de
`BVH_Distance<Standard_Real, 3, BVH_Vec3d, BRepExtrema_TriangleSet>` et
n'implémente que deux méthodes :

- **`RejectNode`** — élague une boîte de l'arbre via
  `BVH_Tools::PointBoxSquareDistance`. Si la distance² minimale du point à la
  boîte dépasse le meilleur trouvé, tout le sous-arbre est rejeté.
- **`Accept`** — pour un triangle feuille, projette le point avec
  `BVH_Tools::PointTriangleProjection`, met à jour le minimum et mémorise le
  point projeté (pour pouvoir restituer `point_on_reference`).

On travaille en **distances au carré** pendant toute la descente (pas de `sqrt`
dans la boucle chaude), avec un unique `sqrt` final.

### Pourquoi `BRepExtrema_TriangleSet`

`BRepExtrema_TriangleSet` (qui étend `BVH_PrimitiveSet3d`) sait construire un BVH
directement à partir des triangulations de faces OCCT et fournit
`GetVertices(i, v0, v1, v2)`. On réutilise l'infrastructure BVH d'OCCT plutôt que
de réimplémenter un arbre.

## 4. Optimisations

L'ordre d'attaque convenu était : **(3) cache → (1) MT → (2) BVH-paire**.
Conclusion : le cache et la périodicité sont les vrais gains ; le MT n'en vaut
pas la peine.

### 4.1 Cache de tessellation côté requête

**Constat de profilage** : `BRepMesh_IncrementalMesh` domine le coût par appel
quand la requête est fraîche (≈ 98 % du temps « fresh »). La descente BVH
elle-même est marginale.

**Mécanisme** : `MeshDistance` mémorise les TShapes de requête déjà maillés à la
déflection courante (`myTessellatedQueries`). Sur un *cache hit*, on saute la
tessellation et on enchaîne directement la descente.

- **Clé du cache** : `Handle(TopoDS_TShape)` (et non un pointeur brut).
- **Pourquoi un Handle** : un pointeur brut `const TopoDS_TShape*` devenait
  invalide quand Python collectait une forme et que l'adresse était réutilisée →
  **faux positifs de cache** sur des formes non maillées (« query shape produced
  no triangulation vertices »). Stocker un `Handle` **maintient le TShape en vie**
  (refcount), donc l'identité du pointeur reste stable. Les foncteurs
  `TShapeHandleHash` / `TShapeHandleEqual` hachent / comparent `h.get()`.
- **Garde défensive** : même sur cache hit, on re-vérifie la triangulation de la
  *première* face (cas où du code externe aurait fait `BRepTools::Clean`). Si
  elle a disparu, on invalide l'entrée et on re-maille.
- **Gestion mémoire** : `clear_query_cache()` libère les refcounts. Indispensable
  dans une boucle qui crée une forme neuve à chaque itération, sinon la mémoire
  croît linéairement avec le nombre de requêtes distinctes.

**Mesure** : ≈ **60,9×** sur une forme de requête réutilisée (cache hit pur vs
re-tessellation).

> ⚠️ Le cache n'aide **que si le même objet `TopoDS_Shape` Python est réutilisé**.
> Une boucle qui reconstruit la forme à chaque tour ne bénéficie pas du cache —
> c'est là qu'intervient l'astuce de périodicité (§ 4.2) côté référence, et le
> fait que l'endoscope, s'il est *déformé* plutôt que *reconstruit*, conserve son
> TShape.

### 4.2 Périodicité par partage de TShape (`make_periodic_compound`)

La rangée d'aubes est périodique : **une aube maillée suffit**, les autres sont
des copies positionnées par rotation.

```python
from mod3d.periodic import make_periodic_compound
from mod3d import gp

axis = gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
blade_row = make_periodic_compound(master_blade, axis, n_sectors=24)
```

`make_periodic_compound(master, axis, n_sectors, angular_step=None)` :

- pour chaque secteur `k`, construit une `gp.Trsf` avec
  `set_rotation(axis, k*step)` et ajoute `master.located(TopLoc_Location(trsf))` ;
- `step` par défaut = `2π / n_sectors` (anneau complet) ; un `angular_step` plus
  petit donne un anneau partiel (fenêtre de quelques aubes) ;
- retourne un `TopoDS.Compound` ;
- `n_sectors < 1` → `ValueError`.

**Le point clé** : `located()` crée N copies qui **partagent le même TShape**
sous-jacent (seule la `TopLoc_Location` diffère). `BRepMesh_IncrementalMesh` ne
maille donc **que le master une fois** ; le BVH voit ensuite N copies de cette
triangulation positionnées autour de l'axe.

**Mesure** : ≈ **4,9×** de temps de construction pour **60×** de triangles —
preuve forte que la tessellation n'est payée qu'une fois (le temps croît
beaucoup moins vite que le nombre de triangles).

### 4.3 Multitâche (MT) — étudié puis abandonné

**Décision : pas de MT.** Décidée *après* benchmark, pas a priori.

Profilage (`tests/profile_mt_potential.py`, rangée 24 secteurs + endoscope
cylindrique, déflection variable) :

| Grandeur | Ordre de grandeur |
|----------|-------------------|
| Descente BVH (query en cache) | 170–450 µs |
| Appel complet (query fraîche) | 1–3,5 ms |
| Fraction « descente » | 5–20 % du total |
| Surcoût spinup `OSD_Parallel` | ≈ 50–200 µs |

Le MT n'attaquerait que la descente (5–20 % du temps), pour un surcoût de
threads qui en dévore une grande part. La boucle externe étant séquentielle par
nature, il n'y a pas non plus de parallélisme externe à exploiter. **ROI trop
faible** → abandonné. La piste BVH-paire (descente deux-arbres) a été retirée par
la même logique : la requête est petite et le coût dominant reste la
tessellation, pas la descente.

## 5. API Python

```python
from mod3d import BRepBuilderAPI, BRepExtrema, gp
from mod3d.periodic import make_periodic_compound

# Référence périodique fixe — maillée + BVH construits une fois
axis = gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
blade_row = make_periodic_compound(master_blade, axis, 24)
mq = BRepExtrema.MeshDistance(blade_row, deflection=0.5)

# Boucle d'optimisation : seul l'endoscope change
for _ in range(n_iterations):
    endoscope = deform(...)                 # TopoDS_Shape
    d, p_ref, p_query = mq.distance_to(endoscope)
    ...
```

### Constructeur

```python
MeshDistance(reference, deflection,
             angle_deflection_deg=30.0, parallel=True)
```

Tessellation de la référence + construction du BVH (payé une fois).
`deflection` doit être strictement positif. `reference` doit contenir au moins
une face triangulable.

### Méthodes et propriétés

| Membre | Type | Rôle |
|--------|------|------|
| `set_reference(reference, deflection, angle_deflection_deg=30.0, parallel=True)` | méthode | Remplace la référence, reconstruit le BVH, **vide le cache de requêtes** (la déflection peut changer). |
| `distance_to(shape)` | méthode | Retourne le tuple `(distance, point_on_reference, point_on_shape)`. Maille `shape` si nécessaire (sinon cache hit). |
| `deflection` | propriété (ro) | Déflection linéaire de tessellation. |
| `nb_reference_triangles` | propriété (ro) | Nombre de triangles dans le BVH de référence. |
| `clear_query_cache()` | méthode | Libère le memo des TShapes de requête déjà maillés (refcounts). À appeler dans les boucles à requêtes jetables. |
| `nb_cached_queries` | propriété (ro) | Nombre de TShapes de requête distincts actuellement en cache. |

### Sémantique du résultat

- `distance` ≥ distance exacte − ε (le maillage est « à l'intérieur » de la
  surface, donc ne sous-estime pas significativement) ;
- l'écart au résultat exact est borné par ≈ la déflection (tests : ≤ 2× la
  déflection).

## 6. Résultats mesurés

| Scénario | Référence | Gain |
|----------|-----------|------|
| Distance brute vs `DistShapeShape` | sphère (NURBS courbe) | ≈ **3,8×** |
| Cache requête (forme réutilisée) | sphère vs pipe | ≈ **60,9×** |
| Périodicité (mailler 1 aube pour N) | sphère, N=60 | **4,9×** temps pour **60×** triangles |
| Robustesse culling BVH | cylindre vs 250 sphères | 250× triangles → ≈ 2,77× temps |

> Leçon de benchmark : les boîtes planes sont déjà rapides pour `DistShapeShape`
> (12 triangles, indépendant de la déflection). Les vrais gains se mesurent sur
> de la **géométrie courbe** (sphère, cylindre) où le nombre de triangles croît
> avec la finesse demandée.

## 7. Fichiers

| Fichier | Rôle |
|---------|------|
| `src/extend/extrema/MeshDistance.hpp` | Déclaration de la classe, `MeshDistanceResult`, foncteurs de cache. |
| `src/extend/extrema/MeshDistance.cpp` | `PointToTriangleSetDistance` (descente BVH), `set_reference`, `distance_to` (cache + descente). |
| `src/bind_brep_extrema.cpp` | Binding pybind11 de `MeshDistance`. |
| `python/mod3d/periodic.py` | `make_periodic_compound` (partage de TShape par rotation). |
| `python/mod3d/__init__.py` | Export de `make_periodic_compound`. |
| `CMakeLists.txt` | Ajout de la source `MeshDistance.cpp` et de `periodic.py` à l'install. |
| `tests/test_mesh_distance.py` | 7 tests : justesse, cache, scaling du culling. |
| `tests/test_periodic.py` | 4 tests : compte des copies, accord avec extrema exact, non-scaling du temps de maillage, workflow endoscope. |
| `tests/profile_mesh_distance.py` | Profilage (la tessellation = 98 % du temps fresh). *(non versionné)* |
| `tests/profile_mt_potential.py` | Profilage du potentiel MT — a motivé l'abandon du MT. *(non versionné)* |

## 8. Décisions de conception (résumé)

1. **Mailler n'accélère pas `DistShapeShape`** — le gain est l'amortissement du
   BVH sur une référence fixe.
2. **`ShapeProximity` rejeté** — mauvaise métrique (Hausdorff max-min ≠ min global).
3. **Cache clé `Handle(TopoDS_TShape)`** — refcount obligatoire pour la stabilité
   d'identité du pointeur face au GC Python.
4. **Périodicité par `located()`** — N aubes, un seul maillage, via partage de
   TShape.
5. **Pas de MT** — la descente ne représente que 5–20 % du temps ; surcoût de
   threads supérieur au gain. Boucle externe séquentielle par nature.
6. **Précision bornée par la déflection** — assumée acceptable pour le cas
   d'usage ; `DistShapeShape` reste la voie exacte.
