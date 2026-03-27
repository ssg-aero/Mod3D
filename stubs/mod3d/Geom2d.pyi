"""
Geom2d module
"""
from __future__ import annotations
import mod3d.GeomAbs
import mod3d.gp
import numpy
import numpy.typing
import typing
__all__: list[str] = ['BSplineCurve2d', 'BezierCurve2d', 'BoundedCurve2d', 'Circle2d', 'Conic2d', 'Curve2d', 'Ellipse2d', 'Geometry2d', 'Hyperbola2d', 'Line2d', 'OffsetCurve2d', 'Parabola2d', 'TrimmedCurve2d']
class BSplineCurve2d(BoundedCurve2d):
    """
    Describes a B-spline curve in 2D space.
    A B-spline curve is defined by poles, knots, multiplicities and degree.
    """
    @typing.overload
    def __init__(self, poles: typing.Any, knots: typing.Any, multiplicities: typing.Any, degree: typing.SupportsInt, periodic: bool = False) -> None:
        """
        Create a non-rational B-spline curve
        """
    @typing.overload
    def __init__(self, poles: typing.Any, weights: typing.Any, knots: typing.Any, multiplicities: typing.Any, degree: typing.SupportsInt, periodic: bool = False) -> None:
        """
        Create a rational B-spline curve
        """
    def increase_degree(self, degree: typing.SupportsInt) -> None:
        """
        Increases the degree of this curve
        """
    def insert_knot(self, U: typing.SupportsFloat, M: typing.SupportsInt = 1, ParametricTolerance: typing.SupportsFloat = 0.0) -> None:
        """
        Inserts a knot
        """
    def insert_knots(self, Knots: typing.Any, Multiplicities: typing.Any, ParametricTolerance: typing.SupportsFloat = 0.0, Add: bool = False) -> None:
        """
        Inserts multiple knots
        """
    def knot(self, Index: typing.SupportsInt) -> float:
        ...
    def locate_u(self, U: typing.SupportsFloat, ParametricTolerance: typing.SupportsFloat, WithKnotRepetition: bool = False) -> tuple:
        ...
    def multiplicity(self, Index: typing.SupportsInt) -> int:
        ...
    def pole(self, Index: typing.SupportsInt) -> mod3d.gp.Pnt2d:
        ...
    def remove_knot(self, Index: typing.SupportsInt, M: typing.SupportsInt, Tolerance: typing.SupportsFloat) -> bool:
        """
        Removes a knot
        """
    def segment(self, U1: typing.SupportsFloat, U2: typing.SupportsFloat, theTolerance: typing.SupportsFloat = 1e-12) -> None:
        """
        Modifies this B-spline curve by segmenting it between U1 and U2
        """
    def set_knot(self, Index: typing.SupportsInt, K: typing.SupportsFloat) -> None:
        ...
    def set_not_periodic(self) -> None:
        ...
    def set_origin(self, Index: typing.SupportsInt) -> None:
        ...
    def set_periodic(self) -> None:
        ...
    @typing.overload
    def set_pole(self, Index: typing.SupportsInt, P: mod3d.gp.Pnt2d) -> None:
        ...
    @typing.overload
    def set_pole(self, Index: typing.SupportsInt, P: mod3d.gp.Pnt2d, Weight: typing.SupportsFloat) -> None:
        ...
    def set_weight(self, Index: typing.SupportsInt, Weight: typing.SupportsFloat) -> None:
        ...
    def weight(self, Index: typing.SupportsInt) -> float:
        ...
    @property
    def degree(self) -> int:
        ...
    @property
    def first_u_knot_index(self) -> int:
        ...
    @property
    def is_rational(self) -> bool:
        ...
    @property
    def knot_sequence(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Get knot sequence as numpy array
        """
    @property
    def knots(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Get knots as numpy array
        """
    @property
    def last_u_knot_index(self) -> int:
        ...
    @property
    def multiplicities(self) -> numpy.typing.NDArray[numpy.int32]:
        """
        Get multiplicities as numpy array
        """
    @property
    def nb_knots(self) -> int:
        ...
    @property
    def nb_poles(self) -> int:
        ...
    @property
    def poles(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Get poles as numpy array of shape (n,2)
        """
    @property
    def weights(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Get weights as numpy array
        """
class BezierCurve2d(BoundedCurve2d):
    """
    Describes a Bezier curve in 2D space.
    A Bezier curve is defined by control points (poles).
    Parameter range: [0, 1]
    """
    @staticmethod
    def max_degree() -> int:
        """
        Returns the maximum degree a Bezier curve can have
        """
    @typing.overload
    def __init__(self, poles: typing.Any) -> None:
        """
        Create a non-rational Bezier curve from poles
        """
    @typing.overload
    def __init__(self, poles: typing.Any, weights: typing.Any) -> None:
        """
        Create a rational Bezier curve from poles and weights
        """
    def increase(self, degree: typing.SupportsInt) -> None:
        """
        Increases the degree of this Bezier curve
        """
    def insert_pole_after(self, Index: typing.SupportsInt, P: mod3d.gp.Pnt2d, Weight: typing.SupportsFloat = 1.0) -> None:
        ...
    def insert_pole_before(self, Index: typing.SupportsInt, P: mod3d.gp.Pnt2d, Weight: typing.SupportsFloat = 1.0) -> None:
        ...
    def pole(self, Index: typing.SupportsInt) -> mod3d.gp.Pnt2d:
        ...
    def remove_pole(self, Index: typing.SupportsInt) -> None:
        ...
    def resolution(self, ToleranceUV: typing.SupportsFloat) -> float:
        """
        Computes the parametric resolution corresponding to a 2D resolution
        """
    def segment(self, U1: typing.SupportsFloat, U2: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def set_pole(self, Index: typing.SupportsInt, P: mod3d.gp.Pnt2d) -> None:
        ...
    @typing.overload
    def set_pole(self, Index: typing.SupportsInt, P: mod3d.gp.Pnt2d, Weight: typing.SupportsFloat) -> None:
        ...
    def set_weight(self, Index: typing.SupportsInt, Weight: typing.SupportsFloat) -> None:
        ...
    def weight(self, Index: typing.SupportsInt) -> float:
        ...
    @property
    def degree(self) -> int:
        ...
    @property
    def is_rational(self) -> bool:
        ...
    @property
    def nb_poles(self) -> int:
        ...
    @property
    def poles(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Get poles as numpy array of shape (n,2)
        """
    @property
    def weights(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Get weights as numpy array
        """
class BoundedCurve2d(Curve2d):
    """
    Abstract base class for bounded curves in 2D space.
    A bounded curve has a finite extent defined by start and end points.
    """
    @property
    def end_point(self) -> mod3d.gp.Pnt2d:
        """
        Returns the end point of the curve
        """
    @property
    def start_point(self) -> mod3d.gp.Pnt2d:
        """
        Returns the start point of the curve
        """
class Circle2d(Conic2d):
    """
    Describes a circle in 2D space.
    Parameterization: P(U) = O + R*Cos(U)*XDir + R*Sin(U)*YDir
    Parameter range: [0, 2*Pi), period = 2*Pi
    """
    circ2d: mod3d.gp.Circ2d
    @typing.overload
    def __init__(self, c: mod3d.gp.Circ2d) -> None:
        """
        Constructs a circle by conversion of gp_Circ2d
        """
    @typing.overload
    def __init__(self, a: mod3d.gp.Ax2d, radius: typing.SupportsFloat, sense: bool = True) -> None:
        """
        Constructs a circle with axis A and radius.
        sense=True for counter-clockwise, False for clockwise
        """
    @typing.overload
    def __init__(self, a: mod3d.gp.Ax22d, radius: typing.SupportsFloat) -> None:
        """
        Constructs a circle with coordinate system A and radius
        """
    @property
    def radius(self) -> float:
        ...
    @radius.setter
    def radius(self, arg1: typing.SupportsFloat) -> None:
        ...
class Conic2d(Curve2d):
    """
    Abstract base class for conic curves in 2D space.
    Defines common behavior for Circle, Ellipse, Hyperbola and Parabola.
    """
    @property
    def eccentricity(self) -> float:
        """
        Returns the eccentricity of the conic
        """
    @property
    def location(self) -> mod3d.gp.Pnt2d:
        """
        Get or set the location point (center or apex)
        """
    @location.setter
    def location(self, arg1: mod3d.gp.Pnt2d) -> None:
        ...
    @property
    def position(self) -> mod3d.gp.Ax22d:
        """
        Get or set the local coordinate system of the conic
        """
    @position.setter
    def position(self, arg1: mod3d.gp.Ax22d) -> None:
        ...
    @property
    def x_axis(self) -> mod3d.gp.Ax2d:
        """
        Get or set the X axis of the conic
        """
    @x_axis.setter
    def x_axis(self, arg1: mod3d.gp.Ax2d) -> None:
        ...
    @property
    def y_axis(self) -> mod3d.gp.Ax2d:
        """
        Get or set the Y axis of the conic
        """
    @y_axis.setter
    def y_axis(self, arg1: mod3d.gp.Ax2d) -> None:
        ...
class Curve2d(Geometry2d):
    """
    Abstract base class for 2D curves.
    Provides parametric curve operations in 2D space.
    """
    def center_of_curvature(self, u: typing.SupportsFloat) -> mod3d.gp.Pnt2d:
        """
        Returns the center of curvature at parameter U
        """
    def curvature(self, u: typing.SupportsFloat) -> float:
        """
        Returns (curvature, first derivative of curvature) at parameter U
        """
    def d0(self, u: typing.SupportsFloat) -> mod3d.gp.Pnt2d:
        """
        Returns the point at parameter U
        """
    def d1(self, u: typing.SupportsFloat) -> tuple:
        """
        Returns (point, first derivative) at parameter U
        """
    def d2(self, u: typing.SupportsFloat) -> tuple:
        """
        Returns (point, first derivative, second derivative) at parameter U
        """
    def d3(self, u: typing.SupportsFloat) -> tuple:
        """
        Returns (point, d1, d2, d3) at parameter U
        """
    def dn(self, u: typing.SupportsFloat, n: typing.SupportsInt) -> mod3d.gp.Vec2d:
        """
        Returns the Nth derivative vector at parameter U
        """
    def is_cn(self, n: typing.SupportsInt) -> bool:
        """
        Returns True if the curve has CN continuity
        """
    @typing.overload
    def length(self, u1: typing.SupportsFloat, u2: typing.SupportsFloat) -> float:
        """
        Compute the length of the curve between parameters U1 and U2
        """
    @typing.overload
    def length(self) -> float:
        """
        Compute the length of the curve
        """
    def normal(self, u: typing.SupportsFloat) -> mod3d.gp.Dir2d:
        """
        Returns the normal direction at parameter U 
        """
    def parametric_transformation(self, t: mod3d.gp.Trsf2d) -> float:
        """
        Returns a coefficient to compute the parameter on the transformed curve
        """
    def reverse(self) -> None:
        """
        Changes the direction of parametrization of the curve
        """
    def reversed(self) -> Curve2d:
        """
        Returns a copy of this curve with reversed parametrization
        """
    def reversed_parameter(self, u: typing.SupportsFloat) -> float:
        """
        Returns the parameter on the reversed curve for the point of parameter U
        """
    def tangent(self, u: typing.SupportsFloat) -> mod3d.gp.Dir2d:
        """
        Returns the tangent direction at parameter U 
        """
    def transformed_parameter(self, u: typing.SupportsFloat, t: mod3d.gp.Trsf2d) -> float:
        """
        Returns the parameter on the transformed curve for the transform T
        """
    def value(self, u: typing.SupportsFloat) -> mod3d.gp.Pnt2d:
        """
        Returns the point at parameter U
        """
    @property
    def bounds(self) -> tuple:
        """
        Returns (first_parameter, last_parameter)
        """
    @property
    def continuity(self) -> mod3d.GeomAbs.Shape:
        """
        Returns the global continuity of the curve
        """
    @property
    def first_parameter(self) -> float:
        """
        Returns the first parameter of this curve
        """
    @property
    def is_closed(self) -> bool:
        """
        Returns True if the curve is closed
        """
    @property
    def is_periodic(self) -> bool:
        """
        Returns True if the curve is periodic
        """
    @property
    def last_parameter(self) -> float:
        """
        Returns the last parameter of this curve
        """
    @property
    def period(self) -> float:
        """
        Returns the period of this curve. Raises if not periodic
        """
class Ellipse2d(Conic2d):
    """
    Describes an ellipse in 2D space.
    Parameterization: P(U) = O + MajorRad*Cos(U)*XDir + MinorRad*Sin(U)*YDir
    Parameter range: [0, 2*Pi), period = 2*Pi
    """
    elips2d: mod3d.gp.Elips2d
    @typing.overload
    def __init__(self, e: mod3d.gp.Elips2d) -> None:
        """
        Constructs an ellipse by conversion of gp_Elips2d
        """
    @typing.overload
    def __init__(self, a: mod3d.gp.Ax2d, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat, sense: bool = True) -> None:
        """
        Constructs an ellipse with given axis and radii
        """
    @typing.overload
    def __init__(self, a: mod3d.gp.Ax22d, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat) -> None:
        """
        Constructs an ellipse with coordinate system A and radii
        """
    @property
    def directrix1(self) -> mod3d.gp.Ax2d:
        """
        Returns the first directrix
        """
    @property
    def directrix2(self) -> mod3d.gp.Ax2d:
        """
        Returns the second directrix
        """
    @property
    def focal(self) -> float:
        """
        Returns the focal distance
        """
    @property
    def focus1(self) -> mod3d.gp.Pnt2d:
        """
        Returns the first focus
        """
    @property
    def focus2(self) -> mod3d.gp.Pnt2d:
        """
        Returns the second focus
        """
    @property
    def major_radius(self) -> float:
        ...
    @major_radius.setter
    def major_radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def minor_radius(self) -> float:
        ...
    @minor_radius.setter
    def minor_radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def parameter(self) -> float:
        """
        Returns the parameter p = (1 - e²) * MajorRadius
        """
class Geometry2d:
    def __eq__(self, other: Geometry2d) -> bool:
        """
        Returns True if both refer to the same geometry object
        """
    def __hash__(self) -> int:
        """
        Returns hash value based on object identity
        """
class Hyperbola2d(Conic2d):
    """
    Describes a branch of a hyperbola in 2D space.
    Parameterization: P(U) = O + MajRad*Cosh(U)*XDir + MinRad*Sinh(U)*YDir
    Parameter range: ]-infinite, +infinite[
    """
    @typing.overload
    def __init__(self, h: mod3d.gp.Hypr2d) -> None:
        """
        Constructs a hyperbola by conversion of gp_Hypr2d
        """
    @typing.overload
    def __init__(self, a: mod3d.gp.Ax2d, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat, sense: bool = True) -> None:
        """
        Constructs a hyperbola with given axis and radii
        """
    @typing.overload
    def __init__(self, a: mod3d.gp.Ax22d, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat) -> None:
        """
        Constructs a hyperbola with coordinate system A and radii
        """
    @property
    def asymptote1(self) -> mod3d.gp.Ax2d:
        """
        Returns the first asymptote
        """
    @property
    def asymptote2(self) -> mod3d.gp.Ax2d:
        """
        Returns the second asymptote
        """
    @property
    def conjugate_branch1(self) -> mod3d.gp.Hypr2d:
        """
        Returns the first conjugate branch
        """
    @property
    def conjugate_branch2(self) -> mod3d.gp.Hypr2d:
        """
        Returns the second conjugate branch
        """
    @property
    def directrix1(self) -> mod3d.gp.Ax2d:
        """
        Returns the first directrix
        """
    @property
    def directrix2(self) -> mod3d.gp.Ax2d:
        """
        Returns the second directrix
        """
    @property
    def focal(self) -> float:
        """
        Returns the focal distance
        """
    @property
    def focus1(self) -> mod3d.gp.Pnt2d:
        """
        Returns the first focus
        """
    @property
    def focus2(self) -> mod3d.gp.Pnt2d:
        """
        Returns the second focus
        """
    @property
    def major_radius(self) -> float:
        ...
    @major_radius.setter
    def major_radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def minor_radius(self) -> float:
        ...
    @minor_radius.setter
    def minor_radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def other_branch(self) -> mod3d.gp.Hypr2d:
        """
        Returns the other branch
        """
    @property
    def parameter(self) -> float:
        """
        Returns the parameter
        """
class Line2d(Curve2d):
    """
    Describes a line in 2D space.
    A line is positioned with an axis (gp_Ax2d) defining origin and direction.
    Parameterization: P(U) = O + U*Dir where O is origin, Dir is direction.
    Parameter range: ]-infinite, +infinite[
    """
    direction: mod3d.gp.Dir2d
    line: mod3d.gp.Lin2d
    location: mod3d.gp.Pnt2d
    position: mod3d.gp.Ax2d
    @typing.overload
    def __init__(self, a: mod3d.gp.Ax2d) -> None:
        """
        Constructs a line from an axis
        """
    @typing.overload
    def __init__(self, l: mod3d.gp.Lin2d) -> None:
        """
        Constructs a line by conversion of gp_Lin2d
        """
    @typing.overload
    def __init__(self, p: mod3d.gp.Pnt2d, v: mod3d.gp.Dir2d) -> None:
        """
        Constructs a line passing through point P with direction V
        """
    def distance(self, p: mod3d.gp.Pnt2d) -> float:
        """
        Computes the distance between a point and this line
        """
class OffsetCurve2d(Curve2d):
    """
    Describes an offset curve in 2D space.
    An offset curve is at constant distance (offset) from a basis curve.
    The offset is perpendicular to the basis curve.
    """
    def __init__(self, basis_curve: Curve2d, offset: typing.SupportsFloat, is_not_check_c0: bool = False) -> None:
        """
        Creates an offset curve from a basis curve.
        Offset is the distance (positive = left side of curve direction)
        """
    def basis_curve(self) -> Curve2d:
        """
        Returns the basis curve
        """
    def set_basis_curve(self, c: Curve2d, is_not_check_c0: bool = False) -> None:
        """
        Changes the basis curve
        """
    def set_offset_value(self, d: typing.SupportsFloat) -> None:
        """
        Changes the offset value
        """
    @property
    def offset(self) -> float:
        ...
    @offset.setter
    def offset(self, arg1: typing.SupportsFloat) -> None:
        ...
class Parabola2d(Conic2d):
    """
    Describes a parabola in 2D space.
    Parameterization: P(U) = O + U²/(4*F)*XDir + U*YDir
    Parameter range: ]-infinite, +infinite[
    """
    parab2d: mod3d.gp.Parab2d
    @typing.overload
    def __init__(self, prb: mod3d.gp.Parab2d) -> None:
        """
        Constructs a parabola by conversion of gp_Parab2d
        """
    @typing.overload
    def __init__(self, a: mod3d.gp.Ax22d, focal: typing.SupportsFloat) -> None:
        """
        Constructs a parabola with coordinate system A and focal length
        """
    @typing.overload
    def __init__(self, a: mod3d.gp.Ax2d, focal: typing.SupportsFloat, sense: bool = True) -> None:
        """
        Constructs a parabola with axis A and focal length
        """
    @typing.overload
    def __init__(self, d: mod3d.gp.Ax2d, f: typing.SupportsFloat, sense: bool = True) -> None:
        """
        Constructs a parabola from directrix D and focus F
        """
    @property
    def directrix(self) -> mod3d.gp.Ax2d:
        """
        Returns the directrix of this parabola
        """
    @property
    def focal(self) -> float:
        ...
    @focal.setter
    def focal(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def focus(self) -> mod3d.gp.Pnt2d:
        """
        Returns the focus of this parabola
        """
    @property
    def parameter(self) -> float:
        """
        Returns the parameter (twice the focal length)
        """
class TrimmedCurve2d(BoundedCurve2d):
    """
    Describes a portion of a curve limited by two parameter values.
    The trimmed curve has the same parametric properties as the basis curve.
    """
    def __init__(self, c: Curve2d, u1: typing.SupportsFloat, u2: typing.SupportsFloat, sense: bool = True, adjust_periodic: bool = True) -> None:
        """
        Creates a trimmed curve from curve C between parameters U1 and U2
        """
    def set_trim(self, u1: typing.SupportsFloat, u2: typing.SupportsFloat, sense: bool = True, adjust_periodic: bool = True) -> None:
        """
        Changes the trim parameters
        """
    @property
    def basis_curve(self) -> Curve2d:
        """
        Returns the basis curve
        """
