"""
BRep fillet module
"""
from __future__ import annotations
import mod3d.BRepBuilderAPI
import mod3d.GeomAbs
import mod3d.TopoDS
import typing
__all__: list[str] = ['BothEdgesDegenerated', 'ChamfMode', 'ClassicChamfer', 'ComputationError', 'ConnexionError', 'ConstThroatChamfer', 'ConstThroatWithPenetrationChamfer', 'ConstructionError', 'Error', 'ErrorStatus', 'FilletLocalOperation', 'FilletShape', 'FirstEdgeDegenerated', 'InitialisationError', 'IsDone', 'IsDoneNoError', 'LastEdgeDegenerated', 'MakeChamfer', 'MakeFillet', 'MakeFillet2d', 'NoFace', 'NotAuthorized', 'NotPlanar', 'Ok', 'ParametersError', 'Polynomial', 'QuasiAngular', 'Rational', 'Ready', 'StartsolFailure', 'TangencyError', 'TwistedSurface', 'WalkingFailure']
class ChamfMode:
    """
    Members:
    
      ClassicChamfer : Chamfer with constant distance from spine to one of the two surfaces
    
      ConstThroatChamfer : Symmetric chamfer with constant throat (height of isosceles triangle in section)
    
      ConstThroatWithPenetrationChamfer : Chamfer with constant throat: right-angled triangle section with penetration
    """
    ClassicChamfer: typing.ClassVar[ChamfMode]  # value = <ChamfMode.ClassicChamfer: 0>
    ConstThroatChamfer: typing.ClassVar[ChamfMode]  # value = <ChamfMode.ConstThroatChamfer: 1>
    ConstThroatWithPenetrationChamfer: typing.ClassVar[ChamfMode]  # value = <ChamfMode.ConstThroatWithPenetrationChamfer: 2>
    __members__: typing.ClassVar[dict[str, ChamfMode]]  # value = {'ClassicChamfer': <ChamfMode.ClassicChamfer: 0>, 'ConstThroatChamfer': <ChamfMode.ConstThroatChamfer: 1>, 'ConstThroatWithPenetrationChamfer': <ChamfMode.ConstThroatWithPenetrationChamfer: 2>}
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
class ConstructionError:
    """
    Members:
    
      IsDone : Construction successful
    
      NotPlanar : Face is not planar
    
      NoFace : Face is null
    
      InitialisationError : Initialization error
    
      ParametersError : Invalid parameters
    
      Ready : Ready for construction
    
      IsDoneNoError : Done without error
    
      ComputationError : Computation error
    
      ConnexionError : Connection error
    
      TangencyError : Tangency error
    
      FirstEdgeDegenerated : First edge is degenerated
    
      LastEdgeDegenerated : Last edge is degenerated
    
      BothEdgesDegenerated : Both edges are degenerated
    
      NotAuthorized : Operation not authorized
    """
    BothEdgesDegenerated: typing.ClassVar[ConstructionError]  # value = <ConstructionError.BothEdgesDegenerated: 11>
    ComputationError: typing.ClassVar[ConstructionError]  # value = <ConstructionError.ComputationError: 6>
    ConnexionError: typing.ClassVar[ConstructionError]  # value = <ConstructionError.ConnexionError: 7>
    FirstEdgeDegenerated: typing.ClassVar[ConstructionError]  # value = <ConstructionError.FirstEdgeDegenerated: 9>
    InitialisationError: typing.ClassVar[ConstructionError]  # value = <ConstructionError.InitialisationError: 2>
    IsDone: typing.ClassVar[ConstructionError]  # value = <ConstructionError.IsDone: 5>
    IsDoneNoError: typing.ClassVar[ConstructionError]  # value = <ConstructionError.IsDone: 5>
    LastEdgeDegenerated: typing.ClassVar[ConstructionError]  # value = <ConstructionError.LastEdgeDegenerated: 10>
    NoFace: typing.ClassVar[ConstructionError]  # value = <ConstructionError.NoFace: 1>
    NotAuthorized: typing.ClassVar[ConstructionError]  # value = <ConstructionError.NotAuthorized: 12>
    NotPlanar: typing.ClassVar[ConstructionError]  # value = <ConstructionError.NotPlanar: 0>
    ParametersError: typing.ClassVar[ConstructionError]  # value = <ConstructionError.ParametersError: 3>
    Ready: typing.ClassVar[ConstructionError]  # value = <ConstructionError.Ready: 4>
    TangencyError: typing.ClassVar[ConstructionError]  # value = <ConstructionError.TangencyError: 8>
    __members__: typing.ClassVar[dict[str, ConstructionError]]  # value = {'IsDone': <ConstructionError.IsDone: 5>, 'NotPlanar': <ConstructionError.NotPlanar: 0>, 'NoFace': <ConstructionError.NoFace: 1>, 'InitialisationError': <ConstructionError.InitialisationError: 2>, 'ParametersError': <ConstructionError.ParametersError: 3>, 'Ready': <ConstructionError.Ready: 4>, 'IsDoneNoError': <ConstructionError.IsDone: 5>, 'ComputationError': <ConstructionError.ComputationError: 6>, 'ConnexionError': <ConstructionError.ConnexionError: 7>, 'TangencyError': <ConstructionError.TangencyError: 8>, 'FirstEdgeDegenerated': <ConstructionError.FirstEdgeDegenerated: 9>, 'LastEdgeDegenerated': <ConstructionError.LastEdgeDegenerated: 10>, 'BothEdgesDegenerated': <ConstructionError.BothEdgesDegenerated: 11>, 'NotAuthorized': <ConstructionError.NotAuthorized: 12>}
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
class ErrorStatus:
    """
    Members:
    
      Ok : Computation is Ok
    
      Error : General error
    
      WalkingFailure : Problem in the walking
    
      StartsolFailure : Computation can't start, perhaps radius is too big
    
      TwistedSurface : Computation failed because of a twisted surface
    """
    Error: typing.ClassVar[ErrorStatus]  # value = <ErrorStatus.Error: 1>
    Ok: typing.ClassVar[ErrorStatus]  # value = <ErrorStatus.Ok: 0>
    StartsolFailure: typing.ClassVar[ErrorStatus]  # value = <ErrorStatus.StartsolFailure: 3>
    TwistedSurface: typing.ClassVar[ErrorStatus]  # value = <ErrorStatus.TwistedSurface: 4>
    WalkingFailure: typing.ClassVar[ErrorStatus]  # value = <ErrorStatus.WalkingFailure: 2>
    __members__: typing.ClassVar[dict[str, ErrorStatus]]  # value = {'Ok': <ErrorStatus.Ok: 0>, 'Error': <ErrorStatus.Error: 1>, 'WalkingFailure': <ErrorStatus.WalkingFailure: 2>, 'StartsolFailure': <ErrorStatus.StartsolFailure: 3>, 'TwistedSurface': <ErrorStatus.TwistedSurface: 4>}
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
class FilletLocalOperation(mod3d.BRepBuilderAPI.MakeShape):
    """
    Abstract base class for local operations (fillets, chamfers) on edges.
    
    Manages contours of tangent edges and provides methods for querying
    and manipulating these contours.
    """
    def abscissa(self, ic: typing.SupportsInt, vertex: mod3d.TopoDS.Vertex) -> float:
        """
        Returns the abscissa of the vertex on the contour of index ic
        """
    def add(self, edge: mod3d.TopoDS.Edge) -> None:
        """
        Adds a contour in the builder (builds a contour of tangent edges)
        """
    def closed(self, ic: typing.SupportsInt) -> bool:
        """
        Returns True if the contour of index ic is closed
        """
    def closed_and_tangent(self, ic: typing.SupportsInt) -> bool:
        """
        Returns True if the contour of index ic is closed and tangent
        """
    def contour(self, edge: mod3d.TopoDS.Edge) -> int:
        """
        Returns the index of the contour containing the edge.
        Returns 0 if edge doesn't belong to any contour
        """
    def edge(self, i: typing.SupportsInt, j: typing.SupportsInt) -> mod3d.TopoDS.Edge:
        """
        Returns the edge j in contour i
        """
    def first_vertex(self, ic: typing.SupportsInt) -> mod3d.TopoDS.Vertex:
        """
        Returns the first vertex of the contour of index ic
        """
    def last_vertex(self, ic: typing.SupportsInt) -> mod3d.TopoDS.Vertex:
        """
        Returns the last vertex of the contour of index ic
        """
    def length(self, ic: typing.SupportsInt) -> float:
        """
        Returns the length of the contour of index ic
        """
    def nb_edges(self, i: typing.SupportsInt) -> int:
        """
        Returns the number of edges in contour i
        """
    def nb_surf(self, ic: typing.SupportsInt) -> int:
        """
        Returns the number of surfaces for contour ic
        """
    def relative_abscissa(self, ic: typing.SupportsInt, vertex: mod3d.TopoDS.Vertex) -> float:
        """
        Returns the relative abscissa ([0, 1]) of the vertex on contour ic
        """
    def remove(self, edge: mod3d.TopoDS.Edge) -> None:
        """
        Removes the contour containing the edge
        """
    def reset(self) -> None:
        """
        Resets all fields updated by Build operation.
        
        Leaves the algorithm in the same state as before build call.
        Allows contours and radius modifications to build the result another time
        """
    def reset_contour(self, ic: typing.SupportsInt) -> None:
        """
        Resets the contour of index ic, removing all information in the contour
        """
    def sect(self, ic: typing.SupportsInt, is: typing.SupportsInt) -> ...:
        """
        Returns the section array for contour ic, surface is
        """
    def simulate(self, ic: typing.SupportsInt) -> None:
        """
        Simulates the operation on contour ic
        """
    @property
    def nb_contours(self) -> int:
        """
        Number of contours
        """
class FilletShape:
    """
    Members:
    
      Rational : Classical NURBS representation of circles
    
      QuasiAngular : NURBS representation with parameterization matching the circle
    
      Polynomial : Polynomial representation of circles
    """
    Polynomial: typing.ClassVar[FilletShape]  # value = <FilletShape.Polynomial: 2>
    QuasiAngular: typing.ClassVar[FilletShape]  # value = <FilletShape.QuasiAngular: 1>
    Rational: typing.ClassVar[FilletShape]  # value = <FilletShape.Rational: 0>
    __members__: typing.ClassVar[dict[str, FilletShape]]  # value = {'Rational': <FilletShape.Rational: 0>, 'QuasiAngular': <FilletShape.QuasiAngular: 1>, 'Polynomial': <FilletShape.Polynomial: 2>}
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
class MakeChamfer(FilletLocalOperation):
    """
    Builds chamfers on edges of a shell or solid.
    
    Provides a framework for:
    - Initializing the construction algorithm with a shape
    - Defining chamfer parameters (symmetric, two distances, distance+angle)
    - Building the chamfers and constructing the resulting shape
    - Consulting the result
    """
    def __init__(self, shape: mod3d.TopoDS.Shape) -> None:
        """
        Initializes an algorithm for computing chamfers on the shape.
        The edges on which chamfers are built are defined using Add
        """
    @typing.overload
    def add(self, edge: mod3d.TopoDS.Edge) -> None:
        """
        Adds edge to the table of edges (parameters set later)
        """
    @typing.overload
    def add(self, dis: typing.SupportsFloat, edge: mod3d.TopoDS.Edge) -> None:
        """
        Adds edge with symmetric chamfer distance.
        
        Builds a contour by propagation from the edge (contour contains
        at least this edge). The contour is composed of edges tangential
        to one another, delimiting two series of tangential faces
        """
    @typing.overload
    def add(self, dis1: typing.SupportsFloat, dis2: typing.SupportsFloat, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> None:
        """
        Adds edge with two distances chamfer.
        
        Parameters:
          dis1: distance measured on face
          dis2: distance measured on the other face
          edge: edge to chamfer
          face: face where dis1 is measured
        
        Builds a contour by propagation from the edge
        """
    def add_da(self, dis: typing.SupportsFloat, angle: typing.SupportsFloat, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> None:
        """
        Adds a chamfer contour with distance and angle parameters.
        
        Parameters:
          dis: distance from the edge
          angle: angle of the chamfer
          edge: edge to chamfer
          face: reference face
        """
    def build(self) -> None:
        """
        Builds the chamfers on all contours.
        
        Constructs the resulting shape with chamfers applied.
        Use IsDone() to verify success and Shape() to retrieve result.
        
        Warning: Complex construction may fail. Current limitations:
        - End point is intersection of 4+ edges
        - Chamfer intersection with limiting face not fully contained
        """
    def builder(self) -> ...:
        """
        Returns the internal filleting algorithm builder
        """
    def dists(self, ic: typing.SupportsInt) -> tuple:
        """
        Returns tuple (dis1, dis2) for contour ic.
        Returns (-1, -1) if ic is outside bounds
        """
    def generated(self, shape: mod3d.TopoDS.Shape) -> ...:
        """
        Returns the list of shapes generated from the shape
        """
    def get_dist(self, ic: typing.SupportsInt) -> float:
        """
        Returns the symmetric distance for contour ic
        """
    def get_dist_angle(self, ic: typing.SupportsInt) -> tuple:
        """
        Returns tuple (distance, angle) for contour ic
        """
    def is_deleted(self, shape: mod3d.TopoDS.Shape) -> bool:
        """
        Returns True if the shape was deleted during chamfer operation
        """
    def is_distance_angle(self, ic: typing.SupportsInt) -> bool:
        """
        Returns True if contour ic is made with distance and angle
        """
    def is_symetric(self, ic: typing.SupportsInt) -> bool:
        """
        Returns True if contour ic is a symmetric chamfer
        """
    def is_two_distances(self, ic: typing.SupportsInt) -> bool:
        """
        Returns True if contour ic is made with two distances
        """
    def modified(self, shape: mod3d.TopoDS.Shape) -> ...:
        """
        Returns the list of shapes modified from the shape
        """
    def reset(self) -> None:
        """
        Reinitializes the algorithm, canceling Build effects.
        Allows modifications to contours and parameters to rebuild
        """
    def set_dist(self, dis: typing.SupportsFloat, ic: typing.SupportsInt, face: mod3d.TopoDS.Face) -> None:
        """
        Sets the symmetric distance for contour ic.
        
        Parameters:
          dis: symmetric distance
          ic: contour index
          face: reference face
        """
    def set_dist_angle(self, dis: typing.SupportsFloat, angle: typing.SupportsFloat, ic: typing.SupportsInt, face: mod3d.TopoDS.Face) -> None:
        """
        Sets distance and angle for contour ic.
        
        Parameters:
          dis: distance from edge
          angle: chamfer angle
          ic: contour index
          face: reference face
        """
    def set_dists(self, dis1: typing.SupportsFloat, dis2: typing.SupportsFloat, ic: typing.SupportsInt, face: mod3d.TopoDS.Face) -> None:
        """
        Sets the two distances for contour ic.
        
        Parameters:
          dis1: distance measured on face
          dis2: distance measured on the other face
          ic: contour index
          face: face where dis1 is measured
        """
    def set_mode(self, mode: ChamfMode) -> None:
        """
        Sets the chamfer mode (Sym, TwoDist, or DistAngle)
        """
class MakeFillet(FilletLocalOperation):
    """
    Builds fillets on edges of a shell or solid.
    
    Provides a framework for:
    - Initializing the construction algorithm with a shape
    - Defining fillet parameters (constant, linear evolution, law-based)
    - Building the fillets and constructing the resulting shape
    - Consulting the result and error information
    """
    def __init__(self, shape: mod3d.TopoDS.Shape, fshape: FilletShape = ...) -> None:
        """
        Initializes the computation of fillets.
        
        fshape sets the type of fillet surface:
        - Rational (default): classical NURBS representation of circles
        - QuasiAngular: NURBS with parameterization matching the circle
        - Polynomial: polynomial representation of circles
        """
    @typing.overload
    def add(self, edge: mod3d.TopoDS.Edge) -> None:
        """
        Adds a fillet contour (builds contour of tangent edges).
        The radius must be set after
        """
    @typing.overload
    def add(self, radius: typing.SupportsFloat, edge: mod3d.TopoDS.Edge) -> None:
        """
        Adds a fillet with constant radius.
        
        Builds a contour of tangent edges and sets the radius
        """
    @typing.overload
    def add(self, r1: typing.SupportsFloat, r2: typing.SupportsFloat, edge: mod3d.TopoDS.Edge) -> None:
        """
        Adds a fillet with linear radius evolution.
        
        Builds a contour of tangent edges and sets linear radius evolution
        law between the first and last vertex of the spine
        """
    @typing.overload
    def add(self, law: ..., edge: mod3d.TopoDS.Edge) -> None:
        """
        Adds a fillet with radius evolution law.
        
        Builds a contour of tangent edges and sets the radius evolution law
        """
    @typing.overload
    def add(self, uandr: ..., edge: mod3d.TopoDS.Edge) -> None:
        """
        Adds a fillet with interpolated radius values.
        
        Builds a contour of tangent edges and sets radius evolution law
        interpolating the values given in the array UandR:
          p2d.X() = relative parameter on the spine [0,1]
          p2d.Y() = value of the radius
        """
    def bad_shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the partial result if HasResult() is True
        """
    def build(self) -> None:
        """
        Builds the fillets on all contours.
        
        Constructs the resulting shape with fillets applied.
        Use IsDone() to verify success and Shape() to retrieve result.
        
        Warning: Complex construction may fail. Current limitations:
        - End point is intersection of 4+ edges
        - Fillet intersection with limiting face not fully contained
        """
    def builder(self) -> ...:
        """
        Returns the internal topology building algorithm
        """
    def computed_surface(self, ic: typing.SupportsInt, is: typing.SupportsInt) -> ...:
        """
        Returns surface number is for contour ic
        """
    def faulty_contour(self, i: typing.SupportsInt) -> int:
        """
        Returns the index of the i-th faulty contour.
        Use NbEdges(ic) and Edge(ic,ie) to get contour details
        """
    def faulty_vertex(self, iv: typing.SupportsInt) -> mod3d.TopoDS.Vertex:
        """
        Returns the vertex where computation failed
        """
    def generated(self, shape: mod3d.TopoDS.Shape) -> ...:
        """
        Returns the list of shapes generated from the shape
        """
    def get_bounds(self, ic: typing.SupportsInt, edge: mod3d.TopoDS.Edge) -> tuple:
        """
        Returns tuple (success, first, last) bounds for edge in contour ic
        """
    def get_law(self, ic: typing.SupportsInt, edge: mod3d.TopoDS.Edge) -> ...:
        """
        Returns the radius evolution law for edge in contour ic
        """
    def has_result(self) -> bool:
        """
        Returns True if a part of the result has been computed.
        If filling in a corner failed, a shape with a hole is returned
        """
    def is_constant(self, ic: typing.SupportsInt) -> bool:
        """
        Returns True if the radius along contour ic is constant.
        Returns False if ic is outside bounds
        """
    def is_constant_on_edge(self, ic: typing.SupportsInt, edge: mod3d.TopoDS.Edge) -> bool:
        """
        Returns True if radius along edge E of contour ic is constant.
        Returns False if ic is outside bounds or edge not in contour
        """
    def is_deleted(self, shape: mod3d.TopoDS.Shape) -> bool:
        """
        Returns True if the shape was deleted during fillet operation
        """
    def modified(self, shape: mod3d.TopoDS.Shape) -> ...:
        """
        Returns the list of shapes modified from the shape
        """
    def nb_computed_surfaces(self, ic: typing.SupportsInt) -> int:
        """
        Returns the number of surfaces computed on contour ic
        """
    def new_faces(self, i: typing.SupportsInt) -> ...:
        """
        Returns the faces created for surface i
        """
    def radius(self, ic: typing.SupportsInt) -> float:
        """
        Returns the radius of the fillet along contour ic.
        Use only if radius is constant. Returns -1 if ic outside bounds
        """
    def radius_on_edge(self, ic: typing.SupportsInt, edge: mod3d.TopoDS.Edge) -> float:
        """
        Returns the radius of the fillet along edge of contour ic.
        Use only if radius is constant. Returns -1 if invalid
        """
    def reset(self) -> None:
        """
        Reinitializes the algorithm, canceling Build effects.
        Allows modifications to contours and parameters to rebuild
        """
    def set_continuity(self, internal_continuity: mod3d.GeomAbs.Shape, angular_tolerance: typing.SupportsFloat) -> None:
        """
        Changes the parameters of continuity.
        
        Produces fillet surfaces with continuity Ci (i=0,1 or 2).
        Default InternalContinuity = GeomAbs_C1.
        AngularTolerance is the G1 tolerance between fillet and support faces
        """
    def set_fillet_shape(self, fshape: FilletShape) -> None:
        """
        Assigns fshape as the type of fillet shape built by this algorithm
        """
    def set_law(self, ic: typing.SupportsInt, edge: mod3d.TopoDS.Edge, law: ...) -> None:
        """
        Sets the radius evolution law for edge in contour ic
        """
    def set_params(self, tang: typing.SupportsFloat, tesp: typing.SupportsFloat, t2d: typing.SupportsFloat, tapp3d: typing.SupportsFloat, tolapp2d: typing.SupportsFloat, fleche: typing.SupportsFloat) -> None:
        """
        Sets the computation parameters
        """
    @typing.overload
    def set_radius(self, radius: typing.SupportsFloat, ic: typing.SupportsInt, iinc: typing.SupportsInt) -> None:
        """
        Sets constant radius for contour ic at edge iinc
        """
    @typing.overload
    def set_radius(self, r1: typing.SupportsFloat, r2: typing.SupportsFloat, ic: typing.SupportsInt, iinc: typing.SupportsInt) -> None:
        """
        Sets linear radius evolution from r1 to r2 for contour ic
        """
    @typing.overload
    def set_radius(self, law: ..., ic: typing.SupportsInt, iinc: typing.SupportsInt) -> None:
        """
        Sets radius evolution law for contour ic
        """
    @typing.overload
    def set_radius(self, uandr: ..., ic: typing.SupportsInt, iinc: typing.SupportsInt) -> None:
        """
        Sets radius evolution by interpolating UandR values for contour ic
        """
    def set_radius_on_edge(self, radius: typing.SupportsFloat, ic: typing.SupportsInt, edge: mod3d.TopoDS.Edge) -> None:
        """
        Assigns radius as the radius of the fillet on edge
        """
    def set_radius_on_vertex(self, radius: typing.SupportsFloat, ic: typing.SupportsInt, vertex: mod3d.TopoDS.Vertex) -> None:
        """
        Sets radius at vertex for contour ic
        """
    def stripe_status(self, ic: typing.SupportsInt) -> ErrorStatus:
        """
        Returns the error status for contour ic:
        - Ok: computation successful
        - StartsolFailure: can't start, radius may be too big
        - TwistedSurface: failed due to twisted surface
        - WalkingFailure: problem in the walking
        - Error: other error
        """
    @property
    def fillet_shape(self) -> FilletShape:
        """
        Type of fillet shape (Rational, QuasiAngular, or Polynomial)
        """
    @fillet_shape.setter
    def fillet_shape(self, arg1: FilletShape) -> None:
        ...
    @property
    def nb_faulty_contours(self) -> int:
        """
        Number of contours where computation failed
        """
    @property
    def nb_faulty_vertices(self) -> int:
        """
        Number of vertices where computation failed
        """
    @property
    def nb_surfaces(self) -> int:
        """
        Number of surfaces after shape creation
        """
class MakeFillet2d(mod3d.BRepBuilderAPI.MakeShape):
    """
    Builds fillets and chamfers on vertices of a planar face.
    
    Provides a framework for:
    - Initializing the construction with a planar face
    - Adding/modifying/removing fillets at vertices
    - Adding/modifying/removing chamfers between edges
    - Building the result and consulting modifications
    
    Warning: Only segments of straight lines and arcs of circles are treated.
    BSplines are not processed.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Initializes an empty algorithm for computing fillets and chamfers.
        The face must be defined using Init function.
        Check status with Status() method after initialization
        """
    @typing.overload
    def __init__(self, face: mod3d.TopoDS.Face) -> None:
        """
        Initializes the algorithm for computing fillets and chamfers on face F.
        
        Status after initialization can be:
        - Ready: initialization correct
        - NotPlanar: F is not planar
        - NoFace: F is a null face
        """
    @typing.overload
    def add_chamfer(self, e1: mod3d.TopoDS.Edge, e2: mod3d.TopoDS.Edge, d1: typing.SupportsFloat, d2: typing.SupportsFloat) -> mod3d.TopoDS.Edge:
        """
        Adds a chamfer between adjacent edges E1 and E2.
        
        Chamfer extremities are on E1 and E2 at distances D1 and D2 respectively.
        For non-rectilinear edges, distances measured using curvilinear abscissa.
        Returns the chamfer edge and builds the resulting face
        """
    @typing.overload
    def add_chamfer(self, edge: mod3d.TopoDS.Edge, vertex: mod3d.TopoDS.Vertex, d: typing.SupportsFloat, ang: typing.SupportsFloat) -> mod3d.TopoDS.Edge:
        """
        Adds a chamfer between two edges connected by vertex V.
        
        E is one of the two edges. Chamfer makes angle Ang with E
        and one extremity is on E at distance D from V.
        Angle Ang is given in radians.
        Returns the chamfer edge and builds the resulting face.
        
        Status can be:
        - IsDone: chamfer built
        - ParametersError: D or Ang <= 0
        - ConnexionError: E doesn't belong to face or V not on E
        - ComputationError: parameters too large
        - NotAuthorized: edge is fillet/chamfer or not straight/arc
        """
    def add_fillet(self, vertex: mod3d.TopoDS.Vertex, radius: typing.SupportsFloat) -> mod3d.TopoDS.Edge:
        """
        Adds a fillet of radius between two edges adjacent to vertex V.
        
        The two edges do not need to be rectilinear.
        Returns the fillet edge and builds the resulting face.
        
        Status can be:
        - IsDone: fillet built successfully
        - ConnexionError: V doesn't belong to initial face
        - ComputationError: Radius too large
        - NotAuthorized: edge connected to V is fillet/chamfer or not straight/arc
        
        Raises Standard_NegativeValue if Radius <= 0
        """
    def basis_edge(self, edge: mod3d.TopoDS.Edge) -> mod3d.TopoDS.Edge:
        """
        Returns the basis edge from which chamfered/filleted edge E is built.
        If E has not been modified, returns E.
        Returns E if it doesn't belong to initial face
        """
    def build(self) -> None:
        """
        Updates the result and sets the Done flag
        """
    def descendant_edge(self, edge: mod3d.TopoDS.Edge) -> mod3d.TopoDS.Edge:
        """
        Returns the chamfered or filleted edge built from edge E.
        If E has not been modified, returns E.
        
        Raises Standard_NoSuchObject if E doesn't belong to initial face
        """
    def has_descendant(self, edge: mod3d.TopoDS.Edge) -> bool:
        """
        Returns True if edge E has a descendant (modified edge)
        """
    @typing.overload
    def init(self, face: mod3d.TopoDS.Face) -> None:
        """
        Initializes this algorithm with face F.
        
        Status after initialization can be:
        - Ready: initialization correct
        - NotPlanar: F is not planar
        - NoFace: F is a null face
        """
    @typing.overload
    def init(self, ref_face: mod3d.TopoDS.Face, mod_face: mod3d.TopoDS.Face) -> None:
        """
        Initializes from reference face and modified face.
        
        This is useful to modify a fillet or chamfer already created on ModFace
        which derives from RefFace
        """
    def is_modified(self, edge: mod3d.TopoDS.Edge) -> bool:
        """
        Returns True if edge E is chamfered or filleted.
        Returns False if E doesn't belong to the face
        """
    def modified(self, shape: mod3d.TopoDS.Shape) -> ...:
        """
        Returns the list of shapes modified from the shape
        """
    @typing.overload
    def modify_chamfer(self, chamfer: mod3d.TopoDS.Edge, e1: mod3d.TopoDS.Edge, e2: mod3d.TopoDS.Edge, d1: typing.SupportsFloat, d2: typing.SupportsFloat) -> mod3d.TopoDS.Edge:
        """
        Modifies existing chamfer with new distances.
        
        E1 and E2 are the two adjacent edges on which Chamfer is built.
        New chamfer extremities are on E1 and E2 at distances D1 and D2.
        Returns the new chamfer edge
        """
    @typing.overload
    def modify_chamfer(self, chamfer: mod3d.TopoDS.Edge, edge: mod3d.TopoDS.Edge, d: typing.SupportsFloat, ang: typing.SupportsFloat) -> mod3d.TopoDS.Edge:
        """
        Modifies existing chamfer with new distance and angle.
        
        E is one of the two adjacent edges on which Chamfer is built.
        New chamfer makes angle Ang with E and one extremity is
        on E at distance D from the vertex.
        Angle Ang is given in radians.
        Returns the new chamfer edge and modifies the existing face.
        
        Status can be:
        - IsDone: chamfer built
        - ParametersError: D or Ang <= 0
        - ConnexionError: edge or chamfer doesn't belong to face
        - ComputationError: parameters too large
        - NotAuthorized: E1 or E2 is fillet/chamfer
        """
    def modify_fillet(self, fillet: mod3d.TopoDS.Edge, radius: typing.SupportsFloat) -> mod3d.TopoDS.Edge:
        """
        Assigns new radius to existing fillet.
        
        Returns the new fillet edge and modifies the existing face.
        
        Status can be:
        - IsDone: new fillet built
        - ConnexionError: Fillet doesn't belong to face
        - ComputationError: Radius too large
        
        Raises Standard_NegativeValue if Radius <= 0
        """
    def new_edges(self, i: typing.SupportsInt) -> ...:
        """
        Returns the edges created for curve i
        """
    def remove_chamfer(self, chamfer: mod3d.TopoDS.Edge) -> mod3d.TopoDS.Vertex:
        """
        Removes the chamfer and modifies the existing face.
        
        Returns the vertex connecting the two adjacent edges.
        Vertex is only valid if Status returns IsDone.
        Returns null vertex if edge doesn't belong to initial face
        """
    def remove_fillet(self, fillet: mod3d.TopoDS.Edge) -> mod3d.TopoDS.Vertex:
        """
        Removes the fillet and modifies the existing face.
        
        Returns the vertex connecting the two adjacent edges.
        Vertex is only valid if Status returns IsDone.
        Returns null vertex if edge doesn't belong to initial face
        """
    @property
    def chamfer_edges(self) -> ...:
        """
        Table of chamfers on the modified face
        """
    @property
    def fillet_edges(self) -> ...:
        """
        Table of fillets on the modified face
        """
    @property
    def nb_chamfer(self) -> int:
        """
        Number of chamfers on the modified face
        """
    @property
    def nb_curves(self) -> int:
        """
        Number of new curves after shape creation
        """
    @property
    def nb_fillet(self) -> int:
        """
        Number of fillets on the modified face
        """
    @property
    def status(self) -> ConstructionError:
        """
        Returns the construction status:
        - IsDone/Ready: successful
        - NotPlanar: face not planar
        - NoFace: null face
        - ParametersError: invalid parameters
        - ConnexionError: connectivity error
        - ComputationError: computation failed
        - NotAuthorized: operation not allowed
        """
BothEdgesDegenerated: ConstructionError  # value = <ConstructionError.BothEdgesDegenerated: 11>
ClassicChamfer: ChamfMode  # value = <ChamfMode.ClassicChamfer: 0>
ComputationError: ConstructionError  # value = <ConstructionError.ComputationError: 6>
ConnexionError: ConstructionError  # value = <ConstructionError.ConnexionError: 7>
ConstThroatChamfer: ChamfMode  # value = <ChamfMode.ConstThroatChamfer: 1>
ConstThroatWithPenetrationChamfer: ChamfMode  # value = <ChamfMode.ConstThroatWithPenetrationChamfer: 2>
Error: ErrorStatus  # value = <ErrorStatus.Error: 1>
FirstEdgeDegenerated: ConstructionError  # value = <ConstructionError.FirstEdgeDegenerated: 9>
InitialisationError: ConstructionError  # value = <ConstructionError.InitialisationError: 2>
IsDone: ConstructionError  # value = <ConstructionError.IsDone: 5>
IsDoneNoError: ConstructionError  # value = <ConstructionError.IsDone: 5>
LastEdgeDegenerated: ConstructionError  # value = <ConstructionError.LastEdgeDegenerated: 10>
NoFace: ConstructionError  # value = <ConstructionError.NoFace: 1>
NotAuthorized: ConstructionError  # value = <ConstructionError.NotAuthorized: 12>
NotPlanar: ConstructionError  # value = <ConstructionError.NotPlanar: 0>
Ok: ErrorStatus  # value = <ErrorStatus.Ok: 0>
ParametersError: ConstructionError  # value = <ConstructionError.ParametersError: 3>
Polynomial: FilletShape  # value = <FilletShape.Polynomial: 2>
QuasiAngular: FilletShape  # value = <FilletShape.QuasiAngular: 1>
Rational: FilletShape  # value = <FilletShape.Rational: 0>
Ready: ConstructionError  # value = <ConstructionError.Ready: 4>
StartsolFailure: ErrorStatus  # value = <ErrorStatus.StartsolFailure: 3>
TangencyError: ConstructionError  # value = <ConstructionError.TangencyError: 8>
TwistedSurface: ErrorStatus  # value = <ErrorStatus.TwistedSurface: 4>
WalkingFailure: ErrorStatus  # value = <ErrorStatus.WalkingFailure: 2>
