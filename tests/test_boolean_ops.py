"""
Test suite for Boolean operations: Common, Cut, Fuse, and Section.

Tests various scenarios including:
- Basic solid operations
- Edge cases (touching, overlapping, disjoint)
- Non-destructive mode
- Error handling
- History tracking (modified, generated, deleted)
"""

import pytest
import mod3d


class TestBooleanCommon:
    """Tests for BRepAlgoAPI_Common (intersection) operation."""
    
    def test_common_two_overlapping_boxes(self):
        """Test intersection of two overlapping boxes."""
        # Create two overlapping boxes
        box1 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        box2 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        
        # Move box2 to overlap with box1
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(5.0, 0.0, 0.0))
        box2 =box2.moved(translation)
        
        # Perform intersection
        common = mod3d.BooleanOp.Common(box1, box2)
        result = common.shape()
        
        assert not result.is_null()
        assert not common.has_errors()
        
    def test_common_disjoint_shapes(self):
        """Test intersection of two disjoint shapes (should be empty)."""
        box1 = mod3d.BRepBuilderAPI.MakeBox(5.0, 5.0, 5.0).shape()
        box2 = mod3d.BRepBuilderAPI.MakeBox(5.0, 5.0, 5.0).shape()
        
        # Move box2 far away
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(20.0, 0.0, 0.0))
        box2.move(translation)
        
        common = mod3d.BooleanOp.Common(box1, box2)
        result = common.shape()
        
        # Result should be null or empty compound
        assert result.is_null() or result.shape_type == mod3d.TopAbs.ShapeEnum.COMPOUND
        
    def test_common_sphere_box(self):
        """Test intersection of sphere and box."""
        sphere = mod3d.BRepBuilderAPI.MakeSphere(5.0).shape()
        box = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        
        # Move box to center on sphere
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(-5.0, -5.0, -5.0))
        box.move(translation)
        
        common = mod3d.BooleanOp.Common(sphere, box)
        result = common.shape()
        
        assert not result.is_null()
        assert not common.has_errors()
        
    def test_common_with_fuzzy_tolerance(self):
        """Test intersection with fuzzy tolerance."""
        box1 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        box2 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        
        # Move box2 slightly
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(0.0001, 0.0, 0.0))
        box2 = box2.moved(translation)
        
        common = mod3d.BooleanOp.Common(box1, box2)
        common.fuzzy_value = 0.001  # Set fuzzy tolerance
        result = common.shape()
        
        assert not result.is_null()


class TestBooleanCut:
    """Tests for BRepAlgoAPI_Cut (subtraction) operation."""
    
    def test_cut_hole_in_box(self):
        """Test cutting a hole through a box."""
        box = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        cylinder = mod3d.BRepBuilderAPI.MakeCylinder(2.0, 15.0).shape()
        
        # Position cylinder through box
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(5.0, 5.0, -2.5))
        cylinder = cylinder.moved(translation)
        
        cut = mod3d.BooleanOp.Cut(box, cylinder)
        result = cut.shape()
        
        assert not result.is_null()
        assert not cut.has_errors()
        
    def test_cut_disjoint_shapes(self):
        """Test cutting with disjoint tool (should return original)."""
        box = mod3d.BRepBuilderAPI.MakeBox(5.0, 5.0, 5.0).shape()
        tool = mod3d.BRepBuilderAPI.MakeBox(2.0, 2.0, 2.0).shape()
        
        # Move tool away
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(20.0, 0.0, 0.0))
        tool = tool.moved(translation)
        
        cut = mod3d.BooleanOp.Cut(box, tool)
        result = cut.shape()
        
        assert not result.is_null()
        assert not cut.has_errors()
        
    def test_cut_corner_from_box(self):
        """Test cutting a corner from a box."""
        box = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        corner = mod3d.BRepBuilderAPI.MakeBox(5.0, 5.0, 5.0).shape()
        
        cut = mod3d.BooleanOp.Cut(box, corner)
        result = cut.shape()
        
        assert not result.is_null()
        assert not cut.has_errors()
        
    def test_cut_non_destructive(self):
        """Test cut operation in non-destructive mode."""
        box = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        sphere = mod3d.BRepBuilderAPI.MakeSphere(5.0).shape()
        
        # Move sphere to intersect
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(5.0, 5.0, 5.0))
        sphere = sphere.moved(translation)
        
        cut = mod3d.BooleanOp.Cut(box, sphere)
        cut.non_destructive = True
        result = cut.shape()
        
        assert not result.is_null()
        assert cut.non_destructive


class TestBooleanFuse:
    """Tests for BRepAlgoAPI_Fuse (union) operation."""
    
    def test_fuse_two_touching_boxes(self):
        """Test fusing two touching boxes."""
        box1 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        box2 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        
        # Move box2 to touch box1
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(10.0, 0.0, 0.0))
        box2 = box2.moved(translation)
        
        fuse = mod3d.BooleanOp.Fuse(box1, box2)
        result = fuse.shape()
        
        assert not result.is_null()
        assert not fuse.has_errors()
        
    def test_fuse_overlapping_spheres(self):
        """Test fusing two overlapping spheres."""
        sphere1 = mod3d.BRepBuilderAPI.MakeSphere(5.0).shape()
        sphere2 = mod3d.BRepBuilderAPI.MakeSphere(5.0).shape()
        
        # Move sphere2 to overlap
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(6.0, 0.0, 0.0))
        sphere2 = sphere2.moved(translation)
        
        fuse = mod3d.BooleanOp.Fuse(sphere1, sphere2)
        result = fuse.shape()
        
        assert not result.is_null()
        assert not fuse.has_errors()
        
    def test_fuse_box_cylinder(self):
        """Test fusing a box and cylinder."""
        box = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        cylinder = mod3d.BRepBuilderAPI.MakeCylinder(3.0, 10.0).shape()
        
        # Position cylinder on top of box
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(5.0, 5.0, 10.0))
        cylinder = cylinder.moved(translation)
        
        fuse = mod3d.BooleanOp.Fuse(box, cylinder)
        result = fuse.shape()
        
        assert not result.is_null()
        assert not fuse.has_errors()
        
    def test_fuse_with_simplification(self):
        """Test fusing with result simplification."""
        box1 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        box2 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(10.0, 0.0, 0.0))
        box2 = box2.moved(translation)
        
        fuse = mod3d.BooleanOp.Fuse(box1, box2)
        fuse.simplify_result(unify_edges=True, unify_faces=True)
        result = fuse.shape()
        
        assert not result.is_null()


class TestBooleanSection:
    """Tests for BRepAlgoAPI_Section (intersection curve) operation."""
    
    def test_section_two_boxes(self):
        """Test section between two overlapping boxes."""
        box1 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        box2 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        
        # Move box2 to intersect
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(5.0, 5.0, 5.0))
        box2 = box2.moved(translation)
        
        section = mod3d.BooleanOp.Section(box1, box2)
        result = section.shape()
        
        assert not result.is_null()
        assert not section.has_errors()
        
    def test_section_sphere_plane(self):
        """Test section between sphere and plane (creates circle)."""
        sphere = mod3d.BRepBuilderAPI.MakeSphere(10.0).shape()
        
        # Create a plane as a large face
        plane = mod3d.BRepBuilderAPI.MakeFace(
            mod3d.gp.Pln(mod3d.gp.Pnt(0, 0, 0), mod3d.gp.Dir(0, 0, 1)),
            -20.0, 20.0, -20.0, 20.0
        ).shape()
        
        section = mod3d.BooleanOp.Section(sphere, plane)
        result = section.shape()
        
        assert not result.is_null()
        assert not section.has_errors()
    # @pytest.skip()
    # def test_section_two_cylinders(self):
    #     """Test section between two intersecting cylinders."""
    #     cyl1 = mod3d.BRepBuilderAPI.MakeCylinder(5.0, 20.0).shape()
    #     cyl2 = mod3d.BRepBuilderAPI.MakeCylinder(5.0, 20.0).shape()
        
    #     # Rotate second cylinder 90 degrees
    #     rotation = mod3d.gp.Trsf()
    #     rotation.set_rotation(
    #         mod3d.gp.Ax1(mod3d.gp.Pnt(0, 0, 0), mod3d.gp.Dir(0, 1, 0)),
    #         1.5708  # 90 degrees in radians
    #     )
    #     cyl2 = cyl2.moved(rotation)
        
    #     section = mod3d.BooleanOp.Section(cyl1, cyl2, False)
    #     section.approximation(True)
    #     section.compute_pcurve_on1(True)
    #     section.compute_pcurve_on2(True)
    #     section.build()
    #     result = section.shape()
        
    #     assert not result.is_null()
        
    def test_section_disjoint_shapes(self):
        """Test section of disjoint shapes (should be empty)."""
        box1 = mod3d.BRepBuilderAPI.MakeBox(5.0, 5.0, 5.0).shape()
        box2 = mod3d.BRepBuilderAPI.MakeBox(5.0, 5.0, 5.0).shape()
        
        # Move box2 far away
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(20.0, 0.0, 0.0))
        box2 = box2.moved(translation)
        
        section = mod3d.BooleanOp.Section(box1, box2)
        result = section.shape()
        
        # Result should be null or empty
        assert result.is_null() or result.shape_type == mod3d.TopAbs.ShapeEnum.COMPOUND


class TestBooleanEdgeCases:
    """Tests for edge cases and error handling."""
    
    def test_self_intersection(self):
        """Test Boolean operation on same shape."""
        box = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        
        # Common of shape with itself should give the shape back
        common = mod3d.BooleanOp.Common(box, box)
        result = common.shape()
        
        assert not result.is_null()
        
    def test_parallel_processing(self):
        """Test enabling parallel processing."""
        box1 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        box2 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(5.0, 0.0, 0.0))
        box2 = box2.moved(translation)
        
        fuse = mod3d.BooleanOp.Fuse(box1, box2)
        fuse.run_parallel = True
        result = fuse.shape()
        
        assert fuse.run_parallel
        assert not result.is_null()
        
    def test_check_inverted_solids(self):
        """Test inverted solids check option."""
        box1 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        box2 = mod3d.BRepBuilderAPI.MakeBox(5.0, 5.0, 5.0).shape()
        
        fuse = mod3d.BooleanOp.Fuse(box1, box2)
        fuse.check_inverted = True
        result = fuse.shape()
        
        assert fuse.check_inverted
        assert not result.is_null()
        
    def test_section_edges(self):
        """Test getting section edges from Boolean operation."""
        box1 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        box2 = mod3d.BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        
        translation = mod3d.gp.Trsf()
        translation.set_translation(mod3d.gp.Vec(5.0, 0.0, 0.0))
        box2 = box2.moved(translation)
        
        fuse = mod3d.BooleanOp.Fuse(box1, box2)
        result = fuse.shape()
        
        # Get section edges
        edges = fuse.section_edges()
        assert edges is not None


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
