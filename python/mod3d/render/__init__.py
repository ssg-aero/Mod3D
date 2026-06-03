"""Rendering backends for OCCT shapes.

- :mod:`mod3d.render._scene` — backend-neutral scene decomposition (the IR);
  importable without any rendering library, and always eager here.
- :mod:`mod3d.render.threejs` — pythreejs backend (notebook): ``ShapeRenderer``,
  ``occt_to_threejs``.
- :mod:`mod3d.render.pyvista` — PyVista backend (desktop / notebook-trame /
  web): ``PyVistaRenderer``, ``occt_to_pyvista``.

The backends are imported lazily: pulling pythreejs or pyvista (both heavy,
optional) only happens when their renderer is actually accessed, so ``import
mod3d`` — and ``from mod3d.render import shape_instance_groups`` — need neither.
"""
from __future__ import annotations

import importlib

from ._scene import (
    InstanceGroup,
    group_partners,
    shape_instance_groups,
    shape_occurrences,
    trsf_to_matrix4,
)

# name -> submodule providing it (imported on first access)
_LAZY = {
    "ShapeRenderer": ".threejs",
    "occt_to_threejs": ".threejs",
    "PyVistaRenderer": ".pyvista",
    "occt_to_pyvista": ".pyvista",
}


def __getattr__(name):
    target = _LAZY.get(name)
    if target is None:
        raise AttributeError(f"module {__name__!r} has no attribute {name!r}")
    module = importlib.import_module(target, __name__)
    return getattr(module, name)


def __dir__():
    return sorted(list(globals()) + list(_LAZY))


__all__ = [
    "InstanceGroup",
    "PyVistaRenderer",
    "ShapeRenderer",
    "group_partners",
    "occt_to_pyvista",
    "occt_to_threejs",
    "shape_instance_groups",
    "shape_occurrences",
    "trsf_to_matrix4",
]
