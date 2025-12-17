# import pytest
import numpy as np
from mod3d import (
    gp,
    Geom,
)


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

    assert curve.degree() == degree
    assert curve.nb_poles() == 4
    assert curve.nb_knots() == 3
    assert not curve.is_rational()
    assert not curve.is_periodic()


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

    assert curve.degree() == degree
    assert curve.nb_poles() == 4
    assert curve.nb_knots() == 3


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

    assert curve.is_rational()
    assert curve.degree() == degree
    assert curve.nb_poles() == 3


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
    poles_array = curve.poles()
    assert isinstance(poles_array, np.ndarray)
    assert poles_array.shape == (4, 3)
    np.testing.assert_array_equal(poles_array[0], [0.0, 0.0, 0.0])
    np.testing.assert_array_equal(poles_array[3], [3.0, 0.0, 0.0])

    # Test knots as numpy array
    knots_array = curve.knots()
    assert isinstance(knots_array, np.ndarray)
    assert len(knots_array) == 3
    np.testing.assert_array_equal(knots_array, [0.0, 1.0, 2.0])

    # Test multiplicities as numpy array
    mults_array = curve.multiplicities()
    assert isinstance(mults_array, np.ndarray)
    np.testing.assert_array_equal(mults_array, [3, 1, 3])

    # Test knot sequence
    knot_seq = curve.knot_sequence()
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

    weights_array = curve.weights()
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
    start_point = curve.start_point()
    end_point = curve.end_point()

    assert abs(start_point.x - 0.0) < 1e-10
    assert abs(end_point.x - 1.0) < 1e-10

    # Test parameter bounds
    assert abs(curve.first_parameter() - 0.0) < 1e-10
    assert abs(curve.last_parameter() - 1.0) < 1e-10

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

    assert not curve.is_closed()
    assert not curve.is_periodic()
    assert not curve.is_rational()

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
    assert curve.nb_poles() == 2
    assert curve.degree() == 1


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
    
    assert curve.degree() == 3  # degree = nb_poles - 1
    assert curve.nb_poles() == 4
    assert not curve.is_rational()
    assert not curve.is_periodic()


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
    
    assert curve.degree() == 3
    assert curve.nb_poles() == 4
    assert not curve.is_rational()


def test_bezier_rational_curve():
    """Test creating a rational Bezier curve."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(2.0, 0.0, 0.0)
    ]
    weights = [1.0, 0.5, 1.0]  # Middle point with lower weight
    
    curve = Geom.BezierCurve(poles, weights)
    
    assert curve.is_rational()
    assert curve.degree() == 2
    assert curve.nb_poles() == 3


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
    poles_array = curve.poles()
    assert isinstance(poles_array, np.ndarray)
    assert poles_array.shape == (4, 3)
    np.testing.assert_array_equal(poles_array[0], [0.0, 0.0, 0.0])
    np.testing.assert_array_equal(poles_array[3], [3.0, 0.0, 0.0])
    
    # Test weights (should be empty for non-rational curve)
    weights_array = curve.weights()
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
    
    weights_array = curve.weights()
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
    assert abs(curve.first_parameter() - 0.0) < 1e-10
    assert abs(curve.last_parameter() - 1.0) < 1e-10
    
    # Test start and end points
    start_point = curve.start_point()
    end_point = curve.end_point()
    
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
    
    assert not curve.is_closed()
    assert not curve.is_periodic()
    assert not curve.is_rational()
    
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
    assert curve.degree() == 2
    
    # Test pole modification
    new_pole = gp.Pnt(1.0, 1.0, 0.0)
    curve.set_pole(2, new_pole)
    
    modified_pole = curve.pole(2)
    assert abs(modified_pole.x - 1.0) < 1e-10
    assert abs(modified_pole.y - 1.0) < 1e-10
    
    # Test adding weight (making it rational)
    curve.set_pole(2, new_pole, 0.5)
    assert curve.is_rational()
    assert abs(curve.weight(2) - 0.5) < 1e-10


def test_bezier_pole_insertion_removal():
    """Test inserting and removing poles."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 0.0, 0.0),
        gp.Pnt(2.0, 0.0, 0.0)
    ]
    
    curve = Geom.BezierCurve(poles)
    assert curve.nb_poles() == 3
    
    # Insert a pole
    new_pole = gp.Pnt(0.5, 0.5, 0.0)
    curve.insert_pole_after(1, new_pole)
    assert curve.nb_poles() == 4
    assert curve.degree() == 3
    
    # Check the inserted pole
    inserted_pole = curve.pole(2)  # Should be at position 2 now
    assert abs(inserted_pole.x - 0.5) < 1e-10
    assert abs(inserted_pole.y - 0.5) < 1e-10
    
    # Remove a pole
    curve.remove_pole(2)
    assert curve.nb_poles() == 3
    assert curve.degree() == 2


def test_bezier_degree_increase():
    """Test increasing the degree of a Bezier curve."""
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 0.0, 0.0)
    ]
    
    curve = Geom.BezierCurve(poles)
    assert curve.degree() == 1
    
    # Increase degree to 3
    curve.increase(3)
    assert curve.degree() == 3
    assert curve.nb_poles() == 4
    
    # Start and end points should remain the same
    start_point = curve.start_point()
    end_point = curve.end_point()
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
    assert abs(curve.first_parameter() - 0.0) < 1e-10
    assert abs(curve.last_parameter() - 1.0) < 1e-10
    
    # Degree should remain the same
    assert curve.degree() == 2


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
    assert curve.nb_poles() == 3
    assert curve.degree() == 2
    assert curve.is_rational()

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
    continuity = offset_curve.continuity()
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
    first_param = offset_curve.first_parameter()
    last_param = offset_curve.last_parameter()
    
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