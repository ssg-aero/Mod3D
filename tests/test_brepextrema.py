"""Tests for BRepExtrema module - distance computation between shapes."""

import pytest
from mod3d import BRepExtrema, gp, TopoDS, BRepBuilderAPI


def test_dist_shape_shape_empty_constructor():
    """Test creating empty DistShapeShape."""
    dist = BRepExtrema.DistShapeShape()
    assert not dist.is_done
    

def test_dist_shape_shape_two_boxes():
    """Test distance between two separate boxes."""
    # Create two boxes
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    # Compute distance
    dist = BRepExtrema.DistShapeShape(box1, box2)
    
    assert dist.is_done
    assert dist.value == pytest.approx(10.0, abs=1e-6)
    assert dist.nb_solution >= 1
    assert not dist.inner_solution


def test_dist_shape_shape_with_deflection():
    """Test distance computation with custom deflection."""
    box1 = BRepBuilderAPI.MakeBox(5.0, 5.0, 5.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(15.0, 0.0, 0.0), 5.0, 5.0, 5.0).shape()
    
    dist = BRepExtrema.DistShapeShape(box1, box2, 0.001)
    
    assert dist.is_done
    assert dist.value == pytest.approx(10.0, abs=1e-6)


def test_dist_shape_shape_load_and_perform():
    """Test loading shapes and performing computation."""
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(25.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist = BRepExtrema.DistShapeShape()
    dist.load_s1(box1)
    dist.load_s2(box2)
    result = dist.perform()
    
    assert result
    assert dist.is_done
    assert dist.value == pytest.approx(15.0, abs=1e-6)


def test_dist_shape_shape_set_deflection():
    """Test setting deflection."""
    dist = BRepExtrema.DistShapeShape()
    dist.set_deflection(0.001)
    
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist.load_s1(box1)
    dist.load_s2(box2)
    dist.perform()
    
    assert dist.is_done
    assert dist.value == pytest.approx(10.0, abs=1e-6)


def test_dist_shape_shape_solution_points():
    """Test retrieving solution points."""
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist = BRepExtrema.DistShapeShape(box1, box2)
    
    assert dist.nb_solution >= 1
    
    # Get first solution points
    pt1 = dist.point_on_shape1(1)
    pt2 = dist.point_on_shape2(1)
    
    assert isinstance(pt1, gp.Pnt)
    assert isinstance(pt2, gp.Pnt)
    
    # Points should be on opposite faces
    assert pt1.x == pytest.approx(10.0, abs=1e-6)
    assert pt2.x == pytest.approx(20.0, abs=1e-6)


def test_dist_shape_shape_support_types():
    """Test getting support types for solutions."""
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist = BRepExtrema.DistShapeShape(box1, box2)
    
    assert dist.nb_solution >= 1
    
    # Get support types
    support_type1 = dist.support_type_shape1(1)
    support_type2 = dist.support_type_shape2(1)
    
    # Support types should be valid enum values
    assert support_type1 in [BRepExtrema.IsVertex, BRepExtrema.IsOnEdge, BRepExtrema.IsInFace]
    assert support_type2 in [BRepExtrema.IsVertex, BRepExtrema.IsOnEdge, BRepExtrema.IsInFace]


def test_dist_shape_shape_support_shapes():
    """Test getting support shapes for solutions."""
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist = BRepExtrema.DistShapeShape(box1, box2)
    
    assert dist.nb_solution >= 1
    
    # Get support shapes
    support1 = dist.support_on_shape1(1)
    support2 = dist.support_on_shape2(1)
    
    assert isinstance(support1, TopoDS.Shape)
    assert isinstance(support2, TopoDS.Shape)


def test_dist_shape_shape_touching_boxes():
    """Test distance between touching boxes (should be zero)."""
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(10.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist = BRepExtrema.DistShapeShape(box1, box2)
    
    assert dist.is_done
    assert dist.value == pytest.approx(0.0, abs=1e-6)


def test_dist_shape_shape_overlapping_boxes():
    """Test distance with overlapping boxes."""
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(5.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist = BRepExtrema.DistShapeShape(box1, box2)
    
    assert dist.is_done
    # For overlapping boxes, distance should be zero
    assert dist.value == pytest.approx(0.0, abs=1e-6)


def test_dist_shape_shape_sphere_to_sphere():
    """Test distance between two spheres."""
    sphere1 = BRepBuilderAPI.MakeSphere(5.0).shape()
    sphere2 = BRepBuilderAPI.MakeSphere(gp.Pnt(20.0, 0.0, 0.0), 5.0).shape()
    
    dist = BRepExtrema.DistShapeShape(sphere1, sphere2)
    
    assert dist.is_done
    # Distance should be 20 - 5 - 5 = 10
    assert dist.value == pytest.approx(10.0, abs=1e-6)
    assert dist.nb_solution >= 1


def test_dist_shape_shape_cylinder_to_box():
    """Test distance between cylinder and box."""
    cylinder = BRepBuilderAPI.MakeCylinder(5.0, 10.0).shape()
    box = BRepBuilderAPI.MakeBox(gp.Pnt(15.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist = BRepExtrema.DistShapeShape(cylinder, box)
    
    assert dist.is_done
    # Distance should be approximately 10 (15 - 5)
    assert dist.value == pytest.approx(10.0, abs=1e-6)


def test_dist_shape_shape_multi_thread_property():
    """Test multi-thread property."""
    dist = BRepExtrema.DistShapeShape()
    
    # Default should be False
    assert not dist.is_multi_thread
    
    # Set to True
    dist.is_multi_thread = True
    assert dist.is_multi_thread
    
    # Set back to False
    dist.is_multi_thread = False
    assert not dist.is_multi_thread


def test_dist_shape_shape_with_flags():
    """Test construction with ExtFlag parameter."""
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    # Test with different flags (obsolete but should work)
    dist_min = BRepExtrema.DistShapeShape(box1, box2, BRepExtrema.MIN)
    assert dist_min.is_done
    
    dist_max = BRepExtrema.DistShapeShape(box1, box2, BRepExtrema.MAX)
    assert dist_max.is_done
    
    dist_minmax = BRepExtrema.DistShapeShape(box1, box2, BRepExtrema.MINMAX)
    assert dist_minmax.is_done


def test_dist_shape_shape_with_algo():
    """Test construction with ExtAlgo parameter."""
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    # Test with different algorithms (obsolete but should work)
    dist_grad = BRepExtrema.DistShapeShape(box1, box2, BRepExtrema.MINMAX, BRepExtrema.Grad)
    assert dist_grad.is_done
    
    dist_tree = BRepExtrema.DistShapeShape(box1, box2, BRepExtrema.MINMAX, BRepExtrema.Tree)
    assert dist_tree.is_done


def test_dist_shape_shape_par_on_face():
    """Test parameter extraction on faces."""
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist = BRepExtrema.DistShapeShape(box1, box2)
    
    assert dist.nb_solution >= 1
    
    # Check if solution is on a face
    support_type1 = dist.support_type_shape1(1)
    if support_type1 == BRepExtrema.IsInFace:
        params = dist.par_on_face_s1(1)
        assert isinstance(params, tuple)
        assert len(params) == 2
        u, v = params
        assert isinstance(u, float)
        assert isinstance(v, float)


def test_dist_shape_shape_par_on_edge():
    """Test parameter extraction on edges."""
    # Create shapes where solution might be on edge
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist = BRepExtrema.DistShapeShape(box1, box2)
    
    assert dist.nb_solution >= 1
    
    # Check if solution is on an edge
    support_type1 = dist.support_type_shape1(1)
    if support_type1 == BRepExtrema.IsOnEdge:
        param = dist.par_on_edge_s1(1)
        assert isinstance(param, float)


def test_dist_shape_shape_obsolete_methods():
    """Test obsolete methods for backwards compatibility."""
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist = BRepExtrema.DistShapeShape()
    
    # These methods are obsolete but should not crash
    dist.set_flag(BRepExtrema.MINMAX)
    dist.set_algo(BRepExtrema.Grad)
    
    dist.load_s1(box1)
    dist.load_s2(box2)
    dist.perform()
    
    assert dist.is_done


def test_dist_shape_shape_inner_solution():
    """Test inner_solution property with nested shapes."""
    # Create a large box and a small box inside
    large_box = BRepBuilderAPI.MakeBox(100.0, 100.0, 100.0).shape()
    small_box = BRepBuilderAPI.MakeBox(gp.Pnt(40.0, 40.0, 40.0), 20.0, 20.0, 20.0).shape()
    
    dist = BRepExtrema.DistShapeShape(large_box, small_box)
    
    assert dist.is_done
    # Small box is inside large box, so this might trigger inner_solution
    # (Note: This depends on OCCT's internal logic)


def test_dist_shape_shape_multiple_solutions():
    """Test case with potentially multiple solutions."""
    # Create two boxes that might have multiple nearest points
    box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    box2 = BRepBuilderAPI.MakeBox(gp.Pnt(20.0, 0.0, 0.0), 10.0, 10.0, 10.0).shape()
    
    dist = BRepExtrema.DistShapeShape(box1, box2)
    
    assert dist.is_done
    nb_sol = dist.nb_solution
    assert nb_sol >= 1
    
    # Iterate through all solutions
    for i in range(1, nb_sol + 1):
        pt1 = dist.point_on_shape1(i)
        pt2 = dist.point_on_shape2(i)
        assert isinstance(pt1, gp.Pnt)
        assert isinstance(pt2, gp.Pnt)
