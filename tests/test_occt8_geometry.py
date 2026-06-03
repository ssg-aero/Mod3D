"""Tests for the OCCT 8.0 geometry packages bound by mod3d:
GeomEval (analytic geometries), GeomBndLib (bounding boxes) and GeomHash
(geometry hashing / fuzzy equality).

All three are new in OCCT 8.0, so the submodules are absent on the 7.9.x
default CI legs and the tests skip there. Exercised by the occt>=8 canary leg.
"""
import math

import pytest

import mod3d
from mod3d import gp, Geom

requires_geomeval = pytest.mark.skipif(
    not hasattr(mod3d, "GeomEval"), reason="GeomEval requires OCCT >= 8.0"
)
requires_geombndlib = pytest.mark.skipif(
    not hasattr(mod3d, "GeomBndLib"), reason="GeomBndLib requires OCCT >= 8.0"
)
requires_geomhash = pytest.mark.skipif(
    not hasattr(mod3d, "GeomHash"), reason="GeomHash requires OCCT >= 8.0"
)


def _ellipsoid(a=3.0, b=2.0, c=1.0):
    ax3 = gp.Ax3(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    return mod3d.GeomEval.EllipsoidSurface(ax3, a, b, c)


def _segment(p0, p1):
    poles = [gp.Pnt(*p0), gp.Pnt(*p1)]
    return Geom.BSplineCurve(poles, [0.0, 1.0], [2, 2], 1)


# --------------------------------------------------------------------------- #
# GeomEval
# --------------------------------------------------------------------------- #
@requires_geomeval
def test_ellipsoid_semi_axes_get_set():
    ell = _ellipsoid(3.0, 2.0, 1.0)
    assert ell.semi_axis_a == pytest.approx(3.0)
    assert ell.semi_axis_b == pytest.approx(2.0)
    assert ell.semi_axis_c == pytest.approx(1.0)
    ell.semi_axis_a = 5.0
    assert ell.semi_axis_a == pytest.approx(5.0)


@requires_geomeval
def test_ellipsoid_inherits_surface_value():
    ell = _ellipsoid(3.0, 2.0, 1.0)
    # value() is inherited from the bound Geom.Surface base.
    p = ell.value(0.0, 0.0)
    assert isinstance(p, gp.Pnt)


@requires_geomeval
def test_circular_helix_accessors():
    ax2 = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    helix = mod3d.GeomEval.CircularHelixCurve(ax2, 2.0, 1.0)
    assert helix.radius == pytest.approx(2.0)
    assert helix.pitch == pytest.approx(1.0)
    # Inherited Geom.Curve.value(); at t=0 the point lies at radius from axis.
    p = helix.value(0.0)
    assert math.hypot(p.x, p.y) == pytest.approx(2.0, abs=1e-9)


# --------------------------------------------------------------------------- #
# GeomBndLib
# --------------------------------------------------------------------------- #
@requires_geombndlib
def test_bndlib_curve_box():
    curve = _segment((0.0, 0.0, 0.0), (1.0, 2.0, 3.0))
    box = mod3d.GeomBndLib.Curve(curve).box(1e-7)
    assert box is not None
    xmin, ymin, zmin, xmax, ymax, zmax = box
    assert xmin == pytest.approx(0.0, abs=1e-6)
    assert xmax == pytest.approx(1.0, abs=1e-6)
    assert ymax == pytest.approx(2.0, abs=1e-6)
    assert zmax == pytest.approx(3.0, abs=1e-6)


@requires_geombndlib
def test_bndlib_surface_box_brackets_ellipsoid():
    ell = _ellipsoid(3.0, 2.0, 1.0)
    box = mod3d.GeomBndLib.Surface(ell).box_optimal(1e-6)
    assert box is not None
    xmin, ymin, zmin, xmax, ymax, zmax = box
    # An ellipsoid with semi-axes (3,2,1) is bracketed by [-3,3]x[-2,2]x[-1,1].
    assert xmax == pytest.approx(3.0, abs=1e-2)
    assert ymax == pytest.approx(2.0, abs=1e-2)
    assert zmax == pytest.approx(1.0, abs=1e-2)


# --------------------------------------------------------------------------- #
# GeomHash
# --------------------------------------------------------------------------- #
@requires_geomhash
def test_surface_hasher_equal_and_hash():
    hasher = mod3d.GeomHash.SurfaceHasher()
    e1 = _ellipsoid(3.0, 2.0, 1.0)
    e2 = _ellipsoid(3.0, 2.0, 1.0)
    e3 = _ellipsoid(3.0, 2.0, 1.5)
    assert hasher.equal(e1, e2)
    assert hasher.hash(e1) == hasher.hash(e2)
    assert not hasher.equal(e1, e3)


@requires_geomhash
def test_curve_hasher_runs():
    hasher = mod3d.GeomHash.CurveHasher()
    c1 = _segment((0.0, 0.0, 0.0), (1.0, 0.0, 0.0))
    c2 = _segment((0.0, 0.0, 0.0), (1.0, 0.0, 0.0))
    assert hasher.equal(c1, c2)
    assert isinstance(hasher.hash(c1), int)
