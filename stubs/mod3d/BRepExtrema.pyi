"""
BRep extrema computations
"""
from __future__ import annotations
import mod3d.TopoDS
import mod3d.gp
import typing
__all__: list[str] = ['DistShapeShape', 'ExtAlgo', 'ExtCC', 'ExtCF', 'ExtFF', 'ExtFlag', 'ExtPC', 'ExtPF', 'Grad', 'IsInFace', 'IsOnEdge', 'IsVertex', 'MAX', 'MIN', 'MINMAX', 'SupportType', 'Tree']
class DistShapeShape:
    """
    Compute minimum distance between two shapes (Compound, CompSolid, Solid, Shell, Face, Wire, Edge, Vertex)
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Create empty distance computation tool
        """
    @typing.overload
    def __init__(self, shape1: mod3d.TopoDS.Shape, shape2: mod3d.TopoDS.Shape, flag: ExtFlag = ..., algo: ExtAlgo = ...) -> None:
        """
        Create tool and compute minimum distance using default deflection (Precision::Confusion())
        
        Parameters:
          shape1: First shape for distance computation
          shape2: Second shape for distance computation
          flag: Extrema flag (obsolete, not used)
          algo: Extrema algorithm (obsolete, not used)
        """
    @typing.overload
    def __init__(self, shape1: mod3d.TopoDS.Shape, shape2: mod3d.TopoDS.Shape, deflection: typing.SupportsFloat, flag: ExtFlag = ..., algo: ExtAlgo = ...) -> None:
        """
        Create tool and compute minimum distance with specified deflection
        
        Parameters:
          shape1: First shape for distance computation
          shape2: Second shape for distance computation
          deflection: Precision of distance computation
          flag: Extrema flag (obsolete, not used)
          algo: Extrema algorithm (obsolete, not used)
        """
    def dump(self, stream: ..., std: ...) -> None:
        """
        Print information on the current state to output stream
        """
    def load_s1(self, shape1: mod3d.TopoDS.Shape) -> None:
        """
        Load first shape into extrema
        """
    def load_s2(self, shape2: mod3d.TopoDS.Shape) -> None:
        """
        Load second shape into extrema
        """
    def par_on_edge_s1(self, n: typing.SupportsInt) -> float:
        """
        Return the parameter t if the Nth solution is on an edge of the first shape
        """
    def par_on_edge_s2(self, n: typing.SupportsInt) -> float:
        """
        Return the parameter t if the Nth solution is on an edge of the second shape
        """
    def par_on_face_s1(self, n: typing.SupportsInt) -> tuple:
        """
        Return the parameters (u, v) if the Nth solution is on a face of the first shape
        """
    def par_on_face_s2(self, n: typing.SupportsInt) -> tuple:
        """
        Return the parameters (u, v) if the Nth solution is on a face of the second shape
        """
    def perform(self) -> bool:
        """
        Compute minimum distance (value and pair of points)
        
        Returns:
          True if computation succeeded
        """
    def point_on_shape1(self, n: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Return the point corresponding to the Nth solution on the first shape
        """
    def point_on_shape2(self, n: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Return the point corresponding to the Nth solution on the second shape
        """
    def points_and_distances(self) -> list[tuple]:
        """
        Returns a list of tuples (point_on_shape1, point_on_shape2, distance) for each solution
        """
    def set_algo(self, algo: ExtAlgo) -> None:
        """
        Set extrema algorithm (obsolete, not used)
        """
    def set_deflection(self, deflection: typing.SupportsFloat) -> None:
        """
        Set deflection for computation of minimum distance
        """
    def set_flag(self, flag: ExtFlag) -> None:
        """
        Set extrema flag (obsolete, not used)
        """
    def support_on_shape1(self, n: typing.SupportsInt) -> mod3d.TopoDS.Shape:
        """
        Return the support (Vertex, Edge or Face) where the Nth solution on the first shape is situated
        """
    def support_on_shape2(self, n: typing.SupportsInt) -> mod3d.TopoDS.Shape:
        """
        Return the support (Vertex, Edge or Face) where the Nth solution on the second shape is situated
        """
    def support_type_shape1(self, n: typing.SupportsInt) -> SupportType:
        """
        Return the type of support where the Nth solution on the first shape is situated
        
        Returns:
          IsVertex: Solution is a vertex
          IsOnEdge: Solution is on an edge
          IsInFace: Solution is inside a face
        """
    def support_type_shape2(self, n: typing.SupportsInt) -> SupportType:
        """
        Return the type of support where the Nth solution on the second shape is situated
        
        Returns:
          IsVertex: Solution is a vertex
          IsOnEdge: Solution is on an edge
          IsInFace: Solution is inside a face
        """
    @property
    def inner_solution(self) -> bool:
        """
        True if one shape is a solid and the other is completely or partially inside
        """
    @property
    def is_done(self) -> bool:
        """
        True if the minimum distance is found
        """
    @property
    def is_multi_thread(self) -> bool:
        """
        Enable/disable parallel computation (default: False)
        """
    @is_multi_thread.setter
    def is_multi_thread(self, arg1: bool) -> None:
        ...
    @property
    def nb_solution(self) -> int:
        """
        Number of solutions satisfying the minimum distance
        """
    @property
    def value(self) -> float:
        """
        Return the value of the minimum distance
        """
class ExtAlgo:
    """
    Members:
    
      Grad
    
      Tree
    """
    Grad: typing.ClassVar[ExtAlgo]  # value = <ExtAlgo.Grad: 0>
    Tree: typing.ClassVar[ExtAlgo]  # value = <ExtAlgo.Tree: 1>
    __members__: typing.ClassVar[dict[str, ExtAlgo]]  # value = {'Grad': <ExtAlgo.Grad: 0>, 'Tree': <ExtAlgo.Tree: 1>}
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
class ExtCC:
    """
    Computes extrema distances between two edges (curves).
    
    This class finds all local extremum distances between two curves.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty ExtCC object
        """
    @typing.overload
    def __init__(self, edge1: mod3d.TopoDS.Edge, edge2: mod3d.TopoDS.Edge) -> None:
        """
        Creates and computes all extremum distances between two edges
        """
    def initialize(self, edge2: mod3d.TopoDS.Edge) -> None:
        """
        Initializes the algorithm with the second edge
        """
    def pamameters_and_points(self) -> list[tuple]:
        ...
    def parameter_on_e1(self, n: typing.SupportsInt) -> float:
        """
        Returns the parameter on the first edge of the N-th extremum distance
        """
    def parameter_on_e2(self, n: typing.SupportsInt) -> float:
        """
        Returns the parameter on the second edge of the N-th extremum distance
        """
    def perform(self, edge1: mod3d.TopoDS.Edge) -> None:
        """
        Computes extrema for the given first edge
        """
    def point_on_e1(self, n: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Returns the point on the first edge of the N-th extremum distance
        """
    def point_on_e2(self, n: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Returns the point on the second edge of the N-th extremum distance
        """
    def square_distance(self, n: typing.SupportsInt) -> float:
        """
        Returns the value of the N-th extremum square distance
        """
    def square_distances(self) -> list[float]:
        """
        Returns all extremum square distances as a list
        """
    def trimmed_square_distances(self) -> tuple:
        """
        Returns ((dist11, dist12, dist21, dist22), (p11, p12, p21, p22)).
        Square distances between trimmed curve endpoints:
          dist11: E1.First to E2.First
          dist12: E1.First to E2.Last
          dist21: E1.Last to E2.First
          dist22: E1.Last to E2.Last
        """
    @property
    def is_done(self) -> bool:
        """
        True if the distances are found
        """
    @property
    def is_parallel(self) -> bool:
        """
        Returns True if the two edges are parallel
        """
    @property
    def nb_ext(self) -> int:
        """
        Returns the number of extremum distances
        """
class ExtCF:
    """
    Computes extrema distances between an edge (curve) and a face (surface).
    
    This class finds all local extremum distances between a curve and a surface.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty ExtCF object
        """
    @typing.overload
    def __init__(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> None:
        """
        Creates and computes all extremum distances between edge and face
        """
    def initialize(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> None:
        """
        Initializes the algorithm with an edge and face
        """
    def parameter_on_edge(self, n: typing.SupportsInt) -> float:
        """
        Returns the parameter on the edge of the N-th extremum distance
        """
    def parameter_on_face(self, n: typing.SupportsInt) -> tuple:
        """
        Returns the (u, v) parameters on the face of the N-th extremum distance
        """
    def perform(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> None:
        """
        Computes extrema for the given edge and face
        """
    def point_on_edge(self, n: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Returns the point on the edge of the N-th extremum distance
        """
    def point_on_face(self, n: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Returns the point on the face of the N-th extremum distance
        """
    def points_and_parameters(self) -> list[tuple]:
        ...
    def square_distance(self, n: typing.SupportsInt) -> float:
        """
        Returns the value of the N-th extremum square distance
        """
    @property
    def is_done(self) -> bool:
        """
        True if the distances are found
        """
    @property
    def is_parallel(self) -> bool:
        """
        Returns True if the curve lies on a parallel surface
        """
    @property
    def nb_ext(self) -> int:
        """
        Returns the number of extremum distances
        """
class ExtFF:
    """
    Computes extrema distances between two faces (surfaces).
    
    This class finds all local extremum distances between two surfaces.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty ExtFF object
        """
    @typing.overload
    def __init__(self, face1: mod3d.TopoDS.Face, face2: mod3d.TopoDS.Face) -> None:
        """
        Creates and computes all extremum distances between two faces
        """
    def initialize(self, face2: mod3d.TopoDS.Face) -> None:
        """
        Initializes the algorithm with the second face
        """
    def parameter_on_face1(self, n: typing.SupportsInt) -> tuple:
        """
        Returns the (u, v) parameters on face1 of the N-th extremum distance
        """
    def parameter_on_face2(self, n: typing.SupportsInt) -> tuple:
        """
        Returns the (u, v) parameters on face2 of the N-th extremum distance
        """
    def perform(self, face1: mod3d.TopoDS.Face, face2: mod3d.TopoDS.Face) -> None:
        """
        Computes extrema for the given faces
        """
    def point_on_face1(self, n: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Returns the point on face1 of the N-th extremum distance
        """
    def point_on_face2(self, n: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Returns the point on face2 of the N-th extremum distance
        """
    def points_and_parameters(self) -> list[tuple]:
        ...
    def square_distance(self, n: typing.SupportsInt) -> float:
        """
        Returns the value of the N-th extremum square distance
        """
    @property
    def is_done(self) -> bool:
        """
        True if the distances are found
        """
    @property
    def is_parallel(self) -> bool:
        """
        Returns True if the two surfaces are parallel
        """
    @property
    def nb_ext(self) -> int:
        """
        Returns the number of extremum distances
        """
class ExtFlag:
    """
    Members:
    
      MIN
    
      MAX
    
      MINMAX
    """
    MAX: typing.ClassVar[ExtFlag]  # value = <ExtFlag.MAX: 1>
    MIN: typing.ClassVar[ExtFlag]  # value = <ExtFlag.MIN: 0>
    MINMAX: typing.ClassVar[ExtFlag]  # value = <ExtFlag.MINMAX: 2>
    __members__: typing.ClassVar[dict[str, ExtFlag]]  # value = {'MIN': <ExtFlag.MIN: 0>, 'MAX': <ExtFlag.MAX: 1>, 'MINMAX': <ExtFlag.MINMAX: 2>}
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
class ExtPC:
    """
    Computes extrema distances between a vertex (point) and an edge (curve).
    
    This class finds all local extremum distances between a point and a curve,
    including both minima and maxima.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty ExtPC object
        """
    @typing.overload
    def __init__(self, vertex: mod3d.TopoDS.Vertex, edge: mod3d.TopoDS.Edge) -> None:
        """
        Creates and computes all extremum distances between vertex and edge
        """
    def initialize(self, edge: mod3d.TopoDS.Edge) -> None:
        """
        Initializes the algorithm with an edge
        """
    def is_min(self, n: typing.SupportsInt) -> bool:
        """
        Returns True if the N-th extremum distance is a minimum
        """
    def parameter(self, n: typing.SupportsInt) -> float:
        """
        Returns the parameter on the edge of the N-th extremum distance
        """
    def perform(self, vertex: mod3d.TopoDS.Vertex) -> None:
        """
        Computes extrema for the given vertex
        """
    def point(self, n: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Returns the point of the N-th extremum distance
        """
    def points(self) -> list[mod3d.gp.Pnt]:
        """
        Returns all extremum points as a list
        """
    def square_distance(self, n: typing.SupportsInt) -> float:
        """
        Returns the value of the N-th extremum square distance
        """
    def trimmed_square_distances(self) -> tuple:
        """
        Returns (dist1, dist2, pnt1, pnt2) for trimmed curve endpoints.
        dist1: square distance to first parameter point
        dist2: square distance to last parameter point
        """
    @property
    def is_done(self) -> bool:
        """
        True if the distances are found
        """
    @property
    def nb_ext(self) -> int:
        """
        Returns the number of extremum distances
        """
class ExtPF:
    """
    Computes extrema distances between a vertex (point) and a face (surface).
    
    This class finds all local extremum distances between a point and a surface.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty ExtPF object
        """
    @typing.overload
    def __init__(self, vertex: mod3d.TopoDS.Vertex, face: mod3d.TopoDS.Face, flag: ExtFlag = ..., algo: ExtAlgo = ...) -> None:
        """
        Creates and computes all extremum distances between vertex and face
        """
    def initialize(self, face: mod3d.TopoDS.Face, flag: ExtFlag = ..., algo: ExtAlgo = ...) -> None:
        """
        Initializes the algorithm with a face
        """
    def parameter(self, n: typing.SupportsInt) -> tuple:
        """
        Returns the (u, v) parameters on the face of the N-th extremum distance
        """
    def parameters(self) -> list[tuple]:
        """
        Returns all extremum parameters as a list of (u, v) tuples
        """
    def perform(self, vertex: mod3d.TopoDS.Vertex, face: mod3d.TopoDS.Face) -> None:
        """
        Computes extrema for the given vertex and face
        """
    def point(self, n: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Returns the point of the N-th extremum distance
        """
    def points(self) -> list[mod3d.gp.Pnt]:
        """
        Returns all extremum points as a list
        """
    def set_algo(self, algo: ExtAlgo) -> None:
        """
        Sets the extrema algorithm (Grad or Tree)
        """
    def set_flag(self, flag: ExtFlag) -> None:
        """
        Sets the extrema flag (MIN, MAX, or MINMAX)
        """
    def square_distance(self, n: typing.SupportsInt) -> float:
        """
        Returns the value of the N-th extremum square distance
        """
    @property
    def is_done(self) -> bool:
        """
        True if the distances are found
        """
    @property
    def nb_ext(self) -> int:
        """
        Returns the number of extremum distances
        """
class SupportType:
    """
    Type of support where a solution is located
    
    Members:
    
      IsVertex : Solution is a vertex
    
      IsOnEdge : Solution is on an edge
    
      IsInFace : Solution is inside a face
    """
    IsInFace: typing.ClassVar[SupportType]  # value = <SupportType.IsInFace: 2>
    IsOnEdge: typing.ClassVar[SupportType]  # value = <SupportType.IsOnEdge: 1>
    IsVertex: typing.ClassVar[SupportType]  # value = <SupportType.IsVertex: 0>
    __members__: typing.ClassVar[dict[str, SupportType]]  # value = {'IsVertex': <SupportType.IsVertex: 0>, 'IsOnEdge': <SupportType.IsOnEdge: 1>, 'IsInFace': <SupportType.IsInFace: 2>}
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
Grad: ExtAlgo  # value = <ExtAlgo.Grad: 0>
IsInFace: SupportType  # value = <SupportType.IsInFace: 2>
IsOnEdge: SupportType  # value = <SupportType.IsOnEdge: 1>
IsVertex: SupportType  # value = <SupportType.IsVertex: 0>
MAX: ExtFlag  # value = <ExtFlag.MAX: 1>
MIN: ExtFlag  # value = <ExtFlag.MIN: 0>
MINMAX: ExtFlag  # value = <ExtFlag.MINMAX: 2>
Tree: ExtAlgo  # value = <ExtAlgo.Tree: 1>
