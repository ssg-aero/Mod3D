"""Tests for ShapeAnalysis module bindings."""

import math

import pytest

from mod3d import Geom, ShapeAnalysis, gp


class TestShapeAnalysisSurface:
    """Tests for ShapeAnalysis.Surface."""

    def make_plane(self):
        return Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))

    def make_cylinder(self, radius=5.0):
        ax3 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
        return Geom.CylindricalSurface(ax3, radius)

    def make_sphere(self, radius=3.0):
        ax3 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
        return Geom.SphericalSurface(ax3, radius)

    def test_constructor_and_surface(self):
        surf = self.make_plane()
        sas = ShapeAnalysis.Surface(surf)
        assert sas.surface is not None

    def test_init_reload(self):
        sas = ShapeAnalysis.Surface(self.make_plane())
        cyl = self.make_cylinder()
        sas.init(cyl)
        assert sas.surface is not None

    def test_value_and_value_at(self):
        cyl = self.make_cylinder(radius=5.0)
        sas = ShapeAnalysis.Surface(cyl)
        # On a cylinder centered at origin around Z: at u=0,v=0 -> (R, 0, 0)
        p = sas.value(0.0, 0.0)
        assert p.x == pytest.approx(5.0)
        assert p.y == pytest.approx(0.0)
        assert p.z == pytest.approx(0.0)

        p2 = sas.value_at(gp.Pnt2d(math.pi / 2, 1.0))
        assert p2.x == pytest.approx(0.0, abs=1e-9)
        assert p2.y == pytest.approx(5.0)
        assert p2.z == pytest.approx(1.0)

    def test_closure_cylinder_is_u_closed(self):
        sas = ShapeAnalysis.Surface(self.make_cylinder())
        # Cylindrical surfaces are periodic in U (the angular direction).
        assert sas.is_u_closed() is True
        # And not closed in V (the axial direction is infinite/open).
        assert sas.is_v_closed() is False

    def test_closure_plane_neither(self):
        sas = ShapeAnalysis.Surface(self.make_plane())
        assert sas.is_u_closed() is False
        assert sas.is_v_closed() is False

    def test_singularities_sphere_has_two(self):
        # SphericalSurface has two singular points (the poles).
        sas = ShapeAnalysis.Surface(self.make_sphere(radius=3.0))
        # Use a generous precision; OCCT reports sphere poles as degenerate.
        assert sas.has_singularities(1e-3) is True
        assert sas.nb_singularities(1e-3) == 2

    def test_singularities_plane_has_none(self):
        sas = ShapeAnalysis.Surface(self.make_plane())
        assert sas.has_singularities(1e-3) is False
        assert sas.nb_singularities(1e-3) == 0

    def test_singularity_tuple_shape(self):
        sas = ShapeAnalysis.Surface(self.make_sphere(radius=3.0))
        result = sas.singularity(1)
        assert len(result) == 8
        ok, preci, p3d, first_p2d, last_p2d, first_par, last_par, u_iso_deg = result
        assert ok is True
        assert isinstance(preci, float)
        assert isinstance(u_iso_deg, bool)

    def test_singularity_out_of_range_returns_false(self):
        sas = ShapeAnalysis.Surface(self.make_plane())
        ok = sas.singularity(99)[0]
        assert ok is False

    def test_bounds_plane(self):
        sas = ShapeAnalysis.Surface(self.make_plane())
        uf, ul, vf, vl = sas.bounds()
        # Geom_Plane reports its bounds via Precision::Infinite (~2e+100).
        assert uf < -1e99 and ul > 1e99
        assert vf < -1e99 and vl > 1e99

    def test_close_vals_accessible(self):
        sas = ShapeAnalysis.Surface(self.make_cylinder())
        # The values are populated lazily by IsUClosed/IsVClosed.
        sas.is_u_closed()
        sas.is_v_closed()
        assert isinstance(sas.u_close_val, float)
        assert isinstance(sas.v_close_val, float)

    def test_value_of_uv_projection_on_cylinder(self):
        cyl = self.make_cylinder(radius=5.0)
        sas = ShapeAnalysis.Surface(cyl)
        # Pick a point on the cylinder at angle pi/4, height 2.
        u_expected = math.pi / 4
        v_expected = 2.0
        p_on = sas.value(u_expected, v_expected)
        uv = sas.value_of_uv(p_on, 1e-7)
        # Re-evaluating at the returned uv should land back on p_on.
        p_back = sas.value_at(uv)
        assert p_back.x == pytest.approx(p_on.x, abs=1e-6)
        assert p_back.y == pytest.approx(p_on.y, abs=1e-6)
        assert p_back.z == pytest.approx(p_on.z, abs=1e-6)

    def test_gap_set_by_value_of_uv(self):
        cyl = self.make_cylinder(radius=5.0)
        sas = ShapeAnalysis.Surface(cyl)
        p_on = sas.value(0.0, 0.0)
        sas.value_of_uv(p_on, 1e-7)
        # Gap should be (near) zero since the point is on the surface.
        assert sas.gap == pytest.approx(0.0, abs=1e-6)

    def test_is_degenerated_at_sphere_pole(self):
        sphere = self.make_sphere(radius=3.0)
        sas = ShapeAnalysis.Surface(sphere)
        # The north pole of a sphere centered at origin with Z axis is (0,0,R).
        pole = gp.Pnt(0, 0, 3.0)
        assert sas.is_degenerated(pole, 1e-3) is True
        # An interior point should not be flagged.
        equator = sas.value(0.0, 0.0)
        assert sas.is_degenerated(equator, 1e-3) is False

    def test_iso_curves(self):
        cyl = self.make_cylinder(radius=5.0)
        sas = ShapeAnalysis.Surface(cyl)
        u_iso = sas.u_iso(0.0)
        v_iso = sas.v_iso(0.0)
        # Both must be valid Geom_Curve handles for a regular cylinder.
        assert u_iso is not None
        assert v_iso is not None

    def test_adaptor_lazy(self):
        sas = ShapeAnalysis.Surface(self.make_plane())
        # Adaptor3d builds it; TrueAdaptor3d returns cached.
        ad = sas.adaptor3d
        assert ad is not None
        true_ad = sas.true_adaptor3d
        assert true_ad is not None
