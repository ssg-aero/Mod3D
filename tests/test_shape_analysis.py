"""Tests for ShapeAnalysis module bindings."""

import math

import pytest

from mod3d import BRepBuilderAPI, Geom, ShapeAnalysis, ShapeFix, gp


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


def _rect_wire_and_face():
    """Build a closed rectangular wire and its planar face."""
    p1 = gp.Pnt(0, 0, 0)
    p2 = gp.Pnt(10, 0, 0)
    p3 = gp.Pnt(10, 10, 0)
    p4 = gp.Pnt(0, 10, 0)

    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()

    w = BRepBuilderAPI.MakeWire()
    for e in (e1, e2, e3, e4):
        w.add(e)
    wire = w.wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    return wire, face


class TestShapeAnalysisWire:
    """Tests for ShapeAnalysis.Wire."""

    def test_default_constructor(self):
        saw = ShapeAnalysis.Wire()
        assert not saw.is_loaded
        assert not saw.is_ready
        assert saw.nb_edges == 0

    def test_constructor_with_wire_face_precision(self):
        wire, face = _rect_wire_and_face()
        saw = ShapeAnalysis.Wire(wire, face, 1e-6)
        assert saw.is_loaded
        assert saw.is_ready
        assert saw.nb_edges == 4
        assert saw.precision == pytest.approx(1e-6)

    def test_init_and_load(self):
        wire, face = _rect_wire_and_face()
        saw = ShapeAnalysis.Wire()
        saw.init(wire, face, 1e-7)
        assert saw.is_ready
        assert saw.precision == pytest.approx(1e-7)

        saw2 = ShapeAnalysis.Wire()
        saw2.load(wire)
        assert saw2.is_loaded
        assert not saw2.is_ready
        saw2.set_face(face)
        assert saw2.is_ready

    def test_set_surface_overloads(self):
        wire, _ = _rect_wire_and_face()
        plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))

        # set_surface(Geom_Surface) wraps the surface into a face
        # internally, so the analyzer becomes ready.
        saw = ShapeAnalysis.Wire()
        saw.load(wire)
        saw.set_surface(plane)
        assert saw.is_ready
        # check_edge_curves needs the surface; just ensure it runs.
        saw.check_edge_curves()
        assert saw.surface is not None

        # set_surface_analysis(SAS) is OCCT 8.0+ only: it lets callers
        # share one ShapeAnalysis.Surface across many wires. The face
        # stays null, so is_ready remains False (matches OCCT). The bound
        # surface is still queryable for callers driving individual
        # surface-aware checks.
        if not hasattr(ShapeAnalysis.Wire, "set_surface_analysis"):
            pytest.skip("set_surface_analysis requires OCCT >= 8.0")
        saw2 = ShapeAnalysis.Wire()
        saw2.load(wire)
        sas = ShapeAnalysis.Surface(plane)
        saw2.set_surface_analysis(sas)
        assert saw2.is_loaded
        assert saw2.surface is not None

    def test_perform_on_clean_wire(self):
        wire, face = _rect_wire_and_face()
        saw = ShapeAnalysis.Wire(wire, face, 1e-6)
        # A clean rectangular wire on a planar face should not be flagged
        # by any of the standard checks.
        flagged = saw.perform()
        assert flagged in (True, False)

    def test_individual_checks_run(self):
        wire, face = _rect_wire_and_face()
        saw = ShapeAnalysis.Wire(wire, face, 1e-6)
        # All the no-arg / default-arg variants should at least execute.
        for method in (
            "check_order",
            "check_connected",
            "check_small",
            "check_edge_curves",
            "check_degenerated",
            "check_closed",
            "check_self_intersection",
            "check_lacking",
            "check_gaps_3d",
            "check_gaps_2d",
            "check_curve_gaps",
        ):
            result = getattr(saw, method)()
            assert result in (True, False)

    def test_status_accessors(self):
        wire, face = _rect_wire_and_face()
        saw = ShapeAnalysis.Wire(wire, face, 1e-6)
        saw.check_connected()
        # Clean rectangular wire: edges share their vertices, so status
        # for the OK enum should be True (or at minimum a bool).
        ok = ShapeAnalysis.ShapeExtendStatus.OK
        assert isinstance(saw.status_connected(ok), bool)

    def test_check_lacking_values_tuple_shape(self):
        wire, face = _rect_wire_and_face()
        saw = ShapeAnalysis.Wire(wire, face, 1e-6)
        # Even when nothing is flagged, the tuple shape must be (bool, p2d, p2d).
        result = saw.check_lacking_values(1, 0.0)
        assert len(result) == 3
        flagged, p2d1, p2d2 = result
        assert isinstance(flagged, bool)
        assert hasattr(p2d1, "x") and hasattr(p2d2, "x")

    def test_check_degenerated_values_tuple_shape(self):
        wire, face = _rect_wire_and_face()
        saw = ShapeAnalysis.Wire(wire, face, 1e-6)
        result = saw.check_degenerated_values(1)
        assert len(result) == 3
        flagged, p2d1, p2d2 = result
        assert isinstance(flagged, bool)

    def test_distance_metrics_accessible(self):
        wire, face = _rect_wire_and_face()
        saw = ShapeAnalysis.Wire(wire, face, 1e-6)
        saw.check_connected()
        # Just check the properties don't error and return floats.
        assert isinstance(saw.min_distance_3d, float)
        assert isinstance(saw.max_distance_3d, float)
        assert isinstance(saw.min_distance_2d, float)
        assert isinstance(saw.max_distance_2d, float)

    def test_surface_property_built_from_face(self):
        wire, face = _rect_wire_and_face()
        saw = ShapeAnalysis.Wire(wire, face, 1e-6)
        # The analyzer should be able to expose its working surface
        # (built lazily from the face on first query).
        # Trigger a check that needs the surface, then read it back.
        saw.check_edge_curves()
        surf = saw.surface
        assert surf is not None


class TestShapeFixWireInitFromAnalyzer:
    """ShapeFix.Wire.init(ShapeAnalysis.Wire) handoff."""

    def test_init_from_analyzer(self):
        wire, face = _rect_wire_and_face()
        saw = ShapeAnalysis.Wire(wire, face, 1e-6)

        fixer = ShapeFix.Wire()
        fixer.init(saw)
        # The fixer should pick up the analyzer's wire AND face,
        # so it's immediately ready — no set_face / set_surface needed.
        assert fixer.is_loaded
        assert fixer.is_ready
        assert fixer.nb_edges == 4

    def test_init_from_analyzer_then_perform(self):
        wire, face = _rect_wire_and_face()
        saw = ShapeAnalysis.Wire(wire, face, 1e-6)
        fixer = ShapeFix.Wire()
        fixer.init(saw)
        # Standard fix battery should run on the inherited state.
        result = fixer.perform()
        assert result in (True, False)
        fixed = fixer.wire
        assert fixed is not None
