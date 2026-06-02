"""Tests for the instancing-aware rendering path in mod3d.visualyse3js.

Periodic (and any TShape-sharing) shapes must be tessellated once and the
BufferGeometry reused across copies, with the per-copy placement applied as the
object's model matrix. This both cuts GPU memory (one upload instead of N) and
must reproduce exactly the world-space geometry the old "bake the transform"
path produced.
"""
from __future__ import annotations

import numpy as np
import pytest

from mod3d import BRepBuilderAPI, Render, gp
from mod3d.periodic import make_periodic_compound

# visualyse3js pulls in pythreejs/ipywidgets; skip cleanly if unavailable.
pythreejs = pytest.importorskip("pythreejs")
from mod3d.visualyse3js import occt_to_threejs  # noqa: E402


def _z_axis():
    return gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))


def _sphere(cx=40.0, r=8.0):
    return BRepBuilderAPI.MakeSphere(gp.Pnt(cx, 0.0, 0.0), r).shape()


def _as_list(mesh_face):
    if mesh_face is None:
        return []
    return mesh_face if isinstance(mesh_face, list) else [mesh_face]


def _apply_matrix(matrix16, vertices):
    """Apply a three.js column-major length-16 matrix to (n,3) local vertices."""
    e = np.asarray(matrix16, dtype=np.float64)
    m = e.reshape(4, 4).T  # column-major -> row-major 4x4
    homog = np.column_stack([vertices, np.ones(len(vertices))])
    return (homog @ m.T)[:, :3]


def _world_vertices_from_meshes(meshes):
    """Union of every mesh's local vertices pushed through its model matrix."""
    chunks = []
    for mesh in meshes:
        local = np.asarray(mesh.geometry.attributes["position"].array, dtype=np.float64)
        chunks.append(_apply_matrix(list(mesh.matrix), local))
    return np.vstack(chunks) if chunks else np.empty((0, 3))


def _world_vertices_from_extraction(shape, deflection, angle_deflection=20.0):
    """Reference: vertices the old path uploads (already in world coords)."""
    faces_data, _edges = Render.extract_tessellation(
        shape, deflection, False, angle_deflection, True, True)
    return np.vstack([v for _t, v, _n, _uv in faces_data]).astype(np.float64)


def _max_nearest_distance(a, b, chunk=512):
    """Largest distance from any point of `a` to its nearest point in `b`.

    Order-independent and robust to tiny floating-point noise (unlike sorting
    or grid-snapping, which can misalign points near cell boundaries).
    """
    worst = 0.0
    for start in range(0, len(a), chunk):
        block = a[start:start + chunk]
        d = np.linalg.norm(block[:, None, :] - b[None, :, :], axis=2)
        worst = max(worst, d.min(axis=1).max())
    return worst


def _same_cloud(a, b, atol=1e-4):
    return a.shape == b.shape and _max_nearest_distance(a, b) <= atol


def test_periodic_shares_single_geometry():
    """N periodic copies must yield N meshes referencing ONE BufferGeometry."""
    n = 24
    compound = make_periodic_compound(_sphere(), _z_axis(), n)
    mesh_face, _edges = occt_to_threejs(
        compound, linear_deflection=0.2, angle_deflection=20.0)

    meshes = _as_list(mesh_face)
    assert len(meshes) == n
    unique_geometries = {id(m.geometry) for m in meshes}
    assert len(unique_geometries) == 1, (
        f"expected 1 shared geometry, got {len(unique_geometries)}")


def test_periodic_vram_does_not_scale_with_n():
    """Total vertices resident in VRAM (summed over unique geometries) stays at
    the master's count regardless of N."""
    master_verts = _world_vertices_from_extraction(_sphere(), 0.2).shape[0]

    def vram_vertices(n):
        compound = make_periodic_compound(_sphere(), _z_axis(), n)
        mesh_face, _ = occt_to_threejs(
            compound, linear_deflection=0.2, angle_deflection=20.0)
        seen = {id(m.geometry): m.geometry.attributes["position"].array.shape[0]
                for m in _as_list(mesh_face)}
        return sum(seen.values())

    assert vram_vertices(1) == master_verts
    # 60 copies must not cost 60x the vertices.
    assert vram_vertices(60) == master_verts


def test_instanced_world_geometry_matches_baked_extraction():
    """The shared-geometry + per-mesh-matrix path must reproduce the exact
    world-space vertices the old baked-transform extraction produced."""
    n = 12
    deflection = 0.2
    compound = make_periodic_compound(_sphere(), _z_axis(), n)

    mesh_face, _ = occt_to_threejs(
        compound, linear_deflection=deflection, angle_deflection=20.0)
    world_new = _world_vertices_from_meshes(_as_list(mesh_face))
    world_ref = _world_vertices_from_extraction(compound, deflection)

    # Same point cloud (order may differ): sort then compare within tolerance.
    assert _same_cloud(world_new, world_ref)


def test_non_periodic_shape_unchanged():
    """A plain single solid still renders: one mesh, geometry in world coords."""
    box = BRepBuilderAPI.MakeBox(gp.Pnt(0, 0, 0), 5.0, 5.0, 5.0).shape()
    mesh_face, _ = occt_to_threejs(box, linear_deflection=0.5, angle_deflection=20.0)

    meshes = _as_list(mesh_face)
    assert len(meshes) == 1
    world_new = _world_vertices_from_meshes(meshes)
    world_ref = _world_vertices_from_extraction(box, 0.5)
    assert _same_cloud(world_new, world_ref)
