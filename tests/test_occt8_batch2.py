"""Tests for the second batch of OCCT 8.0 bindings:
Geom2dEval (analytic 2D curves), GeomGridEval (batch grid evaluation),
HelixBRep (helix wires) and StepTidy (STEP duplicate cleanup).

All new in OCCT 8.0 — the submodules are absent on the 7.9.x default CI legs
and these tests skip there. Exercised by the occt>=8 canary leg.
"""
import math

import pytest

import mod3d
from mod3d import gp, Geom, GeomAbs

requires_geom2deval = pytest.mark.skipif(
    not hasattr(mod3d, "Geom2dEval"), reason="Geom2dEval requires OCCT >= 8.0"
)
requires_gridevel = pytest.mark.skipif(
    not hasattr(mod3d, "GeomGridEval"), reason="GeomGridEval requires OCCT >= 8.0"
)
requires_helixbrep = pytest.mark.skipif(
    not hasattr(mod3d, "HelixBRep"), reason="HelixBRep requires OCCT >= 8.0"
)
requires_steptidy = pytest.mark.skipif(
    not hasattr(mod3d, "StepTidy"), reason="StepTidy requires OCCT >= 8.0"
)


def _ax2d_origin():
    return gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))


# --------------------------------------------------------------------------- #
# Geom2dEval
# --------------------------------------------------------------------------- #
@requires_geom2deval
def test_archimedean_spiral_accessors_and_value():
    spiral = mod3d.Geom2dEval.ArchimedeanSpiralCurve(_ax2d_origin(), 1.0, 0.5)
    assert spiral.initial_radius == pytest.approx(1.0)
    assert spiral.growth_rate == pytest.approx(0.5)
    # value() inherited from Geom2d.Curve2d; at t=0, r = initial_radius.
    p0 = spiral.value(0.0)
    assert math.hypot(p0.x, p0.y) == pytest.approx(1.0, abs=1e-9)


@requires_geom2deval
def test_sine_wave_accessors():
    wave = mod3d.Geom2dEval.SineWaveCurve(_ax2d_origin(), 2.0, 3.0)
    assert wave.amplitude == pytest.approx(2.0)
    assert wave.omega == pytest.approx(3.0)
    assert wave.phase == pytest.approx(0.0)


@requires_geom2deval
def test_circle_involute_accessors():
    inv = mod3d.Geom2dEval.CircleInvoluteCurve(_ax2d_origin(), 1.5)
    assert inv.radius == pytest.approx(1.5)


# --------------------------------------------------------------------------- #
# GeomGridEval
# --------------------------------------------------------------------------- #
@requires_gridevel
def test_grid_eval_surface_shape_and_values():
    ax3 = gp.Ax3(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    plane = Geom.Plane(ax3)
    u = [0.0, 1.0, 2.0]
    v = [0.0, 1.0]
    grid = mod3d.GeomGridEval.Surface(plane).evaluate_grid(u, v)
    assert len(grid) == len(u)
    assert all(len(row) == len(v) for row in grid)
    # Grid points must match direct surface evaluation.
    for i, uu in enumerate(u):
        for j, vv in enumerate(v):
            ref = plane.value(uu, vv)
            assert grid[i][j].distance(ref) < 1e-9


@requires_gridevel
def test_grid_eval_curve_matches_value():
    poles = [gp.Pnt(0.0, 0.0, 0.0), gp.Pnt(3.0, 0.0, 0.0)]
    curve = Geom.BSplineCurve(poles, [0.0, 1.0], [2, 2], 1)
    params = [0.0, 0.5, 1.0]
    pts = mod3d.GeomGridEval.Curve(curve).evaluate_grid(params)
    assert len(pts) == len(params)
    for t, p in zip(params, pts):
        assert p.distance(curve.value(t)) < 1e-9


# --------------------------------------------------------------------------- #
# HelixBRep
# --------------------------------------------------------------------------- #
@requires_helixbrep
def test_helix_brep_pure_helix_builds_wire():
    axis = gp.Ax3(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    builder = mod3d.HelixBRep.BuilderHelix()
    builder.set_pure_helix(axis, diameter=10.0, pitches=[2.0], nb_turns=[3.0])
    builder.set_approx_parameters(1e-3, 8, GeomAbs.Shape.C2)
    builder.perform()
    assert builder.error_status() == 0
    shape = builder.shape()
    assert shape is not None
    assert not shape.is_null()


# --------------------------------------------------------------------------- #
# StepTidy
# --------------------------------------------------------------------------- #
@requires_steptidy
def test_step_tidy_remove_duplicates_runs():
    from mod3d import StepControl, BRepBuilderAPI

    box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    writer = StepControl.Writer()
    writer.transfer(box, StepControl.StepModelType.AsIs)
    # Should run without error on a populated work session.
    mod3d.StepTidy.remove_duplicates(writer.work_session)


@requires_steptidy
def test_step_tidy_rejects_none():
    with pytest.raises((ValueError, TypeError)):
        mod3d.StepTidy.remove_duplicates(None)
