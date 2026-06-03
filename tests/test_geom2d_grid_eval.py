"""Test for the OCCT 8.0 Geom2dGridEval binding (2D batch grid evaluation).

New in OCCT 8.0 — the submodule is absent on the 7.9.x default CI legs and
this test skips there. Exercised by the occt>=8 canary leg.
"""
import math

import pytest

import mod3d
from mod3d import gp, Geom2d

requires_geom2dgrid = pytest.mark.skipif(
    not hasattr(mod3d, "Geom2dGridEval"),
    reason="Geom2dGridEval requires OCCT >= 8.0",
)


@requires_geom2dgrid
def test_grid_eval_curve2d_matches_value():
    curve = Geom2d.BSplineCurve2d(
        [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(3.0, 1.0)], [0.0, 1.0], [2, 2], 1
    )
    params = [0.0, 0.5, 1.0]
    pts = mod3d.Geom2dGridEval.Curve(curve).evaluate_grid(params)
    assert len(pts) == len(params)
    for t, p in zip(params, pts):
        ref = curve.value(t)
        assert math.hypot(p.x - ref.x, p.y - ref.y) < 1e-9
