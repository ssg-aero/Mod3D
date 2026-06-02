"""Étape 0 — quantifier la duplication GPU du rendu sur une géométrie périodique.

Le pipeline de rendu (visualyse3js) « cuit » la transformation de chaque face
dans ses coordonnées (TessellationUtils.cpp: Node().Transformed(...)). Pour un
compound périodique fait de N copies d'un même master (TShape partagé), chaque
copie redonne donc les mêmes triangles re-transformés : la triangulation du
master est dupliquée N fois dans ce qui est uploadé au GPU.

Ce script mesure cette duplication AVANT toute optimisation :
  - sommets / triangles extraits pour le compound de N copies
  - vs sommets / triangles du master seul
  - ratio (cible après instanciation : ~1x au lieu de ~Nx)
  - octets uploadés estimés (positions + normales + indices)
  - coût de ré-extraction (ce qui serait re-fait à chaque render())

Aucune dépendance navigateur : on mesure au niveau de Render.extract_tessellation,
qui est exactement la donnée que faces_mesh empile dans la BufferGeometry. Si
pythreejs est importable, on confirme aussi la taille de la BufferGeometry finale.
"""
from __future__ import annotations

import sys
import time

import numpy as np

# La console Windows (cp1252) ne sait pas encoder les caractères accentués /
# mathématiques utilisés ci-dessous ; on force UTF-8 quand c'est possible.
try:
    sys.stdout.reconfigure(encoding="utf-8")
except (AttributeError, ValueError):
    pass

from mod3d import Render, gp, BRepBuilderAPI
from mod3d.periodic import make_periodic_compound


def _master_blade():
    # Sphère = surface courbe : le nombre de triangles dépend de la déflection
    # (un master plan donnerait un compte trivial et non représentatif).
    return BRepBuilderAPI.MakeSphere(gp.Pnt(40.0, 0.0, 0.0), 8.0).shape()


def _z_axis():
    return gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))


def _tessellation_size(shape, deflection, angle_deflection=20.0):
    """Renvoie (n_faces, n_vertices, n_triangles, n_bytes) tels qu'extraits —
    c.-à-d. ce que le pipeline actuel empilerait dans une BufferGeometry."""
    faces_data, _edges = Render.extract_tessellation(
        shape, deflection, False, angle_deflection, True, True)
    n_faces = len(faces_data)
    n_vertices = sum(v.shape[0] for _t, v, _n, _uv in faces_data)
    n_triangles = sum(t.shape[0] for t, _v, _n, _uv in faces_data)
    # positions f32x3 + normales f32x3 + indices u32x3
    n_bytes = n_vertices * 3 * 4 * 2 + n_triangles * 3 * 4
    return n_faces, n_vertices, n_triangles, n_bytes


def main():
    master = _master_blade()
    axis = _z_axis()
    deflection = 0.2

    mf, mv, mt, mb = _tessellation_size(master, deflection)
    print(f"Master seul (deflection={deflection}):")
    print(f"  faces={mf}  vertices={mv}  triangles={mt}  ~{mb/1024:.1f} KiB\n")

    print(f"{'N':>4s} {'faces':>7s} {'vertices':>10s} {'triangles':>10s} "
          f"{'~KiB':>9s} {'vert ratio':>11s} {'extract ms':>11s}")
    for n in [1, 6, 12, 24, 60]:
        compound = make_periodic_compound(master, axis, n)

        t0 = time.perf_counter()
        nf, nv, nt, nb = _tessellation_size(compound, deflection)
        extract_ms = (time.perf_counter() - t0) * 1e3

        ratio = nv / max(mv, 1)
        print(f"{n:>4d} {nf:>7d} {nv:>10d} {nt:>10d} "
              f"{nb/1024:>9.1f} {ratio:>10.1f}x {extract_ms:>10.1f}")

    print("\nLecture :")
    print("  - 'vert ratio' ≈ N : la triangulation du master est dupliquée N")
    print("    fois dans le buffer GPU. Cible après instanciation : ~1x.")
    print("  - '~KiB' est la mémoire GPU approximative (positions+normales+index)")
    print("    uploadée pour les faces du compound.")
    print("  - 'extract ms' est le coût de tessellation/extraction re-payé à")
    print("    chaque render() ; le cache de l'Étape 2 doit le ramener à ~0.")

    # Confirmation optionnelle sur la BufferGeometry réellement construite.
    try:
        from mod3d.visualyse3js import occt_to_threejs
    except Exception as exc:  # pythreejs/ipywidgets absent : on s'arrête là
        print(f"\n(pythreejs indisponible, confirmation BufferGeometry sautée : {exc})")
        return

    n = 24
    compound = make_periodic_compound(master, axis, n)
    mesh_face, _edges = occt_to_threejs(
        compound, linear_deflection=deflection, angle_deflection=20.0)

    # Après l'Étape 1 : une liste de N meshes partageant 1 BufferGeometry.
    meshes = mesh_face if isinstance(mesh_face, list) else [mesh_face]
    # VRAM réelle = somme des sommets sur les BufferGeometry *uniques* (les
    # copies partagent le même objet géométrie, donc un seul upload).
    seen = {}
    for m in meshes:
        seen[id(m.geometry)] = m.geometry.attributes['position'].array.shape[0]
    vram_vertices = sum(seen.values())

    print(f"\nÉtape 1 (N={n}) : {len(meshes)} meshes, "
          f"{len(seen)} BufferGeometry unique(s).")
    print(f"  Sommets résidents en VRAM : {vram_vertices} "
          f"(soit {vram_vertices/max(mv,1):.1f}x le master).")
    print(f"  Avant l'Étape 1 : {n * mv} sommets ({n}x). "
          f"Réduction ~{(n * mv)/max(vram_vertices,1):.1f}x.")


if __name__ == "__main__":
    main()
