"""Package shim that exposes the pybind11-built helpers."""

from __future__ import annotations

from .mod3d import create_box_summary, create_box

# Re-export submodules for convenient access
from .mod3d import gp
from .mod3d import Geom
from .mod3d import TopoDS
from .mod3d import BRepBuilderAPI

__all__ = [
    "create_box_summary",
    "create_box",
    "gp",
    "Geom",
    "TopoDS",
    "BRepBuilderAPI",
]
