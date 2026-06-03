"""Tests for the OCCT 8.0 Gordon surface bindings (GeomFill.Gordon and the
GeomFill.gordon_surface helper).

The Gordon surface API does not exist before OCCT 8.0, so the whole module is
skipped when running against an older OCCT (e.g. the 7.9.x default CI legs).
It is exercised by the ``occt>=8`` canary leg.
"""
import pytest

from mod3d import gp, Geom, GeomFill

requires_gordon = pytest.mark.skipif(
    not hasattr(GeomFill, "Gordon"),
    reason="GeomFill.Gordon requires OCCT >= 8.0",
)


def _segment(p0, p1):
    """A degree-1 (non-rational, bounded) B-spline between two points."""
    poles = [gp.Pnt(*p0), gp.Pnt(*p1)]
    knots = [0.0, 1.0]
    mults = [2, 2]
    return Geom.BSplineCurve(poles, knots, mults, 1)


def _unit_square_network():
    """A 2x2 network spanning the unit square in the z=0 plane.

    guides run along X (U-direction), profiles run along Y (V-direction);
    every profile meets every guide at a corner.
    """
    guides = [
        _segment((0.0, 0.0, 0.0), (1.0, 0.0, 0.0)),  # y = 0
        _segment((0.0, 1.0, 0.0), (1.0, 1.0, 0.0)),  # y = 1
    ]
    profiles = [
        _segment((0.0, 0.0, 0.0), (0.0, 1.0, 0.0)),  # x = 0
        _segment((1.0, 0.0, 0.0), (1.0, 1.0, 0.0)),  # x = 1
    ]
    return profiles, guides


def _corners(surf):
    """The four parametric-corner points of a surface."""
    u1, u2, v1, v2 = surf.bounds()
    return [surf.value(u, v) for u in (u1, u2) for v in (v1, v2)]


@requires_gordon
def test_gordon_class_builds_unit_square():
    profiles, guides = _unit_square_network()

    g = GeomFill.Gordon()
    g.init(profiles, guides, 1e-7)
    g.perform()

    assert g.is_done()
    surf = g.surface()
    assert surf is not None

    # The patch corners should coincide with the four unit-square corners.
    expected = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(0.0, 1.0, 0.0),
        gp.Pnt(1.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
    ]
    for pt in _corners(surf):
        assert min(pt.distance(e) for e in expected) < 1e-6


@requires_gordon
def test_gordon_surface_helper_returns_bspline():
    profiles, guides = _unit_square_network()

    surf = GeomFill.gordon_surface(profiles, guides, 1e-7)
    assert surf is not None
    assert surf.u_degree >= 1
    assert surf.v_degree >= 1


@requires_gordon
def test_gordon_surface_parallel_flag():
    profiles, guides = _unit_square_network()
    surf = GeomFill.gordon_surface(profiles, guides, 1e-7, parallel=True)
    assert surf is not None


@requires_gordon
def test_gordon_requires_at_least_two_curves():
    profiles, guides = _unit_square_network()
    with pytest.raises((ValueError, RuntimeError)):
        GeomFill.gordon_surface(profiles[:1], guides, 1e-7)


@requires_gordon
def test_gordon_incompatible_network_raises():
    # Profiles and guides that do not intersect within tolerance (guides are
    # shifted 10 units away in z), so no valid network exists.
    profiles = [
        _segment((0.0, 0.0, 0.0), (0.0, 1.0, 0.0)),
        _segment((1.0, 0.0, 0.0), (1.0, 1.0, 0.0)),
    ]
    guides = [
        _segment((0.0, 0.0, 10.0), (1.0, 0.0, 10.0)),
        _segment((0.0, 1.0, 10.0), (1.0, 1.0, 10.0)),
    ]
    with pytest.raises(RuntimeError):
        GeomFill.gordon_surface(profiles, guides, 1e-7)
