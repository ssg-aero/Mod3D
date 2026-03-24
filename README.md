# Mod3D Python Bindings

Python bindings for Open Cascade Technology (OCCT) via `pybind11`, providing a Pythonic API for 3D CAD modeling, Boolean operations, geometry creation, tessellation, and STEP I/O.

## Design Philosophy

This binding layer focuses on **simplifying the OCCT interfaces** and leveraging **standard Python containers** (`list`, `numpy` arrays) instead of OCCT-specific collection types (`TColgp_Array1OfPnt`, `TColStd_Array1OfReal`, …). The goal is to offer a natural, Pythonic experience while retaining full access to the underlying geometry kernel.

Key design choices:

- **Simplified interfaces** – verbose OCCT patterns are wrapped behind concise constructors and properties. For example, `BSplineCurve` accepts plain Python lists of `gp.Pnt` or NumPy arrays directly.
- **Standard containers everywhere** – where OCCT expects `TColgp_*` or `TColStd_*` arrays, the bindings accept and return regular Python lists and NumPy arrays.
- **Direct transformation methods on shapes and geometry** – instead of creating a `gp.Trsf` manually, you can call convenience methods such as `.translated()`, `.rotated()`, `.scaled()`, and `.mirrored()` directly on geometric objects:

```python
from mod3d import gp

ax = gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))

# Translate a point without creating a Trsf
p2 = gp.Pnt(1, 2, 3).translated(gp.Vec(10, 0, 0))

# Rotate an axis system in place
ax2 = ax.rotated(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 1.57)

# Scale a circle
circ = gp.Circ(gp.Ax2(), 5.0)
circ_big = circ.scaled(gp.Pnt(0, 0, 0), 2.0)

# Mirror a plane
pln2 = gp.Pln(gp.Ax3()).mirrored(gp.Pnt(10, 0, 0))
```

These methods return a new, transformed copy — the original object is never mutated.


## Prerequisites

- **CMake 3.30+** with a C++23-capable toolchain.
- **Python 3.10+** interpreter and development headers.
- **Open Cascade Technology** (7.6+ recommended). Set `OpenCascade_DIR` to the installation's CMake config directory.
- **pybind11** (must be findable by CMake).

## Build

```powershell
mkdir -p build
cmake -S . -B build -DOpenCascade_DIR="C:/Program Files/OpenCASCADE/lib/cmake/OpenCASCADE"
cmake --build build --config Release
```

On Linux or macOS, adapt the generator and toolchain flags accordingly (`-G Ninja`, `-DCMAKE_BUILD_TYPE=RelWithDebInfo`, etc.).

## Modules

The `mod3d` package exposes the following submodules:

| Module | Description |
|---|---|
| `gp` | Basic geometric primitives: `Pnt`, `Vec`, `Dir`, `XYZ`, `Ax1`, `Ax2`, `Ax3`, `Trsf`, `Pln`, `Lin2d`, `Pnt2d`, `Dir2d`, `Ax2d` |
| `Geom` | 3D geometry: `Line`, `Circle`, `BSplineCurve`, `BSplineSurface`, `TrimmedCurve`, `CylindricalSurface`, `OffsetSurface`, `SurfaceOfRevolution` |
| `Geom2d` | 2D geometry: `Line2d`, `Circle2d`, `BSplineCurve2d` |
| `GeomAbs` | Enumerations: `CurveType`, `SurfaceType`, `Shape`, `IsoType` |
| `GeomAPI` | Curve interpolation and approximation: `Interpolate` |
| `GeomFill` | Sweep trihedron modes: `Trihedron` |
| `BRepBuilderAPI` | Shape construction: `MakeVertex`, `MakeEdge`, `MakeWire`, `MakeFace`, `MakeBox`, `MakeSphere`, `MakeCylinder`, `MakeCone`, `MakeRevolution`, `MakePipe`, `MakePipeShell`, `MakePrism`, `MakePolygon` |
| `BRepOffsetAPI` | Offset/filling operations: `MakeFilling` |
| `BooleanOp` | Boolean operations: `Common`, `Cut`, `Fuse`, `Section` |
| `BRepFillet` | Fillet operations: `MakeFillet`, `MakeFillet2d` |
| `BRepGProp` | Global properties (mass, centre of mass, volume, surface area): `GProps`, `BRepGProp` |
| `BRepExtrema` | Distance computation: `DistShapeShape` |
| `TopoDS` | Topology data structures: `Shape`, `Vertex`, `Edge`, `Wire`, `Face`, `Shell`, `Solid`, `Compound` |
| `TopExp` | Topology exploration: `get_vertices`, etc. |
| `TopAbs` | Topology enumerations: `ShapeEnum` |
| `Adaptor` | Curve/surface adaptors: `GeomCurve` |
| `ShapeFix` | Shape healing: `Wire` |
| `StepControl` | STEP I/O: `Reader`, `Writer`, `ReturnStatus`, `StepModelType` |
| `Render` | Tessellation: `extract_tessellation` |
| `ShapeRenderer` | Interactive 3D visualization widget |

## Usage Examples

### Geometric Primitives

```python
from mod3d import gp

# Points, vectors, directions
p = gp.Pnt(1.0, 2.0, 3.0)
v = gp.Vec(1.0, 0.0, 0.0)
d = gp.Dir(0.0, 0.0, 1.0)

# Transformations
trsf = gp.Trsf()
trsf.set_translation(gp.Vec(5.0, 0.0, 0.0))
```

### Creating Shapes

```python
from mod3d import BRepBuilderAPI, gp, Geom

# Primitives
box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
sphere = BRepBuilderAPI.MakeSphere(5.0).shape()
cylinder = BRepBuilderAPI.MakeCylinder(2.0, 15.0).shape()
cone = BRepBuilderAPI.MakeCone(10.0, 5.0, 20.0).shape()

# Edges and wires
p1, p2 = gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0)
edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
wire = BRepBuilderAPI.MakeWire(edge).wire()

# Faces from wires or surfaces
face = BRepBuilderAPI.MakeFace(wire).face()
```

### B-Spline Curves

```python
import numpy as np
from mod3d import gp, Geom

# Using gp.Pnt list
curve = Geom.BSplineCurve(
    poles=[gp.Pnt(0, 0, 0), gp.Pnt(1, 1, 0), gp.Pnt(2, 1, 0), gp.Pnt(3, 0, 0)],
    knots=[0.0, 1.0, 2.0],
    multiplicities=[3, 1, 3],
    degree=2,
)

# Using numpy arrays
curve = Geom.BSplineCurve(
    poles=np.array([[0, 0, 0], [10, 0, 0], [10, 10, 0], [10, 10, 10]]),
    weights=[1.0, 1.2, 0.8, 0.5],
    knots=[0, 1],
    multiplicities=[4, 4],
    degree=3,
)
```

### Boolean Operations

```python
import mod3d

sphere = mod3d.BRepBuilderAPI.MakeSphere(5.0).shape()
box = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()

# Move box to overlap
trsf = mod3d.gp.Trsf()
trsf.set_translation(mod3d.gp.Vec(-10.0, -10.0, -10.0))
box = box.moved(trsf)

# Intersection, subtraction, union, section
common = mod3d.BooleanOp.Common(sphere, box)
cut    = mod3d.BooleanOp.Cut(box, sphere)
fuse   = mod3d.BooleanOp.Fuse(box, sphere)
section = mod3d.BooleanOp.Section(box, sphere)

result = fuse.shape()

# History tracking
edges = fuse.section_edges()
fuse.has_modified()
fuse.has_generated()
fuse.has_deleted()
```

### Filleting

```python
from mod3d import BRepFillet

# 3D fillet on Boolean result edges
fillet_maker = BRepFillet.MakeFillet(result)
for edge in fuse.section_edges():
    fillet_maker.add(1.0, edge)
filleted = fillet_maker.shape()

# 2D fillet on a planar face
fillet2d = BRepFillet.MakeFillet2d(face)
fillet2d.add_fillet(vertex, 1.0)
fillet2d.build()
```

### Surface Filling

```python
from mod3d import BRepOffsetAPI, GeomAbs

fill_op = BRepOffsetAPI.MakeFilling()
fill_op.add(edge1, GeomAbs.Shape.C0, is_bound=True)
fill_op.add(edge2, GeomAbs.Shape.C0, is_bound=True)
fill_op.add(gp.Pnt(10, 5, 0))  # optional interior constraint
fill_op.build()
face = fill_op.shape()
```

### Sweeps (Pipe and PipeShell)

```python
from mod3d import BRepBuilderAPI, Geom, gp

# Simple pipe
pipe = BRepBuilderAPI.MakePipe(spine_wire, profile_wire)
pipe_shape = pipe.shape()

# Multi-profile pipe shell
pipe_shell = BRepBuilderAPI.MakePipeShell(spine_wire)
pipe_shell.add(profile1_wire)
pipe_shell.add(profile2_wire)
pipe_shell.build()
result = pipe_shell.shape()
```

### Curve Interpolation

```python
from mod3d import gp, GeomAPI

points = [gp.Pnt(0, 0, 0), gp.Pnt(1, 1, 0), gp.Pnt(2, 0, 0)]
interp = GeomAPI.Interpolate(points)
interp.perform()
curve = interp.curve
```

### Distance Computation

```python
from mod3d import BRepExtrema, BRepBuilderAPI, gp

box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()

dist = BRepExtrema.DistShapeShape(box1, box2)
print(dist.value)  # 10.0
```

### Global Properties

```python
from mod3d import BRepGProp, BRepBuilderAPI

box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
props = BRepGProp.BRepGProp.linear_properties(box)
print(props.centre_of_mass)  # (5.0, 5.0, 5.0)
```

### STEP I/O

```python
from mod3d import StepControl, BRepBuilderAPI

# Reading
reader = StepControl.Reader()
reader.read_file("model.step")
reader.transfer_roots()
shape = reader.one_shape

# Writing
writer = StepControl.Writer()
writer.transfer(shape, StepControl.StepModelType.AsIs)
writer.write("output.step")
```

### Tessellation

```python
from mod3d import Render, BRepBuilderAPI

box = BRepBuilderAPI.MakeBox(10.0, 20.0, 30.0).shape()
faces, edges = Render.extract_tessellation(box, linear_deflection=0.1)

# Each face: (triangles, vertices, normals, uvs) as numpy arrays
for triangles, vertices, normals, uvs in faces:
    print(f"{vertices.shape[0]} vertices, {triangles.shape[0]} triangles")
```

### Interactive Rendering

```python
from mod3d import ShapeRenderer

renderer = ShapeRenderer()
renderer.angle_deflection = 5
renderer.linear_deflection = 0.01
renderer.add_shape(shape)
renderer.render(background='lightgray')
```

## Running Tests

```bash
pytest tests/
```

## Notes

- `pybind11` must be available to CMake (install via `pip install pybind11` or system package).
- The build sets `CMAKE_POSITION_INDEPENDENT_CODE` so the module can be linked into larger Python extension packages.
- If you need to customize the OCCT search paths, pass `-DOpenCascade_DIR` or set `OPEN_CASCADE_DIR` before configuring.
- Optional [pygbs](https://github.com/ssg-aero/pygbs) integration is available when `MOD3D_USE_GBS=ON` (default) and the `Gbs` package is found.
