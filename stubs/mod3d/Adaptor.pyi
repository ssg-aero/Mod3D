"""
Adaptor module
"""
from __future__ import annotations
import mod3d.Geom
import mod3d.GeomAbs
import mod3d.TopoDS
import mod3d.gp
import typing
__all__: list[str] = ['BRepCurve', 'BRepSurface', 'CompCurve', 'Curve', 'CurveOnSurface', 'GeomCurve', 'GeomSurface', 'IsoCurve', 'Surface']
class BRepCurve(Curve):
    """
    Adaptor to use an Edge of BRep topology like a 3D curve.
    
    The Curve from BRepAdaptor allows to use an Edge of the BRep topology
    like a 3D curve. It has the methods of the class Curve from Adaptor3d.
    
    It is created or initialized with an Edge. It takes into account local
    coordinate systems. If the Edge has a 3D curve it is used with priority.
    If the edge has no 3D curve one of the curves on surface is used.
    It is possible to enforce using a curve on surface by creating or
    initializing with an Edge and a Face.
    
    Example:
      edge = ...  # Get an edge from a shape
      adaptor = Adaptor.BRepCurve(edge)
      pt = adaptor.value(0.5)  # Evaluate at mid-parameter
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an undefined Curve with no Edge loaded.
        """
    @typing.overload
    def __init__(self, edge: mod3d.TopoDS.Edge) -> None:
        """
        Creates a Curve to access the geometry of edge E.
        
        Parameters:
          edge: The edge to adapt
        """
    @typing.overload
    def __init__(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> None:
        """
        Creates a Curve to access the geometry of edge E on face F.
        
        The geometry will be computed using the parametric curve of E on F.
        
        Parameters:
          edge: The edge to adapt
          face: The face containing the edge
        
        Raises:
          Standard_DomainError if the edge does not have a pcurve on the face
        """
    @typing.overload
    def initialize(self, edge: mod3d.TopoDS.Edge) -> None:
        """
        Sets the Curve to access the geometry of edge E.
        
        Parameters:
          edge: The edge to adapt
        """
    @typing.overload
    def initialize(self, edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> None:
        """
        Sets the Curve to access the geometry of edge E on face F.
        
        Parameters:
          edge: The edge to adapt
          face: The face containing the edge
        
        Raises:
          Standard_DomainError if the edge does not have a pcurve on the face
        """
    def reset(self) -> None:
        """
        Reset the currently loaded curve (undoes Initialize()).
        """
    @property
    def curve_on_surface(self) -> Adaptor3d_CurveOnSurface:
        """
        Returns the Adaptor3d_CurveOnSurface of the edge.
        """
    @property
    def edge(self) -> mod3d.TopoDS.Edge:
        """
        Returns the edge.
        """
    @property
    def geom_curve(self) -> GeomCurve:
        """
        Returns the GeomAdaptor_Curve of the edge.
        """
    @property
    def is_3d_curve(self) -> bool:
        """
        Returns True if the edge geometry is computed from a 3D curve.
        """
    @property
    def is_curve_on_surface(self) -> bool:
        """
        Returns True if the edge geometry is computed from a pcurve on a surface.
        """
    @property
    def tolerance(self) -> float:
        """
        Returns the edge tolerance.
        """
    @property
    def trsf(self) -> mod3d.gp.Trsf:
        """
        Returns the coordinate system of the curve.
        """
class BRepSurface(Surface):
    """
    Adaptor to use a Face of BRep topology like a 3D surface.
    
    The Surface from BRepAdaptor allows to use a Face of the BRep topology
    like a 3D surface. It has the methods of the class Surface from Adaptor3d.
    
    It is created or initialized with a Face. It takes into account the local
    coordinate system.
    
    The u,v parameter range is the minmax value for the restriction, unless
    the flag restriction is set to false.
    
    Example:
      face = ...  # Get a face from a shape
      adaptor = Adaptor.BRepSurface(face)
      pt = adaptor.value(0.5, 0.5)  # Evaluate at parameters
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an undefined surface with no face loaded.
        """
    @typing.overload
    def __init__(self, face: mod3d.TopoDS.Face, restriction: bool = True) -> None:
        """
        Creates a surface to access the geometry of F.
        
        Parameters:
          face: The face to adapt
          restriction: If True, the parameter range is the restriction range
        """
    def initialize(self, face: mod3d.TopoDS.Face, restriction: bool = True) -> None:
        """
        Sets the surface to the geometry of F.
        
        Parameters:
          face: The face to adapt
          restriction: If True, the parameter range is the restriction range
        """
    @property
    def face(self) -> mod3d.TopoDS.Face:
        """
        Returns the face.
        """
    @property
    def geom_surface(self) -> GeomSurface:
        """
        Returns the GeomAdaptor_Surface of the face.
        """
    @property
    def tolerance(self) -> float:
        """
        Returns the face tolerance.
        """
    @property
    def trsf(self) -> mod3d.gp.Trsf:
        """
        Returns the surface coordinate system.
        """
class CompCurve(Curve):
    """
    Adaptor to use a Wire of BRep topology like a 3D curve.
    
    The Curve from BRepAdaptor allows to use a Wire of the BRep topology
    like a 3D curve.
    
    Warning: With this class of curve, C0 and C1 continuities are not assumed.
    So be careful with some algorithms!
    
    Please note that BRepAdaptor_CompCurve cannot be a periodic curve at all
    (even if it contains a single periodic edge).
    
    BRepAdaptor_CompCurve can only work on valid wires where all edges are
    connected to each other to make a chain.
    
    Example:
      wire = ...  # Get a wire from a shape
      adaptor = Adaptor.CompCurve(wire)
      pt = adaptor.value(0.5)  # Evaluate at mid-parameter
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an undefined Curve with no Wire loaded.
        """
    @typing.overload
    def __init__(self, wire: mod3d.TopoDS.Wire, knot_by_curvilinear_abscissa: bool = False) -> None:
        """
        Creates a Curve to access the geometry of wire W.
        
        Parameters:
          wire: The wire to adapt
          knot_by_curvilinear_abscissa: If True, use curvilinear abscissa for knots
        """
    @typing.overload
    def __init__(self, wire: mod3d.TopoDS.Wire, knot_by_curvilinear_abscissa: bool, first: typing.SupportsFloat, last: typing.SupportsFloat, tol: typing.SupportsFloat) -> None:
        """
        Creates a Curve to access a trimmed portion of wire W.
        
        Parameters:
          wire: The wire to adapt
          knot_by_curvilinear_abscissa: If True, use curvilinear abscissa for knots
          first: First parameter
          last: Last parameter
          tol: Tolerance
        """
    def edge(self, u: typing.SupportsFloat) -> tuple[mod3d.TopoDS.Edge, float]:
        """
        Returns an edge and one parameter on it corresponding to parameter U.
        
        Parameters:
          u: Parameter on the composite curve
        
        Returns:
          Tuple of (edge, parameter_on_edge)
        """
    @typing.overload
    def initialize(self, wire: mod3d.TopoDS.Wire, knot_by_curvilinear_abscissa: bool) -> None:
        """
        Sets the wire W.
        
        Parameters:
          wire: The wire to adapt
          knot_by_curvilinear_abscissa: If True, use curvilinear abscissa for knots
        """
    @typing.overload
    def initialize(self, wire: mod3d.TopoDS.Wire, knot_by_curvilinear_abscissa: bool, first: typing.SupportsFloat, last: typing.SupportsFloat, tol: typing.SupportsFloat) -> None:
        """
        Sets a trimmed portion of wire W.
        
        Parameters:
          wire: The wire to adapt
          knot_by_curvilinear_abscissa: If True, use curvilinear abscissa for knots
          first: First parameter
          last: Last parameter
          tol: Tolerance
        """
    @property
    def wire(self) -> mod3d.TopoDS.Wire:
        """
        Returns the wire.
        """
class Curve:
    """
    Root class for 3D curves on which geometric algorithms work.
    
    An adapted curve is an interface between the services provided by a curve
    and those required of the curve by algorithms which use it.
    
    Derived concrete classes:
    - GeomAdaptor_Curve for a curve from the Geom package
    - Adaptor3d_CurveOnSurface for a curve lying on a surface
    - BRepAdaptor_Curve for an edge geometry as a 3D curve
    
    Note: Polynomial coefficients of BSpline curves used for their evaluation
    are cached for better performance. Therefore these evaluations are not
    thread-safe and parallel evaluations need to be prevented.
    """
    @typing.overload
    def __init__(self, geom_curve: mod3d.Geom.Curve) -> None:
        """
        Constructor from a Geom_Curve.
        """
    @typing.overload
    def __init__(self, edge: mod3d.TopoDS.Edge) -> None:
        """
        Constructor from a TopoDS_Edge.
        """
    @typing.overload
    def __init__(self, wire: mod3d.TopoDS.Wire) -> None:
        """
        Constructor from a TopoDS_Wire.
        """
    def bezier(self) -> mod3d.Geom.BezierCurve:
        """
        Returns the Bezier curve if GetType() == BezierCurve.
        
        Raises:
          Standard_NoSuchObject if curve is not a Bezier curve
        """
    def bspline(self) -> mod3d.Geom.BSplineCurve:
        """
        Returns the BSpline curve if GetType() == BSplineCurve.
        
        Raises:
          Standard_NoSuchObject if curve is not a BSpline curve
        """
    def circle(self) -> mod3d.gp.Circ:
        """
        Returns the circle if GetType() == Circle.
        
        Raises:
          Standard_NoSuchObject if curve is not a circle
        """
    def d0(self, u: typing.SupportsFloat) -> mod3d.gp.Pnt:
        """
        Computes the point of parameter U on the curve.
        
        Parameters:
          u: Parameter value
        
        Returns:
          gp_Pnt at parameter u
        """
    def d1(self, u: typing.SupportsFloat) -> tuple[mod3d.gp.Pnt, mod3d.gp.Vec]:
        """
        Computes the point and first derivative at parameter U.
        
        Parameters:
          u: Parameter value
        
        Returns:
          Tuple of (point, first_derivative)
        
        Raises:
          If continuity of current interval is not C1
        """
    def d2(self, u: typing.SupportsFloat) -> tuple[mod3d.gp.Pnt, mod3d.gp.Vec, mod3d.gp.Vec]:
        """
        Computes the point, first and second derivatives at parameter U.
        
        Parameters:
          u: Parameter value
        
        Returns:
          Tuple of (point, first_derivative, second_derivative)
        
        Raises:
          If continuity of current interval is not C2
        """
    def d3(self, u: typing.SupportsFloat) -> tuple[mod3d.gp.Pnt, mod3d.gp.Vec, mod3d.gp.Vec, mod3d.gp.Vec]:
        """
        Computes the point and first three derivatives at parameter U.
        
        Parameters:
          u: Parameter value
        
        Returns:
          Tuple of (point, d1, d2, d3)
        
        Raises:
          If continuity of current interval is not C3
        """
    def dn(self, u: typing.SupportsFloat, n: typing.SupportsInt) -> mod3d.gp.Vec:
        """
        Returns the derivative of order N at parameter U.
        
        Parameters:
          u: Parameter value
          n: Order of derivation (>= 1)
        
        Returns:
          gp_Vec derivative
        
        Raises:
          If continuity of current interval is not CN
          If N < 1
        """
    def ellipse(self) -> mod3d.gp.Elips:
        """
        Returns the ellipse if GetType() == Ellipse.
        
        Raises:
          Standard_NoSuchObject if curve is not an ellipse
        """
    def get_type(self) -> mod3d.GeomAbs.CurveType:
        """
        Returns the type of the curve in the current interval.
        
        Returns:
          GeomAbs.CurveType: Line, Circle, Ellipse, Hyperbola, Parabola,
          BezierCurve, BSplineCurve, OffsetCurve, or OtherCurve
        """
    def hyperbola(self) -> mod3d.gp.Hypr:
        """
        Returns the hyperbola if GetType() == Hyperbola.
        
        Raises:
          Standard_NoSuchObject if curve is not a hyperbola
        """
    def intervals(self, continuity: mod3d.GeomAbs.Shape) -> list[float]:
        """
        Returns the parameters bounding the intervals of continuity S.
        
        Parameters:
          continuity: The desired continuity (GeomAbs.Shape)
        
        Returns:
          List of parameter values bounding the intervals
        """
    def line(self) -> mod3d.gp.Lin:
        """
        Returns the line if GetType() == Line.
        
        Raises:
          Standard_NoSuchObject if curve is not a line
        """
    def nb_intervals(self, continuity: mod3d.GeomAbs.Shape) -> int:
        """
        Returns the number of intervals for continuity S.
        
        Parameters:
          continuity: The desired continuity (GeomAbs.Shape)
        
        Returns:
          Number of intervals. May be one if Continuity(me) >= S
        """
    def offset_curve(self) -> mod3d.Geom.OffsetCurve:
        """
        Returns the offset curve if GetType() == OffsetCurve.
        
        Raises:
          Standard_NoSuchObject if curve is not an offset curve
        """
    def parabola(self) -> mod3d.gp.Parab:
        """
        Returns the parabola if GetType() == Parabola.
        
        Raises:
          Standard_NoSuchObject if curve is not a parabola
        """
    def resolution(self, r3d: typing.SupportsFloat) -> float:
        """
        Returns the parametric resolution corresponding to the real space resolution R3d.
        
        Parameters:
          r3d: Real space resolution
        
        Returns:
          Parametric resolution
        """
    def shallow_copy(self) -> Curve:
        """
        Returns a shallow copy of this adaptor.
        """
    def trim(self, first: typing.SupportsFloat, last: typing.SupportsFloat, tol: typing.SupportsFloat) -> Curve:
        """
        Returns a curve equivalent of this curve between parameters First and Last.
        
        Parameters:
          first: First parameter
          last: Last parameter
          tol: Tolerance for 3D points confusion
        """
    def value(self, u: typing.SupportsFloat) -> mod3d.gp.Pnt:
        """
        Computes the point of parameter U on the curve.
        
        Parameters:
          u: Parameter value
        
        Returns:
          gp_Pnt at parameter u
        """
    @property
    def continuity(self) -> mod3d.GeomAbs.Shape:
        """
        Returns the global continuity of the curve.
        """
    @property
    def degree(self) -> int:
        """
        Returns the degree for BSpline/Bezier curves.
        """
    @property
    def first_parameter(self) -> float:
        """
        Returns the first parameter of the curve.
        """
    @property
    def is_closed(self) -> bool:
        """
        Returns True if the curve is closed.
        """
    @property
    def is_periodic(self) -> bool:
        """
        Returns True if the curve is periodic.
        """
    @property
    def is_rational(self) -> bool:
        """
        Returns True if the curve is rational (BSpline/Bezier).
        """
    @property
    def last_parameter(self) -> float:
        """
        Returns the last parameter of the curve.
        """
    @property
    def nb_knots(self) -> int:
        """
        Returns the number of knots for BSpline curves.
        """
    @property
    def nb_poles(self) -> int:
        """
        Returns the number of poles for BSpline/Bezier curves.
        """
    @property
    def period(self) -> float:
        """
        Returns the period of the curve (raises if not periodic).
        """
class CurveOnSurface(Curve):
    """
    An interface between the services provided by a curve lying on a surface
    from the package Geom and those required of the curve by algorithms which use it.
    
    The curve is defined as a 2D curve from the Geom2d package, in the parametric
    space of the surface.
    
    Example:
      curve_2d = ...  # A 2D curve adaptor in (u,v) space
      surface = ...   # A surface adaptor
      curve_on_surf = Adaptor.CurveOnSurface(curve_2d, surface)
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an undefined CurveOnSurface.
        """
    @typing.overload
    def __init__(self, surface: Adaptor3d_Surface) -> None:
        """
        Creates a CurveOnSurface with only a surface defined.
        
        Parameters:
          surface: The surface adaptor
        """
    @typing.overload
    def __init__(self, curve_2d: Adaptor2d_Curve2d, surface: Adaptor3d_Surface) -> None:
        """
        Creates a CurveOnSurface from the 2D curve and the surface.
        
        Parameters:
          curve_2d: The 2D curve adaptor in parametric space
          surface: The surface adaptor
        """
    @typing.overload
    def load(self, surface: Adaptor3d_Surface) -> None:
        """
        Changes the surface.
        
        Parameters:
          surface: The new surface adaptor
        """
    @typing.overload
    def load(self, curve_2d: Adaptor2d_Curve2d) -> None:
        """
        Changes the 2D curve.
        
        Parameters:
          curve_2d: The new 2D curve adaptor
        """
    @typing.overload
    def load(self, curve_2d: Adaptor2d_Curve2d, surface: Adaptor3d_Surface) -> None:
        """
        Loads both curve and surface.
        
        Parameters:
          curve_2d: The 2D curve adaptor
          surface: The surface adaptor
        """
    @property
    def get_curve(self) -> Adaptor2d_Curve2d:
        """
        Returns the 2D curve adaptor.
        """
    @property
    def get_surface(self) -> Adaptor3d_Surface:
        """
        Returns the surface adaptor.
        """
class GeomCurve(Curve):
    """
    Adaptor interface between Geom_Curve and algorithms that use adapted curves.
    
    This class provides an interface between the services provided by any curve
    from the package Geom and those required of the curve by algorithms which use it.
    Creation of the loaded curve the curve is C1 by piece.
    
    Note: Polynomial coefficients of BSpline curves used for their evaluation are
    cached for better performance. Therefore these evaluations are not thread-safe.
    
    Example:
      curve = Geom.BSplineCurve(...)  # Create a Geom curve
      adaptor = Adaptor.GeomCurve(curve)  # Adapt it
      pt = adaptor.value(0.5)  # Evaluate at parameter 0.5
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an undefined curve with no Geom_Curve loaded.
        """
    @typing.overload
    def __init__(self, curve: mod3d.Geom.Curve) -> None:
        """
        Creates an adaptor for the given Geom_Curve.
        
        Parameters:
          curve: The Geom_Curve to adapt
        
        Raises:
          Standard_NullObject if curve is null
        """
    @typing.overload
    def __init__(self, curve: mod3d.Geom.Curve, u_first: typing.SupportsFloat, u_last: typing.SupportsFloat) -> None:
        """
        Creates an adaptor for a portion of the Geom_Curve.
        
        Parameters:
          curve: The Geom_Curve to adapt
          u_first: First parameter
          u_last: Last parameter
        
        Raises:
          Standard_NullObject if curve is null
          Standard_ConstructionError if u_first > u_last
        """
    @typing.overload
    def load(self, curve: mod3d.Geom.Curve) -> None:
        """
        Loads a Geom_Curve.
        
        Parameters:
          curve: The Geom_Curve to load
        
        Raises:
          Standard_NullObject if curve is null
        """
    @typing.overload
    def load(self, curve: mod3d.Geom.Curve, u_first: typing.SupportsFloat, u_last: typing.SupportsFloat) -> None:
        """
        Loads a portion of a Geom_Curve.
        
        Parameters:
          curve: The Geom_Curve to load
          u_first: First parameter
          u_last: Last parameter
        
        Raises:
          Standard_NullObject if curve is null
          Standard_ConstructionError if u_first > u_last
        """
    def reset(self) -> None:
        """
        Reset the currently loaded curve (undoes Load()).
        """
    @property
    def curve(self) -> mod3d.Geom.Curve:
        """
        Returns the underlying Geom_Curve.
        """
class GeomSurface(Surface):
    """
    Adaptor interface between Geom_Surface and algorithms that use adapted surfaces.
    
    This class provides an interface between the services provided by any surface
    from the package Geom and those required of the surface by algorithms which use it.
    
    Example:
      surface = Geom.Plane(...)  # Create a Geom surface
      adaptor = Adaptor.GeomSurface(surface)  # Adapt it
      pt = adaptor.value(0.5, 0.5)  # Evaluate at parameters (0.5, 0.5)
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an undefined surface with no Geom_Surface loaded.
        """
    @typing.overload
    def __init__(self, surface: mod3d.Geom.Surface) -> None:
        """
        Creates an adaptor for the given Geom_Surface.
        
        Parameters:
          surface: The Geom_Surface to adapt
        """
    @typing.overload
    def __init__(self, surface: mod3d.Geom.Surface, u_first: typing.SupportsFloat, u_last: typing.SupportsFloat, v_first: typing.SupportsFloat, v_last: typing.SupportsFloat, tol_u: typing.SupportsFloat = 0.0, tol_v: typing.SupportsFloat = 0.0) -> None:
        """
        Creates an adaptor for a portion of the Geom_Surface.
        
        Parameters:
          surface: The Geom_Surface to adapt
          u_first, u_last: U parameter bounds
          v_first, v_last: V parameter bounds
          tol_u, tol_v: Tolerances
        """
    @typing.overload
    def load(self, surface: mod3d.Geom.Surface) -> None:
        """
        Loads a Geom_Surface.
        """
    @typing.overload
    def load(self, surface: mod3d.Geom.Surface, u_first: typing.SupportsFloat, u_last: typing.SupportsFloat, v_first: typing.SupportsFloat, v_last: typing.SupportsFloat, tol_u: typing.SupportsFloat = 0.0, tol_v: typing.SupportsFloat = 0.0) -> None:
        """
        Loads a portion of a Geom_Surface with specified bounds.
        """
    @property
    def surface(self) -> mod3d.Geom.Surface:
        """
        Returns the underlying Geom_Surface.
        """
class IsoCurve(Curve):
    """
    Defines an isoparametric curve on a surface.
    
    The type of isoparametric curve (U or V) is defined with the enumeration
    IsoType from GeomAbs. If NoneIso is given an error is raised.
    
    Example:
      surface_adaptor = ...  # Get a surface adaptor
      iso_u = Adaptor.IsoCurve(surface_adaptor, GeomAbs.IsoU, 0.5)
      pt = iso_u.value(0.0)  # Point at V=0 on the U=0.5 isocurve
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an undefined IsoCurve. The iso is set to NoneIso.
        """
    @typing.overload
    def __init__(self, surface: Adaptor3d_Surface) -> None:
        """
        Creates an IsoCurve on a surface. The iso is set to NoneIso.
        
        Parameters:
          surface: The surface adaptor
        """
    @typing.overload
    def __init__(self, surface: Adaptor3d_Surface, iso: mod3d.GeomAbs.IsoType, param: typing.SupportsFloat) -> None:
        """
        Creates an IsoCurve on a surface.
        
        Parameters:
          surface: The surface adaptor
          iso: The type of iso curve (IsoU or IsoV)
          param: The parameter value defining the iso curve
        
        The bounds of the iso curve are the bounds of the surface.
        """
    @typing.overload
    def __init__(self, surface: Adaptor3d_Surface, iso: mod3d.GeomAbs.IsoType, param: typing.SupportsFloat, w_first: typing.SupportsFloat, w_last: typing.SupportsFloat) -> None:
        """
        Creates an IsoCurve on a surface with specified bounds.
        
        Parameters:
          surface: The surface adaptor
          iso: The type of iso curve (IsoU or IsoV)
          param: The parameter value defining the iso curve
          w_first: First bound
          w_last: Last bound
        """
    @typing.overload
    def load(self, surface: Adaptor3d_Surface) -> None:
        """
        Changes the surface. The iso is reset to NoneIso.
        
        Parameters:
          surface: The new surface adaptor
        """
    @typing.overload
    def load(self, iso: mod3d.GeomAbs.IsoType, param: typing.SupportsFloat) -> None:
        """
        Changes the iso on the current surface.
        
        Parameters:
          iso: The type of iso curve (IsoU or IsoV)
          param: The parameter value
        """
    @typing.overload
    def load(self, iso: mod3d.GeomAbs.IsoType, param: typing.SupportsFloat, w_first: typing.SupportsFloat, w_last: typing.SupportsFloat) -> None:
        """
        Changes the iso on the current surface with specified bounds.
        
        Parameters:
          iso: The type of iso curve (IsoU or IsoV)
          param: The parameter value
          w_first: First bound
          w_last: Last bound
        """
    @property
    def iso(self) -> mod3d.GeomAbs.IsoType:
        """
        Returns the type of iso (IsoU, IsoV, or NoneIso).
        """
    @property
    def parameter(self) -> float:
        """
        Returns the iso parameter value.
        """
    @property
    def surface(self) -> Adaptor3d_Surface:
        """
        Returns the surface adaptor.
        """
class Surface:
    """
    Root class for surfaces on which geometric algorithms work.
    
    An adapted surface is an interface between the services provided by a surface
    and those required of the surface by algorithms which use it.
    
    Derived concrete classes:
    - GeomAdaptor_Surface for a surface from the Geom package
    - BRepAdaptor_Surface for a face geometry as a surface
    """
    def cone(self) -> mod3d.gp.Cone:
        """
        Returns the cone if GetType() == Cone.
        """
    def cylinder(self) -> mod3d.gp.Cylinder:
        """
        Returns the cylinder if GetType() == Cylinder.
        """
    def d0(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> mod3d.gp.Pnt:
        """
        Computes the point at parameters (U, V).
        """
    def d1(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> tuple[mod3d.gp.Pnt, mod3d.gp.Vec, mod3d.gp.Vec]:
        """
        Computes the point and first derivatives at parameters (U, V).
        
        Returns:
          Tuple of (point, d1u, d1v)
        """
    def d2(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> tuple[mod3d.gp.Pnt, mod3d.gp.Vec, mod3d.gp.Vec, mod3d.gp.Vec, mod3d.gp.Vec, mod3d.gp.Vec]:
        """
        Computes the point and first and second derivatives at parameters (U, V).
        
        Returns:
          Tuple of (point, d1u, d1v, d2u, d2v, d2uv)
        """
    def dn(self, u: typing.SupportsFloat, v: typing.SupportsFloat, nu: typing.SupportsInt, nv: typing.SupportsInt) -> mod3d.gp.Vec:
        """
        Returns the derivative of order (Nu, Nv) at parameters (U, V).
        """
    def get_type(self) -> mod3d.GeomAbs.SurfaceType:
        """
        Returns the type of the surface.
        """
    def nb_u_intervals(self, continuity: mod3d.GeomAbs.Shape) -> int:
        """
        Returns the number of intervals for continuity S in U direction.
        """
    def nb_v_intervals(self, continuity: mod3d.GeomAbs.Shape) -> int:
        """
        Returns the number of intervals for continuity S in V direction.
        """
    def plane(self) -> mod3d.gp.Pln:
        """
        Returns the plane if GetType() == Plane.
        """
    def shallow_copy(self) -> Surface:
        """
        Returns a shallow copy of this adaptor.
        """
    def sphere(self) -> mod3d.gp.Sphere:
        """
        Returns the sphere if GetType() == Sphere.
        """
    def torus(self) -> mod3d.gp.Torus:
        """
        Returns the torus if GetType() == Torus.
        """
    def u_resolution(self, r3d: typing.SupportsFloat) -> float:
        """
        Returns the parametric U resolution for the real space resolution R3d.
        """
    def v_resolution(self, r3d: typing.SupportsFloat) -> float:
        """
        Returns the parametric V resolution for the real space resolution R3d.
        """
    def value(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> mod3d.gp.Pnt:
        """
        Computes the point at parameters (U, V).
        """
    @property
    def first_u_parameter(self) -> float:
        """
        Returns the first parameter in U direction.
        """
    @property
    def first_v_parameter(self) -> float:
        """
        Returns the first parameter in V direction.
        """
    @property
    def is_u_closed(self) -> bool:
        """
        Returns True if the surface is closed in U direction.
        """
    @property
    def is_u_periodic(self) -> bool:
        """
        Returns True if the surface is periodic in U direction.
        """
    @property
    def is_v_closed(self) -> bool:
        """
        Returns True if the surface is closed in V direction.
        """
    @property
    def is_v_periodic(self) -> bool:
        """
        Returns True if the surface is periodic in V direction.
        """
    @property
    def last_u_parameter(self) -> float:
        """
        Returns the last parameter in U direction.
        """
    @property
    def last_v_parameter(self) -> float:
        """
        Returns the last parameter in V direction.
        """
    @property
    def u_continuity(self) -> mod3d.GeomAbs.Shape:
        """
        Returns the continuity in U direction.
        """
    @property
    def u_period(self) -> float:
        """
        Returns the U period (raises if not periodic).
        """
    @property
    def v_continuity(self) -> mod3d.GeomAbs.Shape:
        """
        Returns the continuity in V direction.
        """
    @property
    def v_period(self) -> float:
        """
        Returns the V period (raises if not periodic).
        """
