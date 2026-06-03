"""Tests for the OCCT 8.0 ExtremaPC binding (point-to-curve extrema).

New in OCCT 8.0, so the submodule is absent on the 7.9.x default CI legs and
these tests skip there. Exercised by the occt>=8 canary leg.
"""
import math

import pytest

import mod3d
from mod3d import gp, Geom

requires_extrema_pc = pytest.mark.skipif(
    not hasattr(mod3d, "ExtremaPC"), reason="ExtremaPC requires OCCT >= 8.0"
)


def _segment(p0, p1):
    poles = [gp.Pnt(*p0), gp.Pnt(*p1)]
    return Geom.BSplineCurve(poles, [0.0, 1.0], [2, 2], 1)


@requires_extrema_pc
def test_extrema_point_to_segment():
    # Segment along X from (0,0,0) to (10,0,0); query point above its middle.
    curve = _segment((0.0, 0.0, 0.0), (10.0, 0.0, 0.0))
    ext = mod3d.ExtremaPC.Curve(curve, 0.0, 1.0)
    assert ext.is_initialized()

    res = ext.perform(gp.Pnt(5.0, 3.0, 0.0), tol=1e-9, include_endpoints=True)
    assert res["status"] == mod3d.ExtremaPC.Status.OK
    assert res["is_done"]
    assert res["min_distance"] is not None
    # Closest point on the segment is its midpoint, at distance 3.
    assert res["min_distance"] == pytest.approx(3.0, abs=1e-6)

    closest = res["extrema"][res["min_index"]]
    assert closest["point"].x == pytest.approx(5.0, abs=1e-6)
    assert closest["is_minimum"]


@requires_extrema_pc
def test_extrema_min_search_mode():
    curve = _segment((0.0, 0.0, 0.0), (10.0, 0.0, 0.0))
    ext = mod3d.ExtremaPC.Curve(curve, 0.0, 1.0)
    res = ext.perform(
        gp.Pnt(5.0, 3.0, 0.0), mode=mod3d.ExtremaPC.SearchMode.Min
    )
    assert res["min_distance"] == pytest.approx(3.0, abs=1e-6)
