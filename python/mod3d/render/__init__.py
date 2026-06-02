"""Rendering backends for OCCT shapes.

- :mod:`mod3d.render._scene` — backend-neutral scene decomposition (the IR);
  importable without any rendering library.
- :mod:`mod3d.render.threejs` — pythreejs backend (notebook), exposing
  :class:`ShapeRenderer` and :func:`occt_to_threejs`.

A PyVista backend (desktop / notebook-trame / web) is added alongside; both
backends consume the same ``_scene`` IR so the instancing logic stays shared.
"""
from __future__ import annotations

from ._scene import (
    InstanceGroup,
    group_partners,
    shape_instance_groups,
    shape_occurrences,
    trsf_to_matrix4,
)
from .threejs import ShapeRenderer, occt_to_threejs

__all__ = [
    "InstanceGroup",
    "ShapeRenderer",
    "group_partners",
    "occt_to_threejs",
    "shape_instance_groups",
    "shape_occurrences",
    "trsf_to_matrix4",
]
