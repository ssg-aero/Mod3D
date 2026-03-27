"""
BRep offset API
"""
from __future__ import annotations
import mod3d.BRepBuilderAPI
import mod3d.GeomAbs
import mod3d.TopoDS
import mod3d.gp
import typing
__all__: list[str] = ['Centripetal', 'ChordLength', 'Done', 'Failed', 'IsoParametric', 'MakeEvolved', 'MakeFilling', 'MakeOffset', 'MakeOffsetShape', 'MiddlePath', 'NormalProjection', 'NotDone', 'NotSameTopology', 'Null3DCurve', 'OffsetMode', 'ParametrizationType', 'Pipe', 'ProfilesInconsistent', 'RectoVerso', 'Skin', 'ThruSectionStatus', 'ThruSections', 'WrongUsage']
class MakeEvolved(mod3d.BRepBuilderAPI.MakeShape):
    """
    Builds evolved shapes by sweeping a profile along a spine.
    
    An evolved shape is the unlooped sweep (pipe) of a profile (wire)
    along a planar spine (face or wire) with self-intersections removed.
    
    The profile is defined in a reference frame R. The position of the
    profile at the current point of the spine is given by confusing R
    and the local referential (D0, D1, and spine normal).
    
    Coordinate system determination (is_axe_prof argument):
    - True: R is the global coordinate system
    - False: R is computed with:
      * Origin: point on spine closest to profile
      * X Axis: tangent to spine at this point
      * Z Axis: normal to plane containing spine
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty evolved shape
        """
    @typing.overload
    def __init__(self, spine: mod3d.TopoDS.Shape, profile: mod3d.TopoDS.Wire, join_type: mod3d.GeomAbs.JoinType = ..., is_axe_prof: bool = True, is_solid: bool = False, is_prof_on_spine: bool = False, tol: typing.SupportsFloat = 1e-07, is_volume: bool = False, run_in_parallel: bool = False) -> None:
        """
        Constructs an evolved shape by sweeping profile along spine.
        
        Parameters:
          spine: Wire or face to sweep along (must be planar)
          profile: Wire profile to sweep (expected to be planar or a line)
          join_type: Type of pipe at salient vertices (default: Arc)
          is_axe_prof: True for global coords, False for computed frame
          is_solid: True to complete result as solid/compound of solids
          is_prof_on_spine: True if profile must connect with spine
          tol: Tolerance for construction (default: 1e-7)
          is_volume: True to remove self-intersections with MakerVolume
          run_in_parallel: True to enable parallel computation
        
        Note: When is_volume=True, join_type, is_axe_prof, and
        is_prof_on_spine are not used
        """
    def build(self) -> None:
        """
        Builds the resulting evolved shape.
        
        Performs the sweep operation and removes self-intersections.
        Use IsDone() to check success and Shape() to retrieve result
        """
    def evolved(self) -> BRepFill_Evolved:
        """
        Returns the internal BRepFill_Evolved object
        """
    def generated_shapes(self, spine_shape: mod3d.TopoDS.Shape, prof_shape: mod3d.TopoDS.Shape) -> ...:
        """
        Returns shapes created from spine and profile subshapes.
        
        Parameters:
          spine_shape: A subshape of the spine
          prof_shape: A subshape of the profile
        
        Returns the list of shapes generated from these subshapes
        """
    @property
    def bottom(self) -> mod3d.TopoDS.Shape:
        """
        Returns the bottom face (only valid if is_solid=True in constructor)
        """
    @property
    def top(self) -> mod3d.TopoDS.Shape:
        """
        Returns the top face (only valid if is_solid=True in constructor)
        """
class MakeFilling(mod3d.BRepBuilderAPI.MakeShape):
    """
    N-Side Filling surface construction.
    
    This algorithm builds a face from:
    - A set of edges defining the bounds of the face and some constraints
      the surface has to satisfy
    - A set of edges and points defining constraints the support surface
      has to satisfy
    - An optional initial surface to deform for satisfying the constraints
    - A set of parameters to control the constraints
    
    The support surface is computed by deformation of the initial surface
    (or an automatically computed one) to satisfy the given constraints.
    
    Constraint orders:
    - GeomAbs_C0: Surface passes through the 3D representation
    - GeomAbs_G1: Surface passes through and respects tangency
    - GeomAbs_G2: Surface passes through and respects tangency and curvature
    
    Typical usage:
      filling = BRepOffsetAPI.MakeFilling()
      filling.add(edge1, GeomAbs.C0)  # Boundary constraint
      filling.add(edge2, face, GeomAbs.G1)  # Tangent to face
      filling.add(point)  # Point constraint
      filling.build()
      result = filling.shape()
    """
    def __init__(self, degree: typing.SupportsInt = 3, nb_pts_on_cur: typing.SupportsInt = 15, nb_iter: typing.SupportsInt = 2, anisotropie: bool = False, tol_2d: typing.SupportsFloat = 1e-05, tol_3d: typing.SupportsFloat = 0.0001, tol_ang: typing.SupportsFloat = 0.01, tol_curv: typing.SupportsFloat = 0.1, max_deg: typing.SupportsInt = 8, max_segments: typing.SupportsInt = 9) -> None:
        """
        Constructs a wire filling object.
        
        Parameters:
          degree: Order of energy criterion to minimize (default: 3)
                  Recommended: i+2 where i is max order of constraints
          nb_pts_on_cur: Average number of points for edge discretization (default: 15)
          nb_iter: Maximum iterations, discretization points increase each iteration (default: 2)
          anisotropie: Better performance when U/V length ratio is very different (default: False)
          tol_2d: 2D tolerance (default: 1e-5)
          tol_3d: Max distance between surface and constraints (default: 1e-4)
          tol_ang: Max angle between surface normal and constraints (default: 0.01)
          tol_curv: Max curvature difference between surface and constraints (default: 0.1)
          max_deg: Highest polynomial degree for filling surface (default: 8)
          max_segments: Greatest number of segments (default: 9)
        """
    @typing.overload
    def add(self, edge: mod3d.TopoDS.Edge, order: mod3d.GeomAbs.Shape, is_bound: bool = True) -> int:
        """
        Adds an edge constraint which also defines a boundary edge.
        
        Parameters:
          edge: The constraint edge
          order: Continuity order (C0, G1, or G2)
          is_bound: True if edge is a boundary of the face (default: True)
        
        Returns the index of the constraint.
        
        Raises ConstructionError if order is G1/G2 and edge has no face representation
        """
    @typing.overload
    def add(self, edge: mod3d.TopoDS.Edge, support: mod3d.TopoDS.Face, order: mod3d.GeomAbs.Shape, is_bound: bool = True) -> int:
        """
        Adds an edge constraint with a support face for tangency/curvature.
        
        Parameters:
          edge: The constraint edge
          support: Face to respect tangency/curvature with
          order: Continuity order (C0, G1, or G2)
          is_bound: True if edge is a boundary of the face (default: True)
        
        Returns the index of the constraint.
        
        Raises ConstructionError if edge has no 2D representation on the support face
        """
    @typing.overload
    def add(self, support: mod3d.TopoDS.Face, order: mod3d.GeomAbs.Shape) -> int:
        """
        Adds a free constraint on a face.
        
        The corresponding edge is automatically computed. It is always a bound.
        
        Parameters:
          support: The support face
          order: Continuity order
        
        Returns the index of the constraint
        """
    @typing.overload
    def add(self, point: mod3d.gp.Pnt) -> int:
        """
        Adds a punctual constraint.
        
        The surface will pass through this point.
        
        Returns the index of the constraint
        """
    @typing.overload
    def add(self, u: typing.SupportsFloat, v: typing.SupportsFloat, support: mod3d.TopoDS.Face, order: mod3d.GeomAbs.Shape) -> int:
        """
        Adds a punctual constraint at parametric coordinates on a face.
        
        Parameters:
          u, v: Parametric coordinates on the support face
          support: The support face
          order: Continuity order at this point
        
        Returns the index of the constraint
        """
    def build(self) -> None:
        """
        Builds the resulting filling face.
        
        Computes the surface that satisfies all constraints and builds the face.
        Use is_done() to check success and shape() to retrieve the result
        """
    def g0_error_at(self, index: typing.SupportsInt) -> float:
        """
        Returns maximum distance for constraint at given index
        """
    def g1_error_at(self, index: typing.SupportsInt) -> float:
        """
        Returns maximum angle for constraint at given index
        """
    def g2_error_at(self, index: typing.SupportsInt) -> float:
        """
        Returns maximum curvature difference for constraint at given index
        """
    def generated(self, shape: mod3d.TopoDS.Shape) -> ...:
        """
        Returns the list of shapes generated from the given shape
        """
    def load_init_surface(self, surf: mod3d.TopoDS.Face) -> None:
        """
        Loads an initial surface to begin construction.
        
        This is useful if the resulting surface is likely to be complex.
        The support surface is computed by deforming this surface to
        satisfy the given constraints.
        
        Important: The initial surface must have orthogonal local coordinates,
        i.e., partial derivatives dS/du and dS/dv must be orthogonal at each point.
        If this condition is not met, distortions may occur.
        
        If no initial surface is given, the algorithm computes one automatically.
        """
    def set_approx_param(self, max_deg: typing.SupportsInt = 8, max_segments: typing.SupportsInt = 9) -> None:
        """
        Sets parameters used to approximate the filling surface.
        
        Parameters:
          max_deg: Highest polynomial degree for the surface
          max_segments: Greatest number of segments
        """
    def set_constr_param(self, tol_2d: typing.SupportsFloat = 1e-05, tol_3d: typing.SupportsFloat = 0.0001, tol_ang: typing.SupportsFloat = 0.01, tol_curv: typing.SupportsFloat = 0.1) -> None:
        """
        Sets tolerance values used to control constraints.
        
        Parameters:
          tol_2d: 2D tolerance
          tol_3d: Max distance between surface and constraints
          tol_ang: Max angle between surface normal and constraints
          tol_curv: Max curvature difference between surface and constraints
        """
    def set_resol_param(self, degree: typing.SupportsInt = 3, nb_pts_on_cur: typing.SupportsInt = 15, nb_iter: typing.SupportsInt = 2, anisotropie: bool = False) -> None:
        """
        Sets parameters used for resolution.
        
        Parameters:
          degree: Order of energy criterion to minimize
          nb_pts_on_cur: Average number of points for edge discretization
          nb_iter: Maximum number of iterations
          anisotropie: Better performance for surfaces with extreme U/V ratio
        """
    @property
    def g0_error(self) -> float:
        """
        Maximum distance between the result and the constraints
        """
    @property
    def g1_error(self) -> float:
        """
        Maximum angle (radians) between result normal and constraints
        """
    @property
    def g2_error(self) -> float:
        """
        Maximum curvature difference between result and constraints
        """
class MakeOffset(mod3d.BRepBuilderAPI.MakeShape):
    """
    
            Creates 2D offset curves from wires on a planar face.
    
            This algorithm offsets wires contained in a planar face by a specified
            distance, creating parallel curves. It handles corners using different
            join types and can produce open or closed results.
    
            The algorithm works with wires lying on a plane. For 3D solid offsets,
            use BRepOffsetAPI_MakeOffsetShape instead.
    
            Join types at salient vertices:
            - GeomAbs_Arc: Corners are filleted with circular arcs (default)
            - GeomAbs_Tangent: Corners are extended tangentially
            - GeomAbs_Intersection: Edges are extended until they intersect
    
            Typical usage:
              # From a planar face containing wires
              offset = BRepOffsetAPI.MakeOffset(face, GeomAbs.JoinType.Arc)
              offset.perform(5.0)  # Offset by 5 units
              result = offset.shape()
    
              # From a single wire
              offset = BRepOffsetAPI.MakeOffset(wire, GeomAbs.JoinType.Arc)
              offset.perform(-2.0)  # Negative offset (inward)
              result = offset.shape()
    
              # Multiple wires
              offset = BRepOffsetAPI.MakeOffset()
              offset.init(GeomAbs.JoinType.Arc)
              offset.add_wire(wire1)
              offset.add_wire(wire2)
              offset.perform(3.0)
              result = offset.shape()
            
    """
    @staticmethod
    def convert_face(face: mod3d.TopoDS.Face, angle_tolerance: typing.SupportsFloat) -> mod3d.TopoDS.Face:
        """
                    Converts a face's wires to arcs and segments only.
        
                    This utility method converts each wire of the face into contours
                    consisting only of circular arcs and linear segments. New 3D curves
                    are built as well. This can be useful for preprocessing complex
                    faces before offsetting.
        
                    Parameters:
                      face: The face to convert
                      angle_tolerance: Angular tolerance for the conversion
        
                    Returns:
                      A new face with simplified wire geometry
        """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty offset algorithm.
        
        Use init() to initialize and add_wire() to add wires before calling perform().
        """
    @typing.overload
    def __init__(self, spine: mod3d.TopoDS.Face, join: mod3d.GeomAbs.JoinType = ..., is_open_result: bool = False) -> None:
        """
                    Creates an offset algorithm from a planar face.
        
                    All wires contained in the face will be offset.
        
                    Parameters:
                      spine: Planar face containing the wires to offset
                      join: Corner treatment type (default: Arc)
                        - Arc: Filleted corners with circular arcs
                        - Tangent: Extended tangentially
                        - Intersection: Extended until intersection
                      is_open_result: If True, allows open wire results (default: False)
        """
    @typing.overload
    def __init__(self, spine: mod3d.TopoDS.Wire, join: mod3d.GeomAbs.JoinType = ..., is_open_result: bool = False) -> None:
        """
                    Creates an offset algorithm from a single wire.
        
                    The wire should be planar.
        
                    Parameters:
                      spine: Wire to offset (should be planar)
                      join: Corner treatment type (default: Arc)
                      is_open_result: If True, allows open wire results (default: False)
        """
    def add_wire(self, spine: mod3d.TopoDS.Wire) -> None:
        """
                    Adds a wire to be offset.
        
                    Multiple wires can be added. All wires should lie in the same plane.
                    Call init() before adding wires if not using the face constructor.
        
                    Parameters:
                      spine: Wire to add for offsetting
        """
    def build(self) -> None:
        """
        Builds the resulting offset shape.
        
        This is called automatically by perform(), but can be called
        explicitly if needed. Use is_done() to check success.
        """
    def generated(self, shape: mod3d.TopoDS.Shape) -> list:
        """
                    Returns shapes generated from an input shape.
        
                    Parameters:
                      shape: A subshape of the original wire/face
        
                    Returns:
                      List of shapes created from the input shape
        """
    @typing.overload
    def init(self, spine: mod3d.TopoDS.Face, join: mod3d.GeomAbs.JoinType = ..., is_open_result: bool = False) -> None:
        """
                    Initializes the algorithm with a planar face.
        
                    Parameters:
                      spine: Planar face containing the wires to offset
                      join: Corner treatment type (default: Arc)
                      is_open_result: If True, allows open wire results (default: False)
        """
    @typing.overload
    def init(self, join: mod3d.GeomAbs.JoinType = ..., is_open_result: bool = False) -> None:
        """
                    Initializes the algorithm for manual wire addition.
        
                    Use add_wire() to add wires after calling this method.
        
                    Parameters:
                      join: Corner treatment type (default: Arc)
                      is_open_result: If True, allows open wire results (default: False)
        """
    def perform(self, offset: typing.SupportsFloat, alt: typing.SupportsFloat = 0.0) -> None:
        """
                    Computes the offset at the specified distance.
        
                    Positive offset values expand outward, negative values contract inward.
                    The alt parameter allows creating the offset at a different altitude
                    from the original plane.
        
                    Parameters:
                      offset: Distance to offset (positive=outward, negative=inward)
                      alt: Altitude from the plane of the spine (default: 0.0)
        
                    Raises:
                      StdFail_NotDone: If the offset cannot be computed
        """
    def set_approx(self, to_approx: bool) -> None:
        """
                    Sets the approximation flag.
        
                    When enabled, input contours are converted to curves consisting
                    only of 2D circular arcs and 2D linear segments. This can improve
                    performance and stability for complex input curves.
        
                    Parameters:
                      to_approx: True to enable approximation of input contours
        """
class MakeOffsetShape(mod3d.BRepBuilderAPI.MakeShape):
    """
    
            Creates 3D offset shapes (shells/solids) from a shape.
    
            This algorithm builds a shell or solid parallel to the source shape
            at a specified offset distance. It handles faces, shells, solids, and
            compounds of these shapes.
    
            The offset shape is constructed:
            - Outside the shape if offset is positive
            - Inside the shape if offset is negative
    
            Two algorithms are available:
            - PerformByJoin: Full algorithm with intersection computation (default)
            - PerformBySimple: Simple algorithm without intersection computation
    
            Join types at edges:
            - GeomAbs_Arc: Pipes between edges, spheres at vertices (default)
            - GeomAbs_Intersection: Adjacent faces enlarged and intersected
    
            Requirements:
            - All faces must have C1 continuous underlying surfaces
            - Offset value should be small enough to avoid self-intersections
            - Vertices with more than 3 converging edges may cause failures
            - BSpline surfaces must have continuity > C0
    
            Typical usage:
              # Simple offset
              offset = BRepOffsetAPI.MakeOffsetShape()
              offset.perform_by_join(solid, 2.0, 1e-3)
              result = offset.shape()
    
              # With options
              offset = BRepOffsetAPI.MakeOffsetShape()
              offset.perform_by_join(solid, -1.0, 1e-3,
                                     mode=BRepOffsetAPI.OffsetMode.Skin,
                                     join=GeomAbs.JoinType.Arc)
              result = offset.shape()
            
    """
    def __init__(self) -> None:
        """
        Creates an empty offset shape algorithm.
        
        Use perform_by_join() or perform_by_simple() to compute the offset.
        """
    def perform_by_join(self, shape: mod3d.TopoDS.Shape, offset: typing.SupportsFloat, tol: typing.SupportsFloat, mode: OffsetMode = ..., intersection: bool = False, self_inter: bool = False, join: mod3d.GeomAbs.JoinType = ..., remove_internal_edges: bool = False) -> None:
        """
                    Computes offset using the full algorithm with intersection computation.
        
                    This is the recommended method for most use cases. It handles
                    self-intersections and produces clean results.
        
                    Parameters:
                      shape: The shape to offset (face, shell, solid, or compound)
                      offset: Offset distance (positive=outward, negative=inward)
                      tol: Coincidence tolerance for generated shapes
                      mode: Construction mode (default: Skin)
                        - Skin: Offset along solid surface
                        - Pipe: Offset of a curve
                        - RectoVerso: Offset on both sides
                      intersection: If True, compute intersections with all parallels
                                    (more general but not fully implemented, default: False)
                      self_inter: If True, eliminate self-intersections
                                  (not yet implemented, use False)
                      join: How to fill holes between adjacent faces (default: Arc)
                        - Arc: Pipes between edges, spheres at vertices
                        - Intersection: Enlarge and intersect adjacent faces
                      remove_internal_edges: If True, remove internal edges from result
                                             (default: False)
        
                    Warnings:
                      - Faces must have C1 continuous underlying surfaces
                      - Offset should be small enough to avoid self-intersections
                      - Vertices with >3 converging edges may cause failures
        """
    def perform_by_simple(self, shape: mod3d.TopoDS.Shape, offset_value: typing.SupportsFloat) -> None:
        """
                    Computes offset using a simple algorithm without intersection computation.
        
                    This is faster but may produce self-intersecting results for complex
                    shapes or large offset values.
        
                    Parameters:
                      shape: The shape to offset (face, shell, solid, or compound)
                      offset_value: Offset distance (positive=outward, negative=inward)
        """
    @property
    def join_type(self) -> mod3d.GeomAbs.JoinType:
        """
        Returns the join type used for the offset computation.
        """
class MiddlePath(mod3d.BRepBuilderAPI.MakeShape):
    """
    
            Computes the middle path of a pipe-like shape.
    
            This algorithm extracts a wire representing the middle path (spine)
            of a pipe-shaped solid or shell. The pipe must have identifiable
            start and end sections.
    
            The algorithm works by:
            1. Identifying edges on the start and end sections
            2. Finding corresponding paths through the pipe
            3. Computing the middle of those paths
    
            Requirements:
            - The shape must be pipe-like (constant section along a path)
            - Start and end shapes must be faces or wires on the pipe boundaries
    
            Typical usage:
              # Extract middle path from a pipe
              pipe = make_pipe(...)  # A pipe-shaped solid
              start_face = ...  # Face at one end
              end_face = ...    # Face at the other end
              
              middle = BRepOffsetAPI.MiddlePath(pipe, start_face, end_face)
              middle.build()
              path_wire = middle.shape()
            
    """
    def __init__(self, shape: mod3d.TopoDS.Shape, start_shape: mod3d.TopoDS.Shape, end_shape: mod3d.TopoDS.Shape) -> None:
        """
                    Constructs a middle path algorithm.
        
                    Parameters:
                      shape: The pipe-like shape to analyze
                      start_shape: A wire or face at one end of the pipe
                      end_shape: A wire or face at the other end of the pipe
        """
    def build(self) -> None:
        """
        Computes the middle path. Check is_done() for success.
        """
class NormalProjection(mod3d.BRepBuilderAPI.MakeShape):
    """
    
            Projects edges/wires onto a shape along the normal direction.
    
            This algorithm projects edges or wires onto a target shape (typically
            a face or shell) by projecting each point along the normal to the
            target surface at that point.
    
            The projection produces new edges that lie on the target surface,
            with associated 2D curves (pcurves) on the surface.
    
            Features:
            - Can project multiple edges/wires in one operation
            - Results are built as oriented wires when possible
            - Optional 3D curve computation (vs. keeping original curves)
            - Configurable approximation parameters
    
            Typical usage:
              # Project a wire onto a surface
              proj = BRepOffsetAPI.NormalProjection(target_face)
              proj.add(wire_to_project)
              proj.build()
              projected = proj.shape()
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty normal projection algorithm.
        
        Use init() to set the target shape and add() to add shapes to project.
        """
    @typing.overload
    def __init__(self, target: mod3d.TopoDS.Shape) -> None:
        """
                    Creates a normal projection algorithm with a target shape.
        
                    Parameters:
                      target: The shape to project onto (typically a face or shell)
        
                    Default parameters:
                      Tol3D = 1e-4, Tol2D = sqrt(Tol3D)
                      InternalContinuity = C2
                      MaxDegree = 14, MaxSeg = 16
        """
    def add(self, to_project: mod3d.TopoDS.Shape) -> None:
        """
                    Adds a shape to be projected onto the target.
        
                    Parameters:
                      to_project: An edge or wire to project
        
                    Raises:
                      Standard_ConstructionError: If the shape cannot be added
        """
    def ancestor(self, edge: mod3d.TopoDS.Edge) -> mod3d.TopoDS.Shape:
        """
                    Returns the original edge corresponding to a projected edge.
        
                    Parameters:
                      edge: A projected edge from the result
        
                    Returns:
                      The original edge that was projected
        """
    def build(self) -> None:
        """
        Builds the projection result as a compound of wires.
        """
    def build_wire(self) -> tuple:
        """
                    Builds the result as a list of wires if possible.
        
                    Returns:
                      Tuple of (success, list_of_wires)
                      success is True only if there is exactly one wire
        """
    def compute_3d(self, with_3d: bool = True) -> None:
        """
                    Sets whether to compute new 3D curves.
        
                    Parameters:
                      with_3d: If True, compute new 3D curves;
                               If False, keep original curves for result edges
        """
    def couple(self, edge: mod3d.TopoDS.Edge) -> mod3d.TopoDS.Shape:
        """
                    Returns the target face corresponding to a projected edge.
        
                    Parameters:
                      edge: A projected edge from the result
        
                    Returns:
                      The face on which the edge was projected
        """
    def init(self, target: mod3d.TopoDS.Shape) -> None:
        """
        Initializes with a target shape to project onto.
        """
    def projection(self) -> mod3d.TopoDS.Shape:
        """
        Returns the projection result (same as shape()).
        """
    def set_limit(self, face_boundaries: bool = True) -> None:
        """
                    Controls limitation of projected edges to face boundaries.
        
                    Parameters:
                      face_boundaries: If True, limit projection to face boundaries
        """
    def set_max_distance(self, max_dist: typing.SupportsFloat) -> None:
        """
                    Sets maximum distance between target and source shape.
        
                    If the distance exceeds this value, that part of the projection
                    is discarded. Use negative value to disable this check.
        
                    Parameters:
                      max_dist: Maximum allowed distance (< 0 to disable)
        """
    def set_params(self, tol_3d: typing.SupportsFloat, tol_2d: typing.SupportsFloat, continuity: mod3d.GeomAbs.Shape, max_degree: typing.SupportsInt, max_seg: typing.SupportsInt) -> None:
        """
                    Sets approximation parameters.
        
                    Parameters:
                      tol_3d: Required tolerance between 3D curve and 2D representation
                      tol_2d: 2D tolerance
                      continuity: Order of continuity for approximation (C0, C1, C2, etc.)
                      max_degree: Maximum degree for BSpline approximation
                      max_seg: Maximum number of segments for BSpline approximation
        """
class OffsetMode:
    """
    
            Offset construction modes.
    
            Defines how the offset algorithm handles the shape:
            - Skin: Offset along the surface of a solid (default)
            - Pipe: Offset of a curve to obtain a pre-surface
            - RectoVerso: Offset along both sides of a surface shell
            
    
    Members:
    
      Skin : Offset along the surface of a solid to obtain a manifold space
    
      Pipe : Offset of a curve to obtain a pre-surface
    
      RectoVerso : Offset along both sides of a surface shell
    """
    Pipe: typing.ClassVar[OffsetMode]  # value = <OffsetMode.Pipe: 1>
    RectoVerso: typing.ClassVar[OffsetMode]  # value = <OffsetMode.RectoVerso: 2>
    Skin: typing.ClassVar[OffsetMode]  # value = <OffsetMode.Skin: 0>
    __members__: typing.ClassVar[dict[str, OffsetMode]]  # value = {'Skin': <OffsetMode.Skin: 0>, 'Pipe': <OffsetMode.Pipe: 1>, 'RectoVerso': <OffsetMode.RectoVerso: 2>}
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
class ParametrizationType:
    """
    Parametrization types for curve/surface approximation.
    
    Members:
    
      ChordLength : Parametrization by chord length
    
      Centripetal : Centripetal parametrization
    
      IsoParametric : Iso-parametric (uniform) parametrization
    """
    Centripetal: typing.ClassVar[ParametrizationType]  # value = <ParametrizationType.Centripetal: 1>
    ChordLength: typing.ClassVar[ParametrizationType]  # value = <ParametrizationType.ChordLength: 0>
    IsoParametric: typing.ClassVar[ParametrizationType]  # value = <ParametrizationType.IsoParametric: 2>
    __members__: typing.ClassVar[dict[str, ParametrizationType]]  # value = {'ChordLength': <ParametrizationType.ChordLength: 0>, 'Centripetal': <ParametrizationType.Centripetal: 1>, 'IsoParametric': <ParametrizationType.IsoParametric: 2>}
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
class ThruSectionStatus:
    """
    Status codes for ThruSections algorithm.
    
    Members:
    
      Done : Operation completed successfully
    
      NotDone : Operation not completed
    
      NotSameTopology : Profiles have different topology (all must be closed or all open)
    
      ProfilesInconsistent : Profiles are inconsistent
    
      WrongUsage : Wrong usage of punctual sections
    
      Null3DCurve : Null 3D curve in edge
    
      Failed : Algorithm has failed
    """
    Done: typing.ClassVar[ThruSectionStatus]  # value = <ThruSectionStatus.Done: 0>
    Failed: typing.ClassVar[ThruSectionStatus]  # value = <ThruSectionStatus.Failed: 6>
    NotDone: typing.ClassVar[ThruSectionStatus]  # value = <ThruSectionStatus.NotDone: 1>
    NotSameTopology: typing.ClassVar[ThruSectionStatus]  # value = <ThruSectionStatus.NotSameTopology: 2>
    Null3DCurve: typing.ClassVar[ThruSectionStatus]  # value = <ThruSectionStatus.Null3DCurve: 5>
    ProfilesInconsistent: typing.ClassVar[ThruSectionStatus]  # value = <ThruSectionStatus.ProfilesInconsistent: 3>
    WrongUsage: typing.ClassVar[ThruSectionStatus]  # value = <ThruSectionStatus.WrongUsage: 4>
    __members__: typing.ClassVar[dict[str, ThruSectionStatus]]  # value = {'Done': <ThruSectionStatus.Done: 0>, 'NotDone': <ThruSectionStatus.NotDone: 1>, 'NotSameTopology': <ThruSectionStatus.NotSameTopology: 2>, 'ProfilesInconsistent': <ThruSectionStatus.ProfilesInconsistent: 3>, 'WrongUsage': <ThruSectionStatus.WrongUsage: 4>, 'Null3DCurve': <ThruSectionStatus.Null3DCurve: 5>, 'Failed': <ThruSectionStatus.Failed: 6>}
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
class ThruSections(mod3d.BRepBuilderAPI.MakeShape):
    """
    
            Creates a shell or solid by lofting through a set of wire sections.
    
            This algorithm builds a shape by connecting a sequence of wire profiles.
            The wires are connected either by ruled surfaces (straight lines between
            corresponding points) or by smooth approximated surfaces.
    
            Features:
            - Can create shells (open) or solids (closed with end caps)
            - First and last sections can be vertices (points)
            - Optional smoothing with configurable continuity
            - Automatic wire compatibility checking and adjustment
    
            Requirements:
            - At least one wire section must be provided
            - Wires should have compatible orientations
            - Vertices can only be first or last sections
    
            Typical usage:
              # Create a lofted solid through three circles
              loft = BRepOffsetAPI.ThruSections(is_solid=True)
              loft.add_wire(circle1)
              loft.add_wire(circle2)
              loft.add_wire(circle3)
              loft.build()
              result = loft.shape()
    
              # Loft from point to circle
              loft = BRepOffsetAPI.ThruSections(is_solid=True)
              loft.add_vertex(apex_vertex)
              loft.add_wire(base_circle)
              loft.build()
              cone = loft.shape()
            
    """
    def __init__(self, is_solid: bool = False, ruled: bool = False, pres3d: typing.SupportsFloat = 1e-06) -> None:
        """
                    Creates a ThruSections algorithm.
        
                    Parameters:
                      is_solid: If True, create a solid with end caps;
                                If False, create an open shell (default)
                      ruled: If True, create ruled surfaces between sections;
                             If False, create smooth approximated surfaces (default)
                      pres3d: Precision for 3D approximation (default: 1e-6)
        """
    def add_vertex(self, vertex: mod3d.TopoDS.Vertex) -> None:
        """
                    Adds a vertex (point) section to the loft.
        
                    A vertex can only be added as the first or last section.
                    This creates a pointed end (like a cone apex).
        
                    Parameters:
                      vertex: A vertex representing a point section
        """
    def add_wire(self, wire: mod3d.TopoDS.Wire) -> None:
        """
                    Adds a wire section to the loft.
        
                    Wires are processed in the order they are added.
                    Use build() after adding all sections.
        
                    Parameters:
                      wire: A wire representing a cross-section
        """
    def build(self) -> None:
        """
        Builds the lofted shape. Check is_done() for success.
        """
    def check_compatibility(self, check: bool = True) -> None:
        """
                    Enables/disables wire compatibility checking.
        
                    When enabled, the algorithm adjusts wires to have the same
                    number of edges and consistent orientations to avoid twisted results.
        
                    Parameters:
                      check: If True, enable compatibility checking (default)
        """
    def criterium_weight(self) -> tuple:
        """
        Returns the criterion weights as a tuple (w1, w2, w3).
        """
    def first_shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the bottom face of the loft (if solid).
        """
    def generated_face(self, edge: mod3d.TopoDS.Shape) -> mod3d.TopoDS.Shape:
        """
                    Returns the face generated by an edge.
        
                    For ruled lofts: returns face generated by edge (except last wire).
                    For smoothed lofts: returns face generated by edge of first wire.
        
                    Parameters:
                      edge: An edge from one of the input wires
        
                    Returns:
                      The face generated by the edge
        """
    def init(self, is_solid: bool = False, ruled: bool = False, pres3d: typing.SupportsFloat = 1e-06) -> None:
        """
        Reinitializes the algorithm with new parameters.
        """
    def last_shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the top face of the loft (if solid).
        """
    def set_continuity(self, continuity: mod3d.GeomAbs.Shape) -> None:
        """
                    Sets the continuity for the approximation.
        
                    Parameters:
                      continuity: Geometric continuity (C0, C1, C2, etc.)
        """
    def set_criterium_weight(self, w1: typing.SupportsFloat, w2: typing.SupportsFloat, w3: typing.SupportsFloat) -> None:
        """
                    Sets weights for optimization criteria.
        
                    Each weight must be positive (> 0).
        
                    Parameters:
                      w1: Weight for first criterion
                      w2: Weight for second criterion
                      w3: Weight for third criterion
        """
    def set_max_degree(self, max_deg: typing.SupportsInt) -> None:
        """
        Sets the maximum U degree for the result surface.
        """
    def set_mutable_input(self, is_mutable: bool) -> None:
        """
                    Sets whether input profiles can be modified.
        
                    Parameters:
                      is_mutable: If True, input wires may be modified (default);
                                  If False, input wires are preserved
        """
    def set_par_type(self, par_type: ParametrizationType) -> None:
        """
                    Sets the parametrization type for approximation.
        
                    Parameters:
                      par_type: ChordLength, Centripetal, or IsoParametric
        """
    def set_smoothing(self, use_smoothing: bool) -> None:
        """
        Enables/disables smoothing in the approximation algorithm.
        """
    def wires(self) -> list:
        """
        Returns the list of original input wires.
        """
    @property
    def continuity(self) -> mod3d.GeomAbs.Shape:
        """
        Returns the continuity used in approximation.
        """
    @property
    def is_mutable_input(self) -> bool:
        """
        Returns True if input profiles can be modified.
        """
    @property
    def max_degree(self) -> int:
        """
        Returns the maximum U degree of result surface.
        """
    @property
    def par_type(self) -> ParametrizationType:
        """
        Returns the parametrization type used.
        """
    @property
    def status(self) -> ThruSectionStatus:
        """
        Returns the status of the thrusections operation.
        """
    @property
    def use_smoothing(self) -> bool:
        """
        Returns True if smoothing is enabled.
        """
Centripetal: ParametrizationType  # value = <ParametrizationType.Centripetal: 1>
ChordLength: ParametrizationType  # value = <ParametrizationType.ChordLength: 0>
Done: ThruSectionStatus  # value = <ThruSectionStatus.Done: 0>
Failed: ThruSectionStatus  # value = <ThruSectionStatus.Failed: 6>
IsoParametric: ParametrizationType  # value = <ParametrizationType.IsoParametric: 2>
NotDone: ThruSectionStatus  # value = <ThruSectionStatus.NotDone: 1>
NotSameTopology: ThruSectionStatus  # value = <ThruSectionStatus.NotSameTopology: 2>
Null3DCurve: ThruSectionStatus  # value = <ThruSectionStatus.Null3DCurve: 5>
Pipe: OffsetMode  # value = <OffsetMode.Pipe: 1>
ProfilesInconsistent: ThruSectionStatus  # value = <ThruSectionStatus.ProfilesInconsistent: 3>
RectoVerso: OffsetMode  # value = <OffsetMode.RectoVerso: 2>
Skin: OffsetMode  # value = <OffsetMode.Skin: 0>
WrongUsage: ThruSectionStatus  # value = <ThruSectionStatus.WrongUsage: 4>
