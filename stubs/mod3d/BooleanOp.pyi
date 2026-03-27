"""
BRep boolean operations module
"""
from __future__ import annotations
import mod3d.BRepBuilderAPI
import mod3d.TopoDS
import mod3d.gp
import typing
__all__: list[str] = ['Algo', 'BuilderAlgo', 'Common', 'Cut', 'Full', 'Fuse', 'GlueEnum', 'Off', 'Section', 'Shift', 'revolve_cut']
class Algo(mod3d.BRepBuilderAPI.MakeShape):
    """
    Root interface for Boolean operation algorithms.
            
            This is the base class providing common functionality for all Boolean
            and general fuse algorithms in OCCT. Note: This class has a protected
            constructor and cannot be instantiated directly - use derived classes.
    """
    @staticmethod
    def get_parallel_mode() -> bool:
        """
        Gets the global parallel mode setting
        """
    @staticmethod
    def set_parallel_mode(new_mode: bool) -> None:
        """
        Sets the global parallel mode setting
        """
    def clear(self) -> None:
        """
        Clears all warnings and errors, and any cached data
        """
    def clear_warnings(self) -> None:
        """
        Clears all warnings
        """
    def has_errors(self) -> bool:
        """
        Returns True if algorithm has failed with errors
        """
    def has_warnings(self) -> bool:
        """
        Returns True if algorithm has generated warnings
        """
    def set_use_obb(self, use_obb: bool) -> None:
        """
        Enables/disables the usage of Oriented Bounding Boxes
        """
    def shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the result shape built by the algorithm
        """
    @property
    def fuzzy_value(self) -> float:
        """
        Additional tolerance for operations (read/write)
        """
    @fuzzy_value.setter
    def fuzzy_value(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def run_parallel(self) -> bool:
        """
        Enable/disable parallel processing (read/write)
        """
    @run_parallel.setter
    def run_parallel(self, arg1: bool) -> None:
        ...
class BuilderAlgo(Algo):
    """
    General Fuse algorithm for Boolean operations.
            
            This class provides advanced Boolean operations with support for
            multiple arguments, non-manifold topology, and sophisticated error handling.
            
            Key features:
            - Safe processing mode (non-destructive)
            - Gluing options for performance optimization
            - Check for inverted solids
            - History collection of shape modifications
    """
    def __init__(self) -> None:
        """
        Default constructor
        """
    def arguments(self) -> list:
        """
        Returns the list of arguments
        """
    def build(self, arg0: Message_ProgressRange) -> None:
        """
        Performs the algorithm and builds the result shape
        """
    def generated(self, shape: mod3d.TopoDS.Shape) -> list:
        """
        Returns shapes generated from the given shape.
                    
                    Only Edges and Faces can have generated elements:
                    - Edges can generate new vertices
                    - Faces can generate new edges and vertices
                    
                    Parameters:
                    -----------
                    shape : TopoDS_Shape
                        Original shape
                        
                    Returns:
                    --------
                    list[TopoDS_Shape]
                        List of generated shapes
        """
    def has_deleted(self) -> bool:
        """
        Returns True if any input shape has been deleted during operation
        """
    def has_generated(self) -> bool:
        """
        Returns True if any input shape has generated shapes during operation
        """
    def has_modified(self) -> bool:
        """
        Returns True if any input shape has been modified during operation
        """
    def is_deleted(self, shape: mod3d.TopoDS.Shape) -> bool:
        """
        Checks if the shape has been completely removed from the result.
                    
                    Parameters:
                    -----------
                    shape : TopoDS_Shape
                        Shape to check
                        
                    Returns:
                    --------
                    bool
                        True if the shape has been deleted
        """
    def modified(self, shape: mod3d.TopoDS.Shape) -> list:
        """
        Returns shapes modified from the given shape.
                    
                    Parameters:
                    -----------
                    shape : TopoDS_Shape
                        Original shape
                        
                    Returns:
                    --------
                    list[TopoDS_Shape]
                        List of modified shapes (splits) contained in the result
        """
    def section_edges(self) -> list:
        """
        Returns a list of section edges.
                    
                    The edges represent the result of intersection between arguments
                    of the operation.
                    
                    Returns:
                    --------
                    TopTools_ListOfShape
                        List of section edges
        """
    def set_arguments(self, arguments: list) -> None:
        """
        Sets the list of arguments (shapes) for the operation.
                    
                    Parameters:
                    -----------
                    arguments : list[TopoDS_Shape]
                    List of input shapes
        """
    def set_glue(self, glue: GlueEnum) -> None:
        """
        Sets the glue option for the algorithm.
                    
                    Allows increasing performance of intersection when some sub-shapes
                    are coinciding.
                    
                    Parameters:
                    -----------
                    glue : BOPAlgo_GlueEnum
                        Gluing mode setting
        """
    def set_non_destructive(self, flag: bool) -> None:
        """
        Sets the flag for non-destructive mode.
                    
                    In non-destructive mode, input shapes are not modified. Instead,
                    a copy of a sub-shape is created in the result if needed.
                    
                    Parameters:
                    -----------
                    flag : bool
                        True to preserve input shapes
        """
    def shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the result shape built by the algorithm
        """
    def simplify_result(self, unify_edges: bool = True, unify_faces: bool = True, angular_tol: typing.SupportsFloat = 1e-12) -> None:
        """
        Simplifies the result by unifying tangential edges and faces.
                    
                    Uses ShapeUpgrade_UnifySameDomain algorithm to simplify the result
                    without breaking shape connections. Works on the whole result shape,
                    including unmodified connected tangent edges/faces from input.
                    
                    Parameters:
                    -----------
                    unify_edges : bool
                        Controls edge unification (default: True)
                    unify_faces : bool
                        Controls face unification (default: True)
                    angular_tol : float
                        Angular criteria for tangency (default: Precision::Angular()
        """
    @property
    def check_inverted(self) -> bool:
        """
        Enable/disable check for inverted solids (read/write)
        """
    @check_inverted.setter
    def check_inverted(self, arg1: bool) -> None:
        ...
    @property
    def fill_history(self) -> bool:
        """
        Enable/disable history collection (read/write)
        """
    @fill_history.setter
    def fill_history(self, arg1: bool) -> None:
        ...
    @property
    def glue(self) -> GlueEnum:
        """
        Gluing mode for performance optimization (read/write)
        """
    @glue.setter
    def glue(self, arg1: GlueEnum) -> None:
        ...
    @property
    def non_destructive(self) -> bool:
        """
        Enable/disable non-destructive mode (read/write)
        """
    @non_destructive.setter
    def non_destructive(self, arg1: bool) -> None:
        ...
class Common(BuilderAlgo):
    """
    Boolean INTERSECTION operation.
            
            Computes the common (intersection) part between two or more shapes.
            The result contains only the volume/area that exists in ALL input shapes.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Default constructor
        """
    @typing.overload
    def __init__(self, shape1: mod3d.TopoDS.Shape, shape2: mod3d.TopoDS.Shape) -> None:
        """
        Constructor with two shapes.
                    
                    Parameters:
                    -----------
                    shape1 : TopoDS_Shape
                        First shape
                    shape2 : TopoDS_Shape
                        Second shape
        """
class Cut(BuilderAlgo):
    """
    Boolean SUBTRACTION (difference) operation.
            
            Subtracts the tool shape(s) from the object shape.
            The result is the part of the object that does NOT overlap with the tools.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Default constructor
        """
    @typing.overload
    def __init__(self, shape: mod3d.TopoDS.Shape, tool: mod3d.TopoDS.Shape) -> None:
        """
        Constructor with object and tool shapes.
                    
                    Parameters:
                    -----------
                    shape : TopoDS_Shape
                        Object shape to cut from
                    tool : TopoDS_Shape
                        Tool shape to subtract
        """
class Fuse(BuilderAlgo):
    """
    Boolean UNION operation.
            
            Fuses (merges) two or more shapes into a single shape.
            The result contains the combined volume/area of all input shapes.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Default constructor
        """
    @typing.overload
    def __init__(self, shape1: mod3d.TopoDS.Shape, shape2: mod3d.TopoDS.Shape) -> None:
        """
        Constructor with two shapes.
                    
                    Parameters:
                    -----------
                    shape1 : TopoDS_Shape
                        First shape
                    shape2 : TopoDS_Shape
                        Second shape
        """
class GlueEnum:
    """
    Enumeration for gluing options in Boolean operations
    
    Members:
    
      Off : No gluing - default
    
      Shift : Gluing enabled for partial coincident sub-shapes
    
      Full : Gluing enabled for full coincident sub-shapes
    """
    Full: typing.ClassVar[GlueEnum]  # value = <GlueEnum.Full: 2>
    Off: typing.ClassVar[GlueEnum]  # value = <GlueEnum.Off: 0>
    Shift: typing.ClassVar[GlueEnum]  # value = <GlueEnum.Shift: 1>
    __members__: typing.ClassVar[dict[str, GlueEnum]]  # value = {'Off': <GlueEnum.Off: 0>, 'Shift': <GlueEnum.Shift: 1>, 'Full': <GlueEnum.Full: 2>}
    def __eq__(self, other: typing.Any) -> bool:
        ...
    def __getstate__(self) -> int:
        ...
    def __hash__(self) -> int:
        ...
    def __index__(self) -> int:
        ...
    def __init__(self, value: typing.SupportsInt) -> None:
        ...
    def __int__(self) -> int:
        ...
    def __ne__(self, other: typing.Any) -> bool:
        ...
    def __repr__(self) -> str:
        ...
    def __setstate__(self, state: typing.SupportsInt) -> None:
        ...
    def __str__(self) -> str:
        ...
    @property
    def name(self) -> str:
        ...
    @property
    def value(self) -> int:
        ...
class Section(BuilderAlgo):
    """
    Computes the intersection curve/edge between shapes.
            
            Unlike Common which gives the volume/area, Section computes only the
            curve/edge where the shapes intersect.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Default constructor
        """
    @typing.overload
    def __init__(self, shape1: mod3d.TopoDS.Shape, shape2: mod3d.TopoDS.Shape) -> None:
        """
        Constructor with two shapes.
                    
                    Parameters:
                    -----------
                    shape1 : TopoDS_Shape
                        First shape
                    shape2 : TopoDS_Shape
                        Second shape
        """
    @typing.overload
    def __init__(self, shape1: mod3d.TopoDS.Shape, shape2: mod3d.TopoDS.Shape, perform_now: bool) -> None:
        """
        Constructor with option to compute immediately.
                    
                    Parameters:
                    -----------
                    shape1 : TopoDS_Shape
                        First shape
                    shape2 : TopoDS_Shape
                        Second shape
                    perform_now : bool
                        If True, compute the section immediately
        """
    def approximation(self, flag: bool) -> None:
        """
        Sets whether to approximate intersection curves.
                    
                    Parameters:
                    -----------
                    flag : bool
                        True to enable approximation
        """
    def compute_pcurve_on1(self, flag: bool) -> None:
        """
        Sets whether to compute parametric curves on the first shape.
                    
                    Parameters:
                    -----------
                    flag : bool
                        True to compute parametric curves
        """
    def compute_pcurve_on2(self, flag: bool) -> None:
        """
        Sets whether to compute parametric curves on the second shape.
                    
                    Parameters:
                    -----------
                    flag : bool
                        True to compute parametric curves
        """
@typing.overload
def revolve_cut(shape: mod3d.TopoDS.Shape, profile: mod3d.TopoDS.Face, axis: mod3d.gp.Ax1) -> mod3d.TopoDS.Shape:
    """
    Performs a revolve cut operation.
        Parameters:
        -----------
        shape : TopoDS_Shape
            Object shape to cut from
        profile : TopoDS_Face
            Face profile to revolve
        axis : gp_Ax1
            Axis of revolutio
    """
@typing.overload
def revolve_cut(shape: mod3d.TopoDS.Shape, profile: mod3d.TopoDS.Face, axis: mod3d.gp.Ax1, angle: typing.SupportsFloat) -> mod3d.TopoDS.Shape:
    """
    Performs a revolve cut operation.
        Parameters:
        -----------
        shape : TopoDS_Shape
            Object shape to cut from
        profile : TopoDS_Face
            Face profile to revolve
        axis : gp_Ax1
            Axis of revolution
        angle : float
            Angle of revolution in radians
    """
@typing.overload
def revolve_cut(shape: mod3d.TopoDS.Shape, profile: mod3d.TopoDS.Wire, axis: mod3d.gp.Ax1) -> mod3d.TopoDS.Shape:
    """
    Performs a revolve cut operation.
    
        Parameters:
        -----------
        shape : TopoDS_Shape
            Object shape to cut from
        profile : TopoDS_Wire
            Wire profile to revolve
        axis : gp_Ax1
            Axis of revolution in radians
    """
@typing.overload
def revolve_cut(shape: mod3d.TopoDS.Shape, profile: mod3d.TopoDS.Wire, axis: mod3d.gp.Ax1, angle: typing.SupportsFloat) -> mod3d.TopoDS.Shape:
    """
    Performs a revolve cut operation.
        
        Parameters:
        -----------
        shape : TopoDS_Shape
            Object shape to cut from
        profile : TopoDS_Wire
            Wire profile to revolve
        axis : gp_Ax1
            Axis of revolution
        angle : float
            Angle of revolution in radians
    """
Full: GlueEnum  # value = <GlueEnum.Full: 2>
Off: GlueEnum  # value = <GlueEnum.Off: 0>
Shift: GlueEnum  # value = <GlueEnum.Shift: 1>
