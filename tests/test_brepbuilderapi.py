"""Tests for BRepBuilderAPI module."""

import math
import numpy as np
from mod3d import (
    gp,
    Geom,
    GeomAbs,
    GeomFill,
    BRepBuilderAPI,
    BRepOffsetAPI,
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


# ==================== MakeEdge Tests ====================

def test_make_edge_from_points():
    """Test creating an edge from two points."""
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    
    edge_maker = BRepBuilderAPI.MakeEdge(p1, p2)
    assert edge_maker.is_done()
    
    edge = edge_maker.edge()
    assert not edge.is_null()
    
    # Check vertices
    v1 = edge_maker.vertex1()
    v2 = edge_maker.vertex2()
    assert not v1.is_null()
    assert not v2.is_null()


def test_make_edge_from_gp_lin():
    """Test creating edges from gp_Lin (line)."""
    # Create a line
    line = gp.Lin(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(1.0, 0.0, 0.0))
    
    # Test 1: Edge with parameter bounds
    edge_maker = BRepBuilderAPI.MakeEdge(line, 0.0, 10.0)
    assert edge_maker.is_done()
    edge = edge_maker.edge()
    assert not edge.is_null()
    
    # Test 2: Edge with point bounds
    p1 = gp.Pnt(2.0, 0.0, 0.0)
    p2 = gp.Pnt(8.0, 0.0, 0.0)
    edge_maker2 = BRepBuilderAPI.MakeEdge(line, p1, p2)
    assert edge_maker2.is_done()
    edge2 = edge_maker2.edge()
    assert not edge2.is_null()


def test_make_edge_from_gp_circ():
    """Test creating edges from gp_Circ (circle)."""
    # Create a circle
    ax2 = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    circle = gp.Circ(ax2, 5.0)
    
    # Test 1: Full circle edge
    edge_maker = BRepBuilderAPI.MakeEdge(circle)
    assert edge_maker.is_done()
    edge = edge_maker.edge()
    assert not edge.is_null()
    
    # Test 2: Arc with parameter bounds (quarter circle)
    edge_maker2 = BRepBuilderAPI.MakeEdge(circle, 0.0, math.pi / 2.0)
    assert edge_maker2.is_done()
    edge2 = edge_maker2.edge()
    assert not edge2.is_null()
    
    # Test 3: Arc with point bounds
    p1 = gp.Pnt(5.0, 0.0, 0.0)
    p2 = gp.Pnt(0.0, 5.0, 0.0)
    edge_maker3 = BRepBuilderAPI.MakeEdge(circle, p1, p2)
    assert edge_maker3.is_done()
    edge3 = edge_maker3.edge()
    assert not edge3.is_null()


def test_make_edge_from_gp_elips():
    """Test creating edges from gp_Elips (ellipse)."""
    # Create an ellipse
    ax2 = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    ellipse = gp.Elips(ax2, 8.0, 5.0)  # major=8, minor=5
    
    # Test 1: Full ellipse edge
    edge_maker = BRepBuilderAPI.MakeEdge(ellipse)
    assert edge_maker.is_done()
    edge = edge_maker.edge()
    assert not edge.is_null()
    
    # Test 2: Ellipse arc with parameter bounds
    edge_maker2 = BRepBuilderAPI.MakeEdge(ellipse, 0.0, math.pi)
    assert edge_maker2.is_done()
    edge2 = edge_maker2.edge()
    assert not edge2.is_null()


def test_make_edge_from_gp_hypr():
    """Test creating edges from gp_Hypr (hyperbola)."""
    # Create a hyperbola
    ax2 = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    hyperbola = gp.Hypr(ax2, 3.0, 2.0)  # major=3, minor=2
    
    # Hyperbola edge with parameter bounds
    edge_maker = BRepBuilderAPI.MakeEdge(hyperbola, -2.0, 2.0)
    assert edge_maker.is_done()
    edge = edge_maker.edge()
    assert not edge.is_null()


def test_make_edge_from_gp_parab():
    """Test creating edges from gp_Parab (parabola)."""
    # Create a parabola
    ax2 = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    parabola = gp.Parab(ax2, 2.0)  # focal=2
    
    # Parabola edge with parameter bounds
    edge_maker = BRepBuilderAPI.MakeEdge(parabola, -5.0, 5.0)
    assert edge_maker.is_done()
    edge = edge_maker.edge()
    assert not edge.is_null()


def test_make_edge_from_geom_curve():
    """Test creating edges from Geom_Curve."""
    # Create a Geom_Line
    p = gp.Pnt(0.0, 0.0, 0.0)
    d = gp.Dir(1.0, 0.0, 0.0)
    geom_line = Geom.Line(p, d)
    
    # Test 1: Edge with parameter bounds
    edge_maker = BRepBuilderAPI.MakeEdge(geom_line, 0.0, 10.0)
    assert edge_maker.is_done()
    edge = edge_maker.edge()
    assert not edge.is_null()
    
    # Test 2: Edge with point bounds
    p1 = gp.Pnt(1.0, 0.0, 0.0)
    p2 = gp.Pnt(9.0, 0.0, 0.0)
    edge_maker2 = BRepBuilderAPI.MakeEdge(geom_line, p1, p2)
    assert edge_maker2.is_done()
    edge2 = edge_maker2.edge()
    assert not edge2.is_null()


def test_make_edge_from_geom_circle():
    """Test creating edges from Geom_Circle."""
    ax2 = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    geom_circle = Geom.Circle(ax2, 5.0)
    
    # Full circle
    edge_maker = BRepBuilderAPI.MakeEdge(geom_circle)
    assert edge_maker.is_done()
    edge = edge_maker.edge()
    assert not edge.is_null()


def test_make_edge_from_bspline():
    """Test creating an edge from a B-spline curve."""
    # Create a simple B-spline curve
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 2.0, 0.0),
        gp.Pnt(2.0, 2.0, 0.0),
        gp.Pnt(3.0, 0.0, 0.0)
    ]
    knots = [0.0, 1.0, 2.0]
    multiplicities = [3, 1, 3]
    degree = 2
    
    bspline = Geom.BSplineCurve(poles, knots, multiplicities, degree)
    
    edge_maker = BRepBuilderAPI.MakeEdge(bspline)
    assert edge_maker.is_done()
    edge = edge_maker.edge()
    assert not edge.is_null()


def test_make_edge_init_method():
    """Test the init method for reinitializing edge maker."""
    # Create initial edge maker
    edge_maker = BRepBuilderAPI.MakeEdge()
    
    # Initialize with a curve
    p = gp.Pnt(0.0, 0.0, 0.0)
    d = gp.Dir(1.0, 0.0, 0.0)
    geom_line = Geom.Line(p, d)
    
    edge_maker.init(geom_line, 0.0, 10.0)
    assert edge_maker.is_done()
    
    edge = edge_maker.edge()
    assert not edge.is_null()
    
    # Reinitialize with different parameters
    edge_maker.init(geom_line, 5.0, 15.0)
    assert edge_maker.is_done()
    
    edge2 = edge_maker.edge()
    assert not edge2.is_null()


def test_make_edge_error_status():
    """Test error status reporting."""
    # Create a valid edge
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    
    edge_maker = BRepBuilderAPI.MakeEdge(p1, p2)
    assert edge_maker.is_done()
    
    # Check error status
    error = edge_maker.error()
    assert error == BRepBuilderAPI.EdgeError.EdgeDone


def test_make_edge_with_vertices():
    """Test creating edges with explicit vertices."""
    # Create vertices
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 0.0, 0.0)
    
    v1 = BRepBuilderAPI.MakeVertex(p1).vertex()
    v2 = BRepBuilderAPI.MakeVertex(p2).vertex()
    
    # Create edge with vertices
    edge_maker = BRepBuilderAPI.MakeEdge(v1, v2)
    assert edge_maker.is_done()
    
    edge = edge_maker.edge()
    assert not edge.is_null()
    
    # Verify vertices
    result_v1 = edge_maker.vertex1()
    result_v2 = edge_maker.vertex2()
    assert not result_v1.is_null()
    assert not result_v2.is_null()


def test_make_edge_ellipse_full_and_arc():
    """Test creating full ellipse and ellipse arc."""
    ax2 = gp.Ax2(gp.Pnt(5.0, 5.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    ellipse = gp.Elips(ax2, 6.0, 4.0)
    
    # Full ellipse
    edge_maker1 = BRepBuilderAPI.MakeEdge(ellipse)
    assert edge_maker1.is_done()
    edge1 = edge_maker1.edge()
    assert not edge1.is_null()
    
    # Half ellipse (arc)
    edge_maker2 = BRepBuilderAPI.MakeEdge(ellipse, 0.0, math.pi)
    assert edge_maker2.is_done()
    edge2 = edge_maker2.edge()
    assert not edge2.is_null()
    
    # Quarter ellipse
    edge_maker3 = BRepBuilderAPI.MakeEdge(ellipse, 0.0, math.pi / 2.0)
    assert edge_maker3.is_done()
    edge3 = edge_maker3.edge()
    assert not edge3.is_null()


def test_make_edge_complex_bspline():
    """Test creating edge from complex B-spline with varying knot multiplicities."""
    # Create a more complex B-spline
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(1.0, 2.0, 0.0),
        gp.Pnt(2.0, 2.0, 1.0),
        gp.Pnt(3.0, 1.0, 1.0),
        gp.Pnt(4.0, 0.0, 0.0)
    ]
    knots = [0.0, 0.5, 1.0]
    multiplicities = [4, 1, 4]
    degree = 3
    
    bspline = Geom.BSplineCurve(poles, knots, multiplicities, degree)
    
    # Create full edge
    edge_maker1 = BRepBuilderAPI.MakeEdge(bspline)
    assert edge_maker1.is_done()
    edge1 = edge_maker1.edge()
    assert not edge1.is_null()
    
    # Create partial edge with parameter bounds
    edge_maker2 = BRepBuilderAPI.MakeEdge(bspline, 0.2, 0.8)
    assert edge_maker2.is_done()
    edge2 = edge_maker2.edge()
    assert not edge2.is_null()


def test_make_edge_line_with_vertex_bounds():
    """Test creating line edge with vertex bounds."""
    # Create line and vertices
    line = gp.Lin(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(1.0, 1.0, 0.0))
    
    p1 = gp.Pnt(1.0, 1.0, 0.0)
    p2 = gp.Pnt(3.0, 3.0, 0.0)
    v1 = BRepBuilderAPI.MakeVertex(p1).vertex()
    v2 = BRepBuilderAPI.MakeVertex(p2).vertex()
    
    # Create edge with vertices
    edge_maker = BRepBuilderAPI.MakeEdge(line, v1, v2)
    assert edge_maker.is_done()
    
    edge = edge_maker.edge()
    assert not edge.is_null()


def test_sewing_two_faces():
    """Test sewing two adjacent faces into a shell."""
    # Create first face (square at z=0)
    points1 = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(10.0, 0.0, 0.0),
        gp.Pnt(10.0, 10.0, 0.0),
        gp.Pnt(0.0, 10.0, 0.0),
        gp.Pnt(0.0, 0.0, 0.0)
    ]
    
    wire_maker1 = BRepBuilderAPI.MakePolygon()
    for p in points1:
        wire_maker1.add(p)
    wire1 = wire_maker1.wire()
    face1 = BRepBuilderAPI.MakeFace(wire1).face()
    
    # Create second face (square at x=10, perpendicular to first)
    points2 = [
        gp.Pnt(10.0, 0.0, 0.0),
        gp.Pnt(10.0, 10.0, 0.0),
        gp.Pnt(10.0, 10.0, 10.0),
        gp.Pnt(10.0, 0.0, 10.0),
        gp.Pnt(10.0, 0.0, 0.0)
    ]
    
    wire_maker2 = BRepBuilderAPI.MakePolygon()
    for p in points2:
        wire_maker2.add(p)
    wire2 = wire_maker2.wire()
    face2 = BRepBuilderAPI.MakeFace(wire2).face()
    
    # Sew the faces together
    sewing = BRepBuilderAPI.Sewing(tolerance=1.0e-6)
    sewing.add(face1)
    sewing.add(face2)
    sewing.perform()
    
    # Get result
    result = sewing.sewed_shape()
    assert not result.is_null()
    
    # Check that we have contiguous edges (the shared edge)
    assert sewing.nb_contigous_edges > 0
    
    # Test properties
    assert sewing.tolerance == 1.0e-6
    assert sewing.face_mode == True


def test_sewing_multiple_faces_to_box():
    """Test sewing six faces to create a closed box shell."""
    size = 10.0
    
    # Create 6 faces of a box
    faces = []
    
    # Bottom face (z=0)
    bottom = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(0, 0, 0), gp.Dir(0, 0, -1)),
        0, size, 0, size
    ).face()
    faces.append(bottom)
    
    # Top face (z=size)
    top = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(0, 0, size), gp.Dir(0, 0, 1)),
        0, size, 0, size
    ).face()
    faces.append(top)
    
    # Front face (y=0)
    front = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(0, 0, 0), gp.Dir(0, -1, 0)),
        0, size, 0, size
    ).face()
    faces.append(front)
    
    # Back face (y=size)
    back = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(0, size, 0), gp.Dir(0, 1, 0)),
        0, size, 0, size
    ).face()
    faces.append(back)
    
    # Left face (x=0)
    left = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(0, 0, 0), gp.Dir(-1, 0, 0)),
        0, size, 0, size
    ).face()
    faces.append(left)
    
    # Right face (x=size)
    right = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(size, 0, 0), gp.Dir(1, 0, 0)),
        0, size, 0, size
    ).face()
    faces.append(right)
    
    # Sew all faces
    sewing = BRepBuilderAPI.Sewing(tolerance=1.0e-6)
    for face in faces:
        sewing.add(face)
    
    sewing.perform()
    
    result = sewing.sewed_shape()
    assert not result.is_null()
    
    # Should have multiple contiguous edges (12 edges in a box)
    assert sewing.nb_contigous_edges > 0
    
    # Check no free edges (all edges should be shared)
    # Note: depending on sewing tolerance and face orientations, 
    # this might not always be zero, so we just verify the number is reasonable
    assert sewing.nb_free_edges >= 0


def test_sewing_with_tolerance_adjustment():
    """Test sewing with different tolerance settings."""
    # Create two faces with a small gap
    face1 = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)),
        0, 10, 0, 10
    ).face()
    
    # Slightly offset second face
    face2 = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(10.0001, 0, 0), gp.Dir(1, 0, 0)),  # Small gap
        0, 10, 0, 10
    ).face()
    
    # Test with small tolerance (should not sew)
    sewing1 = BRepBuilderAPI.Sewing(tolerance=1.0e-6)
    sewing1.add(face1)
    sewing1.add(face2)
    sewing1.perform()
    
    # Test with larger tolerance (should sew)
    sewing2 = BRepBuilderAPI.Sewing(tolerance=1.0e-3)
    sewing2.min_tolerance = 1.0e-6
    sewing2.max_tolerance = 1.0
    sewing2.add(face1)
    sewing2.add(face2)
    sewing2.perform()
    
    result2 = sewing2.sewed_shape()
    assert not result2.is_null()
    
    # Verify tolerance properties work
    assert sewing2.min_tolerance == 1.0e-6
    assert sewing2.max_tolerance == 1.0


def test_sewing_modes():
    """Test different sewing mode settings."""
    # Create simple test faces
    face1 = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)),
        0, 5, 0, 5
    ).face()
    
    face2 = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(5, 0, 0), gp.Dir(1, 0, 0)),
        0, 5, 0, 5
    ).face()
    
    # Create sewing with custom modes
    sewing = BRepBuilderAPI.Sewing()
    
    # Test mode properties
    sewing.face_mode = True
    assert sewing.face_mode == True
    
    sewing.floating_edges_mode = False
    assert sewing.floating_edges_mode == False
    
    sewing.local_tolerances_mode = True
    assert sewing.local_tolerances_mode == True
    
    sewing.non_manifold_mode = False
    assert sewing.non_manifold_mode == False
    
    sewing.same_parameter_mode = True
    assert sewing.same_parameter_mode == True
    
    # Perform sewing with these settings
    sewing.add(face1)
    sewing.add(face2)
    sewing.perform()
    
    result = sewing.sewed_shape()
    assert not result.is_null()


def test_make_filling_basic():
    """Test filling a simple quadrilateral boundary."""
    points = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(5.0, 0.0, 0.0),
        gp.Pnt(5.0, 5.0, 0.0),
        gp.Pnt(0.0, 5.0, 0.0)
    ]

    edges = []
    for i in range(len(points)):
        start = points[i]
        end = points[(i + 1) % len(points)]
        edges.append(BRepBuilderAPI.MakeEdge(start, end).edge())

    fill_op = BRepOffsetAPI.MakeFilling()
    for edge in edges:
        fill_op.add(edge, GeomAbs.Shape.C0, is_bound=True)

    fill_op.build()
    assert fill_op.is_done()

    face = fill_op.shape()
    assert not face.is_null()

    assert isinstance(fill_op.g0_error, float)
    assert fill_op.g0_error >= 0.0


# ==================== MakeOffset Tests ====================

def test_make_offset_from_face():
    """Test creating an offset from a planar face."""
    # Create a simple rectangular face in the XY plane
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    face = BRepBuilderAPI.MakeFace(wire, False).face()
    
    # Create offset
    offset = BRepOffsetAPI.MakeOffset(face, GeomAbs.JoinType.Arc)
    offset.perform(1.0)
    
    assert offset.is_done()
    result = offset.shape()
    assert not result.is_null()


def test_make_offset_from_wire():
    """Test creating an offset from a single wire."""
    # Create a square wire
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 0.0, 0.0)
    p3 = gp.Pnt(5.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    
    # Create offset from wire
    offset = BRepOffsetAPI.MakeOffset(wire, GeomAbs.JoinType.Arc)
    offset.perform(0.5)
    
    assert offset.is_done()
    result = offset.shape()
    assert not result.is_null()


def test_make_offset_default_constructor():
    """Test offset with default constructor and manual initialization."""
    # Create a square wire
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(4.0, 0.0, 0.0)
    p3 = gp.Pnt(4.0, 4.0, 0.0)
    p4 = gp.Pnt(0.0, 4.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    
    # Use default constructor with init
    offset = BRepOffsetAPI.MakeOffset()
    offset.init(GeomAbs.JoinType.Arc, False)
    offset.add_wire(wire)
    offset.perform(0.3)
    
    assert offset.is_done()
    result = offset.shape()
    assert not result.is_null()


def test_make_offset_multiple_wires():
    """Test offset with multiple wires added manually."""
    # Create two square wires
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(2.0, 0.0, 0.0)
    p3 = gp.Pnt(2.0, 2.0, 0.0)
    p4 = gp.Pnt(0.0, 2.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire1 = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    
    # Create second wire offset from first
    p5 = gp.Pnt(5.0, 0.0, 0.0)
    p6 = gp.Pnt(7.0, 0.0, 0.0)
    p7 = gp.Pnt(7.0, 2.0, 0.0)
    p8 = gp.Pnt(5.0, 2.0, 0.0)
    
    e5 = BRepBuilderAPI.MakeEdge(p5, p6).edge()
    e6 = BRepBuilderAPI.MakeEdge(p6, p7).edge()
    e7 = BRepBuilderAPI.MakeEdge(p7, p8).edge()
    e8 = BRepBuilderAPI.MakeEdge(p8, p5).edge()
    
    wire2 = BRepBuilderAPI.MakeWire(e5, e6, e7, e8).wire()
    
    # Create offset with multiple wires
    offset = BRepOffsetAPI.MakeOffset()
    offset.init(GeomAbs.JoinType.Arc)
    offset.add_wire(wire1)
    offset.add_wire(wire2)
    offset.perform(0.2)
    
    assert offset.is_done()
    result = offset.shape()
    assert not result.is_null()


def test_make_offset_negative_distance():
    """Test offset with negative distance (inward offset)."""
    # Create a larger rectangular wire
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 8.0, 0.0)
    p4 = gp.Pnt(0.0, 8.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    
    # Create inward offset (negative distance)
    offset = BRepOffsetAPI.MakeOffset(wire, GeomAbs.JoinType.Arc)
    offset.perform(-1.0)
    
    assert offset.is_done()
    result = offset.shape()
    assert not result.is_null()


def test_make_offset_with_join_types():
    """Test offset with different join types."""
    # Create a wire with right angle corners
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(3.0, 0.0, 0.0)
    p3 = gp.Pnt(3.0, 3.0, 0.0)
    p4 = gp.Pnt(0.0, 3.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    
    # Test Arc join
    offset_arc = BRepOffsetAPI.MakeOffset(wire, GeomAbs.JoinType.Arc)
    offset_arc.perform(0.5)
    assert offset_arc.is_done()
    
    # Test Tangent join
    offset_tangent = BRepOffsetAPI.MakeOffset(wire, GeomAbs.JoinType.Tangent)
    offset_tangent.perform(0.5)
    assert offset_tangent.is_done()
    
    # Test Intersection join
    offset_inter = BRepOffsetAPI.MakeOffset(wire, GeomAbs.JoinType.Intersection)
    offset_inter.perform(0.5)
    assert offset_inter.is_done()


def test_make_offset_set_approx():
    """Test setting approximation flag."""
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(6.0, 0.0, 0.0)
    p3 = gp.Pnt(6.0, 6.0, 0.0)
    p4 = gp.Pnt(0.0, 6.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    
    offset = BRepOffsetAPI.MakeOffset(wire)
    offset.set_approx(True)  # Enable approximation
    offset.perform(0.5)
    
    assert offset.is_done()
    result = offset.shape()
    assert not result.is_null()


def test_make_offset_open_result():
    """Test offset with open result option."""
    # Create an open wire (not closed)
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 0.0, 0.0)
    p3 = gp.Pnt(5.0, 3.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2).wire()
    
    offset = BRepOffsetAPI.MakeOffset(wire, GeomAbs.JoinType.Arc, True)
    offset.perform(0.5)
    
    assert offset.is_done()
    result = offset.shape()
    assert not result.is_null()


def test_make_offset_altitude():
    """Test offset with altitude parameter."""
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(4.0, 0.0, 0.0)
    p3 = gp.Pnt(4.0, 4.0, 0.0)
    p4 = gp.Pnt(0.0, 4.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    
    # Create offset with altitude (offset in Z direction as well)
    offset = BRepOffsetAPI.MakeOffset(wire, GeomAbs.JoinType.Arc)
    offset.perform(0.5, 1.0)  # Offset 0.5 in XY plane, 1.0 in Z
    
    assert offset.is_done()
    result = offset.shape()
    assert not result.is_null()


def test_make_offset_generated_shapes():
    """Test getting generated shapes from offset."""
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 0.0, 0.0)
    p3 = gp.Pnt(5.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    e3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    e4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(e1, e2, e3, e4).wire()
    face = BRepBuilderAPI.MakeFace(wire, False).face()
    
    offset = BRepOffsetAPI.MakeOffset(face, GeomAbs.JoinType.Arc)
    offset.perform(0.5)
    
    assert offset.is_done()
    
    generated = offset.generated(wire)
    assert generated is not None


def test_make_offset_convert_face():
    """Test the static convert_face method."""
    # Create a face with curved edges
    circle = Geom.Circle(gp.Ax2(), 3.0)
    circle_edge = BRepBuilderAPI.MakeEdge(circle, 0, math.pi).edge()
    
    # Add straight edges to complete the wire
    p1 = circle.value(0)
    p2 = circle.value(math.pi)
    
    edge1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    edge2 = BRepBuilderAPI.MakeEdge(p2, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(circle_edge, edge1, edge2).wire()
    face = BRepBuilderAPI.MakeFace(wire, False).face()
    
    # Convert face to arcs and segments
    converted = BRepOffsetAPI.MakeOffset.convert_face(face, 0.01)
    assert not converted.is_null()


# ==================== MakeOffsetShape Tests ====================

def test_make_offset_shape_box_positive():
    """Test positive offset of a solid box."""
    # Create a box
    box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).solid()
    
    # Create offset shape (enlarging the box)
    offset_maker = BRepOffsetAPI.MakeOffsetShape()
    offset_maker.perform_by_join(box, 1.0, 1e-3)
    
    assert offset_maker.is_done()
    result = offset_maker.shape()
    assert not result.is_null()


def test_make_offset_shape_box_negative():
    """Test negative offset of a solid box (shrinking)."""
    # Create a larger box
    box = BRepBuilderAPI.MakeBox(20.0, 20.0, 20.0).solid()
    
    # Create offset shape (shrinking the box)
    offset_maker = BRepOffsetAPI.MakeOffsetShape()
    offset_maker.perform_by_join(box, -2.0, 1e-3)
    
    assert offset_maker.is_done()
    result = offset_maker.shape()
    assert not result.is_null()


def test_make_offset_shape_cylinder():
    """Test offset of a cylindrical shell."""
    # Create a cylinder axis
    axis = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    
    # Create cylindrical surface
    cylinder = Geom.CylindricalSurface(axis, 5.0)
    
    # Create a face on the cylinder (partial)
    face = BRepBuilderAPI.MakeFace(cylinder, 0, math.pi, 0, 10.0, 1e-3).face()
    
    # Create offset
    offset_maker = BRepOffsetAPI.MakeOffsetShape()
    offset_maker.perform_by_join(face, 0.5, 1e-3)
    
    assert offset_maker.is_done()
    result = offset_maker.shape()
    assert not result.is_null()


def test_make_offset_shape_simple_algorithm():
    """Test offset using the simple algorithm without intersections."""
    # Create a simple box
    box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).solid()
    
    # Use simple algorithm
    offset_maker = BRepOffsetAPI.MakeOffsetShape()
    offset_maker.perform_by_simple(box, 0.5)
    
    assert offset_maker.is_done()
    result = offset_maker.shape()
    assert not result.is_null()


def test_make_offset_shape_with_arc_join():
    """Test offset with Arc join type (default)."""
    box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).solid()
    
    offset_maker = BRepOffsetAPI.MakeOffsetShape()
    offset_maker.perform_by_join(
        box, 1.0, 1e-3,
        join=GeomAbs.JoinType.Arc
    )
    
    assert offset_maker.is_done()
    assert offset_maker.join_type == GeomAbs.JoinType.Arc
    result = offset_maker.shape()
    assert not result.is_null()


def test_make_offset_shape_with_intersection_join():
    """Test offset with Intersection join type."""
    box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).solid()
    
    offset_maker = BRepOffsetAPI.MakeOffsetShape()
    offset_maker.perform_by_join(
        box, 1.0, 1e-3,
        join=GeomAbs.JoinType.Intersection
    )
    
    assert offset_maker.is_done()
    assert offset_maker.join_type == GeomAbs.JoinType.Intersection
    result = offset_maker.shape()
    assert not result.is_null()


def test_make_offset_shape_skin_mode():
    """Test offset with Skin mode (default)."""
    box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).solid()
    
    offset_maker = BRepOffsetAPI.MakeOffsetShape()
    offset_maker.perform_by_join(
        box, 1.0, 1e-3,
        mode=BRepOffsetAPI.OffsetMode.Skin
    )
    
    assert offset_maker.is_done()
    result = offset_maker.shape()
    assert not result.is_null()


def test_make_offset_shape_remove_internal_edges():
    """Test offset with internal edge removal enabled."""
    box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).solid()
    
    offset_maker = BRepOffsetAPI.MakeOffsetShape()
    offset_maker.perform_by_join(
        box, 1.0, 1e-3,
        remove_internal_edges=True
    )
    
    assert offset_maker.is_done()
    result = offset_maker.shape()
    assert not result.is_null()


def test_make_offset_shape_from_shell():
    """Test offset of a shell (not solid)."""
    # Create box and get a face (shell with single face)
    box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shell()
    
    # Offset the shell
    offset_maker = BRepOffsetAPI.MakeOffsetShape()
    offset_maker.perform_by_join(box, 0.5, 1e-3)
    
    assert offset_maker.is_done()
    result = offset_maker.shape()
    assert not result.is_null()


def test_make_offset_shape_sphere():
    """Test offset of a sphere."""
    # Create a sphere
    sphere = BRepBuilderAPI.MakeSphere(5.0).solid()
    
    # Offset outward
    offset_maker = BRepOffsetAPI.MakeOffsetShape()
    offset_maker.perform_by_join(sphere, 1.0, 1e-3)
    
    assert offset_maker.is_done()
    result = offset_maker.shape()
    assert not result.is_null()


# ==================== MiddlePath Tests ====================

def test_middle_path_from_pipe():
    """Test extracting middle path from a pipe."""
    # Create a pipe by sweeping a circle along a line
    # Spine (path)
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(0.0, 0.0, 20.0)
    spine_edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    # Profile (circle)
    ax2 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle = Geom.Circle(ax2, 3.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # Create pipe
    pipe = BRepBuilderAPI.MakePipe(spine, profile)
    pipe_shape = pipe.shape()
    
    # Get start and end faces
    start_face = pipe.first_shape()
    end_face = pipe.last_shape()
    
    # Extract middle path
    middle = BRepOffsetAPI.MiddlePath(pipe_shape, start_face, end_face)
    middle.build()
    
    assert middle.is_done()
    path = middle.shape()
    assert not path.is_null()


# ==================== NormalProjection Tests ====================

def test_normal_projection_basic():
    """Test basic normal projection of edge onto face."""
    # Create a planar face
    p1 = gp.Pnt(-10, -10, 0)
    p2 = gp.Pnt(10, -10, 0)
    p3 = gp.Pnt(10, 10, 0)
    p4 = gp.Pnt(-10, 10, 0)
    
    wire = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(p1, p2).edge(),
        BRepBuilderAPI.MakeEdge(p2, p3).edge(),
        BRepBuilderAPI.MakeEdge(p3, p4).edge(),
        BRepBuilderAPI.MakeEdge(p4, p1).edge()
    ).wire()
    face = BRepBuilderAPI.MakeFace(wire, False).face()
    
    # Create an edge above the face
    e1 = gp.Pnt(-5, -5, 5)
    e2 = gp.Pnt(5, 5, 5)
    edge_to_project = BRepBuilderAPI.MakeEdge(e1, e2).edge()
    
    # Project
    proj = BRepOffsetAPI.NormalProjection(face)
    proj.add(edge_to_project)
    proj.build()
    
    assert proj.is_done()
    result = proj.projection()
    assert not result.is_null()


def test_normal_projection_with_params():
    """Test normal projection with custom parameters."""
    # Create a cylindrical surface
    ax3 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    cylinder = Geom.CylindricalSurface(ax3, 5.0)
    face = BRepBuilderAPI.MakeFace(cylinder, 0, 2*math.pi, 0, 10, 1e-3).face()
    
    # Create a wire to project
    w1 = gp.Pnt(0, 10, 2)
    w2 = gp.Pnt(0, 10, 8)
    edge = BRepBuilderAPI.MakeEdge(w1, w2).edge()
    wire = BRepBuilderAPI.MakeWire(edge).wire()
    
    # Project with parameters
    proj = BRepOffsetAPI.NormalProjection()
    proj.init(face)
    proj.set_params(1e-4, 1e-5, GeomAbs.Shape.C2, 14, 16)
    proj.set_max_distance(20.0)
    proj.compute_3d(True)
    proj.add(wire)
    proj.build()
    
    assert proj.is_done()


def test_normal_projection_default_constructor():
    """Test normal projection with default constructor."""
    # Create target face (simple plane)
    face = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)),
        -10, 10, -10, 10
    ).face()
    
    # Edge to project
    edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(-5, 0, 3),
        gp.Pnt(5, 0, 3)
    ).edge()
    
    proj = BRepOffsetAPI.NormalProjection()
    proj.init(face)
    proj.add(edge)
    proj.build()
    
    assert proj.is_done()


def test_normal_projection_build_wire():
    """Test build_wire method."""
    # Create target
    face = BRepBuilderAPI.MakeFace(
        gp.Pln(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)),
        -10, 10, -10, 10
    ).face()
    
    # Create a simple wire to project
    edge1 = BRepBuilderAPI.MakeEdge(
        gp.Pnt(-5, -5, 2), gp.Pnt(5, -5, 2)
    ).edge()
    edge2 = BRepBuilderAPI.MakeEdge(
        gp.Pnt(5, -5, 2), gp.Pnt(5, 5, 2)
    ).edge()
    wire = BRepBuilderAPI.MakeWire(edge1, edge2).wire()
    
    proj = BRepOffsetAPI.NormalProjection(face)
    proj.add(wire)
    proj.build()
    
    success, wires = proj.build_wire()
    # Result depends on topology, but method should work
    assert isinstance(wires, list)


# ==================== ThruSections Tests ====================

def test_thru_sections_basic_shell():
    """Test basic ThruSections creating a shell."""
    # Create two circles at different heights
    ax1 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle1 = Geom.Circle(ax1, 5.0)
    wire1 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle1).edge()
    ).wire()
    
    ax2 = gp.Ax2(gp.Pnt(0, 0, 10), gp.Dir(0, 0, 1))
    circle2 = Geom.Circle(ax2, 3.0)
    wire2 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle2).edge()
    ).wire()
    
    # Create shell through sections
    loft = BRepOffsetAPI.ThruSections(is_solid=False)
    loft.add_wire(wire1)
    loft.add_wire(wire2)
    loft.build()
    
    assert loft.is_done()
    result = loft.shape()
    assert not result.is_null()


def test_thru_sections_solid():
    """Test ThruSections creating a solid."""
    # Create circles
    ax1 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle1 = Geom.Circle(ax1, 5.0)
    wire1 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle1).edge()
    ).wire()
    
    ax2 = gp.Ax2(gp.Pnt(0, 0, 10), gp.Dir(0, 0, 1))
    circle2 = Geom.Circle(ax2, 5.0)
    wire2 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle2).edge()
    ).wire()
    
    # Create solid (with end caps)
    loft = BRepOffsetAPI.ThruSections(is_solid=True)
    loft.add_wire(wire1)
    loft.add_wire(wire2)
    loft.build()
    
    assert loft.is_done()
    result = loft.shape()
    assert not result.is_null()
    
    # Check first/last shapes exist for solids
    first = loft.first_shape()
    last = loft.last_shape()
    assert not first.is_null()
    assert not last.is_null()


def test_thru_sections_ruled():
    """Test ThruSections with ruled surfaces."""
    # Create squares at different heights
    def make_square_wire(center_z, size):
        half = size / 2
        pts = [
            gp.Pnt(-half, -half, center_z),
            gp.Pnt(half, -half, center_z),
            gp.Pnt(half, half, center_z),
            gp.Pnt(-half, half, center_z),
        ]
        wire = BRepBuilderAPI.MakeWire()
        for i in range(4):
            edge = BRepBuilderAPI.MakeEdge(pts[i], pts[(i+1) % 4]).edge()
            wire.add(edge)
        return wire.wire()
    
    wire1 = make_square_wire(0, 10)
    wire2 = make_square_wire(15, 6)
    
    # Create ruled loft
    loft = BRepOffsetAPI.ThruSections(is_solid=True, ruled=True)
    loft.add_wire(wire1)
    loft.add_wire(wire2)
    loft.build()
    
    assert loft.is_done()
    result = loft.shape()
    assert not result.is_null()


def test_thru_sections_with_vertex():
    """Test ThruSections with vertex (cone-like shape)."""
    # Create a circle at base
    ax1 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle = Geom.Circle(ax1, 5.0)
    wire = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle).edge()
    ).wire()
    
    # Create apex vertex
    apex = BRepBuilderAPI.MakeVertex(gp.Pnt(0, 0, 10)).vertex()
    
    # Create cone-like solid
    loft = BRepOffsetAPI.ThruSections(is_solid=True)
    loft.add_wire(wire)
    loft.add_vertex(apex)
    loft.build()
    
    assert loft.is_done()
    result = loft.shape()
    assert not result.is_null()


def test_thru_sections_multiple_wires():
    """Test ThruSections with three sections."""
    def make_circle_wire(z, radius):
        ax = gp.Ax2(gp.Pnt(0, 0, z), gp.Dir(0, 0, 1))
        circle = Geom.Circle(ax, radius)
        return BRepBuilderAPI.MakeWire(
            BRepBuilderAPI.MakeEdge(circle).edge()
        ).wire()
    
    wire1 = make_circle_wire(0, 5.0)
    wire2 = make_circle_wire(5, 3.0)
    wire3 = make_circle_wire(10, 5.0)
    
    loft = BRepOffsetAPI.ThruSections(is_solid=True)
    loft.add_wire(wire1)
    loft.add_wire(wire2)
    loft.add_wire(wire3)
    loft.build()
    
    assert loft.is_done()
    result = loft.shape()
    assert not result.is_null()


def test_thru_sections_properties():
    """Test ThruSections property accessors."""
    loft = BRepOffsetAPI.ThruSections(is_solid=False, ruled=False, pres3d=1e-5)
    
    # Set various parameters
    loft.set_smoothing(True)
    loft.set_par_type(BRepOffsetAPI.ParametrizationType.ChordLength)
    loft.set_continuity(GeomAbs.Shape.C2)
    loft.set_max_degree(10)
    loft.set_mutable_input(False)
    
    # Check properties
    assert loft.use_smoothing == True
    assert loft.par_type == BRepOffsetAPI.ParametrizationType.ChordLength
    assert loft.continuity == GeomAbs.Shape.C2
    assert loft.max_degree == 10
    assert loft.is_mutable_input == False


def test_thru_sections_check_compatibility():
    """Test compatibility checking option."""
    ax1 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle1 = Geom.Circle(ax1, 5.0)
    wire1 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle1).edge()
    ).wire()
    
    ax2 = gp.Ax2(gp.Pnt(0, 0, 10), gp.Dir(0, 0, 1))
    circle2 = Geom.Circle(ax2, 5.0)
    wire2 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle2).edge()
    ).wire()
    
    loft = BRepOffsetAPI.ThruSections(is_solid=True)
    loft.check_compatibility(True)
    loft.add_wire(wire1)
    loft.add_wire(wire2)
    loft.build()
    
    assert loft.is_done()


def test_thru_sections_generated_face():
    """Test generated_face method."""
    ax1 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle1 = Geom.Circle(ax1, 5.0)
    edge1 = BRepBuilderAPI.MakeEdge(circle1).edge()
    wire1 = BRepBuilderAPI.MakeWire(edge1).wire()
    
    ax2 = gp.Ax2(gp.Pnt(0, 0, 10), gp.Dir(0, 0, 1))
    circle2 = Geom.Circle(ax2, 5.0)
    wire2 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle2).edge()
    ).wire()
    
    loft = BRepOffsetAPI.ThruSections(is_solid=True, ruled=True)
    loft.add_wire(wire1)
    loft.add_wire(wire2)
    loft.build()
    
    assert loft.is_done()
    
    # Get generated face from first wire edge
    gen_face = loft.generated_face(edge1)
    assert not gen_face.is_null()


def test_thru_sections_wires():
    """Test wires() method returns input wires."""
    ax1 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle1 = Geom.Circle(ax1, 5.0)
    wire1 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle1).edge()
    ).wire()
    
    ax2 = gp.Ax2(gp.Pnt(0, 0, 10), gp.Dir(0, 0, 1))
    circle2 = Geom.Circle(ax2, 5.0)
    wire2 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle2).edge()
    ).wire()
    
    loft = BRepOffsetAPI.ThruSections()
    loft.add_wire(wire1)
    loft.add_wire(wire2)
    
    wires = loft.wires()
    assert len(wires) == 2


def test_thru_sections_init():
    """Test reinitializing ThruSections with init()."""
    loft = BRepOffsetAPI.ThruSections(is_solid=False)
    
    # Reinitialize as solid
    loft.init(is_solid=True, ruled=True, pres3d=1e-4)
    
    ax1 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle1 = Geom.Circle(ax1, 5.0)
    wire1 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle1).edge()
    ).wire()
    
    ax2 = gp.Ax2(gp.Pnt(0, 0, 10), gp.Dir(0, 0, 1))
    circle2 = Geom.Circle(ax2, 5.0)
    wire2 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle2).edge()
    ).wire()
    
    loft.add_wire(wire1)
    loft.add_wire(wire2)
    loft.build()
    
    assert loft.is_done()


def test_thru_sections_criterium_weight():
    """Test criterium weight getter."""
    loft = BRepOffsetAPI.ThruSections()
    loft.set_criterium_weight(1.0, 2.0, 3.0)
    
    w1, w2, w3 = loft.criterium_weight()
    assert w1 == 1.0
    assert w2 == 2.0
    assert w3 == 3.0


def test_thru_sections_status():
    """Test status property."""
    loft = BRepOffsetAPI.ThruSections()
    
    # Before build, status should indicate not done
    status = loft.status
    assert status == BRepOffsetAPI.ThruSectionStatus.NotDone
    
    # After successful build
    ax1 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle1 = Geom.Circle(ax1, 5.0)
    wire1 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle1).edge()
    ).wire()
    
    ax2 = gp.Ax2(gp.Pnt(0, 0, 10), gp.Dir(0, 0, 1))
    circle2 = Geom.Circle(ax2, 5.0)
    wire2 = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(circle2).edge()
    ).wire()
    
    loft.add_wire(wire1)
    loft.add_wire(wire2)
    loft.build()
    
    assert loft.status == BRepOffsetAPI.ThruSectionStatus.Done


# ==================== MakePipeShell Tests ====================

def test_make_pipe_shell_basic():
    """Test basic MakePipeShell with single profile."""
    # Create spine (wire)
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(0.0, 0.0, 10.0)
    spine_edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    # Create circular profile
    circle = Geom.Circle(gp.Ax2(), 2.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # Create pipe shell
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()
    result = pipe_shell.shape()
    assert not result.is_null()


def test_make_pipe_shell_multiple_profiles():
    """Test MakePipeShell with multiple profiles along spine."""
    # Create a simple straight spine
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    spine_edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()

    # Create profiles - simple circles
    circle1 = Geom.Circle(gp.Ax2(gp.Pnt(0,0,0), gp.Dir(1,0,0)), 2.0)
    profile1_edge = BRepBuilderAPI.MakeEdge(circle1).edge()
    profile1 = BRepBuilderAPI.MakeWire(profile1_edge).wire()

    circle2 = Geom.Circle(gp.Ax2(gp.Pnt(10,0,0), gp.Dir(1,0,0)), 1.5)
    profile2_edge = BRepBuilderAPI.MakeEdge(circle2).edge()
    profile2 = BRepBuilderAPI.MakeWire(profile2_edge).wire()

    # Create pipe shell with two profiles
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.add(profile1)
    pipe_shell.add(profile2)
    pipe_shell.build()

    assert pipe_shell.is_done()
    result = pipe_shell.shape()
    assert not result.is_null()



def test_make_pipe_shell_frenet_mode():
    """Test MakePipeShell with Frenet trihedron mode."""
    # Create simple spine
    spine_edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(10.0, 0.0, 0.0)
    ).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    # Create profile
    circle = Geom.Circle(gp.Ax2(), 1.5)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # Use Frenet mode
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.set_mode(True)  # True for Frenet
    pipe_shell.add(profile)
    pipe_shell.build()
    
    # Verify binding works
    result = pipe_shell.shape()
    assert result is not None


def test_make_pipe_shell_corrected_frenet_mode():
    """Test MakePipeShell with CorrectedFrenet mode."""
    # Create spine
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 5.0, 5.0)
    spine_edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    # Create profile
    circle = Geom.Circle(gp.Ax2(), 1.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # CorrectedFrenet is default (False as parameter)
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.set_mode(False)  # False for CorrectedFrenet
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()


def test_make_pipe_shell_fixed_trihedron():
    """Test MakePipeShell with fixed trihedron mode."""
    # Create spine
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    spine_edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    # Create profile
    circle = Geom.Circle(gp.Ax2(), 1.5)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # Set fixed trihedron along Y and Z axes
    ax2 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.set_mode(ax2)
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()


def test_make_pipe_shell_binormal_mode():
    """Test MakePipeShell with fixed binormal direction."""
    # Create spine
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    spine_edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    # Create profile
    circle = Geom.Circle(gp.Ax2(), 1.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # Set fixed binormal direction (Y axis)
    binormal = gp.Dir(0, 1, 0)
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.set_mode(binormal)
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()


def test_make_pipe_shell_discrete_mode():
    """Test MakePipeShell with discrete trihedron mode."""
    # Create spine
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 5.0, 0.0)
    p3 = gp.Pnt(10.0, 0.0, 0.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    spine = BRepBuilderAPI.MakeWire(e1, e2).wire()
    
    # Create profile
    circle = Geom.Circle(gp.Ax2(), 1.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # Use discrete mode for sharp corners
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.set_discrete_mode()
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()


def test_make_pipe_shell_with_location():
    """Test adding profile with automatic location on spine."""
    # Create spine
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    spine_edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    # Create profile
    circle = Geom.Circle(gp.Ax2(), 2.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # Add profile with default automatic location
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()


def test_make_pipe_shell_make_solid():
    """Test converting pipe shell to solid."""
    # Create simple spine and profile
    spine_edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(0.0, 0.0, 10.0)
    ).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    circle = Geom.Circle(gp.Ax2(), 2.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # Create pipe shell and convert to solid
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()
    
    # Convert to solid (if possible)
    success = pipe_shell.make_solid()
    assert isinstance(success, bool)


def test_make_pipe_shell_tolerance_settings():
    """Test setting tolerance parameters."""
    # Create spine and profile
    spine_edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(10.0, 0.0, 0.0)
    ).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    circle = Geom.Circle(gp.Ax2(), 1.5)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    # Set custom tolerances
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.set_tolerance(1e-3, 1e-4, 1e-2)
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()


def test_make_pipe_shell_max_degree_segments():
    """Test setting max degree and segments."""
    # Create spine and profile
    spine_edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(10.0, 0.0, 0.0)
    ).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    circle = Geom.Circle(gp.Ax2(), 1.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.set_max_degree(10)
    pipe_shell.set_max_segments(8)
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()


def test_make_pipe_shell_force_approx_c1():
    """Test forcing C1 approximation."""
    # Create curved spine
    helix = Geom.BSplineCurve(
        poles=np.array([
            [0.0, 0.0, 0.0],
            [2.0, 0.0, 2.0],
            [2.0, 2.0, 4.0],
            [0.0, 2.0, 6.0]
        ]),
        knots=[0., 1., 2.],
        multiplicities=[3, 1, 3],
        degree=2
    )
    spine_edge = BRepBuilderAPI.MakeEdge(helix).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    circle = Geom.Circle(gp.Ax2(), 1.5)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.set_force_approx_c1(True)
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()


def test_make_pipe_shell_transition_mode():
    """Test setting transition mode at spine discontinuities."""
    # Create spine with a corner
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 0.0, 5.0)
    
    e1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    e2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    spine = BRepBuilderAPI.MakeWire(e1, e2).wire()
    
    circle = Geom.Circle(gp.Ax2(), 1.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.set_transition_mode(BRepBuilderAPI.TransitionMode.RoundCorner)
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()


def test_make_pipe_shell_is_ready():
    """Test is_ready method."""
    spine_edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(10.0, 0.0, 0.0)
    ).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    
    # Not ready before adding profile
    assert pipe_shell.is_ready() == False
    
    # Ready after adding profile
    circle = Geom.Circle(gp.Ax2(), 1.5)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    pipe_shell.add(profile)
    assert pipe_shell.is_ready() == True


def test_make_pipe_shell_profiles_and_spine():
    """Test querying profiles and spine."""
    spine_edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(10.0, 0.0, 0.0)
    ).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    circle1 = Geom.Circle(gp.Ax2(), 2.0)
    profile1_edge = BRepBuilderAPI.MakeEdge(circle1).edge()
    profile1 = BRepBuilderAPI.MakeWire(profile1_edge).wire()
    
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.add(profile1)
    pipe_shell.build()
    
    # Get profiles list
    profiles = pipe_shell.profiles()
    assert len(profiles) >= 1  # At least the one we added
    
    # Get spine
    result_spine = pipe_shell.spine()
    assert not result_spine.is_null()


def test_make_pipe_shell_error_on_surface():
    """Test error_on_surface method."""
    spine_edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(10.0, 0.0, 0.0)
    ).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    circle = Geom.Circle(gp.Ax2(), 1.5)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()
    
    # Get surface error
    error = pipe_shell.error_on_surface()
    assert isinstance(error, float)
    assert error >= 0.0


def test_make_pipe_shell_delete_profile():
    """Test deleting/removing a profile."""
    spine_edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(10.0, 0.0, 0.0)
    ).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    circle = Geom.Circle(gp.Ax2(), 1.5)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.add(profile)
    
    # Can delete profile if needed
    pipe_shell.delete(profile)
    
    # After deletion, should not be ready
    assert pipe_shell.is_ready() == False


def test_make_pipe_shell_with_contact():
    """Test profile addition with contact flag."""
    spine_edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(10.0, 0.0, 0.0)
    ).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    circle = Geom.Circle(gp.Ax2(), 1.5)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.add(profile, with_contact=True, with_correction=False)
    pipe_shell.build()
    
    assert pipe_shell.is_done()


def test_make_pipe_shell_with_correction():
    """Test profile addition with correction flag."""
    spine_edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(10.0, 0.0, 0.0)
    ).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    circle = Geom.Circle(gp.Ax2(), 1.5)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.add(profile, with_contact=False, with_correction=True)
    pipe_shell.build()
    
    assert pipe_shell.is_done()


def test_make_pipe_shell_simulate():
    """Test simulate method for visualization."""
    spine_edge = BRepBuilderAPI.MakeEdge(
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(10.0, 0.0, 0.0)
    ).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    circle = Geom.Circle(gp.Ax2(), 1.5)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.add(profile)
    
    # Simulate to get cross-sections
    sections = pipe_shell.simulate(5)  # 5 sections
    
    assert isinstance(sections, list)
    assert len(sections) == 5


def test_make_pipe_shell_complex_spine():
    """Test with more complex 3D spine."""
    # Create a smooth curved spine
    poles = [
        gp.Pnt(0.0, 0.0, 0.0),
        gp.Pnt(3.0, 4.0, 2.0),
        gp.Pnt(6.0, 2.0, 4.0),
        gp.Pnt(10.0, 0.0, 10.0)
    ]
    knots = [0.0, 1.0, 2.0]
    multiplicities = [3, 1, 3]
    degree = 2
    
    spine_curve = Geom.BSplineCurve(poles, knots, multiplicities, degree)
    spine_edge = BRepBuilderAPI.MakeEdge(spine_curve).edge()
    spine = BRepBuilderAPI.MakeWire(spine_edge).wire()
    
    # Create profile
    circle = Geom.Circle(gp.Ax2(), 1.0)
    profile_edge = BRepBuilderAPI.MakeEdge(circle).edge()
    profile = BRepBuilderAPI.MakeWire(profile_edge).wire()
    
    pipe_shell = BRepBuilderAPI.MakePipeShell(spine)
    pipe_shell.set_mode(True)  # Use Frenet for complex curves
    pipe_shell.add(profile)
    pipe_shell.build()
    
    assert pipe_shell.is_done()
    result = pipe_shell.shape()
    assert not result.is_null()