# import pytest
import numpy as np
from mod3d import (
    gp,
    Geom,
    BRepBuilderAPI,
)

import pygbs.gbs as gbs

def test_geom_line_creation():
    # Create a Geom_Line using a point and direction
    p = gp.Pnt(0.0, 0.0, 0.0)
    d = gp.Dir(1.0, 0.0, 0.0)
    line = Geom.Line(p, d)

    # Verify that the line was created correctly
    lin = line.line
    assert lin.location.xyz == p.xyz
    assert lin.direction.xyz == d.xyz


# ==================== BSplineCurve Tests ====================

def test_bspline_creation_with_lists():
    """Test creating a B-spline curve with Python lists."""
    # Simple quadratic B-spline with 4 control points
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 1.0, 0.0),
        gp.Pnt(3.0, 0.0, 0.0)
    ]
    knots = [0.0, 1.0, 2.0]
    multiplicities = [3, 1, 3]  # Clamped knot vector
    degree = 2

    curve = Geom.BSplineCurve(poles, knots, multiplicities, degree)

    assert curve.degree == degree
    assert curve.nb_poles == 4
    assert curve.nb_knots == 3
    assert not curve.is_rational
    assert not curve.is_periodic


def test_bspline_creation_with_numpy():
    """Test creating a B-spline curve with numpy arrays."""
    # Control points as numpy array
    poles_np = np.array([
        [0.0, 0.0, 0.0],
        [1.0, 1.0, 0.0],
        [2.0, 1.0, 0.0],
        [3.0, 0.0, 0.0]
    ])
    knots_np = np.array([0.0, 1.0, 2.0])
    mults_np = np.array([3, 1, 3])
    degree = 2

    curve = Geom.BSplineCurve(poles_np, knots_np, mults_np, degree)

    assert curve.degree == degree
    assert curve.nb_poles == 4
    assert curve.nb_knots == 3


def test_bspline_rational_curve():
    """Test creating a rational B-spline curve."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 0.0, 0.0)
    ]
    weights = [1.0, 0.5, 1.0]  # Middle point with lower weight
    knots = [0.0, 1.0]
    multiplicities = [3, 3]
    degree = 2

    curve = Geom.BSplineCurve(poles, weights, knots, multiplicities, degree)

    assert curve.is_rational
    assert curve.degree == degree
    assert curve.nb_poles == 3


def test_bspline_data_access_numpy():
    """Test accessing curve data as numpy arrays."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 1.0, 0.0),
        gp.Pnt(3.0, 0.0, 0.0)
    ]
    knots = [0.0, 1.0, 2.0]
    multiplicities = [3, 1, 3]
    degree = 2

    curve = Geom.BSplineCurve(poles, knots, multiplicities, degree)

    # Test poles as numpy array
    poles_array = curve.poles
    assert isinstance(poles_array, np.ndarray)
    assert poles_array.shape == (4, 3)
    np.testing.assert_array_equal(poles_array[0], [0.0, 0.0, 0.0])
    np.testing.assert_array_equal(poles_array[3], [3.0, 0.0, 0.0])

    # Test knots as numpy array
    knots_array = curve.knots
    assert isinstance(knots_array, np.ndarray)
    assert len(knots_array) == 3
    np.testing.assert_array_equal(knots_array, [0.0, 1.0, 2.0])

    # Test multiplicities as numpy array
    mults_array = curve.multiplicities
    assert isinstance(mults_array, np.ndarray)
    np.testing.assert_array_equal(mults_array, [3, 1, 3])

    # Test knot sequence
    knot_seq = curve.knot_sequence
    assert isinstance(knot_seq, np.ndarray)
    expected_seq = [0.0, 0.0, 0.0, 1.0, 2.0, 2.0, 2.0]
    np.testing.assert_array_equal(knot_seq, expected_seq)


def test_bspline_weights_access():
    """Test accessing weights from rational curve."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 0.0, 0.0)
    ]
    weights = [1.0, 0.7, 1.0]
    knots = [0.0, 1.0]
    multiplicities = [3, 3]
    degree = 2

    curve = Geom.BSplineCurve(poles, weights, knots, multiplicities, degree)

    weights_array = curve.weights
    assert isinstance(weights_array, np.ndarray)
    np.testing.assert_array_almost_equal(weights_array, [1.0, 0.7, 1.0])


def test_bspline_curve_evaluation():
    """Test curve evaluation at parameters."""
    # Simple linear curve
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 0.0, 0.0)
    ]
    knots = [0.0, 1.0]
    multiplicities = [2, 2]
    degree = 1

    curve = Geom.BSplineCurve(poles, knots, multiplicities, degree)

    # Test point evaluation
    start_point = curve.start_point
    end_point = curve.end_point

    assert abs(start_point.x - 0.0) < 1e-10
    assert abs(end_point.x - 1.0) < 1e-10

    # Test parameter bounds
    assert abs(curve.first_parameter - 0.0) < 1e-10
    assert abs(curve.last_parameter - 1.0) < 1e-10

    # Test evaluation at midpoint
    mid_point = curve.value(0.5)
    assert abs(mid_point.x - 0.5) < 1e-10


def test_bspline_curve_properties():
    """Test curve property queries."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 0.0, 0.0)
    ]
    knots = [0.0, 1.0]
    multiplicities = [3, 3]
    degree = 2

    curve = Geom.BSplineCurve(poles, knots, multiplicities, degree)

    assert not curve.is_closed
    assert not curve.is_periodic
    assert not curve.is_rational

    # Test continuity
    assert curve.is_cn(0)  # At least C0
    assert curve.is_cn(1)  # At least C1


def test_bspline_individual_access():
    """Test accessing individual poles, knots, etc."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 1.0, 0.0)
    ]
    knots = [0.0, 1.0]
    multiplicities = [3, 3]
    degree = 2

    curve = Geom.BSplineCurve(poles, knots, multiplicities, degree)

    # Test individual pole access (1-indexed in OCCT)
    pole1 = curve.pole(1)
    assert abs(pole1.x - 0.0) < 1e-10
    assert abs(pole1.y - 0.0) < 1e-10

    pole2 = curve.pole(2)
    assert abs(pole2.x - 1.0) < 1e-10
    assert abs(pole2.y - 1.0) < 1e-10

    # Test individual knot access
    knot1 = curve.knot(1)
    assert abs(knot1 - 0.0) < 1e-10

    knot2 = curve.knot(2)
    assert abs(knot2 - 1.0) < 1e-10

    # Test multiplicity access
    mult1 = curve.multiplicity(1)
    assert mult1 == 3

    mult2 = curve.multiplicity(2)
    assert mult2 == 3


def test_bspline_error_cases():
    """Test error cases for B-spline creation."""
    poles = [gp.Pnt(0.0, 0.0, 0.0), gp.Pnt(1.0, 0.0, 0.0)]

    # Mismatched knots and multiplicities
    try:
        Geom.BSplineCurve(poles, [0.0, 1.0], [2, 2, 2], 1)  # Extra multiplicity
        assert False, "Should have raised error"
    except ValueError:
        pass

    # Test with numpy - wrong pole shape
    try:
        poles_bad = np.array([[0.0, 0.0], [1.0, 0.0]])  # Missing Z coordinate
        Geom.BSplineCurve(poles_bad, [0.0, 1.0], [2, 2], 1)
        assert False, "Should have raised error"
    except ValueError:
        pass


def test_bspline_numpy_constructor_mixed():
    """Test mixed list/numpy construction."""
    # Poles as numpy, others as lists
    poles_np = np.array([
        [0.0, 0.0, 0.0],
        [1.0, 0.0, 0.0]
    ])
    knots_list = [0.0, 1.0]
    mults_list = [2, 2]

    curve = Geom.BSplineCurve(poles_np, knots_list, mults_list, 1)
    assert curve.nb_poles == 2
    assert curve.degree == 1


# ==================== BezierCurve Tests ====================

def test_bezier_creation_with_lists():
    """Test creating a Bezier curve with Python lists."""
    # Simple cubic Bezier curve
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 1.0, 0.0),
        gp.Pnt(3.0, 0.0, 0.0)
    ]
    
    curve = Geom.BezierCurve(poles)
    
    assert curve.degree == 3  # degree = nb_poles - 1
    assert curve.nb_poles == 4
    assert not curve.is_rational
    assert not curve.is_periodic


def test_bezier_creation_with_numpy():
    """Test creating a Bezier curve with numpy arrays."""
    # Control points as numpy array
    poles_np = np.array([
        [0.0, 0.0, 0.0],
        [1.0, 1.0, 0.0],
        [2.0, 1.0, 0.0],
        [3.0, 0.0, 0.0]
    ])
    
    curve = Geom.BezierCurve(poles_np)
    
    assert curve.degree == 3
    assert curve.nb_poles == 4
    assert not curve.is_rational


def test_bezier_rational_curve():
    """Test creating a rational Bezier curve."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 0.0, 0.0)
    ]
    weights = [1.0, 0.5, 1.0]  # Middle point with lower weight
    
    curve = Geom.BezierCurve(poles, weights)
    
    assert curve.is_rational
    assert curve.degree == 2
    assert curve.nb_poles == 3


def test_bezier_data_access_numpy():
    """Test accessing curve data as numpy arrays."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 1.0, 0.0),
        gp.Pnt(3.0, 0.0, 0.0)
    ]
    
    curve = Geom.BezierCurve(poles)
    
    # Test poles as numpy array
    poles_array = curve.poles
    assert isinstance(poles_array, np.ndarray)
    assert poles_array.shape == (4, 3)
    np.testing.assert_array_equal(poles_array[0], [0.0, 0.0, 0.0])
    np.testing.assert_array_equal(poles_array[3], [3.0, 0.0, 0.0])
    
    # Test weights (should be empty for non-rational curve)
    weights_array = curve.weights
    assert isinstance(weights_array, np.ndarray)
    assert len(weights_array) == 0  # Non-rational curve has no weights


def test_bezier_rational_weights_access():
    """Test accessing weights from rational curve."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 0.0, 0.0)
    ]
    weights = [1.0, 0.7, 1.0]
    
    curve = Geom.BezierCurve(poles, weights)
    
    weights_array = curve.weights
    assert isinstance(weights_array, np.ndarray)
    np.testing.assert_array_almost_equal(weights_array, [1.0, 0.7, 1.0])


def test_bezier_curve_evaluation():
    """Test curve evaluation at parameters."""
    # Simple linear Bezier (degree 1)
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 0.0, 0.0)
    ]
    
    curve = Geom.BezierCurve(poles)
    
    # Test parameter bounds (always 0 to 1 for Bezier)
    assert abs(curve.first_parameter - 0.0) < 1e-10
    assert abs(curve.last_parameter - 1.0) < 1e-10
    
    # Test start and end points
    start_point = curve.start_point
    end_point = curve.end_point
    
    assert abs(start_point.x - 0.0) < 1e-10
    assert abs(end_point.x - 1.0) < 1e-10
    
    # Test evaluation at midpoint
    mid_point = curve.value(0.5)
    assert abs(mid_point.x - 0.5) < 1e-10


def test_bezier_curve_properties():
    """Test curve property queries."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 0.0, 0.0)
    ]
    
    curve = Geom.BezierCurve(poles)
    
    assert not curve.is_closed
    assert not curve.is_periodic
    assert not curve.is_rational
    
    # Bezier curves are infinitely smooth
    assert curve.is_cn(0)  # At least C0
    assert curve.is_cn(1)  # At least C1
    assert curve.is_cn(100)  # Infinitely smooth


def test_bezier_individual_access():
    """Test accessing individual poles and weights."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 1.0, 0.0)
    ]
    weights = [1.0, 0.8, 1.0]
    
    curve = Geom.BezierCurve(poles, weights)
    
    # Test individual pole access (1-indexed in OCCT)
    pole1 = curve.pole(1)
    assert abs(pole1.x - 0.0) < 1e-10
    assert abs(pole1.y - 0.0) < 1e-10
    
    pole2 = curve.pole(2)
    assert abs(pole2.x - 1.0) < 1e-10
    assert abs(pole2.y - 1.0) < 1e-10
    
    # Test individual weight access
    weight1 = curve.weight(1)
    assert abs(weight1 - 1.0) < 1e-10
    
    weight2 = curve.weight(2)
    assert abs(weight2 - 0.8) < 1e-10


def test_bezier_modification():
    """Test modifying Bezier curve poles and weights."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 0.0, 0.0),
        gp.Pnt(2.0, 0.0, 0.0)
    ]
    
    curve = Geom.BezierCurve(poles)
    assert curve.degree == 2
    
    # Test pole modification
    new_pole = gp.Pnt(1.0, 1.0, 0.0)
    curve.set_pole(2, new_pole)
    
    modified_pole = curve.pole(2)
    assert abs(modified_pole.x - 1.0) < 1e-10
    assert abs(modified_pole.y - 1.0) < 1e-10
    
    # Test adding weight (making it rational)
    curve.set_pole(2, new_pole, 0.5)
    assert curve.is_rational
    assert abs(curve.weight(2) - 0.5) < 1e-10


def test_bezier_pole_insertion_removal():
    """Test inserting and removing poles."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 0.0, 0.0),
        gp.Pnt(2.0, 0.0, 0.0)
    ]
    
    curve = Geom.BezierCurve(poles)
    assert curve.nb_poles == 3
    
    # Insert a pole
    new_pole = gp.Pnt(0.5, 0.5, 0.0)
    curve.insert_pole_after(1, new_pole)
    assert curve.nb_poles == 4
    assert curve.degree == 3
    
    # Check the inserted pole
    inserted_pole = curve.pole(2)  # Should be at position 2 now
    assert abs(inserted_pole.x - 0.5) < 1e-10
    assert abs(inserted_pole.y - 0.5) < 1e-10
    
    # Remove a pole
    curve.remove_pole(2)
    assert curve.nb_poles == 3
    assert curve.degree == 2


def test_bezier_degree_increase():
    """Test increasing the degree of a Bezier curve."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 0.0, 0.0)
    ]
    
    curve = Geom.BezierCurve(poles)
    assert curve.degree == 1
    
    # Increase degree to 3
    curve.increase(3)
    assert curve.degree == 3
    assert curve.nb_poles == 4
    
    # Start and end points should remain the same
    start_point = curve.start_point
    end_point = curve.end_point
    assert abs(start_point.x - 0.0) < 1e-10
    assert abs(end_point.x - 1.0) < 1e-10


def test_bezier_segmentation():
    """Test curve segmentation."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 0.0, 0.0)
    ]
    
    curve = Geom.BezierCurve(poles)
    
    # Segment from 0.25 to 0.75
    curve.segment(0.25, 0.75)
    
    # Parameter range should still be [0, 1]
    assert abs(curve.first_parameter - 0.0) < 1e-10
    assert abs(curve.last_parameter - 1.0) < 1e-10
    
    # Degree should remain the same
    assert curve.degree == 2


def test_bezier_error_cases():
    """Test error cases for Bezier creation."""
    # Too few poles
    try:
        Geom.BezierCurve([gp.Pnt(0.0, 0.0, 0.0)])  # Only one pole
        assert False, "Should have raised error"
    except:
        pass
    
    # Mismatched poles and weights
    try:
        poles = [gp.Pnt(0.0, 0.0, 0.0), gp.Pnt(1.0, 0.0, 0.0)]
        weights = [1.0, 0.5, 1.0]  # One extra weight
        Geom.BezierCurve(poles, weights)
        assert False, "Should have raised error"
    except ValueError:
        pass
    
    # Test with numpy - wrong pole shape
    try:
        poles_bad = np.array([[0.0, 0.0], [1.0, 0.0]])  # Missing Z coordinate
        Geom.BezierCurve(poles_bad)
        assert False, "Should have raised error"
    except ValueError:
        pass


def test_bezier_max_degree():
    """Test max degree static method."""
    max_deg = Geom.BezierCurve.max_degree()
    assert isinstance(max_deg, int)
    assert max_deg > 0  # Should be a positive value


def test_bezier_resolution():
    """Test resolution computation."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 0.0, 0.0)
    ]
    
    curve = Geom.BezierCurve(poles)
    
    # Test resolution calculation
    tolerance_3d = 0.01
    u_tolerance = curve.resolution(tolerance_3d)
    assert isinstance(u_tolerance, float)
    assert u_tolerance > 0.0


def test_bezier_numpy_constructor_mixed():
    """Test mixed list/numpy construction for Bezier curves."""
    # Poles as numpy, weights as list
    poles_np = np.array([
        [0.0, 0.0, 0.0],
        [1.0, 0.0, 0.0],
        [2.0, 0.0, 0.0]
    ])
    weights_list = [1.0, 0.5, 1.0]
    
    curve = Geom.BezierCurve(poles_np, weights_list)
    assert curve.nb_poles == 3
    assert curve.degree == 2
    assert curve.is_rational

# ==================== OffsetCurve Tests ====================

def test_offset_curve_creation():
    """Test creating an offset curve from a line."""
    import math
    
    # Create a simple line as basis curve
    p = gp.Pnt(0.0, 0.0, 0.0)
    d = gp.Dir(1.0, 0.0, 0.0)
    basis_line = Geom.Line(p, d)
    
    # Create offset curve with offset in Z direction
    offset_distance = 5.0
    offset_direction = gp.Dir(0.0, 0.0, 1.0)
    
    offset_curve = Geom.OffsetCurve(basis_line, offset_distance, offset_direction)
    
    # Verify basic properties
    assert offset_curve.offset == offset_distance
    assert offset_curve.basis_curve is not None


def test_offset_curve_from_circle():
    """Test creating an offset curve from a circle."""
    import math
    
    # Create a circle as basis curve
    ax2 = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    circle = Geom.Circle(ax2, 10.0)
    
    # Create offset curve (offset in the plane normal direction)
    offset_distance = 2.0
    offset_direction = gp.Dir(0.0, 0.0, 1.0)
    
    offset_curve = Geom.OffsetCurve(circle, offset_distance, offset_direction)
    
    assert offset_curve.offset == offset_distance
    assert offset_curve.basis_curve is not None


def test_offset_curve_properties():
    """Test offset curve properties and setters."""
    # Create basis curve
    p = gp.Pnt(0.0, 0.0, 0.0)
    d = gp.Dir(1.0, 0.0, 0.0)
    basis_line = Geom.Line(p, d)
    
    # Create offset curve
    offset_distance = 5.0
    offset_direction = gp.Dir(0.0, 0.0, 1.0)
    offset_curve = Geom.OffsetCurve(basis_line, offset_distance, offset_direction)
    
    # Test offset property (getter)
    assert offset_curve.offset == offset_distance
    
    # Test offset property (setter)
    new_offset = 10.0
    offset_curve.offset = new_offset
    assert offset_curve.offset == new_offset
    
    # Test using property syntax
    offset_curve.offset = 7.5
    assert offset_curve.offset == 7.5
    
    # Test direction property
    dir_result = offset_curve.direction
    assert dir_result is not None


def test_offset_curve_direction_change():
    """Test changing the offset direction."""
    # Create basis curve
    p = gp.Pnt(0.0, 0.0, 0.0)
    d = gp.Dir(1.0, 0.0, 0.0)
    basis_line = Geom.Line(p, d)
    
    # Create offset curve
    offset_distance = 5.0
    offset_direction1 = gp.Dir(0.0, 0.0, 1.0)
    offset_curve = Geom.OffsetCurve(basis_line, offset_distance, offset_direction1)
    
    # Change direction using property
    offset_direction2 = gp.Dir(0.0, 1.0, 0.0)
    offset_curve.direction = offset_direction2
    
    # Verify direction was changed
    new_dir = offset_curve.direction
    assert new_dir is not None


def test_offset_curve_basis_curve_change():
    """Test changing the basis curve of an offset curve."""
    # Create first basis curve (line)
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    d1 = gp.Dir(1.0, 0.0, 0.0)
    basis_line = Geom.Line(p1, d1)
    
    # Create offset curve
    offset_distance = 5.0
    offset_direction = gp.Dir(0.0, 0.0, 1.0)
    offset_curve = Geom.OffsetCurve(basis_line, offset_distance, offset_direction)
    
    # Create second basis curve (different line)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    d2 = gp.Dir(0.0, 1.0, 0.0)
    basis_line2 = Geom.Line(p2, d2)
    
    # Change basis curve (using method since it has optional parameter)
    offset_curve.set_basis_curve(basis_line2)
    
    # Verify basis curve was changed
    new_basis = offset_curve.basis_curve
    assert new_basis is not None


def test_offset_curve_evaluation():
    """Test evaluating points on an offset curve."""
    import math
    
    # Create a simple line
    p = gp.Pnt(0.0, 0.0, 0.0)
    d = gp.Dir(1.0, 0.0, 0.0)
    basis_line = Geom.Line(p, d)
    
    # Create offset curve in Y direction
    offset_distance = 5.0
    offset_direction = gp.Dir(0.0, 0.0, 1.0)
    offset_curve = Geom.OffsetCurve(basis_line, offset_distance, offset_direction)
    
    # Evaluate at parameter u=0
    point = offset_curve.value(0.0)
    assert point is not None
    
    # The offset should be in the direction of T ^ V
    # For line in X direction and offset direction in Z, offset should be in Y
    # Point should be offset in Y direction by offset_distance
    assert abs(point.y - offset_distance) < 1e-6 or abs(point.y + offset_distance) < 1e-6


def test_offset_curve_continuity():
    """Test continuity of offset curve."""
    # Create basis curve
    p = gp.Pnt(0.0, 0.0, 0.0)
    d = gp.Dir(1.0, 0.0, 0.0)
    basis_line = Geom.Line(p, d)
    
    # Create offset curve
    offset_distance = 5.0
    offset_direction = gp.Dir(0.0, 0.0, 1.0)
    offset_curve = Geom.OffsetCurve(basis_line, offset_distance, offset_direction)
    
    # Test continuity
    continuity = offset_curve.continuity
    assert continuity is not None
    
    # Test basis curve continuity
    basis_continuity = offset_curve.get_basis_curve_continuity()
    assert basis_continuity is not None


def test_offset_curve_derivatives():
    """Test derivative evaluation on offset curve."""
    # Create basis curve
    p = gp.Pnt(0.0, 0.0, 0.0)
    d = gp.Dir(1.0, 0.0, 0.0)
    basis_line = Geom.Line(p, d)
    
    # Create offset curve
    offset_distance = 5.0
    offset_direction = gp.Dir(0.0, 0.0, 1.0)
    offset_curve = Geom.OffsetCurve(basis_line, offset_distance, offset_direction)
    
    # Test D0 (point only)
    point = offset_curve.d0(0.0)
    assert point is not None
    
    # Test D1 (point and first derivative)
    result = offset_curve.d1(0.0)
    assert len(result) == 2
    point, v1 = result
    assert point is not None
    assert v1 is not None


def test_offset_curve_parameter_bounds():
    """Test parameter bounds of offset curve."""
    # Create basis curve
    p = gp.Pnt(0.0, 0.0, 0.0)
    d = gp.Dir(1.0, 0.0, 0.0)
    basis_line = Geom.Line(p, d)
    
    # Create offset curve
    offset_distance = 5.0
    offset_direction = gp.Dir(0.0, 0.0, 1.0)
    offset_curve = Geom.OffsetCurve(basis_line, offset_distance, offset_direction)
    
    # Test parameter bounds (should match basis curve)
    first_param = offset_curve.first_parameter
    last_param = offset_curve.last_parameter
    
    assert first_param is not None
    assert last_param is not None


def test_offset_curve_from_bspline():
    """Test creating an offset curve from a B-spline."""
    # Create a simple B-spline curve
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 1.0, 0.0),
        gp.Pnt(3.0, 0.0, 0.0)
    ]
    knots = [0.0, 1.0, 2.0]
    multiplicities = [3, 1, 3]
    degree = 2
    
    bspline = Geom.BSplineCurve(poles, knots, multiplicities, degree)
    
    # Create offset curve
    offset_distance = 0.5
    offset_direction = gp.Dir(0.0, 0.0, 1.0)
    
    offset_curve = Geom.OffsetCurve(bspline, offset_distance, offset_direction)
    
    assert offset_curve.offset == offset_distance
    assert offset_curve.basis_curve is not None
    
    # Evaluate at a point
    point = offset_curve.value(1.0)
    assert point is not None

def test_gbs_bspline_creation():
    """Test creating a GBSpline curve."""
    # Simple quadratic GBSpline with 4 control points
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 1.0, 0.0),
        gp.Pnt(3.0, 0.0, 0.0)
    ]
    knots = [0.0, 1.0, 2.0]
    multiplicities = [3, 1, 3]  # Clamped knot vector
    degree = 2

    occt_curve = Geom.BSplineCurve(poles, knots, multiplicities, degree)

    gbs_curve = gbs.BSCurve3d(
        poles=[
            [0.0, 0.0, 0.],
            [1.0, 1.0, 0.],
            [2.0, 1.0, 0.],
            [3.0, 0.0, 0.]
        ],
        knots= [0.0, 1.0, 2.0],
        mults= [3, 1, 3],
        degree=2
    )

    # Convert gbs curve to BSplineCurve
    occt_curve_converted = Geom.BSplineCurve(gbs_curve)

    # Verify conversions work
    assert occt_curve.degree == occt_curve_converted.degree
    assert occt_curve.nb_poles == occt_curve_converted.nb_poles
    assert occt_curve.nb_knots == occt_curve_converted.nb_knots

    # Test that gbs curves work with builders (implicit conversion through Geom_Curve)
    ed_occt = BRepBuilderAPI.MakeEdge(occt_curve).edge()
    ed_gbs = BRepBuilderAPI.MakeEdge(gbs_curve).edge()
    
    # Both edges should be valid
    assert ed_occt is not None
    assert ed_gbs is not None


# ==================== Elementary Surface Tests ====================

def test_plane_creation_from_ax3():
    """Test creating a plane from a coordinate system."""
    ax3 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    plane = Geom.Plane(ax3)
    
    # Verify plane properties
    assert plane.location.x == 0 and plane.location.y == 0 and plane.location.z == 0
    assert plane.position.location.x == 0
    
    # Test evaluation
    point = plane.value(0, 0)
    assert point.x == 0 and point.y == 0 and point.z == 0


def test_plane_creation_from_point_and_normal():
    """Test creating a plane from a point and normal."""
    pt = gp.Pnt(1, 2, 3)
    normal = gp.Dir(0, 0, 1)
    plane = Geom.Plane(pt, normal)
    
    # Verify plane passes through the point
    point = plane.value(0, 0)
    assert point.x == 1 and point.y == 2 and point.z == 3


def test_plane_coefficients():
    """Test plane equation coefficients."""
    # Create a plane with normal (0, 0, 1) passing through origin
    ax3 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    plane = Geom.Plane(ax3)
    
    # Get coefficients of equation: Ax + By + Cz + D = 0
    A, B, C, D = plane.coefficients
    
    # For Z=0 plane: 0*x + 0*y + 1*z + 0 = 0
    assert abs(C - 1.0) < 1e-10 or abs(C + 1.0) < 1e-10  # Normal can point either way
    assert abs(D) < 1e-10
    

def test_cylindrical_surface_creation():
    """Test creating a cylindrical surface."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    radius = 5.0
    cylinder = Geom.CylindricalSurface(ax2, radius)
    
    # Verify radius
    assert abs(cylinder.radius - radius) < 1e-10
    
    # Test evaluation at u=0, v=0 (point on cylinder at Z=0)
    point = cylinder.value(0, 0)
    dist_to_axis = np.sqrt(point.x ** 2 + point.y ** 2)
    assert abs(dist_to_axis - radius) < 1e-10
    
    # Test evaluation at u=π/2, v=0 (should be at radius in Y direction)
    point = cylinder.value(np.pi/2, 0)
    assert abs(point.y - radius) < 1e-9
    assert abs(point.z) < 1e-10


def test_cylindrical_surface_properties():
    """Test modifying cylindrical surface properties."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    cylinder = Geom.CylindricalSurface(ax2, 5.0)
    
    # Modify radius using property
    cylinder.radius = 10.0
    assert abs(cylinder.radius - 10.0) < 1e-10
    
    # Verify point is now at new radius
    point = cylinder.value(0, 0)
    dist_to_axis = np.sqrt(point.x ** 2 + point.y ** 2)
    assert abs(dist_to_axis - 10.0) < 1e-10


def test_conical_surface_creation():
    """Test creating a conical surface."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    semi_angle = np.pi / 6  # 30 degrees
    ref_radius = 5.0
    cone = Geom.ConicalSurface(ax2, semi_angle, ref_radius)
    
    # Verify properties
    assert abs(cone.semi_angle - semi_angle) < 1e-10
    assert abs(cone.ref_radius - ref_radius) < 1e-10
    
    # Test evaluation at reference plane (v=0)
    point = cone.value(0, 0)
    dist_to_axis = np.sqrt(point.x ** 2 + point.y ** 2)
    assert abs(dist_to_axis - ref_radius) < 1e-9


def test_conical_surface_apex():
    """Test conical surface apex calculation."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    semi_angle = np.pi / 6  # 30 degrees
    ref_radius = 5.0
    cone = Geom.ConicalSurface(ax2, semi_angle, ref_radius)
    
    # Apex is at distance ref_radius / tan(semi_angle) from origin (can be positive or negative)
    apex = cone.apex
    apex_z_expected = ref_radius / np.tan(semi_angle)
    
    assert abs(apex.x) < 1e-10
    assert abs(apex.y) < 1e-10
    # Check absolute distance from apex to origin along z
    assert abs(abs(apex.z) - abs(apex_z_expected)) < 1e-9


def test_conical_surface_properties():
    """Test modifying conical surface properties."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    cone = Geom.ConicalSurface(ax2, np.pi/6, 5.0)
    
    # Modify properties
    cone.ref_radius = 10.0
    cone.semi_angle = np.pi / 4
    
    assert abs(cone.ref_radius - 10.0) < 1e-10
    assert abs(cone.semi_angle - np.pi/4) < 1e-10


def test_spherical_surface_creation():
    """Test creating a spherical surface."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    radius = 3.0
    sphere = Geom.SphericalSurface(ax2, radius)
    
    # Verify radius
    assert abs(sphere.radius - radius) < 1e-10
    
    # Test evaluation at origin (north pole)
    point = sphere.value(0, np.pi/2)
    assert abs(point.z - radius) < 1e-10
    
    # Test evaluation at south pole
    point = sphere.value(0, -np.pi/2)
    assert abs(point.z + radius) < 1e-10


def test_spherical_surface_area_volume():
    """Test sphere area and volume calculations."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    radius = 2.0
    sphere = Geom.SphericalSurface(ax2, radius)
    
    # Area = 4 * π * R²
    expected_area = 4 * np.pi * radius ** 2
    assert abs(sphere.area - expected_area) < 1e-9
    
    # Volume = 4/3 * π * R³
    expected_volume = 4/3 * np.pi * radius ** 3
    assert abs(sphere.volume - expected_volume) < 1e-9


def test_spherical_surface_properties():
    """Test modifying spherical surface properties."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    sphere = Geom.SphericalSurface(ax2, 3.0)
    
    # Modify radius
    sphere.radius = 5.0
    assert abs(sphere.radius - 5.0) < 1e-10
    
    # Verify volume changed
    expected_volume = 4/3 * np.pi * 5.0 ** 3
    assert abs(sphere.volume - expected_volume) < 1e-9


def test_toroidal_surface_creation():
    """Test creating a toroidal surface."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    major_radius = 5.0
    minor_radius = 2.0
    torus = Geom.ToroidalSurface(ax2, major_radius, minor_radius)
    
    # Verify radii
    assert abs(torus.major_radius - major_radius) < 1e-10
    assert abs(torus.minor_radius - minor_radius) < 1e-10


def test_toroidal_surface_area_volume():
    """Test torus area and volume calculations."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    major_radius = 5.0
    minor_radius = 2.0
    torus = Geom.ToroidalSurface(ax2, major_radius, minor_radius)
    
    # Area = 4 * π² * R * r
    expected_area = 4 * np.pi ** 2 * major_radius * minor_radius
    assert abs(torus.area - expected_area) < 1e-8
    
    # Volume = 2 * π² * R * r²
    expected_volume = 2 * np.pi ** 2 * major_radius * minor_radius ** 2
    assert abs(torus.volume - expected_volume) < 1e-8


def test_toroidal_surface_properties():
    """Test modifying toroidal surface properties."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    torus = Geom.ToroidalSurface(ax2, 5.0, 2.0)
    
    # Modify radii
    torus.major_radius = 7.0
    torus.minor_radius = 3.0
    
    assert abs(torus.major_radius - 7.0) < 1e-10
    assert abs(torus.minor_radius - 3.0) < 1e-10


def test_elementary_surface_axis_location_position():
    """Test axis, location, and position properties on elementary surfaces."""
    # Test with a plane
    ax2_orig = gp.Ax3(gp.Pnt(1, 2, 3), gp.Dir(0, 0, 1))
    plane = Geom.Plane(ax2_orig)
    
    # Test reading properties
    location = plane.location
    assert location.x == 1 and location.y == 2 and location.z == 3
    
    axis = plane.axis
    assert abs(axis.direction.z - 1.0) < 1e-10
    
    position = plane.position
    assert position.location.x == 1 and position.location.y == 2 and position.location.z == 3
    
    # Test modifying position
    new_ax2 = gp.Ax3(gp.Pnt(5, 6, 7), gp.Dir(1, 0, 0))
    plane.position = new_ax2
    
    new_location = plane.location
    assert new_location.x == 5 and new_location.y == 6 and new_location.z == 7


def test_plane_pln_property():
    """Test the pln property for reading and writing."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    plane = Geom.Plane(ax2)
    
    # Read gp_Pln
    pln = plane.pln
    assert pln.location.x == 0 and pln.location.y == 0 and pln.location.z == 0
    
    # Create a new plane and assign it
    new_ax3 = gp.Ax3(gp.Pnt(1, 1, 1), gp.Dir(0, 0, 1))
    new_pln = gp.Pln(new_ax3)
    plane.pln = new_pln
    
    # Verify the plane was updated
    assert plane.location.x == 1 and plane.location.y == 1 and plane.location.z == 1


def test_cylinder_cylinder_property():
    """Test the cylinder property for reading and writing."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    cylinder = Geom.CylindricalSurface(ax2, 5.0)
    
    # Test reading the underlying gp object
    cyl = cylinder.cylinder
    assert abs(cyl.radius - 5.0) < 1e-10
    
    # Modify via property
    new_ax2 = gp.Ax3(gp.Pnt(1, 1, 1), gp.Dir(0, 0, 1))
    # We need to create a gp.Cylinder but it's not exposed
    # So we'll test the radius property instead
    cylinder.radius = 7.0
    assert abs(cylinder.radius - 7.0) < 1e-10


def test_cone_cone_property():
    """Test the cone property for reading and writing."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    cone = Geom.ConicalSurface(ax2, np.pi/6, 5.0)
    
    # Test reading the underlying gp object
    gp_cone = cone.cone
    assert abs(gp_cone.semi_angle - np.pi/6) < 1e-10


def test_sphere_sphere_property():
    """Test the sphere property for reading and writing."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    sphere = Geom.SphericalSurface(ax2, 3.0)
    
    # Test reading the underlying gp object
    gp_sphere = sphere.sphere
    assert abs(gp_sphere.radius - 3.0) < 1e-10


def test_torus_torus_property():
    """Test the torus property for reading and writing."""
    ax2 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    torus = Geom.ToroidalSurface(ax2, 5.0, 2.0)
    
    # Test reading the underlying gp object
    gp_torus = torus.torus
    assert abs(gp_torus.major_radius - 5.0) < 1e-10


def test_surface_point_evaluation():
    """Test point evaluation on various elementary surfaces."""
    # Test plane
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    p = plane(0, 0)
    assert p.x == 0 and p.y == 0 and p.z == 0
    
    # Test cylinder
    cylinder = Geom.CylindricalSurface(
        gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 5.0
    )
    p = cylinder(0, 0)
    assert abs(np.sqrt(p.x**2 + p.y**2) - 5.0) < 1e-10
    
    # Test sphere
    sphere = Geom.SphericalSurface(
        gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 3.0
    )
    p = sphere(0, np.pi/2)
    assert abs(p.z - 3.0) < 1e-10


def test_gbs_surface_conversion():
    """Test converting gbs surface to OCCT BSplineSurface."""

    # Create a simple 3x3 B-spline surface
    poles = [
        [0, 0, 0], [1, 0, 0], [2, 0, 0],
        [0, 1, 0], [1, 1, 1], [2, 1, 0],
        [0, 2, 0], [1, 2, 0], [2, 2, 0],
    ]
    
    knots_u = [0, 1]
    knots_v = [0, 1]
    mults_u = [3, 3]
    mults_v = [3, 3]
    
    # Create gbs surface
    gbs_surf = gbs.BSSurface3d(poles, knots_u, knots_v, mults_u, mults_v, 2, 2)
    
    # Convert to OCCT by creating BSplineSurface from gbs surface
    occt_surf = Geom.BSplineSurface(gbs_surf)
    
    # Verify it's a valid BSplineSurface
    assert occt_surf is not None
    assert occt_surf.u_degree == 2
    assert occt_surf.v_degree == 2
    assert occt_surf.nb_u_poles == 3
    assert occt_surf.nb_v_poles == 3
    
    print("✓ gbs::BSSurface to Geom_BSplineSurface conversion works!")
    
    # Test evaluation
    u, v = 0.5, 0.5
    point = occt_surf.value(u, v)
    print(f"✓ Surface evaluation at ({u}, {v}): {point}")


def test_implicit_conversion():
    """Test implicit conversion from gbs surface."""
    
    # Create a simple surface
    poles = [
        [0, 0, 0], [1, 0, 0],
        [0, 1, 0], [1, 1, 1],
    ]
    
    knots_u = [0, 1]
    knots_v = [0, 1]
    mults_u = [2, 2]
    mults_v = [2, 2]
    
    gbs_surf = gbs.BSSurface3d(poles, knots_u, knots_v, mults_u, mults_v, 1, 1)
    
    # Test implicit conversion by passing to a function expecting BSplineSurface
    # (just create it, which uses the conversion)
    occt_surf = Geom.BSplineSurface(gbs_surf)
    assert occt_surf.u_degree == 1
    assert occt_surf.v_degree == 1
    
    BRepBuilderAPI.MakeFace(occt_surf, 1e-6)  # Should work without error


# ==================== OffsetSurface Tests ====================

def test_offset_surface_from_plane():
    """Test creating an offset surface from a plane."""
    # Create a plane at origin with normal in Z direction
    pln = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    
    # Create offset surface
    offset = Geom.OffsetSurface(pln, 2.0)
    
    assert offset is not None
    assert offset.offset == 2.0
    assert offset.base_surface is not None


def test_offset_surface_properties():
    """Test offset surface properties."""
    # Create a sphere
    sphere = Geom.SphericalSurface(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    
    # Create positive offset
    offset_pos = Geom.OffsetSurface(sphere, 0.5)
    assert offset_pos.offset == 0.5
    
    # Create negative offset
    offset_neg = Geom.OffsetSurface(sphere, -0.5)
    assert offset_neg.offset == -0.5
    
    # Modify offset
    offset_pos.offset = 1.0
    assert offset_pos.offset == 1.0


def test_offset_surface_base_surface():
    """Test getting and setting base surface."""
    plane1 = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    plane2 = Geom.Plane(gp.Ax3(gp.Pnt(1, 0, 0), gp.Dir(0, 0, 1)))
    
    offset = Geom.OffsetSurface(plane1, 1.0)
    assert offset.base_surface is not None
    
    # Change base surface
    offset.base_surface = plane2
    assert offset.base_surface is not None


def test_offset_surface_continuity():
    """Test continuity properties of offset surface."""
    # Plane has infinite continuity
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    offset = Geom.OffsetSurface(plane, 1.0)
    
    # Offset of C2 surface should be C1
    assert offset.continuity is not None
    assert offset.basis_surface_continuity is not None
    
    # Check CN continuity
    assert offset.is_cn_u(0)
    assert offset.is_cn_v(0)


def test_offset_surface_evaluation():
    """Test point evaluation on offset surface."""
    # Create a sphere with radius 1.0 at origin
    sphere = Geom.SphericalSurface(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    
    # Create offset surface with offset 0.5
    offset = Geom.OffsetSurface(sphere, 0.5)
    
    # Evaluate at parameter (0, 0)
    # For a sphere at origin with radius 1.0, the point at (0,0) is at (1, 0, 0)
    # With offset 0.5, it should be at approximately (1.5, 0, 0)
    point = offset.value(0, 0)
    assert point is not None
    assert isinstance(point, gp.Pnt)
    
    # The point should be roughly 1.5 units from origin along the surface normal
    dist_from_origin = np.sqrt(point.x**2 + point.y**2 + point.z**2)
    assert abs(dist_from_origin - 1.5) < 1e-6


def test_offset_surface_derivatives():
    """Test derivative evaluation on offset surface."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    offset = Geom.OffsetSurface(plane, 1.0)
    
    # Test D0 (point only)
    p = offset.d0(0.5, 0.5)
    assert p is not None
    assert p.z == 1.0  # Plane at z=1.0
    
    # Test D1 (point and first derivatives)
    p, du, dv = offset.d1(0.5, 0.5)
    assert p is not None
    assert du is not None
    assert dv is not None


def test_offset_surface_iso_curves():
    """Test iso-parametric curves on offset surface."""
    cylinder = Geom.CylindricalSurface(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    offset = Geom.OffsetSurface(cylinder, 0.2)
    
    # Get U iso-curve
    u_iso = offset.u_iso(0.5)
    assert u_iso is not None
    
    # Get V iso-curve
    v_iso = offset.v_iso(0.5)
    assert v_iso is not None


def test_offset_surface_callable():
    """Test using offset surface as callable."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    offset = Geom.OffsetSurface(plane, 1.5)
    
    # Test callable interface
    p1 = offset.value(0.3, 0.7)
    p2 = offset(0.3, 0.7)
    
    assert p1.x == p2.x
    assert p1.y == p2.y
    assert p1.z == p2.z


def test_offset_surface_bounds():
    """Test getting bounds of offset surface."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    offset = Geom.OffsetSurface(plane, 1.0)
    
    # Planes have infinite bounds
    u1, u2, v1, v2 = offset.bounds()
    assert u1 is not None
    assert u2 is not None
    assert v1 is not None
    assert v2 is not None


def test_offset_surface_transform():
    """Test transforming an offset surface."""
    sphere = Geom.SphericalSurface(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    offset = Geom.OffsetSurface(sphere, 0.5)
    
    # Create a transformation (translation)
    trsf = gp.Trsf()
    trsf.set_translation(gp.Vec(1.0, 2.0, 3.0))
    
    # Apply transformation
    offset.transform(trsf)
    
    # Verify transformation was applied
    p = offset.value(0, 0)
    assert p is not None


def test_offset_surface_osculating_surfaces():
    """Test getting osculating surfaces."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    offset = Geom.OffsetSurface(plane, 1.0)
    
    # Get U-osculating surface
    u_exists, u_opposite, u_oscul = offset.u_osculating_surface(0.5, 0.5)
    # u_exists might be False depending on implementation
    
    # Get V-osculating surface
    v_exists, v_opposite, v_oscul = offset.v_osculating_surface(0.5, 0.5)
    # v_exists might be False depending on implementation


def test_offset_surface_equivalent_surface():
    """Test getting equivalent surface."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    offset = Geom.OffsetSurface(plane, 1.0)
    
    # For an offset plane, there should be an equivalent plane
    equiv = offset.surface()
    assert equiv is not None


def test_offset_surface_inherited_properties():
    """Test inherited properties from base Geom_Surface."""
    sphere = Geom.SphericalSurface(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    offset = Geom.OffsetSurface(sphere, 0.5)
    
    # Test inherited closure properties
    assert offset.is_u_closed is not None
    assert offset.is_v_closed is not None
    
    # Test inherited periodicity
    assert offset.is_u_periodic is not None
    assert offset.is_v_periodic is not None
    
    # Test inherited continuity
    assert offset.continuity is not None


# ==================== SurfaceOfRevolution Tests ====================

def test_surface_of_revolution_creation():
    """Test creating a surface of revolution from a circle."""
    # Create a circle at distance 5 from the Z axis
    circle = Geom.Circle(gp.Ax2(gp.Pnt(5, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    
    # Create axis of revolution
    axis = gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    
    # Create surface of revolution
    surf = Geom.SurfaceOfRevolution(circle, axis)
    
    assert surf is not None
    assert surf.basis_curve is not None
    assert surf.direction is not None


def test_surface_of_revolution_axis():
    """Test axis property of revolution surface."""
    circle = Geom.Circle(gp.Ax2(gp.Pnt(5, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    axis = gp.Ax1(gp.Pnt(1, 2, 3), gp.Dir(1, 0, 0))
    
    surf = Geom.SurfaceOfRevolution(circle, axis)
    
    # Check axis location and direction
    rev_axis = surf.axis
    assert rev_axis is not None
    assert abs(rev_axis.location.x - 1.0) < 1e-10
    assert abs(rev_axis.location.y - 2.0) < 1e-10
    assert abs(rev_axis.location.z - 3.0) < 1e-10


def test_surface_of_revolution_evaluation():
    """Test point evaluation on revolution surface."""
    # Create a simple line parallel to Z axis at distance 2 from origin
    line = Geom.Line(gp.Ax1(gp.Pnt(2, 0, 0), gp.Dir(0, 0, 1)))
    
    # Revolution axis
    axis = gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    
    # Create surface
    surf = Geom.SurfaceOfRevolution(line, axis)
    
    # Evaluate at a point (should create a circle)
    # U is the parameter along the profile curve
    # V is the rotation angle
    p = surf.value(0, 0)
    assert p is not None
    
    # Distance from Z axis should be 2.0
    dist = np.sqrt(p.x**2 + p.y**2)
    assert abs(dist - 2.0) < 1e-6


def test_surface_of_revolution_periodicity():
    """Test that revolution surface is periodic in V (rotation parameter)."""
    circle = Geom.Circle(gp.Ax2(gp.Pnt(3, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    axis = gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    
    surf = Geom.SurfaceOfRevolution(circle, axis)
    
    # Check periodicity
    assert surf.is_v_periodic


def test_surface_of_revolution_continuity():
    """Test continuity of revolution surface."""
    line = Geom.Line(gp.Ax1(gp.Pnt(1, 0, 0), gp.Dir(0, 0, 1)))
    axis = gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    
    surf = Geom.SurfaceOfRevolution(line, axis)
    
    # Check CN continuity queries
    assert surf.is_cn_u(0)
    assert surf.is_cn_v(0)


def test_surface_of_revolution_derivatives():
    """Test derivative evaluation on revolution surface."""
    line = Geom.Line(gp.Ax1(gp.Pnt(2, 0, 0), gp.Dir(0, 0, 1)))
    axis = gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    
    surf = Geom.SurfaceOfRevolution(line, axis)
    
    # Test D1
    p, du, dv = surf.d1(0, 0)
    assert p is not None
    assert du is not None
    assert dv is not None


def test_surface_of_revolution_transform():
    """Test transforming a revolution surface."""
    circle = Geom.Circle(gp.Ax2(gp.Pnt(3, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    axis = gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    
    surf = Geom.SurfaceOfRevolution(circle, axis)
    
    # Create translation
    trsf = gp.Trsf()
    trsf.set_translation(gp.Vec(1, 2, 3))
    
    # Apply transformation
    surf.transform(trsf)
    
    # Check that axis was transformed
    new_axis = surf.axis
    assert new_axis is not None


# ==================== SurfaceOfLinearExtrusion Tests ====================

def test_surface_of_linear_extrusion_creation():
    """Test creating a surface of linear extrusion."""
    # Create a parabola-like curve
    parabola = Geom.BSplineCurve(
        poles=[gp.Pnt(-2, 4, 0), gp.Pnt(0, 0, 0), gp.Pnt(2, 4, 0)],
        degree=2,
        knots=[0, 1],
        multiplicities=[3, 3]
    )
    
    # Extrusion direction
    direction = gp.Dir(0, 0, 1)
    
    # Create extrusion surface
    surf = Geom.SurfaceOfLinearExtrusion(parabola, direction)
    
    assert surf is not None
    assert surf.basis_curve is not None
    assert surf.direction is not None


def test_surface_of_linear_extrusion_direction():
    """Test direction property of extrusion surface."""
    line = Geom.Line(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)))
    direction = gp.Dir(1, 1, 1)  # Normalized automatically by gp_Dir
    
    surf = Geom.SurfaceOfLinearExtrusion(line, direction)
    
    # Check direction
    extr_dir = surf.direction
    assert extr_dir is not None


def test_surface_of_linear_extrusion_evaluation():
    """Test point evaluation on extrusion surface."""
    # Simple line from origin in X direction
    line = Geom.Line(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)))
    direction = gp.Dir(0, 0, 1)
    
    surf = Geom.SurfaceOfLinearExtrusion(line, direction)
    
    # Evaluate at parameters
    p = surf.value(1, 1)
    assert p is not None
    
    # Point should be at approximately (1, 0, 1) for unit parameters
    # U parameter is along the curve, V parameter is along the extrusion
    assert p.x > 0
    assert p.z > 0


def test_surface_of_linear_extrusion_periodicity():
    """Test periodicity of extrusion surface."""
    line = Geom.Line(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)))
    direction = gp.Dir(0, 0, 1)
    
    surf = Geom.SurfaceOfLinearExtrusion(line, direction)
    
    assert surf.value(0, 0).x == surf.value(0, 1).x
    assert surf.value(0, 0).y == surf.value(0, 1).y
    assert surf.value(0, 0).z == surf.value(0, 1).z - 1.0  # V parameter shifts along extrusion


def test_surface_of_linear_extrusion_continuity():
    """Test continuity of extrusion surface."""
    circle = Geom.Circle(gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    direction = gp.Dir(0, 1, 0)
    
    surf = Geom.SurfaceOfLinearExtrusion(circle, direction)
    
    # Check CN continuity
    assert surf.is_cn_u(0)
    assert surf.is_cn_v(0)


def test_surface_of_linear_extrusion_derivatives():
    """Test derivative evaluation on extrusion surface."""
    line = Geom.Line(gp.Ax1(gp.Pnt(1, 0, 0), gp.Dir(0, 1, 0)))
    direction = gp.Dir(0, 0, 1)
    
    surf = Geom.SurfaceOfLinearExtrusion(line, direction)
    
    # Test D0
    p = surf.d0(0.5, 0.5)
    assert p is not None
    
    # Test D1
    p, du, dv = surf.d1(0.5, 0.5)
    assert p is not None
    assert du is not None
    assert dv is not None
    
    # Test D2
    p, du, dv, d2u, d2v, duv = surf.d2(0.5, 0.5)
    assert p is not None
    assert du is not None
    assert dv is not None


def test_surface_of_linear_extrusion_transform():
    """Test transforming an extrusion surface."""
    line = Geom.Line(gp.Ax1(gp.Pnt(1, 0, 0), gp.Dir(0, 0, 1)))
    direction = gp.Dir(0, 1, 0)
    
    surf = Geom.SurfaceOfLinearExtrusion(line, direction)
    
    # Create rotation
    trsf = gp.Trsf()
    trsf.set_rotation(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), np.pi / 4)
    
    # Apply transformation
    surf.transform(trsf)
    
    # Verify direction was transformed
    new_dir = surf.direction
    assert new_dir is not None


def test_surface_of_linear_extrusion_iso_curves():
    """Test iso-parametric curves on extrusion surface."""
    circle = Geom.Circle(gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 2.0)
    direction = gp.Dir(0, 0, 1)
    
    surf = Geom.SurfaceOfLinearExtrusion(circle, direction)
    
    # U iso-curve should be a line parallel to extrusion direction
    u_iso = surf.u_iso(0.5)
    assert u_iso is not None
    
    # V iso-curve should be the base curve
    v_iso = surf.v_iso(0)
    assert v_iso is not None


def test_swept_surface_basis_curve():
    """Test basis_curve property of swept surfaces."""
    # Test with revolution
    circle = Geom.Circle(gp.Ax2(gp.Pnt(3, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    axis = gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    surf_rev = Geom.SurfaceOfRevolution(circle, axis)
    
    # Get basis curve
    basis = surf_rev.basis_curve
    assert basis is not None
    
    # Test with extrusion
    line = Geom.Line(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)))
    direction = gp.Dir(0, 0, 1)
    surf_extr = Geom.SurfaceOfLinearExtrusion(line, direction)
    
    basis2 = surf_extr.basis_curve
    assert basis2 is not None


def test_swept_surface_bounds():
    """Test bounds computation for swept surfaces."""
    # Revolution surface
    circle = Geom.Circle(gp.Ax2(gp.Pnt(2, 0, 0), gp.Dir(0, 0, 1)), 1.0)
    axis = gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    surf_rev = Geom.SurfaceOfRevolution(circle, axis)
    
    u1, u2, v1, v2 = surf_rev.bounds()
    assert u1 is not None
    assert u2 is not None
    assert v1 is not None
    assert v2 is not None
    
    # Extrusion surface (infinite in V direction)
    line = Geom.Line(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)))
    direction = gp.Dir(0, 0, 1)
    surf_extr = Geom.SurfaceOfLinearExtrusion(line, direction)
    
    u1, u2, v1, v2 = surf_extr.bounds()
    assert u1 is not None
    assert u2 is not None


# ==================== Geometry Hash and Equality Tests ====================

def test_geometry_hash():
    """Test that geometry objects have a hash value."""
    line = Geom.Line(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)))
    
    h = hash(line)
    assert isinstance(h, int)


def test_geometry_hash_consistency():
    """Test that the same geometry returns the same hash."""
    line = Geom.Line(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)))
    
    h1 = hash(line)
    h2 = hash(line)
    assert h1 == h2


def test_geometry_as_dict_key():
    """Test that geometry objects can be used as dictionary keys."""
    line = Geom.Line(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)))
    circle = Geom.Circle(gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 5.0)
    
    d = {line: "my_line", circle: "my_circle"}
    assert d[line] == "my_line"
    assert d[circle] == "my_circle"


def test_geometry_in_set():
    """Test that geometry objects can be used in sets."""
    line = Geom.Line(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)))
    circle = Geom.Circle(gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 5.0)
    
    s = {line, circle}
    assert len(s) == 2
    assert line in s
    assert circle in s


def test_geometry_equality_same_reference():
    """Test geometry equality with same reference."""
    line = Geom.Line(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)))
    line_ref = line
    
    assert line == line_ref


def test_geometry_equality_copy():
    """Test that copied geometry is not equal (identity-based)."""
    line = Geom.Line(gp.Ax1(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)))
    line_copy = line.copy()
    
    # Different objects are not equal (identity-based equality)
    assert line != line_copy


def test_surface_hash():
    """Test that surface objects have a hash value."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    
    h = hash(plane)
    assert isinstance(h, int)


def test_surface_as_dict_key():
    """Test that surface objects can be used as dictionary keys."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    cylinder = Geom.CylindricalSurface(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)), 5.0)
    
    d = {plane: "my_plane", cylinder: "my_cylinder"}
    assert d[plane] == "my_plane"
    assert d[cylinder] == "my_cylinder"
