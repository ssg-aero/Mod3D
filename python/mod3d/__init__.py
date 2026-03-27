"""Package shim that exposes the pybind11-built helpers."""

from __future__ import annotations

# Re-export submodules for convenient access
from .mod3d import gp
from .mod3d import Geom2d
from .mod3d import Geom
from .mod3d import GeomAPI
from .mod3d import GeomFill
from .mod3d import GeomAbs
from .mod3d import TopoDS
from .mod3d import Adaptor
from .mod3d import TopAbs
from .mod3d import TopExp
from .mod3d import BRep
from .mod3d import BRepBuilderAPI
from .mod3d import BRepOffsetAPI
from .mod3d import BRepPrim
from .mod3d import BRepExtrema
from .mod3d import BRepFillet
from .mod3d import BooleanOp
from .mod3d import BRepTools
from .mod3d import BRepGProp
from .mod3d import BRepLib
from .mod3d import ShapeAnalysis
from .mod3d import ShapeFix
from .mod3d import ShapeUpgrade
from .mod3d import StepControl
from .mod3d import StepData
from .mod3d import Render
from .visualyse3js import ShapeRenderer

import sys as _sys

_submodules = [
    gp, Geom2d, Geom, GeomAPI, GeomFill, GeomAbs, TopoDS, Adaptor,
    TopAbs, TopExp, BRep, BRepBuilderAPI, BRepOffsetAPI, BRepPrim,
    BRepExtrema, BRepFillet, BooleanOp, BRepTools, BRepGProp, BRepLib,
    ShapeAnalysis, ShapeFix, ShapeUpgrade, StepControl, StepData, Render,
]
for _m in _submodules:
    _short_name = 'mod3d.' + _m.__name__.split('.')[-1]
    _m.__name__ = _short_name
    _sys.modules.setdefault(_short_name, _m)
del _sys, _submodules, _m, _short_name

__all__ = [
    "create_box_summary",
    "create_box",
    "gp",
    "Geom",
    "GeomAPI",
    "Geom2d",
    "GeomFill",
    "GeomAbs",
    "TopoDS",
    "Adaptor",
    "TopAbs",
    "TopExp",
    "BRep",
    "BRepBuilderAPI",
    "BRepOffsetAPI",
    "BRepPrim",
    "BRepExtrema",
    "BRepFillet",
    "BooleanOp",
    "BRepTools",
    "BRepGProp",
    "BRepLib",
    "ShapeAnalysis",
    "ShapeFix",
    "ShapeUpgrade",
    "StepControl",
    "StepData",
    "Render",
    "ShapeRenderer",
]

