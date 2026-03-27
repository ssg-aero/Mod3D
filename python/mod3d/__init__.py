"""Package shim that exposes the pybind11-built helpers."""

from __future__ import annotations

import sys as _sys
from . import mod3d as _ext

_submodule_names = [
    "gp", "Geom2d", "Geom", "GeomAPI", "GeomFill", "GeomAbs",
    "TopoDS", "Adaptor", "TopAbs", "TopExp", "BRep", "BRepBuilderAPI",
    "BRepOffsetAPI", "BRepPrim", "BRepExtrema", "BRepFillet", "BooleanOp",
    "BRepTools", "BRepGProp", "BRepLib", "ShapeAnalysis", "ShapeFix",
    "ShapeUpgrade", "StepControl", "StepData", "Render",
]

for _name in _submodule_names:
    _m = getattr(_ext, _name)
    _m.__name__ = f"mod3d.{_name}"
    _sys.modules.setdefault(f"mod3d.{_name}", _m)
    globals()[_name] = _m

del _sys, _ext, _m, _name

from .visualyse3js import ShapeRenderer

__all__ = _submodule_names + ["ShapeRenderer"]
