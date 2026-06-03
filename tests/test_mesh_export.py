"""Tests for the mesh export helpers (Render.export_stl/obj/gltf).

These wrap BRepMesh_IncrementalMesh + OCCT writers and work on every supported
OCCT version (not 8.0-specific), so they are not skipped.
"""
import os
import tempfile

import pytest

from mod3d import Render, BRepBuilderAPI


@pytest.fixture
def box():
    return BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()


def _written(path):
    return os.path.exists(path) and os.path.getsize(path) > 0


def test_export_stl_binary(box, tmp_path):
    out = str(tmp_path / "box.stl")
    Render.export_stl(box, out, deflection=0.5)
    assert _written(out)


def test_export_stl_ascii_has_solid_keyword(box, tmp_path):
    out = str(tmp_path / "box_ascii.stl")
    Render.export_stl(box, out, deflection=0.5, ascii=True)
    assert _written(out)
    with open(out, "r", errors="ignore") as f:
        head = f.read(256)
    assert "solid" in head


def test_export_obj(box, tmp_path):
    out = str(tmp_path / "box.obj")
    Render.export_obj(box, out, deflection=0.5)
    assert _written(out)


def test_export_gltf_text(box, tmp_path):
    out = str(tmp_path / "box.gltf")
    Render.export_gltf(box, out, deflection=0.5)
    assert _written(out)


def test_export_gltf_binary(box, tmp_path):
    out = str(tmp_path / "box.glb")
    Render.export_gltf(box, out, deflection=0.5, binary=True)
    assert _written(out)


def test_export_invalid_deflection_raises(box, tmp_path):
    out = str(tmp_path / "bad.stl")
    with pytest.raises((ValueError, RuntimeError)):
        Render.export_stl(box, out, deflection=0.0)
