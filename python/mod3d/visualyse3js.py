"""Backward-compatibility shim.

The pythreejs renderer moved to :mod:`mod3d.render.threejs` when a second
backend (PyVista) was added and the backend-neutral scene decomposition was
factored into :mod:`mod3d.render._scene`. This module re-exports the public
names so existing imports (``from mod3d.visualyse3js import occt_to_threejs``)
keep working. New code should import from :mod:`mod3d.render`.
"""
from __future__ import annotations

from mod3d.render.threejs import (
    ShapeRenderer,
    axis_angle_quaternion,
    create_trihedron,
    create_trihedron_helper,
    edges_mesh,
    faces_mesh,
    make_axis_decoration,
    mesh_vertices,
    occt_to_threejs,
)

__all__ = [
    "ShapeRenderer",
    "axis_angle_quaternion",
    "create_trihedron",
    "create_trihedron_helper",
    "edges_mesh",
    "faces_mesh",
    "make_axis_decoration",
    "mesh_vertices",
    "occt_to_threejs",
]
