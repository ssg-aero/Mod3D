"""
Topological exploration module
"""
from __future__ import annotations
import mod3d.TopAbs
import mod3d.TopoDS
import typing
__all__: list[str] = ['Explorer', 'common_vertex', 'first_vertex', 'get_edges', 'get_faces', 'get_solids', 'get_vertices', 'last_vertex', 'map_all_shapes', 'map_shapes', 'map_shapes_and_ancestors', 'map_shapes_and_unique_ancestors', 'vertices_of_edge', 'vertices_of_wire']
class Explorer:
    """
    Tool to visit and explore a topological data structure.
    
    An Explorer visits all shapes in a structure to find shapes of a
    requested type that are not contained in a type to avoid.
    
    Example to find all faces in shape S:
      explorer = TopExp.Explorer(S, TopAbs.ShapeEnum.FACE)
      while explorer.more():
          face = explorer.current()
          process_face(face)
          explorer.next()
    
    To find all vertices not in an edge:
      explorer = TopExp.Explorer(S, TopAbs.ShapeEnum.VERTEX, TopAbs.ShapeEnum.EDGE)
    
    Note: If the type to avoid is the same or less complex than the type
    to find, it has no effect on the exploration
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty explorer.
        
        Becomes useful after calling init() to set the shape and search parameters
        """
    @typing.overload
    def __init__(self, shape: mod3d.TopoDS.Shape, to_find: mod3d.TopAbs.ShapeEnum, to_avoid: mod3d.TopAbs.ShapeEnum = ...) -> None:
        """
        Creates an Explorer on a shape.
        
        Parameters:
          shape: The shape to explore
          to_find: The type of shapes to search for
                   (e.g., TopAbs.ShapeEnum.VERTEX, TopAbs.ShapeEnum.EDGE)
          to_avoid: The type of shapes to skip in the exploration (default: SHAPE)
                    If to_avoid is equal to or less complex than to_find,
                    or if to_avoid is SHAPE, it has no effect
        
        Example:
          # Find all edges in a shape
          explorer = TopExp.Explorer(shape, TopAbs.ShapeEnum.EDGE)
          # Find all vertices not in edges
          explorer = TopExp.Explorer(shape, TopAbs.ShapeEnum.VERTEX, TopAbs.ShapeEnum.EDGE)
        """
    def __iter__(self) -> Explorer:
        """
        Returns the iterator itself (enables Python 'for' loops)
        """
    def __next__(self) -> typing.Any:
        """
        Returns the next shape in iteration (enables Python 'for' loops).
        
        Example:
          for face in TopExp.Explorer(shape, TopAbs.ShapeEnum.FACE):
              process_face(face)
        """
    def clear(self) -> None:
        """
        Clears the content of the explorer.
        
        After calling this, more() will return False
        """
    def current(self) -> typing.Any:
        """
        Returns the current shape in the exploration.
        
        Returns:
          The current shape being visited, downcast to its proper type
        
        Raises:
          Standard_NoSuchObject if there are no more shapes to explore
        """
    def get_shapes(self, unique: bool = False) -> list:
        """
        Returns a list of all shapes found by the explorer.
        
        Parameters:
          unique: If True, only unique shapes are included (default: False)
        
        Returns:
          A list of shapes found during exploration
        """
    def init(self, shape: mod3d.TopoDS.Shape, to_find: mod3d.TopAbs.ShapeEnum, to_avoid: mod3d.TopAbs.ShapeEnum = ...) -> None:
        """
        Resets this explorer on a shape.
        
        Initializes the explorer to search the shape for shapes of type to_find
        that are not part of a shape to_avoid.
        
        Parameters:
          shape: The shape to explore
          to_find: The type of shapes to search for
          to_avoid: The type of shapes to skip (default: SHAPE = no restriction)
        
        If to_avoid is equal to TopAbs.ShapeEnum.SHAPE, or if it is the same as
        or less complex than to_find, it has no effect on the search
        """
    def more(self) -> bool:
        """
        Returns True if there are more shapes in the exploration.
        
        Use this in a loop condition to iterate through all matching shapes
        """
    def next(self) -> None:
        """
        Moves to the next shape in the exploration.
        
        Raises:
          Standard_NoMoreObject if there are no more shapes to explore
        """
    def reinit(self) -> None:
        """
        Reinitializes the exploration with the original arguments.
        
        Restarts the exploration from the beginning with the same shape
        and search parameters that were used in the constructor or last init() call
        """
    def value(self) -> typing.Any:
        """
        Returns the current shape in the exploration.
        
        This is an alias for current().
        
        Returns:
          The current shape being visited, downcast to its proper type
        
        Raises:
          Standard_NoSuchObject if there are no more shapes to explore
        """
    @property
    def depth(self) -> int:
        """
        Returns the current depth of the exploration.
        
        0 is the shape to explore itself, 1 is its direct sub-shapes, etc.
        """
    @property
    def explored_shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the shape being explored
        """
def common_vertex(edge1: mod3d.TopoDS.Edge, edge2: mod3d.TopoDS.Edge) -> typing.Any:
    """
    Returns the common vertex between two edges, if any.
    
    Parameters:
      edge1: First edge
      edge2: Second edge
    
    Returns:
      The common vertex, or None if the edges have no common vertex
    """
def first_vertex(edge: mod3d.TopoDS.Edge, cum_ori: bool = False) -> mod3d.TopoDS.Vertex:
    """
    Returns the first vertex (FORWARD orientation) of an edge.
    
    Parameters:
      edge: The edge to query
      cum_ori: If True, takes edge orientation into account (default: False)
    
    Returns:
      The first vertex, or a null shape if none exists
    """
def get_edges(shape: mod3d.TopoDS.Shape) -> list:
    """
    Returns a list of all edges in the given shape.
    
    Parameters:
      shape: The shape to explore
    
    Returns:
      A list of TopoDS_Edge objects contained in the shape
    """
def get_faces(shape: mod3d.TopoDS.Shape) -> list:
    """
    Returns a list of all faces in the given shape.
    
    Parameters:
      shape: The shape to explore
    
    Returns:
      A list of TopoDS_Face objects contained in the shape
    """
def get_solids(shape: mod3d.TopoDS.Shape) -> list:
    """
    Returns a list of all solids in the given shape.
    
    Parameters:
      shape: The shape to explore
    
    Returns:
      A list of TopoDS_Solid objects contained in the shape
    """
def get_vertices(shape: mod3d.TopoDS.Shape) -> list:
    """
    Returns a list of all vertices in the given shape.
    
    Parameters:
      shape: The shape to explore
    
    Returns:
      A list of TopoDS_Vertex objects contained in the shape
    """
def last_vertex(edge: mod3d.TopoDS.Edge, cum_ori: bool = False) -> mod3d.TopoDS.Vertex:
    """
    Returns the last vertex (REVERSED orientation) of an edge.
    
    Parameters:
      edge: The edge to query
      cum_ori: If True, takes edge orientation into account (default: False)
    
    Returns:
      The last vertex, or a null shape if none exists
    """
def map_all_shapes(shape: mod3d.TopoDS.Shape, cum_ori: bool = True, cum_loc: bool = True) -> ...:
    """
    Returns an indexed map of all sub-shapes.
    
    Parameters:
      shape: The shape to explore
      cum_ori: If True, composes sub-shapes with orientation of S (default: True)
      cum_loc: If True, multiplies sub-shapes by location of S (default: True)
    
    Returns:
      TopTools_IndexedMapOfShape containing all sub-shapes
    """
def map_shapes(shape: mod3d.TopoDS.Shape, shape_type: mod3d.TopAbs.ShapeEnum) -> ...:
    """
    Returns an indexed map of all sub-shapes of a given type.
    
    Parameters:
      shape: The shape to explore
      shape_type: The type of sub-shapes to find (e.g., TopAbs.ShapeEnum.FACE)
    
    Returns:
      TopTools_IndexedMapOfShape containing all sub-shapes of the requested type
    """
def map_shapes_and_ancestors(shape: mod3d.TopoDS.Shape, shape_type: mod3d.TopAbs.ShapeEnum, ancestor_type: mod3d.TopAbs.ShapeEnum) -> dict:
    """
    Returns a dict mapping sub-shapes to their ancestors.
    
    For each sub-shape of type shape_type, builds a list of ancestor shapes
    of type ancestor_type. For example, map all edges to their adjacent faces.
    
    Parameters:
      shape: The shape to explore
      shape_type: The type of sub-shapes to find (e.g., TopAbs.ShapeEnum.EDGE)
      ancestor_type: The type of ancestors to collect (e.g., TopAbs.ShapeEnum.FACE)
    
    Returns:
      dict[Shape, list[Shape]] mapping sub-shapes to ancestor lists
    """
def map_shapes_and_unique_ancestors(shape: mod3d.TopoDS.Shape, shape_type: mod3d.TopAbs.ShapeEnum, ancestor_type: mod3d.TopAbs.ShapeEnum, use_orientation: bool = False) -> dict:
    """
    Returns a dict mapping sub-shapes to their unique ancestors.
    
    Similar to map_shapes_and_ancestors but only includes unique ancestors.
    
    Parameters:
      shape: The shape to explore
      shape_type: The type of sub-shapes to find
      ancestor_type: The type of ancestors to collect
      use_orientation: If True, takes ancestor orientation into account (default: False)
    
    Returns:
      dict[Shape, list[Shape]] mapping sub-shapes to unique ancestor lists
    """
def vertices_of_edge(edge: mod3d.TopoDS.Edge, cum_ori: bool = False) -> tuple[mod3d.TopoDS.Vertex, mod3d.TopoDS.Vertex]:
    """
    Returns the first and last vertices of an edge.
    
    Parameters:
      edge: The edge to query
      cum_ori: If True, takes edge orientation into account (default: False)
    
    Returns:
      A tuple (first_vertex, last_vertex). May be null shapes.
    """
def vertices_of_wire(wire: mod3d.TopoDS.Wire) -> tuple[mod3d.TopoDS.Vertex, mod3d.TopoDS.Vertex]:
    """
    Returns the first and last vertices of an open wire.
    
    If the wire is closed, both vertices will be the same vertex.
    If the wire is non-manifold, both will be null shapes.
    
    Parameters:
      wire: The wire to query
    
    Returns:
      A tuple (first_vertex, last_vertex)
    """
