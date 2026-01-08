"""Package shim that exposes the pybind11-built helpers."""

from __future__ import annotations

# from .mod3d import create_box_summary, create_box

# Re-export submodules for convenient access
from .mod3d import gp
from .mod3d import Geom
from .mod3d import GeomFill
from .mod3d import GeomAbs
from .mod3d import TopoDS
from .mod3d import TopAbs
from .mod3d import TopExp
from .mod3d import BRep
from .mod3d import BRepBuilderAPI
from .mod3d import BRepOffsetAPI
from .mod3d import BRepPrim
from .mod3d import BRepExtrema
from .mod3d import BRepFillet
from .mod3d import BooleanOp
from .mod3d import ShapeAnalysis
from .mod3d import StepControl
from .mod3d import StepData
from .mod3d import Render
from .visualyse3js import ShapeRenderer
__all__ = [
    "create_box_summary",
    "create_box",
    "gp",
    "Geom",
    "GeomFill",
    "GeomAbs",
    "TopoDS",
    "TopAbs",
    "TopExp",
    "BRep",
    "BRepBuilderAPI",
    "BRepOffsetAPI",
    "BRepPrim",
    "BRepExtrema",
    "BRepFillet",
    "BooleanOp",
    "ShapeAnalysis",
    "StepControl",
    "StepData",
    "Render",
    "ShapeRenderer",
]
