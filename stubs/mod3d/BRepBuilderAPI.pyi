"""
BRep builder API
"""
from __future__ import annotations
import mod3d.BRepPrim
import mod3d.Geom
import mod3d.GeomFill
import mod3d.TopoDS
import mod3d.gp
import typing
__all__: list[str] = ['BRepFill_Pipe', 'BRepSweep_Prism', 'BRepSweep_Revol', 'Command', 'Contact', 'ContactOnBorder', 'CurveProjectionFailed', 'DifferentPointsOnClosedCurve', 'DifferentsPointAndParameter', 'DisconnectedShell', 'DisconnectedWire', 'EdgeDone', 'EdgeError', 'EmptyShell', 'EmptyWire', 'FaceDone', 'FaceError', 'ImpossibleContact', 'LineThroughIdenticPoints', 'MakeBox', 'MakeCone', 'MakeCylinder', 'MakeEdge', 'MakeFace', 'MakeOneAxis', 'MakePipe', 'MakePipeShell', 'MakePolygon', 'MakePrism', 'MakeRevol', 'MakeRevolution', 'MakeShape', 'MakeShell', 'MakeSolid', 'MakeSphere', 'MakeSweep', 'MakeVertex', 'MakeWire', 'NoContact', 'NoFace', 'NonManifoldWire', 'NotPlanar', 'ParameterOutOfRange', 'ParametersOutOfRange', 'PipeDone', 'PipeError', 'PipeNotDone', 'PlaneNotIntersectGuide', 'PointProjectionFailed', 'PointWithInfiniteParameter', 'RightCorner', 'RoundCorner', 'Sewing', 'ShellDone', 'ShellError', 'ShellParametersOutOfRange', 'Transformed', 'TransitionMode', 'TypeOfContact', 'WireDone', 'WireError', 'make_edge', 'make_face', 'make_wire']
class BRepFill_Pipe:
    """
    Internal pipe filling algorithm used by MakePipe
    """
class BRepSweep_Prism:
    """
    Internal prism sweep algorithm used by MakePrism
    """
class BRepSweep_Revol:
    """
    Internal revolution sweep algorithm used by MakeRevol
    """
class Command:
    def check(self) -> None:
        ...
    def is_done(self) -> bool:
        ...
class EdgeError:
    """
    Members:
    
      EdgeDone
    
      PointProjectionFailed
    
      ParameterOutOfRange
    
      DifferentPointsOnClosedCurve
    
      PointWithInfiniteParameter
    
      DifferentsPointAndParameter
    
      LineThroughIdenticPoints
    """
    DifferentPointsOnClosedCurve: typing.ClassVar[EdgeError]  # value = <EdgeError.DifferentPointsOnClosedCurve: 3>
    DifferentsPointAndParameter: typing.ClassVar[EdgeError]  # value = <EdgeError.DifferentsPointAndParameter: 5>
    EdgeDone: typing.ClassVar[EdgeError]  # value = <EdgeError.EdgeDone: 0>
    LineThroughIdenticPoints: typing.ClassVar[EdgeError]  # value = <EdgeError.LineThroughIdenticPoints: 6>
    ParameterOutOfRange: typing.ClassVar[EdgeError]  # value = <EdgeError.ParameterOutOfRange: 2>
    PointProjectionFailed: typing.ClassVar[EdgeError]  # value = <EdgeError.PointProjectionFailed: 1>
    PointWithInfiniteParameter: typing.ClassVar[EdgeError]  # value = <EdgeError.PointWithInfiniteParameter: 4>
    __members__: typing.ClassVar[dict[str, EdgeError]]  # value = {'EdgeDone': <EdgeError.EdgeDone: 0>, 'PointProjectionFailed': <EdgeError.PointProjectionFailed: 1>, 'ParameterOutOfRange': <EdgeError.ParameterOutOfRange: 2>, 'DifferentPointsOnClosedCurve': <EdgeError.DifferentPointsOnClosedCurve: 3>, 'PointWithInfiniteParameter': <EdgeError.PointWithInfiniteParameter: 4>, 'DifferentsPointAndParameter': <EdgeError.DifferentsPointAndParameter: 5>, 'LineThroughIdenticPoints': <EdgeError.LineThroughIdenticPoints: 6>}
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
class FaceError:
    """
    Members:
    
      FaceDone
    
      NoFace
    
      NotPlanar
    
      CurveProjectionFailed
    
      ParametersOutOfRange
    """
    CurveProjectionFailed: typing.ClassVar[FaceError]  # value = <FaceError.CurveProjectionFailed: 3>
    FaceDone: typing.ClassVar[FaceError]  # value = <FaceError.FaceDone: 0>
    NoFace: typing.ClassVar[FaceError]  # value = <FaceError.NoFace: 1>
    NotPlanar: typing.ClassVar[FaceError]  # value = <FaceError.NotPlanar: 2>
    ParametersOutOfRange: typing.ClassVar[FaceError]  # value = <FaceError.ParametersOutOfRange: 4>
    __members__: typing.ClassVar[dict[str, FaceError]]  # value = {'FaceDone': <FaceError.FaceDone: 0>, 'NoFace': <FaceError.NoFace: 1>, 'NotPlanar': <FaceError.NotPlanar: 2>, 'CurveProjectionFailed': <FaceError.CurveProjectionFailed: 3>, 'ParametersOutOfRange': <FaceError.ParametersOutOfRange: 4>}
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
class MakeBox(MakeShape):
    """
    Describes functions to build parallelepiped boxes.
    
    A MakeBox object provides a framework for:
    - defining the construction of a box,
    - implementing the construction algorithm, and
    - consulting the result.
    
    Constructs a box such that its sides are parallel to the axes of
    - the global coordinate system, or
    - the local coordinate system Axis, and
    - with a corner at (0, 0, 0) and of size (dx, dy, dz), or
    - with a corner at point P and of size (dx, dy, dz), or
    - with corners at points P1 and P2.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Default constructor
        """
    @typing.overload
    def __init__(self, dx: typing.SupportsFloat, dy: typing.SupportsFloat, dz: typing.SupportsFloat) -> None:
        """
        Make a box with a corner at (0,0,0) and the other at (dx, dy, dz)
        
        Parameters:
          dx: Size along X axis
          dy: Size along Y axis
          dz: Size along Z axis
        
        Raises:
          Standard_DomainError if dx, dy, or dz <= Precision::Confusion()
        """
    @typing.overload
    def __init__(self, p: mod3d.gp.Pnt, dx: typing.SupportsFloat, dy: typing.SupportsFloat, dz: typing.SupportsFloat) -> None:
        """
        Make a box with a corner at P and size dx, dy, dz
        
        Parameters:
          p: Corner point of the box
          dx: Size along X axis
          dy: Size along Y axis
          dz: Size along Z axis
        
        Raises:
          Standard_DomainError if dx, dy, or dz <= Precision::Confusion()
        """
    @typing.overload
    def __init__(self, p1: mod3d.gp.Pnt, p2: mod3d.gp.Pnt) -> None:
        """
        Make a box with corners P1 and P2
        
        Parameters:
          p1: First corner point
          p2: Second corner point (opposite corner)
        
        Raises:
          Standard_DomainError if any component of the vector P1-P2 <= Precision::Confusion()
        """
    @typing.overload
    def __init__(self, axes: mod3d.gp.Ax2, dx: typing.SupportsFloat, dy: typing.SupportsFloat, dz: typing.SupportsFloat) -> None:
        """
        Make a box with Ax2 (the left corner and the axis) and size dx, dy, dz
        
        Parameters:
          axes: Local coordinate system (origin is the corner)
          dx: Size along local X axis
          dy: Size along local Y axis
          dz: Size along local Z axis
        
        Raises:
          Standard_DomainError if dx, dy, or dz <= Precision::Confusion()
        """
    def build(self) -> None:
        """
        Builds the box and stores the result
        """
    @typing.overload
    def init(self, dx: typing.SupportsFloat, dy: typing.SupportsFloat, dz: typing.SupportsFloat) -> None:
        """
        Initialize a box with a corner at (0,0,0) and the other at (dx, dy, dz)
        """
    @typing.overload
    def init(self, pnt: mod3d.gp.Pnt, dx: typing.SupportsFloat, dy: typing.SupportsFloat, dz: typing.SupportsFloat) -> None:
        """
        Initialize a box with a corner at pnt and size dx, dy, dz
        """
    @typing.overload
    def init(self, pnt1: mod3d.gp.Pnt, pnt2: mod3d.gp.Pnt) -> None:
        """
        Initialize a box with corners pnt1 and pnt2
        """
    @typing.overload
    def init(self, axes: mod3d.gp.Ax2, dx: typing.SupportsFloat, dy: typing.SupportsFloat, dz: typing.SupportsFloat) -> None:
        """
        Initialize a box with Ax2 (the left corner and the axes) and size dx, dy, dz
        """
    def shell(self) -> mod3d.TopoDS.Shell:
        """
        Returns the constructed box as a shell
        """
    def solid(self) -> mod3d.TopoDS.Solid:
        """
        Returns the constructed box as a solid
        """
    @property
    def back_face(self) -> mod3d.TopoDS.Face:
        """
        Returns XMin face (back face)
        """
    @property
    def bottom_face(self) -> mod3d.TopoDS.Face:
        """
        Returns ZMin face (bottom face)
        """
    @property
    def front_face(self) -> mod3d.TopoDS.Face:
        """
        Returns XMax face (front face)
        """
    @property
    def left_face(self) -> mod3d.TopoDS.Face:
        """
        Returns YMin face (left face)
        """
    @property
    def right_face(self) -> mod3d.TopoDS.Face:
        """
        Returns YMax face (right face)
        """
    @property
    def top_face(self) -> mod3d.TopoDS.Face:
        """
        Returns ZMax face (top face)
        """
    @property
    def wedge(self) -> BRepPrim_Wedge:
        """
        Returns the internal wedge algorithm
        """
class MakeCone(MakeOneAxis):
    @typing.overload
    def __init__(self, r1: typing.SupportsFloat, r2: typing.SupportsFloat, h: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, r1: typing.SupportsFloat, r2: typing.SupportsFloat, h: typing.SupportsFloat, angle: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, Axes: mod3d.gp.Ax2, r1: typing.SupportsFloat, r2: typing.SupportsFloat, h: typing.SupportsFloat) -> None:
        ...
class MakeCylinder(MakeOneAxis):
    @typing.overload
    def __init__(self, r: typing.SupportsFloat, h: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, r: typing.SupportsFloat, h: typing.SupportsFloat, angle: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, Axes: mod3d.gp.Ax2, r: typing.SupportsFloat, h: typing.SupportsFloat) -> None:
        ...
class MakeEdge(MakeShape):
    """
    Provides methods to build edges.
    An edge can be created from:
    - Two vertices or two points
    - A curve (gp_Lin, gp_Circ, gp_Elips, gp_Hypr, gp_Parab, or Geom_Curve)
    - A curve with parameter bounds or vertex/point bounds
    - A 2D curve on a surface
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty edge
        """
    @typing.overload
    def __init__(self, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex) -> None:
        """
        Creates a straight edge between two vertices
        """
    @typing.overload
    def __init__(self, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt) -> None:
        """
        Creates a straight edge between two points
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Lin) -> None:
        """
        Creates an infinite edge on the line L
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Lin, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> None:
        """
        Creates an edge on the line L between parameters p1 and p2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Lin, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt) -> None:
        """
        Creates an edge on the line L between points P1 and P2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Lin, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex) -> None:
        """
        Creates an edge on the line L between vertices V1 and V2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Circ) -> None:
        """
        Creates a closed edge on the circle L
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Circ, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> None:
        """
        Creates an edge on the circle L between parameters p1 and p2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Circ, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt) -> None:
        """
        Creates an edge on the circle L between points P1 and P2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Circ, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex) -> None:
        """
        Creates an edge on the circle L between vertices V1 and V2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Elips) -> None:
        """
        Creates a closed edge on the ellipse L
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Elips, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> None:
        """
        Creates an edge on the ellipse L between parameters p1 and p2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Elips, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt) -> None:
        """
        Creates an edge on the ellipse L between points P1 and P2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Elips, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex) -> None:
        """
        Creates an edge on the ellipse L between vertices V1 and V2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Hypr) -> None:
        """
        Creates an infinite edge on the hyperbola L
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Hypr, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> None:
        """
        Creates an edge on the hyperbola L between parameters p1 and p2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Hypr, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt) -> None:
        """
        Creates an edge on the hyperbola L between points P1 and P2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Hypr, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex) -> None:
        """
        Creates an edge on the hyperbola L between vertices V1 and V2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Parab) -> None:
        """
        Creates an infinite edge on the parabola L
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Parab, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> None:
        """
        Creates an edge on the parabola L between parameters p1 and p2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Parab, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt) -> None:
        """
        Creates an edge on the parabola L between points P1 and P2
        """
    @typing.overload
    def __init__(self, L: mod3d.gp.Parab, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex) -> None:
        """
        Creates an edge on the parabola L between vertices V1 and V2
        """
    @typing.overload
    def __init__(self, L: mod3d.Geom.Curve) -> None:
        """
        Creates an edge on the curve L with natural bounds
        """
    @typing.overload
    def __init__(self, L: mod3d.Geom.Curve, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> None:
        """
        Creates an edge on the curve L between parameters p1 and p2
        """
    @typing.overload
    def __init__(self, L: mod3d.Geom.Curve, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt) -> None:
        """
        Creates an edge on the curve L between points P1 and P2
        """
    @typing.overload
    def __init__(self, L: mod3d.Geom.Curve, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex) -> None:
        """
        Creates an edge on the curve L between vertices V1 and V2
        """
    @typing.overload
    def __init__(self, L: mod3d.Geom.Curve, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> None:
        """
        Creates an edge on the curve L between points P1, P2 with parameters p1, p2
        """
    @typing.overload
    def __init__(self, L: mod3d.Geom.Curve, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> None:
        """
        Creates an edge on the curve L between vertices V1, V2 with parameters p1, p2
        """
    def edge(self) -> mod3d.TopoDS.Edge:
        """
        Returns the constructed edge
        """
    def error(self) -> EdgeError:
        """
        Returns the construction status
        """
    @typing.overload
    def init(self, C: mod3d.Geom.Curve) -> None:
        """
        Reinitializes with curve C
        """
    @typing.overload
    def init(self, C: mod3d.Geom.Curve, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> None:
        """
        Reinitializes with curve C between parameters p1 and p2
        """
    @typing.overload
    def init(self, C: mod3d.Geom.Curve, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt) -> None:
        """
        Reinitializes with curve C between points P1 and P2
        """
    @typing.overload
    def init(self, C: mod3d.Geom.Curve, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex) -> None:
        """
        Reinitializes with curve C between vertices V1 and V2
        """
    @typing.overload
    def init(self, C: mod3d.Geom.Curve, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> None:
        """
        Reinitializes with curve C between points P1, P2 with parameters p1, p2
        """
    @typing.overload
    def init(self, C: mod3d.Geom.Curve, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> None:
        """
        Reinitializes with curve C between vertices V1, V2 with parameters p1, p2
        """
    def vertex1(self) -> mod3d.TopoDS.Vertex:
        """
        Returns the first vertex of the edge (may be null)
        """
    def vertex2(self) -> mod3d.TopoDS.Vertex:
        """
        Returns the second vertex of the edge (may be null)
        """
class MakeFace(MakeShape):
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, F: mod3d.TopoDS.Face) -> None:
        ...
    @typing.overload
    def __init__(self, P: mod3d.gp.Pln) -> None:
        ...
    @typing.overload
    def __init__(self, C: mod3d.gp.Cylinder) -> None:
        ...
    @typing.overload
    def __init__(self, C: mod3d.gp.Cone) -> None:
        ...
    @typing.overload
    def __init__(self, S: mod3d.gp.Sphere) -> None:
        ...
    @typing.overload
    def __init__(self, T: mod3d.gp.Torus) -> None:
        ...
    @typing.overload
    def __init__(self, S: mod3d.Geom.Surface, TolDegen: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, P: mod3d.gp.Pln, UMin: typing.SupportsFloat, UMax: typing.SupportsFloat, VMin: typing.SupportsFloat, VMax: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, C: mod3d.gp.Cylinder, UMin: typing.SupportsFloat, UMax: typing.SupportsFloat, VMin: typing.SupportsFloat, VMax: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, C: mod3d.gp.Cone, UMin: typing.SupportsFloat, UMax: typing.SupportsFloat, VMin: typing.SupportsFloat, VMax: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, S: mod3d.gp.Sphere, UMin: typing.SupportsFloat, UMax: typing.SupportsFloat, VMin: typing.SupportsFloat, VMax: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, T: mod3d.gp.Torus, UMin: typing.SupportsFloat, UMax: typing.SupportsFloat, VMin: typing.SupportsFloat, VMax: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, S: mod3d.Geom.Surface, UMin: typing.SupportsFloat, UMax: typing.SupportsFloat, VMin: typing.SupportsFloat, VMax: typing.SupportsFloat, TolDegen: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, W: mod3d.TopoDS.Wire, OnlyPlane: bool = False) -> None:
        ...
    @typing.overload
    def __init__(self, P: mod3d.gp.Pln, W: mod3d.TopoDS.Wire, Inside: bool = True) -> None:
        ...
    @typing.overload
    def __init__(self, C: mod3d.gp.Cylinder, W: mod3d.TopoDS.Wire, Inside: bool = True) -> None:
        ...
    @typing.overload
    def __init__(self, C: mod3d.gp.Cone, W: mod3d.TopoDS.Wire, Inside: bool = True) -> None:
        ...
    @typing.overload
    def __init__(self, S: mod3d.gp.Sphere, W: mod3d.TopoDS.Wire, Inside: bool = True) -> None:
        ...
    @typing.overload
    def __init__(self, T: mod3d.gp.Torus, W: mod3d.TopoDS.Wire, Inside: bool = True) -> None:
        ...
    @typing.overload
    def __init__(self, S: mod3d.Geom.Surface, W: mod3d.TopoDS.Wire, Inside: bool = True) -> None:
        ...
    @typing.overload
    def __init__(self, F: mod3d.TopoDS.Face, W: mod3d.TopoDS.Wire) -> None:
        """
        From face + wire (add hole)
        """
    def add(self, W: mod3d.TopoDS.Wire) -> None:
        ...
    def error(self) -> FaceError:
        ...
    def face(self) -> mod3d.TopoDS.Face:
        ...
    @typing.overload
    def init(self, F: mod3d.TopoDS.Face) -> None:
        ...
    @typing.overload
    def init(self, S: mod3d.Geom.Surface, Bound: bool, TolDegen: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def init(self, S: mod3d.Geom.Surface, UMin: typing.SupportsFloat, UMax: typing.SupportsFloat, VMin: typing.SupportsFloat, VMax: typing.SupportsFloat, TolDegen: typing.SupportsFloat) -> None:
        ...
class MakeOneAxis(MakeShape):
    def face(self) -> mod3d.TopoDS.Face:
        ...
    def shell(self) -> mod3d.TopoDS.Shell:
        ...
    def solid(self) -> mod3d.TopoDS.Solid:
        ...
class MakePipe(MakeSweep):
    @typing.overload
    def __init__(self, spine: mod3d.TopoDS.Wire, profile: mod3d.TopoDS.Shape) -> None:
        """
        Constructs a pipe by sweeping the shape Profile along the wire Spine
        """
    @typing.overload
    def __init__(self, spine: mod3d.TopoDS.Wire, profile: mod3d.TopoDS.Shape, mode: mod3d.GeomFill.Trihedron, force_approx_c1: bool = False) -> None:
        """
        Same as previous but with setting of mode of sweep and C1 approximation flag
        """
    def error_on_surface(self) -> float:
        """
        Returns the maximum error on the generated surface
        """
    def generated(self, spine_shape: mod3d.TopoDS.Shape, profile_shape: mod3d.TopoDS.Shape) -> mod3d.TopoDS.Shape:
        """
        Returns the shape generated from the given spine and profile shapes
        """
    def pipe(self) -> BRepFill_Pipe:
        """
        Returns the internal BRepFill_Pipe object
        """
class MakePipeShell(MakeSweep):
    def __init__(self, spine: mod3d.TopoDS.Wire) -> None:
        """
        Constructs the shell-generating framework defined by the wire Spine
        """
    @typing.overload
    def add(self, profile: mod3d.TopoDS.Shape, with_contact: bool = False, with_correction: bool = False) -> None:
        """
        Adds a section Profile. Point on spine computed automatically
        """
    @typing.overload
    def add(self, profile: mod3d.TopoDS.Shape, location: mod3d.TopoDS.Vertex, with_contact: bool = False, with_correction: bool = False) -> None:
        """
        Adds a section Profile at the given Location on spine
        """
    def delete(self, profile: mod3d.TopoDS.Shape) -> None:
        """
        Removes the section Profile
        """
    def error_on_surface(self) -> float:
        """
        Returns maximum error on generated surface
        """
    def get_status(self) -> PipeError:
        """
        Get status when Simulate or Build failed
        """
    def is_ready(self) -> bool:
        """
        Returns true if ready to build (has profile definition)
        """
    def make_solid(self) -> bool:
        """
        Transforms the sweeping Shell into Solid
        """
    def profiles(self) -> list:
        """
        Returns the list of original profiles
        """
    def set_discrete_mode(self) -> None:
        """
        Sets a Discrete trihedron to perform the sweeping
        """
    def set_force_approx_c1(self, force_approx_c1: bool) -> None:
        """
        Set flag to attempt C1-continuous surface approximation
        """
    @typing.overload
    def set_law(self, profile: mod3d.TopoDS.Shape, law: ..., with_contact: bool = False, with_correction: bool = False) -> None:
        """
        Sets evolution law with homotetic law defined by function
        """
    @typing.overload
    def set_law(self, profile: mod3d.TopoDS.Shape, law: ..., location: mod3d.TopoDS.Vertex, with_contact: bool = False, with_correction: bool = False) -> None:
        """
        Sets evolution law with homotetic law defined by function at given location
        """
    def set_max_degree(self, new_max_degree: typing.SupportsInt) -> None:
        """
        Define maximum V degree of resulting surface
        """
    def set_max_segments(self, new_max_segments: typing.SupportsInt) -> None:
        """
        Define maximum number of spans in V-direction
        """
    @typing.overload
    def set_mode(self, is_frenet: bool = False) -> None:
        """
        Sets a Frenet or CorrectedFrenet trihedron. If IsFrenet is false, a corrected Frenet trihedron is used
        """
    @typing.overload
    def set_mode(self, axe: mod3d.gp.Ax2) -> None:
        """
        Sets a fixed trihedron - all sections will be parallel
        """
    @typing.overload
    def set_mode(self, binormal: mod3d.gp.Dir) -> None:
        """
        Sets a fixed BiNormal direction - angular relations will be constant
        """
    @typing.overload
    def set_mode(self, spine_support: mod3d.TopoDS.Shape) -> bool:
        """
        Sets support to the spine to define the BiNormal
        """
    @typing.overload
    def set_mode(self, auxiliary_spine: mod3d.TopoDS.Wire, curvilinear_equivalence: bool, keep_contact: TypeOfContact = ...) -> None:
        """
        Sets an auxiliary spine to define the Normal
        """
    def set_tolerance(self, tol_3d: typing.SupportsFloat = 0.0001, bound_tol: typing.SupportsFloat = 0.0001, tol_angular: typing.SupportsFloat = 0.01) -> None:
        """
        Sets tolerance values
        """
    def set_transition_mode(self, mode: TransitionMode = ...) -> None:
        """
        Sets transition mode for discontinuities on spine
        """
    def simulate(self, number_of_section: typing.SupportsInt) -> list:
        """
        Simulates the resulting shape by calculating cross-sections
        """
    def spine(self) -> mod3d.TopoDS.Wire:
        """
        Returns the spine
        """
class MakePolygon(MakeShape):
    """
    Constructs polygonal wires from points or vertices.
    
    A polygonal wire consists of a sequence of connected rectilinear edges.
    When a point or vertex is added, if it is identical to the previous point,
    no edge is built. Use the added() method to test if the vertex was added.
    
    Construction approaches:
    1. Complete polygon by defining all points/vertices (up to 4)
    2. Empty polygon and add points/vertices in sequence (unlimited)
    
    Warning: If the sequence p1-p2-p1 appears among the points, you will
    create a polygonal wire with two consecutive coincident edges
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Initializes an empty polygonal wire.
        
        Points or vertices are added using the add() method.
        As soon as the wire contains vertices, it can be consulted using wire()
        """
    @typing.overload
    def __init__(self, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt) -> None:
        """
        Constructs a polygonal wire from 2 points.
        Vertices are automatically created on the given points
        """
    @typing.overload
    def __init__(self, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt, P3: mod3d.gp.Pnt, close: bool = False) -> None:
        """
        Constructs a polygonal wire from 3 points.
        
        Parameters:
          close: If True, creates a closed polygon
        """
    @typing.overload
    def __init__(self, P1: mod3d.gp.Pnt, P2: mod3d.gp.Pnt, P3: mod3d.gp.Pnt, P4: mod3d.gp.Pnt, close: bool = False) -> None:
        """
        Constructs a polygonal wire from 4 points.
        
        Parameters:
          close: If True, creates a closed polygon
        """
    @typing.overload
    def __init__(self, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex) -> None:
        """
        Constructs a polygonal wire from 2 vertices
        """
    @typing.overload
    def __init__(self, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex, V3: mod3d.TopoDS.Vertex, close: bool = False) -> None:
        """
        Constructs a polygonal wire from 3 vertices.
        
        Parameters:
          close: If True, creates a closed polygon
        """
    @typing.overload
    def __init__(self, V1: mod3d.TopoDS.Vertex, V2: mod3d.TopoDS.Vertex, V3: mod3d.TopoDS.Vertex, V4: mod3d.TopoDS.Vertex, close: bool = False) -> None:
        """
        Constructs a polygonal wire from 4 vertices.
        
        Parameters:
          close: If True, creates a closed polygon
        """
    @typing.overload
    def add(self, P: mod3d.gp.Pnt) -> None:
        """
        Adds the point P at the end of the polygonal wire.
        
        A vertex is automatically created on the point P.
        
        Warning:
        - When P is coincident to the previous vertex, no edge is built
        - P is not checked against other vertices except the last one
        - You can add vertices to a closed polygon
        - This may create consecutive coincident edges or non-manifold wires
        """
    @typing.overload
    def add(self, V: mod3d.TopoDS.Vertex) -> None:
        """
        Adds the vertex V at the end of the polygonal wire.
        
        Warning:
        - When V is coincident to the previous vertex, no edge is built
        - V is not checked against other vertices except the last one
        - You can add vertices to a closed polygon
        - This may create consecutive coincident edges or non-manifold wires
        """
    def added(self) -> bool:
        """
        Returns True if the last vertex added was not coincident with the previous one.
        
        Use this method after add() to check if an edge was actually created
        """
    def close(self) -> None:
        """
        Closes the polygonal wire under construction.
        
        This is equivalent to adding the first vertex to the polygon
        """
    def is_done(self) -> bool:
        """
        Returns True if this algorithm contains a valid polygonal wire.
        
        A valid wire has at least one edge (i.e., at least two vertices)
        """
    def wire(self) -> mod3d.TopoDS.Wire:
        """
        Returns the constructed polygonal wire.
        
        Or the already built part of the polygonal wire under construction.
        
        Raises StdFail_NotDone if fewer than two vertices have been added
        """
    @property
    def edge(self) -> mod3d.TopoDS.Edge:
        """
        Returns the edge built between the last two points or vertices added.
        
        Warning: If there is only one vertex in the polygon, the result is a null edge
        """
    @property
    def first_vertex(self) -> mod3d.TopoDS.Vertex:
        """
        Returns the first vertex of the polygonal wire.
        If the polygon is closed, first and last vertices are identical
        """
    @property
    def last_vertex(self) -> mod3d.TopoDS.Vertex:
        """
        Returns the last vertex of the polygonal wire.
        If the polygon is closed, first and last vertices are identical
        """
class MakePrism(MakeSweep):
    @typing.overload
    def __init__(self, shape: mod3d.TopoDS.Shape, vector: mod3d.gp.Vec, copy: bool = False, canonize: bool = True) -> None:
        """
        Builds the prism of base S and vector V. If Copy is true, S is copied
        """
    @typing.overload
    def __init__(self, shape: mod3d.TopoDS.Shape, direction: mod3d.gp.Dir, infinite: bool = True, copy: bool = False, canonize: bool = True) -> None:
        """
        Builds a semi-infinite or infinite prism. If Inf is true the prism is infinite
        """
    @typing.overload
    def first_shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the TopoDS Shape of the bottom of the prism
        """
    @typing.overload
    def first_shape(self, the_shape: mod3d.TopoDS.Shape) -> mod3d.TopoDS.Shape:
        """
        Returns the bottom shape generated with theShape
        """
    def is_deleted(self, shape: mod3d.TopoDS.Shape) -> bool:
        """
        Returns true if the shape S has been deleted
        """
    @typing.overload
    def last_shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the TopoDS Shape of the top of the prism
        """
    @typing.overload
    def last_shape(self, the_shape: mod3d.TopoDS.Shape) -> mod3d.TopoDS.Shape:
        """
        Returns the top shape generated with theShape
        """
    def prism(self) -> BRepSweep_Prism:
        """
        Returns the internal sweeping algorithm
        """
class MakeRevol(MakeSweep):
    @typing.overload
    def __init__(self, shape: mod3d.TopoDS.Shape, axis: mod3d.gp.Ax1, angle: typing.SupportsFloat, copy: bool = False) -> None:
        """
        Builds the Revol of base S, axis A and angle D. If Copy is true, S is copied
        """
    @typing.overload
    def __init__(self, shape: mod3d.TopoDS.Shape, axis: mod3d.gp.Ax1, copy: bool = False) -> None:
        """
        Builds the Revol of base S, axis A and angle 2*Pi. If Copy is true, S is copied
        """
    def degenerated(self) -> list:
        """
        Returns the list of degenerated edges
        """
    @typing.overload
    def first_shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the first shape of the revol (coinciding with the generating shape)
        """
    @typing.overload
    def first_shape(self, the_shape: mod3d.TopoDS.Shape) -> mod3d.TopoDS.Shape:
        """
        Returns the first shape generated with theShape
        """
    def has_degenerated(self) -> bool:
        """
        Check if there are degenerated edges in the result
        """
    def is_deleted(self, shape: mod3d.TopoDS.Shape) -> bool:
        """
        Returns true if the shape S has been deleted
        """
    @typing.overload
    def last_shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the TopoDS Shape of the end of the revol
        """
    @typing.overload
    def last_shape(self, the_shape: mod3d.TopoDS.Shape) -> mod3d.TopoDS.Shape:
        """
        Returns the last shape generated with theShape
        """
    def revol(self) -> BRepSweep_Revol:
        """
        Returns the internal sweeping algorithm
        """
class MakeRevolution(MakeOneAxis):
    @typing.overload
    def __init__(self, meridian: mod3d.Geom.Curve) -> None:
        ...
    @typing.overload
    def __init__(self, meridian: mod3d.Geom.Curve, angle: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, meridian: mod3d.Geom.Curve, v_min: typing.SupportsFloat, v_max: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, meridian: mod3d.Geom.Curve, v_min: typing.SupportsFloat, v_max: typing.SupportsFloat, angle: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, axes: mod3d.gp.Ax2, meridian: mod3d.Geom.Curve) -> None:
        ...
    @typing.overload
    def __init__(self, axes: mod3d.gp.Ax2, meridian: mod3d.Geom.Curve, angle: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, axes: mod3d.gp.Ax2, meridian: mod3d.Geom.Curve, v_min: typing.SupportsFloat, v_max: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, axes: mod3d.gp.Ax2, meridian: mod3d.Geom.Curve, v_min: typing.SupportsFloat, v_max: typing.SupportsFloat, angle: typing.SupportsFloat) -> None:
        ...
    def revolution(self) -> mod3d.BRepPrim.Revolution:
        ...
class MakeShape(Command):
    def build(self) -> None:
        ...
    def deleted(self, arg0: mod3d.TopoDS.Shape) -> bool:
        ...
    def generated(self, arg0: mod3d.TopoDS.Shape) -> list:
        ...
    def modified(self, arg0: mod3d.TopoDS.Shape) -> list:
        ...
    def shape(self) -> mod3d.TopoDS.Shape:
        ...
class MakeShell(MakeShape):
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, S: mod3d.Geom.Surface, Segment: bool = False) -> None:
        ...
    @typing.overload
    def __init__(self, S: mod3d.Geom.Surface, UMin: typing.SupportsFloat, UMax: typing.SupportsFloat, VMin: typing.SupportsFloat, VMax: typing.SupportsFloat, Segment: bool = False) -> None:
        ...
    def error(self) -> ShellError:
        ...
    def init(self, S: mod3d.Geom.Surface, UMin: typing.SupportsFloat, UMax: typing.SupportsFloat, VMin: typing.SupportsFloat, VMax: typing.SupportsFloat, Segment: bool = False) -> None:
        ...
    def shell(self) -> mod3d.TopoDS.Shell:
        ...
class MakeSolid(MakeShape):
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, S: mod3d.TopoDS.CompSolid) -> None:
        ...
    @typing.overload
    def __init__(self, S: mod3d.TopoDS.Shell) -> None:
        ...
    @typing.overload
    def __init__(self, S1: mod3d.TopoDS.Shell, S2: mod3d.TopoDS.Shell) -> None:
        ...
    @typing.overload
    def __init__(self, S1: mod3d.TopoDS.Shell, S2: mod3d.TopoDS.Shell, S3: mod3d.TopoDS.Shell) -> None:
        ...
    @typing.overload
    def __init__(self, So: mod3d.TopoDS.Solid) -> None:
        ...
    @typing.overload
    def __init__(self, So: mod3d.TopoDS.Solid, S: mod3d.TopoDS.Shell) -> None:
        ...
    def add(self, S: mod3d.TopoDS.Shell) -> None:
        ...
    def solid(self) -> mod3d.TopoDS.Solid:
        ...
class MakeSphere(MakeOneAxis):
    """
    Describes functions to build spheres or portions of spheres.
    
    A MakeSphere object provides a framework for:
    - defining the construction of a sphere,
    - implementing the construction algorithm, and
    - consulting the result.
    
    The resulting shape is composed of:
    - a lateral spherical face,
    - two planar faces parallel to the plane z=0 if truncated in v direction,
    - and in case of a portion, two planar faces to close the shape (in planes u=0 and u=angle)
    """
    @typing.overload
    def __init__(self, r: typing.SupportsFloat) -> None:
        """
        Make a sphere of radius R
        
        Parameters:
          r: Sphere radius
        """
    @typing.overload
    def __init__(self, r: typing.SupportsFloat, angle: typing.SupportsFloat) -> None:
        """
        Make a sphere (spherical wedge)
        
        Parameters:
          r: Sphere radius
          angle: Angle between the radii lying within the bounding semidisks
        """
    @typing.overload
    def __init__(self, r: typing.SupportsFloat, angle1: typing.SupportsFloat, angle2: typing.SupportsFloat) -> None:
        """
        Make a sphere (spherical segment)
        
        Parameters:
          r: Sphere radius
          angle1: First angle defining a spherical segment
          angle2: Second angle defining a spherical segment
        """
    @typing.overload
    def __init__(self, r: typing.SupportsFloat, angle1: typing.SupportsFloat, angle2: typing.SupportsFloat, angle3: typing.SupportsFloat) -> None:
        """
        Make a sphere (spherical segment with wedge)
        
        Parameters:
          r: Sphere radius
          angle1: First angle defining a spherical segment
          angle2: Second angle defining a spherical segment
          angle3: Angle between the radii lying within the bounding semidisks
        """
    @typing.overload
    def __init__(self, center: mod3d.gp.Pnt, r: typing.SupportsFloat) -> None:
        """
        Make a sphere at given center point
        
        Parameters:
          center: Sphere center coordinates
          r: Sphere radius
        """
    @typing.overload
    def __init__(self, center: mod3d.gp.Pnt, r: typing.SupportsFloat, angle: typing.SupportsFloat) -> None:
        """
        Make a sphere (spherical wedge) at given center
        
        Parameters:
          center: Sphere center coordinates
          r: Sphere radius
          angle: Angle between the radii lying within the bounding semidisks
        """
    @typing.overload
    def __init__(self, center: mod3d.gp.Pnt, r: typing.SupportsFloat, angle1: typing.SupportsFloat, angle2: typing.SupportsFloat) -> None:
        """
        Make a sphere (spherical segment) at given center
        
        Parameters:
          center: Sphere center coordinates
          r: Sphere radius
          angle1: First angle defining a spherical segment
          angle2: Second angle defining a spherical segment
        """
    @typing.overload
    def __init__(self, center: mod3d.gp.Pnt, r: typing.SupportsFloat, angle1: typing.SupportsFloat, angle2: typing.SupportsFloat, angle3: typing.SupportsFloat) -> None:
        """
        Make a sphere (spherical segment with wedge) at given center
        
        Parameters:
          center: Sphere center coordinates
          r: Sphere radius
          angle1: First angle defining a spherical segment
          angle2: Second angle defining a spherical segment
          angle3: Angle between the radii lying within the bounding semidisks
        """
    @typing.overload
    def __init__(self, axes: mod3d.gp.Ax2, r: typing.SupportsFloat) -> None:
        """
        Make a sphere with custom coordinate system
        
        Parameters:
          axes: Coordinate system for the construction of the sphere
          r: Sphere radius
        """
    @typing.overload
    def __init__(self, axes: mod3d.gp.Ax2, r: typing.SupportsFloat, angle: typing.SupportsFloat) -> None:
        """
        Make a sphere (spherical wedge) with custom coordinate system
        
        Parameters:
          axes: Coordinate system for the construction of the sphere
          r: Sphere radius
          angle: Angle between the radii lying within the bounding semidisks
        """
    @typing.overload
    def __init__(self, axes: mod3d.gp.Ax2, r: typing.SupportsFloat, angle1: typing.SupportsFloat, angle2: typing.SupportsFloat) -> None:
        """
        Make a sphere (spherical segment) with custom coordinate system
        
        Parameters:
          axes: Coordinate system for the construction of the sphere
          r: Sphere radius
          angle1: First angle defining a spherical segment
          angle2: Second angle defining a spherical segment
        """
    @typing.overload
    def __init__(self, axes: mod3d.gp.Ax2, r: typing.SupportsFloat, angle1: typing.SupportsFloat, angle2: typing.SupportsFloat, angle3: typing.SupportsFloat) -> None:
        """
        Make a sphere (spherical segment with wedge) with custom coordinate system
        
        Parameters:
          axes: Coordinate system for the construction of the sphere
          r: Sphere radius
          angle1: First angle defining a spherical segment
          angle2: Second angle defining a spherical segment
          angle3: Angle between the radii lying within the bounding semidisks
        """
    @property
    def sphere(self) -> BRepPrim_Sphere:
        """
        Returns the internal BRepPrim_Sphere algorithm
        """
class MakeSweep(MakeShape):
    def first_shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the TopoDS Shape of the bottom of the sweep
        """
    def last_shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the TopoDS Shape of the top of the sweep
        """
class MakeVertex(MakeShape):
    @typing.overload
    def __init__(self, Pnt: mod3d.gp.Pnt) -> None:
        ...
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat, z: typing.SupportsFloat) -> None:
        ...
    def vertex(self) -> mod3d.TopoDS.Vertex:
        ...
class MakeWire(MakeShape):
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, E: mod3d.TopoDS.Edge) -> None:
        ...
    @typing.overload
    def __init__(self, E1: mod3d.TopoDS.Edge, E2: mod3d.TopoDS.Edge) -> None:
        ...
    @typing.overload
    def __init__(self, E1: mod3d.TopoDS.Edge, E2: mod3d.TopoDS.Edge, E3: mod3d.TopoDS.Edge) -> None:
        ...
    @typing.overload
    def __init__(self, E1: mod3d.TopoDS.Edge, E2: mod3d.TopoDS.Edge, E3: mod3d.TopoDS.Edge, E4: mod3d.TopoDS.Edge) -> None:
        ...
    @typing.overload
    def __init__(self, W: mod3d.TopoDS.Wire) -> None:
        ...
    @typing.overload
    def __init__(self, W: mod3d.TopoDS.Wire, E: mod3d.TopoDS.Edge) -> None:
        ...
    @typing.overload
    def add(self, E: mod3d.TopoDS.Edge) -> None:
        ...
    @typing.overload
    def add(self, W: mod3d.TopoDS.Wire) -> None:
        ...
    @typing.overload
    def add(self, L: ...) -> None:
        ...
    def edge(self) -> mod3d.TopoDS.Edge:
        ...
    def error(self) -> WireError:
        ...
    def vertex(self) -> mod3d.TopoDS.Vertex:
        ...
    def wire(self) -> mod3d.TopoDS.Wire:
        ...
class PipeError:
    """
    Members:
    
      PipeDone
    
      PipeNotDone
    
      PlaneNotIntersectGuide
    
      ImpossibleContact
    """
    ImpossibleContact: typing.ClassVar[PipeError]  # value = <PipeError.ImpossibleContact: 3>
    PipeDone: typing.ClassVar[PipeError]  # value = <PipeError.PipeDone: 0>
    PipeNotDone: typing.ClassVar[PipeError]  # value = <PipeError.PipeNotDone: 1>
    PlaneNotIntersectGuide: typing.ClassVar[PipeError]  # value = <PipeError.PlaneNotIntersectGuide: 2>
    __members__: typing.ClassVar[dict[str, PipeError]]  # value = {'PipeDone': <PipeError.PipeDone: 0>, 'PipeNotDone': <PipeError.PipeNotDone: 1>, 'PlaneNotIntersectGuide': <PipeError.PlaneNotIntersectGuide: 2>, 'ImpossibleContact': <PipeError.ImpossibleContact: 3>}
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
class Sewing:
    """
    Provides methods to identify possible contiguous boundaries and
    assemble contiguous shapes into one shape.
    
    Only manifold shapes will be found. Sewing will not be done in case
    of multiple edges.
    
    Typical usage:
      sewing = BRepBuilderAPI.Sewing(tolerance=1e-6)
      sewing.add(shape1)
      sewing.add(shape2)
      sewing.perform()
      result = sewing.sewed_shape()
    """
    def __init__(self, tolerance: typing.SupportsFloat = 1e-06, option1: bool = True, option2: bool = True, option3: bool = True, option4: bool = False) -> None:
        """
        Creates a sewing object.
        
        Parameters:
          tolerance: Tolerance of connexity
          option1: Option for sewing (if False only control)
          option2: Option for analysis of degenerated shapes
          option3: Option for cutting of free edges
          option4: Option for non manifold processing
        """
    def add(self, shape: mod3d.TopoDS.Shape) -> None:
        """
        Defines the shapes to be sewed or controlled
        """
    def contigous_edge(self, index: typing.SupportsInt) -> mod3d.TopoDS.Edge:
        """
        Gives each contiguous edge
        """
    def contigous_edge_couple(self, index: typing.SupportsInt) -> ...:
        """
        Gives the sections (edge) belonging to a contiguous edge
        """
    def degenerated_shape(self, index: typing.SupportsInt) -> mod3d.TopoDS.Shape:
        """
        Gives each degenerated shape
        """
    def deleted_face(self, index: typing.SupportsInt) -> mod3d.TopoDS.Face:
        """
        Gives each deleted face
        """
    def dump(self) -> None:
        """
        Print the information
        """
    def free_edge(self, index: typing.SupportsInt) -> mod3d.TopoDS.Edge:
        """
        Gives each free edge
        """
    def init(self, tolerance: typing.SupportsFloat = 1e-06, option1: bool = True, option2: bool = True, option3: bool = True, option4: bool = False) -> None:
        """
        Initialize the parameters if necessary
        """
    def is_degenerated(self, shape: mod3d.TopoDS.Shape) -> bool:
        """
        Indicates if an input shape is degenerated
        """
    def is_modified(self, shape: mod3d.TopoDS.Shape) -> bool:
        """
        Indicates if an input shape has been modified
        """
    def is_modified_subshape(self, shape: mod3d.TopoDS.Shape) -> bool:
        """
        Indicates if an input subshape has been modified
        """
    def is_section_bound(self, section: mod3d.TopoDS.Edge) -> bool:
        """
        Indicates if a section is bound (before use section_to_boundary)
        """
    def load(self, shape: mod3d.TopoDS.Shape) -> None:
        """
        Loads the context shape
        """
    def modified(self, shape: mod3d.TopoDS.Shape) -> mod3d.TopoDS.Shape:
        """
        Gives a modified shape
        """
    def modified_subshape(self, shape: mod3d.TopoDS.Shape) -> mod3d.TopoDS.Shape:
        """
        Gives a modified subshape
        """
    def multiple_edge(self, index: typing.SupportsInt) -> mod3d.TopoDS.Edge:
        """
        Gives each multiple edge
        """
    def perform(self) -> None:
        """
        Computing - performs the sewing operation
        """
    def section_to_boundary(self, section: mod3d.TopoDS.Edge) -> mod3d.TopoDS.Edge:
        """
        Gives the original edge (free boundary) which becomes the section.
        
        Remember that sections constitute common edges.
        This information is important for control because with
        original edge we can find the surface to which the section is attached
        """
    def sewed_shape(self) -> mod3d.TopoDS.Shape:
        """
        Gives the sewed shape.
        
        Returns a null shape if nothing constructed.
        May be a face, a shell, a solid or a compound
        """
    def which_face(self, edge: mod3d.TopoDS.Edge, index: typing.SupportsInt = 1) -> mod3d.TopoDS.Face:
        """
        Gives a modified shape
        """
    @property
    def context(self) -> BRepTools_ReShape:
        """
        Gets/sets the context
        """
    @context.setter
    def context(self, arg1: BRepTools_ReShape) -> None:
        ...
    @property
    def face_mode(self) -> bool:
        """
        Gets/sets mode for sewing faces (default: True)
        """
    @face_mode.setter
    def face_mode(self, arg1: bool) -> None:
        ...
    @property
    def floating_edges_mode(self) -> bool:
        """
        Gets/sets mode for sewing floating edges (default: False)
        """
    @floating_edges_mode.setter
    def floating_edges_mode(self, arg1: bool) -> None:
        ...
    @property
    def local_tolerances_mode(self) -> bool:
        """
        Gets/sets mode for accounting of local tolerances.
        
        During merging: WorkTolerance = tolerance + tolEdge1 + tolEdge2
        """
    @local_tolerances_mode.setter
    def local_tolerances_mode(self, arg1: bool) -> None:
        ...
    @property
    def max_tolerance(self) -> float:
        """
        Gets/sets max tolerance
        """
    @max_tolerance.setter
    def max_tolerance(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def min_tolerance(self) -> float:
        """
        Gets/sets min tolerance
        """
    @min_tolerance.setter
    def min_tolerance(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def nb_contigous_edges(self) -> int:
        """
        Gives the number of contiguous edges (edge shared by two faces)
        """
    @property
    def nb_degenerated_shapes(self) -> int:
        """
        Gives the number of degenerated shapes
        """
    @property
    def nb_deleted_faces(self) -> int:
        """
        Gives the number of deleted faces (faces smaller than tolerance)
        """
    @property
    def nb_free_edges(self) -> int:
        """
        Gives the number of free edges (edge shared by one face)
        """
    @property
    def nb_multiple_edges(self) -> int:
        """
        Gives the number of multiple edges (edge shared by more than two faces)
        """
    @property
    def non_manifold_mode(self) -> bool:
        """
        Gets/sets mode for non-manifold sewing
        """
    @non_manifold_mode.setter
    def non_manifold_mode(self, arg1: bool) -> None:
        ...
    @property
    def same_parameter_mode(self) -> bool:
        """
        Gets/sets same parameter mode
        """
    @same_parameter_mode.setter
    def same_parameter_mode(self, arg1: bool) -> None:
        ...
    @property
    def tolerance(self) -> float:
        """
        Gets/sets tolerance
        """
    @tolerance.setter
    def tolerance(self, arg1: typing.SupportsFloat) -> None:
        ...
class ShellError:
    """
    Members:
    
      ShellDone
    
      EmptyShell
    
      DisconnectedShell
    
      ShellParametersOutOfRange
    """
    DisconnectedShell: typing.ClassVar[ShellError]  # value = <ShellError.DisconnectedShell: 2>
    EmptyShell: typing.ClassVar[ShellError]  # value = <ShellError.EmptyShell: 1>
    ShellDone: typing.ClassVar[ShellError]  # value = <ShellError.ShellDone: 0>
    ShellParametersOutOfRange: typing.ClassVar[ShellError]  # value = <ShellError.ShellParametersOutOfRange: 3>
    __members__: typing.ClassVar[dict[str, ShellError]]  # value = {'ShellDone': <ShellError.ShellDone: 0>, 'EmptyShell': <ShellError.EmptyShell: 1>, 'DisconnectedShell': <ShellError.DisconnectedShell: 2>, 'ShellParametersOutOfRange': <ShellError.ShellParametersOutOfRange: 3>}
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
class TransitionMode:
    """
    Members:
    
      Transformed
    
      RightCorner
    
      RoundCorner
    """
    RightCorner: typing.ClassVar[TransitionMode]  # value = <TransitionMode.RightCorner: 1>
    RoundCorner: typing.ClassVar[TransitionMode]  # value = <TransitionMode.RoundCorner: 2>
    Transformed: typing.ClassVar[TransitionMode]  # value = <TransitionMode.Transformed: 0>
    __members__: typing.ClassVar[dict[str, TransitionMode]]  # value = {'Transformed': <TransitionMode.Transformed: 0>, 'RightCorner': <TransitionMode.RightCorner: 1>, 'RoundCorner': <TransitionMode.RoundCorner: 2>}
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
class TypeOfContact:
    """
    Members:
    
      NoContact
    
      Contact
    
      ContactOnBorder
    """
    Contact: typing.ClassVar[TypeOfContact]  # value = <TypeOfContact.Contact: 1>
    ContactOnBorder: typing.ClassVar[TypeOfContact]  # value = <TypeOfContact.ContactOnBorder: 2>
    NoContact: typing.ClassVar[TypeOfContact]  # value = <TypeOfContact.NoContact: 0>
    __members__: typing.ClassVar[dict[str, TypeOfContact]]  # value = {'NoContact': <TypeOfContact.NoContact: 0>, 'Contact': <TypeOfContact.Contact: 1>, 'ContactOnBorder': <TypeOfContact.ContactOnBorder: 2>}
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
class WireError:
    """
    Members:
    
      WireDone
    
      EmptyWire
    
      DisconnectedWire
    
      NonManifoldWire
    """
    DisconnectedWire: typing.ClassVar[WireError]  # value = <WireError.DisconnectedWire: 2>
    EmptyWire: typing.ClassVar[WireError]  # value = <WireError.EmptyWire: 1>
    NonManifoldWire: typing.ClassVar[WireError]  # value = <WireError.NonManifoldWire: 3>
    WireDone: typing.ClassVar[WireError]  # value = <WireError.WireDone: 0>
    __members__: typing.ClassVar[dict[str, WireError]]  # value = {'WireDone': <WireError.WireDone: 0>, 'EmptyWire': <WireError.EmptyWire: 1>, 'DisconnectedWire': <WireError.DisconnectedWire: 2>, 'NonManifoldWire': <WireError.NonManifoldWire: 3>}
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
@typing.overload
def make_edge(Pnt1: mod3d.gp.Pnt, Pnt2: mod3d.gp.Pnt) -> mod3d.TopoDS.Edge:
    """
    Creates a straight edge between two points
    """
@typing.overload
def make_edge(C: mod3d.Geom.Curve) -> mod3d.TopoDS.Edge:
    """
    Creates an edge from a Geom_Curve
    """
@typing.overload
def make_edge(C: mod3d.gp.Circ) -> mod3d.TopoDS.Edge:
    """
    Creates a closed edge from a gp_Circ
    """
@typing.overload
def make_edge(C: mod3d.gp.Elips) -> mod3d.TopoDS.Edge:
    """
    Creates a closed edge from a gp_Elips
    """
@typing.overload
def make_edge(L: mod3d.gp.Lin, p1: typing.SupportsFloat, p2: typing.SupportsFloat) -> mod3d.TopoDS.Edge:
    """
    Creates an edge on a line between parameters p1 and p2
    """
@typing.overload
def make_face(S: mod3d.Geom.Surface, TolDegen: typing.SupportsFloat = 1e-06) -> mod3d.TopoDS.Face:
    """
    Creates a face from a Geom_Surface
    """
@typing.overload
def make_face(W: mod3d.TopoDS.Wire) -> mod3d.TopoDS.Face:
    """
    Creates a face from a planar wire
    """
@typing.overload
def make_face(S: mod3d.Geom.Surface, W: mod3d.TopoDS.Wire) -> mod3d.TopoDS.Face:
    """
    Creates a face from a surface trimmed by a wire
    """
@typing.overload
def make_wire(E: mod3d.TopoDS.Edge) -> mod3d.TopoDS.Wire:
    """
    Creates a wire from a single edge
    """
@typing.overload
def make_wire(E1: mod3d.TopoDS.Edge, E2: mod3d.TopoDS.Edge) -> mod3d.TopoDS.Wire:
    """
    Creates a wire from two edges
    """
@typing.overload
def make_wire(C: mod3d.Geom.Curve) -> mod3d.TopoDS.Wire:
    """
    Creates a wire directly from a Geom_Curve (edge is created internally)
    """
@typing.overload
def make_wire(C: mod3d.gp.Circ) -> mod3d.TopoDS.Wire:
    """
    Creates a wire directly from a gp_Circ
    """
@typing.overload
def make_wire(C: mod3d.gp.Elips) -> mod3d.TopoDS.Wire:
    """
    Creates a wire directly from a gp_Elips
    """
Contact: TypeOfContact  # value = <TypeOfContact.Contact: 1>
ContactOnBorder: TypeOfContact  # value = <TypeOfContact.ContactOnBorder: 2>
CurveProjectionFailed: FaceError  # value = <FaceError.CurveProjectionFailed: 3>
DifferentPointsOnClosedCurve: EdgeError  # value = <EdgeError.DifferentPointsOnClosedCurve: 3>
DifferentsPointAndParameter: EdgeError  # value = <EdgeError.DifferentsPointAndParameter: 5>
DisconnectedShell: ShellError  # value = <ShellError.DisconnectedShell: 2>
DisconnectedWire: WireError  # value = <WireError.DisconnectedWire: 2>
EdgeDone: EdgeError  # value = <EdgeError.EdgeDone: 0>
EmptyShell: ShellError  # value = <ShellError.EmptyShell: 1>
EmptyWire: WireError  # value = <WireError.EmptyWire: 1>
FaceDone: FaceError  # value = <FaceError.FaceDone: 0>
ImpossibleContact: PipeError  # value = <PipeError.ImpossibleContact: 3>
LineThroughIdenticPoints: EdgeError  # value = <EdgeError.LineThroughIdenticPoints: 6>
NoContact: TypeOfContact  # value = <TypeOfContact.NoContact: 0>
NoFace: FaceError  # value = <FaceError.NoFace: 1>
NonManifoldWire: WireError  # value = <WireError.NonManifoldWire: 3>
NotPlanar: FaceError  # value = <FaceError.NotPlanar: 2>
ParameterOutOfRange: EdgeError  # value = <EdgeError.ParameterOutOfRange: 2>
ParametersOutOfRange: FaceError  # value = <FaceError.ParametersOutOfRange: 4>
PipeDone: PipeError  # value = <PipeError.PipeDone: 0>
PipeNotDone: PipeError  # value = <PipeError.PipeNotDone: 1>
PlaneNotIntersectGuide: PipeError  # value = <PipeError.PlaneNotIntersectGuide: 2>
PointProjectionFailed: EdgeError  # value = <EdgeError.PointProjectionFailed: 1>
PointWithInfiniteParameter: EdgeError  # value = <EdgeError.PointWithInfiniteParameter: 4>
RightCorner: TransitionMode  # value = <TransitionMode.RightCorner: 1>
RoundCorner: TransitionMode  # value = <TransitionMode.RoundCorner: 2>
ShellDone: ShellError  # value = <ShellError.ShellDone: 0>
ShellParametersOutOfRange: ShellError  # value = <ShellError.ShellParametersOutOfRange: 3>
Transformed: TransitionMode  # value = <TransitionMode.Transformed: 0>
WireDone: WireError  # value = <WireError.WireDone: 0>
