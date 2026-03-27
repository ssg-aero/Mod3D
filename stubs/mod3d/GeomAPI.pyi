"""
GeomAPI module
"""
from __future__ import annotations
import collections.abc
import mod3d.Geom
import mod3d.GeomAbs
import mod3d.gp
import typing
__all__: list[str] = ['ExtremaCurveCurve', 'ExtremaCurveCurve2d', 'ExtremaCurveSurface', 'ExtremaSurfaceSurface', 'InterCurveCurve2d', 'Interpolate', 'Interpolate2d', 'PointsToBSpline', 'PointsToBSpline2d', 'ProjectPointOnCurve', 'ProjectPointOnCurve2d', 'to_2d', 'to_3d']
class ExtremaCurveCurve:
    """
    
            Computes all extrema (minimum and maximum distances) between two 3D curves.
    
            An ExtremaCurveCurve algorithm minimizes or maximizes the distance between
            a point on the first curve and a point on the second curve. Thus, it computes
            start and end points of perpendiculars common to the two curves.
    
            Solutions consist of pairs of points, and an extremum is considered to be
            a segment joining the two points of a solution.
    
            Warning: In some cases, the nearest points between two curves do not
            correspond to one of the computed extrema. Instead, they may be given by:
            - A limit point of one curve and its orthogonal projection on the other
            - A limit point of both curves
            - An intersection point between the two curves
    
            Use TotalNearestPoints() to get the true nearest points accounting for
            curve endpoints.
    
            Example:
                c1 = Geom.Line(gp.Pnt(0,0,0), gp.Dir(1,0,0))
                c2 = Geom.Line(gp.Pnt(0,1,0), gp.Dir(0,0,1))
                extrema = GeomAPI.ExtremaCurveCurve(c1, c2)
                if extrema.nb_extrema > 0:
                    dist = extrema.lower_distance
                    p1, p2 = extrema.nearest_points()
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty algorithm. Use init() to define the curves.
        """
    @typing.overload
    def __init__(self, c1: mod3d.Geom.Curve, c2: mod3d.Geom.Curve) -> None:
        """
        Computes the extrema between curves C1 and C2.
        """
    @typing.overload
    def __init__(self, c1: mod3d.Geom.Curve, c2: mod3d.Geom.Curve, u1_min: typing.SupportsFloat, u1_max: typing.SupportsFloat, u2_min: typing.SupportsFloat, u2_max: typing.SupportsFloat) -> None:
        """
        Computes extrema between portions of the curves.
        
        Parameters:
          c1, c2: The two curves
          u1_min, u1_max: Parameter bounds for C1
          u2_min, u2_max: Parameter bounds for C2
        """
    def distance(self, index: typing.SupportsInt) -> float:
        """
        Returns the distance between the endpoints of extremum at index.
        
        Parameters:
          index: Index in range [1, nb_extrema]
        """
    @typing.overload
    def init(self, c1: mod3d.Geom.Curve, c2: mod3d.Geom.Curve) -> None:
        """
        Initializes and computes extrema between curves C1 and C2.
        """
    @typing.overload
    def init(self, c1: mod3d.Geom.Curve, c2: mod3d.Geom.Curve, u1_min: typing.SupportsFloat, u1_max: typing.SupportsFloat, u2_min: typing.SupportsFloat, u2_max: typing.SupportsFloat) -> None:
        """
        Initializes and computes extrema between portions of curves.
        """
    def parameters(self, index: typing.SupportsInt) -> tuple[float, float]:
        """
        Returns parameters U1 on first curve and U2 on second curve for extremum at index.
        
        Parameters:
          index: Index in range [1, nb_extrema]
        
        Returns:
          Tuple of (U1, U2)
        """
    def points(self, index: typing.SupportsInt) -> tuple[mod3d.gp.Pnt, mod3d.gp.Pnt]:
        """
        Returns points P1 on first curve and P2 on second curve for extremum at index.
        
        Parameters:
          index: Index in range [1, nb_extrema]
        
        Returns:
          Tuple of (P1, P2)
        """
    @property
    def is_parallel(self) -> bool:
        """
        Returns True if the two curves are parallel.
        """
    @property
    def lower_distance(self) -> float:
        """
        Returns the distance of the shortest extremum.
        
        Raises:
          StdFail_NotDone if algorithm fails
        """
    @property
    def lower_distance_parameters(self) -> tuple[float, float]:
        """
        Returns parameters U1 and U2 of the shortest extremum endpoints.
        
        Returns:
          Tuple of (U1, U2)
        
        Raises:
          StdFail_NotDone if algorithm fails
        """
    @property
    def nb_extrema(self) -> int:
        """
        Returns the number of extrema computed. Returns 0 if algorithm fails.
        """
    @property
    def nearest_points(self) -> tuple[mod3d.gp.Pnt, mod3d.gp.Pnt]:
        """
        Returns points P1 and P2 that are the ends of the shortest extremum.
        
        Returns:
          Tuple of (P1, P2)
        
        Raises:
          StdFail_NotDone if algorithm fails
        """
    @property
    def result(self) -> tuple:
        """
        Returns a complete result of the nearest points computation.
        """
    @property
    def total_lower_distance(self) -> float:
        """
        Returns the distance of the true nearest points.
        
        This accounts for curve endpoints and may be smaller than lower_distance.
        """
    @property
    def total_lower_distance_parameters(self) -> tuple:
        """
        Returns parameters of the true nearest points.
        
        Returns:
          Tuple of (found, U1, U2) where found is True if successful
        """
    @property
    def total_nearest_points(self) -> tuple:
        """
        Returns the true nearest points considering curve endpoints.
        
        This method accounts for limit points of the curves, which may be
        closer than any computed extremum.
        
        Returns:
          Tuple of (found, P1, P2) where found is True if successful
        """
class ExtremaCurveCurve2d:
    """
    
            Computes all extrema between two 2D curves.
    
            An ExtremaCurveCurve2d algorithm minimizes or maximizes the distance
            between a point on the first curve and a point on the second curve.
    
            Note: Unlike the 3D version, this class requires parameter bounds.
    
            Example:
                c1 = Geom2d.Line(gp.Pnt2d(0,0), gp.Dir2d(1,0))
                c2 = Geom2d.Line(gp.Pnt2d(0,5), gp.Dir2d(1,0))
                extrema = GeomAPI.ExtremaCurveCurve2d(c1, c2, 0, 10, 0, 10)
                dist = extrema.lower_distance  # Should be 5
            
    """
    def __init__(self, c1: Geom2d_Curve, c2: Geom2d_Curve, u1_min: typing.SupportsFloat, u1_max: typing.SupportsFloat, u2_min: typing.SupportsFloat, u2_max: typing.SupportsFloat) -> None:
        """
        Computes extrema between portions of curves C1 and C2.
        
        Parameters:
          c1, c2: The two 2D curves
          u1_min, u1_max: Parameter bounds for C1
          u2_min, u2_max: Parameter bounds for C2
        """
    def distance(self, index: typing.SupportsInt) -> float:
        """
        Returns the distance between endpoints of extremum at index.
        """
    def nearest_points(self) -> tuple[mod3d.gp.Pnt2d, mod3d.gp.Pnt2d]:
        """
        Returns points P1 and P2 of the shortest extremum.
        
        Returns:
          Tuple of (P1, P2)
        """
    def parameters(self, index: typing.SupportsInt) -> tuple[float, float]:
        """
        Returns parameters U1 on C1 and U2 on C2 for extremum at index.
        
        Returns:
          Tuple of (U1, U2)
        """
    def points(self, index: typing.SupportsInt) -> tuple[mod3d.gp.Pnt2d, mod3d.gp.Pnt2d]:
        """
        Returns points P1 on first curve and P2 on second curve.
        
        Parameters:
          index: Index in range [1, nb_extrema]
        
        Returns:
          Tuple of (P1, P2)
        """
    @property
    def is_parallel(self) -> bool:
        """
        Returns True if the two curves are parallel.
        """
    @property
    def lower_distance(self) -> float:
        """
        Returns the distance of the shortest extremum.
        """
    @property
    def lower_distance_parameters(self) -> tuple[float, float]:
        """
        Returns parameters U1 and U2 of the shortest extremum endpoints.
        
        Returns:
          Tuple of (U1, U2)
        """
    @property
    def nb_extrema(self) -> int:
        """
        Returns the number of extrema computed. Returns 0 if algorithm fails.
        """
    @property
    def result(self) -> tuple:
        """
        Returns a complete result of the nearest points computation.
        """
class ExtremaCurveSurface:
    """
    
            Computes all extrema between a 3D curve and a surface.
    
            An ExtremaCurveSurface algorithm minimizes or maximizes the distance
            between a point on the curve and a point on the surface. Thus, it
            computes start and end points of perpendiculars common to the curve
            and the surface.
    
            Solutions consist of pairs of points, and an extremum is considered
            to be a segment joining the two points of a solution.
    
            Warning: In some cases, the nearest points may be given by:
            - A point of a bounding curve of the surface and its projection
            - A limit point of the curve and its projection on the surface
            - An intersection point between the curve and the surface
    
            Example:
                curve = Geom.Line(gp.Pnt(0,0,5), gp.Dir(1,0,0))
                surface = Geom.Plane(gp.Ax3())
                extrema = GeomAPI.ExtremaCurveSurface(curve, surface)
                dist = extrema.lower_distance  # Should be 5
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty algorithm. Use init() to define curve and surface.
        """
    @typing.overload
    def __init__(self, curve: mod3d.Geom.Curve, surface: mod3d.Geom.Surface) -> None:
        """
        Computes extrema between a curve and a surface.
        """
    @typing.overload
    def __init__(self, curve: mod3d.Geom.Curve, surface: mod3d.Geom.Surface, w_min: typing.SupportsFloat, w_max: typing.SupportsFloat, u_min: typing.SupportsFloat, u_max: typing.SupportsFloat, v_min: typing.SupportsFloat, v_max: typing.SupportsFloat) -> None:
        """
        Computes extrema on portions of curve and surface.
        
        Parameters:
          curve, surface: The curve and surface
          w_min, w_max: Parameter bounds for the curve
          u_min, u_max, v_min, v_max: Parameter bounds for the surface
        """
    def distance(self, index: typing.SupportsInt) -> float:
        """
        Returns the distance between endpoints of extremum at index.
        """
    @typing.overload
    def init(self, curve: mod3d.Geom.Curve, surface: mod3d.Geom.Surface) -> None:
        """
        Initializes and computes extrema between curve and surface.
        """
    @typing.overload
    def init(self, curve: mod3d.Geom.Curve, surface: mod3d.Geom.Surface, w_min: typing.SupportsFloat, w_max: typing.SupportsFloat, u_min: typing.SupportsFloat, u_max: typing.SupportsFloat, v_min: typing.SupportsFloat, v_max: typing.SupportsFloat) -> None:
        """
        Initializes and computes extrema on portions of curve and surface.
        """
    def parameters(self, index: typing.SupportsInt) -> tuple[float, float, float]:
        """
        Returns parameters W on curve and (U,V) on surface for extremum at index.
        
        Parameters:
          index: Index in range [1, nb_extrema]
        
        Returns:
          Tuple of (W, U, V)
        """
    def points(self, index: typing.SupportsInt) -> tuple[mod3d.gp.Pnt, mod3d.gp.Pnt]:
        """
        Returns points P1 on the curve and P2 on the surface for extremum at index.
        
        Parameters:
          index: Index in range [1, nb_extrema]
        
        Returns:
          Tuple of (P1, P2)
        """
    @property
    def is_parallel(self) -> bool:
        """
        Returns True if the curve lies on a surface parallel to the given surface.
        """
    @property
    def lower_distance(self) -> float:
        """
        Returns the distance of the shortest extremum.
        """
    @property
    def lower_distance_parameters(self) -> tuple[float, float, float]:
        """
        Returns parameters of the shortest extremum endpoints.
        
        Returns:
          Tuple of (W, U, V) - W on curve, (U,V) on surface
        """
    @property
    def nb_extrema(self) -> int:
        """
        Returns the number of extrema computed. Returns 0 if algorithm fails.
        """
    @property
    def nearest_points(self) -> tuple[mod3d.gp.Pnt, mod3d.gp.Pnt]:
        """
        Returns points PC on curve and PS on surface of the shortest extremum.
        
        Returns:
          Tuple of (PC, PS)
        """
    @property
    def result(self) -> tuple:
        """
        Returns a complete result of the nearest points computation.
        """
class ExtremaSurfaceSurface:
    """
    
            Computes all extrema between two surfaces.
    
            An ExtremaSurfaceSurface algorithm minimizes or maximizes the distance
            between a point on the first surface and a point on the second surface.
            Results are start and end points of perpendiculars common to the two surfaces.
    
            Warning: In some cases, the nearest points may be given by:
            - A point of a bounding curve and its projection
            - Any point on intersection curves between the two surfaces
    
            Example:
                s1 = Geom.Plane(gp.Ax3(gp.Pnt(0,0,0), gp.Dir(0,0,1)))
                s2 = Geom.Plane(gp.Ax3(gp.Pnt(0,0,10), gp.Dir(0,0,1)))
                extrema = GeomAPI.ExtremaSurfaceSurface(s1, s2)
                dist = extrema.lower_distance  # Should be 10
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty algorithm. Use init() to define the surfaces.
        """
    @typing.overload
    def __init__(self, s1: mod3d.Geom.Surface, s2: mod3d.Geom.Surface) -> None:
        """
        Computes extrema between surfaces S1 and S2.
        """
    @typing.overload
    def __init__(self, s1: mod3d.Geom.Surface, s2: mod3d.Geom.Surface, u1_min: typing.SupportsFloat, u1_max: typing.SupportsFloat, v1_min: typing.SupportsFloat, v1_max: typing.SupportsFloat, u2_min: typing.SupportsFloat, u2_max: typing.SupportsFloat, v2_min: typing.SupportsFloat, v2_max: typing.SupportsFloat) -> None:
        """
        Computes extrema on portions of the surfaces.
        
        Parameters:
          s1, s2: The two surfaces
          u1_min, u1_max, v1_min, v1_max: Parameter bounds for S1
          u2_min, u2_max, v2_min, v2_max: Parameter bounds for S2
        """
    def distance(self, index: typing.SupportsInt) -> float:
        """
        Returns the distance between endpoints of extremum at index.
        """
    @typing.overload
    def init(self, s1: mod3d.Geom.Surface, s2: mod3d.Geom.Surface) -> None:
        """
        Initializes and computes extrema between surfaces S1 and S2.
        """
    @typing.overload
    def init(self, s1: mod3d.Geom.Surface, s2: mod3d.Geom.Surface, u1_min: typing.SupportsFloat, u1_max: typing.SupportsFloat, v1_min: typing.SupportsFloat, v1_max: typing.SupportsFloat, u2_min: typing.SupportsFloat, u2_max: typing.SupportsFloat, v2_min: typing.SupportsFloat, v2_max: typing.SupportsFloat) -> None:
        """
        Initializes and computes extrema on portions of surfaces.
        """
    def parameters(self, index: typing.SupportsInt) -> tuple[float, float, float, float]:
        """
        Returns parameters (U1,V1) on S1 and (U2,V2) on S2 for extremum at index.
        
        Returns:
          Tuple of (U1, V1, U2, V2)
        """
    def points(self, index: typing.SupportsInt) -> tuple[mod3d.gp.Pnt, mod3d.gp.Pnt]:
        """
        Returns points P1 on first surface and P2 on second surface.
        
        Parameters:
          index: Index in range [1, nb_extrema]
        
        Returns:
          Tuple of (P1, P2)
        """
    @property
    def is_parallel(self) -> bool:
        """
        Returns True if the two surfaces are parallel.
        """
    @property
    def lower_distance(self) -> float:
        """
        Returns the distance of the shortest extremum.
        """
    @property
    def lower_distance_parameters(self) -> tuple[float, float, float, float]:
        """
        Returns parameters of the shortest extremum endpoints.
        
        Returns:
          Tuple of (U1, V1, U2, V2)
        """
    @property
    def nb_extrema(self) -> int:
        """
        Returns the number of extrema computed. Returns 0 if algorithm fails.
        """
    @property
    def nearest_points(self) -> tuple[mod3d.gp.Pnt, mod3d.gp.Pnt]:
        """
        Returns points P1 and P2 of the shortest extremum.
        
        Returns:
          Tuple of (P1, P2)
        """
    @property
    def result(self) -> tuple:
        """
        Returns a complete result of the nearest points computation.
        """
class InterCurveCurve2d:
    """
    
            Computes intersections between two 2D curves, or self-intersections of a curve.
    
            This class finds:
            - Intersection points in case of cross intersections
            - Intersection segments in case of tangential intersections
            - Nothing if no intersections exist
    
            Example:
                c1 = Geom2d.Line(gp.Pnt2d(0,0), gp.Dir2d(1,1))
                c2 = Geom2d.Line(gp.Pnt2d(10,0), gp.Dir2d(-1,1))
                inter = GeomAPI.InterCurveCurve2d(c1, c2)
                if inter.nb_points > 0:
                    pt = inter.point(1)  # First intersection point
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty intersector. Use init() to define curves.
        """
    @typing.overload
    def __init__(self, c1: Geom2d_Curve, c2: Geom2d_Curve, tol: typing.SupportsFloat = 1e-06) -> None:
        """
        Creates an intersector and computes intersections between C1 and C2.
        
        Parameters:
          c1, c2: The two curves
          tol: Tolerance for intersection computation
        """
    @typing.overload
    def __init__(self, c1: Geom2d_Curve, tol: typing.SupportsFloat = 1e-06) -> None:
        """
        Creates an intersector and computes self-intersections of C1.
        
        Parameters:
          c1: The curve to check for self-intersections
          tol: Tolerance for intersection computation
        """
    @typing.overload
    def init(self, c1: Geom2d_Curve, c2: Geom2d_Curve, tol: typing.SupportsFloat = 1e-06) -> None:
        """
        Initializes and computes intersections between C1 and C2.
        """
    @typing.overload
    def init(self, c1: Geom2d_Curve, tol: typing.SupportsFloat = 1e-06) -> None:
        """
        Initializes and computes self-intersections of C1.
        """
    def point(self, index: typing.SupportsInt) -> mod3d.gp.Pnt2d:
        """
        Returns intersection point at index.
        
        Parameters:
          index: Index in range [1, nb_points]
        """
    def segment(self, index: typing.SupportsInt) -> tuple[Geom2d_Curve, Geom2d_Curve]:
        """
        Returns intersection segment curves for tangential intersections.
        
        Parameters:
          index: Index in range [1, nb_segments]
        
        Returns:
          Tuple of (segment_on_c1, segment_on_c2)
        """
    @property
    def nb_points(self) -> int:
        """
        Returns the number of intersection points (cross intersections).
        """
    @property
    def nb_segments(self) -> int:
        """
        Returns the number of tangential intersection segments.
        """
class Interpolate:
    """
    
            Interpolates a BSpline curve passing through an array of 3D points.
    
            The resulting curve will have:
            - C2 continuity if no tangency is requested
            - C1 continuity at points where tangency is specified
            - C1 continuity if periodic
    
            Example:
                points = [gp.Pnt(0,0,0), gp.Pnt(1,1,0), gp.Pnt(2,0,0)]
                interp = GeomAPI.Interpolate(points)
                interp.perform()
                if interp.is_done:
                    curve = interp.curve
            
    """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt], periodic: bool = False, tolerance: typing.SupportsFloat = 1e-06) -> None:
        """
        Creates interpolation from points list.
        periodic: if True, curve will be closed
        tolerance: used to check if points are too close
        """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt], parameters: collections.abc.Sequence[typing.SupportsFloat], periodic: bool = False, tolerance: typing.SupportsFloat = 1e-06) -> None:
        """
        Creates interpolation from points with explicit parameters.
        parameters: parameter values for each point
        """
    def load(self, initial_tangent: mod3d.gp.Vec, final_tangent: mod3d.gp.Vec, scale: bool = True) -> None:
        """
        Sets tangent constraints at first and last points
        """
    def load_tangents(self, tangents: collections.abc.Sequence[mod3d.gp.Vec], tangent_flags: collections.abc.Sequence[bool], scale: bool = True) -> None:
        """
        Sets tangent constraints at specified points.
        tangents: list of tangent vectors (parallel to points)
        tangent_flags: True where tangent should be applied
        """
    def perform(self) -> None:
        """
        Computes the interpolated BSpline curve
        """
    @property
    def curve(self) -> mod3d.Geom.BSplineCurve:
        """
        Returns the computed BSpline curve (raises if not done)
        """
    @property
    def is_done(self) -> bool:
        """
        True if interpolation was successful
        """
class Interpolate2d:
    """
    
            Interpolates a BSpline curve passing through an array of 2D points.
    
            The resulting curve will have:
            - C2 continuity if no tangency is requested
            - C1 continuity at points where tangency is specified
            - C1 continuity if periodic
    
            Example:
                points = [gp.Pnt2d(0,0), gp.Pnt2d(1,1), gp.Pnt2d(2,0)]
                interp = GeomAPI.Interpolate2d(points)
                interp.perform()
                if interp.is_done:
                    curve = interp.curve
            
    """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt2d], periodic: bool = False, tolerance: typing.SupportsFloat = 1e-06) -> None:
        """
        Creates interpolation from 2D points list.
        periodic: if True, curve will be closed
        tolerance: used to check if points are too close
        """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt2d], parameters: collections.abc.Sequence[typing.SupportsFloat], periodic: bool = False, tolerance: typing.SupportsFloat = 1e-06) -> None:
        """
        Creates interpolation from 2D points with explicit parameters
        """
    def load(self, initial_tangent: mod3d.gp.Vec2d, final_tangent: mod3d.gp.Vec2d, scale: bool = True) -> None:
        """
        Sets tangent constraints at first and last points
        """
    def load_tangents(self, tangents: collections.abc.Sequence[mod3d.gp.Vec2d], tangent_flags: collections.abc.Sequence[bool], scale: bool = True) -> None:
        """
        Sets tangent constraints at specified points
        """
    def perform(self) -> None:
        """
        Computes the interpolated BSpline curve
        """
    @property
    def curve(self) -> Geom2d_BSplineCurve:
        """
        Returns the computed 2D BSpline curve (raises if not done)
        """
    @property
    def is_done(self) -> bool:
        """
        True if interpolation was successful
        """
class PointsToBSpline:
    """
    
            Approximates a BSpline curve passing through an array of 3D points.
    
            Unlike Interpolate, this creates an approximation that may not pass
            exactly through each point, but minimizes the distance while maintaining
            specified continuity and degree constraints.
    
            Example:
                points = [gp.Pnt(0,0,0), gp.Pnt(1,1,0), gp.Pnt(2,0,0)]
                approx = GeomAPI.PointsToBSpline(points, deg_min=3, deg_max=8)
                if approx.is_done:
                    curve = approx.curve
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty approximation object. Use init() to define the curve.
        """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt], deg_min: typing.SupportsInt = 3, deg_max: typing.SupportsInt = 8, continuity: mod3d.GeomAbs.Shape = ..., tol3d: typing.SupportsFloat = 0.001) -> None:
        """
        Creates approximation from points with degree and continuity constraints
        """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt], par_type: Approx_ParametrizationType, deg_min: typing.SupportsInt = 3, deg_max: typing.SupportsInt = 8, continuity: mod3d.GeomAbs.Shape = ..., tol3d: typing.SupportsFloat = 0.001) -> None:
        """
        Creates approximation with specified parametrization type
        """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt], parameters: collections.abc.Sequence[typing.SupportsFloat], deg_min: typing.SupportsInt = 3, deg_max: typing.SupportsInt = 8, continuity: mod3d.GeomAbs.Shape = ..., tol3d: typing.SupportsFloat = 0.001) -> None:
        """
        Creates approximation with explicit parameter values
        """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt], weight1: typing.SupportsFloat, weight2: typing.SupportsFloat, weight3: typing.SupportsFloat, deg_max: typing.SupportsInt = 8, continuity: mod3d.GeomAbs.Shape = ..., tol3d: typing.SupportsFloat = 0.001) -> None:
        """
        Creates approximation using variational smoothing.
        Minimizes: weight1*Length + weight2*Curvature + weight3*Torsion
        """
    def init(self, points: collections.abc.Sequence[mod3d.gp.Pnt], deg_min: typing.SupportsInt = 3, deg_max: typing.SupportsInt = 8, continuity: mod3d.GeomAbs.Shape = ..., tol3d: typing.SupportsFloat = 0.001) -> None:
        """
        Initializes and computes approximation from points
        """
    @property
    def curve(self) -> mod3d.Geom.BSplineCurve:
        """
        Returns the computed BSpline curve (raises if not done)
        """
    @property
    def is_done(self) -> bool:
        """
        True if approximation was successful
        """
class PointsToBSpline2d:
    """
    
            Approximates a BSpline curve passing through an array of 2D points.
    
            Unlike Interpolate2d, this creates an approximation that may not pass
            exactly through each point, but minimizes the distance while maintaining
            specified continuity and degree constraints.
    
            Example:
                points = [gp.Pnt2d(0,0), gp.Pnt2d(1,1), gp.Pnt2d(2,0)]
                approx = GeomAPI.PointsToBSpline2d(points)
                if approx.is_done:
                    curve = approx.curve
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty approximation object. Use init() to define the curve.
        """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt2d], deg_min: typing.SupportsInt = 3, deg_max: typing.SupportsInt = 8, continuity: mod3d.GeomAbs.Shape = ..., tol2d: typing.SupportsFloat = 1e-06) -> None:
        """
        Creates approximation from 2D points with degree and continuity constraints
        """
    @typing.overload
    def __init__(self, y_values: collections.abc.Sequence[typing.SupportsFloat], x0: typing.SupportsFloat, dx: typing.SupportsFloat, deg_min: typing.SupportsInt = 3, deg_max: typing.SupportsInt = 8, continuity: mod3d.GeomAbs.Shape = ..., tol2d: typing.SupportsFloat = 1e-06) -> None:
        """
        Creates approximation from Y values at uniform X spacing.
        Points are: (x0 + i*dx, y_values[i])
        """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt2d], par_type: Approx_ParametrizationType, deg_min: typing.SupportsInt = 3, deg_max: typing.SupportsInt = 8, continuity: mod3d.GeomAbs.Shape = ..., tol2d: typing.SupportsFloat = 0.001) -> None:
        """
        Creates approximation with specified parametrization type
        """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt2d], parameters: collections.abc.Sequence[typing.SupportsFloat], deg_min: typing.SupportsInt = 3, deg_max: typing.SupportsInt = 8, continuity: mod3d.GeomAbs.Shape = ..., tol2d: typing.SupportsFloat = 0.001) -> None:
        """
        Creates approximation with explicit parameter values
        """
    @typing.overload
    def __init__(self, points: collections.abc.Sequence[mod3d.gp.Pnt2d], weight1: typing.SupportsFloat, weight2: typing.SupportsFloat, weight3: typing.SupportsFloat, deg_max: typing.SupportsInt = 8, continuity: mod3d.GeomAbs.Shape = ..., tol2d: typing.SupportsFloat = 0.001) -> None:
        """
        Creates approximation using variational smoothing.
        Minimizes: weight1*Length + weight2*Curvature + weight3*Torsion
        """
    def init(self, points: collections.abc.Sequence[mod3d.gp.Pnt2d], deg_min: typing.SupportsInt = 3, deg_max: typing.SupportsInt = 8, continuity: mod3d.GeomAbs.Shape = ..., tol2d: typing.SupportsFloat = 1e-06) -> None:
        """
        Initializes and computes approximation from 2D points
        """
    @property
    def curve(self) -> Geom2d_BSplineCurve:
        """
        Returns the computed 2D BSpline curve (raises if not done)
        """
    @property
    def is_done(self) -> bool:
        """
        True if approximation was successful
        """
class ProjectPointOnCurve:
    """
    
            Computes all orthogonal projections of a 3D point onto a 3D curve.
    
            This class finds all points on the curve where the line from the
            given point to the curve is perpendicular to the curve tangent.
    
            Example:
                curve = Geom.Circle(gp.Ax2(), 10.0)
                proj = GeomAPI.ProjectPointOnCurve(gp.Pnt(15, 0, 0), curve)
                nearest = proj.nearest_point  # Point on circle closest to (15,0,0)
                dist = proj.lower_distance    # Should be 5 (15 - radius 10)
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty projector. Use init() to define point and curve.
        """
    @typing.overload
    def __init__(self, point: mod3d.gp.Pnt, curve: mod3d.Geom.Curve) -> None:
        """
        Creates a projection of point P onto curve.
        """
    @typing.overload
    def __init__(self, point: mod3d.gp.Pnt, curve: mod3d.Geom.Curve, u_min: typing.SupportsFloat, u_max: typing.SupportsFloat) -> None:
        """
        Creates a projection onto a portion of the curve.
        
        Parameters:
          point: The point to project
          curve: The curve to project onto
          u_min, u_max: Parameter bounds
        """
    def distance(self, index: typing.SupportsInt) -> float:
        """
        Returns the distance from the point to projection at index.
        """
    @typing.overload
    def init(self, point: mod3d.gp.Pnt, curve: mod3d.Geom.Curve) -> None:
        """
        Initializes projection of point onto curve.
        """
    @typing.overload
    def init(self, point: mod3d.gp.Pnt, curve: mod3d.Geom.Curve, u_min: typing.SupportsFloat, u_max: typing.SupportsFloat) -> None:
        """
        Initializes projection onto a portion of the curve.
        """
    def init_curve(self, curve: mod3d.Geom.Curve, u_min: typing.SupportsFloat, u_max: typing.SupportsFloat) -> None:
        """
        Initializes the curve only. Use perform() to project points.
        """
    def parameter(self, index: typing.SupportsInt) -> float:
        """
        Returns the parameter on the curve of projection at index.
        
        Parameters:
          index: Index in range [1, nb_points]
        """
    def perform(self, point: mod3d.gp.Pnt) -> None:
        """
        Projects a point onto the current curve.
        """
    def point(self, index: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Returns the orthogonal projection point at index.
        
        Parameters:
          index: Index in range [1, nb_points]
        """
    @property
    def lower_distance(self) -> float:
        """
        Returns the distance to the nearest projection.
        """
    @property
    def lower_distance_parameter(self) -> float:
        """
        Returns the parameter on curve of the nearest projection.
        """
    @property
    def nb_points(self) -> int:
        """
        Returns the number of computed projection points. Returns 0 if fails.
        """
    @property
    def nearest_point(self) -> mod3d.gp.Pnt:
        """
        Returns the nearest orthogonal projection of the point on the curve.
        """
    @property
    def result(self) -> tuple:
        """
        Returns a complete result of the nearest point projection.
        
        Returns:
          Tuple of (P, U, distance) where P is the nearest point on curve,
          U is its parameter, and distance is the distance to the original point.
        """
class ProjectPointOnCurve2d:
    """
    
            Computes all orthogonal projections of a 2D point onto a 2D curve.
    
            This class finds all points on the curve where the line from the
            given point to the curve is perpendicular to the curve tangent.
    
            Example:
                curve = Geom2d.Circle(gp.Ax2d(), 10.0)
                proj = GeomAPI.ProjectPointOnCurve2d(gp.Pnt2d(15, 0), curve)
                nearest = proj.nearest_point  # Point on circle closest to (15,0)
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty projector. Use init() to define point and curve.
        """
    @typing.overload
    def __init__(self, point: mod3d.gp.Pnt2d, curve: Geom2d_Curve) -> None:
        """
        Creates a projection of 2D point onto curve.
        """
    @typing.overload
    def __init__(self, point: mod3d.gp.Pnt2d, curve: Geom2d_Curve, u_min: typing.SupportsFloat, u_max: typing.SupportsFloat) -> None:
        """
        Creates a projection onto a portion of the curve.
        
        Parameters:
          point: The 2D point to project
          curve: The 2D curve to project onto
          u_min, u_max: Parameter bounds
        """
    def distance(self, index: typing.SupportsInt) -> float:
        """
        Returns the distance from the point to projection at index.
        """
    @typing.overload
    def init(self, point: mod3d.gp.Pnt2d, curve: Geom2d_Curve) -> None:
        """
        Initializes projection of point onto curve.
        """
    @typing.overload
    def init(self, point: mod3d.gp.Pnt2d, curve: Geom2d_Curve, u_min: typing.SupportsFloat, u_max: typing.SupportsFloat) -> None:
        """
        Initializes projection onto a portion of the curve.
        """
    def parameter(self, index: typing.SupportsInt) -> float:
        """
        Returns the parameter on the curve of projection at index.
        """
    def point(self, index: typing.SupportsInt) -> mod3d.gp.Pnt2d:
        """
        Returns the orthogonal projection point at index.
        
        Parameters:
          index: Index in range [1, nb_points]
        """
    @property
    def lower_distance(self) -> float:
        """
        Returns the distance to the nearest projection.
        """
    @property
    def lower_distance_parameter(self) -> float:
        """
        Returns the parameter on curve of the nearest projection.
        """
    @property
    def nb_points(self) -> int:
        """
        Returns the number of computed projection points. Returns 0 if fails.
        """
    @property
    def nearest_point(self) -> mod3d.gp.Pnt2d:
        """
        Returns the nearest orthogonal projection of the point on the curve.
        """
    @property
    def result(self) -> tuple:
        """
        Returns a complete result of the nearest point projection.
        
        Returns:
          Tuple of (P, U, distance) where P is the nearest point on curve,
          U is its parameter, and distance is the distance to the original point.
        """
def to_2d(curve3d: mod3d.Geom.Curve, plane: mod3d.gp.Pln) -> Geom2d_Curve:
    """
    Projects a 3D curve onto a plane, returning a 2D curve
    """
def to_3d(curve2d: Geom2d_Curve, plane: mod3d.gp.Pln) -> mod3d.Geom.Curve:
    """
    Embeds a 2D curve into 3D space on the given plane
    """
