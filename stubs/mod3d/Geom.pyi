"""
Geom module
"""
from __future__ import annotations
import mod3d.GeomAbs
import mod3d.gp
import numpy
import numpy.typing
import typing
__all__: list[str] = ['BSplineCurve', 'BSplineSurface', 'BezierCurve', 'BezierSurface', 'BoundedCurve', 'BoundedSurface', 'Circle', 'Conic', 'ConicalSurface', 'Curve', 'CylindricalSurface', 'ElementarySurface', 'Ellipse', 'Geometry', 'Hyperbola', 'Line', 'OffsetCurve', 'OffsetSurface', 'Parabola', 'Plane', 'RectangularTrimmedSurface', 'SphericalSurface', 'Surface', 'SurfaceOfLinearExtrusion', 'SurfaceOfRevolution', 'SweptSurface', 'ToroidalSurface', 'TrimmedCurve']
class BSplineCurve(BoundedCurve):
    @staticmethod
    @typing.overload
    def __init__(*args, **kwargs) -> None:
        """
        Create a B-spline curve from a gbs::BSCurve object.
        """
    @staticmethod
    @typing.overload
    def __init__(*args, **kwargs) -> None:
        """
        Create a rational B-spline curve from a gbs::BSCurveRational object.
        """
    @staticmethod
    def max_degree() -> int:
        ...
    @typing.overload
    def __init__(self, poles: typing.Any, knots: typing.Any, multiplicities: typing.Any, degree: typing.SupportsInt, periodic: bool = False) -> None:
        """
        Create a non-rational B-spline curve. Poles can be list of gp_Pnt or numpy array of shape (n,3). Knots and multiplicities can be lists or 1D numpy arrays.
        """
    @typing.overload
    def __init__(self, poles: typing.Any, weights: typing.Any, knots: typing.Any, multiplicities: typing.Any, degree: typing.SupportsInt, periodic: bool = False, check_rational: bool = True) -> None:
        """
        Create a rational B-spline curve. All arrays can be lists or numpy arrays.
        """
    def increase_degree(self, degree: typing.SupportsInt) -> None:
        """
        Increases the degree of this BSpline curve to Degree
        """
    @typing.overload
    def increase_multiplicity(self, index: typing.SupportsInt, M: typing.SupportsInt) -> None:
        ...
    @typing.overload
    def increase_multiplicity(self, I1: typing.SupportsInt, I2: typing.SupportsInt, M: typing.SupportsInt) -> None:
        ...
    def increment_multiplicity(self, I1: typing.SupportsInt, I2: typing.SupportsInt, M: typing.SupportsInt) -> None:
        ...
    def insert_knot(self, U: typing.SupportsFloat, M: typing.SupportsInt = 1, ParametricTolerance: typing.SupportsFloat = 0.0, Add: bool = True) -> None:
        ...
    def is_equal(self, other: BSplineCurve, precision: typing.SupportsFloat) -> bool:
        ...
    def is_g1(self, theTf: typing.SupportsFloat, theTl: typing.SupportsFloat, theAngTol: typing.SupportsFloat) -> bool:
        ...
    def knot(self, Index: typing.SupportsInt) -> float:
        ...
    def local_d0(self, U: typing.SupportsFloat, FromK1: typing.SupportsInt, ToK2: typing.SupportsInt) -> mod3d.gp.Pnt:
        ...
    def local_d1(self, U: typing.SupportsFloat, FromK1: typing.SupportsInt, ToK2: typing.SupportsInt) -> tuple:
        ...
    def local_d2(self, U: typing.SupportsFloat, FromK1: typing.SupportsInt, ToK2: typing.SupportsInt) -> tuple:
        ...
    def local_d3(self, U: typing.SupportsFloat, FromK1: typing.SupportsInt, ToK2: typing.SupportsInt) -> tuple:
        ...
    def local_dn(self, U: typing.SupportsFloat, FromK1: typing.SupportsInt, ToK2: typing.SupportsInt, N: typing.SupportsInt) -> mod3d.gp.Vec:
        ...
    def local_value(self, U: typing.SupportsFloat, FromK1: typing.SupportsInt, ToK2: typing.SupportsInt) -> mod3d.gp.Pnt:
        ...
    def locate_u(self, U: typing.SupportsFloat, ParametricTolerance: typing.SupportsFloat, WithKnotRepetition: bool = False) -> tuple:
        ...
    def move_point(self, U: typing.SupportsFloat, P: mod3d.gp.Pnt, Index1: typing.SupportsInt, Index2: typing.SupportsInt) -> tuple:
        ...
    def move_point_and_tangent(self, U: typing.SupportsFloat, P: mod3d.gp.Pnt, Tangent: mod3d.gp.Vec, Tolerance: typing.SupportsFloat, StartingCondition: typing.SupportsInt, EndingCondition: typing.SupportsInt) -> int:
        ...
    def multiplicity(self, Index: typing.SupportsInt) -> int:
        ...
    def periodic_normalization(self, U: typing.SupportsFloat) -> None:
        ...
    def pole(self, Index: typing.SupportsInt) -> mod3d.gp.Pnt:
        ...
    def remove_knot(self, Index: typing.SupportsInt, M: typing.SupportsInt, Tolerance: typing.SupportsFloat) -> bool:
        ...
    def resolution(self, Tolerance3D: typing.SupportsFloat) -> float:
        ...
    def segment(self, U1: typing.SupportsFloat, U2: typing.SupportsFloat, theTolerance: typing.SupportsFloat = 1e-12) -> None:
        ...
    @typing.overload
    def set_knot(self, Index: typing.SupportsInt, K: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def set_knot(self, Index: typing.SupportsInt, K: typing.SupportsFloat, M: typing.SupportsInt) -> None:
        ...
    def set_not_periodic(self) -> None:
        ...
    @typing.overload
    def set_origin(self, Index: typing.SupportsInt) -> None:
        ...
    @typing.overload
    def set_origin(self, U: typing.SupportsFloat, Tol: typing.SupportsFloat) -> None:
        ...
    def set_periodic(self) -> None:
        ...
    @typing.overload
    def set_pole(self, Index: typing.SupportsInt, P: mod3d.gp.Pnt) -> None:
        ...
    @typing.overload
    def set_pole(self, Index: typing.SupportsInt, P: mod3d.gp.Pnt, Weight: typing.SupportsFloat) -> None:
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
        Get poles as numpy array of shape (n,3)
        """
    @property
    def weights(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Get weights as numpy array
        """
class BSplineSurface(BoundedSurface):
    """
    
            A B-spline surface (rational or non-rational).
    
            In each parametric direction, a B-spline surface can be:
            - Uniform or non-uniform
            - Rational or non-rational
            - Periodic or non-periodic
    
            Defined by:
            - Degrees in U and V (max 25)
            - Table of poles (control points)
            - Table of weights (for rational surfaces)
            - Knot vectors with multiplicities in U and V
    
            The surface is infinitely differentiable except at knots, where
            continuity = degree - multiplicity.
            
    """
    max_degree: typing.ClassVar[int] = 25
    @staticmethod
    @typing.overload
    def __init__(*args, **kwargs) -> None:
        """
        Create a B-spline surface from a gbs::BSSurface object.
        """
    @staticmethod
    @typing.overload
    def __init__(*args, **kwargs) -> None:
        """
        Create a rational B-spline surface from a gbs::BSSurfaceRational object.
        """
    @typing.overload
    def __init__(self, poles: typing.Annotated[numpy.typing.ArrayLike, numpy.float64], u_knots: typing.Any, v_knots: typing.Any, u_multiplicities: typing.Any, v_multiplicities: typing.Any, u_degree: typing.SupportsInt, v_degree: typing.SupportsInt, u_periodic: bool = False, v_periodic: bool = False) -> None:
        """
                    Create a non-rational B-spline surface.
        
                    Args:
                        poles: 2D array of 3D points with shape [nu, nv, 3].
                        u_knots, v_knots: Knot vectors (list or 1D numpy array).
                        u_multiplicities, v_multiplicities: Multiplicities (list or 1D numpy array).
                        u_degree, v_degree: Degrees in U and V directions.
                        u_periodic, v_periodic: Whether the surface is periodic in U/V.
        """
    @typing.overload
    def __init__(self, poles: typing.Annotated[numpy.typing.ArrayLike, numpy.float64], weights: typing.Annotated[numpy.typing.ArrayLike, numpy.float64], u_knots: typing.Any, v_knots: typing.Any, u_multiplicities: typing.Any, v_multiplicities: typing.Any, u_degree: typing.SupportsInt, v_degree: typing.SupportsInt, u_periodic: bool = False, v_periodic: bool = False) -> None:
        """
                    Create a rational B-spline surface.
        
                    Args:
                        poles: 2D array of 3D points with shape [nu, nv, 3].
                        weights: 2D array of weights with shape [nu, nv].
                        u_knots, v_knots: Knot vectors (list or 1D numpy array).
                        u_multiplicities, v_multiplicities: Multiplicities (list or 1D numpy array).
                        u_degree, v_degree: Degrees in U and V directions.
                        u_periodic, v_periodic: Whether the surface is periodic in U/V.
        """
    def exchange_uv(self) -> None:
        """
        Exchange U and V parametric directions.
        """
    def increase_degree(self, u_degree: typing.SupportsInt, v_degree: typing.SupportsInt) -> None:
        """
        Increase the degrees to (u_degree, v_degree). Preserves surface shape.
        """
    def locate_u(self, u: typing.SupportsFloat, tolerance: typing.SupportsFloat) -> tuple:
        """
        Locate U in the knot sequence. Returns (i1, i2) where UKnot(i1) <= U <= UKnot(i2).
        """
    def locate_v(self, v: typing.SupportsFloat, tolerance: typing.SupportsFloat) -> tuple:
        """
        Locate V in the knot sequence. Returns (i1, i2) where VKnot(i1) <= V <= VKnot(i2).
        """
    def pole(self, u_index: typing.SupportsInt, v_index: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Return the pole at (u_index, v_index). Indices are 1-based.
        """
    def poles(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Return all poles as a numpy array of shape [nb_u_poles, nb_v_poles, 3].
        """
    def resolution(self, tolerance_3d: typing.SupportsFloat) -> tuple:
        """
                    Compute parametric tolerances from a 3D tolerance.
        
                    Returns (u_tolerance, v_tolerance) such that if |du| < u_tolerance
                    and |dv| < v_tolerance, then |S(u+du,v+dv) - S(u,v)| < tolerance_3d.
        """
    def segment(self, u1: typing.SupportsFloat, u2: typing.SupportsFloat, v1: typing.SupportsFloat, v2: typing.SupportsFloat, u_tolerance: typing.SupportsFloat = 1e-07, v_tolerance: typing.SupportsFloat = 1e-07) -> None:
        """
        Segment the surface to the parameter range [u1, u2] x [v1, v2].
        """
    @typing.overload
    def set_pole(self, u_index: typing.SupportsInt, v_index: typing.SupportsInt, p: mod3d.gp.Pnt) -> None:
        """
        Set the pole at (u_index, v_index) to point p.
        """
    @typing.overload
    def set_pole(self, u_index: typing.SupportsInt, v_index: typing.SupportsInt, p: mod3d.gp.Pnt, weight: typing.SupportsFloat) -> None:
        """
        Set the pole and weight at (u_index, v_index).
        """
    def set_u_not_periodic(self) -> None:
        """
        Make the surface non-periodic in U.
        """
    def set_u_periodic(self) -> None:
        """
        Make the surface periodic in U (must be closed first).
        """
    def set_v_not_periodic(self) -> None:
        """
        Make the surface non-periodic in V.
        """
    def set_v_periodic(self) -> None:
        """
        Make the surface periodic in V (must be closed first).
        """
    def set_weight(self, u_index: typing.SupportsInt, v_index: typing.SupportsInt, weight: typing.SupportsFloat) -> None:
        """
        Set the weight at (u_index, v_index).
        """
    def u_knot(self, index: typing.SupportsInt) -> float:
        """
        Return the U knot at index (1-based).
        """
    def u_knot_sequence(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Return U knots with repetitions as a numpy array.
        """
    def u_knots(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Return all distinct U knots as a numpy array.
        """
    def u_multiplicities(self) -> numpy.typing.NDArray[numpy.int32]:
        """
        Return all U knot multiplicities as a numpy array.
        """
    def u_multiplicity(self, index: typing.SupportsInt) -> int:
        """
        Return the multiplicity of U knot at index (1-based).
        """
    def v_knot(self, index: typing.SupportsInt) -> float:
        """
        Return the V knot at index (1-based).
        """
    def v_knot_sequence(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Return V knots with repetitions as a numpy array.
        """
    def v_knots(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Return all distinct V knots as a numpy array.
        """
    def v_multiplicities(self) -> numpy.typing.NDArray[numpy.int32]:
        """
        Return all V knot multiplicities as a numpy array.
        """
    def v_multiplicity(self, index: typing.SupportsInt) -> int:
        """
        Return the multiplicity of V knot at index (1-based).
        """
    def weight(self, u_index: typing.SupportsInt, v_index: typing.SupportsInt) -> float:
        """
        Return the weight at (u_index, v_index). Indices are 1-based.
        """
    def weights(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Return all weights as a numpy array of shape [nb_u_poles, nb_v_poles].
        """
    @property
    def first_u_knot_index(self) -> int:
        """
        Index of the first U knot (start of parameter range).
        """
    @property
    def first_v_knot_index(self) -> int:
        """
        Index of the first V knot (start of parameter range).
        """
    @property
    def is_u_rational(self) -> bool:
        """
        True if the surface is rational in the U direction.
        """
    @property
    def is_v_rational(self) -> bool:
        """
        True if the surface is rational in the V direction.
        """
    @property
    def last_u_knot_index(self) -> int:
        """
        Index of the last U knot (end of parameter range).
        """
    @property
    def last_v_knot_index(self) -> int:
        """
        Index of the last V knot (end of parameter range).
        """
    @property
    def nb_u_knots(self) -> int:
        """
        The number of distinct knots in the U direction.
        """
    @property
    def nb_u_poles(self) -> int:
        """
        The number of poles in the U direction.
        """
    @property
    def nb_v_knots(self) -> int:
        """
        The number of distinct knots in the V direction.
        """
    @property
    def nb_v_poles(self) -> int:
        """
        The number of poles in the V direction.
        """
    @property
    def u_degree(self) -> int:
        """
        The degree in the U parametric direction.
        """
    @property
    def u_knot_distribution(self) -> GeomAbs_BSplKnotDistribution:
        """
        Knot distribution type in U: Uniform, QuasiUniform, PiecewiseBezier, or NonUniform.
        """
    @property
    def v_degree(self) -> int:
        """
        The degree in the V parametric direction.
        """
    @property
    def v_knot_distribution(self) -> GeomAbs_BSplKnotDistribution:
        """
        Knot distribution type in V: Uniform, QuasiUniform, PiecewiseBezier, or NonUniform.
        """
class BezierCurve(BoundedCurve):
    @staticmethod
    def max_degree() -> int:
        """
        Get maximum allowed degree
        """
    @typing.overload
    def __init__(self, poles: typing.Any) -> None:
        """
        Create a non-rational Bezier curve with control points. Poles can be list of gp_Pnt or numpy array of shape (n,3).
        """
    @typing.overload
    def __init__(self, poles: typing.Any, weights: typing.Any) -> None:
        """
        Create a rational Bezier curve with control points and weights. Both can be lists or numpy arrays.
        """
    def increase(self, degree: typing.SupportsInt) -> None:
        """
        Increases the degree of the Bezier curve
        """
    @typing.overload
    def insert_pole_after(self, index: typing.SupportsInt, P: mod3d.gp.Pnt) -> None:
        """
        Insert pole after given index
        """
    @typing.overload
    def insert_pole_after(self, index: typing.SupportsInt, P: mod3d.gp.Pnt, weight: typing.SupportsFloat) -> None:
        """
        Insert pole with weight after given index
        """
    @typing.overload
    def insert_pole_before(self, index: typing.SupportsInt, P: mod3d.gp.Pnt) -> None:
        """
        Insert pole before given index
        """
    @typing.overload
    def insert_pole_before(self, index: typing.SupportsInt, P: mod3d.gp.Pnt, weight: typing.SupportsFloat) -> None:
        """
        Insert pole with weight before given index
        """
    def is_cn(self, N: typing.SupportsInt) -> bool:
        """
        Check if curve has CN continuity
        """
    def pole(self, index: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
        Get pole at given index (1-based)
        """
    def remove_pole(self, index: typing.SupportsInt) -> None:
        """
        Remove pole at given index
        """
    def resolution(self, Tolerance3D: typing.SupportsFloat) -> float:
        """
        Compute parametric tolerance for given 3D tolerance
        """
    def segment(self, U1: typing.SupportsFloat, U2: typing.SupportsFloat) -> None:
        """
        Segment the curve between parameters U1 and U2
        """
    @typing.overload
    def set_pole(self, index: typing.SupportsInt, P: mod3d.gp.Pnt) -> None:
        """
        Set pole at given index
        """
    @typing.overload
    def set_pole(self, index: typing.SupportsInt, P: mod3d.gp.Pnt, weight: typing.SupportsFloat) -> None:
        """
        Set pole and weight at given index
        """
    def set_weight(self, index: typing.SupportsInt, weight: typing.SupportsFloat) -> None:
        """
        Set weight at given index
        """
    def weight(self, index: typing.SupportsInt) -> float:
        """
        Get weight at given index (1-based)
        """
    @property
    def continuity(self) -> mod3d.GeomAbs.Shape:
        """
        Get continuity of the curve
        """
    @property
    def degree(self) -> int:
        """
        Get the degree of the Bezier curve
        """
    @property
    def is_rational(self) -> bool:
        """
        Check if curve is rational
        """
    @property
    def nb_poles(self) -> int:
        """
        Get number of poles
        """
    @property
    def poles(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Get all poles as numpy array of shape (n,3)
        """
    @property
    def weights(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Get all weights as numpy array
        """
class BezierSurface(BoundedSurface):
    """
    
            A rational or non-rational Bezier surface.
    
            A non-rational Bezier surface is defined by a table of poles (control points).
            A rational Bezier surface is defined by poles with varying associated weights.
    
            Key characteristics:
            - Parameter range is [0, 1] in both U and V directions
            - Degree = number of poles - 1 in each direction (max 25)
            - Infinite continuity (C∞) in both parametric directions
            - Corner poles are the four parametric corners of the surface
            - First/last rows and columns define boundary Bezier curves
    
            The poles form a rectangular network that shapes the surface. If all weights
            are equal, the surface is polynomial (non-rational).
            
    """
    max_degree: typing.ClassVar[int] = 25
    @typing.overload
    def __init__(self, poles: typing.Annotated[numpy.typing.ArrayLike, numpy.float64]) -> None:
        """
                    Create a non-rational Bezier surface from a 2D array of poles.
        
                    Args:
                        poles: 2D array of 3D points with shape [nu, nv, 3].
                               nu and nv must be >= 2 and <= MaxDegree + 1.
        
                    The surface degrees will be (nu-1) in U and (nv-1) in V.
        """
    @typing.overload
    def __init__(self, poles: typing.Annotated[numpy.typing.ArrayLike, numpy.float64], weights: typing.Annotated[numpy.typing.ArrayLike, numpy.float64]) -> None:
        """
                    Create a rational Bezier surface from poles and weights.
        
                    Args:
                        poles: 2D array of 3D points with shape [nu, nv, 3].
                        weights: 2D array of weights with shape [nu, nv].
                                 All weights must be positive.
        
                    If all weights are identical, the surface is treated as non-rational.
        """
    def exchange_uv(self) -> None:
        """
        Exchange U and V parametric directions (transposes poles/weights).
        """
    def increase(self, u_degree: typing.SupportsInt, v_degree: typing.SupportsInt) -> None:
        """
                    Increase the degree to (u_degree, v_degree).
        
                    The new degrees must be >= current degrees and <= max_degree.
                    The surface shape is preserved.
        """
    def pole(self, u_index: typing.SupportsInt, v_index: typing.SupportsInt) -> mod3d.gp.Pnt:
        """
                    Return the pole at indices (u_index, v_index).
        
                    Args:
                        u_index: 1-based index in U direction (1 to nb_u_poles).
                        v_index: 1-based index in V direction (1 to nb_v_poles).
        
                    Returns:
                        gp.Pnt: The control point.
        """
    def poles(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Return all poles as a numpy array of shape [nb_u_poles, nb_v_poles, 3].
        """
    def segment(self, u1: typing.SupportsFloat, u2: typing.SupportsFloat, v1: typing.SupportsFloat, v2: typing.SupportsFloat) -> None:
        """
                    Segment the surface to the parameter range [u1, u2] x [v1, v2].
        
                    The poles are modified so the new parameter range becomes [0, 1] x [0, 1].
        """
    @typing.overload
    def set_pole(self, u_index: typing.SupportsInt, v_index: typing.SupportsInt, p: mod3d.gp.Pnt) -> None:
        """
        Set the pole at (u_index, v_index) to point p.
        """
    @typing.overload
    def set_pole(self, u_index: typing.SupportsInt, v_index: typing.SupportsInt, p: mod3d.gp.Pnt, weight: typing.SupportsFloat) -> None:
        """
        Set the pole and weight at (u_index, v_index).
        """
    def set_weight(self, u_index: typing.SupportsInt, v_index: typing.SupportsInt, weight: typing.SupportsFloat) -> None:
        """
        Set the weight at (u_index, v_index).
        """
    def weight(self, u_index: typing.SupportsInt, v_index: typing.SupportsInt) -> float:
        """
                    Return the weight at indices (u_index, v_index).
        
                    Args:
                        u_index: 1-based index in U direction.
                        v_index: 1-based index in V direction.
        
                    Returns:
                        float: The weight value (1.0 for non-rational surfaces).
        """
    def weights(self) -> numpy.typing.NDArray[numpy.float64]:
        """
        Return all weights as a numpy array of shape [nb_u_poles, nb_v_poles].
        """
    @property
    def is_u_rational(self) -> bool:
        """
        True if the surface is rational in the U direction.
        """
    @property
    def is_v_rational(self) -> bool:
        """
        True if the surface is rational in the V direction.
        """
    @property
    def nb_u_poles(self) -> int:
        """
        The number of poles in the U direction.
        """
    @property
    def nb_v_poles(self) -> int:
        """
        The number of poles in the V direction.
        """
    @property
    def u_degree(self) -> int:
        """
        The degree in the U parametric direction.
        """
    @property
    def v_degree(self) -> int:
        """
        The degree in the V parametric direction.
        """
class BoundedCurve(Curve):
    @property
    def end_point(self) -> mod3d.gp.Pnt:
        ...
    @property
    def start_point(self) -> mod3d.gp.Pnt:
        ...
class BoundedSurface(Surface):
    """
    
            Abstract base class for bounded surfaces in 3D space.
    
            A bounded surface is defined by a finite rectangle in its 2D parametric space:
            - U parameter ranges between u0 ("First u parameter") and u1 ("Last u parameter")
            - V parameter ranges between v0 ("First v parameter") and v1 ("Last v parameter")
    
            The surface is limited by four boundary curves:
            - u0 and u1 isoparametric curves in the U direction
            - v0 and v1 isoparametric curves in the V direction
    
            A bounded surface is always finite.
    
            Concrete implementations:
            - BezierSurface: Bezier polynomial surface
            - BSplineSurface: B-spline surface
            - RectangularTrimmedSurface: Surface trimmed by isoparametric curves
            
    """
class Circle(Conic):
    """
    Describes a circle in 3D space.
    A circle is defined by its radius and positioned with a right-handed coordinate system:
    - origin is the center of the circle
    - origin, X Direction and Y Direction define the plane of the circle
    - main Direction is the vector normal to the plane
    Parameterization: P(U) = O + R*Cos(U)*XDir + R*Sin(U)*YDir
    where U is the parameter (angle), O is origin, R is radius.
    Parameter range: [0, 2*Pi), period = 2*Pi
    """
    @typing.overload
    def __init__(self, c: mod3d.gp.Circ) -> None:
        """
        Constructs a circle by conversion of the gp_Circ circle
        """
    @typing.overload
    def __init__(self, a2: mod3d.gp.Ax2, radius: typing.SupportsFloat) -> None:
        """
        Constructs a circle of given radius, where a2 locates the circle:
        - center is the origin of a2
        - origin, X Direction and Y Direction of a2 define the plane
        - a2 is the local coordinate system of the circle
        Note: Radius can be 0.0. Raises exception if radius < 0
        """
    @property
    def circ(self) -> mod3d.gp.Circ:
        """
        Get or set the gp_Circ with the same geometric properties
        """
    @circ.setter
    def circ(self, arg1: mod3d.gp.Circ) -> None:
        ...
    @property
    def radius(self) -> float:
        """
        Get or set the radius of the circle
        """
    @radius.setter
    def radius(self, arg1: typing.SupportsFloat) -> None:
        ...
class Conic(Curve):
    """
    Abstract base class for conic curves in 3D space.
    Defines common behavior for Circle, Ellipse, Hyperbola and Parabola.
    A conic is positioned with a right-handed coordinate system (gp_Ax2):
    - origin is the center (or apex for parabola)
    - origin, X Direction and Y Direction define the plane of the conic
    - main Direction is the vector normal to the plane
    """
    @property
    def axis(self) -> mod3d.gp.Ax1:
        """
        Get or set the main Axis of this conic (normal to the plane)
        """
    @axis.setter
    def axis(self, arg1: mod3d.gp.Ax1) -> None:
        ...
    @property
    def eccentricity(self) -> float:
        """
        Returns the eccentricity value of the conic:
        e = 0 for a circle
        0 < e < 1 for an ellipse (e = 0 if MajorRadius = MinorRadius)
        e > 1 for a hyperbola
        e = 1 for a parabola
        Raises exception for hyperbola if major radius is null
        """
    @property
    def location(self) -> mod3d.gp.Pnt:
        """
        Get or set the location point of the conic
        """
    @location.setter
    def location(self, arg1: mod3d.gp.Pnt) -> None:
        ...
    @property
    def position(self) -> mod3d.gp.Ax2:
        """
        Get or set the local coordinate system of the conic
        """
    @position.setter
    def position(self, arg1: mod3d.gp.Ax2) -> None:
        ...
    @property
    def x_axis(self) -> mod3d.gp.Ax1:
        """
        Returns the XAxis of the conic (defines origin of parametrization)
        """
    @property
    def y_axis(self) -> mod3d.gp.Ax1:
        """
        Returns the YAxis of the conic (perpendicular to XAxis)
        """
class ConicalSurface(ElementarySurface):
    """
    
            Describes a cone in 3D space.
    
            A cone is defined by:
            - A coordinate system (gp_Ax3) whose Z direction is the axis of symmetry
            - A semi-angle (can be negative)
            - A reference radius (radius at the reference plane Z=0)
    
            The parametric equation is:
                P(u, v) = O + (RefRadius + v*sin(Ang))*(cos(u)*XDir + sin(u)*YDir) + v*cos(Ang)*ZDir
    
            where:
            - O is the origin of the coordinate system
            - Ang is the semi-angle
            - RefRadius is the reference radius
            - u is in [0, 2*PI], v is in (-∞, +∞)
    
            The apex is located at distance RefRadius/tan(Ang) from the origin along ZDir.
            
    """
    @typing.overload
    def __init__(self, a3: mod3d.gp.Ax3, angle: typing.SupportsFloat, radius: typing.SupportsFloat) -> None:
        """
                    Creates a conical surface.
        
                    Parameters
                    ----------
                    a3 : gp_Ax3
                        The coordinate system. Z direction is the axis of symmetry.
                    angle : float
                        The semi-angle in radians. Absolute value must be in (0, PI/2).
                    radius : float
                        The reference radius at the placement plane (must be >= 0).
                        If radius = 0, the origin is at the apex.
        
                    Raises
                    ------
                    Standard_ConstructionError
                        If radius < 0 or |angle| is not in (0, PI/2)
        """
    @typing.overload
    def __init__(self, c: mod3d.gp.Cone) -> None:
        """
                    Creates a conical surface from a gp_Cone.
        
                    Parameters
                    ----------
                    c : gp_Cone
                        The cone definition
        """
    def coefficients(self) -> tuple:
        """
                    Returns the coefficients of the implicit quadric equation.
        
                    Returns
                    -------
                    tuple[float, ...]
                        Coefficients (A1, A2, A3, B1, B2, B3, C1, C2, C3, D)
        """
    @property
    def apex(self) -> mod3d.gp.Pnt:
        """
                    The apex of the cone.
        
                    The apex is on the negative side of the axis if the semi-angle is positive,
                    and on the positive side if the semi-angle is negative.
        
                    Returns
                    -------
                    gp_Pnt
                        The apex point
        """
    @property
    def cone(self) -> mod3d.gp.Cone:
        """
                    The non-persistent cone representation.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    gp_Cone
                        The gp_Cone representation
        """
    @cone.setter
    def cone(self, arg1: mod3d.gp.Cone) -> None:
        ...
    @property
    def ref_radius(self) -> float:
        """
                    The reference radius (at the reference plane Z=0).
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    float
                        The reference radius (must be >= 0)
        """
    @ref_radius.setter
    def ref_radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def semi_angle(self) -> float:
        """
                    The semi-angle at the apex. Can be negative.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    float
                        The semi-angle in radians. Absolute value must be in (0, PI/2).
        """
    @semi_angle.setter
    def semi_angle(self, arg1: typing.SupportsFloat) -> None:
        ...
class Curve(Geometry):
    @staticmethod
    @typing.overload
    def __init__(*args, **kwargs) -> None:
        """
        Create a Geom_Curve from a gbs::BSCurve object (implicit conversion).
        """
    @staticmethod
    @typing.overload
    def __init__(*args, **kwargs) -> None:
        """
        Create a Geom_Curve from a gbs::BSCurveRational object (implicit conversion).
        """
    def center_of_curvature(self, u: typing.SupportsFloat) -> mod3d.gp.Pnt:
        """
        Returns the center of curvature at parameter U
        """
    def curvature(self, u: typing.SupportsFloat) -> float:
        """
        Returns (curvature, first derivative of curvature) at parameter U
        """
    def d0(self, u: typing.SupportsFloat) -> mod3d.gp.Pnt:
        ...
    def d1(self, u: typing.SupportsFloat) -> tuple:
        ...
    def d2(self, u: typing.SupportsFloat) -> tuple:
        ...
    def d3(self, u: typing.SupportsFloat) -> tuple:
        ...
    def dn(self, u: typing.SupportsFloat, n: typing.SupportsInt) -> mod3d.gp.Vec:
        ...
    def is_cn(self, n: typing.SupportsInt) -> bool:
        ...
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
    def normal(self, u: typing.SupportsFloat) -> mod3d.gp.Dir:
        """
        Returns the normal vector at parameter U
        """
    def parametric_transformation(self, t: mod3d.gp.Trsf) -> float:
        ...
    def reverse(self) -> None:
        ...
    def reversed(self) -> Curve:
        ...
    def reversed_parameter(self, u: typing.SupportsFloat) -> float:
        ...
    def tangent(self, u: typing.SupportsFloat) -> mod3d.gp.Dir:
        """
        Returns the tangent vector at parameter U
        """
    def transformed_parameter(self, u: typing.SupportsFloat, t: mod3d.gp.Trsf) -> float:
        ...
    def value(self, u: typing.SupportsFloat) -> mod3d.gp.Pnt:
        ...
    @property
    def bounds(self) -> tuple:
        ...
    @property
    def continuity(self) -> mod3d.GeomAbs.Shape:
        ...
    @property
    def first_parameter(self) -> float:
        ...
    @property
    def is_closed(self) -> bool:
        ...
    @property
    def is_periodic(self) -> bool:
        ...
    @property
    def last_parameter(self) -> float:
        ...
    @property
    def period(self) -> float:
        ...
class CylindricalSurface(ElementarySurface):
    """
    
            Describes a cylinder in 3D space.
    
            A cylinder is defined by:
            - A coordinate system (gp_Ax3) whose Z direction is the axis of symmetry
            - A radius
    
            The parametric equation is:
                P(u, v) = O + R*(cos(u)*XDir + sin(u)*YDir) + v*ZDir
    
            where:
            - O is the origin of the coordinate system
            - R is the radius
            - XDir, YDir, ZDir are the axis directions
            - u is in [0, 2*PI], v is in (-∞, +∞)
    
            The cylinder is infinite in the V direction.
            
    """
    @typing.overload
    def __init__(self, a3: mod3d.gp.Ax3, radius: typing.SupportsFloat) -> None:
        """
                    Creates a cylindrical surface.
        
                    Parameters
                    ----------
                    a3 : gp_Ax3
                        The coordinate system. Z direction is the axis of symmetry.
                    radius : float
                        The cylinder radius (must be >= 0)
        
                    Raises
                    ------
                    Standard_ConstructionError
                        If radius < 0
        """
    @typing.overload
    def __init__(self, c: mod3d.gp.Cylinder) -> None:
        """
                    Creates a cylindrical surface from a gp_Cylinder.
        
                    Parameters
                    ----------
                    c : gp_Cylinder
                        The cylinder definition
        """
    def coefficients(self) -> tuple:
        """
                    Returns the coefficients of the implicit quadric equation.
        
                    The equation is:
                    A1*X² + A2*Y² + A3*Z² + 2*(B1*X*Y + B2*X*Z + B3*Y*Z) + 2*(C1*X + C2*Y + C3*Z) + D = 0
        
                    The coefficients are normalized.
        
                    Returns
                    -------
                    tuple[float, ...]
                        Coefficients (A1, A2, A3, B1, B2, B3, C1, C2, C3, D)
        """
    @property
    def cylinder(self) -> mod3d.gp.Cylinder:
        """
                    The non-persistent cylinder representation.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    gp_Cylinder
                        The gp_Cylinder representation
        """
    @cylinder.setter
    def cylinder(self, arg1: mod3d.gp.Cylinder) -> None:
        ...
    @property
    def radius(self) -> float:
        """
                    The radius of this cylinder.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    float
                        The cylinder radius (must be >= 0)
        """
    @radius.setter
    def radius(self, arg1: typing.SupportsFloat) -> None:
        ...
class ElementarySurface(Surface):
    """
    
            Abstract base class for elementary surfaces in 3D space.
    
            Elementary surfaces are defined by simple geometric shapes such as:
            - Planes
            - Cylinders
            - Cones
            - Spheres
            - Tori
    
            These surfaces have well-known mathematical representations and properties.
            All elementary surfaces are positioned in space by a coordinate system (gp_Ax3).
            
    """
    @property
    def axis(self) -> mod3d.gp.Ax1:
        """
                    The main axis (Z-axis) of the local coordinate system.
        
                    This defines the axis of symmetry for the surface. Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    gp_Ax1
                        The main axis of symmetry for the surface
        """
    @axis.setter
    def axis(self, arg1: mod3d.gp.Ax1) -> None:
        ...
    @property
    def location(self) -> mod3d.gp.Pnt:
        """
                    The location (origin) of the local coordinate system.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    gp_Pnt
                        The origin point of the local coordinate system
        """
    @location.setter
    def location(self, arg1: mod3d.gp.Pnt) -> None:
        ...
    @property
    def position(self) -> mod3d.gp.Ax3:
        """
                    The local coordinate system of the surface.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    gp_Ax3
                        The complete coordinate system (origin, X, Y, Z axes)
        """
    @position.setter
    def position(self, arg1: mod3d.gp.Ax3) -> None:
        ...
class Ellipse(Conic):
    """
    Describes an ellipse in 3D space.
    An ellipse is defined by its major and minor radii and positioned with a
    right-handed coordinate system where:
    - origin is the center of the ellipse
    - X Direction defines the major axis
    - Y Direction defines the minor axis
    Parameterization: P(U) = O + MajorRad*Cos(U)*XDir + MinorRad*Sin(U)*YDir
    Parameter range: [0, 2*Pi), period = 2*Pi
    """
    @typing.overload
    def __init__(self, e: mod3d.gp.Elips) -> None:
        """
        Constructs an ellipse by conversion of the gp_Elips ellipse
        """
    @typing.overload
    def __init__(self, a2: mod3d.gp.Ax2, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat) -> None:
        """
        Constructs an ellipse with given major and minor radii, where a2 locates the ellipse:
        - center is the origin of a2
        - X Direction defines the major axis
        - Y Direction defines the minor axis
        Raises exception if major_radius < minor_radius or minor_radius < 0
        """
    @property
    def directrix1(self) -> mod3d.gp.Ax1:
        """
        Returns the first directrix of the ellipse.
        This directrix is normal to the XAxis at distance d = MajorRadius / e from center,
        where e is the eccentricity. Parallel to YAxis, on positive side of XAxis.
        Raises exception if eccentricity = 0 (ellipse degenerates into circle)
        """
    @property
    def directrix2(self) -> mod3d.gp.Ax1:
        """
        Returns the second directrix (symmetrical transformation of directrix1 w.r.t. YAxis)
        """
    @property
    def elips(self) -> mod3d.gp.Elips:
        """
        Get or set the gp_Elips with the same geometric properties
        """
    @elips.setter
    def elips(self, arg1: mod3d.gp.Elips) -> None:
        ...
    @property
    def focal(self) -> float:
        """
        Computes the focal distance (distance between the two foci)
        """
    @property
    def focus1(self) -> mod3d.gp.Pnt:
        """
        Returns the first focus of the ellipse (on positive side of XAxis)
        """
    @property
    def focus2(self) -> mod3d.gp.Pnt:
        """
        Returns the second focus of the ellipse (on negative side of XAxis)
        """
    @property
    def major_radius(self) -> float:
        """
        Get or set the major radius of the ellipse
        """
    @major_radius.setter
    def major_radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def minor_radius(self) -> float:
        """
        Get or set the minor radius of the ellipse
        """
    @minor_radius.setter
    def minor_radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def parameter(self) -> float:
        """
        Returns p = (1 - e²) * MajorRadius where e is the eccentricity
        """
class Geometry:
    def __eq__(self, other: Geometry) -> bool:
        """
        Returns True if both refer to the same geometry object
        """
    def __hash__(self) -> int:
        """
        Returns hash value based on object identity
        """
    def copy(self) -> Geometry:
        ...
    def dump_json(self, stream: ..., std: ..., depth: typing.SupportsInt = -1) -> None:
        ...
    @typing.overload
    def mirror(self, p: mod3d.gp.Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: mod3d.gp.Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: mod3d.gp.Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: mod3d.gp.Pnt) -> Geometry:
        ...
    @typing.overload
    def mirrored(self, a1: mod3d.gp.Ax1) -> Geometry:
        ...
    @typing.overload
    def mirrored(self, a2: mod3d.gp.Ax2) -> Geometry:
        ...
    def rotate(self, a1: mod3d.gp.Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: mod3d.gp.Ax1, ang: typing.SupportsFloat) -> Geometry:
        ...
    def scale(self, p: mod3d.gp.Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: mod3d.gp.Pnt, s: typing.SupportsFloat) -> Geometry:
        ...
    def transform(self, t: mod3d.gp.Trsf) -> None:
        ...
    def transformed(self, t: mod3d.gp.Trsf) -> Geometry:
        ...
    @typing.overload
    def translate(self, v: mod3d.gp.Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: mod3d.gp.Pnt, p2: mod3d.gp.Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: mod3d.gp.Vec) -> Geometry:
        ...
    @typing.overload
    def translated(self, p1: mod3d.gp.Pnt, p2: mod3d.gp.Pnt) -> Geometry:
        ...
class Hyperbola(Conic):
    """
    Describes a branch of a hyperbola in 3D space.
    A hyperbola is defined by its major and minor radii and positioned with a
    right-handed coordinate system where:
    - origin is the center of the hyperbola
    - X Direction defines the major axis
    - Y Direction defines the minor axis
    The branch described is on the positive side of the major axis.
    Parameterization: P(U) = O + MajRad*Cosh(U)*XDir + MinRad*Sinh(U)*YDir
    Parameter range: ]-infinite, +infinite[
    """
    @typing.overload
    def __init__(self, h: mod3d.gp.Hypr) -> None:
        """
        Constructs a hyperbola by conversion of the gp_Hypr hyperbola
        """
    @typing.overload
    def __init__(self, a2: mod3d.gp.Ax2, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat) -> None:
        """
        Constructs a hyperbola with given major and minor radii, where a2 locates the hyperbola:
        - center is the origin of a2
        - X Direction defines the major axis
        - Y Direction defines the minor axis
        Raises exception if major_radius < 0 or minor_radius < 0
        """
    @property
    def asymptote1(self) -> mod3d.gp.Ax1:
        """
        Returns the first asymptote of the hyperbola (equation: Y = (B/A)*X).
        Raises exception if MajorRadius = 0
        """
    @property
    def asymptote2(self) -> mod3d.gp.Ax1:
        """
        Returns the second asymptote of the hyperbola
        """
    @property
    def conjugate_branch1(self) -> mod3d.gp.Hypr:
        """
        Returns the conjugate branch on the positive side of the YAxis
        """
    @property
    def conjugate_branch2(self) -> mod3d.gp.Hypr:
        """
        Returns the conjugate branch on the negative side of the YAxis
        """
    @property
    def directrix1(self) -> mod3d.gp.Ax1:
        """
        Returns the first directrix of the hyperbola
        """
    @property
    def directrix2(self) -> mod3d.gp.Ax1:
        """
        Returns the second directrix of the hyperbola
        """
    @property
    def focal(self) -> float:
        """
        Computes the focal distance (distance between the two foci)
        """
    @property
    def focus1(self) -> mod3d.gp.Pnt:
        """
        Returns the first focus of the hyperbola (on positive side of XAxis)
        """
    @property
    def focus2(self) -> mod3d.gp.Pnt:
        """
        Returns the second focus of the hyperbola (on negative side of XAxis)
        """
    @property
    def hypr(self) -> mod3d.gp.Hypr:
        """
        Get or set the gp_Hypr with the same geometric properties
        """
    @hypr.setter
    def hypr(self, arg1: mod3d.gp.Hypr) -> None:
        ...
    @property
    def major_radius(self) -> float:
        """
        Get or set the major radius of the hyperbola
        """
    @major_radius.setter
    def major_radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def minor_radius(self) -> float:
        """
        Get or set the minor radius of the hyperbola
        """
    @minor_radius.setter
    def minor_radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def other_branch(self) -> mod3d.gp.Hypr:
        """
        Returns the other branch of this hyperbola
        """
    @property
    def parameter(self) -> float:
        """
        Returns p = (e² - 1) * MajorRadius where e is the eccentricity
        """
class Line(Curve):
    direction: mod3d.gp.Dir
    line: mod3d.gp.Lin
    position: mod3d.gp.Ax1
    @typing.overload
    def __init__(self, a1: mod3d.gp.Ax1) -> None:
        ...
    @typing.overload
    def __init__(self, line: mod3d.gp.Lin) -> None:
        ...
    @typing.overload
    def __init__(self, p: mod3d.gp.Pnt, v: mod3d.gp.Dir) -> None:
        ...
class OffsetCurve(Curve):
    """
    Describes an offset curve in 3D space.
    An offset curve is a curve at constant distance (Offset) from a basis curve
    in a reference direction V. The offset curve takes its parametrization from
    the basis curve. The offset direction is N = T ^ V / ||T ^ V||, where T is
    the first derivative of the basis curve.
    Formula: Value(U) = BasisCurve.Value(U) + (Offset * (T ^ V)) / ||T ^ V||
    Note: The offset curve can be self-intersecting even if the basis curve is not.
    Continuity is one degree less than the basis curve.
    """
    def __init__(self, basis_curve: Curve, offset: typing.SupportsFloat, direction: mod3d.gp.Dir, is_not_check_c0: bool = False) -> None:
        """
        Creates an offset curve from a basis curve.
        basis_curve: The basis curve (must be at least C1)
        offset: Distance from the basis curve (positive or negative)
        direction: Fixed reference direction V for offset computation
        is_not_check_c0: If True, skip C0 continuity check
        Raises exception if basis curve is not at least C1.
        Warning: No check is done that ||T^V|| != 0 at any point
        """
    def get_basis_curve_continuity(self) -> mod3d.GeomAbs.Shape:
        """
        Returns the continuity of the basis curve
        """
    def set_basis_curve(self, basis_curve: Curve, is_not_check_c0: bool = False) -> None:
        """
        Changes the basis curve of this offset curve.
        Raises exception if the curve is not at least C1 continuous
        """
    @property
    def basis_curve(self) -> Curve:
        """
        Basis curve of this offset curve (can itself be an offset curve)
        """
    @property
    def direction(self) -> mod3d.gp.Dir:
        """
        Reference direction used to compute the offset direction
        """
    @direction.setter
    def direction(self, arg1: mod3d.gp.Dir) -> None:
        ...
    @property
    def offset(self) -> float:
        """
        Offset distance from the basis curve
        """
    @offset.setter
    def offset(self, arg1: typing.SupportsFloat) -> None:
        ...
class OffsetSurface(Surface):
    """
    
            Represents a surface offset from a base surface by a constant distance.
    
            An offset surface is defined by:
            - A base surface (Geom_Surface)
            - An offset distance (can be positive or negative)
    
            The offset surface is generated by moving each point on the base surface
            along its normal direction by the specified offset distance.
    
            The normal vector is computed as the cross product of the tangent vectors
            in U and V directions. The offset is measured along this normal.
    
            Warning: Offset surfaces may have self-intersections or singularities
            depending on the geometry of the base surface and the offset distance.
            
    """
    def __init__(self, base_surface: Surface, offset: typing.SupportsFloat) -> None:
        """
                     Create an offset surface from a base surface and offset distance.
        
                     Args:
                         base_surface (Geom_Surface): The base surface to offset from.
                         offset (float): The offset distance (positive or negative).
        
                     Raises:
                         Standard_ConstructionError: If the base surface is not at least C1 continuous.
        """
    def is_cn_u(self, n: typing.SupportsInt) -> bool:
        """
                    Check if the offset surface has Cn continuity in the U direction.
        
                    Args:
                        n: The order of continuity to check (n >= 0).
        
                    Returns:
                        bool: True if the surface is at least Cn continuous in U.
        """
    def is_cn_v(self, n: typing.SupportsInt) -> bool:
        """
                    Check if the offset surface has Cn continuity in the V direction.
        
                    Args:
                        n: The order of continuity to check (n >= 0).
        
                    Returns:
                        bool: True if the surface is at least Cn continuous in V.
        """
    def surface(self) -> Surface:
        """
                    Return an equivalent surface if possible.
        
                    If this offset surface can be represented as an equivalent canonical
                    surface (e.g., offset of a plane becomes another plane), returns that
                    surface. Otherwise returns this offset surface.
        
                    Returns:
                        Geom_Surface: An equivalent surface or this offset surface.
        """
    def transform(self, transform: mod3d.gp.Trsf) -> None:
        """
                    Apply a geometric transformation to this offset surface.
        
                    Modifies both the offset surface and its base surface in-place.
        
                    Args:
                        transform (gp.Trsf): The transformation to apply.
        """
    def u_osculating_surface(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> tuple:
        """
                    Get the local osculating surface along U at point (U, V).
        
                    The osculating surface has the same derivatives as the offset surface
                    in the U direction at the given point.
        
                    Args:
                        u: The U parameter.
                        v: The V parameter.
        
                    Returns:
                        tuple: (exists, is_opposite, osculating_surface)
                               where exists=True if an osculating surface exists,
                               is_opposite=True if derivatives point in opposite directions,
                               and osculating_surface is the B-spline osculating surface.
        """
    def v_osculating_surface(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> tuple:
        """
                    Get the local osculating surface along V at point (U, V).
        
                    The osculating surface has the same derivatives as the offset surface
                    in the V direction at the given point.
        
                    Args:
                        u: The U parameter.
                        v: The V parameter.
        
                    Returns:
                        tuple: (exists, is_opposite, osculating_surface)
        """
    @property
    def base_surface(self) -> Surface:
        """
                              The base surface from which this offset surface is derived.
        
                              Can be both read and modified. When modified, the continuity
                              of the base surface must be at least C1.
        
                              Returns/Sets
                              -----------
                              Geom_Surface
                                  The base surface of the offset surface
        """
    @base_surface.setter
    def base_surface(self, arg1: Surface) -> None:
        ...
    @property
    def basis_surface_continuity(self) -> mod3d.GeomAbs.Shape:
        """
                                      The continuity of the base surface.
        
                                      Returns
                                      -------
                                      GeomAbs_Shape
                                          The continuity level of the base surface
        """
    @property
    def continuity(self) -> mod3d.GeomAbs.Shape:
        """
                                      The continuity of this offset surface.
        
                                      Returns the continuity of the base surface minus 1.
                                      For example, if the base is C2, this offset is C1.
        
                                      Returns
                                      -------
                                      GeomAbs_Shape
                                          The continuity level
        """
    @property
    def offset(self) -> float:
        """
                              The offset distance from the base surface.
        
                              Can be both read and modified. Positive values offset in the
                              direction of the normal, negative values in the opposite direction.
        
                              Returns/Sets
                              -----------
                              float
                                  The offset distance
        """
    @offset.setter
    def offset(self, arg1: typing.SupportsFloat) -> None:
        ...
class Parabola(Conic):
    """
    Describes a parabola in 3D space.
    A parabola is defined by its focal length and positioned with a
    right-handed coordinate system where:
    - origin is the apex of the parabola
    - X Axis defines the axis of symmetry (parabola on positive side)
    - Y Direction is parallel to the directrix
    Parameterization: P(U) = O + U²/(4*F)*XDir + U*YDir
    where F is the focal length. Parameter range: ]-infinite, +infinite[
    """
    @typing.overload
    def __init__(self, prb: mod3d.gp.Parab) -> None:
        """
        Constructs a parabola by conversion of the gp_Parab parabola
        """
    @typing.overload
    def __init__(self, a2: mod3d.gp.Ax2, focal: typing.SupportsFloat) -> None:
        """
        Constructs a parabola with its local coordinate system a2 and focal length.
        The XDirection of a2 defines the axis of symmetry.
        The YDirection is parallel to the directrix.
        The Location point of a2 is the vertex (apex) of the parabola.
        Raises exception if focal < 0
        """
    @typing.overload
    def __init__(self, d: mod3d.gp.Ax1, f: mod3d.gp.Pnt) -> None:
        """
        Constructs a parabola from directrix d and focus point f.
        The symmetry axis (XAxis) is normal to the directrix and passes through focus,
        but its location point is the vertex of the parabola.
        The YAxis is parallel to d with location at the vertex
        """
    @property
    def directrix(self) -> mod3d.gp.Ax1:
        """
        Returns the directrix of this parabola.
        This is a line normal to the axis of symmetry, in the plane of the parabola,
        on the negative side of its axis, at distance from apex equal to focal length.
        Origin is located on the X Axis of this parabola
        """
    @property
    def focal(self) -> float:
        """
        Get or set the focal distance
        """
    @focal.setter
    def focal(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def focus(self) -> mod3d.gp.Pnt:
        """
        Returns the focus of this parabola (on positive side of X Axis)
        """
    @property
    def parab(self) -> mod3d.gp.Parab:
        """
        Get or set the gp_Parab with the same geometric properties
        """
    @parab.setter
    def parab(self, arg1: mod3d.gp.Parab) -> None:
        ...
    @property
    def parameter(self) -> float:
        """
        Returns the parameter of the parabola (distance between focus and directrix)
        """
class Plane(ElementarySurface):
    """
    
            Describes a plane in 3D space.
    
            A plane is defined by a coordinate system (gp_Ax3) where:
            - The origin is a point on the plane
            - The "Z Direction" is the normal to the plane
            - The "X Direction" and "Y Direction" define the U and V parametric directions
    
            The parametric equation is:
                P(u, v) = O + u*XDir + v*YDir
    
            where O is the origin, XDir and YDir are the X and Y directions.
    
            The plane is infinite in both U and V directions.
            
    """
    @typing.overload
    def __init__(self, a3: mod3d.gp.Ax3) -> None:
        """
                    Creates a plane from a coordinate system.
        
                    Parameters
                    ----------
                    a3 : gp_Ax3
                        The coordinate system defining the plane. The Z direction is
                        normal to the plane.
        """
    @typing.overload
    def __init__(self, pl: mod3d.gp.Pln) -> None:
        """
                    Creates a plane from a gp_Pln.
        
                    Parameters
                    ----------
                    pl : gp_Pln
                        The plane definition
        """
    @typing.overload
    def __init__(self, point: mod3d.gp.Pnt, normal: mod3d.gp.Dir) -> None:
        """
                    Creates a plane passing through a point with given normal.
        
                    Parameters
                    ----------
                    point : gp_Pnt
                        A point on the plane
                    normal : gp_Dir
                        The normal direction to the plane
        """
    @property
    def coefficients(self) -> tuple:
        """
                    The coefficients of the plane equation: Ax + By + Cz + D = 0.
        
                    The coefficients are normalized (A² + B² + C² = 1).
        
                    Returns
                    -------
                    tuple[float, float, float, float]
                        Coefficients (A, B, C, D)
        """
    @property
    def pln(self) -> mod3d.gp.Pln:
        """
                    The non-persistent plane representation.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    gp_Pln
                        The gp_Pln representation
        """
    @pln.setter
    def pln(self, arg1: mod3d.gp.Pln) -> None:
        ...
class RectangularTrimmedSurface(BoundedSurface):
    """
    
            A surface trimmed by isoparametric curves.
    
            Describes a portion of a surface limited by parameter ranges:
            - [u1, u2] in the U direction
            - [v1, v2] in the V direction
    
            The trimmed surface is built from a copy of the basis surface, so
            modifications to the basis surface don't affect the trimmed surface.
    
            The trimmed surface may not have the same orientation as the basis surface.
            
    """
    @typing.overload
    def __init__(self, surface: Surface, u1: typing.SupportsFloat, u2: typing.SupportsFloat, v1: typing.SupportsFloat, v2: typing.SupportsFloat, u_sense: bool = True, v_sense: bool = True) -> None:
        """
                    Create a rectangular trimmed surface.
        
                    Args:
                        surface: The basis surface to trim.
                        u1, u2: Parameter range in U direction.
                        v1, v2: Parameter range in V direction.
                        u_sense: If True, U direction goes from u1 to u2.
                        v_sense: If True, V direction goes from v1 to v2.
        
                    For periodic surfaces, u_sense and v_sense control which part is used.
        """
    @typing.overload
    def __init__(self, surface: Surface, param1: typing.SupportsFloat, param2: typing.SupportsFloat, u_trim: bool, sense: bool = True) -> None:
        """
                    Create a surface trimmed in one direction only.
        
                    Args:
                        surface: The basis surface to trim.
                        param1, param2: Parameter range for trimming.
                        u_trim: If True, trim in U direction; else trim in V.
                        sense: Direction sense for periodic surfaces.
        """
    @typing.overload
    def set_trim(self, u1: typing.SupportsFloat, u2: typing.SupportsFloat, v1: typing.SupportsFloat, v2: typing.SupportsFloat, u_sense: bool = True, v_sense: bool = True) -> None:
        """
        Modify the trim values in both directions.
        """
    @typing.overload
    def set_trim(self, param1: typing.SupportsFloat, param2: typing.SupportsFloat, u_trim: bool, sense: bool = True) -> None:
        """
        Modify the trim values in one direction only.
        """
    @property
    def basis_surface(self) -> Surface:
        """
        The underlying basis surface.
        """
class SphericalSurface(ElementarySurface):
    """
    
            Describes a sphere in 3D space.
    
            A sphere is defined by:
            - A coordinate system (gp_Ax3) whose origin is the center
            - A radius
    
            The parametric equation is:
                P(u, v) = O + R*sin(v)*ZDir + R*cos(v)*(cos(u)*XDir + sin(u)*YDir)
    
            where:
            - O is the center (origin of the coordinate system)
            - R is the radius
            - u is in [0, 2*PI] (longitude), v is in [-PI/2, PI/2] (latitude)
    
            The "north pole" is at v = PI/2, the "south pole" is at v = -PI/2.
            
    """
    @typing.overload
    def __init__(self, a3: mod3d.gp.Ax3, radius: typing.SupportsFloat) -> None:
        """
                    Creates a spherical surface.
        
                    Parameters
                    ----------
                    a3 : gp_Ax3
                        The coordinate system. Origin is the center of the sphere.
                    radius : float
                        The sphere radius (must be >= 0)
        
                    Raises
                    ------
                    Standard_ConstructionError
                        If radius < 0
        """
    @typing.overload
    def __init__(self, s: mod3d.gp.Sphere) -> None:
        """
                    Creates a spherical surface from a gp_Sphere.
        
                    Parameters
                    ----------
                    s : gp_Sphere
                        The sphere definition
        """
    def coefficients(self) -> tuple:
        """
                    Returns the coefficients of the implicit quadric equation.
        
                    Returns
                    -------
                    tuple[float, ...]
                        Coefficients (A1, A2, A3, B1, B2, B3, C1, C2, C3, D)
        """
    @property
    def area(self) -> float:
        """
                    Computes the surface area of the sphere.
        
                    Area = 4 * PI * R²
        
                    Returns
                    -------
                    float
                        The surface area
        """
    @property
    def radius(self) -> float:
        """
                    The radius of this sphere.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    float
                        The sphere radius (must be >= 0)
        """
    @radius.setter
    def radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def sphere(self) -> mod3d.gp.Sphere:
        """
                    The non-persistent sphere representation.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    gp_Sphere
                        The gp_Sphere representation
        """
    @sphere.setter
    def sphere(self, arg1: mod3d.gp.Sphere) -> None:
        ...
    @property
    def volume(self) -> float:
        """
                    Computes the volume of the sphere.
        
                    Volume = 4/3 * PI * R³
        
                    Returns
                    -------
                    float
                        The volume
        """
class Surface(Geometry):
    """
    
            Abstract base class for all surfaces in 3D space.
    
            Geom_Surface describes the common behavior of surfaces in 3D space.
            Surfaces are parameterized by (U, V) parameters.
    
            Key characteristics:
            - All surfaces must have at least C0 geometric continuity
            - Surfaces can be closed and/or periodic in U and/or V directions
            - Provides methods for point evaluation, derivatives, and iso-curves
    
            Derived classes include planes, cylinders, cones, spheres, tori,
            surfaces of linear extrusion, surfaces of revolution, Bezier and
            BSpline surfaces.
            
    """
    @staticmethod
    @typing.overload
    def __init__(*args, **kwargs) -> None:
        """
        Create a Geom_Surface from a gbs::BSSurface object (implicit conversion).
        """
    @staticmethod
    @typing.overload
    def __init__(*args, **kwargs) -> None:
        """
        Create a Geom_Surface from a gbs::BSSurfaceRational object (implicit conversion).
        """
    def __call__(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> mod3d.gp.Pnt:
        """
                    Evaluate the surface at parameters (U, V).
        
                    Allows using the surface as a callable: surface(u, v).
        
                    Args:
                        u: The U parameter.
                        v: The V parameter.
        
                    Returns:
                        gp.Pnt: The 3D point on the surface.
        """
    def bounds(self) -> tuple:
        """
                    Return the parametric bounds of this surface.
        
                    Returns:
                        tuple: (u1, u2, v1, v2) parametric bounds.
                               For infinite surfaces, values may be very large.
        """
    def d0(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> mod3d.gp.Pnt:
        """
                    Compute the point at parameters (U, V).
        
                    Equivalent to value(u, v).
        
                    Args:
                        u: The U parameter.
                        v: The V parameter.
        
                    Returns:
                        gp.Pnt: The 3D point on the surface.
        """
    def d1(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> tuple:
        """
                    Compute the point and first derivatives at (U, V).
        
                    Requires C1 continuity.
        
                    Args:
                        u: The U parameter.
                        v: The V parameter.
        
                    Returns:
                        tuple: (point, d1u, d1v) where d1u and d1v are the first
                               partial derivatives in U and V directions.
        """
    def d2(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> tuple:
        """
                    Compute the point, first and second derivatives at (U, V).
        
                    Requires C2 continuity.
        
                    Args:
                        u: The U parameter.
                        v: The V parameter.
        
                    Returns:
                        tuple: (point, d1u, d1v, d2u, d2v, d2uv) containing the point,
                               first derivatives, second derivatives, and mixed derivative.
        """
    def d3(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> tuple:
        """
                    Compute the point and derivatives up to third order at (U, V).
        
                    Requires C3 continuity.
        
                    Args:
                        u: The U parameter.
                        v: The V parameter.
        
                    Returns:
                        tuple: (point, d1u, d1v, d2u, d2v, d2uv, d3u, d3v, d3uuv, d3uvv)
                               containing all derivatives up to order 3.
        """
    def dn(self, u: typing.SupportsFloat, v: typing.SupportsFloat, nu: typing.SupportsInt, nv: typing.SupportsInt) -> mod3d.gp.Vec:
        """
                    Compute a specific partial derivative at (U, V).
        
                    Returns the derivative of order Nu in U and Nv in V.
                    Requires CNu continuity in U and CNv continuity in V.
        
                    Args:
                        u: The U parameter.
                        v: The V parameter.
                        nu: The derivative order in U (nu >= 0).
                        nv: The derivative order in V (nv >= 0).
        
                    Returns:
                        gp.Vec: The partial derivative vector.
        
                    Raises:
                        Standard_RangeError: If nu + nv < 1 or nu < 0 or nv < 0.
        """
    def is_cn_u(self, n: typing.SupportsInt) -> bool:
        """
                    Check if the surface has Cn continuity in the U direction.
        
                    Args:
                        n: The order of continuity to check (n >= 0).
        
                    Returns:
                        bool: True if the surface is at least Cn continuous in U.
        """
    def is_cn_v(self, n: typing.SupportsInt) -> bool:
        """
                    Check if the surface has Cn continuity in the V direction.
        
                    Args:
                        n: The order of continuity to check (n >= 0).
        
                    Returns:
                        bool: True if the surface is at least Cn continuous in V.
        """
    def parametric_transformation(self, transform: mod3d.gp.Trsf) -> gp_GTrsf2d:
        """
                    Return a 2D transformation for parameter space.
        
                    Returns a 2D transformation that maps (U, V) parameters on this
                    surface to (U', V') parameters on the transformed surface.
        
                    Args:
                        transform: The 3D geometric transformation.
        
                    Returns:
                        gp.GTrsf2d: The 2D parameter transformation.
        """
    def transform_parameters(self, u: typing.SupportsFloat, v: typing.SupportsFloat, transform: mod3d.gp.Trsf) -> tuple:
        """
                    Compute parameters on a transformed surface.
        
                    For a point P(U, V) on this surface, returns the parameters (U', V')
                    such that transformed_surface.Value(U', V') == P(U, V).Transformed(T).
        
                    Args:
                        u: The U parameter on this surface.
                        v: The V parameter on this surface.
                        transform: The geometric transformation.
        
                    Returns:
                        tuple: (u', v') parameters on the transformed surface.
        """
    def u_iso(self, u: typing.SupportsFloat) -> Curve:
        """
                    Compute the U isoparametric curve.
        
                    Returns the curve at constant U parameter.
        
                    Args:
                        u: The U parameter value.
        
                    Returns:
                        Curve: The isoparametric curve in the V direction at U.
        """
    def u_reverse(self) -> None:
        """
                    Reverse the U direction of parametrization in-place.
        
                    The bounds of the surface are not modified.
        """
    def u_reversed(self) -> Surface:
        """
                    Return a copy of this surface with U direction reversed.
        
                    The bounds of the surface are not modified.
        
                    Returns:
                        Surface: A new surface with reversed U parametrization.
        """
    def u_reversed_parameter(self, u: typing.SupportsFloat) -> float:
        """
                    Compute the U parameter on the U-reversed surface.
        
                    For a point at parameter U on this surface, returns the corresponding
                    parameter on the U-reversed surface.
        
                    Args:
                        u: The U parameter on this surface.
        
                    Returns:
                        float: The corresponding U parameter on the reversed surface.
        """
    def v_iso(self, v: typing.SupportsFloat) -> Curve:
        """
                    Compute the V isoparametric curve.
        
                    Returns the curve at constant V parameter.
        
                    Args:
                        v: The V parameter value.
        
                    Returns:
                        Curve: The isoparametric curve in the U direction at V.
        """
    def v_reverse(self) -> None:
        """
                    Reverse the V direction of parametrization in-place.
        
                    The bounds of the surface are not modified.
        """
    def v_reversed(self) -> Surface:
        """
                    Return a copy of this surface with V direction reversed.
        
                    The bounds of the surface are not modified.
        
                    Returns:
                        Surface: A new surface with reversed V parametrization.
        """
    def v_reversed_parameter(self, v: typing.SupportsFloat) -> float:
        """
                    Compute the V parameter on the V-reversed surface.
        
                    For a point at parameter V on this surface, returns the corresponding
                    parameter on the V-reversed surface.
        
                    Args:
                        v: The V parameter on this surface.
        
                    Returns:
                        float: The corresponding V parameter on the reversed surface.
        """
    def value(self, u: typing.SupportsFloat, v: typing.SupportsFloat) -> mod3d.gp.Pnt:
        """
                    Compute the point at parameters (U, V).
        
                    Args:
                        u: The U parameter.
                        v: The V parameter.
        
                    Returns:
                        gp.Pnt: The 3D point on the surface.
        """
    @property
    def continuity(self) -> mod3d.GeomAbs.Shape:
        """
                    The global continuity of the surface.
        
                    Returns the minimum continuity between U and V directions.
                    Possible values: C0, C1, C2, C3, CN, G1, G2.
        """
    @property
    def is_u_closed(self) -> bool:
        """
                    Check if the surface is closed in the U direction.
        
                    A surface is closed in U if for each V, the points at U_first and
                    U_last are coincident within tolerance.
        """
    @property
    def is_u_periodic(self) -> bool:
        """
                    Check if the surface is periodic in the U direction.
        
                    A surface is U-periodic if it is U-closed and P(U, V) = P(U + T, V)
                    for some period T.
        """
    @property
    def is_v_closed(self) -> bool:
        """
                    Check if the surface is closed in the V direction.
        
                    A surface is closed in V if for each U, the points at V_first and
                    V_last are coincident within tolerance.
        """
    @property
    def is_v_periodic(self) -> bool:
        """
                    Check if the surface is periodic in the V direction.
        
                    A surface is V-periodic if it is V-closed and P(U, V) = P(U, V + T)
                    for some period T.
        """
    @property
    def u_period(self) -> typing.Any:
        """
                    The period in the U direction, or None if not U-periodic.
        """
    @property
    def v_period(self) -> typing.Any:
        """
                    The period in the V direction, or None if not V-periodic.
        """
class SurfaceOfLinearExtrusion(SweptSurface):
    """
    Describes a surface of linear extrusion.
    A surface of linear extrusion is created by extruding a curve along a direction.
    """
    def __init__(self, basis_curve: Curve, direction: mod3d.gp.Dir) -> None:
        """
        Constructor:
          basis_curve: The curve to extrude
          direction: The extrusion direction
        """
class SurfaceOfRevolution(SweptSurface):
    """
    Describes a surface of revolution.
    A surface of revolution is created by rotating a curve about an axis.
    """
    def __init__(self, basis_curve: Curve, axis: mod3d.gp.Ax1) -> None:
        """
        Constructor:
          basis_curve: The curve to revolve
          axis: The axis of revolution
        """
    @property
    def axis(self) -> mod3d.gp.Ax1:
        """
        Returns the axis of revolution.
        """
class SweptSurface(Surface):
    """
    Abstract base class for surfaces created by sweeping a curve with another curve.
    Concrete derived classes are SurfaceOfRevolution and SurfaceOfLinearExtrusion.
    """
    @property
    def basis_curve(self) -> Curve:
        """
        Returns the referenced curve of the surface.
        For a surface of revolution it is the revolution curve,
        for a surface of linear extrusion it is the extruded curve.
        """
    @property
    def direction(self) -> mod3d.gp.Dir:
        """
        Returns the reference direction of the swept surface.
        For a surface of revolution it is the direction of the revolution axis,
        for a surface of linear extrusion it is the direction of extrusion.
        """
class ToroidalSurface(ElementarySurface):
    """
    
            Describes a torus in 3D space.
    
            A torus is defined by:
            - A coordinate system (gp_Ax3) whose origin is the center
            - A major radius (from center to tube center)
            - A minor radius (tube radius)
    
            The parametric equation is:
                P(u, v) = O + (R + r*cos(v))*(cos(u)*XDir + sin(u)*YDir) + r*sin(v)*ZDir
    
            where:
            - O is the center (origin of the coordinate system)
            - R is the major radius
            - r is the minor radius
            - u is in [0, 2*PI] (around the axis), v is in [0, 2*PI] (around the tube)
    
            Note: It is valid to create a torus with R = 0 (spindle torus) or
            r = 0 (degenerate torus).
            
    """
    @typing.overload
    def __init__(self, a3: mod3d.gp.Ax3, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat) -> None:
        """
                    Creates a toroidal surface.
        
                    Parameters
                    ----------
                    a3 : gp_Ax3
                        The coordinate system. Origin is the center of the torus.
                    major_radius : float
                        The major radius (must be >= 0)
                    minor_radius : float
                        The minor radius (must be >= 0)
        
                    Raises
                    ------
                    Standard_ConstructionError
                        If major_radius < 0 or minor_radius < 0
        """
    @typing.overload
    def __init__(self, t: mod3d.gp.Torus) -> None:
        """
                    Creates a toroidal surface from a gp_Torus.
        
                    Parameters
                    ----------
                    t : gp_Torus
                        The torus definition
        """
    @property
    def area(self) -> float:
        """
                    Computes the surface area of the torus.
        
                    Area = 4 * PI² * R * r
        
                    where R is the major radius and r is the minor radius.
        
                    Returns
                    -------
                    float
                        The surface area
        """
    @property
    def major_radius(self) -> float:
        """
                    The major radius of this torus.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    float
                        The major radius (must be >= 0)
        """
    @major_radius.setter
    def major_radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def minor_radius(self) -> float:
        """
                    The minor radius of this torus.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    float
                        The minor radius (must be >= 0)
        """
    @minor_radius.setter
    def minor_radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def torus(self) -> mod3d.gp.Torus:
        """
                    The non-persistent torus representation.
        
                    Can be both read and modified.
        
                    Returns/Sets
                    -----------
                    gp_Torus
                        The gp_Torus representation
        """
    @torus.setter
    def torus(self, arg1: mod3d.gp.Torus) -> None:
        ...
    @property
    def volume(self) -> float:
        """
                    Computes the volume of the torus.
        
                    Volume = 2 * PI² * R * r²
        
                    where R is the major radius and r is the minor radius.
        
                    Returns
                    -------
                    float
                        The volume
        """
class TrimmedCurve(BoundedCurve):
    def __init__(self, c: Curve, u1: typing.SupportsFloat, u2: typing.SupportsFloat, sense: bool = True, adjust_periodic: bool = True) -> None:
        ...
    def set_trim(self, u1: typing.SupportsFloat, u2: typing.SupportsFloat, sense: bool = True, adjust_periodic: bool = True) -> None:
        ...
    @property
    def basis_curve(self) -> Curve:
        ...
