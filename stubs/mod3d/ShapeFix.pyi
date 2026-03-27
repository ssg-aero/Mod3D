"""
Shape fixing module
"""
from __future__ import annotations
import mod3d.TopoDS
import typing
__all__: list[str] = ['Shape', 'Wire']
class Shape:
    """
    
            A tool for repairing shapes in general.
    
            ShapeFix_Shape iterates on sub-shapes and performs fixes on each level:
            solids, shells, faces, wires, and edges.
    
            The fixes can be controlled via mode properties (fix_solid_mode, etc.).
            Use perform() to run all applicable fixes.
    
            Example:
                fixer = ShapeFix.Shape(shape)
                fixer.perform()
                fixed_shape = fixer.shape
    
            Or with configuration:
                fixer = ShapeFix.Shape()
                fixer.init(shape)
                fixer.set_precision(0.001)
                fixer.fix_solid_mode = 1  # Force
                fixer.perform()
                fixed_shape = fixer.shape
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty shape fixer.
        """
    @typing.overload
    def __init__(self, shape: mod3d.TopoDS.Shape) -> None:
        """
        Creates a shape fixer initialized with the given shape.
        """
    def init(self, shape: mod3d.TopoDS.Shape) -> None:
        """
        Initializes with a shape.
        """
    def perform(self) -> bool:
        """
        Iterates on sub-shapes and performs fixes.
        
        Returns True if any fix was applied.
        """
    def set_max_tolerance(self, max_tolerance: typing.SupportsFloat) -> None:
        """
        Sets maximal allowed tolerance.
        """
    def set_min_tolerance(self, min_tolerance: typing.SupportsFloat) -> None:
        """
        Sets minimal allowed tolerance.
        """
    def set_precision(self, precision: typing.SupportsFloat) -> None:
        """
        Sets the basic precision value.
        """
    @property
    def fix_free_face_mode(self) -> int:
        """
        Mode for fixing free faces (-1 default, 0 skip, 1 force). Default True.
        """
    @fix_free_face_mode.setter
    def fix_free_face_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_free_shell_mode(self) -> int:
        """
        Mode for fixing free shells (-1 default, 0 skip, 1 force). Default True.
        """
    @fix_free_shell_mode.setter
    def fix_free_shell_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_free_wire_mode(self) -> int:
        """
        Mode for fixing free wires (-1 default, 0 skip, 1 force). Default True.
        """
    @fix_free_wire_mode.setter
    def fix_free_wire_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_same_parameter_mode(self) -> int:
        """
        Mode for applying SameParameter after all fixes (-1 default, 0 skip, 1 force). Default True.
        """
    @fix_same_parameter_mode.setter
    def fix_same_parameter_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_solid_mode(self) -> int:
        """
        Mode for fixing solids (-1 default, 0 skip, 1 force). Default True.
        """
    @fix_solid_mode.setter
    def fix_solid_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_vertex_position_mode(self) -> int:
        """
        Mode for fixing vertex positions before all fixes (-1 default, 0 skip, 1 force). Default False.
        """
    @fix_vertex_position_mode.setter
    def fix_vertex_position_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_vertex_tol_mode(self) -> int:
        """
        Mode for fixing tolerances of vertices on whole shape after all fixes.
        """
    @fix_vertex_tol_mode.setter
    def fix_vertex_tol_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the resulting fixed shape.
        """
class Wire:
    """
    
            A tool for repairing wires.
    
            ShapeFix_Wire provides methods to fix various problems in wires:
            - Reordering edges
            - Fixing small edges
            - Fixing connectivity between edges
            - Fixing degenerated edges
            - Fixing self-intersections
            - Fixing gaps between edges
            - And more...
    
            The fixes can be controlled via mode flags (Fix...Mode properties).
            Use perform() to run all applicable fixes, or call individual Fix...
            methods for specific repairs.
    
            Example:
                fixer = ShapeFix.Wire(wire, face, precision)
                fixer.perform()  # Run all fixes
                fixed_wire = fixer.wire
    
            Or with more control:
                fixer = ShapeFix.Wire()
                fixer.init(wire, face, 0.001)
                fixer.fix_reorder()
                fixer.fix_connected()
                fixer.fix_degenerated()
                fixed_wire = fixer.wire
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty wire fixer with default flags.
        """
    @typing.overload
    def __init__(self, wire: mod3d.TopoDS.Wire, face: mod3d.TopoDS.Face, precision: typing.SupportsFloat) -> None:
        """
        Creates a wire fixer and loads data for the wire and face.
        """
    def clear_modes(self) -> None:
        """
        Sets all modes to default values.
        """
    def clear_statuses(self) -> None:
        """
        Clears all statuses.
        """
    def fix_closed(self, precision: typing.SupportsFloat = -1.0) -> bool:
        """
        Fixes wire to be well closed.
        
        Performs FixConnected, FixDegenerated, and FixLacking between
        last and first edges.
        
        Returns True if any fix was applied.
        """
    def fix_connected(self, precision: typing.SupportsFloat = -1.0) -> bool:
        """
        Fixes connections between adjacent edges.
        
        Forces vertices to be shared between adjacent edges.
        If precision is -1, MaxTolerance is used.
        
        Returns True if any fix was applied.
        """
    def fix_degenerated(self) -> bool:
        """
        Fixes degenerated edges in the wire.
        
        Returns True if any fix was applied.
        """
    def fix_edge_curves(self) -> bool:
        """
        Fixes issues with 3D and pcurves of edges.
        
        Returns True if any fix was applied.
        """
    def fix_gaps_2d(self) -> bool:
        """
        Fixes gaps between ends of pcurves on adjacent edges.
        
        Returns True if any fix was applied.
        """
    def fix_gaps_3d(self) -> bool:
        """
        Fixes gaps between ends of 3D curves on adjacent edges.
        
        Returns True if any fix was applied.
        """
    def fix_lacking(self, force: bool = False) -> bool:
        """
        Fixes lacking edges (gaps in 2D while connected in 3D).
        
        Parameters:
          force: If True, uses minimal tolerance for gap detection
        
        Returns True if edge was added or tolerance was increased.
        """
    def fix_notched_edges(self) -> bool:
        """
        Fixes notched edges.
        
        Returns True if any fix was applied.
        """
    def fix_reorder(self, mode_both: bool = False) -> bool:
        """
        Reorders edges in the wire using class WireOrder.
        
        Returns True if reordering was performed.
        """
    def fix_self_intersection(self) -> bool:
        """
        Fixes self-intersecting edges and intersections between edges.
        
        Returns True if any fix was applied.
        """
    def fix_shifted(self) -> bool:
        """
        Fixes edges with pcurves shifted by parameter range on closed surface.
        
        Returns True if any fix was applied.
        """
    def fix_small(self, lock_vertex: bool, precision_small: typing.SupportsFloat = 0.0) -> int:
        """
        Removes small edges from the wire.
        
        Parameters:
          lock_vertex: If True, vertices are not modified
          precision_small: Threshold for small edges (0 = use default)
        
        Returns the number of removed edges.
        """
    def fix_tails(self) -> bool:
        """
        Fixes tails in the wire.
        
        Returns True if any fix was applied.
        """
    def init(self, wire: mod3d.TopoDS.Wire, face: mod3d.TopoDS.Face, precision: typing.SupportsFloat) -> None:
        """
        Loads analyzer with all data for the wire and face, and drops all fixing statuses.
        """
    def load(self, wire: mod3d.TopoDS.Wire) -> None:
        """
        Loads data for the wire and drops all fixing statuses.
        """
    def perform(self) -> bool:
        """
        Performs all available fixes based on mode flags.
        
        Returns True if any fix was applied.
        """
    def set_face(self, face: mod3d.TopoDS.Face) -> None:
        """
        Sets the working face for the wire.
        """
    def set_max_tail_angle(self, max_tail_angle: typing.SupportsFloat) -> None:
        """
        Sets the maximal allowed angle of the tails in radians.
        """
    def set_max_tail_width(self, max_tail_width: typing.SupportsFloat) -> None:
        """
        Sets the maximal allowed width of the tails.
        """
    def set_precision(self, precision: typing.SupportsFloat) -> None:
        """
        Sets working precision.
        """
    @property
    def closed_wire_mode(self) -> bool:
        """
        Flag defining whether the wire is to be closed.
        """
    @closed_wire_mode.setter
    def closed_wire_mode(self, arg1: bool) -> None:
        ...
    @property
    def face(self) -> mod3d.TopoDS.Face:
        """
        Returns the working face.
        """
    @property
    def fix_connected_mode(self) -> int:
        """
        Mode for FixConnected (-1 default, 0 skip, 1 force).
        """
    @fix_connected_mode.setter
    def fix_connected_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_degenerated_mode(self) -> int:
        """
        Mode for FixDegenerated (-1 default, 0 skip, 1 force).
        """
    @fix_degenerated_mode.setter
    def fix_degenerated_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_gaps_2d_mode(self) -> int:
        """
        Mode for FixGaps2d (-1 default, 0 skip, 1 force).
        """
    @fix_gaps_2d_mode.setter
    def fix_gaps_2d_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_gaps_3d_mode(self) -> int:
        """
        Mode for FixGaps3d (-1 default, 0 skip, 1 force).
        """
    @fix_gaps_3d_mode.setter
    def fix_gaps_3d_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_lacking_mode(self) -> int:
        """
        Mode for FixLacking (-1 default, 0 skip, 1 force).
        """
    @fix_lacking_mode.setter
    def fix_lacking_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_reorder_mode(self) -> int:
        """
        Mode for FixReorder (-1 default, 0 skip, 1 force).
        """
    @fix_reorder_mode.setter
    def fix_reorder_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_self_intersection_mode(self) -> int:
        """
        Mode for FixSelfIntersection (-1 default, 0 skip, 1 force).
        """
    @fix_self_intersection_mode.setter
    def fix_self_intersection_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def fix_small_mode(self) -> int:
        """
        Mode for FixSmall (-1 default, 0 skip, 1 force).
        """
    @fix_small_mode.setter
    def fix_small_mode(self, arg1: typing.SupportsInt) -> None:
        ...
    @property
    def is_loaded(self) -> bool:
        """
        Returns True if the wire is loaded.
        """
    @property
    def is_ready(self) -> bool:
        """
        Returns True if the wire and face are loaded.
        """
    @property
    def modify_geometry_mode(self) -> bool:
        """
        Flag defining whether geometry modification is allowed during fixing.
        """
    @modify_geometry_mode.setter
    def modify_geometry_mode(self, arg1: bool) -> None:
        ...
    @property
    def modify_topology_mode(self) -> bool:
        """
        Flag defining whether topology modification is allowed during fixing.
        """
    @modify_topology_mode.setter
    def modify_topology_mode(self, arg1: bool) -> None:
        ...
    @property
    def nb_edges(self) -> int:
        """
        Returns the number of edges in the working wire.
        """
    @property
    def wire(self) -> mod3d.TopoDS.Wire:
        """
        Returns the resulting wire (using basic BRep_Builder).
        """
    @property
    def wire_api_make(self) -> mod3d.TopoDS.Wire:
        """
        Returns the resulting wire (using BRepAPI_MakeWire).
        """
