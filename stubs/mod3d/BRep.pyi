"""
Boundary representation module
"""
from __future__ import annotations
import mod3d.Geom
import mod3d.GeomAbs
import mod3d.TopAbs
import mod3d.TopoDS
import mod3d.gp
import typing
__all__: list[str] = ['Tool']
class Tool:
    """
    Provides class methods to access the geometry of BRep shapes.
    
    This is a utility class with static methods for extracting geometric
    information from topological shapes (faces, edges, vertices).
    """
    @staticmethod
    def continuity(edge: mod3d.TopoDS.Edge, face1: mod3d.TopoDS.Face, face2: mod3d.TopoDS.Face) -> mod3d.GeomAbs.Shape:
        """
        Returns the continuity of the edge between two faces
        """
    @staticmethod
    def curve(edge: mod3d.TopoDS.Edge) -> tuple:
        """
        Returns tuple (curve, first, last) for the edge's 3D curve
        """
    @staticmethod
    def curve_on_surface(edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> tuple:
        """
        Returns tuple (2d_curve, first, last) for the edge on the face
        """
    @staticmethod
    def curve_with_location(edge: mod3d.TopoDS.Edge) -> tuple:
        """
        Returns tuple (curve, location, first, last) for the edge's 3D curve
        """
    @staticmethod
    def degenerated(edge: mod3d.TopoDS.Edge) -> bool:
        """
        Returns True if the edge is degenerated
        """
    @staticmethod
    def has_continuity(edge: mod3d.TopoDS.Edge, face1: mod3d.TopoDS.Face, face2: mod3d.TopoDS.Face) -> bool:
        """
        Returns True if the edge is on the surfaces of the two faces
        """
    @staticmethod
    def has_continuity_edge(edge: mod3d.TopoDS.Edge) -> bool:
        """
        Returns True if the edge has regularity on some two surfaces
        """
    @staticmethod
    def is_closed(shape: mod3d.TopoDS.Shape) -> bool:
        """
        Returns True if the shape is closed.
        
        - For Shell: True if it has no free boundaries (edges)
        - For Wire: True if it has no free ends (vertices)
        - For Edge: True if its vertices are the same
        - For other types: returns shape.Closed()
        """
    @staticmethod
    def is_closed_edge_on_face(edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> bool:
        """
        Returns True if edge has two PCurves on the face (closed surface)
        """
    @staticmethod
    def is_geometric(face: mod3d.TopoDS.Face) -> bool:
        """
        Returns True if the face has a surface
        """
    @staticmethod
    def is_geometric_edge(edge: mod3d.TopoDS.Edge) -> bool:
        """
        Returns True if edge is a 3D curve or curve on surface
        """
    @staticmethod
    def max_continuity(edge: mod3d.TopoDS.Edge) -> mod3d.GeomAbs.Shape:
        """
        Returns the max continuity of edge between surfaces or GeomAbs_C0
        """
    @staticmethod
    def max_tolerance(shape: mod3d.TopoDS.Shape, sub_shape: mod3d.TopAbs.ShapeEnum) -> float:
        """
        Returns the maximum tolerance of shape's subshapes.
        
        Parameters:
          shape: Shape to search tolerance in
          sub_shape: Type of subshape (Face, Edge, or Vertex)
        """
    @staticmethod
    def natural_restriction(face: mod3d.TopoDS.Face) -> bool:
        """
        Returns the NaturalRestriction flag of the face
        """
    @staticmethod
    def parameter(vertex: mod3d.TopoDS.Vertex, edge: mod3d.TopoDS.Edge) -> tuple:
        """
        Returns tuple (found, parameter) for the vertex on the edge
        """
    @staticmethod
    def parameter_on_edge(vertex: mod3d.TopoDS.Vertex, edge: mod3d.TopoDS.Edge) -> float:
        """
        Returns the parameter of the vertex on the edge.
        Raises exception if no parameter exists
        """
    @staticmethod
    def parameter_on_face(vertex: mod3d.TopoDS.Vertex, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> float:
        """
        Returns the parameter of the vertex on the edge's pcurve on the face
        """
    @staticmethod
    def parameters(vertex: mod3d.TopoDS.Vertex, face: mod3d.TopoDS.Face) -> mod3d.gp.Pnt2d:
        """
        Returns the UV parameters of the vertex on the face
        """
    @staticmethod
    def pnt(vertex: mod3d.TopoDS.Vertex) -> mod3d.gp.Pnt:
        """
        Returns the 3D point of the vertex
        """
    @staticmethod
    def range(edge: mod3d.TopoDS.Edge, first: typing.SupportsFloat, last: typing.SupportsFloat) -> None:
        """
        Gets the range of the 3D curve (outputs first and last parameters)
        """
    @staticmethod
    def range_on_face(edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> tuple:
        """
        Returns tuple (first, last) for the edge's range on the face
        """
    @staticmethod
    def same_parameter(edge: mod3d.TopoDS.Edge) -> bool:
        """
        Returns the SameParameter flag for the edge
        """
    @staticmethod
    def same_range(edge: mod3d.TopoDS.Edge) -> bool:
        """
        Returns the SameRange flag for the edge
        """
    @staticmethod
    def set_uv_points(edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face, p_first: mod3d.gp.Pnt2d, p_last: mod3d.gp.Pnt2d) -> None:
        """
        Sets the UV locations of the edge extremities on the face
        """
    @staticmethod
    def surface(face: mod3d.TopoDS.Face) -> mod3d.Geom.Surface:
        """
        Returns the geometric surface of the face.
        May be a copy if there is a location
        """
    @staticmethod
    def surface_with_location(face: mod3d.TopoDS.Face) -> tuple:
        """
        Returns tuple (surface, location) for the face
        """
    @staticmethod
    def tolerance(face: mod3d.TopoDS.Face) -> float:
        """
        Returns the tolerance of the face
        """
    @staticmethod
    def tolerance_edge(edge: mod3d.TopoDS.Edge) -> float:
        """
        Returns the tolerance of the edge
        """
    @staticmethod
    def tolerance_vertex(vertex: mod3d.TopoDS.Vertex) -> float:
        """
        Returns the tolerance of the vertex
        """
    @staticmethod
    def triangulation(face: mod3d.TopoDS.Face) -> tuple:
        """
        Returns tuple (triangulation, location) for the face
        """
    @staticmethod
    def uv_points(edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> tuple:
        """
        Returns tuple (uv_first, uv_last) for the edge extremities on the face
        """
