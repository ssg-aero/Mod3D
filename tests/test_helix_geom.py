"""Tests for the OCCT 8.0 HelixGeom binding (mod3d.HelixGeom.appr_helix).

The HelixGeom package is new in OCCT 8.0, so the submodule is absent on the
7.9.x default CI legs and these tests skip there. Exercised by the occt>=8
canary leg.
"""
import math

import pytest

import mod3d
from mod3d import gp

requires_helix = pytest.mark.skipif(
    not hasattr(mod3d, "HelixGeom"),
    reason="mod3d.HelixGeom requires OCCT >= 8.0",
)


@requires_helix
def test_appr_helix_cylindrical_one_turn():
    pitch = 2.0
    curve, max_error = mod3d.HelixGeom.appr_helix(
        t1=0.0, t2=2.0 * math.pi, pitch=pitch, r_start=1.0, tol=1e-4
    )
    assert curve is not None
    assert curve.degree >= 1
    assert max_error <= 1e-3

    # Default frame is the Z axis; one full turn advances z by exactly `pitch`.
    u1, u2 = curve.first_parameter, curve.last_parameter
    p0 = curve.value(u1)
    p1 = curve.value(u2)
    assert abs((p1.z - p0.z) - pitch) < 1e-2
    # Stays on the unit-radius cylinder.
    assert abs(math.hypot(p0.x, p0.y) - 1.0) < 1e-2


@requires_helix
def test_appr_helix_defaults_taper_and_orientation():
    # taper_angle and cw have defaults; a non-tapered CCW helix should build.
    curve, _ = mod3d.HelixGeom.appr_helix(
        t1=0.0, t2=math.pi, pitch=1.0, r_start=2.0
    )
    assert curve is not None


@requires_helix
def test_appr_helix_clockwise_flag():
    ccw, _ = mod3d.HelixGeom.appr_helix(0.0, math.pi, 1.0, 1.0, cw=False, tol=1e-4)
    cw, _ = mod3d.HelixGeom.appr_helix(0.0, math.pi, 1.0, 1.0, cw=True, tol=1e-4)
    # Opposite handedness => the y-sign at a quarter turn differs.
    q_ccw = ccw.value((ccw.first_parameter + ccw.last_parameter) / 2)
    q_cw = cw.value((cw.first_parameter + cw.last_parameter) / 2)
    assert q_ccw.y * q_cw.y <= 1e-9
