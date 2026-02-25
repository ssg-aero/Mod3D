"""Tests for BRepFilletAPI_MakeFillet2d module."""

import math
from mod3d import (
    gp,
    BRepBuilderAPI,
    BRepFillet,
    TopExp,
    TopAbs,
)


def test_fillet2d_basic():
    """Test basic 2D fillet creation on a planar face."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    # Use MakePolygon for proper vertex merging
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    # Create fillet2d builder
    fillet2d = BRepFillet.MakeFillet2d(face)
    
    # Check initialization status
    assert fillet2d.status == BRepFillet.ConstructionError.Ready
    
    # Get vertices from the wire using Python iterator
    vertices = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.VERTEX))

    
    assert len(vertices) == 4
    
    # Add fillet at first vertex (corner)
    fillet_edge = fillet2d.add_fillet(vertices[1], 1.0)
    
    # Check status after adding fillet
    assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    assert not fillet_edge.is_null()
    
    # Build the result
    fillet2d.build()
    assert fillet2d.is_done()
    
    # Get modified shape
    result = fillet2d.shape()
    assert not result.is_null()
    
    # Check number of fillets
    assert fillet2d.nb_fillet == 1
    assert fillet2d.nb_chamfer == 0


def test_fillet2d_multiple_fillets():
    """Test adding multiple fillets to a planar face."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    # Create wire and face
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    # Create fillet2d builder
    fillet2d = BRepFillet.MakeFillet2d(face)
    
    # Get all vertices
    # explorer = TopExp.Explorer(wire, TopAbs.ShapeEnum.VERTEX)
    # vertices = []
    # while explorer.more():
    #     vertices.append(explorer.current())
    #     explorer.next()
    vertices = TopExp.getvetices(wire)
    
    # Add fillets at all corners
    radius = 0.5
    for vertex in vertices:
        fillet_edge = fillet2d.add_fillet(vertex, radius)
        assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    
    # Build and check
    fillet2d.build()
    assert fillet2d.is_done()
    assert fillet2d.nb_fillet == len(vertices)


def test_fillet2d_modify_fillet():
    """Test modifying an existing fillet."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    # Create fillet with initial radius
    fillet2d = BRepFillet.MakeFillet2d(face)
    
    explorer = TopExp.Explorer(wire, TopAbs.ShapeEnum.VERTEX)
    vertex = explorer.current()
    
    # Add fillet
    fillet_edge = fillet2d.add_fillet(vertex, 0.5)
    assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    
    # Modify fillet with new radius
    new_fillet = fillet2d.modify_fillet(fillet_edge, 1.0)
    assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    assert not new_fillet.is_null()
    
    fillet2d.build()
    assert fillet2d.is_done()


def test_fillet2d_remove_fillet():
    """Test removing a fillet."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    fillet2d = BRepFillet.MakeFillet2d(face)
    
    explorer = TopExp.Explorer(wire, TopAbs.ShapeEnum.VERTEX)
    vertex = explorer.current()
    
    # Add fillet
    fillet_edge = fillet2d.add_fillet(vertex, 0.5)
    assert fillet2d.nb_fillet == 1
    
    # Remove fillet
    result_vertex = fillet2d.remove_fillet(fillet_edge)
    assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    assert not result_vertex.is_null()
    
    fillet2d.build()
    assert fillet2d.is_done()
    assert fillet2d.nb_fillet == 0


def test_chamfer2d_two_distances():
    """Test adding a chamfer with two distances."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    
    # Get edges for chamfer
    edges = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.EDGE))
    e1 = edges[0]
    e2 = edges[1]
    
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    # Create chamfer
    fillet2d = BRepFillet.MakeFillet2d(face)
    
    # Add chamfer between e1 and e2
    chamfer_edge = fillet2d.add_chamfer(e1, e2, 1.0, 1.5)
    assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    assert not chamfer_edge.is_null()
    
    fillet2d.build()
    assert fillet2d.is_done()
    assert fillet2d.nb_chamfer == 1
    assert fillet2d.nb_fillet == 0


def test_chamfer2d_distance_angle():
    """Test adding a chamfer with distance and angle."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    fillet2d = BRepFillet.MakeFillet2d(face)
    
    # Get vertices and edges using Python iterator
    vertices = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.VERTEX))
    edges = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.EDGE))
    
    # Add chamfer with distance and angle (45 degrees = pi/4 radians)
    chamfer_edge = fillet2d.add_chamfer(edges[0], vertices[1], 1.0, math.pi / 4)
    assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    
    fillet2d.build()
    assert fillet2d.is_done()
    assert fillet2d.nb_chamfer == 1


def test_chamfer2d_modify():
    """Test modifying a chamfer."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    
    edges = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.EDGE))
    e1 = edges[0]
    e2 = edges[1]
    
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    fillet2d = BRepFillet.MakeFillet2d(face)
    
    # Add chamfer
    chamfer_edge = fillet2d.add_chamfer(e1, e2, 0.5, 0.5)
    assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    
    # Modify chamfer with new distances
    new_chamfer = fillet2d.modify_chamfer(chamfer_edge, e1, e2, 1.0, 1.5)
    assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    assert not new_chamfer.is_null()
    
    fillet2d.build()
    assert fillet2d.is_done()


def test_chamfer2d_remove():
    """Test removing a chamfer."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    
    edges = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.EDGE))
    e1 = edges[0]
    e2 = edges[1]
    
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    fillet2d = BRepFillet.MakeFillet2d(face)
    
    # Add chamfer
    chamfer_edge = fillet2d.add_chamfer(e1, e2, 1.0, 1.0)
    assert fillet2d.nb_chamfer == 1
    
    # Remove chamfer
    result_vertex = fillet2d.remove_chamfer(chamfer_edge)
    assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    assert not result_vertex.is_null()
    
    fillet2d.build()
    assert fillet2d.is_done()
    assert fillet2d.nb_chamfer == 0


def test_fillet2d_mixed_operations():
    """Test mixing fillets and chamfers on the same face."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    fillet2d = BRepFillet.MakeFillet2d(face)
    
    # Get vertices and edges using Python iterator
    vertices = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.VERTEX))
    edges = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.EDGE))
    
    # Add fillet at first corner
    fillet_edge = fillet2d.add_fillet(vertices[0], 0.5)
    assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    
    # Add chamfer at opposite corner
    chamfer_edge = fillet2d.add_chamfer(edges[1], edges[2], 0.8, 0.8)
    assert fillet2d.status == BRepFillet.ConstructionError.IsDone
    
    fillet2d.build()
    assert fillet2d.is_done()
    assert fillet2d.nb_fillet == 1
    assert fillet2d.nb_chamfer == 1


def test_fillet2d_query_methods():
    """Test query methods for checking modified edges."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    fillet2d = BRepFillet.MakeFillet2d(face)
    
    # Add fillet using Python iterator
    vertices = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.VERTEX))
    edges = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.EDGE))
    
    fillet_edge = fillet2d.add_fillet(vertices[0], 0.5)
    
    # Check if edge is modified
    assert fillet2d.is_modified(fillet_edge)
    
    # Check descendant/basis edge relationships
    assert fillet2d.has_descendant(edges[0])
    descendant = fillet2d.descendant_edge(e1)
    assert not descendant.is_null()
    
    # Check that we can get the basis edge back
    basis = fillet2d.basis_edge(descendant)
    assert not basis.is_null()
    
    fillet2d.build()
    assert fillet2d.is_done()


def test_fillet2d_error_handling():
    """Test error handling for invalid operations."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    fillet2d = BRepFillet.MakeFillet2d(face)
    
    # Get first vertex using Python iterator
    vertices = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.VERTEX))
    vertex = vertices[0]
    
    # Try to add fillet with radius too large
    # This should result in ComputationError status
    fillet_edge = fillet2d.add_fillet(vertex, 100.0)
    # The status should indicate an error
    assert fillet2d.status != BRepFillet.ConstructionError.IsDone


def test_fillet2d_init_variations():
    """Test different initialization methods."""
    # Create a rectangular face
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 0.0, 0.0)
    p3 = gp.Pnt(10.0, 5.0, 0.0)
    p4 = gp.Pnt(0.0, 5.0, 0.0)
    
    wire_maker = BRepBuilderAPI.MakePolygon(p1, p2, p3, p4, True)
    wire = wire_maker.wire()
    face = BRepBuilderAPI.MakeFace(wire).face()
    
    # Test empty constructor + init
    fillet2d1 = BRepFillet.MakeFillet2d()
    fillet2d1.init(face)
    assert fillet2d1.status == BRepFillet.ConstructionError.Ready
    
    # Test constructor with face
    fillet2d2 = BRepFillet.MakeFillet2d(face)
    assert fillet2d2.status == BRepFillet.ConstructionError.Ready
    
    # Add a fillet and create modified face using Python iterator
    vertices = list(TopExp.Explorer(wire, TopAbs.ShapeEnum.VERTEX))
    fillet2d1.add_fillet(vertices[0], 0.5)
    fillet2d1.build()
    modified_face = fillet2d1.shape()
    
    # Test init with ref_face and mod_face
    fillet2d3 = BRepFillet.MakeFillet2d()
    fillet2d3.init(face, modified_face)
    assert fillet2d3.status == BRepFillet.ConstructionError.Ready
    assert fillet2d3.status == BRepFillet.ConstructionError.Ready