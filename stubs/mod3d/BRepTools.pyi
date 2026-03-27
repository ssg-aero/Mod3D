"""
BRep tools module
"""
from __future__ import annotations
import mod3d.TopAbs
import mod3d.TopoDS
import mod3d.gp
import typing
__all__: list[str] = ['Quilt', 'ReShape', 'WireExplorer', 'is_closed', 'is_realy_closed', 'outer_wire', 'write']
class Quilt:
    """
    
            A tool to glue faces at common edges and reconstruct shells.
    
            The user designates pairs of common edges using the bind() method.
            One edge is designated as the edge to use in place of the other one
            (they are supposed to be geometrically confused, but this is not checked).
            They can be of opposite directions, specified by the orientations.
    
            The user can add shapes with the add() method. All faces are registered
            and copies of faces and edges are made to glue at the bound edges.
    
            The user can call shells() to compute a compound of shells from the
            current set of faces.
    
            If no binding is made, this class can be used to make shells from faces
            already sharing their edges.
    
            Example:
                quilt = BRepTools.Quilt()
                quilt.bind(edge_old, edge_new)  # bind edges to be merged
                quilt.add(face1)
                quilt.add(face2)
                result = quilt.shells()  # get the resulting shell compound
            
    """
    def __init__(self) -> None:
        """
        Creates an empty Quilt object.
        """
    def add(self, shape: mod3d.TopoDS.Shape) -> None:
        """
        Adds the faces of shape to the Quilt.
        
        Faces containing bound edges are copied.
        """
    @typing.overload
    def bind(self, edge_old: mod3d.TopoDS.Edge, edge_new: mod3d.TopoDS.Edge) -> None:
        """
        Binds edge_new to be the new edge instead of edge_old.
        
        The faces of the added shape containing edge_old will be copied
        to substitute edge_old by edge_new.
        
        The vertices of edge_old will be bound to the vertices of edge_new
        with the same orientation.
        
        If edge_old and edge_new have different orientations, the curves
        are considered opposite and the pcurves of edge_old will be copied
        and reversed in the new faces.
        
        Note: edge_old must belong to the next added shape, edge_new must
        belong to a shape added before.
        """
    @typing.overload
    def bind(self, vertex_old: mod3d.TopoDS.Vertex, vertex_new: mod3d.TopoDS.Vertex) -> None:
        """
        Binds vertex_new to be a new vertex instead of vertex_old.
        
        The faces of the added shape containing vertex_old will be copied
        to substitute vertex_old by vertex_new.
        """
    def copy(self, shape: mod3d.TopoDS.Shape) -> mod3d.TopoDS.Shape:
        """
        Returns the shape substituted to the given shape in the Quilt.
        """
    def is_copied(self, shape: mod3d.TopoDS.Shape) -> bool:
        """
        Returns True if shape has been copied.
        
        Shape can be a vertex, an edge, or a face.
        """
    def shells(self) -> mod3d.TopoDS.Shape:
        """
        Returns a compound of shells made from the current set of faces.
        
        The shells will be flagged as closed or not closed.
        """
class ReShape:
    """
    Rebuilds a shape by making pre-defined substitutions on some of its components.
    
    In a first phase, it records requests to replace or remove some individual shapes.
    For each shape, the last given request is recorded.
    
    Then, these requests may be applied to any shape which may contain one or more
    of these individual shapes.
    
    Typical usage:
      reshape = BRepTools.ReShape()
      reshape.replace(old_edge, new_edge)
      reshape.remove(face_to_remove)
      result = reshape.apply(original_shape)
    """
    def __init__(self) -> None:
        """
        Creates an empty ReShape object
        """
    def apply(self, shape: mod3d.TopoDS.Shape, until: mod3d.TopAbs.ShapeEnum = ...) -> mod3d.TopoDS.Shape:
        """
        Applies the substitution requests to a shape.
        
        The 'until' parameter gives the level of type until which requests
        are taken into account. For subshapes of the type 'until', no rebuild
        and further exploring are done.
        
        Note: Each subshape can be replaced by a shape of the same type or by
        a shape containing only shapes of that type. If incompatible shape type
        is encountered, it is ignored and flag FAIL1 is set in status
        """
    def clear(self) -> None:
        """
        Clears all substitution requests
        """
    @typing.overload
    def copy_vertex(self, vertex: mod3d.TopoDS.Vertex, tolerance: typing.SupportsFloat = -1.0) -> mod3d.TopoDS.Vertex:
        """
        Returns modified copy of vertex if original one is not recorded,
        or returns modified original vertex otherwise.
        
        Parameters:
          tolerance: New tolerance of vertex (optional, -1.0 keeps original)
        """
    @typing.overload
    def copy_vertex(self, vertex: mod3d.TopoDS.Vertex, new_position: mod3d.gp.Pnt, tolerance: typing.SupportsFloat) -> mod3d.TopoDS.Vertex:
        """
        Returns modified copy of vertex if original one is not recorded,
        or returns modified original vertex otherwise.
        
        Parameters:
          new_position: New position for vertex copy
          tolerance: New tolerance of vertex
        """
    def is_new_shape(self, shape: mod3d.TopoDS.Shape) -> bool:
        """
        Checks if shape has been recorded by reshaper as a value
        """
    def is_recorded(self, shape: mod3d.TopoDS.Shape) -> bool:
        """
        Tells if a shape is recorded for Replace/Remove
        """
    def remove(self, shape: mod3d.TopoDS.Shape) -> None:
        """
        Sets a request to remove a shape whatever the orientation
        """
    def replace(self, shape: mod3d.TopoDS.Shape, new_shape: mod3d.TopoDS.Shape) -> None:
        """
        Sets a request to replace a shape by a new one.
        
        The orientation of the replacing shape respects that of the original one
        """
    def status(self, shape: mod3d.TopoDS.Shape, last: bool = False) -> tuple:
        """
        Returns a complete substitution status for a shape.
        
        Returns: (status, new_shape) where:
          status = 0:  not recorded, new_shape = original shape
          status < 0:  to be removed, new_shape is null
          status > 0:  to be replaced, new_shape is the replacement
        
        Parameters:
          last: If False, returns status recorded directly for the shape.
                If True and status > 0, recursively searches for last status
        """
    def value(self, shape: mod3d.TopoDS.Shape) -> mod3d.TopoDS.Shape:
        """
        Returns the new value for an individual shape.
        
        If not recorded, returns the original shape itself.
        If to be removed, returns a null shape.
        Else, returns the replacing item
        """
    @property
    def history(self) -> BRepTools_History:
        """
        Returns the history of the substituted shapes
        """
    @property
    def mode_consider_location(self) -> bool:
        """
        Gets/sets whether location of shape is taken into account during replacing shapes
        """
    @mode_consider_location.setter
    def mode_consider_location(self, arg1: bool) -> None:
        ...
class WireExplorer:
    """
    
            Explores the edges of a wire in connection order.
    
            Each edge is connected to the previous one by its origin vertex.
            This is useful for traversing wire edges in their natural sequence.
    
            If the wire is not closed, returns only a segment of edges depending
            on the starting edge.
    
            Note: The algorithm assumes the wire is valid. Defects like loops,
            wrong edge orientations, or branching may cause incomplete traversal.
    
            Example:
                wire = ...  # some TopoDS_Wire
                explorer = BRepTools.WireExplorer(wire)
                while explorer.more:
                    edge = explorer.current
                    vertex = explorer.current_vertex
                    # process edge...
                    explorer.next()
    
            Or using iteration:
                for edge in BRepTools.WireExplorer(wire):
                    # process edge...
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty explorer. Use init() to set the wire.
        """
    @typing.overload
    def __init__(self, wire: mod3d.TopoDS.Wire) -> None:
        """
        Creates an explorer for the given wire.
        """
    @typing.overload
    def __init__(self, wire: mod3d.TopoDS.Wire, face: mod3d.TopoDS.Face) -> None:
        """
        Creates an explorer for the wire on a face.
        
        The face is used to select the edge connected to the previous
        in the parametric representation of the face.
        """
    def __iter__(self) -> WireExplorer:
        ...
    def __next__(self) -> mod3d.TopoDS.Edge:
        """
        Returns the next edge in the wire.
        """
    def clear(self) -> None:
        """
        Clears the content of the explorer.
        """
    @typing.overload
    def init(self, wire: mod3d.TopoDS.Wire) -> None:
        """
        Initializes exploration of the wire.
        """
    @typing.overload
    def init(self, wire: mod3d.TopoDS.Wire, face: mod3d.TopoDS.Face) -> None:
        """
        Initializes exploration of the wire on a face.
        """
    @typing.overload
    def init(self, wire: mod3d.TopoDS.Wire, face: mod3d.TopoDS.Face, u_min: typing.SupportsFloat, u_max: typing.SupportsFloat, v_min: typing.SupportsFloat, v_max: typing.SupportsFloat) -> None:
        """
        Initializes exploration with explicit UV bounds of the face.
        """
    def next(self) -> None:
        """
        Proceeds to the next edge in the wire.
        """
    @property
    def current(self) -> mod3d.TopoDS.Edge:
        """
        Returns the current edge being explored.
        """
    @property
    def current_vertex(self) -> mod3d.TopoDS.Vertex:
        """
        Returns the vertex connecting the current edge to the previous one.
        """
    @property
    def more(self) -> bool:
        """
        Returns True if there is a current edge to explore.
        """
    @property
    def orientation(self) -> TopAbs_Orientation:
        """
        Returns the orientation of the current edge.
        """
def is_closed(wire: mod3d.TopoDS.Shape) -> bool:
    """
    Checks if a wire is really closed.
    
    Returns True if the wire is closed and the first and last vertices are
    """
def is_realy_closed(edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> bool:
    """
    Checks if an edge is really closed on a face.
    
    Returns True if the edge is closed and the first and last vertices are
    """
def outer_wire(face: mod3d.TopoDS.Face) -> mod3d.TopoDS.Wire:
    """
    Returns the outer wire of a face.
    
    The outer wire is the one with the largest area among the wires of the face.
    If there are no wires, returns a null wire.
    """
@typing.overload
def write(shape: mod3d.TopoDS.Shape, filename: str) -> None:
    """
    Writes a shape to a file in BRep format.
    
    The file is created if it does not exist, or overwritten if it exists.
    Raises StdFail_InfiniteLoop if the shape contains an infinite loop of
    references (e.g., a face referencing itself).
    """
@typing.overload
def write(shape: mod3d.TopoDS.Shape, filename: str, with_triangles: bool = False, with_normals: bool = False) -> None:
    """
    Writes a shape to a file in BRep format with options for triangles and normals.
    
    Parameters:
      shape: The shape to write
      filename: The name of the file to write to
      with_triangles: If True, includes triangulation data in the file (default: False)
      with_normals: If True, includes normal vectors in the file (default: False)
    
    The file is created if it does not exist, or overwritten if it exists.
    Raises StdFail_InfiniteLoop if the shape contains an infinite loop of
    references (e.g., a face referencing itself).
    """
