"""
Shape analysis module
"""
from __future__ import annotations
import mod3d.Geom
import mod3d.Geom2d
import mod3d.TopoDS
import typing
__all__: list[str] = ['Done1', 'Done2', 'Done3', 'Done4', 'Done5', 'Done6', 'Done7', 'Done8', 'Edge', 'Fail1', 'Fail2', 'Fail3', 'Fail4', 'Fail5', 'Fail6', 'Fail7', 'Fail8', 'OK', 'ShapeExtendStatus']
class Edge:
    """
    Tool for analyzing edges.
    
    Queries geometrical representations of the edge (3d curve, pcurve
    on the given face or surface) and topological sub-shapes (bounding
    vertices).
    Provides methods for analyzing geometry and topology consistency
    (3d and pcurve(s) consistency, their adjacency to the vertices).
    """
    def __init__(self) -> None:
        """
        Empty constructor; initializes Status to OK
        """
    def bound_uv(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> tuple:
        """
        Returns tuple (success, first_uv, last_uv) for the pcurve ends.
        Calls PCurve with orient=True
        """
    def bound_uv_on_surface(self, edge: mod3d.TopoDS.Edge, surface: mod3d.Geom.Surface, location: mod3d.TopoDS.TopLoc_Location) -> tuple:
        """
        Returns tuple (success, first_uv, last_uv) for the pcurve ends on surface
        """
    def check_curve3d_with_pcurve(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> bool:
        """
        Checks mutual orientation of 3d curve and pcurve on face.
        Analysis is based on curves' bounding points
        """
    def check_curve3d_with_pcurve_on_surface(self, edge: mod3d.TopoDS.Edge, surface: mod3d.Geom.Surface, location: mod3d.TopoDS.TopLoc_Location) -> bool:
        """
        Checks mutual orientation of 3d curve and pcurve on surface.
        Analysis is based on curves' bounding points
        """
    def check_overlapping(self, edge1: mod3d.TopoDS.Edge, edge2: mod3d.TopoDS.Edge, domain_dist: typing.SupportsFloat = 0.0) -> tuple:
        """
        Returns tuple (is_overlapped, tolerance).
        
        Checks if the first edge is overlapped with second edge.
        If distance between edges < tolerance, edges are overlapped.
        domain_dist: length of part of edges on which edges are overlapped
        """
    def check_pcurve_range(self, first: typing.SupportsFloat, last: typing.SupportsFloat, pcurve: mod3d.Geom2d.Curve2d) -> bool:
        """
        Checks possibility for pcurve to have range [first, last] (edge range)
        with respect to real first, last parameters of the pcurve
        """
    def check_same_parameter(self, edge: mod3d.TopoDS.Edge, nb_control: typing.SupportsInt = 23) -> tuple:
        """
        Returns tuple (is_valid, max_deviation).
        
        Checks the edge to be SameParameter.
        Calculates the maximal deviation between 3d curve and each pcurve
        of the edge on nb_control equidistant points (default 23, as in BRepCheck).
        Returns True if deviation <= edge tolerance, False otherwise
        """
    def check_same_parameter_on_face(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face, nb_control: typing.SupportsInt = 23) -> tuple:
        """
        Returns tuple (is_valid, max_deviation).
        
        Checks the edge to be SameParameter on the specific face.
        Calculates the maximal deviation between 3d curve and pcurve
        on nb_control equidistant points (default 23, as in BRepCheck).
        Returns True if deviation <= edge tolerance, False otherwise
        """
    def check_vertex_tolerance(self, edge: mod3d.TopoDS.Edge) -> tuple:
        """
        Returns tuple (needs_update, toler1, toler2).
        
        Checks if it's necessary to increase tolerances of edge vertices
        to comprise the ends of 3d curve and all pcurves stored in the edge.
        toler1: necessary tolerance for first vertex
        toler2: necessary tolerance for last vertex
        """
    def check_vertex_tolerance_on_face(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> tuple:
        """
        Returns tuple (needs_update, toler1, toler2).
        
        Checks if it's necessary to increase tolerances of edge vertices
        to comprise the ends of 3d curve and pcurve on the face.
        toler1: necessary tolerance for first vertex
        toler2: necessary tolerance for last vertex
        """
    def check_vertices_with_curve3d(self, edge: mod3d.TopoDS.Edge, preci: typing.SupportsFloat = -1, vtx: typing.SupportsInt = 0) -> bool:
        """
        Checks start/end vertex of edge for matching with 3d curve.
        
        Parameters:
          vtx: 0 = both vertices (default), 1 = start only, 2 = end only
          preci: precision to use (< 0: use vertex tolerances)
        """
    def check_vertices_with_pcurve(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face, preci: typing.SupportsFloat = -1, vtx: typing.SupportsInt = 0) -> bool:
        """
        Checks start/end vertex of edge for matching with pcurve on face.
        
        Parameters:
          vtx: 0 = both vertices (default), 1 = start only, 2 = end only
          preci: precision to use (< 0: use vertex tolerances)
        """
    def check_vertices_with_pcurve_on_surface(self, edge: mod3d.TopoDS.Edge, surface: mod3d.Geom.Surface, location: mod3d.TopoDS.TopLoc_Location, preci: typing.SupportsFloat = -1, vtx: typing.SupportsInt = 0) -> bool:
        """
        Checks start/end vertex of edge for matching with pcurve on surface.
        
        Parameters:
          vtx: 0 = both vertices (default), 1 = start only, 2 = end only
          preci: precision to use (< 0: use vertex tolerances)
        """
    def curve3d(self, edge: mod3d.TopoDS.Edge, orient: bool = True) -> tuple:
        """
        Returns tuple (success, curve, first, last) for the edge's 3d curve.
        
        If orient is True (default), takes orientation into account:
        if the edge is reversed, first and last are toggled
        """
    def first_vertex(self, edge: mod3d.TopoDS.Edge) -> mod3d.TopoDS.Vertex:
        """
        Returns start vertex of the edge (taking edge orientation into account)
        """
    def get_end_tangent2d(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face, at_end: bool, dparam: typing.SupportsFloat = 0.0) -> tuple:
        """
        Returns tuple (success, position, tangent) for the edge pcurve end.
        
        at_end: False for start, True for end of edge
        dparam: parameter increment for tangent computation
        
        Returns tangent at start (at_end=False) or end (at_end=True),
        regarding edge orientation. If edge is REVERSED, tangent is reversed.
        Returns True if pcurve exists and tangent is computed and non-null
        """
    def get_end_tangent2d_on_surface(self, edge: mod3d.TopoDS.Edge, surface: mod3d.Geom.Surface, location: mod3d.TopoDS.TopLoc_Location, at_end: bool, dparam: typing.SupportsFloat = 0.0) -> tuple:
        """
        Returns tuple (success, position, tangent) for the edge pcurve end on surface
        """
    def has_curve3d(self, edge: mod3d.TopoDS.Edge) -> bool:
        """
        Returns True if the edge has a 3d curve
        """
    def has_pcurve(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> bool:
        """
        Returns True if the edge has a pcurve on the face
        """
    def has_pcurve_on_surface(self, edge: mod3d.TopoDS.Edge, surface: mod3d.Geom.Surface, location: mod3d.TopoDS.TopLoc_Location) -> bool:
        """
        Returns True if the edge has a pcurve on the surface (with location)
        """
    def is_closed3d(self, edge: mod3d.TopoDS.Edge) -> bool:
        """
        Returns True if the edge has a 3d curve, this curve is closed,
        and the edge has the same vertex at start and end
        """
    def is_seam(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> bool:
        """
        Returns True if the edge is a seam on the face
        """
    def is_seam_on_surface(self, edge: mod3d.TopoDS.Edge, surface: mod3d.Geom.Surface, location: mod3d.TopoDS.TopLoc_Location) -> bool:
        """
        Returns True if the edge has two pcurves on the surface (seam)
        """
    def last_vertex(self, edge: mod3d.TopoDS.Edge) -> mod3d.TopoDS.Vertex:
        """
        Returns end vertex of the edge (taking edge orientation into account)
        """
    def pcurve(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face, orient: bool = True) -> tuple:
        """
        Returns tuple (success, pcurve, first, last) for the edge on the face.
        
        If orient is True (default), takes orientation into account:
        if the edge is reversed, first and last are toggled
        """
    def pcurve_on_surface(self, edge: mod3d.TopoDS.Edge, surface: mod3d.Geom.Surface, location: mod3d.TopoDS.TopLoc_Location, orient: bool = True) -> tuple:
        """
        Returns tuple (success, pcurve, first, last) for the edge on the surface.
        
        If orient is True (default), takes orientation into account:
        if the edge is reversed, first and last are toggled
        """
    def status(self, status: ShapeExtendStatus) -> bool:
        """
        Returns the status (as True/False) of the last check operation
        """
class ShapeExtendStatus:
    """
    Members:
    
      OK
    
      Done1
    
      Done2
    
      Done3
    
      Done4
    
      Done5
    
      Done6
    
      Done7
    
      Done8
    
      Fail1
    
      Fail2
    
      Fail3
    
      Fail4
    
      Fail5
    
      Fail6
    
      Fail7
    
      Fail8
    """
    Done1: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Done1: 1>
    Done2: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Done2: 2>
    Done3: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Done3: 3>
    Done4: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Done4: 4>
    Done5: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Done5: 5>
    Done6: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Done6: 6>
    Done7: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Done7: 7>
    Done8: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Done8: 8>
    Fail1: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Fail1: 10>
    Fail2: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Fail2: 11>
    Fail3: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Fail3: 12>
    Fail4: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Fail4: 13>
    Fail5: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Fail5: 14>
    Fail6: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Fail6: 15>
    Fail7: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Fail7: 16>
    Fail8: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.Fail8: 17>
    OK: typing.ClassVar[ShapeExtendStatus]  # value = <ShapeExtendStatus.OK: 0>
    __members__: typing.ClassVar[dict[str, ShapeExtendStatus]]  # value = {'OK': <ShapeExtendStatus.OK: 0>, 'Done1': <ShapeExtendStatus.Done1: 1>, 'Done2': <ShapeExtendStatus.Done2: 2>, 'Done3': <ShapeExtendStatus.Done3: 3>, 'Done4': <ShapeExtendStatus.Done4: 4>, 'Done5': <ShapeExtendStatus.Done5: 5>, 'Done6': <ShapeExtendStatus.Done6: 6>, 'Done7': <ShapeExtendStatus.Done7: 7>, 'Done8': <ShapeExtendStatus.Done8: 8>, 'Fail1': <ShapeExtendStatus.Fail1: 10>, 'Fail2': <ShapeExtendStatus.Fail2: 11>, 'Fail3': <ShapeExtendStatus.Fail3: 12>, 'Fail4': <ShapeExtendStatus.Fail4: 13>, 'Fail5': <ShapeExtendStatus.Fail5: 14>, 'Fail6': <ShapeExtendStatus.Fail6: 15>, 'Fail7': <ShapeExtendStatus.Fail7: 16>, 'Fail8': <ShapeExtendStatus.Fail8: 17>}
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
Done1: ShapeExtendStatus  # value = <ShapeExtendStatus.Done1: 1>
Done2: ShapeExtendStatus  # value = <ShapeExtendStatus.Done2: 2>
Done3: ShapeExtendStatus  # value = <ShapeExtendStatus.Done3: 3>
Done4: ShapeExtendStatus  # value = <ShapeExtendStatus.Done4: 4>
Done5: ShapeExtendStatus  # value = <ShapeExtendStatus.Done5: 5>
Done6: ShapeExtendStatus  # value = <ShapeExtendStatus.Done6: 6>
Done7: ShapeExtendStatus  # value = <ShapeExtendStatus.Done7: 7>
Done8: ShapeExtendStatus  # value = <ShapeExtendStatus.Done8: 8>
Fail1: ShapeExtendStatus  # value = <ShapeExtendStatus.Fail1: 10>
Fail2: ShapeExtendStatus  # value = <ShapeExtendStatus.Fail2: 11>
Fail3: ShapeExtendStatus  # value = <ShapeExtendStatus.Fail3: 12>
Fail4: ShapeExtendStatus  # value = <ShapeExtendStatus.Fail4: 13>
Fail5: ShapeExtendStatus  # value = <ShapeExtendStatus.Fail5: 14>
Fail6: ShapeExtendStatus  # value = <ShapeExtendStatus.Fail6: 15>
Fail7: ShapeExtendStatus  # value = <ShapeExtendStatus.Fail7: 16>
Fail8: ShapeExtendStatus  # value = <ShapeExtendStatus.Fail8: 17>
OK: ShapeExtendStatus  # value = <ShapeExtendStatus.OK: 0>
