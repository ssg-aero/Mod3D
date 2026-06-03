# Rendering OCCT shapes

Mod3D ships two rendering backends. Both take and return OCCT types, and both
share one backend-neutral scene decomposition (`mod3d.render._scene`), so the
geometry **instancing** logic is identical between them: shapes that share a
TShape — typically the rotated copies of a periodic blade row built with
[`make_periodic_compound`](theory/mesh_distance.md) — are tessellated **once**
and drawn N times under N placement matrices, instead of uploading N identical
meshes to the GPU.

## Which backend?

| | `ShapeRenderer` (pythreejs) | `PyVistaRenderer` (PyVista / VTK) |
|---|---|---|
| Target | Jupyter notebook only | Desktop window, notebook (trame), **and** shareable HTML |
| Dependency | `pythreejs` | `pyvista` (+ `trame` for the web export) |
| Best for | a quick look inside a notebook, no VTK needed | desktop inspection, large scenes, sharing a result, and a path to CFD scalar fields later |

Rule of thumb: reach for **pythreejs** when you just want to glance at a shape
in a notebook without pulling VTK; reach for **PyVista** when you want a desktop
window, need to share an interactive result, or are heading toward CFD
post-processing.

Both renderers are imported **lazily**: `import mod3d` pulls neither pythreejs
nor pyvista — only constructing a renderer triggers its backend import.

## PyVista: three output targets from one call

```python
from mod3d.render.pyvista import PyVistaRenderer

renderer = PyVistaRenderer(linear_deflection=0.4)
renderer.add_shape(blade_row, {"surface_color": "#2194ce"})
renderer.add_shape(endoscope, {"color": "#d62728", "face_opacity": 0.6})

renderer.render(mode="window")                       # interactive desktop window
renderer.render(mode="notebook")                     # interactive Jupyter widget (trame)
renderer.render(mode="html", path="scene.html")      # self-contained shareable HTML
```

`mode="html"` serialises the scene through trame's vtk.js bridge into a single
self-contained file — no server, openable in any browser. It needs the trame
stack (`trame`, `trame-vtk`) and, depending on the PyVista build, `nest_asyncio2`.

## pythreejs

```python
from mod3d import ShapeRenderer

renderer = ShapeRenderer()
renderer.add_shape(blade_row)
renderer.render()        # returns a pythreejs Renderer widget for the notebook
```

## Instancing in practice

For a 24-blade row, both backends upload the master blade's triangulation
**once** and place 24 copies by matrix — GPU memory stays flat regardless of the
blade count. This is what makes interactively spinning a full periodic stage (or
querying a moving endoscope against it) cheap.

## Worked example

`examples/vista.py` builds a periodic blade row, measures the minimum clearance
to a moving "endoscope" with
[`MeshDistance`](theory/mesh_distance.md), and renders the scene in any of the
three PyVista modes:

```bash
python examples/vista.py --mode window
python examples/vista.py --mode html --output blade_row.html
```
