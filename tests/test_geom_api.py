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
