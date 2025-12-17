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

# def test_make_revolution_with_angle():
#     # Test partial revolution (e.g., 180 degrees)
#     from mod3d import geom
#     import math
    
#     p1 = gp.Pnt(10.0, 0.0, 0.0)
#     p2 = gp.Pnt(10.0, 0.0, 20.0)
#     dir_vec = gp.Dir(gp.Vec(p1, p2))
#     line = Geom.Line(p1, dir_vec)
    
#     # Half revolution
#     rev_builder = BRepBuilderAPI.MakeRevolution(line, math.pi)
#     rev_solid = rev_builder.shape()
#     assert not rev_solid.is_null()
#     assert rev_builder.is_done()

# def test_make_revolution_with_custom_axis():
#     # Test revolution with custom axis
#     from mod3d import geom
    
#     # Define custom axis (e.g., X axis)
#     custom_axis = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(1.0, 0.0, 0.0))
    
#     # Create meridian curve in XZ plane
#     p1 = gp.Pnt(0.0, 5.0, 0.0)
#     p2 = gp.Pnt(0.0, 10.0, 20.0)
#     dir_vec = gp.Dir(gp.Vec(p1, p2))
#     line = Geom.Line(p1, dir_vec)
    
#     rev_builder = BRepBuilderAPI.MakeRevolution(custom_axis, line)
#     rev_solid = rev_builder.shape()
#     assert not rev_solid.is_null()
#     assert rev_builder.is_done()

# def test_make_revolution_with_bounds():
#     # Test revolution with parameter bounds
#     from mod3d import geom
    
#     p1 = gp.Pnt(10.0, 0.0, 0.0)
#     p2 = gp.Pnt(5.0, 0.0, 20.0)
#     dir_vec = gp.Dir(gp.Vec(p1, p2))
#     line = Geom.Line(p1, dir_vec)
    
#     # Revolution with parameter bounds (vmin, vmax)
#     rev_builder = BRepBuilderAPI.MakeRevolution(line, 0.0, 20.0)
#     rev_solid = rev_builder.shape()
#     assert not rev_solid.is_null()
#     assert rev_builder.is_done()

# def test_make_revolution_access_surface():
#     # Test accessing the revolution surface
#     from mod3d import geom
    
#     p1 = gp.Pnt(10.0, 0.0, 0.0)
#     p2 = gp.Pnt(10.0, 0.0, 20.0)
#     dir_vec = gp.Dir(gp.Vec(p1, p2))
#     line = Geom.Line(p1, dir_vec)
    
#     rev_builder = BRepBuilderAPI.MakeRevolution(line)
    
#     # Access the revolution surface
#     rev_surface = rev_builder.revolution()
#     assert rev_surface is not None
