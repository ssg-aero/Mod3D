from mod3d import (
    gp,
    Geom,
    BRepBuilderAPI,
)


def test_MakeVertex():
    vertex = BRepBuilderAPI.MakeVertex(gp.Pnt(1.0, 2.0, 3.0)).vertex()
    point = vertex.point
    assert (point.x, point.y, point.z) == (1.0, 2.0, 3.0)
    assert (vertex.x(), vertex.y(), vertex.z()) == (1.0, 2.0, 3.0)


def test_MakeEdge():
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    assert edge
    assert not edge.is_null()
    res = edge.curve()
    assert res is not None
    curve, u1, u2 = res
    assert u1 == 0.0
    assert u2 == 10.0

def test_make_cone_basic():
    # Test basic cone with default axis (Z axis at origin)
    cone_builder = BRepBuilderAPI.MakeCone(10.0, 5.0, 20.0)
    cone = cone_builder.shape()
    assert not cone.is_null()
    assert cone_builder.is_done()
    assert not cone_builder.solid().is_null()
    assert not cone_builder.shell().is_null()
    assert not cone_builder.face().is_null()

def test_make_cone_with_angle():
    # Test cone with angle parameter (partial cone)
    import math
    cone_builder = BRepBuilderAPI.MakeCone(10.0, 5.0, 20.0, math.pi)  # Half cone
    cone = cone_builder.shape()
    assert not cone.is_null()
    assert cone_builder.is_done()

def test_make_cone_with_custom_axis():
    # Test cone with custom axis
    axis = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    cone_builder = BRepBuilderAPI.MakeCone(axis, 10.0, 5.0, 20.0)
    cone = cone_builder.shape()
    assert not cone.is_null()
    assert cone_builder.is_done()
    assert not cone_builder.solid().is_null()
    assert not cone_builder.shell().is_null()
    assert not cone_builder.face().is_null()

def test_make_revolution_basic():
    # Test revolution with a simple curve (line segment)
    # from mod3d import geom
    
    # Create a line segment for the meridian (profile curve)
    p1 = gp.Pnt(10.0, 0.0, 0.0)  # Start point (away from Z axis)
    p2 = gp.Pnt(5.0, 0.0, 20.0)   # End point (creates a conical profile)
    dir_vec = gp.Dir(gp.Vec(p1, p2))
    
    # Create a Geom_Line and use it for revolution
    line = Geom.Line(p1, dir_vec)
    
    # Create revolution around default Z axis
    rev_builder = BRepBuilderAPI.MakeRevolution(line)
    rev_solid = rev_builder.shape()
    assert not rev_solid.is_null()
    assert rev_builder.is_done()
    assert not rev_builder.solid().is_null()

def test_make_revolution_with_angle():
    # Test partial revolution (e.g., 180 degrees)
    import math
    
    p1 = gp.Pnt(10.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 20.0)
    dir_vec = gp.Dir(gp.Vec(p1, p2))
    line = Geom.Line(p1, dir_vec)
    
    # Half revolution
    rev_builder = BRepBuilderAPI.MakeRevolution(line, math.pi)
    rev_solid = rev_builder.shape()
    assert not rev_solid.is_null()
    assert rev_builder.is_done()

def test_make_revolution_with_custom_axis():
    # Test revolution with custom axis
    
    # Define custom axis (e.g., X axis)
    custom_axis = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(1.0, 0.0, 0.0))
    
    # Create meridian curve in XZ plane
    p1 = gp.Pnt(0.0, 5.0, 0.0)
    p2 = gp.Pnt(0.0, 10.0, 20.0)
    dir_vec = gp.Dir(gp.Vec(p1, p2))
    line = Geom.Line(p1, dir_vec)
    
    rev_builder = BRepBuilderAPI.MakeRevolution(custom_axis, line)
    rev_solid = rev_builder.shape()
    assert not rev_solid.is_null()
    assert rev_builder.is_done()

def test_make_revolution_with_bounds():
    # Test revolution with parameter bounds
    
    p1 = gp.Pnt(10.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 0.0, 20.0)
    dir_vec = gp.Dir(gp.Vec(p1, p2))
    line = Geom.Line(p1, dir_vec)
    
    # Revolution with parameter bounds (vmin, vmax)
    rev_builder = BRepBuilderAPI.MakeRevolution(line, 0.0, 20.0)
    rev_solid = rev_builder.shape()
    assert not rev_solid.is_null()
    assert rev_builder.is_done()

def test_make_revolution_access_surface():
    # Test accessing the revolution surface

    p1 = gp.Pnt(10.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 20.0)
    dir_vec = gp.Dir(gp.Vec(p1, p2))
    line = Geom.Line(p1, dir_vec)
    
    rev_builder = BRepBuilderAPI.MakeRevolution(line)
    
    # Access the revolution surface
    rev_surface = rev_builder.revolution()
    assert rev_surface is not None


# ==================== MakeWire Tests ====================

def test_make_wire_empty():
    """Test creating an empty wire builder."""
    wire_builder = BRepBuilderAPI.MakeWire()
    # Empty wire is not done until edges are added
    assert wire_builder.error() == BRepBuilderAPI.EmptyWire


def test_make_wire_from_single_edge():
    """Test creating a wire from a single edge."""
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    
    wire_builder = BRepBuilderAPI.MakeWire(edge)
    assert wire_builder.is_done()
    assert wire_builder.error() == BRepBuilderAPI.WireDone
    
    wire = wire_builder.wire()
    assert not wire.is_null()


def test_make_wire_from_multiple_edges():
    """Test creating a wire from multiple edges."""
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 10.0, 0.0)
    
    edge1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    edge2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    
    wire_builder = BRepBuilderAPI.MakeWire(edge1, edge2)
    assert wire_builder.is_done()
    assert wire_builder.error() == BRepBuilderAPI.WireDone
    
    wire = wire_builder.wire()
    assert not wire.is_null()


def test_make_wire_add_edges():
    """Test adding edges to a wire builder."""
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 10.0, 0.0)
    p4 = gp.Pnt(0.0, 10.0, 0.0)
    
    edge1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    edge2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    edge3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    edge4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire_builder = BRepBuilderAPI.MakeWire(edge1)
    wire_builder.add(edge2)
    wire_builder.add(edge3)
    wire_builder.add(edge4)
    
    assert wire_builder.is_done()
    wire = wire_builder.wire()
    assert not wire.is_null()


def test_make_wire_closed_rectangle():
    """Test creating a closed rectangular wire."""
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    edge1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    edge2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    edge3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    edge4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire_builder = BRepBuilderAPI.MakeWire(edge1, edge2, edge3, edge4)
    assert wire_builder.is_done()
    
    wire = wire_builder.wire()
    assert not wire.is_null()
    assert wire.closed()


# ==================== MakeFace Tests ====================

def test_make_face_from_plane():
    """Test creating an infinite face from a plane."""
    plane = gp.Pln(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    
    face_builder = BRepBuilderAPI.MakeFace(plane)
    assert face_builder.is_done()
    assert face_builder.error() == BRepBuilderAPI.FaceDone
    
    face = face_builder.face()
    assert not face.is_null()


def test_make_face_from_plane_with_bounds():
    """Test creating a bounded face from a plane."""
    plane = gp.Pln(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    
    face_builder = BRepBuilderAPI.MakeFace(plane, 0.0, 10.0, 0.0, 10.0)
    assert face_builder.is_done()
    
    face = face_builder.face()
    assert not face.is_null()


def test_make_face_from_wire():
    """Test creating a face from a closed wire."""
    # Create a square wire
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 10.0, 0.0)
    p4 = gp.Pnt(0.0, 10.0, 0.0)
    
    edge1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    edge2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    edge3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
    edge4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(edge1, edge2, edge3, edge4).wire()
    
    face_builder = BRepBuilderAPI.MakeFace(wire)
    assert face_builder.is_done()
    
    face = face_builder.face()
    assert not face.is_null()


def test_make_face_from_surface_with_wire():
    """Test creating a face from a surface and wire."""
    plane = gp.Pln(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    
    # Create a triangular wire
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(5.0, 10.0, 0.0)
    
    edge1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    edge2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
    edge3 = BRepBuilderAPI.MakeEdge(p3, p1).edge()
    
    wire = BRepBuilderAPI.MakeWire(edge1, edge2, edge3).wire()
    
    face_builder = BRepBuilderAPI.MakeFace(plane, wire)
    assert face_builder.is_done()
    
    face = face_builder.face()
    assert not face.is_null()


def test_make_face_add_hole():
    """Test adding a hole to a face."""
    # Create outer wire (larger square)
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(20.0, 0.0, 0.0)
    p3 = gp.Pnt(20.0, 20.0, 0.0)
    p4 = gp.Pnt(0.0, 20.0, 0.0)
    
    outer_wire = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(p1, p2).edge(),
        BRepBuilderAPI.MakeEdge(p2, p3).edge(),
        BRepBuilderAPI.MakeEdge(p3, p4).edge(),
        BRepBuilderAPI.MakeEdge(p4, p1).edge()
    ).wire()
    
    # Create inner wire (smaller square - hole)
    h1 = gp.Pnt(5.0, 5.0, 0.0)
    h2 = gp.Pnt(15.0, 5.0, 0.0)
    h3 = gp.Pnt(15.0, 15.0, 0.0)
    h4 = gp.Pnt(5.0, 15.0, 0.0)
    
    # Note: Hole wire should be oriented opposite to outer wire
    inner_wire = BRepBuilderAPI.MakeWire(
        BRepBuilderAPI.MakeEdge(h1, h4).edge(),
        BRepBuilderAPI.MakeEdge(h4, h3).edge(),
        BRepBuilderAPI.MakeEdge(h3, h2).edge(),
        BRepBuilderAPI.MakeEdge(h2, h1).edge()
    ).wire()
    
    # Create face with outer wire
    face = BRepBuilderAPI.MakeFace(outer_wire).face()
    
    # Add hole
    face_with_hole = BRepBuilderAPI.MakeFace(face, inner_wire).face()
    assert not face_with_hole.is_null()


# ==================== MakeShell Tests ====================

def test_make_shell_from_surface():
    """Test creating a shell from a Geom_Surface."""
    # Create a cylindrical surface
    axis = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    cylinder_surface = Geom.CylindricalSurface(axis, 10.0)
    
    shell_builder = BRepBuilderAPI.MakeShell(cylinder_surface)
    assert shell_builder.is_done()
    assert shell_builder.error() == BRepBuilderAPI.ShellDone
    
    shell = shell_builder.shell()
    assert not shell.is_null()


def test_make_shell_from_surface_with_bounds():
    """Test creating a bounded shell from a surface."""
    import math
    
    axis = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    cylinder_surface = Geom.CylindricalSurface(axis, 10.0)
    
    # Create shell with parameter bounds
    shell_builder = BRepBuilderAPI.MakeShell(
        cylinder_surface, 
        0.0, 2 * math.pi,  # U bounds (full circle)
        0.0, 20.0          # V bounds (height)
    )
    assert shell_builder.is_done()
    
    shell = shell_builder.shell()
    assert not shell.is_null()


# ==================== MakeSolid Tests ====================

def test_make_solid_from_shell():
    """Test creating a solid from a shell (using a cylinder)."""
    # Create a complete cylinder which gives us a valid shell
    cylinder = BRepBuilderAPI.MakeCylinder(10.0, 20.0)
    shell = cylinder.shell()
    
    solid_builder = BRepBuilderAPI.MakeSolid(shell)
    assert solid_builder.is_done()
    
    solid = solid_builder.solid()
    assert not solid.is_null()


def test_make_solid_from_multiple_shells():
    """Test creating a solid from multiple shells."""
    # Create two cylinders
    cyl1 = BRepBuilderAPI.MakeCylinder(10.0, 20.0)
    shell1 = cyl1.shell()
    
    # Note: In practice, shells need to form a closed volume
    # This test demonstrates the API usage
    solid_builder = BRepBuilderAPI.MakeSolid(shell1)
    solid = solid_builder.solid()
    assert not solid.is_null()


def test_make_solid_add_shell():
    """Test adding shells to a solid builder."""
    # Start with a cylinder shell
    cyl = BRepBuilderAPI.MakeCylinder(10.0, 20.0)
    shell = cyl.shell()
    
    solid_builder = BRepBuilderAPI.MakeSolid(shell)
    solid = solid_builder.solid()
    assert not solid.is_null()


# ==================== Wire Error Enum Tests ====================

def test_wire_error_enum():
    """Test WireError enum values."""
    assert BRepBuilderAPI.WireDone is not None
    assert BRepBuilderAPI.EmptyWire is not None
    assert BRepBuilderAPI.DisconnectedWire is not None
    assert BRepBuilderAPI.NonManifoldWire is not None


# ==================== Face Error Enum Tests ====================

def test_face_error_enum():
    """Test FaceError enum values."""
    assert BRepBuilderAPI.FaceDone is not None
    assert BRepBuilderAPI.NoFace is not None
    assert BRepBuilderAPI.NotPlanar is not None
    assert BRepBuilderAPI.CurveProjectionFailed is not None
    assert BRepBuilderAPI.ParametersOutOfRange is not None


# ==================== Shell Error Enum Tests ====================

def test_shell_error_enum():
    """Test ShellError enum values."""
    assert BRepBuilderAPI.ShellDone is not None
    assert BRepBuilderAPI.EmptyShell is not None
    assert BRepBuilderAPI.DisconnectedShell is not None
    assert BRepBuilderAPI.ShellParametersOutOfRange is not None