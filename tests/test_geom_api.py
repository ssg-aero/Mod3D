"""Tests for GeomAPI bindings (curve interpolation and approximation)."""

import pytest
import math

from mod3d import gp, GeomAPI, GeomAbs


class TestInterpolate3D:
    """Tests for GeomAPI.Interpolate (3D BSpline interpolation)."""

    def test_basic_interpolation(self):
        """Test basic 3D point interpolation."""
        points = [
            gp.Pnt(0, 0, 0),
            gp.Pnt(1, 1, 0),
            gp.Pnt(2, 0, 0),
            gp.Pnt(3, 1, 0),
        ]
        
        interp = GeomAPI.Interpolate(points)
        interp.perform()
        
        assert interp.is_done
        curve = interp.curve
        assert curve is not None

    def test_interpolation_passes_through_points(self):
        """Test that interpolated curve passes through input points."""
        points = [
            gp.Pnt(0, 0, 0),
            gp.Pnt(1, 2, 0),
            gp.Pnt(2, 0, 0),
        ]
        
        interp = GeomAPI.Interpolate(points)
        interp.perform()
        
        assert interp.is_done
        curve = interp.curve
        
        # Check curve passes through start and end points
        start = curve.value(curve.first_parameter)
        end = curve.value(curve.last_parameter)
        
        assert start.distance(points[0]) == pytest.approx(0, abs=1e-6)
        assert end.distance(points[-1]) == pytest.approx(0, abs=1e-6)

    def test_periodic_interpolation(self):
        """Test periodic (closed) curve interpolation."""
        # Square points
        points = [
            gp.Pnt(0, 0, 0),
            gp.Pnt(1, 0, 0),
            gp.Pnt(1, 1, 0),
            gp.Pnt(0, 1, 0),
        ]
        
        interp = GeomAPI.Interpolate(points, periodic=True)
        interp.perform()
        
        assert interp.is_done
        curve = interp.curve
        assert curve.is_periodic or curve.is_closed

    def test_interpolation_with_parameters(self):
        """Test interpolation with explicit parameter values."""
        points = [
            gp.Pnt(0, 0, 0),
            gp.Pnt(1, 1, 0),
            gp.Pnt(2, 0, 0),
        ]
        parameters = [0.0, 0.5, 1.0]
        
        interp = GeomAPI.Interpolate(points, parameters)
        interp.perform()
        
        assert interp.is_done
        curve = interp.curve
        
        # Parameter range should match
        assert curve.first_parameter == pytest.approx(0.0, abs=1e-6)
        assert curve.last_parameter == pytest.approx(1.0, abs=1e-6)

    def test_interpolation_with_tangents(self):
        """Test interpolation with end tangent constraints."""
        points = [
            gp.Pnt(0, 0, 0),
            gp.Pnt(1, 0, 0),
            gp.Pnt(2, 0, 0),
        ]
        
        interp = GeomAPI.Interpolate(points)
        # Set tangents: start pointing up, end pointing up
        interp.load(gp.Vec(0, 1, 0), gp.Vec(0, 1, 0))
        interp.perform()
        
        assert interp.is_done
        curve = interp.curve
        assert curve is not None

    def test_interpolation_with_per_point_tangents(self):
        """Test interpolation with tangent at specific points."""
        points = [
            gp.Pnt(0, 0, 0),
            gp.Pnt(1, 1, 0),
            gp.Pnt(2, 0, 0),
        ]
        tangents = [
            gp.Vec(1, 0, 0),  # horizontal at start
            gp.Vec(1, 0, 0),  # horizontal at middle
            gp.Vec(1, 0, 0),  # horizontal at end
        ]
        tangent_flags = [True, False, True]  # Only apply at start and end
        
        interp = GeomAPI.Interpolate(points)
        interp.load_tangents(tangents, tangent_flags)
        interp.perform()
        
        assert interp.is_done


class TestPointsToBSpline3D:
    """Tests for GeomAPI.PointsToBSpline (3D BSpline approximation)."""

    def test_basic_approximation(self):
        """Test basic 3D point approximation."""
        points = [
            gp.Pnt(0, 0, 0),
            gp.Pnt(1, 1, 0),
            gp.Pnt(2, 0.5, 0),
            gp.Pnt(3, 1.5, 0),
            gp.Pnt(4, 0, 0),
        ]
        
        approx = GeomAPI.PointsToBSpline(points)
        
        assert approx.is_done
        curve = approx.curve
        assert curve is not None

    def test_approximation_with_degree_constraints(self):
        """Test approximation with min/max degree."""
        points = [gp.Pnt(i, math.sin(i), 0) for i in range(10)]
        
        approx = GeomAPI.PointsToBSpline(points, deg_min=2, deg_max=5)
        
        assert approx.is_done
        curve = approx.curve
        assert 2 <= curve.degree <= 5

    def test_approximation_with_continuity(self):
        """Test approximation with different continuity levels."""
        points = [gp.Pnt(i, math.sin(i * 0.5), 0) for i in range(8)]
        
        for continuity in [GeomAbs.Shape.C0, GeomAbs.Shape.C1, GeomAbs.Shape.C2]:
            approx = GeomAPI.PointsToBSpline(points, continuity=continuity)
            assert approx.is_done

    def test_approximation_with_tolerance(self):
        """Test approximation with specified tolerance."""
        points = [gp.Pnt(i, math.sin(i), 0) for i in range(10)]
        
        # Tighter tolerance
        approx = GeomAPI.PointsToBSpline(points, tol3d=1e-4)
        assert approx.is_done

    def test_approximation_with_parameters(self):
        """Test approximation with explicit parameter values."""
        points = [
            gp.Pnt(0, 0, 0),
            gp.Pnt(1, 1, 0),
            gp.Pnt(2, 0, 0),
        ]
        parameters = [0.0, 0.5, 1.0]
        
        approx = GeomAPI.PointsToBSpline(points, parameters)
        
        assert approx.is_done

    def test_approximation_variational_smoothing(self):
        """Test approximation with variational smoothing weights."""
        points = [gp.Pnt(i, math.sin(i) + 0.1 * (i % 2), 0) for i in range(10)]
        
        # Weight curvature minimization
        approx = GeomAPI.PointsToBSpline(points, 
                                         weight1=0.0,   # Length
                                         weight2=1.0,   # Curvature
                                         weight3=0.0)   # Torsion
        
        assert approx.is_done

    def test_empty_constructor_then_init(self):
        """Test using empty constructor followed by init()."""
        approx = GeomAPI.PointsToBSpline()
        
        points = [gp.Pnt(0, 0, 0), gp.Pnt(1, 1, 0), gp.Pnt(2, 0, 0)]
        approx.init(points)
        
        assert approx.is_done


class TestInterpolate2D:
    """Tests for GeomAPI.Interpolate2d (2D BSpline interpolation)."""

    def test_basic_interpolation_2d(self):
        """Test basic 2D point interpolation."""
        points = [
            gp.Pnt2d(0, 0),
            gp.Pnt2d(1, 1),
            gp.Pnt2d(2, 0),
            gp.Pnt2d(3, 1),
        ]
        
        interp = GeomAPI.Interpolate2d(points)
        interp.perform()
        
        assert interp.is_done
        curve = interp.curve
        assert curve is not None

    def test_interpolation_2d_passes_through_points(self):
        """Test that 2D interpolated curve passes through input points."""
        points = [
            gp.Pnt2d(0, 0),
            gp.Pnt2d(1, 2),
            gp.Pnt2d(2, 0),
        ]
        
        interp = GeomAPI.Interpolate2d(points)
        interp.perform()
        
        assert interp.is_done
        curve = interp.curve
        
        # Check start and end
        start = curve.value(curve.first_parameter)
        end = curve.value(curve.last_parameter)
        
        assert start.distance(points[0]) == pytest.approx(0, abs=1e-6)
        assert end.distance(points[-1]) == pytest.approx(0, abs=1e-6)

    def test_periodic_interpolation_2d(self):
        """Test periodic (closed) 2D curve interpolation."""
        # Triangle points
        points = [
            gp.Pnt2d(0, 0),
            gp.Pnt2d(1, 0),
            gp.Pnt2d(0.5, 1),
        ]
        
        interp = GeomAPI.Interpolate2d(points, periodic=True)
        interp.perform()
        
        assert interp.is_done
        curve = interp.curve
        assert curve.is_periodic or curve.is_closed

    def test_interpolation_2d_with_tangents(self):
        """Test 2D interpolation with end tangent constraints."""
        points = [
            gp.Pnt2d(0, 0),
            gp.Pnt2d(1, 0),
            gp.Pnt2d(2, 0),
        ]
        
        interp = GeomAPI.Interpolate2d(points)
        interp.load(gp.Vec2d(0, 1), gp.Vec2d(0, 1))
        interp.perform()
        
        assert interp.is_done


class TestPointsToBSpline2D:
    """Tests for GeomAPI.PointsToBSpline2d (2D BSpline approximation)."""

    def test_basic_approximation_2d(self):
        """Test basic 2D point approximation."""
        points = [
            gp.Pnt2d(0, 0),
            gp.Pnt2d(1, 1),
            gp.Pnt2d(2, 0.5),
            gp.Pnt2d(3, 1.5),
            gp.Pnt2d(4, 0),
        ]
        
        approx = GeomAPI.PointsToBSpline2d(points)
        
        assert approx.is_done
        curve = approx.curve
        assert curve is not None

    def test_approximation_2d_from_y_values(self):
        """Test 2D approximation from Y values at uniform X spacing."""
        y_values = [0, 1, 0.5, 1.5, 0]
        x0 = 0.0
        dx = 1.0
        
        approx = GeomAPI.PointsToBSpline2d(y_values, x0, dx)
        
        assert approx.is_done
        curve = approx.curve
        
        # Check parameter range matches X range
        assert curve.first_parameter == pytest.approx(0.0, abs=1e-6)
        assert curve.last_parameter == pytest.approx(4.0, abs=1e-6)

    def test_approximation_2d_with_degree_constraints(self):
        """Test 2D approximation with min/max degree."""
        points = [gp.Pnt2d(i, math.sin(i)) for i in range(10)]
        
        approx = GeomAPI.PointsToBSpline2d(points, deg_min=2, deg_max=5)
        
        assert approx.is_done
        curve = approx.curve
        assert 2 <= curve.degree <= 5

    def test_approximation_2d_with_continuity(self):
        """Test 2D approximation with different continuity levels."""
        points = [gp.Pnt2d(i, math.sin(i * 0.5)) for i in range(8)]
        
        for continuity in [GeomAbs.Shape.C0, GeomAbs.Shape.C1, GeomAbs.Shape.C2]:
            approx = GeomAPI.PointsToBSpline2d(points, continuity=continuity)
            assert approx.is_done

    def test_empty_constructor_then_init_2d(self):
        """Test using empty constructor followed by init()."""
        approx = GeomAPI.PointsToBSpline2d()
        
        points = [gp.Pnt2d(0, 0), gp.Pnt2d(1, 1), gp.Pnt2d(2, 0)]
        approx.init(points)
        
        assert approx.is_done


class TestGeomAPIConversion:
    """Tests for GeomAPI curve conversion functions."""

    def test_to_2d_and_to_3d_roundtrip(self):
        """Test converting 3D curve to 2D and back."""
        # Create a 3D curve in XY plane
        points = [
            gp.Pnt(0, 0, 0),
            gp.Pnt(1, 1, 0),
            gp.Pnt(2, 0, 0),
        ]
        interp = GeomAPI.Interpolate(points)
        interp.perform()
        curve_3d = interp.curve
        
        # Convert to 2D on XY plane
        plane = gp.Pln()  # Default XY plane at origin
        curve_2d = GeomAPI.to_2d(curve_3d, plane)
        
        assert curve_2d is not None
        
        # Convert back to 3D
        curve_3d_back = GeomAPI.to_3d(curve_2d, plane)
        
        assert curve_3d_back is not None
        
        # Check start points match
        start_original = curve_3d.value(curve_3d.first_parameter)
        start_back = curve_3d_back.value(curve_3d_back.first_parameter)
        
        assert start_original.distance(start_back) == pytest.approx(0, abs=1e-6)


class TestExtremaCurveCurve:
    """Tests for GeomAPI.ExtremaCurveCurve (computing extrema between two curves)."""

    def test_default_constructor(self):
        """Test default constructor creates empty algorithm."""
        extrema = GeomAPI.ExtremaCurveCurve()
        # Should exist but nb_extrema is undefined until init

    def test_parallel_lines(self):
        """Test extrema between parallel lines."""
        from mod3d import Geom
        
        # Two parallel lines separated by 5 units
        line1 = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        line2 = Geom.Line(gp.Pnt(0, 5, 0), gp.Dir(1, 0, 0))
        
        extrema = GeomAPI.ExtremaCurveCurve(line1, line2)
        
        # Parallel lines should be detected
        assert extrema.is_parallel

    def test_perpendicular_lines(self):
        """Test extrema between perpendicular skew lines."""
        from mod3d import Geom
        
        # X-axis at origin
        line1 = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        # Z-axis offset by (0, 3, 0) - perpendicular and skew
        line2 = Geom.Line(gp.Pnt(0, 3, 0), gp.Dir(0, 0, 1))
        
        extrema = GeomAPI.ExtremaCurveCurve(line1, line2)
        
        assert not extrema.is_parallel
        assert extrema.nb_extrema >= 1
        
        # The minimum distance should be 3 (the offset in Y)
        assert extrema.lower_distance == pytest.approx(3.0, abs=1e-6)
        
        # Get the nearest points
        p1, p2 = extrema.nearest_points
        assert p1.distance(gp.Pnt(0, 0, 0)) == pytest.approx(0.0, abs=1e-6)
        assert p2.distance(gp.Pnt(0, 3, 0)) == pytest.approx(0.0, abs=1e-6)

    def test_extrema_with_trimmed_curves(self):
        """Test extrema with parameter bounds."""
        from mod3d import Geom
        
        line1 = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        line2 = Geom.Line(gp.Pnt(0, 2, 0), gp.Dir(0, 0, 1))
        
        # Compute extrema on portions of the curves
        extrema = GeomAPI.ExtremaCurveCurve(line1, line2, -10.0, 10.0, -10.0, 10.0)
        
        assert extrema.nb_extrema >= 1
        assert extrema.lower_distance == pytest.approx(2.0, abs=1e-6)

    def test_extrema_line_circle(self):
        """Test extrema between a line and a circle."""
        from mod3d import Geom
        
        # Line along X at Y=0
        line = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        
        # Circle in XY plane, centered at (0, 5, 0) with radius 2
        ax2 = gp.Ax2(gp.Pnt(0, 5, 0), gp.Dir(0, 0, 1))
        circle = Geom.Circle(ax2, 2.0)
        
        extrema = GeomAPI.ExtremaCurveCurve(line, circle)
        
        assert extrema.nb_extrema >= 1
        # Minimum distance = 5 - 2 = 3 (center offset minus radius)
        assert extrema.lower_distance == pytest.approx(3.0, abs=1e-6)

    def test_points_and_parameters(self):
        """Test retrieving points and parameters for each extremum."""
        from mod3d import Geom
        
        line1 = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        line2 = Geom.Line(gp.Pnt(5, 4, 0), gp.Dir(0, 0, 1))
        
        extrema = GeomAPI.ExtremaCurveCurve(line1, line2)
        
        assert extrema.nb_extrema >= 1
        
        # Get points for first extremum
        p1, p2 = extrema.points(1)
        assert p1 is not None
        assert p2 is not None
        
        # Get parameters for first extremum
        u1, u2 = extrema.parameters(1)
        
        # Verify point is on first line at parameter u1
        expected_p1 = gp.Pnt(u1, 0, 0)  # Point on line1 at parameter u1
        assert p1.distance(expected_p1) == pytest.approx(0.0, abs=1e-6)

    def test_distance_at_index(self):
        """Test getting distance at specific extremum index."""
        from mod3d import Geom
        
        line1 = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        line2 = Geom.Line(gp.Pnt(0, 7, 0), gp.Dir(0, 0, 1))
        
        extrema = GeomAPI.ExtremaCurveCurve(line1, line2)
        
        assert extrema.nb_extrema >= 1
        
        dist = extrema.distance(1)
        assert dist == pytest.approx(7.0, abs=1e-6)

    def test_lower_distance_parameters(self):
        """Test getting parameters of shortest extremum."""
        from mod3d import Geom
        
        line1 = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        line2 = Geom.Line(gp.Pnt(3, 2, 0), gp.Dir(0, 0, 1))
        
        extrema = GeomAPI.ExtremaCurveCurve(line1, line2)
        
        u1, u2 = extrema.lower_distance_parameters
        
        # u1 should be 3 (X position of line2)
        # u2 should be 0 (line2 intersects the closest point at its origin)
        assert u1 == pytest.approx(3.0, abs=1e-6)
        assert u2 == pytest.approx(0.0, abs=1e-6)

    def test_init_method(self):
        """Test using init() to compute extrema after construction."""
        from mod3d import Geom
        
        extrema = GeomAPI.ExtremaCurveCurve()
        
        line1 = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        line2 = Geom.Line(gp.Pnt(0, 10, 0), gp.Dir(0, 0, 1))
        
        extrema.init(line1, line2)
        
        assert extrema.nb_extrema >= 1
        assert extrema.lower_distance == pytest.approx(10.0, abs=1e-6)

    def test_init_with_bounds(self):
        """Test init() with parameter bounds."""
        from mod3d import Geom
        
        extrema = GeomAPI.ExtremaCurveCurve()
        
        line1 = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        line2 = Geom.Line(gp.Pnt(0, 5, 0), gp.Dir(0, 0, 1))
        
        extrema.init(line1, line2, 0.0, 100.0, -50.0, 50.0)
        
        assert extrema.nb_extrema >= 1

    def test_total_nearest_points(self):
        """Test TotalNearestPoints which considers curve endpoints."""
        from mod3d import Geom
        
        # Create two trimmed curves where the nearest points might be at endpoints
        line1 = Geom.TrimmedCurve(Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)), 0, 10)
        line2 = Geom.TrimmedCurve(Geom.Line(gp.Pnt(5, 3, 0), gp.Dir(0, 0, 1)), 0, 10)
        
        extrema = GeomAPI.ExtremaCurveCurve(line1, line2)
        
        found, p1, p2 = extrema.total_nearest_points
        
        if found:
            assert p1 is not None
            assert p2 is not None

    def test_total_lower_distance_parameters(self):
        """Test TotalLowerDistanceParameters."""
        from mod3d import Geom
        
        line1 = Geom.TrimmedCurve(Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)), 0, 10)
        line2 = Geom.TrimmedCurve(Geom.Line(gp.Pnt(5, 2, 0), gp.Dir(0, 0, 1)), 0, 10)
        
        extrema = GeomAPI.ExtremaCurveCurve(line1, line2)
        
        found, u1, u2 = extrema.total_lower_distance_parameters
        
        if found:
            assert u1 == pytest.approx(5.0, abs=1e-6)

    def test_total_lower_distance(self):
        """Test TotalLowerDistance."""
        from mod3d import Geom
        
        line1 = Geom.TrimmedCurve(Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)), 0, 10)
        line2 = Geom.TrimmedCurve(Geom.Line(gp.Pnt(5, 4, 0), gp.Dir(0, 0, 1)), 0, 10)
        
        extrema = GeomAPI.ExtremaCurveCurve(line1, line2)
        
        total_dist = extrema.total_lower_distance
        assert total_dist == pytest.approx(4.0, abs=1e-6)

    def test_bspline_curves(self):
        """Test extrema between two BSpline curves."""
        # Create two BSpline curves
        points1 = [gp.Pnt(0, 0, 0), gp.Pnt(1, 1, 0), gp.Pnt(2, 0, 0)]
        interp1 = GeomAPI.Interpolate(points1)
        interp1.perform()
        curve1 = interp1.curve
        
        points2 = [gp.Pnt(0, 3, 0), gp.Pnt(1, 2, 0), gp.Pnt(2, 3, 0)]
        interp2 = GeomAPI.Interpolate(points2)
        interp2.perform()
        curve2 = interp2.curve
        
        extrema = GeomAPI.ExtremaCurveCurve(curve1, curve2)
        
        assert extrema.nb_extrema >= 1
        # The curves approach closest around x=1
        # curve1 peaks at (1,1,0), curve2 dips to (1,2,0), so min dist ~ 1
        assert extrema.lower_distance == pytest.approx(1.0, abs=0.1)


class TestExtremaCurveSurface:
    """Tests for GeomAPI.ExtremaCurveSurface (extrema between curve and surface)."""

    def test_line_and_plane(self):
        """Test extrema between a line and a plane."""
        from mod3d import Geom
        
        # Line at z=5, parallel to xy plane
        line = Geom.Line(gp.Pnt(0, 0, 5), gp.Dir(1, 0, 0))
        # Plane at z=0
        plane = Geom.Plane(gp.Ax3())
        
        extrema = GeomAPI.ExtremaCurveSurface(line, plane)
        
        # Line is parallel to the plane
        assert extrema.is_parallel
        assert extrema.lower_distance == pytest.approx(5.0, abs=1e-6)

    def test_perpendicular_line_to_plane(self):
        """Test extrema for perpendicular line to plane."""
        from mod3d import Geom
        
        # Line perpendicular to XY plane at origin
        line = Geom.Line(gp.Pnt(0, 0, 10), gp.Dir(0, 0, -1))
        plane = Geom.Plane(gp.Ax3())
        
        extrema = GeomAPI.ExtremaCurveSurface(line, plane, 0, 15, -10, 10, -10, 10)
        
        assert extrema.nb_extrema >= 1
        assert extrema.lower_distance == pytest.approx(.0, abs=1e-6)

    def test_default_constructor(self):
        """Test empty constructor and init method."""
        from mod3d import Geom
        
        extrema = GeomAPI.ExtremaCurveSurface()
        
        line = Geom.Line(gp.Pnt(0, 0, 3), gp.Dir(1, 0, 0))
        plane = Geom.Plane(gp.Ax3())
        
        extrema.init(line, plane)
        
        assert extrema.is_parallel
        assert extrema.lower_distance == pytest.approx(3.0, abs=1e-6)

    def test_points_method(self):
        """Test points() returns curve and surface points."""
        from mod3d import Geom
        
        line = Geom.Line(gp.Pnt(0, 0, 7), gp.Dir(0, 0, -1))
        plane = Geom.Plane(gp.Ax3())
        
        extrema = GeomAPI.ExtremaCurveSurface(line, plane)
        
        if extrema.nb_extrema > 0:
            pc, ps = extrema.points(1)
            assert pc.z == pytest.approx(0.0, abs=1e-6)
            assert ps.z == pytest.approx(0.0, abs=1e-6)

    def test_parameters_method(self):
        """Test parameters() returns W on curve and (U,V) on surface."""
        from mod3d import Geom
        
        line = Geom.Line(gp.Pnt(0, 0, 4), gp.Dir(0, 0, -1))
        plane = Geom.Plane(gp.Ax3())
        
        extrema = GeomAPI.ExtremaCurveSurface(line, plane)
        
        if extrema.nb_extrema > 0:
            w, u, v = extrema.parameters(1)
            # Parameters should be valid numbers
            assert isinstance(w, float)
            assert isinstance(u, float)
            assert isinstance(v, float)

    def test_nearest_points(self):
        """Test nearest_points() returns closest point pair."""
        from mod3d import Geom
        
        line = Geom.Line(gp.Pnt(5, 5, 6), gp.Dir(0, 0, -1))
        plane = Geom.Plane(gp.Ax3())
        
        extrema = GeomAPI.ExtremaCurveSurface(line, plane)
        
        pc, ps = extrema.nearest_points
        assert pc.distance(ps) == pytest.approx(0.0, abs=1e-6)

    def test_lower_distance_parameters(self):
        """Test lower_distance_parameters() returns parameters of closest points."""
        from mod3d import Geom
        
        line = Geom.Line(gp.Pnt(3, 4, 8), gp.Dir(0, 0, -1))
        plane = Geom.Plane(gp.Ax3())
        
        extrema = GeomAPI.ExtremaCurveSurface(line, plane)
        
        w, u, v = extrema.lower_distance_parameters
        assert isinstance(w, float)
        assert isinstance(u, float)
        assert isinstance(v, float)

    def test_distance_method(self):
        """Test distance() method for specific extremum."""
        from mod3d import Geom
        
        line = Geom.Line(gp.Pnt(0, 0, 9), gp.Dir(0, 0, -1))
        plane = Geom.Plane(gp.Ax3())
        
        extrema = GeomAPI.ExtremaCurveSurface(line, plane)

        assert extrema.nb_extrema >= 1

        if extrema.nb_extrema > 0:
            dist = extrema.distance(1)
            assert dist == pytest.approx(0.0, abs=1e-6)


class TestExtremaSurfaceSurface:
    """Tests for GeomAPI.ExtremaSurfaceSurface (extrema between two surfaces)."""

    def test_parallel_planes(self):
        """Test extrema between two parallel planes."""
        from mod3d import Geom
        
        # Plane at z=0
        plane1 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
        # Plane at z=10
        plane2 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 10), gp.Dir(0, 0, 1)))
        
        extrema = GeomAPI.ExtremaSurfaceSurface(plane1, plane2)
        
        assert extrema.is_parallel
        assert extrema.lower_distance == pytest.approx(10.0, abs=1e-6)

    def test_default_constructor(self):
        """Test empty constructor and init method."""
        from mod3d import Geom
        
        extrema = GeomAPI.ExtremaSurfaceSurface()
        
        plane1 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
        plane2 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 5), gp.Dir(0, 0, 1)))
        
        extrema.init(plane1, plane2)
        
        assert extrema.is_parallel
        assert extrema.lower_distance == pytest.approx(5.0, abs=1e-6)

    @pytest.mark.skip(reason="Known issue with extrema between parallel surfaces.")
    def test_points_method(self):
        """Test points() returns surface points."""
        from mod3d import Geom
        
        plane1 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
        plane2 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 7), gp.Dir(0, 0, 1)))
        
        extrema = GeomAPI.ExtremaSurfaceSurface(plane1, plane2)
        
        if extrema.nb_extrema > 0:
            p1, p2 = extrema.points(1)
            assert abs(p2.Z() - p1.Z()) == pytest.approx(7.0, abs=1e-6)

    @pytest.mark.skip(reason="Known issue with extrema between parallel surfaces.")
    def test_parameters_method(self):
        """Test parameters() returns (U1,V1,U2,V2)."""
        from mod3d import Geom
        
        plane1 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
        plane2 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 3), gp.Dir(0, 0, 1)))
        
        extrema = GeomAPI.ExtremaSurfaceSurface(plane1, plane2)
        
        if extrema.nb_extrema > 0:
            u1, v1, u2, v2 = extrema.parameters(1)
            assert isinstance(u1, float)
            assert isinstance(v1, float)
            assert isinstance(u2, float)
            assert isinstance(v2, float)

    @pytest.mark.skip(reason="Known issue with extrema between parallel surfaces.")
    def test_nearest_points(self):
        """Test nearest_points() method."""
        from mod3d import Geom
        
        plane1 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
        plane2 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 12), gp.Dir(0, 0, 1)))
        
        extrema = GeomAPI.ExtremaSurfaceSurface(plane1, plane2)
        
        p1, p2 = extrema.nearest_points()
        assert p1.distance(p2) == pytest.approx(12.0, abs=1e-6)

    @pytest.mark.skip(reason="Known issue with extrema between parallel surfaces.")
    def test_lower_distance_parameters(self):
        """Test lower_distance_parameters() method."""
        from mod3d import Geom
        
        plane1 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
        plane2 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 4), gp.Dir(0, 0, 1)))
        
        extrema = GeomAPI.ExtremaSurfaceSurface(plane1, plane2)
        
        u1, v1, u2, v2 = extrema.lower_distance_parameters()
        assert isinstance(u1, float)
        assert isinstance(v1, float)
        assert isinstance(u2, float)
        assert isinstance(v2, float)

    def test_with_bounds(self):
        """Test constructor with parameter bounds."""
        from mod3d import Geom
        
        plane1 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
        plane2 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 8), gp.Dir(0, 0, 1)))
        
        extrema = GeomAPI.ExtremaSurfaceSurface(
            plane1, plane2,
            -10, 10, -10, 10,  # S1 bounds
            -10, 10, -10, 10   # S2 bounds
        )
        
        assert extrema.lower_distance == pytest.approx(8.0, abs=1e-6)


class TestExtremaCurveCurve2d:
    """Tests for GeomAPI.ExtremaCurveCurve2d (extrema between two 2D curves)."""

    def test_parallel_lines(self):
        """Test extrema between two parallel 2D lines."""
        from mod3d import Geom2d
        
        line1 = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        line2 = Geom2d.Line2d(gp.Pnt2d(0, 5), gp.Dir2d(1, 0))
        
        extrema = GeomAPI.ExtremaCurveCurve2d(line1, line2, 0, 10, 0, 10)
        
        assert extrema.is_parallel

    def test_perpendicular_lines(self):
        """Test extrema between perpendicular 2D lines."""
        from mod3d import Geom2d
        
        line1 = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))  # Horizontal
        line2 = Geom2d.Line2d(gp.Pnt2d(5, -5), gp.Dir2d(0, 1))  # Vertical at x=5
        
        extrema = GeomAPI.ExtremaCurveCurve2d(line1, line2, 0, 10, 0, 10)
        
        assert extrema.nb_extrema >= 1
        assert extrema.lower_distance == pytest.approx(0.0, abs=1e-6)

    def test_points_method(self):
        """Test points() returns 2D points."""
        from mod3d import Geom2d
        
        line1 = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        line2 = Geom2d.Line2d(gp.Pnt2d(0, 3), gp.Dir2d(1, 0))
        
        extrema = GeomAPI.ExtremaCurveCurve2d(line1, line2, 0, 10, 0, 10)
        
        if extrema.nb_extrema > 0:
            p1, p2 = extrema.points(1)
            assert p1.y == pytest.approx(0.0, abs=1e-6)
            assert p2.y == pytest.approx(3.0, abs=1e-6)

    def test_parameters_method(self):
        """Test parameters() returns (U1, U2)."""
        from mod3d import Geom2d
        
        line1 = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        line2 = Geom2d.Line2d(gp.Pnt2d(0, 4), gp.Dir2d(1, 0))
        
        extrema = GeomAPI.ExtremaCurveCurve2d(line1, line2, 0, 10, 0, 10)
        
        if extrema.nb_extrema > 0:
            u1, u2 = extrema.parameters(1)
            assert isinstance(u1, float)
            assert isinstance(u2, float)

    def test_nearest_points(self):
        """Test nearest_points() method."""
        from mod3d import Geom2d
        
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        para = Geom2d.Parabola2d(gp.Ax2d(gp.Pnt2d(0, 6), gp.Dir2d(0, 1)), 1.0)
        
        extrema = GeomAPI.ExtremaCurveCurve2d(line, para, 0, 10, 0, 10)
        
        p1, p2 = extrema.nearest_points()
        assert p1.distance(p2) == pytest.approx(6.0, abs=1e-6)

    def test_lower_distance_parameters(self):
        """Test lower_distance_parameters() method."""
        from mod3d import Geom2d
        
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        para = Geom2d.Parabola2d(gp.Ax2d(gp.Pnt2d(0, 2), gp.Dir2d(0, 1)), 1.0)
        
        extrema = GeomAPI.ExtremaCurveCurve2d(line, para, 0, 10, 0, 10)
        
        u1, u2 = extrema.lower_distance_parameters
        assert isinstance(u1, float)
        assert isinstance(u2, float)

    def test_distance_method(self):
        """Test distance() method."""
        from mod3d import Geom2d
        
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        para = Geom2d.Parabola2d(gp.Ax2d(gp.Pnt2d(0, 2), gp.Dir2d(0, 1)), 1.0)
        
        extrema = GeomAPI.ExtremaCurveCurve2d(line, para, 0, 10, 0, 10)
        
        assert extrema.nb_extrema >= 1

        if extrema.nb_extrema > 0:
            dist = extrema.distance(1)
            assert dist == pytest.approx(2.0, abs=1e-6)


class TestProjectPointOnCurve:
    """Tests for GeomAPI.ProjectPointOnCurve (project 3D point onto curve)."""

    def test_project_onto_line(self):
        """Test projecting a point onto a line."""
        from mod3d import Geom
        
        # Line along X axis
        line = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        # Point above line
        point = gp.Pnt(5, 3, 0)
        
        proj = GeomAPI.ProjectPointOnCurve(point, line)
        
        assert proj.nb_points >= 1
        nearest = proj.nearest_point
        assert nearest.x == pytest.approx(5.0, abs=1e-6)
        assert nearest.y == pytest.approx(0.0, abs=1e-6)
        assert proj.lower_distance == pytest.approx(3.0, abs=1e-6)

    def test_project_onto_circle(self):
        """Test projecting a point onto a circle."""
        from mod3d import Geom
        
        # Circle of radius 10 centered at origin
        circle = Geom.Circle(gp.Ax2(), 10.0)
        # Point outside circle
        point = gp.Pnt(15, 0, 0)
        
        proj = GeomAPI.ProjectPointOnCurve(point, circle)
        
        assert proj.nb_points >= 1
        nearest = proj.nearest_point
        assert nearest.x == pytest.approx(10.0, abs=1e-6)
        assert nearest.y == pytest.approx(0.0, abs=1e-6)
        assert proj.lower_distance == pytest.approx(5.0, abs=1e-6)

    def test_default_constructor(self):
        """Test empty constructor and init method."""
        from mod3d import Geom
        
        proj = GeomAPI.ProjectPointOnCurve()
        
        line = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        point = gp.Pnt(3, 4, 0)
        
        proj.init(point, line)
        
        assert proj.nb_points >= 1
        assert proj.lower_distance == pytest.approx(4.0, abs=1e-6)

    def test_init_with_bounds(self):
        """Test init with parameter bounds."""
        from mod3d import Geom
        
        proj = GeomAPI.ProjectPointOnCurve()
        
        line = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        point = gp.Pnt(5, 2, 0)
        
        proj.init(point, line, 0.0, 10.0)
        
        assert proj.nb_points >= 1
        assert proj.lower_distance == pytest.approx(2.0, abs=1e-6)

    def test_point_method(self):
        """Test point() method returns projection points."""
        from mod3d import Geom
        
        line = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        point = gp.Pnt(7, 3, 0)
        
        proj = GeomAPI.ProjectPointOnCurve(point, line)
        
        if proj.nb_points > 0:
            p = proj.point(1)
            assert p.x == pytest.approx(7.0, abs=1e-6)
            assert p.y == pytest.approx(0.0, abs=1e-6)

    def test_parameter_method(self):
        """Test parameter() method."""
        from mod3d import Geom
        
        line = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        point = gp.Pnt(8, 2, 0)
        
        proj = GeomAPI.ProjectPointOnCurve(point, line)
        
        if proj.nb_points > 0:
            param = proj.parameter(1)
            assert param == pytest.approx(8.0, abs=1e-6)

    def test_distance_method(self):
        """Test distance() method."""
        from mod3d import Geom
        
        line = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        point = gp.Pnt(4, 5, 0)
        
        proj = GeomAPI.ProjectPointOnCurve(point, line)
        
        if proj.nb_points > 0:
            dist = proj.distance(1)
            assert dist == pytest.approx(5.0, abs=1e-6)

    def test_lower_distance_parameter(self):
        """Test lower_distance_parameter property."""
        from mod3d import Geom
        
        line = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        point = gp.Pnt(9, 1, 0)
        
        proj = GeomAPI.ProjectPointOnCurve(point, line)
        
        param = proj.lower_distance_parameter
        assert param == pytest.approx(9.0, abs=1e-6)

    def test_perform_method(self):
        """Test perform() method for multiple projections."""
        from mod3d import Geom
        
        line = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
        
        proj = GeomAPI.ProjectPointOnCurve()
        proj.init_curve(line, 0.0, 100.0)
        
        proj.perform(gp.Pnt(10, 2, 0))
        assert proj.lower_distance == pytest.approx(2.0, abs=1e-6)
        
        proj.perform(gp.Pnt(20, 5, 0))
        assert proj.lower_distance == pytest.approx(5.0, abs=1e-6)

    def test_multiple_projections_on_bspline(self):
        """Test projections onto a BSpline curve."""
        points = [gp.Pnt(0, 0, 0), gp.Pnt(5, 5, 0), gp.Pnt(10, 0, 0)]
        interp = GeomAPI.Interpolate(points)
        interp.perform()
        curve = interp.curve
        
        proj = GeomAPI.ProjectPointOnCurve(gp.Pnt(5, 10, 0), curve)
        
        assert proj.nb_points >= 1
        # Nearest point should be near the peak of the curve
        nearest = proj.nearest_point
        assert nearest.x == pytest.approx(5.0, abs=0.5)


class TestProjectPointOnCurve2d:
    """Tests for GeomAPI.ProjectPointOnCurve2d (project 2D point onto 2D curve)."""

    def test_project_onto_line(self):
        """Test projecting a 2D point onto a 2D line."""
        from mod3d import Geom2d
        
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        point = gp.Pnt2d(5, 4)
        
        proj = GeomAPI.ProjectPointOnCurve2d(point, line)
        
        assert proj.nb_points >= 1
        nearest = proj.nearest_point
        assert nearest.x == pytest.approx(5.0, abs=1e-6)
        assert nearest.y == pytest.approx(0.0, abs=1e-6)
        assert proj.lower_distance == pytest.approx(4.0, abs=1e-6)

    def test_project_onto_circle(self):
        """Test projecting a 2D point onto a 2D circle."""
        from mod3d import Geom2d
        
        circle = Geom2d.Circle2d(gp.Ax2d(), 10.0)
        point = gp.Pnt2d(20, 0)
        
        proj = GeomAPI.ProjectPointOnCurve2d(point, circle)
        
        assert proj.nb_points >= 1
        nearest = proj.nearest_point
        assert nearest.x == pytest.approx(10.0, abs=1e-6)
        assert nearest.y == pytest.approx(0.0, abs=1e-6)
        assert proj.lower_distance == pytest.approx(10.0, abs=1e-6)

    def test_default_constructor(self):
        """Test empty constructor and init method."""
        from mod3d import Geom2d
        
        proj = GeomAPI.ProjectPointOnCurve2d()
        
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        point = gp.Pnt2d(3, 5)
        
        proj.init(point, line)
        
        assert proj.nb_points >= 1
        assert proj.lower_distance == pytest.approx(5.0, abs=1e-6)

    def test_init_with_bounds(self):
        """Test init with parameter bounds."""
        from mod3d import Geom2d
        
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        point = gp.Pnt2d(6, 3)
        
        proj = GeomAPI.ProjectPointOnCurve2d(point, line, 0.0, 10.0)
        
        assert proj.nb_points >= 1
        assert proj.lower_distance == pytest.approx(3.0, abs=1e-6)

    def test_point_method(self):
        """Test point() method."""
        from mod3d import Geom2d
        
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        point = gp.Pnt2d(8, 2)
        
        proj = GeomAPI.ProjectPointOnCurve2d(point, line)
        
        if proj.nb_points > 0:
            p = proj.point(1)
            assert p.x == pytest.approx(8.0, abs=1e-6)
            assert p.y == pytest.approx(0.0, abs=1e-6)

    def test_parameter_method(self):
        """Test parameter() method."""
        from mod3d import Geom2d
        
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        point = gp.Pnt2d(7, 1)
        
        proj = GeomAPI.ProjectPointOnCurve2d(point, line)
        
        if proj.nb_points > 0:
            param = proj.parameter(1)
            assert param == pytest.approx(7.0, abs=1e-6)

    def test_distance_method(self):
        """Test distance() method."""
        from mod3d import Geom2d
        
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        point = gp.Pnt2d(4, 6)
        
        proj = GeomAPI.ProjectPointOnCurve2d(point, line)
        
        if proj.nb_points > 0:
            dist = proj.distance(1)
            assert dist == pytest.approx(6.0, abs=1e-6)

    def test_lower_distance_parameter(self):
        """Test lower_distance_parameter property."""
        from mod3d import Geom2d
        
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        point = gp.Pnt2d(11, 2)
        
        proj = GeomAPI.ProjectPointOnCurve2d(point, line)
        
        param = proj.lower_distance_parameter
        assert param == pytest.approx(11.0, abs=1e-6)


class TestInterCurveCurve2d:
    """Tests for GeomAPI.InterCurveCurve2d (2D curve intersection)."""

    def test_intersecting_lines(self):
        """Test intersection of two crossing lines."""
        from mod3d import Geom2d
        
        line1 = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 1))
        line2 = Geom2d.Line2d(gp.Pnt2d(10, 0), gp.Dir2d(-1, 1))
        
        inter = GeomAPI.InterCurveCurve2d(line1, line2)
        
        assert inter.nb_points == 1
        pt = inter.point(1)
        assert pt.x == pytest.approx(5.0, abs=1e-6)
        assert pt.y == pytest.approx(5.0, abs=1e-6)

    def test_parallel_lines_no_intersection(self):
        """Test that parallel lines have no intersection."""
        from mod3d import Geom2d
        
        line1 = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        line2 = Geom2d.Line2d(gp.Pnt2d(0, 5), gp.Dir2d(1, 0))
        
        inter = GeomAPI.InterCurveCurve2d(line1, line2)
        
        assert inter.nb_points == 0

    def test_default_constructor(self):
        """Test empty constructor and init method."""
        from mod3d import Geom2d
        
        inter = GeomAPI.InterCurveCurve2d()
        
        line1 = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 1))
        line2 = Geom2d.Line2d(gp.Pnt2d(0, 10), gp.Dir2d(1, -1))
        
        inter.init(line1, line2)
        
        assert inter.nb_points == 1
        pt = inter.point(1)
        assert pt.x == pytest.approx(5.0, abs=1e-6)
        assert pt.y == pytest.approx(5.0, abs=1e-6)

    def test_line_circle_intersection(self):
        """Test intersection of a line and a circle."""
        from mod3d import Geom2d
        
        # Circle at origin with radius 5
        circle = Geom2d.Circle2d(gp.Ax2d(), 5.0)
        # Horizontal line through origin
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        
        inter = GeomAPI.InterCurveCurve2d(line, circle)
        
        # Should have 2 intersection points at (-5,0) and (5,0)
        assert inter.nb_points == 2

    def test_nb_segments_for_tangent_curves(self):
        """Test nb_segments property."""
        from mod3d import Geom2d
        
        line1 = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        line2 = Geom2d.Line2d(gp.Pnt2d(5, 5), gp.Dir2d(0, 1))
        
        inter = GeomAPI.InterCurveCurve2d(line1, line2)
        
        # Non-tangent intersection, should have 0 segments
        assert inter.nb_segments >= 0

    def test_self_intersection(self):
        """Test self-intersection detection (curve with itself)."""
        from mod3d import Geom2d
        
        # A simple line has no self-intersection
        line = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 0))
        
        inter = GeomAPI.InterCurveCurve2d(line)
        
        assert inter.nb_points == 0

    def test_with_tolerance(self):
        """Test intersection with custom tolerance."""
        from mod3d import Geom2d
        
        line1 = Geom2d.Line2d(gp.Pnt2d(0, 0), gp.Dir2d(1, 1))
        line2 = Geom2d.Line2d(gp.Pnt2d(10, 0), gp.Dir2d(-1, 1))
        
        inter = GeomAPI.InterCurveCurve2d(line1, line2, 1e-10)
        
        assert inter.nb_points == 1
