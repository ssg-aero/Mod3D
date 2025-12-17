"""Package shim that exposes the pybind11-built helpers."""

from __future__ import annotations

from .mod3d import create_box_summary

__all__ = ["create_box_summary"]
