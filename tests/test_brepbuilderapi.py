"""Tests for BRepBuilderAPI module."""

import math
import numpy as np
from mod3d import (
    gp,
    Geom,
    GeomFill,
    BRepBuilderAPI,
)


def test_make_pipe_basic():
    """Test basic pipe creation with straight spine and circular profile."""
    # Create a straight spine (wire)
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(0.0, 0.0, 10.0)
    edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    spine = BRepBuilderAPI.MakeWire(edge).wire()
    
    # Create circular profile
    circle = Geom.Circle(gp.Ax2(), 2.0)  # Circle with radius 2
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # Create pipe
    pipe_maker = BRepBuilderAPI.MakePipe(spine, profile)
    assert pipe_maker.is_done()
    
    pipe_shape = pipe_maker.shape()
    assert not pipe_shape.is_null()
    
    # Test sweep methods
    first_shape = pipe_maker.first_shape()
    last_shape = pipe_maker.last_shape()
    assert not first_shape.is_null()
    assert not last_shape.is_null()


def test_make_pipe_with_trihedron():
    """Test pipe creation with specified trihedron mode."""
    # Create helical spine
    helix = Geom.BSplineCurve(
        poles=np.array([
            [0.0, 0.0, 0.0],
            [2.0, 0.0, 2.0],
            [0.0, 2.0, 4.0],
            [-2.0, 0.0, 6.0],
            [0.0, -2.0, 8.0],
            [2.0, 0.0, 10.0]
        ]),
        knots=[0., 1., 2., 3.],
        multiplicities=[4, 1, 1, 4],
        degree=3  # Cubic curve
    )
    spine_edge = BRepBuilderAPI.MakeEdge(helix).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    # Create square profile
    profile_points = [
        gp.Pnt(-0.5, -0.5, 0.0),
        gp.Pnt(0.5, -0.5, 0.0),
        gp.Pnt(0.5, 0.5, 0.0),
        gp.Pnt(-0.5, 0.5, 0.0),
        gp.Pnt(-0.5, -0.5, 0.0)  # Close the profile
    ]
    
    wire_maker = BRepBuilderAPI.MakeWire()
    for i in range(len(profile_points) - 1):
        edge = BRepBuilderAPI.MakeEdge(profile_points[i], profile_points[i + 1]).edge()
        wire_maker.add(edge)
    
    profile = wire_maker.wire()
    
    # Create pipe with Frenet trihedron
    pipe_maker = BRepBuilderAPI.MakePipe(
        spine, profile, GeomFill.Trihedron.Frenet
    )
    assert pipe_maker.is_done()
    
    pipe_shape = pipe_maker.shape()
    assert not pipe_shape.is_null()


def test_make_pipe_with_force_approx_c1():
    """Test pipe creation with C1 approximation."""
    # Create curved spine
    points = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(5.0, 0.0, 2.0),
        gp.Pnt(10.0, 5.0, 4.0),
        gp.Pnt(15.0, 5.0, 6.0)
    ]
    
    wire_maker = BRepBuilderAPI.MakeWire()
    for i in range(len(points) - 1):
        edge = BRepBuilderAPI.MakeEdge(points[i], points[i + 1]).edge()
        wire_maker.add(edge)
    
    spine = wire_maker.wire()
    
    # Create elliptical profile
    ellipse = Geom.Ellipse(gp.Ax2(), 3.0, 1.5)  # Major radius 3, minor radius 1.5
    profile_edge = BRepBuilderAPI.MakeEdge(ellipse).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # Create pipe with C1 approximation
    pipe_maker = BRepBuilderAPI.MakePipe(
        spine, profile, GeomFill.Trihedron.CorrectedFrenet, True  # Force C1 approximation
    )
    assert pipe_maker.is_done()
    
    pipe_shape = pipe_maker.shape()
    assert not pipe_shape.is_null()


def test_make_pipe_methods():
    """Test additional MakePipe methods."""
    # Create simple pipe
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    spine_edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    circle = Geom.Circle(gp.Ax2(), 1.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    pipe_maker = BRepBuilderAPI.MakePipe(spine, profile)
    assert pipe_maker.is_done()
    
    # Test pipe method (access to internal BRepFill_Pipe)
    pipe_obj = pipe_maker.pipe()
    assert pipe_obj is not None
    
    # Test error_on_surface method
    error = pipe_maker.error_on_surface()
    assert isinstance(error, float)
    assert error >= 0.0
    
    # Test generated method (though specific implementation may vary)
    # This tests the binding works even if result might be empty
    try:
        generated = pipe_maker.generated(spine_edge, profile_edge)
        # The result might be a valid shape or null depending on internal logic
    except Exception as e:
        # Some combinations might not be valid, which is OK for testing the binding
        pass


def test_make_pipe_complex_profile():
    """Test pipe with complex multi-edge profile."""
    # Create L-shaped spine
    points = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(5.0, 0.0, 0.0),
        gp.Pnt(5.0, 5.0, 0.0)
    ]
    
    wire_maker = BRepBuilderAPI.MakeWire()
    for i in range(len(points) - 1):
        edge = BRepBuilderAPI.MakeEdge(points[i], points[i + 1]).edge()
        wire_maker.add(edge)
    
    spine = wire_maker.wire()
    
    # Create triangular profile
    triangle_points = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 0.0, 0.0),
        gp.Pnt(0.5, 1.0, 0.0),
        gp.Pnt(0.0, 0.0, 0.0)  # Close triangle
    ]
    
    profile_maker = BRepBuilderAPI.MakeWire()
    for i in range(len(triangle_points) - 1):
        edge = BRepBuilderAPI.MakeEdge(triangle_points[i], triangle_points[i + 1]).edge()
        profile_maker.add(edge)
    
    profile = profile_maker.wire()
    
    # Create pipe
    pipe_maker = BRepBuilderAPI.MakePipe(spine, profile)
    assert pipe_maker.is_done()
    
    pipe_shape = pipe_maker.shape()
    assert not pipe_shape.is_null()


def test_make_pipe_errors():
    """Test error handling in pipe creation."""
    # Test with invalid/empty spine
    empty_wire = BRepBuilderAPI.MakeWire().wire()
    
    circle = Geom.Circle(gp.Ax2(), 1.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # This should create a pipe maker but likely not be done
    pipe_maker = BRepBuilderAPI.MakePipe(empty_wire, profile)
    # The exact behavior depends on OpenCASCADE's validation
    # but the binding should not crash
    
    # Test is_done functionality works regardless
    done = pipe_maker.is_done()
    assert isinstance(done, bool)


def test_trihedron_enum_values():
    """Test GeomFill.Trihedron enum values are accessible."""
    # Test all enum values are available
    assert hasattr(GeomFill.Trihedron, 'Frenet')
    assert hasattr(GeomFill.Trihedron, 'CorrectedFrenet')
    assert hasattr(GeomFill.Trihedron, 'Fixed')
    assert hasattr(GeomFill.Trihedron, 'ConstantNormal')
    assert hasattr(GeomFill.Trihedron, 'Darboux')
    
    # Test enum values can be used
    trihedron = GeomFill.Trihedron.Frenet
    assert trihedron is not None


# BRepPrimAPI_MakePrism Tests

def test_make_prism_basic():
    """Test basic prism creation with vector."""
    # Create a rectangular face to extrude
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    # Create edges and wire
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    # Create prism by extruding along Z axis
    vec = gp.Vec(0.0, 0.0, 15.0)
    prism_maker = BRepBuilderAPI.MakePrism(face, vec)
    
    assert prism_maker.is_done()
    prism_shape = prism_maker.shape()
    assert not prism_shape.is_null()
    
    # Test first and last shapes
    first = prism_maker.first_shape()
    last = prism_maker.last_shape()
    assert not first.is_null()
    assert not last.is_null()


def test_make_prism_with_copy():
    """Test prism creation with copy option."""
    # Create a circle edge
    circle = Geom.Circle(gp.Ax2(), 3.0)
    edge = BRepBuilderAPI.MakeEdge(circle).edge()
    wire = BRepBuilderAPI.MakeWire(edge).wire()
    
    # Create prism with copy
    vec = gp.Vec(0.0, 0.0, 10.0)
    prism_maker = BRepBuilderAPI.MakePrism(wire, vec, copy=True)
    
    assert prism_maker.is_done()
    shape = prism_maker.shape()
    assert not shape.is_null()


def test_make_prism_infinite():
    """Test infinite prism creation."""
    # Create a triangular profile
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(3.0, 0.0, 0.0)
    p3 = gp.Pnt(1.5, 2.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3).wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    # Create infinite prism
    direction = gp.Dir(0.0, 1.0, 1.0)  # Diagonal direction
    prism_maker = BRepBuilderAPI.MakePrism(face, direction, infinite=True)
    
    assert prism_maker.is_done()
    shape = prism_maker.shape()
    assert not shape.is_null()


def test_make_prism_semi_infinite():
    """Test semi-infinite prism creation."""
    # Create a square wire
    points = [
        gp.Pnt(-1.0, -1.0, 0.0),
        gp.Pnt(1.0, -1.0, 0.0),
        gp.Pnt(1.0, 1.0, 0.0),
        gp.Pnt(-1.0, 1.0, 0.0),
        gp.Pnt(-1.0, -1.0, 0.0)
    ]
    
    wire_maker = BRepBuilderAPI.MakeWire()
    for i in range(len(points) - 1):
        edge = BRepBuilderAPI.MakeEdge(points[i], points[i + 1]).edge()
        wire_maker.add(edge)
    
    wire = wire_maker.wire()
    
    # Create semi-infinite prism
    direction = gp.Dir(1.0, 0.0, 0.0)
    prism_maker = BRepBuilderAPI.MakePrism(wire, direction, infinite=False)
    
    assert prism_maker.is_done()
    shape = prism_maker.shape()
    assert not shape.is_null()


def test_make_prism_methods():
    """Test MakePrism methods."""
    # Create a simple face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 0.0, 0.0)
    p3 = gp.Pnt(5.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    vec = gp.Vec(0.0, 0.0, 8.0)
    prism_maker = BRepBuilderAPI.MakePrism(face, vec)
    
    # Test prism method
    prism_obj = prism_maker.prism()
    assert prism_obj is not None
    
    # Test first_shape and last_shape with edge parameter
    first_edge = prism_maker.first_shape(e1)
    last_edge = prism_maker.last_shape(e1)
    assert not first_edge.is_null()
    assert not last_edge.is_null()
    
    # Test is_deleted
    deleted = prism_maker.is_deleted(e1)
    assert isinstance(deleted, bool)


# BRepPrimAPI_MakeRevol Tests

def test_make_revol_basic():
    """Test basic revolution creation."""
    # Create a rectangular profile to revolve
    p1 = gp.Pnt(5.0, 0.0, 0.0)
    p2 = gp.Pnt(8.0, 0.0, 0.0)
    p3 = gp.Pnt(8.0, 0.0, 10.0)
    p4 = gp.Pnt(5.0, 0.0, 10.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    # Revolve around Z axis
    axis = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    angle = math.pi  # 180 degrees
    
    revol_maker = BRepBuilderAPI.MakeRevol(face, axis, angle)
    assert revol_maker.is_done()
    
    revol_shape = revol_maker.shape()
    assert not revol_shape.is_null()
    
    # Test first and last shapes
    first = revol_maker.first_shape()
    last = revol_maker.last_shape()
    assert not first.is_null()
    assert not last.is_null()


def test_make_revol_full_rotation():
    """Test revolution with full 2*Pi rotation."""
    # Create a trapezoidal profile
    p1 = gp.Pnt(3.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 0.0, 0.0)
    p3 = gp.Pnt(4.5, 0.0, 6.0)
    p4 = gp.Pnt(3.5, 0.0, 6.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    # Full revolution (2*Pi is default)
    axis = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    revol_maker = BRepBuilderAPI.MakeRevol(face, axis)
    
    assert revol_maker.is_done()
    shape = revol_maker.shape()
    assert not shape.is_null()


def test_make_revol_with_copy():
    """Test revolution with copy option."""
    # Create a circular edge for profile
    circle = Geom.Circle(gp.Ax2(gp.Pnt(5.0, 0.0, 5.0), gp.Dir(0.0, 1.0, 0.0)), 2.0)
    edge = BRepBuilderAPI.MakeEdge(circle).edge()
    wire = BRepBuilderAPI.MakeWire(edge).wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    # Revolve with copy
    axis = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    angle = 2.0 * math.pi / 3.0  # 120 degrees
    
    revol_maker = BRepBuilderAPI.MakeRevol(face, axis, angle, copy=True)
    assert revol_maker.is_done()
    
    shape = revol_maker.shape()
    assert not shape.is_null()


def test_make_revol_different_axis():
    """Test revolution around different axes."""
    # Create a square profile
    p1 = gp.Pnt(0.0, 3.0, 0.0)
    p2 = gp.Pnt(0.0, 5.0, 0.0)
    p3 = gp.Pnt(0.0, 5.0, 2.0)
    p4 = gp.Pnt(0.0, 3.0, 2.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    # Revolve around X axis
    axis = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(1.0, 0.0, 0.0))
    angle = math.pi / 2.0  # 90 degrees
    
    revol_maker = BRepBuilderAPI.MakeRevol(face, axis, angle)
    assert revol_maker.is_done()
    
    shape = revol_maker.shape()
    assert not shape.is_null()


def test_make_revol_methods():
    """Test MakeRevol methods."""
    # Create a simple edge profile
    p1 = gp.Pnt(4.0, 0.0, 0.0)
    p2 = gp.Pnt(6.0, 0.0, 0.0)
    p3 = gp.Pnt(6.0, 0.0, 8.0)
    p4 = gp.Pnt(4.0, 0.0, 8.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    axis = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    angle = math.pi
    
    revol_maker = BRepBuilderAPI.MakeRevol(face, axis, angle)
    assert revol_maker.is_done()
    
    # Test revol method
    revol_obj = revol_maker.revol()
    assert revol_obj is not None
    
    # Test first_shape and last_shape with edge parameter
    first_edge = revol_maker.first_shape(e1)
    last_edge = revol_maker.last_shape(e1)
    assert not first_edge.is_null()
    assert not last_edge.is_null()
    
    # Test is_deleted
    deleted = revol_maker.is_deleted(e1)
    assert isinstance(deleted, bool)
    
    # Test has_degenerated
    has_degen = revol_maker.has_degenerated()
    assert isinstance(has_degen, bool)
    
    # Test degenerated list (may be empty)
    degen_list = revol_maker.degenerated()
    assert isinstance(degen_list, list)


def test_make_revol_wire():
    """Test revolving a wire (generates shell)."""
    # Create an L-shaped wire
    p1 = gp.Pnt(5.0, 0.0, 0.0)
    p2 = gp.Pnt(8.0, 0.0, 0.0)
    p3 = gp.Pnt(8.0, 0.0, 3.0)
    p4 = gp.Pnt(6.0, 0.0, 3.0)
    p5 = gp.Pnt(6.0, 0.0, 6.0)
    p6 = gp.Pnt(5.0, 0.0, 6.0)
    
    wire_maker = BRepBuilderAPI.MakeWire()
    points = [p1, p2, p3, p4, p5, p6, p1]
    for i in range(len(points) - 1):
        edge = BRepBuilderAPI.MakeEdge(points[i], points[i + 1]).edge()
        wire_maker.add(edge)
    
    wire = wire_maker.wire()
    
    # Revolve the wire
    axis = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    angle = math.pi / 2.0
    
    revol_maker = BRepBuilderAPI.MakeRevol(wire, axis, angle)
    assert revol_maker.is_done()
    
    shape = revol_maker.shape()
    assert not shape.is_null()


def test_make_prism_edge():
    """Test extruding an edge (generates face)."""
    # Create a single edge
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 0.0, 0.0)
    edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    
    # Extrude the edge
    vec = gp.Vec(0.0, 3.0, 0.0)
    prism_maker = BRepBuilderAPI.MakePrism(edge, vec)
    
    assert prism_maker.is_done()
    shape = prism_maker.shape()
    assert not shape.is_null()


def test_make_revol_edge():
    """Test revolving an edge (generates face)."""
    # Create a single edge
    p1 = gp.Pnt(3.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 0.0, 5.0)
    edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    
    # Revolve the edge
    axis = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    angle = 2.0 * math.pi
    
    revol_maker = BRepBuilderAPI.MakeRevol(edge, axis, angle)
    assert revol_maker.is_done()
    
    shape = revol_maker.shape()
    assert not shape.is_null()
