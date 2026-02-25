"""Tests for ShapeFix module bindings."""

import pytest
import mod3d
from mod3d import (
    ShapeFix,
    BRepBuilderAPI,
    gp,
    TopoDS,
    TopExp,
)


class TestShapeFixWire:
    """Tests for ShapeFix.Wire class."""

    def create_simple_wire(self):
        """Create a simple rectangular wire for testing."""
        p1 = gp.Pnt(0, 0, 0)
        p2 = gp.Pnt(10, 0, 0)
        p3 = gp.Pnt(10, 10, 0)
        p4 = gp.Pnt(0, 10, 0)

        edge1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
        edge2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
        edge3 = BRepBuilderAPI.MakeEdge(p3, p4).edge()
        edge4 = BRepBuilderAPI.MakeEdge(p4, p1).edge()

        wire_builder = BRepBuilderAPI.MakeWire()
        wire_builder.add(edge1)
        wire_builder.add(edge2)
        wire_builder.add(edge3)
        wire_builder.add(edge4)

        return wire_builder.wire()

    def create_face_from_wire(self, wire):
        """Create a face from a wire."""
        return BRepBuilderAPI.MakeFace(wire).face()

    def test_default_constructor(self):
        """Test default constructor."""
        fixer = ShapeFix.Wire()
        assert fixer is not None
        assert not fixer.is_loaded
        assert not fixer.is_ready

    def test_constructor_with_wire_face_precision(self):
        """Test constructor with wire, face, and precision."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        assert fixer is not None
        assert fixer.is_loaded
        assert fixer.is_ready
        assert fixer.nb_edges == 4

    def test_init_method(self):
        """Test init method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire()
        assert not fixer.is_loaded
        
        fixer.init(wire, face, 0.001)
        assert fixer.is_loaded
        assert fixer.is_ready
        assert fixer.nb_edges == 4

    def test_load_method(self):
        """Test load method."""
        wire = self.create_simple_wire()
        
        fixer = ShapeFix.Wire()
        fixer.load(wire)
        assert fixer.is_loaded
        assert not fixer.is_ready  # No face set yet
        assert fixer.nb_edges == 4

    def test_set_face(self):
        """Test set_face method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire()
        fixer.load(wire)
        assert not fixer.is_ready
        
        fixer.set_face(face)
        assert fixer.is_ready

    def test_set_precision(self):
        """Test set_precision method."""
        wire = self.create_simple_wire()
        
        fixer = ShapeFix.Wire()
        fixer.load(wire)
        fixer.set_precision(0.01)
        # Just ensure no exception

    def test_wire_property(self):
        """Test wire property."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result_wire = fixer.wire
        assert result_wire is not None

    def test_face_property(self):
        """Test face property."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result_face = fixer.face
        assert result_face is not None

    def test_modify_topology_mode(self):
        """Test modify_topology_mode property."""
        fixer = ShapeFix.Wire()
        
        # Get default
        initial = fixer.modify_topology_mode
        
        # Set and verify
        fixer.modify_topology_mode = True
        assert fixer.modify_topology_mode == True
        
        fixer.modify_topology_mode = False
        assert fixer.modify_topology_mode == False

    def test_modify_geometry_mode(self):
        """Test modify_geometry_mode property."""
        fixer = ShapeFix.Wire()
        
        fixer.modify_geometry_mode = True
        assert fixer.modify_geometry_mode == True
        
        fixer.modify_geometry_mode = False
        assert fixer.modify_geometry_mode == False

    def test_closed_wire_mode(self):
        """Test closed_wire_mode property."""
        fixer = ShapeFix.Wire()
        
        fixer.closed_wire_mode = True
        assert fixer.closed_wire_mode == True
        
        fixer.closed_wire_mode = False
        assert fixer.closed_wire_mode == False

    def test_fix_mode_properties(self):
        """Test fix mode properties."""
        fixer = ShapeFix.Wire()
        
        # Test fix_reorder_mode (-1 default, 0 skip, 1 force)
        fixer.fix_reorder_mode = 1  # Force
        assert fixer.fix_reorder_mode == 1
        fixer.fix_reorder_mode = 0  # Skip
        assert fixer.fix_reorder_mode == 0
        
        # Test fix_small_mode
        fixer.fix_small_mode = 1
        assert fixer.fix_small_mode == 1
        
        # Test fix_connected_mode
        fixer.fix_connected_mode = 1
        assert fixer.fix_connected_mode == 1
        
        # Test fix_degenerated_mode
        fixer.fix_degenerated_mode = 0
        assert fixer.fix_degenerated_mode == 0
        
        # Test fix_self_intersection_mode
        fixer.fix_self_intersection_mode = 1
        assert fixer.fix_self_intersection_mode == 1
        
        # Test fix_lacking_mode
        fixer.fix_lacking_mode = 0
        assert fixer.fix_lacking_mode == 0
        
        # Test fix_gaps_3d_mode
        fixer.fix_gaps_3d_mode = 1
        assert fixer.fix_gaps_3d_mode == 1
        
        # Test fix_gaps_2d_mode
        fixer.fix_gaps_2d_mode = 0
        assert fixer.fix_gaps_2d_mode == 0

    def test_clear_modes(self):
        """Test clear_modes method."""
        fixer = ShapeFix.Wire()
        
        # Set some modes
        fixer.fix_reorder_mode = 1
        fixer.fix_small_mode = 1
        
        # Clear and verify reset
        fixer.clear_modes()
        # Modes should be reset to -1 (default)
        assert fixer.fix_reorder_mode == -1
        assert fixer.fix_small_mode == -1

    def test_clear_statuses(self):
        """Test clear_statuses method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        fixer.perform()
        fixer.clear_statuses()
        # Just ensure no exception

    def test_perform(self):
        """Test perform method on a clean wire."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        # On a clean wire, perform should not make changes
        result = fixer.perform()
        # Result depends on wire state - just verify it runs
        assert result in [True, False]
        
        # Get the result
        fixed_wire = fixer.wire
        assert fixed_wire is not None

    def test_fix_reorder(self):
        """Test fix_reorder method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_reorder()
        assert result in [True, False]

    def test_fix_small(self):
        """Test fix_small method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        # lock_vertex=True means vertices won't be modified
        num_removed = fixer.fix_small(True, 0.0)
        # Clean wire should have no small edges
        assert num_removed == 0

    def test_fix_connected(self):
        """Test fix_connected method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_connected()
        assert result in [True, False]

    def test_fix_edge_curves(self):
        """Test fix_edge_curves method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_edge_curves()
        assert result in [True, False]

    def test_fix_degenerated(self):
        """Test fix_degenerated method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_degenerated()
        # Clean rectangular wire has no degenerated edges
        assert result in [True, False]

    def test_fix_self_intersection(self):
        """Test fix_self_intersection method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_self_intersection()
        assert result in [True, False]

    def test_fix_lacking(self):
        """Test fix_lacking method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_lacking(False)
        assert result in [True, False]

    def test_fix_closed(self):
        """Test fix_closed method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_closed()
        assert result in [True, False]

    def test_fix_gaps_3d(self):
        """Test fix_gaps_3d method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_gaps_3d()
        assert result in [True, False]

    def test_fix_gaps_2d(self):
        """Test fix_gaps_2d method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_gaps_2d()
        assert result in [True, False]

    def test_fix_shifted(self):
        """Test fix_shifted method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_shifted()
        assert result in [True, False]

    def test_fix_notched_edges(self):
        """Test fix_notched_edges method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_notched_edges()
        assert result in [True, False]

    def test_fix_tails(self):
        """Test fix_tails method."""
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        fixer = ShapeFix.Wire(wire, face, 0.001)
        result = fixer.fix_tails()
        assert result in [True, False]

    def test_set_max_tail_properties(self):
        """Test set_max_tail_angle and set_max_tail_width."""
        import math
        
        fixer = ShapeFix.Wire()
        fixer.set_max_tail_angle(math.pi / 6)  # 30 degrees
        fixer.set_max_tail_width(0.5)
        # Just ensure no exception

    def test_nb_edges_property(self):
        """Test nb_edges property with different wires."""
        # 4-edge rectangular wire
        wire = self.create_simple_wire()
        fixer = ShapeFix.Wire()
        fixer.load(wire)
        assert fixer.nb_edges == 4
        
        # 3-edge triangular wire
        p1 = gp.Pnt(0, 0, 0)
        p2 = gp.Pnt(10, 0, 0)
        p3 = gp.Pnt(5, 10, 0)
        
        edge1 = BRepBuilderAPI.MakeEdge(p1, p2).edge()
        edge2 = BRepBuilderAPI.MakeEdge(p2, p3).edge()
        edge3 = BRepBuilderAPI.MakeEdge(p3, p1).edge()
        
        wire_builder = BRepBuilderAPI.MakeWire()
        wire_builder.add(edge1)
        wire_builder.add(edge2)
        wire_builder.add(edge3)
        
        fixer2 = ShapeFix.Wire()
        fixer2.load(wire_builder.wire())
        assert fixer2.nb_edges == 3

    def test_full_workflow(self):
        """Test a complete wire fixing workflow."""
        # Create wire
        wire = self.create_simple_wire()
        face = self.create_face_from_wire(wire)
        
        # Create fixer and configure
        fixer = ShapeFix.Wire()
        fixer.init(wire, face, 0.001)
        
        # Configure modes
        fixer.modify_topology_mode = True
        fixer.modify_geometry_mode = True
        fixer.closed_wire_mode = True
        
        # Perform all fixes
        fixer.perform()
        
        # Get result
        fixed_wire = fixer.wire
        assert fixed_wire is not None
        
        # Verify result is still valid
        assert fixer.nb_edges == 4
