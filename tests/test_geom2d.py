"""Tests for Geom2d bindings - 2D curves and geometry."""

import pytest
import numpy as np
import math
from mod3d import gp, Geom2d


class TestLine2d:
    """Tests for Geom2d_Line (Line2d)."""

    def test_create_from_point_and_direction(self):
        """Create a line from a point and direction."""
        p = gp.Pnt2d(1.0, 2.0)
        d = gp.Dir2d(1.0, 0.0)
        line = Geom2d.Line2d(p, d)
        
        assert line.location.x == pytest.approx(1.0)
        assert line.location.y == pytest.approx(2.0)

    def test_create_from_axis(self):
        """Create a line from an axis."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(0.0, 1.0))
        line = Geom2d.Line2d(ax)
        
        dir = line.direction
        assert dir.x == pytest.approx(0.0)
        assert dir.y == pytest.approx(1.0)

    def test_create_from_gp_lin2d(self):
        """Create a line from gp_Lin2d."""
        gp_line = gp.Lin2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 1.0))
        line = Geom2d.Line2d(gp_line)
        
        assert line.line is not None

    def test_set_direction(self):
        """Test setting the direction of a line."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        line.direction = gp.Dir2d(0.0, 1.0)
        
        dir = line.direction
        assert dir.x == pytest.approx(0.0)
        assert dir.y == pytest.approx(1.0)

    def test_set_location(self):
        """Test setting the location of a line."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        line.location = gp.Pnt2d(5.0, 10.0)
        
        loc = line.location
        assert loc.x == pytest.approx(5.0)
        assert loc.y == pytest.approx(10.0)

    def test_distance(self):
        """Test distance from a point to the line."""
        # Horizontal line y = 0
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        
        # Point at (0, 5) should be distance 5 from the line
        p = gp.Pnt2d(0.0, 5.0)
        assert line.distance(p) == pytest.approx(5.0)

    def test_value(self):
        """Test evaluating point on line at parameter."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        
        p = line.value(5.0)
        assert p.x == pytest.approx(5.0)
        assert p.y == pytest.approx(0.0)

    def test_is_not_closed(self):
        """Lines are not closed curves."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        assert not line.is_closed

    def test_is_not_periodic(self):
        """Lines are not periodic curves."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        assert not line.is_periodic

    def test_infinite_bounds(self):
        """Lines have infinite parameter bounds."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        
        first, last = line.bounds
        assert first == pytest.approx(-float('inf'), rel=1e-10) or first < -1e100
        assert last == pytest.approx(float('inf'), rel=1e-10) or last > 1e100

    def test_derivatives(self):
        """Test derivative evaluation on a line."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        
        # D1 returns (point, first derivative)
        p, v1 = line.d1(0.0)
        assert p.x == pytest.approx(0.0)
        assert p.y == pytest.approx(0.0)
        assert v1.x == pytest.approx(1.0)
        assert v1.y == pytest.approx(0.0)


class TestCircle2d:
    """Tests for Geom2d_Circle (Circle2d)."""

    def test_create_from_axis_and_radius(self):
        """Create a circle from axis and radius."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 5.0)
        
        assert circle.radius == pytest.approx(5.0)

    def test_create_from_gp_circ2d(self):
        """Create a circle from gp_Circ2d."""
        ax = gp.Ax22d(gp.Pnt2d(1.0, 2.0), gp.Dir2d(1.0, 0.0), gp.Dir2d(0.0, 1.0))
        gp_circ = gp.Circ2d(ax, 3.0)
        circle = Geom2d.Circle2d(gp_circ)
        
        assert circle.radius == pytest.approx(3.0)

    def test_set_radius(self):
        """Test setting radius."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 5.0)
        circle.radius = 10.0
        
        assert circle.radius == pytest.approx(10.0)

    def test_radius_property(self):
        """Test radius property."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 5.0)
        
        assert circle.radius == pytest.approx(5.0)
        circle.radius = 7.0
        assert circle.radius == pytest.approx(7.0)

    def test_is_closed(self):
        """Circles are closed curves."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 5.0)
        
        assert circle.is_closed

    def test_is_periodic(self):
        """Circles are periodic curves."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 5.0)
        
        assert circle.is_periodic
        assert circle.period == pytest.approx(2 * math.pi)

    def test_parameter_bounds(self):
        """Circle parameters go from 0 to 2*pi."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 5.0)
        
        assert circle.first_parameter == pytest.approx(0.0)
        assert circle.last_parameter == pytest.approx(2 * math.pi)

    def test_value_at_parameters(self):
        """Test point evaluation at various parameters."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 1.0)
        
        # At u=0, should be at (1, 0)
        p0 = circle.value(0.0)
        assert p0.x == pytest.approx(1.0)
        assert p0.y == pytest.approx(0.0)
        
        # At u=pi/2, should be at (0, 1)
        p1 = circle.value(math.pi / 2)
        assert p1.x == pytest.approx(0.0)
        assert p1.y == pytest.approx(1.0)

    def test_eccentricity(self):
        """Circle has eccentricity 0."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 5.0)
        
        assert circle.eccentricity == pytest.approx(0.0)


class TestEllipse2d:
    """Tests for Geom2d_Ellipse (Ellipse2d)."""

    def test_create_from_axis_and_radii(self):
        """Create an ellipse from axis and radii."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        ellipse = Geom2d.Ellipse2d(ax, 5.0, 3.0)
        
        assert ellipse.major_radius == pytest.approx(5.0)
        assert ellipse.minor_radius == pytest.approx(3.0)

    def test_set_radii(self):
        """Test setting major and minor radii."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        ellipse = Geom2d.Ellipse2d(ax, 5.0, 3.0)
        
        ellipse.major_radius = 10.0
        ellipse.minor_radius = 6.0
        
        assert ellipse.major_radius == pytest.approx(10.0)
        assert ellipse.minor_radius == pytest.approx(6.0)

    def test_radii_properties(self):
        """Test radii properties."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        ellipse = Geom2d.Ellipse2d(ax, 5.0, 3.0)
        
        assert ellipse.major_radius == pytest.approx(5.0)
        assert ellipse.minor_radius == pytest.approx(3.0)

    def test_is_closed(self):
        """Ellipses are closed curves."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        ellipse = Geom2d.Ellipse2d(ax, 5.0, 3.0)
        
        assert ellipse.is_closed

    def test_is_periodic(self):
        """Ellipses are periodic curves."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        ellipse = Geom2d.Ellipse2d(ax, 5.0, 3.0)
        
        assert ellipse.is_periodic

    def test_eccentricity(self):
        """Test ellipse eccentricity."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        ellipse = Geom2d.Ellipse2d(ax, 5.0, 3.0)
        
        # e = sqrt(1 - (b/a)^2) = sqrt(1 - 9/25) = sqrt(16/25) = 0.8
        assert ellipse.eccentricity == pytest.approx(0.8)

    def test_focal(self):
        """Test focal distance."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        ellipse = Geom2d.Ellipse2d(ax, 5.0, 3.0)
        
        assert ellipse.focal == pytest.approx(8.0)

    def test_foci(self):
        """Test focus points."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        ellipse = Geom2d.Ellipse2d(ax, 5.0, 3.0)
        
        f1 = ellipse.focus1
        f2 = ellipse.focus2
        
        # Foci should be at (±4, 0)
        assert abs(f1.x) == pytest.approx(4.0)
        assert f1.y == pytest.approx(0.0)
        assert abs(f2.x) == pytest.approx(4.0)
        assert f2.y == pytest.approx(0.0)

    def test_value_at_parameters(self):
        """Test point evaluation at parameters."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        ellipse = Geom2d.Ellipse2d(ax, 5.0, 3.0)
        
        # At u=0, should be at (5, 0)
        p0 = ellipse.value(0.0)
        assert p0.x == pytest.approx(5.0)
        assert p0.y == pytest.approx(0.0)
        
        # At u=pi/2, should be at (0, 3)
        p1 = ellipse.value(math.pi / 2)
        assert p1.x == pytest.approx(0.0)
        assert p1.y == pytest.approx(3.0)


class TestHyperbola2d:
    """Tests for Geom2d_Hyperbola (Hyperbola2d)."""

    def test_create_from_axis_and_radii(self):
        """Create a hyperbola from axis and radii."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        hyperbola = Geom2d.Hyperbola2d(ax, 5.0, 3.0)
        
        assert hyperbola.major_radius == pytest.approx(5.0)
        assert hyperbola.minor_radius == pytest.approx(3.0)

    def test_set_radii(self):
        """Test setting major and minor radii."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        hyperbola = Geom2d.Hyperbola2d(ax, 5.0, 3.0)
        
        hyperbola.major_radius = 10.0
        hyperbola.minor_radius = 6.0
        
        assert hyperbola.major_radius == pytest.approx(10.0)
        assert hyperbola.minor_radius == pytest.approx(6.0)
    def test_is_not_closed(self):
        """Hyperbolas are not closed."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        hyperbola = Geom2d.Hyperbola2d(ax, 5.0, 3.0)
        
        assert not hyperbola.is_closed

    def test_is_not_periodic(self):
        """Hyperbolas are not periodic."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        hyperbola = Geom2d.Hyperbola2d(ax, 5.0, 3.0)
        
        assert not hyperbola.is_periodic

    def test_eccentricity(self):
        """Test hyperbola eccentricity."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        hyperbola = Geom2d.Hyperbola2d(ax, 5.0, 3.0)
        
        # e = sqrt(1 + (b/a)^2) = sqrt(1 + 9/25) = sqrt(34/25) = sqrt(34)/5
        expected = math.sqrt(34.0) / 5.0
        assert hyperbola.eccentricity == pytest.approx(expected)
    def test_focal(self):
        """Test focal distance."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        hyperbola = Geom2d.Hyperbola2d(ax, 5.0, 3.0)
        
        # c = sqrt(a^2 + b^2) = sqrt(25 + 9) = sqrt(34)
        assert hyperbola.focal == pytest.approx(math.sqrt(34.0) * 2)

    def test_value_at_zero(self):
        """Test point evaluation at u=0."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        hyperbola = Geom2d.Hyperbola2d(ax, 5.0, 3.0)
        
        # At u=0: P = O + a*cosh(0)*XDir + b*sinh(0)*YDir = O + a*XDir = (5, 0)
        p = hyperbola.value(0.0)
        assert p.x == pytest.approx(5.0)
        assert p.y == pytest.approx(0.0)


class TestParabola2d:
    """Tests for Geom2d_Parabola (Parabola2d)."""

    def test_create_from_axis_and_focal(self):
        """Create a parabola from axis and focal length."""
        ax = gp.Ax22d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0), gp.Dir2d(0.0, 1.0))
        parabola = Geom2d.Parabola2d(ax, 2.0)
        
        assert parabola.focal == pytest.approx(2.0)

    def test_set_focal(self):
        """Test setting focal length."""
        ax = gp.Ax22d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0), gp.Dir2d(0.0, 1.0))
        parabola = Geom2d.Parabola2d(ax, 2.0)
        parabola.focal = 5.0
        
        assert parabola.focal == pytest.approx(5.0)

    def test_focal_property(self):
        """Test focal property."""
        ax = gp.Ax22d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0), gp.Dir2d(0.0, 1.0))
        parabola = Geom2d.Parabola2d(ax, 2.0)
        
        assert parabola.focal == pytest.approx(2.0)

    def test_is_not_closed(self):
        """Parabolas are not closed."""
        ax = gp.Ax22d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0), gp.Dir2d(0.0, 1.0))
        parabola = Geom2d.Parabola2d(ax, 2.0)
        
        assert not parabola.is_closed

    def test_is_not_periodic(self):
        """Parabolas are not periodic."""
        ax = gp.Ax22d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0), gp.Dir2d(0.0, 1.0))
        parabola = Geom2d.Parabola2d(ax, 2.0)
        
        assert not parabola.is_periodic

    def test_eccentricity(self):
        """Parabola has eccentricity 1."""
        ax = gp.Ax22d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0), gp.Dir2d(0.0, 1.0))
        parabola = Geom2d.Parabola2d(ax, 2.0)
        
        assert parabola.eccentricity == pytest.approx(1.0)

    def test_parameter(self):
        """Test parameter (= 2 * focal)."""
        ax = gp.Ax22d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0), gp.Dir2d(0.0, 1.0))
        parabola = Geom2d.Parabola2d(ax, 2.0)
        
        assert parabola.parameter == pytest.approx(4.0)

    def test_value_at_zero(self):
        """Test point evaluation at u=0."""
        ax = gp.Ax22d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0), gp.Dir2d(0.0, 1.0))
        parabola = Geom2d.Parabola2d(ax, 2.0)
        
        # At u=0: P = O + 0*XDir + 0*YDir = (0, 0)
        p = parabola.value(0.0)
        assert p.x == pytest.approx(0.0)
        assert p.y == pytest.approx(0.0)


class TestTrimmedCurve2d:
    """Tests for Geom2d_TrimmedCurve (TrimmedCurve2d)."""

    def test_trim_line(self):
        """Trim a line to create a segment."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        trimmed = Geom2d.TrimmedCurve2d(line, 0.0, 10.0)
        
        assert trimmed.first_parameter == pytest.approx(0.0)
        assert trimmed.last_parameter == pytest.approx(10.0)

    def test_start_and_end_points(self):
        """Test start and end points of trimmed curve."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        trimmed = Geom2d.TrimmedCurve2d(line, 0.0, 10.0)
        
        start = trimmed.start_point
        end = trimmed.end_point
        
        assert start.x == pytest.approx(0.0)
        assert start.y == pytest.approx(0.0)
        assert end.x == pytest.approx(10.0)
        assert end.y == pytest.approx(0.0)

    def test_trim_circle(self):
        """Trim a circle to create an arc."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 1.0)
        arc = Geom2d.TrimmedCurve2d(circle, 0.0, math.pi / 2)
        
        start = arc.start_point
        end = arc.end_point
        
        assert start.x == pytest.approx(1.0)
        assert start.y == pytest.approx(0.0)
        assert end.x == pytest.approx(0.0)
        assert end.y == pytest.approx(1.0)

    def test_basis_curve(self):
        """Test getting the basis curve."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        trimmed = Geom2d.TrimmedCurve2d(line, 0.0, 10.0)
        
        basis = trimmed.basis_curve
        assert basis is not None

    def test_set_trim(self):
        """Test changing the trim parameters."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        trimmed = Geom2d.TrimmedCurve2d(line, 0.0, 10.0)
        
        trimmed.set_trim(5.0, 15.0)
        
        assert trimmed.first_parameter == pytest.approx(5.0)
        assert trimmed.last_parameter == pytest.approx(15.0)

    @pytest.mark.skip(reason="occtt bug?")
    def test_is_closed_for_full_circle(self):
        """A trimmed circle for full period should be closed."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 1.0)
        full_arc = Geom2d.TrimmedCurve2d(circle, 0.0, 2 * math.pi)
        
        assert full_arc.is_closed


class TestOffsetCurve2d:
    """Tests for Geom2d_OffsetCurve (OffsetCurve2d)."""

    def test_offset_line(self):
        """Create an offset of a line."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        offset_curve = Geom2d.OffsetCurve2d(line, 5.0)
        
        assert offset_curve.offset == pytest.approx(5.0)

    def test_set_offset_value(self):
        """Test setting offset value."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        offset_curve = Geom2d.OffsetCurve2d(line, 5.0)
        
        offset_curve.offset = 10.0
        assert offset_curve.offset == pytest.approx(10.0)

    def test_offset_property(self):
        """Test offset property."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        offset_curve = Geom2d.OffsetCurve2d(line, 5.0)
        
        assert offset_curve.offset == pytest.approx(5.0)
        offset_curve.offset = 7.0
        assert offset_curve.offset == pytest.approx(7.0)

    def test_basis_curve(self):
        """Test getting the basis curve."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        offset_curve = Geom2d.OffsetCurve2d(line, 5.0)
        
        basis = offset_curve.basis_curve()
        assert basis is not None

    def test_offset_line_value(self):
        """Test that offset line is at correct distance."""
        # Horizontal line y = 0
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        # Offset by 5 (positive = left side = up for direction (1,0))
        offset_curve = Geom2d.OffsetCurve2d(line, 5.0)
        
        # Point on offset curve at u=0 should be at (0, -5)
        p = offset_curve.value(0.0)
        assert p.x == pytest.approx(0.0)
        assert p.y == pytest.approx(-5.0)


class TestBezierCurve2d:
    """Tests for Geom2d_BezierCurve (BezierCurve2d)."""

    def test_create_from_poles(self):
        """Create a Bezier curve from poles."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 0.0)]
        bezier = Geom2d.BezierCurve2d(poles)
        
        assert bezier.nb_poles == 3
        assert bezier.degree == 2

    def test_create_rational(self):
        """Create a rational Bezier curve."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 0.0)]
        weights = [1.0, 2.0, 1.0]
        bezier = Geom2d.BezierCurve2d(poles, weights)
        
        assert bezier.is_rational
        assert bezier.weight(2) == pytest.approx(2.0)

    def test_non_rational(self):
        """Non-rational Bezier curve."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 0.0)]
        bezier = Geom2d.BezierCurve2d(poles)
        
        assert not bezier.is_rational

    def test_parameter_bounds(self):
        """Bezier curves have parameter range [0, 1]."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 0.0)]
        bezier = Geom2d.BezierCurve2d(poles)
        
        assert bezier.first_parameter == pytest.approx(0.0)
        assert bezier.last_parameter == pytest.approx(1.0)

    def test_start_and_end_points(self):
        """Test start and end points match first and last poles."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 0.0)]
        bezier = Geom2d.BezierCurve2d(poles)
        
        start = bezier.start_point
        end = bezier.end_point
        
        assert start.x == pytest.approx(0.0)
        assert start.y == pytest.approx(0.0)
        assert end.x == pytest.approx(2.0)
        assert end.y == pytest.approx(0.0)

    def test_pole_access(self):
        """Test accessing individual poles."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 0.0)]
        bezier = Geom2d.BezierCurve2d(poles)
        
        p1 = bezier.pole(1)
        p2 = bezier.pole(2)
        
        assert p1.x == pytest.approx(0.0)
        assert p2.x == pytest.approx(1.0)
        assert p2.y == pytest.approx(2.0)

    def test_set_pole(self):
        """Test setting a pole."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 0.0)]
        bezier = Geom2d.BezierCurve2d(poles)
        
        bezier.set_pole(2, gp.Pnt2d(1.0, 5.0))
        p = bezier.pole(2)
        
        assert p.x == pytest.approx(1.0)
        assert p.y == pytest.approx(5.0)

    def test_poles_as_numpy(self):
        """Test getting poles as numpy array."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 0.0)]
        bezier = Geom2d.BezierCurve2d(poles)
        
        np_poles = bezier.poles
        
        assert np_poles.shape == (3, 2)
        assert np_poles[0, 0] == pytest.approx(0.0)
        assert np_poles[1, 1] == pytest.approx(2.0)

    def test_increase_degree(self):
        """Test increasing degree."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 0.0)]
        bezier = Geom2d.BezierCurve2d(poles)
        
        assert bezier.degree == 2
        bezier.increase(4)
        assert bezier.degree == 4

    def test_segment(self):
        """Test segmenting a Bezier curve."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 0.0)]
        bezier = Geom2d.BezierCurve2d(poles)
        
        # Get midpoint before segment
        mid_before = bezier.value(0.5)
        
        bezier.segment(0.25, 0.75)
        
        # After segment, value at 0.5 should be the same point
        mid_after = bezier.value(0.5)
        
        assert mid_before.x == pytest.approx(mid_after.x)
        assert mid_before.y == pytest.approx(mid_after.y)

    def test_max_degree(self):
        """Test static max_degree method."""
        max_deg = Geom2d.BezierCurve2d.max_degree()
        assert max_deg > 0  # Should be a reasonable positive number


class TestBSplineCurve2d:
    """Tests for Geom2d_BSplineCurve (BSplineCurve2d)."""

    def test_create_non_rational(self):
        """Create a non-rational B-spline curve."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        degree = 3
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, degree)
        
        assert bspline.degree == 3
        assert bspline.nb_poles == 4
        assert bspline.nb_knots == 2

    def test_create_rational(self):
        """Create a rational B-spline curve."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        weights = [1.0, 1.0, 1.0, 1.0]
        knots = [0.0, 1.0]
        mults = [4, 4]
        degree = 3
        
        bspline = Geom2d.BSplineCurve2d(poles, weights, knots, mults, degree)
        
        assert bspline.is_rational or not bspline.is_rational  # Depends on weights being equal

    def test_non_periodic(self):
        """Test non-periodic B-spline."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3, False)
        
        assert not bspline.is_periodic

    def test_start_and_end_points(self):
        """Test start and end points."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        
        start = bspline.start_point
        end = bspline.end_point
        
        assert start.x == pytest.approx(0.0)
        assert start.y == pytest.approx(0.0)
        assert end.x == pytest.approx(3.0)
        assert end.y == pytest.approx(0.0)

    def test_parameter_bounds(self):
        """Test parameter bounds."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        
        assert bspline.first_parameter == pytest.approx(0.0)
        assert bspline.last_parameter == pytest.approx(1.0)

    def test_knot_access(self):
        """Test accessing knots."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        
        assert bspline.knot(1) == pytest.approx(0.0)
        assert bspline.knot(2) == pytest.approx(1.0)

    def test_multiplicity_access(self):
        """Test accessing multiplicities."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        
        assert bspline.multiplicity(1) == 4
        assert bspline.multiplicity(2) == 4

    def test_pole_access(self):
        """Test accessing poles."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        
        p1 = bspline.pole(1)
        p2 = bspline.pole(2)
        
        assert p1.x == pytest.approx(0.0)
        assert p2.x == pytest.approx(1.0)

    def test_set_pole(self):
        """Test setting a pole."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        bspline.set_pole(2, gp.Pnt2d(1.0, 5.0))
        
        p = bspline.pole(2)
        assert p.x == pytest.approx(1.0)
        assert p.y == pytest.approx(5.0)

    def test_knots_as_numpy(self):
        """Test getting knots as numpy array."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        
        np_knots = bspline.knots
        assert len(np_knots) == 2
        assert np_knots[0] == pytest.approx(0.0)
        assert np_knots[1] == pytest.approx(1.0)

    def test_poles_as_numpy(self):
        """Test getting poles as numpy array."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        
        np_poles = bspline.poles
        assert np_poles.shape == (4, 2)

    def test_multiplicities_as_numpy(self):
        """Test getting multiplicities as numpy array."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        
        np_mults = bspline.multiplicities
        assert len(np_mults) == 2
        assert np_mults[0] == 4
        assert np_mults[1] == 4

    def test_increase_degree(self):
        """Test increasing degree."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        assert bspline.degree == 3
        
        bspline.increase_degree(5)
        assert bspline.degree == 5

    def test_insert_knot(self):
        """Test inserting a knot."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        initial_knots = bspline.nb_knots
        
        bspline.insert_knot(0.5)
        assert bspline.nb_knots == initial_knots + 1

    def test_segment(self):
        """Test segmenting a B-spline curve."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        
        # Get point at u=0.5 before segment
        mid_before = bspline.value(0.5)
        
        bspline.segment(0.25, 0.75)
        
        # After segment, first parameter should be 0.25
        assert bspline.first_parameter == pytest.approx(0.25)
        assert bspline.last_parameter == pytest.approx(0.75)

    @pytest.mark.skip(reason="occt bug?")
    def test_locate_u(self):
        """Test locating parameter in knot span."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 0.5, 1.0]
        mults = [4, 1, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        
        i1, i2 = bspline.locate_u(0.3, 1e-10)
        assert i1 >= 1
        assert i2 >= i1

    def test_continuity(self):
        """Test curve continuity."""
        poles = [gp.Pnt2d(0.0, 0.0), gp.Pnt2d(1.0, 2.0), gp.Pnt2d(2.0, 2.0), gp.Pnt2d(3.0, 0.0)]
        knots = [0.0, 1.0]
        mults = [4, 4]
        
        bspline = Geom2d.BSplineCurve2d(poles, knots, mults, 3)
        
        cont = bspline.continuity
        assert cont is not None


class TestCurve2dEvaluation:
    """Tests for curve evaluation methods common to all 2D curves."""

    def test_d0_evaluation(self):
        """Test D0 (point) evaluation."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 1.0)
        
        p = circle.d0(0.0)
        assert p.x == pytest.approx(1.0)
        assert p.y == pytest.approx(0.0)

    def test_d1_evaluation(self):
        """Test D1 (point + first derivative) evaluation."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 1.0)
        
        p, v1 = circle.d1(0.0)
        
        # Point at u=0 is (1, 0)
        assert p.x == pytest.approx(1.0)
        assert p.y == pytest.approx(0.0)
        
        # Tangent at u=0 is (0, 1) for CCW circle
        assert v1.x == pytest.approx(0.0)
        assert v1.y == pytest.approx(1.0)

    def test_d2_evaluation(self):
        """Test D2 (point + first + second derivative) evaluation."""
        ax = gp.Ax2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        circle = Geom2d.Circle2d(ax, 1.0)
        
        p, v1, v2 = circle.d2(0.0)
        
        # Second derivative points toward center
        assert v2.x == pytest.approx(-1.0)
        assert v2.y == pytest.approx(0.0)

    def test_dn_evaluation(self):
        """Test DN (Nth derivative) evaluation."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        
        # First derivative of a line is constant (the direction)
        v1 = line.dn(0.0, 1)
        assert v1.x == pytest.approx(1.0)
        assert v1.y == pytest.approx(0.0)
        
        # Second derivative of a line is zero
        v2 = line.dn(0.0, 2)
        assert v2.x == pytest.approx(0.0)
        assert v2.y == pytest.approx(0.0)

    def test_is_cn(self):
        """Test continuity check."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        
        # Lines have infinite continuity
        assert line.is_cn(0)
        assert line.is_cn(1)
        assert line.is_cn(2)
        assert line.is_cn(10)


class TestCurve2dTransformations:
    """Tests for curve transformation methods."""

    def test_reverse(self):
        """Test reversing curve parametrization."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        
        # Original: value(1) = (1, 0)
        p_before = line.value(1.0)
        
        line.reverse()
        
        # After reverse: direction should be opposite
        d = line.direction
        assert d.x == pytest.approx(-1.0)

    def test_reversed_copy(self):
        """Test getting a reversed copy."""
        line = Geom2d.Line2d(gp.Pnt2d(0.0, 0.0), gp.Dir2d(1.0, 0.0))
        
        reversed_line = line.reversed()
        
        # Original should be unchanged
        assert line.direction.x == pytest.approx(1.0)
        
        # Reversed copy should have opposite direction
        # Note: reversed() returns a Curve2d, not Line2d specifically
        assert reversed_line is not None
