"""Tests for mod3d.periodic.make_periodic_compound.

Validates the shared-triangulation trick: building a periodic compound from a
master sector via TopLoc_Location should let BRepMesh_IncrementalMesh mesh the
master once and reuse the result across all rotated copies.
"""
from __future__ import annotations

import math
import time

import pytest

from mod3d import BRepBuilderAPI, BRepExtrema, TopAbs, TopoDS, gp
from mod3d.periodic import make_periodic_compound


def _z_axis():
    return gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))


def _master_sector():
    """A small box offset from the axis — stands in for a single blade."""
    return BRepBuilderAPI.MakeBox(gp.Pnt(40.0, -5.0, 0.0), 10.0, 10.0, 30.0).shape()


def test_periodic_compound_has_n_copies():
    """The compound advertises N rotated sub-shapes (solids)."""
    from mod3d import TopExp
    master = _master_sector()
    n = 24
    compound = make_periodic_compound(master, _z_axis(), n)

    count = sum(1 for _ in TopExp.Explorer(compound, TopAbs.SOLID))
    assert count == n


def test_periodic_distance_matches_exact_extrema():
    """MeshDistance on a periodic compound agrees with the exact NURBS
    DistShapeShape to within the tessellation deflection."""
    master = _master_sector()
    blade_row = make_periodic_compound(master, _z_axis(), 12)
    probe = BRepBuilderAPI.MakeBox(gp.Pnt(0.0, 0.0, 10.0), 5.0, 5.0, 5.0).shape()

    deflection = 0.2
    mq = BRepExtrema.MeshDistance(blade_row, deflection=deflection)
    d_mesh, _, _ = mq.distance_to(probe)

    exact = BRepExtrema.DistShapeShape(blade_row, probe)
    assert exact.is_done

    # Mesh approximation should never under-estimate the true distance by
    # much (mesh sits "inside" the surface). Allow up to 2x deflection.
    assert d_mesh >= exact.value - 1e-6
    assert d_mesh - exact.value <= 2 * deflection


def test_periodic_mesh_time_does_not_scale_with_n():
    """Building the BVH on a periodic compound of N copies should NOT take
    N× the time of a single copy — the master triangulation is computed
    once and reused. Allow generous slack for BVH construction overhead."""
    master = BRepBuilderAPI.MakeSphere(gp.Pnt(40.0, 0.0, 0.0), 5.0).shape()
    axis = _z_axis()

    def time_build(n_sectors, deflection):
        compound = make_periodic_compound(master, axis, n_sectors)
        t0 = time.perf_counter()
        mq = BRepExtrema.MeshDistance(compound, deflection=deflection)
        elapsed = time.perf_counter() - t0
        return elapsed, mq.nb_reference_triangles

    t1, tri1 = time_build(1, 0.2)
    tN, triN = time_build(60, 0.2)

    # The BVH still contains N times the triangles (locations differ), but
    # the *tessellation* work is paid once. Total construction should
    # therefore scale much slower than N — typically <5x for N=60.
    ratio = tN / max(t1, 1e-9)
    triangle_ratio = triN / max(tri1, 1)

    print(f"\nMesh time ratio N=60 / N=1: {ratio:.1f}x  "
          f"(triangle count ratio: {triangle_ratio:.1f}x)")
    # Strong shared-tessellation signal: time grows much less than triangle count.
    assert ratio < triangle_ratio / 4, (
        f"Periodic mesh time scaled too aggressively: {ratio:.1f}x for "
        f"{triangle_ratio:.0f}x more triangles — TShape sharing may not be working.")


def test_endoscope_vs_blade_row_workflow():
    """End-to-end smoke test mimicking the target workflow: one blade defines
    a periodic compound; a 'pipe' (endoscope stand-in) is queried against it,
    its shape changing each iteration."""
    blade = _master_sector()
    blade_row = make_periodic_compound(blade, _z_axis(), 24)

    mq = BRepExtrema.MeshDistance(blade_row, deflection=0.5)

    distances = []
    for i in range(10):
        # Probe shape moves outward — simulates an evolving endoscope
        z_offset = 5.0 + i * 0.5
        endoscope = BRepBuilderAPI.MakeBox(
            gp.Pnt(20.0, 0.0, z_offset), 3.0, 3.0, 8.0).shape()
        d, _, _ = mq.distance_to(endoscope)
        distances.append(d)

    # Distances should all be finite and positive (probe doesn't intersect blades)
    assert all(d > 0 for d in distances)
