"""Tests for the PyVista rendering backend.

Mirrors tests/test_render_instancing.py (the pythreejs backend) on the same
guarantees: TShape-sharing shapes must be tessellated once and the geometry
reused across copies (one VTK mapper / one GPU upload for N actors), and the
per-actor user_matrix must reproduce exactly the world-space geometry the
"bake the transform" extraction produces.

The PyVista tests skip cleanly when pyvista is unavailable; the lazy-import
test does not need pyvista and runs everywhere.
"""
from __future__ import annotations

import subprocess
import sys

import numpy as np
import pytest

from mod3d import BRepBuilderAPI, Render, gp
from mod3d.periodic import make_periodic_compound


def _z_axis():
    return gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))


def _sphere(cx=40.0, r=8.0):
    return BRepBuilderAPI.MakeSphere(gp.Pnt(cx, 0.0, 0.0), r).shape()


def _max_nearest_distance(a, b, chunk=512):
    """Largest distance from any point of `a` to its nearest point in `b`.

    Order-independent and robust to tiny floating-point noise.
    """
    worst = 0.0
    for start in range(0, len(a), chunk):
        block = a[start:start + chunk]
        d = np.linalg.norm(block[:, None, :] - b[None, :, :], axis=2)
        worst = max(worst, d.min(axis=1).max())
    return worst


def _same_cloud(a, b, atol=1e-4):
    return a.shape == b.shape and _max_nearest_distance(a, b) <= atol


def _dataset_address(actor):
    """Stable identity of an actor's input dataset — one per real GPU upload.

    Uses the underlying VTK object's C++ address, which is deterministic and
    unique per object. Python ``id()`` of ``actor.mapper`` / ``.dataset`` is
    NOT safe here: pyvista hands back transient wrapper objects, so their ids
    can collide or split depending on GC/memory reuse (i.e. on test order).
    """
    return actor.mapper.GetInput().GetAddressAsString("")


def _actor_world_points(actor):
    """World-space points of an actor: its mapper's dataset through user_matrix."""
    poly = actor.mapper.dataset
    pts = np.asarray(poly.points, dtype=np.float64)
    matrix = actor.user_matrix
    m = np.eye(4) if matrix is None else np.asarray(matrix, dtype=np.float64)
    homog = np.column_stack([pts, np.ones(len(pts))])
    return (homog @ m.T)[:, :3]


def _world_vertices_from_extraction(shape, deflection, angle_deflection=20.0):
    """Reference: vertices the bake-the-transform path produces (world coords)."""
    faces_data, _edges = Render.extract_tessellation(
        shape, deflection, False, angle_deflection, True, True)
    return np.vstack([v for _t, v, _n, _uv in faces_data]).astype(np.float64)


def test_import_mod3d_does_not_pull_render_backends():
    """`import mod3d` must not import pythreejs or pyvista — backends are lazy.

    Run in a fresh interpreter so previously-imported modules don't mask it.
    """
    code = (
        "import sys, mod3d; "
        "assert 'pythreejs' not in sys.modules, 'pythreejs imported eagerly'; "
        "assert 'pyvista' not in sys.modules, 'pyvista imported eagerly'; "
        "print('ok')"
    )
    result = subprocess.run([sys.executable, "-c", code],
                            capture_output=True, text=True)
    assert result.returncode == 0, result.stderr
    assert "ok" in result.stdout


def test_pyvista_periodic_shares_single_mapper():
    """N periodic copies must yield N face actors sharing ONE mapper/dataset."""
    pytest.importorskip("pyvista")
    from mod3d.render.pyvista import occt_to_pyvista

    n = 24
    compound = make_periodic_compound(_sphere(), _z_axis(), n)
    mesh_actors, _edges = occt_to_pyvista(
        compound, linear_deflection=0.2, angle_deflection=20.0)

    assert len(mesh_actors) == n
    # Every actor must point at the SAME input dataset (one GPU upload). Use the
    # underlying VTK object address, not Python id() of the pyvista wrappers
    # (those are transient — their ids depend on GC/test order).
    assert len({_dataset_address(a) for a in mesh_actors}) == 1


def test_pyvista_vram_does_not_scale_with_n():
    """Points resident on the GPU (summed over unique datasets) stays at the
    master's count regardless of N."""
    pytest.importorskip("pyvista")
    from mod3d.render.pyvista import occt_to_pyvista

    master_pts = _world_vertices_from_extraction(_sphere(), 0.2).shape[0]

    def vram_points(n):
        compound = make_periodic_compound(_sphere(), _z_axis(), n)
        mesh_actors, _ = occt_to_pyvista(
            compound, linear_deflection=0.2, angle_deflection=20.0)
        # Key by the dataset's stable VTK address; summing per unique dataset
        # gives the points actually resident on the GPU.
        seen = {_dataset_address(a): a.mapper.dataset.n_points for a in mesh_actors}
        return sum(seen.values())

    assert vram_points(1) == master_pts
    assert vram_points(60) == master_pts


def test_pyvista_instanced_world_geometry_matches_baked_extraction():
    """The shared-geometry + per-actor user_matrix path must reproduce the exact
    world-space vertices the baked extraction produces."""
    pytest.importorskip("pyvista")
    from mod3d.render.pyvista import occt_to_pyvista

    n = 12
    deflection = 0.2
    compound = make_periodic_compound(_sphere(), _z_axis(), n)

    mesh_actors, _ = occt_to_pyvista(
        compound, linear_deflection=deflection, angle_deflection=20.0)
    world_new = np.vstack([_actor_world_points(a) for a in mesh_actors])
    world_ref = _world_vertices_from_extraction(compound, deflection)

    assert _same_cloud(world_new, world_ref)


def test_pyvista_non_periodic_shape_unchanged():
    """A plain single solid still renders as one actor in world coords."""
    pytest.importorskip("pyvista")
    from mod3d.render.pyvista import occt_to_pyvista

    box = BRepBuilderAPI.MakeBox(gp.Pnt(0, 0, 0), 5.0, 5.0, 5.0).shape()
    mesh_actors, _ = occt_to_pyvista(box, linear_deflection=0.5, angle_deflection=20.0)

    assert len(mesh_actors) == 1
    world_new = _actor_world_points(mesh_actors[0])
    world_ref = _world_vertices_from_extraction(box, 0.5)
    assert _same_cloud(world_new, world_ref)


def test_pyvista_export_html(tmp_path):
    """The web-sharing path writes a self-contained HTML file."""
    pytest.importorskip("pyvista")
    # export_html serialises the scene through trame's vtk.js bridge.
    pytest.importorskip("trame_vtk")
    import pyvista as pv
    pv.OFF_SCREEN = True
    from mod3d.render.pyvista import PyVistaRenderer

    compound = make_periodic_compound(_sphere(), _z_axis(), 6)
    renderer = PyVistaRenderer(linear_deflection=0.3)
    renderer.add_shape(compound)

    out = tmp_path / "scene.html"
    result = renderer.render(mode="html", path=str(out))
    assert result == str(out)
    assert out.exists() and out.stat().st_size > 0
