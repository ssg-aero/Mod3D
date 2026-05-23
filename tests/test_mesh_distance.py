"""Tests for MeshDistance: BVH-accelerated approximate min-distance query."""

import math
import random
import time

import pytest

from mod3d import BRepBuilderAPI, BRepExtrema, TopoDS, gp


def _box(x, y, z, dx, dy, dz):
    origin = gp.Pnt(x, y, z)
    return BRepBuilderAPI.MakeBox(origin, dx, dy, dz).shape()


def test_two_boxes_gap():
    """Two axis-aligned boxes separated by a 10mm gap on X."""
    a = _box(0.0, 0.0, 0.0, 10.0, 10.0, 10.0)
    b = _box(20.0, 0.0, 0.0, 10.0, 10.0, 10.0)

    mq = BRepExtrema.MeshDistance(a, deflection=0.1)
    distance, _p_ref, _p_shape = mq.distance_to(b)
    assert distance == pytest.approx(10.0, abs=1e-6)


def test_consistency_vs_dist_shape_shape():
    """On planar boxes, the mesh BVH and the exact NURBS extrema agree."""
    a = _box(0.0, 0.0, 0.0, 5.0, 5.0, 5.0)
    b = _box(15.0, 1.0, 2.0, 5.0, 5.0, 5.0)

    mq = BRepExtrema.MeshDistance(a, deflection=0.05)
    mesh_distance, _, _ = mq.distance_to(b)

    exact = BRepExtrema.DistShapeShape(a, b)
    assert exact.is_done
    assert mesh_distance == pytest.approx(exact.value, abs=1e-4)


def test_reference_can_be_replaced():
    a = _box(0.0, 0.0, 0.0, 5.0, 5.0, 5.0)
    a_far = _box(0.0, 0.0, 0.0, 1.0, 1.0, 1.0)
    b = _box(10.0, 0.0, 0.0, 5.0, 5.0, 5.0)

    mq = BRepExtrema.MeshDistance(a, deflection=0.1)
    d1, _, _ = mq.distance_to(b)
    assert d1 == pytest.approx(5.0, abs=1e-6)

    mq.set_reference(a_far, deflection=0.1)
    d2, _, _ = mq.distance_to(b)
    assert d2 == pytest.approx(9.0, abs=1e-6)


def _sphere(cx, cy, cz, r):
    return BRepBuilderAPI.MakeSphere(gp.Pnt(cx, cy, cz), r).shape()


def test_bench_sphere_vs_pipe():
    """Bench on a curved reference (sphere) — where DistShapeShape pays the
    cost of parametric extrema on a NURBS surface and MeshDistance amortizes."""
    reference = _sphere(0.0, 0.0, 0.0, 50.0)

    # Cheap "pipe-like" probe shape moving along X.
    def build_pipe(x_offset: float):
        return _box(80.0 + x_offset, 0.0, 0.0, 5.0, 5.0, 30.0)

    n_iterations = 100
    offsets = [0.1 * i for i in range(n_iterations)]

    deflection = 0.2

    mq = BRepExtrema.MeshDistance(reference, deflection=deflection)
    t0 = time.perf_counter()
    mesh_values = []
    for off in offsets:
        pipe = build_pipe(off)
        d, _, _ = mq.distance_to(pipe)
        mesh_values.append(d)
    mesh_elapsed = time.perf_counter() - t0

    t0 = time.perf_counter()
    exact_values = []
    for off in offsets:
        pipe = build_pipe(off)
        ext = BRepExtrema.DistShapeShape(reference, pipe)
        exact_values.append(ext.value)
    exact_elapsed = time.perf_counter() - t0

    # Cohérence à la déflection près : la sphère meshée perd un peu de matière,
    # donc la distance approximée peut être un peu plus grande que la distance exacte.
    for mesh_d, exact_d in zip(mesh_values, exact_values):
        assert mesh_d >= exact_d - 1e-6
        assert mesh_d - exact_d <= deflection * 2  # raisonnable

    print(f"\nReference: sphere R=50  query: box ~30mm, x{n_iterations} iterations, deflection={deflection}")
    print(f"  MeshDistance:    {mesh_elapsed*1000:7.1f} ms total  ({mesh_elapsed/n_iterations*1000:.2f} ms/call)")
    print(f"  DistShapeShape:  {exact_elapsed*1000:7.1f} ms total  ({exact_elapsed/n_iterations*1000:.2f} ms/call)")
    print(f"  speedup:         ~{exact_elapsed / max(mesh_elapsed, 1e-9):.1f}x")


def _cylinder(cx, cy, cz, r, h):
    axis = gp.Ax2(gp.Pnt(cx, cy, cz), gp.Dir(0, 0, 1))
    return BRepBuilderAPI.MakeCylinder(axis, r, h).shape()


def _scattered_spheres_compound(n: int, half_extent: float, radius: float, seed: int = 42):
    """Builds a Compound of `n` small spheres uniformly placed in a cube of
    side 2*half_extent. Designed so the nearest sphere to the query is the only
    one that matters — the BVH should reject the others on their bounding boxes."""
    rng = random.Random(seed)
    spheres = []
    for _ in range(n):
        x = rng.uniform(-half_extent, half_extent)
        y = rng.uniform(-half_extent, half_extent)
        z = rng.uniform(-half_extent, half_extent)
        spheres.append(BRepBuilderAPI.MakeSphere(gp.Pnt(x, y, z), radius).shape())
    return TopoDS.Compound(spheres)


def test_query_cache_amortises_tessellation():
    """When the same query TopoDS_Shape is queried repeatedly, the cache must
    short-circuit BRepMesh_IncrementalMesh — the dominant per-call cost.

    Bench: 200 calls of the same query shape should be substantially faster
    than 200 calls each rebuilding the query shape, even though the BVH descent
    work is identical."""
    reference = _sphere(0.0, 0.0, 0.0, 50.0)
    mq = BRepExtrema.MeshDistance(reference, deflection=0.2)

    iterations = 200

    # Warmup
    _ = mq.distance_to(_box(80.0, 0.0, 0.0, 5.0, 5.0, 30.0))

    # Fresh shape every call: no cache hit possible
    t0 = time.perf_counter()
    for _ in range(iterations):
        q = _box(80.0, 0.0, 0.0, 5.0, 5.0, 30.0)
        mq.distance_to(q)
    t_fresh = time.perf_counter() - t0

    # Same shape reused: cache should hit every time after the first call
    q = _box(80.0, 0.0, 0.0, 5.0, 5.0, 30.0)
    mq.clear_query_cache()
    _ = mq.distance_to(q)  # primes the cache
    cached_after_first = mq.nb_cached_queries
    t0 = time.perf_counter()
    for _ in range(iterations):
        mq.distance_to(q)
    t_reused = time.perf_counter() - t0

    assert cached_after_first == 1
    # The reused regime should be at least 5x faster — usually >20x.
    assert t_reused < t_fresh / 5, (
        f"Cache speedup is weak: fresh={t_fresh*1000:.1f} ms, "
        f"reused={t_reused*1000:.1f} ms (ratio {t_fresh/max(t_reused, 1e-9):.1f}x)")
    print(f"\nCache speedup: fresh={t_fresh*1000:.1f} ms vs reused={t_reused*1000:.1f} ms "
          f"(~{t_fresh / max(t_reused, 1e-9):.1f}x)")


def test_query_cache_invalidated_on_set_reference():
    a = _sphere(0.0, 0.0, 0.0, 50.0)
    a_far = _sphere(0.0, 0.0, 200.0, 50.0)
    q = _box(80.0, 0.0, 0.0, 5.0, 5.0, 30.0)

    mq = BRepExtrema.MeshDistance(a, deflection=0.2)
    _ = mq.distance_to(q)
    assert mq.nb_cached_queries == 1

    # Changing the reference may change the deflection / triangulation regime,
    # so the query memo is dropped.
    mq.set_reference(a_far, deflection=0.5)
    assert mq.nb_cached_queries == 0


def test_bvh_culling_scaling():
    """Cylinder query against compounds of 1, 10, 50, 250 spheres.

    If the BVH culling on the reference is effective, the per-call cost should
    grow much slower than N (ideally O(log N) per query vertex). A scaling
    factor < 5x between N=1 and N=250 is a strong signal that culling works.
    """
    query = _cylinder(120.0, 0.0, 0.0, 5.0, 30.0)
    deflection = 0.3
    iterations = 20  # repeat to get stable timings

    results = []
    counts = [1, 10, 50, 250]
    for n in counts:
        reference = _scattered_spheres_compound(n, half_extent=100.0, radius=3.0)
        mq = BRepExtrema.MeshDistance(reference, deflection=deflection)
        nb_ref_tris = mq.nb_reference_triangles

        # warmup
        _ = mq.distance_to(query)

        t0 = time.perf_counter()
        for _ in range(iterations):
            d, _, _ = mq.distance_to(query)
        elapsed = (time.perf_counter() - t0) / iterations
        results.append((n, nb_ref_tris, elapsed, d))

    print(f"\nBVH culling scaling — cylinder query vs compound-of-N-spheres "
          f"(deflection={deflection}, {iterations} iter):")
    base_time = results[0][2]
    for n, tris, elapsed, dist in results:
        print(f"  N={n:>3d}  refTris={tris:>5d}  per-call={elapsed*1000:7.2f} ms  "
              f"x{elapsed/max(base_time, 1e-12):5.2f}  min_dist={dist:.3f}")

    n_max, _, t_max, _ = results[-1]
    n_min, _, t_min, _ = results[0]
    growth = t_max / t_min
    linear_growth_if_no_culling = n_max / n_min  # 250x

    # The BVH should clip growth dramatically below the linear-scan baseline.
    # We assert a generous bound: at least 10x better than the no-culling case.
    assert growth < linear_growth_if_no_culling / 10, (
        f"BVH culling looks weak: {growth:.1f}x growth for {n_max/n_min:.0f}x more "
        f"reference triangles. Expected <{linear_growth_if_no_culling/10:.0f}x.")
