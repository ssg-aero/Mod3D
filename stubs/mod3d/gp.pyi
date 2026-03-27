"""
Geometric primitives
"""
from __future__ import annotations
import typing
__all__: list[str] = ['Ax1', 'Ax1Mirror', 'Ax2', 'Ax22d', 'Ax2Mirror', 'Ax2d', 'Ax3', 'Circ', 'Circ2d', 'CompoundTrsf', 'Cone', 'Cylinder', 'Dir', 'Dir2d', 'Elips', 'Elips2d', 'EulerAngles', 'EulerSequence', 'Extrinsic_XYX', 'Extrinsic_XYZ', 'Extrinsic_XZX', 'Extrinsic_XZY', 'Extrinsic_YXY', 'Extrinsic_YXZ', 'Extrinsic_YZX', 'Extrinsic_YZY', 'Extrinsic_ZXY', 'Extrinsic_ZXZ', 'Extrinsic_ZYX', 'Extrinsic_ZYZ', 'Hypr', 'Hypr2d', 'Identity', 'Intrinsic_XYX', 'Intrinsic_XYZ', 'Intrinsic_XZX', 'Intrinsic_XZY', 'Intrinsic_YXY', 'Intrinsic_YXZ', 'Intrinsic_YZX', 'Intrinsic_YZY', 'Intrinsic_ZXY', 'Intrinsic_ZXZ', 'Intrinsic_ZYX', 'Intrinsic_ZYZ', 'Lin', 'Lin2d', 'Mat', 'Other', 'Parab', 'Parab2d', 'Pln', 'Pnt', 'Pnt2d', 'PntMirror', 'Quaternion', 'Rotation', 'Scale', 'Sphere', 'Torus', 'Translation', 'Trsf', 'Trsf2d', 'TrsfForm', 'Vec', 'Vec2d', 'XY', 'XYZ', 'YawPitchRoll', 'gp']
class Ax1:
    direction: Dir
    location: Pnt
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, p: Pnt, v: Dir) -> None:
        ...
    def __repr__(self) -> str:
        ...
    def angle(self, other: Ax1) -> float:
        ...
    def is_coaxial(self, other: Ax1, angular_tolerance: typing.SupportsFloat, linear_tolerance: typing.SupportsFloat) -> bool:
        ...
    def is_normal(self, other: Ax1, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    def is_opposite(self, other: Ax1, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    def is_parallel(self, other: Ax1, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: gp_Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Ax1:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Ax1:
        ...
    @typing.overload
    def mirrored(self, a2: gp_Ax2) -> Ax1:
        ...
    def reverse(self) -> None:
        ...
    def reversed(self) -> Ax1:
        ...
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Ax1:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Ax1:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Ax1:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Ax1:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Ax1:
        ...
class Ax2:
    axis: Ax1
    direction: Dir
    location: Pnt
    x_direction: Dir
    y_direction: Dir
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, p: Pnt, v: Dir) -> None:
        ...
    @typing.overload
    def __init__(self, p: Pnt, n: Dir, vx: Dir) -> None:
        ...
    def __repr__(self) -> str:
        ...
    def angle(self, other: Ax2) -> float:
        ...
    @typing.overload
    def is_coplanar(self, other: Ax2, linear_tolerance: typing.SupportsFloat, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    @typing.overload
    def is_coplanar(self, a1: Ax1, linear_tolerance: typing.SupportsFloat, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Ax2:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Ax2:
        ...
    @typing.overload
    def mirrored(self, a2: Ax2) -> Ax2:
        ...
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Ax2:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Ax2:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Ax2:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Ax2:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Ax2:
        ...
class Ax22d:
    """
    Describes a coordinate system in 2D: an origin and two perpendicular axes (X and Y).
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates a coordinate system at origin with X=(1,0), Y=(0,1)
        """
    @typing.overload
    def __init__(self, p: Pnt2d, vx: Dir2d, vy: Dir2d) -> None:
        """
        Creates a coordinate system at P with X direction Vx and Y direction Vy
        """
    @typing.overload
    def __init__(self, p: Pnt2d, v: Dir2d, sense: bool = True) -> None:
        """
        Creates a coordinate system at P with X direction V.
        Y is computed perpendicular (counter-clockwise if sense=True)
        """
    @typing.overload
    def __init__(self, a: Ax2d, sense: bool = True) -> None:
        """
        Creates a coordinate system from axis A
        """
    def __repr__(self) -> str:
        ...
    def location(self) -> Pnt2d:
        """
        Returns the origin
        """
    @typing.overload
    def mirror(self, p: Pnt2d) -> None:
        """
        Mirrors about point P
        """
    @typing.overload
    def mirror(self, a: Ax2d) -> None:
        """
        Mirrors about axis A
        """
    @typing.overload
    def mirrored(self, p: Pnt2d) -> Ax22d:
        """
        Returns a mirrored copy
        """
    @typing.overload
    def mirrored(self, a: Ax2d) -> Ax22d:
        """
        Returns a mirrored copy
        """
    def rotate(self, p: Pnt2d, angle: typing.SupportsFloat) -> None:
        """
        Rotates about point P
        """
    def rotated(self, p: Pnt2d, angle: typing.SupportsFloat) -> Ax22d:
        """
        Returns a rotated copy
        """
    def scale(self, p: Pnt2d, s: typing.SupportsFloat) -> None:
        """
        Scales about point P
        """
    def scaled(self, p: Pnt2d, s: typing.SupportsFloat) -> Ax22d:
        """
        Returns a scaled copy
        """
    def set_location(self, p: Pnt2d) -> None:
        """
        Sets the origin
        """
    def set_x_axis(self, a: Ax2d) -> None:
        """
        Sets the X axis
        """
    def set_x_direction(self, vx: Dir2d) -> None:
        """
        Sets the X direction
        """
    def set_y_axis(self, a: Ax2d) -> None:
        """
        Sets the Y axis
        """
    def set_y_direction(self, vy: Dir2d) -> None:
        """
        Sets the Y direction
        """
    def transform(self, t: gp_Trsf2d) -> None:
        """
        Transforms this coordinate system
        """
    def transformed(self, t: gp_Trsf2d) -> Ax22d:
        """
        Returns a transformed copy
        """
    def translate(self, v: Vec2d) -> None:
        """
        Translates by vector V
        """
    def translated(self, v: Vec2d) -> Ax22d:
        """
        Returns a translated copy
        """
    def x_axis(self) -> Ax2d:
        """
        Returns the X axis
        """
    def x_direction(self) -> Dir2d:
        """
        Returns the X direction
        """
    def y_axis(self) -> Ax2d:
        """
        Returns the Y axis
        """
    def y_direction(self) -> Dir2d:
        """
        Returns the Y direction
        """
class Ax2d:
    """
    Describes an axis in 2D space: a point (location) and a direction.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an axis at origin with direction (1, 0)
        """
    @typing.overload
    def __init__(self, p: Pnt2d, v: Dir2d) -> None:
        """
        Creates an axis at point P with direction V
        """
    def __repr__(self) -> str:
        ...
    def angle(self, other: Ax2d) -> float:
        """
        Returns the angle between directions
        """
    def is_coaxial(self, other: Ax2d, angular_tolerance: typing.SupportsFloat, linear_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if axes are coaxial within tolerances
        """
    def is_normal(self, other: Ax2d, angular_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if axes are normal (perpendicular)
        """
    def is_opposite(self, other: Ax2d, angular_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if axes are opposite
        """
    def is_parallel(self, other: Ax2d, angular_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if axes are parallel
        """
    @typing.overload
    def mirror(self, p: Pnt2d) -> None:
        """
        Mirrors about point P
        """
    @typing.overload
    def mirror(self, a: Ax2d) -> None:
        """
        Mirrors about axis A
        """
    @typing.overload
    def mirrored(self, p: Pnt2d) -> Ax2d:
        """
        Returns a mirrored copy
        """
    @typing.overload
    def mirrored(self, a: Ax2d) -> Ax2d:
        """
        Returns a mirrored copy
        """
    def reverse(self) -> None:
        """
        Reverses the direction
        """
    def reversed(self) -> Ax2d:
        """
        Returns a reversed copy
        """
    def rotate(self, p: Pnt2d, angle: typing.SupportsFloat) -> None:
        """
        Rotates about point P by angle (radians)
        """
    def rotated(self, p: Pnt2d, angle: typing.SupportsFloat) -> Ax2d:
        """
        Returns a rotated copy
        """
    def scale(self, p: Pnt2d, s: typing.SupportsFloat) -> None:
        """
        Scales about point P by factor S
        """
    def scaled(self, p: Pnt2d, s: typing.SupportsFloat) -> Ax2d:
        """
        Returns a scaled copy
        """
    def transform(self, t: gp_Trsf2d) -> None:
        """
        Transforms this axis
        """
    def transformed(self, t: gp_Trsf2d) -> Ax2d:
        """
        Returns a transformed copy
        """
    def translate(self, v: Vec2d) -> None:
        """
        Translates by vector V
        """
    def translated(self, v: Vec2d) -> Ax2d:
        """
        Returns a translated copy
        """
    @property
    def direction(self) -> Dir2d:
        """
        Get or set the direction
        """
    @direction.setter
    def direction(self, arg1: Dir2d) -> None:
        ...
    @property
    def location(self) -> Pnt2d:
        """
        Get or set the location point
        """
    @location.setter
    def location(self, arg1: Pnt2d) -> None:
        ...
class Ax3:
    axis: Ax1
    direction: Dir
    location: Pnt
    x_direction: Dir
    y_direction: Dir
    z_direction: Dir
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, p: Pnt, v: Dir) -> None:
        ...
    @typing.overload
    def __init__(self, p: Pnt, n: Dir, vx: Dir) -> None:
        ...
    @typing.overload
    def __init__(self, a2: Ax2) -> None:
        ...
    def __repr__(self) -> str:
        ...
    def angle(self, other: Ax3) -> float:
        ...
    def as_ax2(self) -> Ax2:
        ...
    @typing.overload
    def is_coplanar(self, other: Ax3, linear_tolerance: typing.SupportsFloat, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    @typing.overload
    def is_coplanar(self, a1: Ax1, linear_tolerance: typing.SupportsFloat, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Ax3:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Ax3:
        ...
    @typing.overload
    def mirrored(self, a2: Ax2) -> Ax3:
        ...
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Ax3:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Ax3:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Ax3:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Ax3:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Ax3:
        ...
class Circ:
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an indefinite circle
        """
    @typing.overload
    def __init__(self, a2: Ax2, radius: typing.SupportsFloat) -> None:
        """
        Constructs a circle with a2 locating the circle and giving its orientation:
        - center is the origin of a2
        - origin, X Direction and Y Direction of a2 define the plane
        Note: Radius can be 0.0. Raises exception if radius < 0
        """
    def __repr__(self) -> str:
        ...
    def area(self) -> float:
        """
        Computes the area of the circle (π * r²)
        """
    def contains(self, p: Pnt, linear_tolerance: typing.SupportsFloat) -> bool:
        """
        Returns True if point p is on the circumference within the given tolerance
        """
    def distance(self, p: Pnt) -> float:
        """
        Computes the minimum distance between point p and any point on the circumference
        """
    def length(self) -> float:
        """
        Computes the circumference of the circle (2π * r)
        """
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        """
        Performs symmetrical transformation with respect to point p
        """
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        """
        Performs symmetrical transformation with respect to axis a1
        """
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        """
        Performs symmetrical transformation with respect to plane defined by a2
        """
    @typing.overload
    def mirrored(self, p: Pnt) -> Circ:
        """
        Returns a mirrored copy with respect to point p
        """
    @typing.overload
    def mirrored(self, a1: Ax1) -> Circ:
        """
        Returns a mirrored copy with respect to axis a1
        """
    @typing.overload
    def mirrored(self, a2: Ax2) -> Circ:
        """
        Returns a mirrored copy with respect to plane defined by a2
        """
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        """
        Rotates the circle around axis a1 by angle ang (in radians)
        """
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Circ:
        """
        Returns a rotated copy around axis a1 by angle ang (in radians)
        """
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        """
        Scales the circle with center p and scale factor s.
        If s is negative, radius stays positive but XAxis and YAxis are reversed
        """
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Circ:
        """
        Returns a scaled copy with center p and scale factor s
        """
    def square_distance(self, p: Pnt) -> float:
        """
        Computes the square distance between the circle and point p
        """
    def transform(self, t: gp_Trsf) -> None:
        """
        Transforms the circle with the transformation t
        """
    def transformed(self, t: gp_Trsf) -> Circ:
        """
        Returns a transformed copy with transformation t
        """
    @typing.overload
    def translate(self, v: Vec) -> None:
        """
        Translates the circle in the direction of vector v
        """
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        """
        Translates the circle from point p1 to point p2
        """
    @typing.overload
    def translated(self, v: Vec) -> Circ:
        """
        Returns a translated copy in the direction of vector v
        """
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Circ:
        """
        Returns a translated copy from point p1 to point p2
        """
    @property
    def axis(self) -> Ax1:
        """
        Get or set the main axis of the circle (perpendicular to the plane, passing through center)
        """
    @axis.setter
    def axis(self, arg1: Ax1) -> None:
        ...
    @property
    def location(self) -> Pnt:
        """
        Get or set the center of the circle (Location point of the local coordinate system)
        """
    @location.setter
    def location(self, arg1: Pnt) -> None:
        ...
    @property
    def position(self) -> Ax2:
        """
        Get or set the position (local coordinate system) of the circle
        """
    @position.setter
    def position(self, arg1: Ax2) -> None:
        ...
    @property
    def radius(self) -> float:
        """
        Get or set the radius of this circle
        """
    @radius.setter
    def radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def x_axis(self) -> Ax1:
        """
        Returns the XAxis of the circle (perpendicular to main axis, defines parametrization origin)
        """
    @property
    def y_axis(self) -> Ax1:
        """
        Returns the YAxis of the circle (perpendicular to XAxis, completes the plane)
        """
class Circ2d:
    """
    Describes a circle in 2D space: a center point and a radius.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates a circle with center at origin and radius 1
        """
    @typing.overload
    def __init__(self, a: Ax2d, radius: typing.SupportsFloat, sense: bool = True) -> None:
        """
        Creates a circle with axis A (center at A.Location()) and radius
        """
    @typing.overload
    def __init__(self, a: Ax22d, radius: typing.SupportsFloat) -> None:
        """
        Creates a circle with coordinate system A and radius
        """
    def __repr__(self) -> str:
        ...
    def area(self) -> float:
        """
        Returns the area
        """
    def axis(self) -> Ax22d:
        """
        Returns the main axis
        """
    def contains(self, p: Pnt2d, linear_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if point P is on the circle within tolerance
        """
    def distance(self, p: Pnt2d) -> float:
        """
        Returns the distance to point P
        """
    def is_direct(self) -> bool:
        """
        Returns True if the coordinate system is direct (counter-clockwise)
        """
    def length(self) -> float:
        """
        Returns the circumference
        """
    def location(self) -> Pnt2d:
        """
        Returns the center
        """
    @typing.overload
    def mirror(self, p: Pnt2d) -> None:
        """
        Mirrors about point P
        """
    @typing.overload
    def mirror(self, a: Ax2d) -> None:
        """
        Mirrors about axis A
        """
    @typing.overload
    def mirrored(self, p: Pnt2d) -> Circ2d:
        """
        Returns a mirrored copy
        """
    @typing.overload
    def mirrored(self, a: Ax2d) -> Circ2d:
        """
        Returns a mirrored copy
        """
    def position(self) -> Ax22d:
        """
        Returns the coordinate system
        """
    def radius(self) -> float:
        """
        Returns the radius
        """
    def reverse(self) -> None:
        """
        Reverses the orientation
        """
    def reversed(self) -> Circ2d:
        """
        Returns a reversed copy
        """
    def rotate(self, p: Pnt2d, angle: typing.SupportsFloat) -> None:
        """
        Rotates about point P
        """
    def rotated(self, p: Pnt2d, angle: typing.SupportsFloat) -> Circ2d:
        """
        Returns a rotated copy
        """
    def scale(self, p: Pnt2d, s: typing.SupportsFloat) -> None:
        """
        Scales about point P
        """
    def scaled(self, p: Pnt2d, s: typing.SupportsFloat) -> Circ2d:
        """
        Returns a scaled copy
        """
    def set_axis(self, a: Ax22d) -> None:
        """
        Sets the main axis
        """
    def set_location(self, p: Pnt2d) -> None:
        """
        Sets the center
        """
    def set_radius(self, r: typing.SupportsFloat) -> None:
        """
        Sets the radius
        """
    def set_x_axis(self, a: Ax2d) -> None:
        """
        Sets the X axis
        """
    def set_y_axis(self, a: Ax2d) -> None:
        """
        Sets the Y axis
        """
    def square_distance(self, p: Pnt2d) -> float:
        """
        Returns the squared distance to point P
        """
    def transform(self, t: gp_Trsf2d) -> None:
        """
        Transforms this circle
        """
    def transformed(self, t: gp_Trsf2d) -> Circ2d:
        """
        Returns a transformed copy
        """
    def translate(self, v: Vec2d) -> None:
        """
        Translates by vector V
        """
    def translated(self, v: Vec2d) -> Circ2d:
        """
        Returns a translated copy
        """
    def x_axis(self) -> Ax2d:
        """
        Returns the X axis
        """
    def y_axis(self) -> Ax2d:
        """
        Returns the Y axis
        """
class Cone:
    axis: Ax1
    location: Pnt
    position: Ax3
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, a2: Ax2, semi_angle: typing.SupportsFloat, radius: typing.SupportsFloat) -> None:
        ...
    def __repr__(self) -> str:
        ...
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Cone:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Cone:
        ...
    @typing.overload
    def mirrored(self, a2: Ax2) -> Cone:
        ...
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Cone:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Cone:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Cone:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Cone:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Cone:
        ...
    @property
    def radius(self) -> float:
        ...
    @radius.setter
    def radius(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def semi_angle(self) -> float:
        ...
    @semi_angle.setter
    def semi_angle(self, arg1: typing.SupportsFloat) -> None:
        ...
class Cylinder:
    axis: Ax1
    location: Pnt
    position: Ax3
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, a2: Ax2, radius: typing.SupportsFloat) -> None:
        ...
    def __repr__(self) -> str:
        ...
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Cylinder:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Cylinder:
        ...
    @typing.overload
    def mirrored(self, a2: Ax2) -> Cylinder:
        ...
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Cylinder:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Cylinder:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Cylinder:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Cylinder:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Cylinder:
        ...
    @property
    def radius(self) -> float:
        ...
    @radius.setter
    def radius(self, arg1: typing.SupportsFloat) -> None:
        ...
class Dir:
    xyz: XYZ
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, v: Vec) -> None:
        ...
    @typing.overload
    def __init__(self, coord: XYZ) -> None:
        ...
    @typing.overload
    def __init__(self, xv: typing.SupportsFloat, yv: typing.SupportsFloat, zv: typing.SupportsFloat) -> None:
        ...
    def __mul__(self, arg0: Dir) -> float:
        ...
    def __neg__(self) -> Dir:
        ...
    def __repr__(self) -> str:
        ...
    def __xor__(self, arg0: Dir) -> Dir:
        ...
    def angle(self, other: Dir) -> float:
        ...
    def angle_with_ref(self, other: Dir, vref: Dir) -> float:
        ...
    @typing.overload
    def coord(self, index: typing.SupportsInt) -> float:
        ...
    @typing.overload
    def coord(self) -> tuple:
        ...
    def cross(self, right: Dir) -> None:
        ...
    def cross_cross(self, v1: Dir, v2: Dir) -> None:
        ...
    def cross_crossed(self, v1: Dir, v2: Dir) -> Dir:
        ...
    def crossed(self, right: Dir) -> Dir:
        ...
    def dot(self, other: Dir) -> float:
        ...
    def dot_cross(self, v1: Dir, v2: Dir) -> float:
        ...
    def is_equal(self, other: Dir, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    def is_normal(self, other: Dir, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    def is_opposite(self, other: Dir, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    def is_parallel(self, other: Dir, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    @typing.overload
    def mirror(self, v: Dir) -> None:
        ...
    @typing.overload
    def mirror(self, a1: gp_Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: gp_Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, v: Dir) -> Dir:
        ...
    @typing.overload
    def mirrored(self, a1: gp_Ax1) -> Dir:
        ...
    @typing.overload
    def mirrored(self, a2: gp_Ax2) -> Dir:
        ...
    def reverse(self) -> None:
        ...
    def reversed(self) -> Dir:
        ...
    def rotate(self, a1: gp_Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: gp_Ax1, ang: typing.SupportsFloat) -> Dir:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Dir:
        ...
    @property
    def x(self) -> float:
        ...
    @x.setter
    def x(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def y(self) -> float:
        ...
    @y.setter
    def y(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def z(self) -> float:
        ...
    @z.setter
    def z(self, arg1: typing.SupportsFloat) -> None:
        ...
class Dir2d:
    """
    Describes a unit vector (direction) in 2D space.
    Always normalized to have magnitude 1.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates direction (1, 0)
        """
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat) -> None:
        """
        Creates direction from components (will be normalized)
        """
    @typing.overload
    def __init__(self, coord: XY) -> None:
        """
        Creates direction from XY (will be normalized)
        """
    @typing.overload
    def __init__(self, v: Vec2d) -> None:
        """
        Creates direction from vector (will be normalized)
        """
    def __repr__(self) -> str:
        ...
    def angle(self, other: Dir2d) -> float:
        """
        Returns the angle to another direction (-PI to PI)
        """
    def crossed(self, other: Dir2d) -> float:
        """
        Returns the cross product (X1*Y2 - Y1*X2)
        """
    def dot(self, other: Dir2d) -> float:
        """
        Returns the dot product
        """
    def is_equal(self, other: Dir2d, angular_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests equality within angular tolerance
        """
    def is_normal(self, other: Dir2d, angular_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if directions are normal (perpendicular)
        """
    def is_opposite(self, other: Dir2d, angular_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if directions are opposite
        """
    def is_parallel(self, other: Dir2d, angular_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if directions are parallel
        """
    @typing.overload
    def mirror(self, v: Dir2d) -> None:
        """
        Mirrors about direction V
        """
    @typing.overload
    def mirror(self, a: gp_Ax2d) -> None:
        """
        Mirrors about axis A
        """
    @typing.overload
    def mirrored(self, v: Dir2d) -> Dir2d:
        """
        Returns a mirrored copy
        """
    @typing.overload
    def mirrored(self, a: gp_Ax2d) -> Dir2d:
        """
        Returns a mirrored copy
        """
    def reverse(self) -> None:
        """
        Reverses this direction
        """
    def reversed(self) -> Dir2d:
        """
        Returns a reversed copy
        """
    def rotate(self, angle: typing.SupportsFloat) -> None:
        """
        Rotates by angle (radians)
        """
    def rotated(self, angle: typing.SupportsFloat) -> Dir2d:
        """
        Returns a rotated copy
        """
    def transform(self, t: gp_Trsf2d) -> None:
        """
        Transforms this direction
        """
    def transformed(self, t: gp_Trsf2d) -> Dir2d:
        """
        Returns a transformed copy
        """
    @property
    def coord(self) -> XY:
        """
        Get or set the XY components (will be normalized on set)
        """
    @coord.setter
    def coord(self, arg1: XY) -> None:
        ...
    @property
    def x(self) -> float:
        """
        Get or set the X component (will renormalize on set)
        """
    @x.setter
    def x(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def xy(self) -> XY:
        """
        Get or set the XY components (will be normalized on set)
        """
    @xy.setter
    def xy(self, arg1: XY) -> None:
        ...
    @property
    def y(self) -> float:
        """
        Get or set the Y component (will renormalize on set)
        """
    @y.setter
    def y(self, arg1: typing.SupportsFloat) -> None:
        ...
class Elips:
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
    def __init__(self) -> None:
        """
        Creates an indefinite ellipse
        """
    @typing.overload
    def __init__(self, a2: Ax2, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat) -> None:
        """
        Constructs an ellipse with given major and minor radii, where a2 locates the ellipse:
        - center is the origin of a2
        - X Direction defines the major axis
        - Y Direction defines the minor axis
        Raises exception if major_radius < minor_radius or minor_radius < 0
        """
    def __repr__(self) -> str:
        ...
    def area(self) -> float:
        """
        Computes the area of the ellipse
        """
    def directrix1(self) -> Ax1:
        """
        Returns the first directrix of the ellipse
        """
    def directrix2(self) -> Ax1:
        """
        Returns the second directrix of the ellipse
        """
    def eccentricity(self) -> float:
        """
        Returns the eccentricity of the ellipse (0 < e < 1, or 0 if circle)
        """
    def focal(self) -> float:
        """
        Computes the focal distance (distance between the two foci)
        """
    def focus1(self) -> Pnt:
        """
        Returns the first focus (on positive side of major axis)
        """
    def focus2(self) -> Pnt:
        """
        Returns the second focus (on negative side of major axis)
        """
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Elips:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Elips:
        ...
    @typing.overload
    def mirrored(self, a2: Ax2) -> Elips:
        ...
    def parameter(self) -> float:
        """
        Returns p = (1 - e²) * MajorRadius where e is the eccentricity
        """
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Elips:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Elips:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Elips:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Elips:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Elips:
        ...
    @property
    def axis(self) -> Ax1:
        """
        Get or set the main axis of the ellipse (perpendicular to the plane, passing through center)
        """
    @axis.setter
    def axis(self, arg1: Ax1) -> None:
        ...
    @property
    def location(self) -> Pnt:
        """
        Get or set the center of the ellipse
        """
    @location.setter
    def location(self, arg1: Pnt) -> None:
        ...
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
    def position(self) -> Ax2:
        """
        Get or set the position (local coordinate system) of the ellipse
        """
    @position.setter
    def position(self, arg1: Ax2) -> None:
        ...
    @property
    def x_axis(self) -> Ax1:
        """
        Returns the major axis of the ellipse
        """
    @property
    def y_axis(self) -> Ax1:
        """
        Returns the minor axis of the ellipse
        """
class Elips2d:
    """
    Describes an ellipse in 2D space.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an ellipse
        """
    @typing.overload
    def __init__(self, a: Ax2d, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat, sense: bool = True) -> None:
        """
        Creates an ellipse with axis A and radii
        """
    @typing.overload
    def __init__(self, a: Ax22d, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat) -> None:
        """
        Creates an ellipse with coordinate system A and radii
        """
    def __repr__(self) -> str:
        ...
    def area(self) -> float:
        """
        Returns the area
        """
    def axis(self) -> Ax22d:
        """
        Returns the main axis
        """
    def directrix1(self) -> Ax2d:
        """
        Returns the first directrix
        """
    def directrix2(self) -> Ax2d:
        """
        Returns the second directrix
        """
    def eccentricity(self) -> float:
        """
        Returns the eccentricity
        """
    def focal(self) -> float:
        """
        Returns the focal distance
        """
    def focus1(self) -> Pnt2d:
        """
        Returns the first focus
        """
    def focus2(self) -> Pnt2d:
        """
        Returns the second focus
        """
    def is_direct(self) -> bool:
        """
        Returns True if direct orientation
        """
    def location(self) -> Pnt2d:
        """
        Returns the center
        """
    def major_radius(self) -> float:
        """
        Returns the major radius
        """
    def minor_radius(self) -> float:
        """
        Returns the minor radius
        """
    def parameter(self) -> float:
        """
        Returns the parameter
        """
    def reverse(self) -> None:
        """
        Reverses the orientation
        """
    def reversed(self) -> Elips2d:
        """
        Returns a reversed copy
        """
    def set_axis(self, a: Ax22d) -> None:
        """
        Sets the main axis
        """
    def set_location(self, p: Pnt2d) -> None:
        """
        Sets the center
        """
    def set_major_radius(self, r: typing.SupportsFloat) -> None:
        """
        Sets the major radius
        """
    def set_minor_radius(self, r: typing.SupportsFloat) -> None:
        """
        Sets the minor radius
        """
    def set_x_axis(self, a: Ax2d) -> None:
        """
        Sets the major axis
        """
    def set_y_axis(self, a: Ax2d) -> None:
        """
        Sets the minor axis
        """
    def x_axis(self) -> Ax2d:
        """
        Returns the major axis
        """
    def y_axis(self) -> Ax2d:
        """
        Returns the minor axis
        """
class EulerSequence:
    """
    Enumerates all 24 possible variants of generalized Euler angles.
    
    Members:
    
      EulerAngles : Classic Euler angles (Intrinsic ZXZ)
    
      YawPitchRoll : Yaw Pitch Roll (Intrinsic ZYX)
    
      Extrinsic_XYZ
    
      Extrinsic_XZY
    
      Extrinsic_YZX
    
      Extrinsic_YXZ
    
      Extrinsic_ZXY
    
      Extrinsic_ZYX
    
      Intrinsic_XYZ
    
      Intrinsic_XZY
    
      Intrinsic_YZX
    
      Intrinsic_YXZ
    
      Intrinsic_ZXY
    
      Intrinsic_ZYX
    
      Extrinsic_XYX
    
      Extrinsic_XZX
    
      Extrinsic_YZY
    
      Extrinsic_YXY
    
      Extrinsic_ZYZ
    
      Extrinsic_ZXZ
    
      Intrinsic_XYX
    
      Intrinsic_XZX
    
      Intrinsic_YZY
    
      Intrinsic_YXY
    
      Intrinsic_ZXZ
    
      Intrinsic_ZYZ
    """
    EulerAngles: typing.ClassVar[EulerSequence]  # value = <EulerSequence.EulerAngles: 0>
    Extrinsic_XYX: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_XYX: 14>
    Extrinsic_XYZ: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_XYZ: 2>
    Extrinsic_XZX: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_XZX: 15>
    Extrinsic_XZY: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_XZY: 3>
    Extrinsic_YXY: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_YXY: 17>
    Extrinsic_YXZ: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_YXZ: 5>
    Extrinsic_YZX: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_YZX: 4>
    Extrinsic_YZY: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_YZY: 16>
    Extrinsic_ZXY: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_ZXY: 6>
    Extrinsic_ZXZ: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_ZXZ: 19>
    Extrinsic_ZYX: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_ZYX: 7>
    Extrinsic_ZYZ: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Extrinsic_ZYZ: 18>
    Intrinsic_XYX: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_XYX: 20>
    Intrinsic_XYZ: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_XYZ: 8>
    Intrinsic_XZX: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_XZX: 21>
    Intrinsic_XZY: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_XZY: 9>
    Intrinsic_YXY: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_YXY: 23>
    Intrinsic_YXZ: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_YXZ: 11>
    Intrinsic_YZX: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_YZX: 10>
    Intrinsic_YZY: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_YZY: 22>
    Intrinsic_ZXY: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_ZXY: 12>
    Intrinsic_ZXZ: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_ZXZ: 24>
    Intrinsic_ZYX: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_ZYX: 13>
    Intrinsic_ZYZ: typing.ClassVar[EulerSequence]  # value = <EulerSequence.Intrinsic_ZYZ: 25>
    YawPitchRoll: typing.ClassVar[EulerSequence]  # value = <EulerSequence.YawPitchRoll: 1>
    __members__: typing.ClassVar[dict[str, EulerSequence]]  # value = {'EulerAngles': <EulerSequence.EulerAngles: 0>, 'YawPitchRoll': <EulerSequence.YawPitchRoll: 1>, 'Extrinsic_XYZ': <EulerSequence.Extrinsic_XYZ: 2>, 'Extrinsic_XZY': <EulerSequence.Extrinsic_XZY: 3>, 'Extrinsic_YZX': <EulerSequence.Extrinsic_YZX: 4>, 'Extrinsic_YXZ': <EulerSequence.Extrinsic_YXZ: 5>, 'Extrinsic_ZXY': <EulerSequence.Extrinsic_ZXY: 6>, 'Extrinsic_ZYX': <EulerSequence.Extrinsic_ZYX: 7>, 'Intrinsic_XYZ': <EulerSequence.Intrinsic_XYZ: 8>, 'Intrinsic_XZY': <EulerSequence.Intrinsic_XZY: 9>, 'Intrinsic_YZX': <EulerSequence.Intrinsic_YZX: 10>, 'Intrinsic_YXZ': <EulerSequence.Intrinsic_YXZ: 11>, 'Intrinsic_ZXY': <EulerSequence.Intrinsic_ZXY: 12>, 'Intrinsic_ZYX': <EulerSequence.Intrinsic_ZYX: 13>, 'Extrinsic_XYX': <EulerSequence.Extrinsic_XYX: 14>, 'Extrinsic_XZX': <EulerSequence.Extrinsic_XZX: 15>, 'Extrinsic_YZY': <EulerSequence.Extrinsic_YZY: 16>, 'Extrinsic_YXY': <EulerSequence.Extrinsic_YXY: 17>, 'Extrinsic_ZYZ': <EulerSequence.Extrinsic_ZYZ: 18>, 'Extrinsic_ZXZ': <EulerSequence.Extrinsic_ZXZ: 19>, 'Intrinsic_XYX': <EulerSequence.Intrinsic_XYX: 20>, 'Intrinsic_XZX': <EulerSequence.Intrinsic_XZX: 21>, 'Intrinsic_YZY': <EulerSequence.Intrinsic_YZY: 22>, 'Intrinsic_YXY': <EulerSequence.Intrinsic_YXY: 23>, 'Intrinsic_ZXZ': <EulerSequence.Intrinsic_ZXZ: 24>, 'Intrinsic_ZYZ': <EulerSequence.Intrinsic_ZYZ: 25>}
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
class Hypr:
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
    def __init__(self) -> None:
        """
        Creates an indefinite hyperbola
        """
    @typing.overload
    def __init__(self, a2: Ax2, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat) -> None:
        """
        Constructs a hyperbola with given major and minor radii, where a2 locates the hyperbola:
        - center is the origin of a2
        - X Direction defines the major axis
        - Y Direction defines the minor axis
        Raises exception if major_radius < 0 or minor_radius < 0
        """
    def __repr__(self) -> str:
        ...
    def asymptote1(self) -> Ax1:
        """
        Returns the first asymptote of the hyperbola
        """
    def asymptote2(self) -> Ax1:
        """
        Returns the second asymptote of the hyperbola
        """
    def conjugate_branch1(self) -> Hypr:
        """
        Returns the conjugate branch on the positive side of the YAxis
        """
    def conjugate_branch2(self) -> Hypr:
        """
        Returns the conjugate branch on the negative side of the YAxis
        """
    def directrix1(self) -> Ax1:
        """
        Returns the first directrix of the hyperbola
        """
    def directrix2(self) -> Ax1:
        """
        Returns the second directrix of the hyperbola
        """
    def eccentricity(self) -> float:
        """
        Returns the eccentricity of the hyperbola (e > 1)
        """
    def focal(self) -> float:
        """
        Computes the focal distance (distance between the two foci)
        """
    def focus1(self) -> Pnt:
        """
        Returns the first focus (on positive side of major axis)
        """
    def focus2(self) -> Pnt:
        """
        Returns the second focus (on negative side of major axis)
        """
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Hypr:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Hypr:
        ...
    @typing.overload
    def mirrored(self, a2: Ax2) -> Hypr:
        ...
    def other_branch(self) -> Hypr:
        """
        Returns the other branch of this hyperbola (symmetrical w.r.t. center)
        """
    def parameter(self) -> float:
        """
        Returns p = (e² - 1) * MajorRadius where e is the eccentricity
        """
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Hypr:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Hypr:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Hypr:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Hypr:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Hypr:
        ...
    @property
    def axis(self) -> Ax1:
        """
        Get or set the main axis of the hyperbola (perpendicular to the plane, passing through center)
        """
    @axis.setter
    def axis(self, arg1: Ax1) -> None:
        ...
    @property
    def location(self) -> Pnt:
        """
        Get or set the center of the hyperbola
        """
    @location.setter
    def location(self, arg1: Pnt) -> None:
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
    def position(self) -> Ax2:
        """
        Get or set the position (local coordinate system) of the hyperbola
        """
    @position.setter
    def position(self, arg1: Ax2) -> None:
        ...
    @property
    def x_axis(self) -> Ax1:
        """
        Returns the major axis of the hyperbola
        """
    @property
    def y_axis(self) -> Ax1:
        """
        Returns the minor axis of the hyperbola
        """
class Hypr2d:
    """
    Describes a hyperbola in 2D space.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates a hyperbola
        """
    @typing.overload
    def __init__(self, a: Ax2d, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat, sense: bool = True) -> None:
        """
        Creates a hyperbola with axis A and radii
        """
    @typing.overload
    def __init__(self, a: Ax22d, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat) -> None:
        """
        Creates a hyperbola with coordinate system A and radii
        """
    def __repr__(self) -> str:
        ...
    def asymptote1(self) -> Ax2d:
        """
        Returns the first asymptote
        """
    def asymptote2(self) -> Ax2d:
        """
        Returns the second asymptote
        """
    def axis(self) -> Ax22d:
        """
        Returns the main axis
        """
    def conjugate_branch1(self) -> Hypr2d:
        """
        Returns the first conjugate branch
        """
    def conjugate_branch2(self) -> Hypr2d:
        """
        Returns the second conjugate branch
        """
    def directrix1(self) -> Ax2d:
        """
        Returns the first directrix
        """
    def directrix2(self) -> Ax2d:
        """
        Returns the second directrix
        """
    def eccentricity(self) -> float:
        """
        Returns the eccentricity
        """
    def focal(self) -> float:
        """
        Returns the focal distance
        """
    def focus1(self) -> Pnt2d:
        """
        Returns the first focus
        """
    def focus2(self) -> Pnt2d:
        """
        Returns the second focus
        """
    def is_direct(self) -> bool:
        """
        Returns True if direct orientation
        """
    def location(self) -> Pnt2d:
        """
        Returns the center
        """
    def major_radius(self) -> float:
        """
        Returns the major radius
        """
    def minor_radius(self) -> float:
        """
        Returns the minor radius
        """
    def other_branch(self) -> Hypr2d:
        """
        Returns the other branch
        """
    def parameter(self) -> float:
        """
        Returns the parameter
        """
    def reverse(self) -> None:
        """
        Reverses the orientation
        """
    def reversed(self) -> Hypr2d:
        """
        Returns a reversed copy
        """
    def set_axis(self, a: Ax22d) -> None:
        """
        Sets the main axis
        """
    def set_location(self, p: Pnt2d) -> None:
        """
        Sets the center
        """
    def set_major_radius(self, r: typing.SupportsFloat) -> None:
        """
        Sets the major radius
        """
    def set_minor_radius(self, r: typing.SupportsFloat) -> None:
        """
        Sets the minor radius
        """
    def set_x_axis(self, a: Ax2d) -> None:
        """
        Sets the transverse axis
        """
    def set_y_axis(self, a: Ax2d) -> None:
        """
        Sets the conjugate axis
        """
    def x_axis(self) -> Ax2d:
        """
        Returns the transverse axis
        """
    def y_axis(self) -> Ax2d:
        """
        Returns the conjugate axis
        """
class Lin:
    """
    Representation of an infinite line defined by a point and a direction.
    """
    direction: Dir
    location: Pnt
    position: Ax1
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def __init__(self, p: Pnt, v: Dir) -> None:
        ...
    def __repr__(self) -> str:
        ...
    def angle(self, other: Lin) -> float:
        ...
    def contains(self, p: Pnt, linear_tolerance: typing.SupportsFloat) -> bool:
        ...
    @typing.overload
    def distance(self, p: Pnt) -> float:
        ...
    @typing.overload
    def distance(self, other: Lin) -> float:
        ...
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Lin:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Lin:
        ...
    @typing.overload
    def mirrored(self, a2: Ax2) -> Lin:
        ...
    def normal(self, p: Pnt) -> Lin:
        ...
    def reverse(self) -> None:
        ...
    def reversed(self) -> Lin:
        ...
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Lin:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Lin:
        ...
    @typing.overload
    def square_distance(self, p: Pnt) -> float:
        ...
    @typing.overload
    def square_distance(self, other: Lin) -> float:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Lin:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Lin:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Lin:
        ...
class Lin2d:
    """
    Describes an infinite line in 2D space: a point and a direction.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates a line along X axis through origin
        """
    @typing.overload
    def __init__(self, a: Ax2d) -> None:
        """
        Creates a line from an axis
        """
    @typing.overload
    def __init__(self, p: Pnt2d, v: Dir2d) -> None:
        """
        Creates a line through point P with direction V
        """
    @typing.overload
    def __init__(self, a: typing.SupportsFloat, b: typing.SupportsFloat, c: typing.SupportsFloat) -> None:
        """
        Creates a line from equation A*X + B*Y + C = 0
        """
    def __repr__(self) -> str:
        ...
    def angle(self, other: Lin2d) -> float:
        """
        Returns the angle to another line
        """
    def coefficients(self) -> tuple:
        """
        Returns the coefficients (A, B, C) of A*X + B*Y + C = 0
        """
    def contains(self, p: Pnt2d, linear_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if point P is on the line within tolerance
        """
    def direction(self) -> Dir2d:
        """
        Returns the direction
        """
    @typing.overload
    def distance(self, p: Pnt2d) -> float:
        """
        Returns the distance to point P
        """
    @typing.overload
    def distance(self, other: Lin2d) -> float:
        """
        Returns the distance to another line
        """
    def location(self) -> Pnt2d:
        """
        Returns a point on the line
        """
    @typing.overload
    def mirror(self, p: Pnt2d) -> None:
        """
        Mirrors about point P
        """
    @typing.overload
    def mirror(self, a: Ax2d) -> None:
        """
        Mirrors about axis A
        """
    @typing.overload
    def mirrored(self, p: Pnt2d) -> Lin2d:
        """
        Returns a mirrored copy
        """
    @typing.overload
    def mirrored(self, a: Ax2d) -> Lin2d:
        """
        Returns a mirrored copy
        """
    def normal(self, p: Pnt2d) -> Lin2d:
        """
        Returns a line perpendicular to this line through point P
        """
    def position(self) -> Ax2d:
        """
        Returns the axis
        """
    def reverse(self) -> None:
        """
        Reverses the direction
        """
    def reversed(self) -> Lin2d:
        """
        Returns a reversed copy
        """
    def rotate(self, p: Pnt2d, angle: typing.SupportsFloat) -> None:
        """
        Rotates about point P
        """
    def rotated(self, p: Pnt2d, angle: typing.SupportsFloat) -> Lin2d:
        """
        Returns a rotated copy
        """
    def scale(self, p: Pnt2d, s: typing.SupportsFloat) -> None:
        """
        Scales about point P
        """
    def scaled(self, p: Pnt2d, s: typing.SupportsFloat) -> Lin2d:
        """
        Returns a scaled copy
        """
    def set_direction(self, v: Dir2d) -> None:
        """
        Sets the direction
        """
    def set_location(self, p: Pnt2d) -> None:
        """
        Sets the location
        """
    def set_position(self, a: Ax2d) -> None:
        """
        Sets the axis
        """
    @typing.overload
    def square_distance(self, p: Pnt2d) -> float:
        """
        Returns the squared distance to point P
        """
    @typing.overload
    def square_distance(self, other: Lin2d) -> float:
        """
        Returns the squared distance to another line
        """
    def transform(self, t: gp_Trsf2d) -> None:
        """
        Transforms this line
        """
    def transformed(self, t: gp_Trsf2d) -> Lin2d:
        """
        Returns a transformed copy
        """
    def translate(self, v: Vec2d) -> None:
        """
        Translates by vector V
        """
    def translated(self, v: Vec2d) -> Lin2d:
        """
        Returns a translated copy
        """
class Mat:
    """
    Describes a three column, three row matrix for various vectorial or matrix computations.
    """
    def __add__(self, other: Mat) -> Mat:
        ...
    def __getitem__(self, index: tuple[typing.SupportsInt, typing.SupportsInt]) -> float:
        """
        Access element by (row, col) tuple, 1-indexed
        """
    def __iadd__(self, arg0: Mat) -> Mat:
        ...
    @typing.overload
    def __imul__(self, arg0: Mat) -> Mat:
        ...
    @typing.overload
    def __imul__(self, arg0: typing.SupportsFloat) -> Mat:
        ...
    @typing.overload
    def __init__(self) -> None:
        """
        Creates a matrix with null coefficients
        """
    @typing.overload
    def __init__(self, a11: typing.SupportsFloat, a12: typing.SupportsFloat, a13: typing.SupportsFloat, a21: typing.SupportsFloat, a22: typing.SupportsFloat, a23: typing.SupportsFloat, a31: typing.SupportsFloat, a32: typing.SupportsFloat, a33: typing.SupportsFloat) -> None:
        """
        Creates a matrix with specified coefficients
        """
    @typing.overload
    def __init__(self, col1: XYZ, col2: XYZ, col3: XYZ) -> None:
        """
        Creates a matrix from three column vectors
        """
    def __isub__(self, arg0: Mat) -> Mat:
        ...
    def __itruediv__(self, arg0: typing.SupportsFloat) -> Mat:
        ...
    @typing.overload
    def __mul__(self, other: Mat) -> Mat:
        ...
    @typing.overload
    def __mul__(self, scalar: typing.SupportsFloat) -> Mat:
        ...
    def __repr__(self) -> str:
        ...
    def __rmul__(self, scalar: typing.SupportsFloat) -> Mat:
        ...
    def __setitem__(self, index: tuple[typing.SupportsInt, typing.SupportsInt], value: typing.SupportsFloat) -> None:
        """
        Set element by (row, col) tuple, 1-indexed
        """
    def __sub__(self, other: Mat) -> Mat:
        ...
    def __truediv__(self, scalar: typing.SupportsFloat) -> Mat:
        ...
    def column(self, col: typing.SupportsInt) -> XYZ:
        """
        Returns the column of given index (1-3) as gp_XYZ
        """
    def invert(self) -> None:
        """
        Inverts this matrix in place
        """
    def inverted(self) -> Mat:
        """
        Returns the inverse matrix
        """
    def power(self, n: typing.SupportsInt) -> None:
        """
        Computes self^n in place. n=0 gives identity, n<0 gives inverse^|n|
        """
    def powered(self, n: typing.SupportsInt) -> Mat:
        """
        Returns self^n
        """
    def row(self, row: typing.SupportsInt) -> XYZ:
        """
        Returns the row of given index (1-3) as gp_XYZ
        """
    def set_col(self, col: typing.SupportsInt, value: XYZ) -> None:
        """
        Assigns gp_XYZ to the column of given index
        """
    def set_cols(self, col1: XYZ, col2: XYZ, col3: XYZ) -> None:
        """
        Assigns three gp_XYZ to the columns
        """
    def set_cross(self, ref: XYZ) -> None:
        """
        Sets matrix M such that M * v = ref.Cross(v)
        """
    def set_diagonal(self, x1: typing.SupportsFloat, x2: typing.SupportsFloat, x3: typing.SupportsFloat) -> None:
        """
        Modifies the main diagonal of the matrix
        """
    def set_dot(self, ref: XYZ) -> None:
        """
        Sets matrix M such that M * v = ref.Dot(v)
        """
    def set_identity(self) -> None:
        """
        Modifies this matrix to represent the identity matrix
        """
    def set_rotation(self, axis: XYZ, angle: typing.SupportsFloat) -> None:
        """
        Sets this matrix to represent a rotation around axis by angle (radians)
        """
    def set_row(self, row: typing.SupportsInt, value: XYZ) -> None:
        """
        Assigns gp_XYZ to the row of given index
        """
    def set_rows(self, row1: XYZ, row2: XYZ, row3: XYZ) -> None:
        """
        Assigns three gp_XYZ to the rows
        """
    def set_scale(self, scale: typing.SupportsFloat) -> None:
        """
        Sets this matrix to represent uniform scaling
        """
    def set_value(self, row: typing.SupportsInt, col: typing.SupportsInt, value: typing.SupportsFloat) -> None:
        """
        Sets the coefficient at (row, col), 1-indexed
        """
    def transpose(self) -> None:
        """
        Transposes this matrix in place
        """
    def transposed(self) -> Mat:
        """
        Returns the transposed matrix
        """
    def value(self, row: typing.SupportsInt, col: typing.SupportsInt) -> float:
        """
        Returns the coefficient at (row, col), 1-indexed
        """
    @property
    def determinant(self) -> float:
        """
        Returns the determinant of the matrix
        """
    @property
    def diagonal(self) -> XYZ:
        """
        Returns the main diagonal as gp_XYZ
        """
    @property
    def is_singular(self) -> bool:
        """
        Returns True if the matrix is singular (determinant ~ 0)
        """
class Parab:
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
    def __init__(self) -> None:
        """
        Creates an indefinite parabola
        """
    @typing.overload
    def __init__(self, a2: Ax2, focal: typing.SupportsFloat) -> None:
        """
        Constructs a parabola with its local coordinate system a2 and focal length.
        The XDirection defines the axis of symmetry.
        The YDirection is parallel to the directrix.
        The Location point is the vertex (apex) of the parabola.
        Raises exception if focal < 0
        """
    @typing.overload
    def __init__(self, d: Ax1, f: Pnt) -> None:
        """
        Constructs a parabola from directrix d and focus point f
        """
    def __repr__(self) -> str:
        ...
    def directrix(self) -> Ax1:
        """
        Returns the directrix of the parabola (on negative side of symmetry axis)
        """
    def focus(self) -> Pnt:
        """
        Returns the focus of the parabola (on positive side of symmetry axis)
        """
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Parab:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Parab:
        ...
    @typing.overload
    def mirrored(self, a2: Ax2) -> Parab:
        ...
    def parameter(self) -> float:
        """
        Returns the parameter p (distance between focus and directrix = 2*focal)
        """
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Parab:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Parab:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Parab:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Parab:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Parab:
        ...
    @property
    def axis(self) -> Ax1:
        """
        Get or set the symmetry axis of the parabola (perpendicular to directrix, passing through apex)
        """
    @axis.setter
    def axis(self, arg1: Ax1) -> None:
        ...
    @property
    def focal(self) -> float:
        """
        Get or set the focal length of the parabola
        """
    @focal.setter
    def focal(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def location(self) -> Pnt:
        """
        Get or set the apex (vertex) of the parabola
        """
    @location.setter
    def location(self, arg1: Pnt) -> None:
        ...
    @property
    def position(self) -> Ax2:
        """
        Get or set the position (local coordinate system) of the parabola
        """
    @position.setter
    def position(self, arg1: Ax2) -> None:
        ...
    @property
    def x_axis(self) -> Ax1:
        """
        Returns the symmetry axis of the parabola
        """
    @property
    def y_axis(self) -> Ax1:
        """
        Returns the YAxis of the parabola (parallel to directrix)
        """
class Parab2d:
    """
    Describes a parabola in 2D space.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates a parabola
        """
    @typing.overload
    def __init__(self, a: Ax2d, focal: typing.SupportsFloat, sense: bool = True) -> None:
        """
        Creates a parabola with axis A and focal length
        """
    @typing.overload
    def __init__(self, a: Ax22d, focal: typing.SupportsFloat) -> None:
        """
        Creates a parabola with coordinate system A and focal length
        """
    def __repr__(self) -> str:
        ...
    def axis(self) -> Ax22d:
        """
        Returns the axis of symmetry
        """
    def directrix(self) -> Ax2d:
        """
        Returns the directrix
        """
    def focal(self) -> float:
        """
        Returns the focal length
        """
    def focus(self) -> Pnt2d:
        """
        Returns the focus
        """
    def is_direct(self) -> bool:
        """
        Returns True if direct orientation
        """
    def location(self) -> Pnt2d:
        """
        Returns the vertex
        """
    def mirror_axis(self) -> Ax2d:
        """
        Returns the axis of symmetry
        """
    def parameter(self) -> float:
        """
        Returns the parameter (2 * focal)
        """
    def reverse(self) -> None:
        """
        Reverses the orientation
        """
    def reversed(self) -> Parab2d:
        """
        Returns a reversed copy
        """
    def set_axis(self, a: Ax22d) -> None:
        """
        Sets the axis
        """
    def set_focal(self, f: typing.SupportsFloat) -> None:
        """
        Sets the focal length
        """
    def set_location(self, p: Pnt2d) -> None:
        """
        Sets the vertex
        """
    def set_mirror_axis(self, a: Ax2d) -> None:
        """
        Sets the axis
        """
class Pln:
    axis: Ax1
    location: Pnt
    position: Ax3
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, a3: Ax3) -> None:
        ...
    @typing.overload
    def __init__(self, p: Pnt, v: Dir) -> None:
        ...
    @typing.overload
    def __init__(self, a: typing.SupportsFloat, b: typing.SupportsFloat, c: typing.SupportsFloat, d: typing.SupportsFloat) -> None:
        ...
    def __repr__(self) -> str:
        ...
    def coefficients(self) -> tuple:
        ...
    @typing.overload
    def contains(self, p: Pnt, linear_tolerance: typing.SupportsFloat) -> bool:
        ...
    @typing.overload
    def contains(self, p: gp_Lin, linear_tolerance: typing.SupportsFloat, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    def direct(self) -> bool:
        ...
    @typing.overload
    def distance(self, p: Pnt) -> float:
        ...
    @typing.overload
    def distance(self, l: gp_Lin) -> float:
        ...
    @typing.overload
    def distance(self, other: Pln) -> float:
        ...
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Pln:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Pln:
        ...
    @typing.overload
    def mirrored(self, a2: Ax2) -> Pln:
        ...
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Pln:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Pln:
        ...
    @typing.overload
    def square_distance(self, p: Pnt) -> float:
        ...
    @typing.overload
    def square_distance(self, l: gp_Lin) -> float:
        ...
    @typing.overload
    def square_distance(self, other: Pln) -> float:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Pln:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Pln:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Pln:
        ...
    def x_axis(self) -> Ax1:
        ...
    def y_axis(self) -> Ax1:
        ...
class Pnt:
    xyz: XYZ
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, coord: XYZ) -> None:
        ...
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat, z: typing.SupportsFloat) -> None:
        ...
    def __repr__(self) -> str:
        ...
    def bary_center(self, alpha: typing.SupportsFloat, p: Pnt, beta: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def coord(self, index: typing.SupportsInt) -> float:
        ...
    @typing.overload
    def coord(self) -> tuple:
        """
        Returns tuple of (x, y, z)
        """
    def distance(self, other: Pnt) -> float:
        ...
    def is_equal(self, other: Pnt, linear_tolerance: typing.SupportsFloat) -> bool:
        ...
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: gp_Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: gp_Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Pnt:
        ...
    @typing.overload
    def mirrored(self, a1: gp_Ax1) -> Pnt:
        ...
    @typing.overload
    def mirrored(self, a2: gp_Ax2) -> Pnt:
        ...
    def rotate(self, a1: gp_Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: gp_Ax1, ang: typing.SupportsFloat) -> Pnt:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Pnt:
        ...
    @typing.overload
    def set_coord(self, x: typing.SupportsFloat, y: typing.SupportsFloat, z: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def set_coord(self, index: typing.SupportsInt, value: typing.SupportsFloat) -> None:
        ...
    def square_distance(self, other: Pnt) -> float:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Pnt:
        ...
    @typing.overload
    def translate(self, v: gp_Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: gp_Vec) -> Pnt:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Pnt:
        ...
    @property
    def x(self) -> float:
        ...
    @x.setter
    def x(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def y(self) -> float:
        ...
    @y.setter
    def y(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def z(self) -> float:
        ...
    @z.setter
    def z(self, arg1: typing.SupportsFloat) -> None:
        ...
class Pnt2d:
    """
    Describes a point in 2D space.
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates point at origin (0, 0)
        """
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat) -> None:
        """
        Creates point at (X, Y)
        """
    @typing.overload
    def __init__(self, coord: XY) -> None:
        """
        Creates point from XY coordinates
        """
    def __repr__(self) -> str:
        ...
    def distance(self, other: Pnt2d) -> float:
        """
        Returns the distance to another point
        """
    def is_equal(self, other: Pnt2d, tolerance: typing.SupportsFloat) -> bool:
        """
        Returns True if distance to other is <= tolerance
        """
    @typing.overload
    def mirror(self, p: Pnt2d) -> None:
        """
        Mirrors this point about point P
        """
    @typing.overload
    def mirror(self, a: gp_Ax2d) -> None:
        """
        Mirrors this point about axis A
        """
    @typing.overload
    def mirrored(self, p: Pnt2d) -> Pnt2d:
        """
        Returns a point mirrored about point P
        """
    @typing.overload
    def mirrored(self, a: gp_Ax2d) -> Pnt2d:
        """
        Returns a point mirrored about axis A
        """
    def rotate(self, p: Pnt2d, angle: typing.SupportsFloat) -> None:
        """
        Rotates this point about point P by angle (radians)
        """
    def rotated(self, p: Pnt2d, angle: typing.SupportsFloat) -> Pnt2d:
        """
        Returns a rotated copy
        """
    def scale(self, p: Pnt2d, s: typing.SupportsFloat) -> None:
        """
        Scales this point about point P by factor S
        """
    def scaled(self, p: Pnt2d, s: typing.SupportsFloat) -> Pnt2d:
        """
        Returns a scaled copy
        """
    def square_distance(self, other: Pnt2d) -> float:
        """
        Returns the squared distance to another point
        """
    def transform(self, t: gp_Trsf2d) -> None:
        """
        Transforms this point by transformation T
        """
    def transformed(self, t: gp_Trsf2d) -> Pnt2d:
        """
        Returns a transformed copy
        """
    def translate(self, v: gp_Vec2d) -> None:
        """
        Translates this point by vector V
        """
    def translated(self, v: gp_Vec2d) -> Pnt2d:
        """
        Returns a translated copy
        """
    @property
    def coord(self) -> XY:
        """
        Get or set the XY coordinates
        """
    @coord.setter
    def coord(self, arg1: XY) -> None:
        ...
    @property
    def x(self) -> float:
        """
        Get or set the X coordinate
        """
    @x.setter
    def x(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def xy(self) -> XY:
        """
        Sets both coordinates from XY
        """
    @xy.setter
    def xy(self, arg1: XY) -> None:
        ...
    @property
    def y(self) -> float:
        """
        Get or set the Y coordinate
        """
    @y.setter
    def y(self, arg1: typing.SupportsFloat) -> None:
        ...
class Quaternion:
    """
    Represents rotation in 3D space as a quaternion and provides operations
    with rotations based on quaternion mathematics. Also provides conversion
    to/from other rotation representations (matrix, axis+angle, Euler angles).
    """
    def __add__(self, other: Quaternion) -> Quaternion:
        ...
    def __iadd__(self, arg0: Quaternion) -> Quaternion:
        ...
    @typing.overload
    def __imul__(self, arg0: Quaternion) -> Quaternion:
        ...
    @typing.overload
    def __imul__(self, arg0: typing.SupportsFloat) -> Quaternion:
        ...
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an identity quaternion (no rotation)
        """
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat, z: typing.SupportsFloat, w: typing.SupportsFloat) -> None:
        """
        Creates quaternion from components (x, y, z, w)
        """
    @typing.overload
    def __init__(self, vec_from: Vec, vec_to: Vec) -> None:
        """
        Creates quaternion for shortest-arc rotation from vec_from to vec_to
        """
    @typing.overload
    def __init__(self, vec_from: Vec, vec_to: Vec, help_cross_vec: Vec) -> None:
        """
        Creates quaternion for rotation from vec_from to vec_to with preferred axis
        """
    @typing.overload
    def __init__(self, axis: Vec, angle: typing.SupportsFloat) -> None:
        """
        Creates quaternion for rotation around axis by angle (radians)
        """
    @typing.overload
    def __init__(self, matrix: Mat) -> None:
        """
        Creates quaternion from 3x3 rotation matrix
        """
    def __isub__(self, arg0: Quaternion) -> Quaternion:
        ...
    @typing.overload
    def __mul__(self, other: Quaternion) -> Quaternion:
        ...
    @typing.overload
    def __mul__(self, scalar: typing.SupportsFloat) -> Quaternion:
        ...
    @typing.overload
    def __mul__(self, vec: Vec) -> Vec:
        ...
    def __neg__(self) -> Quaternion:
        ...
    def __repr__(self) -> str:
        ...
    def __rmul__(self, scalar: typing.SupportsFloat) -> Quaternion:
        ...
    def __sub__(self, other: Quaternion) -> Quaternion:
        ...
    def dot(self, other: Quaternion) -> float:
        """
        Returns the dot product with another quaternion
        """
    def get_euler_angles(self, order: EulerSequence) -> tuple:
        """
        Returns (alpha, beta, gamma) Euler angles in specified sequence
        """
    def get_vector_and_angle(self) -> tuple:
        """
        Returns (axis, angle) tuple representing the rotation
        """
    def invert(self) -> None:
        """
        Inverts this quaternion (both direction and norm) in place
        """
    def inverted(self) -> Quaternion:
        """
        Returns the inverse quaternion q^-1
        """
    def is_equal(self, other: Quaternion) -> bool:
        """
        Tests equality (simple comparison without tolerance)
        """
    def multiply_vec(self, vec: Vec) -> Vec:
        """
        Rotates vector by this quaternion
        """
    def normalize(self) -> None:
        """
        Scales quaternion so its norm becomes 1
        """
    def normalized(self) -> Quaternion:
        """
        Returns normalized copy of quaternion
        """
    def reverse(self) -> None:
        """
        Reverses the rotation direction (conjugates) in place
        """
    def reversed(self) -> Quaternion:
        """
        Returns quaternion with reversed rotation direction
        """
    def scale(self, scale: typing.SupportsFloat) -> None:
        """
        Scales all components by given factor
        """
    def scaled(self, scale: typing.SupportsFloat) -> Quaternion:
        """
        Returns quaternion with all components scaled
        """
    def set(self, x: typing.SupportsFloat, y: typing.SupportsFloat, z: typing.SupportsFloat, w: typing.SupportsFloat) -> None:
        """
        Sets quaternion components
        """
    def set_euler_angles(self, order: EulerSequence, alpha: typing.SupportsFloat, beta: typing.SupportsFloat, gamma: typing.SupportsFloat) -> None:
        """
        Sets quaternion from Euler angles (radians) in specified sequence
        """
    def set_ident(self) -> None:
        """
        Sets this to identity quaternion (no rotation)
        """
    def set_matrix(self, matrix: Mat) -> None:
        """
        Sets quaternion from 3x3 rotation matrix
        """
    @typing.overload
    def set_rotation(self, vec_from: Vec, vec_to: Vec) -> None:
        """
        Sets quaternion to shortest-arc rotation from vec_from to vec_to
        """
    @typing.overload
    def set_rotation(self, vec_from: Vec, vec_to: Vec, help_cross_vec: Vec) -> None:
        """
        Sets quaternion to rotation from vec_from to vec_to with preferred axis
        """
    def set_vector_and_angle(self, axis: Vec, angle: typing.SupportsFloat) -> None:
        """
        Sets quaternion from axis and angle (radians)
        """
    def stabilize_length(self) -> None:
        """
        Stabilizes quaternion length within 1 - 1/4 (faster than normalize)
        """
    @property
    def matrix(self) -> Mat:
        """
        Returns rotation as 3x3 matrix
        """
    @property
    def norm(self) -> float:
        """
        Returns the norm of the quaternion
        """
    @property
    def rotation_angle(self) -> float:
        """
        Returns the rotation angle from -PI to PI
        """
    @property
    def square_norm(self) -> float:
        """
        Returns the square norm
        """
    @property
    def w(self) -> float:
        """
        W (scalar) component of quaternion
        """
    @property
    def x(self) -> float:
        """
        X component of quaternion
        """
    @property
    def y(self) -> float:
        """
        Y component of quaternion
        """
    @property
    def z(self) -> float:
        """
        Z component of quaternion
        """
class Sphere:
    location: Pnt
    position: Ax3
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, a2: Ax2, radius: typing.SupportsFloat) -> None:
        ...
    def __repr__(self) -> str:
        ...
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Sphere:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Sphere:
        ...
    @typing.overload
    def mirrored(self, a2: Ax2) -> Sphere:
        ...
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Sphere:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Sphere:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Sphere:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Sphere:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Sphere:
        ...
    @property
    def radius(self) -> float:
        ...
    @radius.setter
    def radius(self, arg1: typing.SupportsFloat) -> None:
        ...
class Torus:
    axis: Ax1
    location: Pnt
    position: Ax3
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, a2: Ax2, major_radius: typing.SupportsFloat, minor_radius: typing.SupportsFloat) -> None:
        ...
    def __repr__(self) -> str:
        ...
    @typing.overload
    def mirror(self, p: Pnt) -> None:
        ...
    @typing.overload
    def mirror(self, a1: Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, p: Pnt) -> Torus:
        ...
    @typing.overload
    def mirrored(self, a1: Ax1) -> Torus:
        ...
    @typing.overload
    def mirrored(self, a2: Ax2) -> Torus:
        ...
    def rotate(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: Ax1, ang: typing.SupportsFloat) -> Torus:
        ...
    def scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, p: Pnt, s: typing.SupportsFloat) -> Torus:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Torus:
        ...
    @typing.overload
    def translate(self, v: Vec) -> None:
        ...
    @typing.overload
    def translate(self, p1: Pnt, p2: Pnt) -> None:
        ...
    @typing.overload
    def translated(self, v: Vec) -> Torus:
        ...
    @typing.overload
    def translated(self, p1: Pnt, p2: Pnt) -> Torus:
        ...
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
class Trsf:
    """
    Defines a non-persistent transformation in 3D space.
    
    Implements transformations:
    - Translation, Rotation, Scale
    - Symmetry with respect to a point, a line, a plane
    
    Complex transformations can be obtained by combining elementary transformations.
    The transformation is represented as a 4x4 matrix where the vectorial part {V1, V2, V3}
    defines the rotation/scale and T defines the translation.
    """
    def __imul__(self, t: Trsf) -> Trsf:
        """
        Composes this transformation with t: self *= t
        """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an identity transformation
        """
    @typing.overload
    def __init__(self, t: gp_Trsf2d) -> None:
        """
        Creates a 3D transformation from a 2D transformation
        """
    def __mul__(self, t: Trsf) -> Trsf:
        """
        Returns the composition self * t
        """
    def __repr__(self) -> str:
        ...
    @typing.overload
    def get_rotation(self) -> tuple:
        """
        Returns (has_rotation, axis, angle) where angle is in range (0, PI]
        """
    @typing.overload
    def get_rotation(self, axis: XYZ, angle: typing.SupportsFloat) -> bool:
        """
        Outputs the axis and angle (in range (0, PI]) of the rotational part.
        Returns True if there is a rotational part, False otherwise
        """
    def get_rotation_quaternion(self) -> gp_Quaternion:
        """
        Returns quaternion representing the rotational part
        """
    def invert(self) -> None:
        """
        Inverts the transformation in place
        """
    def inverted(self) -> Trsf:
        """
        Returns the inverse transformation
        """
    def multiplied(self, t: Trsf) -> Trsf:
        """
        Returns the composition self * t
        """
    def multiply(self, t: Trsf) -> None:
        """
        Composes this transformation with t: self = self * t
        """
    def power(self, n: typing.SupportsInt) -> None:
        """
        Raises the transformation to power n: self = self^n
        """
    def powered(self, n: typing.SupportsInt) -> Trsf:
        """
        Returns the transformation raised to power n
        """
    def pre_multiply(self, t: Trsf) -> None:
        """
        Composes this transformation with t: self = t * self
        """
    def set_displacement(self, from_system: Ax3, to_system: Ax3) -> None:
        """
        Modifies to transform coordinate system from_system into to_system.
        Use SetTransformation for point coordinate conversion
        """
    @typing.overload
    def set_mirror(self, p: Pnt) -> None:
        """
        Makes the transformation into a symmetrical transformation with respect to point p
        """
    @typing.overload
    def set_mirror(self, a1: Ax1) -> None:
        """
        Makes the transformation into an axial symmetry with axis a1
        """
    @typing.overload
    def set_mirror(self, a2: Ax2) -> None:
        """
        Makes the transformation into a planar symmetry with plane defined by a2
        """
    @typing.overload
    def set_rotation(self, a1: Ax1, ang: typing.SupportsFloat) -> None:
        """
        Changes the transformation into a rotation around axis a1 by angle ang (radians)
        """
    @typing.overload
    def set_rotation(self, r: gp_Quaternion) -> None:
        """
        Changes the transformation into a rotation defined by quaternion (around origin)
        """
    def set_rotation_part(self, r: gp_Quaternion) -> None:
        """
        Replaces the rotation part with specified quaternion
        """
    def set_scale(self, p: Pnt, s: typing.SupportsFloat) -> None:
        """
        Changes the transformation into a scale with center p and scaling value s
        """
    @typing.overload
    def set_transformation(self, from_system: Ax3, to_system: Ax3) -> None:
        """
        Sets transformation to convert coordinates from from_system to to_system
        """
    @typing.overload
    def set_transformation(self, to_system: Ax3) -> None:
        """
        Sets transformation from absolute coordinate system to to_system
        """
    @typing.overload
    def set_transformation(self, r: gp_Quaternion, t: Vec) -> None:
        """
        Sets transformation by directly specified rotation and translation
        """
    @typing.overload
    def set_translation(self, v: Vec) -> None:
        """
        Changes the transformation into a translation by vector v
        """
    @typing.overload
    def set_translation(self, p1: Pnt, p2: Pnt) -> None:
        """
        Changes the transformation into a translation from point p1 to point p2
        """
    def set_translation_part(self, v: Vec) -> None:
        """
        Replaces the translation vector with vector v
        """
    def set_values(self, a11: typing.SupportsFloat, a12: typing.SupportsFloat, a13: typing.SupportsFloat, a14: typing.SupportsFloat, a21: typing.SupportsFloat, a22: typing.SupportsFloat, a23: typing.SupportsFloat, a24: typing.SupportsFloat, a31: typing.SupportsFloat, a32: typing.SupportsFloat, a33: typing.SupportsFloat, a34: typing.SupportsFloat) -> None:
        """
        Sets the transformation coefficients directly.
        The matrix is orthogonalized before future use.
        Raises ConstructionError if determinant is null
        """
    @typing.overload
    def transforms(self, x: typing.SupportsFloat, y: typing.SupportsFloat, z: typing.SupportsFloat) -> None:
        """
        Transforms coordinates (x, y, z) in place
        """
    @typing.overload
    def transforms(self, coord: XYZ) -> None:
        """
        Transforms XYZ coordinates in place
        """
    def value(self, row: typing.SupportsInt, col: typing.SupportsInt) -> float:
        """
        Returns coefficient at position (row, col) including scale factor.
        row: 1-3, col: 1-4
        """
    @property
    def form(self) -> TrsfForm:
        """
        Get or set the nature of the transformation
        """
    @form.setter
    def form(self, arg1: TrsfForm) -> None:
        ...
    @property
    def h_vectorial_part(self) -> gp_Mat:
        """
        Returns the homogeneous vectorial part (3x3 matrix without scale factor)
        """
    @property
    def is_negative(self) -> bool:
        """
        True if the determinant of the vectorial part is negative
        """
    @property
    def scale_factor(self) -> float:
        """
        Get or set the scale factor
        """
    @scale_factor.setter
    def scale_factor(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def translation_part(self) -> XYZ:
        """
        Returns the translation part of the transformation
        """
    @property
    def vectorial_part(self) -> gp_Mat:
        """
        Returns the vectorial part (3x3 matrix including scale factor)
        """
class Trsf2d:
    """
    Describes a 2D transformation: translation, rotation, scaling, or combination.
    """
    def __init__(self) -> None:
        """
        Creates an identity transformation
        """
    def __mul__(self, t: Trsf2d) -> Trsf2d:
        ...
    def __repr__(self) -> str:
        ...
    def form(self) -> TrsfForm:
        """
        Returns the transformation form
        """
    def invert(self) -> None:
        """
        Inverts this transformation
        """
    def inverted(self) -> Trsf2d:
        """
        Returns the inverse
        """
    def is_negative(self) -> bool:
        """
        Returns True if determinant is negative
        """
    def multiplied(self, t: Trsf2d) -> Trsf2d:
        """
        Returns the composition self * T
        """
    def multiply(self, t: Trsf2d) -> None:
        """
        Composes with transformation T: self = self * T
        """
    def power(self, n: typing.SupportsInt) -> None:
        """
        Raises to power n
        """
    def powered(self, n: typing.SupportsInt) -> Trsf2d:
        """
        Returns raised to power n
        """
    def pre_multiply(self, t: Trsf2d) -> None:
        """
        Composes with transformation T: self = T * self
        """
    def rotation_part(self) -> float:
        """
        Returns the rotation angle
        """
    def scale_factor(self) -> float:
        """
        Returns the scale factor
        """
    @typing.overload
    def set_mirror(self, p: Pnt2d) -> None:
        """
        Sets as point mirror about P
        """
    @typing.overload
    def set_mirror(self, a: Ax2d) -> None:
        """
        Sets as axis mirror about A
        """
    def set_rotation(self, p: Pnt2d, angle: typing.SupportsFloat) -> None:
        """
        Sets as rotation about point P
        """
    def set_scale(self, p: Pnt2d, s: typing.SupportsFloat) -> None:
        """
        Sets as scaling about point P
        """
    @typing.overload
    def set_transformation(self, from_system: Ax2d, to_system: Ax2d) -> None:
        """
        Sets transformation between coordinate systems
        """
    @typing.overload
    def set_transformation(self, to_system: Ax2d) -> None:
        """
        Sets transformation from global to local system
        """
    @typing.overload
    def set_translation(self, v: Vec2d) -> None:
        """
        Sets as translation by vector V
        """
    @typing.overload
    def set_translation(self, p1: Pnt2d, p2: Pnt2d) -> None:
        """
        Sets as translation from P1 to P2
        """
    def transforms(self, x: typing.SupportsFloat, y: typing.SupportsFloat) -> tuple:
        """
        Transforms coordinates and returns (x, y)
        """
    def translation_part(self) -> XY:
        """
        Returns the translation vector
        """
    def value(self, row: typing.SupportsInt, col: typing.SupportsInt) -> float:
        """
        Returns the coefficient at row, col (1-indexed)
        """
class TrsfForm:
    """
    Identifies the type of transformation
    
    Members:
    
      Identity : No transformation
    
      Rotation : Rotation transformation
    
      Translation : Translation transformation
    
      PntMirror : Symmetry with respect to a point
    
      Ax1Mirror : Symmetry with respect to an axis
    
      Ax2Mirror : Symmetry with respect to a plane
    
      Scale : Scale transformation
    
      CompoundTrsf : Combination of transformations
    
      Other : Other transformation
    """
    Ax1Mirror: typing.ClassVar[TrsfForm]  # value = <TrsfForm.Ax1Mirror: 4>
    Ax2Mirror: typing.ClassVar[TrsfForm]  # value = <TrsfForm.Ax2Mirror: 5>
    CompoundTrsf: typing.ClassVar[TrsfForm]  # value = <TrsfForm.CompoundTrsf: 7>
    Identity: typing.ClassVar[TrsfForm]  # value = <TrsfForm.Identity: 0>
    Other: typing.ClassVar[TrsfForm]  # value = <TrsfForm.Other: 8>
    PntMirror: typing.ClassVar[TrsfForm]  # value = <TrsfForm.PntMirror: 3>
    Rotation: typing.ClassVar[TrsfForm]  # value = <TrsfForm.Rotation: 1>
    Scale: typing.ClassVar[TrsfForm]  # value = <TrsfForm.Scale: 6>
    Translation: typing.ClassVar[TrsfForm]  # value = <TrsfForm.Translation: 2>
    __members__: typing.ClassVar[dict[str, TrsfForm]]  # value = {'Identity': <TrsfForm.Identity: 0>, 'Rotation': <TrsfForm.Rotation: 1>, 'Translation': <TrsfForm.Translation: 2>, 'PntMirror': <TrsfForm.PntMirror: 3>, 'Ax1Mirror': <TrsfForm.Ax1Mirror: 4>, 'Ax2Mirror': <TrsfForm.Ax2Mirror: 5>, 'Scale': <TrsfForm.Scale: 6>, 'CompoundTrsf': <TrsfForm.CompoundTrsf: 7>, 'Other': <TrsfForm.Other: 8>}
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
class Vec:
    xyz: XYZ
    def __add__(self, arg0: Vec) -> Vec:
        ...
    def __iadd__(self, arg0: Vec) -> None:
        ...
    def __imul__(self, arg0: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, dir: gp_Dir) -> None:
        ...
    @typing.overload
    def __init__(self, coord: XYZ) -> None:
        ...
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat, z: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self, p1: Pnt, p2: Pnt) -> None:
        ...
    def __isub__(self, arg0: Vec) -> None:
        ...
    def __itruediv__(self, arg0: typing.SupportsFloat) -> None:
        ...
    def __ixor__(self, arg0: Vec) -> None:
        ...
    @typing.overload
    def __mul__(self, arg0: typing.SupportsFloat) -> Vec:
        ...
    @typing.overload
    def __mul__(self, arg0: Vec) -> float:
        ...
    def __neg__(self) -> Vec:
        ...
    def __repr__(self) -> str:
        ...
    def __rmul__(self, arg0: typing.SupportsFloat) -> Vec:
        ...
    def __sub__(self, arg0: Vec) -> Vec:
        ...
    def __truediv__(self, arg0: typing.SupportsFloat) -> Vec:
        ...
    def __xor__(self, arg0: Vec) -> Vec:
        ...
    def add(self, other: Vec) -> None:
        ...
    def added(self, other: Vec) -> Vec:
        ...
    def angle(self, other: Vec) -> float:
        ...
    def angle_with_ref(self, other: Vec, vref: Vec) -> float:
        ...
    def cross(self, right: Vec) -> None:
        ...
    def cross_cross(self, v1: Vec, v2: Vec) -> None:
        ...
    def cross_crossed(self, v1: Vec, v2: Vec) -> Vec:
        ...
    def cross_magnitude(self, right: Vec) -> float:
        ...
    def cross_square_magnitude(self, right: Vec) -> float:
        ...
    def crossed(self, right: Vec) -> Vec:
        ...
    def divide(self, scalar: typing.SupportsFloat) -> None:
        ...
    def divided(self, scalar: typing.SupportsFloat) -> Vec:
        ...
    def dot(self, other: Vec) -> float:
        ...
    def dot_cross(self, v1: Vec, v2: Vec) -> float:
        ...
    def is_equal(self, other: Vec, linear_tolerance: typing.SupportsFloat, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    def is_normal(self, other: Vec, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    def is_opposite(self, other: Vec, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    def is_parallel(self, other: Vec, angular_tolerance: typing.SupportsFloat) -> bool:
        ...
    @typing.overload
    def mirror(self, v: Vec) -> None:
        ...
    @typing.overload
    def mirror(self, a1: gp_Ax1) -> None:
        ...
    @typing.overload
    def mirror(self, a2: gp_Ax2) -> None:
        ...
    @typing.overload
    def mirrored(self, v: Vec) -> Vec:
        ...
    @typing.overload
    def mirrored(self, a1: gp_Ax1) -> Vec:
        ...
    @typing.overload
    def mirrored(self, a2: gp_Ax2) -> Vec:
        ...
    def multiplied(self, scalar: typing.SupportsFloat) -> Vec:
        ...
    def multiply(self, scalar: typing.SupportsFloat) -> None:
        ...
    def normalize(self) -> None:
        ...
    def normalized(self) -> Vec:
        ...
    def reverse(self) -> None:
        ...
    def reversed(self) -> Vec:
        ...
    def rotate(self, a1: gp_Ax1, ang: typing.SupportsFloat) -> None:
        ...
    def rotated(self, a1: gp_Ax1, ang: typing.SupportsFloat) -> Vec:
        ...
    def scale(self, s: typing.SupportsFloat) -> None:
        ...
    def scaled(self, s: typing.SupportsFloat) -> Vec:
        ...
    def subtract(self, other: Vec) -> None:
        ...
    def subtracted(self, other: Vec) -> Vec:
        ...
    def transform(self, t: gp_Trsf) -> None:
        ...
    def transformed(self, t: gp_Trsf) -> Vec:
        ...
    @property
    def magnitude(self) -> float:
        ...
    @property
    def square_magnitude(self) -> float:
        ...
    @property
    def x(self) -> float:
        ...
    @x.setter
    def x(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def y(self) -> float:
        ...
    @y.setter
    def y(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def z(self) -> float:
        ...
    @z.setter
    def z(self, arg1: typing.SupportsFloat) -> None:
        ...
class Vec2d:
    """
    Describes a vector in 2D space.
    """
    coord: XY
    xy: XY
    def __add__(self, other: Vec2d) -> Vec2d:
        ...
    @typing.overload
    def __init__(self) -> None:
        """
        Creates a null vector
        """
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat) -> None:
        """
        Creates vector with components (X, Y)
        """
    @typing.overload
    def __init__(self, coord: XY) -> None:
        """
        Creates vector from XY coordinates
        """
    @typing.overload
    def __init__(self, v: gp_Dir2d) -> None:
        """
        Creates a unit vector from a direction
        """
    @typing.overload
    def __init__(self, p1: Pnt2d, p2: Pnt2d) -> None:
        """
        Creates vector from P1 to P2
        """
    def __mul__(self, scalar: typing.SupportsFloat) -> Vec2d:
        ...
    def __neg__(self) -> Vec2d:
        ...
    def __repr__(self) -> str:
        ...
    def __sub__(self, other: Vec2d) -> Vec2d:
        ...
    def __truediv__(self, scalar: typing.SupportsFloat) -> Vec2d:
        ...
    def angle(self, other: Vec2d) -> float:
        """
        Returns the angle to another vector (-PI to PI)
        """
    def cross_magnitude(self, other: Vec2d) -> float:
        """
        Returns the magnitude of the cross product
        """
    def cross_square_magnitude(self, other: Vec2d) -> float:
        """
        Returns the squared magnitude of the cross product
        """
    def crossed(self, other: Vec2d) -> float:
        """
        Returns the cross product (X1*Y2 - Y1*X2)
        """
    def dot(self, other: Vec2d) -> float:
        """
        Returns the dot product
        """
    def is_equal(self, other: Vec2d, linear_tolerance: typing.SupportsFloat, angular_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests equality within tolerances
        """
    def is_normal(self, other: Vec2d, angular_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if vectors are normal (perpendicular)
        """
    def is_opposite(self, other: Vec2d, angular_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if vectors are opposite
        """
    def is_parallel(self, other: Vec2d, angular_tolerance: typing.SupportsFloat) -> bool:
        """
        Tests if vectors are parallel
        """
    @typing.overload
    def mirror(self, v: Vec2d) -> None:
        """
        Mirrors this vector about vector V
        """
    @typing.overload
    def mirror(self, a: gp_Ax2d) -> None:
        """
        Mirrors this vector about axis A
        """
    @typing.overload
    def mirrored(self, v: Vec2d) -> Vec2d:
        """
        Returns a mirrored copy
        """
    @typing.overload
    def mirrored(self, a: gp_Ax2d) -> Vec2d:
        """
        Returns a mirrored copy
        """
    def normalize(self) -> None:
        """
        Normalizes this vector
        """
    def normalized(self) -> Vec2d:
        """
        Returns a normalized copy
        """
    def reverse(self) -> None:
        """
        Reverses this vector
        """
    def reversed(self) -> Vec2d:
        """
        Returns a reversed copy
        """
    def rotate(self, angle: typing.SupportsFloat) -> None:
        """
        Rotates this vector by angle (radians)
        """
    def rotated(self, angle: typing.SupportsFloat) -> Vec2d:
        """
        Returns a rotated copy
        """
    def scale(self, s: typing.SupportsFloat) -> None:
        """
        Scales this vector
        """
    def scaled(self, s: typing.SupportsFloat) -> Vec2d:
        """
        Returns a scaled copy
        """
    def transform(self, t: gp_Trsf2d) -> None:
        """
        Transforms this vector
        """
    def transformed(self, t: gp_Trsf2d) -> Vec2d:
        """
        Returns a transformed copy
        """
    @property
    def magnitude(self) -> float:
        """
        Returns the magnitude
        """
    @property
    def square_magnitude(self) -> float:
        """
        Returns the squared magnitude
        """
    @property
    def x(self) -> float:
        """
        Get or set the X component
        """
    @x.setter
    def x(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def y(self) -> float:
        """
        Get or set the Y component
        """
    @y.setter
    def y(self, arg1: typing.SupportsFloat) -> None:
        ...
class XY:
    """
    Describes a coordinate pair (X, Y) in 2D space.
    """
    def __add__(self, other: XY) -> XY:
        ...
    @typing.overload
    def __init__(self) -> None:
        """
        Creates XY with zero coordinates
        """
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat) -> None:
        """
        Creates XY with given coordinates
        """
    def __mul__(self, scalar: typing.SupportsFloat) -> XY:
        ...
    def __neg__(self) -> XY:
        ...
    def __repr__(self) -> str:
        ...
    def __sub__(self, other: XY) -> XY:
        ...
    def __truediv__(self, scalar: typing.SupportsFloat) -> XY:
        ...
    def crossed(self, other: XY) -> float:
        """
        Returns the cross product (X1*Y2 - Y1*X2)
        """
    def dot(self, other: XY) -> float:
        """
        Returns the dot product
        """
    def is_equal(self, other: XY, tolerance: typing.SupportsFloat) -> bool:
        """
        Returns True if distance to other is <= tolerance
        """
    def normalize(self) -> None:
        """
        Normalizes this XY
        """
    def normalized(self) -> XY:
        """
        Returns a normalized copy
        """
    def reverse(self) -> None:
        """
        Reverses both coordinates
        """
    def reversed(self) -> XY:
        """
        Returns a reversed copy
        """
    def square_modulus(self) -> float:
        """
        Returns X^2 + Y^2
        """
    @property
    def coord(self) -> tuple:
        """
        Get or set both coordinates as a tuple (x, y)
        """
    @coord.setter
    def coord(self, arg1: tuple) -> None:
        ...
    @property
    def modulus(self) -> float:
        """
        Returns sqrt(X^2 + Y^2)
        """
    @property
    def x(self) -> float:
        """
        Get or set the X coordinate
        """
    @x.setter
    def x(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def xy(self) -> XY:
        """
        Get or set both coordinates using another XY
        """
    @xy.setter
    def xy(self, arg1: XY) -> None:
        ...
    @property
    def y(self) -> float:
        """
        Get or set the Y coordinate
        """
    @y.setter
    def y(self, arg1: typing.SupportsFloat) -> None:
        ...
class XYZ:
    __hash__: typing.ClassVar[None] = None
    def __add__(self, arg0: XYZ) -> XYZ:
        ...
    def __eq__(self, arg0: XYZ) -> bool:
        ...
    def __iadd__(self, arg0: XYZ) -> None:
        ...
    def __imul__(self, arg0: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def __init__(self) -> None:
        ...
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat, z: typing.SupportsFloat) -> None:
        ...
    def __isub__(self, arg0: XYZ) -> None:
        ...
    def __itruediv__(self, arg0: typing.SupportsFloat) -> None:
        ...
    def __ixor__(self, arg0: XYZ) -> None:
        ...
    @typing.overload
    def __mul__(self, arg0: typing.SupportsFloat) -> XYZ:
        ...
    @typing.overload
    def __mul__(self, arg0: XYZ) -> float:
        ...
    def __repr__(self) -> str:
        ...
    def __rmul__(self, arg0: typing.SupportsFloat) -> XYZ:
        ...
    def __sub__(self, arg0: XYZ) -> XYZ:
        ...
    def __truediv__(self, arg0: typing.SupportsFloat) -> XYZ:
        ...
    def __xor__(self, arg0: XYZ) -> XYZ:
        ...
    def add(self, other: XYZ) -> None:
        ...
    def added(self, other: XYZ) -> XYZ:
        ...
    @typing.overload
    def coord(self, index: typing.SupportsInt) -> float:
        ...
    @typing.overload
    def coord(self) -> tuple:
        """
        Returns tuple of (x, y, z)
        """
    def cross(self, other: XYZ) -> None:
        ...
    def cross_cross(self, coord1: XYZ, coord2: XYZ) -> None:
        ...
    def cross_crossed(self, coord1: XYZ, coord2: XYZ) -> XYZ:
        ...
    def cross_magnitude(self, right: XYZ) -> float:
        ...
    def cross_square_magnitude(self, right: XYZ) -> float:
        ...
    def crossed(self, other: XYZ) -> XYZ:
        ...
    def distance(self, other: XYZ) -> float:
        ...
    def divide(self, scalar: typing.SupportsFloat) -> None:
        ...
    def divided(self, scalar: typing.SupportsFloat) -> XYZ:
        ...
    def dot(self, other: XYZ) -> float:
        ...
    def dot_cross(self, coord1: XYZ, coord2: XYZ) -> float:
        ...
    def is_equal(self, other: XYZ, tolerance: typing.SupportsFloat) -> bool:
        ...
    def modulus(self) -> float:
        ...
    @typing.overload
    def multiplied(self, scalar: typing.SupportsFloat) -> XYZ:
        ...
    @typing.overload
    def multiplied(self, other: XYZ) -> XYZ:
        ...
    @typing.overload
    def multiply(self, scalar: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def multiply(self, other: XYZ) -> None:
        ...
    def normalize(self) -> None:
        ...
    def normalized(self) -> XYZ:
        ...
    def reverse(self) -> None:
        ...
    def reversed(self) -> XYZ:
        ...
    @typing.overload
    def set_coord(self, x: typing.SupportsFloat, y: typing.SupportsFloat, z: typing.SupportsFloat) -> None:
        ...
    @typing.overload
    def set_coord(self, index: typing.SupportsInt, value: typing.SupportsFloat) -> None:
        ...
    def square_modulus(self) -> float:
        ...
    def subtract(self, other: XYZ) -> None:
        ...
    def subtracted(self, other: XYZ) -> XYZ:
        ...
    @property
    def x(self) -> float:
        ...
    @x.setter
    def x(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def y(self) -> float:
        ...
    @y.setter
    def y(self, arg1: typing.SupportsFloat) -> None:
        ...
    @property
    def z(self) -> float:
        ...
    @z.setter
    def z(self, arg1: typing.SupportsFloat) -> None:
        ...
class gp:
    """
    Geometric processor utility class.
    
    Provides implementation of entities for algebraic calculations and
    basis analytic geometry. This class offers static methods for:
    - Tolerance criterion (Resolution)
    - Standard 3D geometric constants (Origin, unit directions, axes)
    - Standard 2D geometric constants
    """
    dx: typing.ClassVar[Dir]  # value = gp_Dir(1.000000, 0.000000, 0.000000)
    dx2d: typing.ClassVar[Dir2d]  # value = gp_Dir2d(1.000000, 0.000000)
    dy: typing.ClassVar[Dir]  # value = gp_Dir(0.000000, 1.000000, 0.000000)
    dy2d: typing.ClassVar[Dir2d]  # value = gp_Dir2d(0.000000, 1.000000)
    dz: typing.ClassVar[Dir]  # value = gp_Dir(0.000000, 0.000000, 1.000000)
    origin: typing.ClassVar[Pnt]  # value = gp_Pnt(0.000000, 0.000000, 0.000000)
    origin2d: typing.ClassVar[Pnt2d]  # value = gp_Pnt2d(0.000000, 0.000000)
    ox: typing.ClassVar[Ax1]  # value = gp_Ax1(loc=(0.000000, 0.000000, 0.000000), dir=(1.000000, 0.000000, 0.000000))
    ox2d: typing.ClassVar[Ax2d]  # value = gp_Ax2d(location=(0.000000, 0.000000), direction=(1.000000, 0.000000))
    oy: typing.ClassVar[Ax1]  # value = gp_Ax1(loc=(0.000000, 0.000000, 0.000000), dir=(0.000000, 1.000000, 0.000000))
    oy2d: typing.ClassVar[Ax2d]  # value = gp_Ax2d(location=(0.000000, 0.000000), direction=(0.000000, 1.000000))
    oz: typing.ClassVar[Ax1]  # value = gp_Ax1(loc=(0.000000, 0.000000, 0.000000), dir=(0.000000, 0.000000, 1.000000))
    xoy: typing.ClassVar[Ax2]  # value = gp_Ax2(loc=(0.000000, 0.000000, 0.000000), dir=(0.000000, 0.000000, 1.000000), xdir=(1.000000, 0.000000, -0.000000))
    yoz: typing.ClassVar[Ax2]  # value = gp_Ax2(loc=(0.000000, 0.000000, 0.000000), dir=(1.000000, 0.000000, 0.000000), xdir=(-0.000000, 1.000000, 0.000000))
    zox: typing.ClassVar[Ax2]  # value = gp_Ax2(loc=(0.000000, 0.000000, 0.000000), dir=(0.000000, 1.000000, 0.000000), xdir=(0.000000, -0.000000, 1.000000))
    @staticmethod
    def resolution() -> float:
        """
        Returns the tolerance criterion for geometric computations.
        
        This tolerance is used to determine when two numbers can be
        considered equal. Many class functions use this tolerance,
        for example, to avoid division by zero
        """
Ax1Mirror: TrsfForm  # value = <TrsfForm.Ax1Mirror: 4>
Ax2Mirror: TrsfForm  # value = <TrsfForm.Ax2Mirror: 5>
CompoundTrsf: TrsfForm  # value = <TrsfForm.CompoundTrsf: 7>
EulerAngles: EulerSequence  # value = <EulerSequence.EulerAngles: 0>
Extrinsic_XYX: EulerSequence  # value = <EulerSequence.Extrinsic_XYX: 14>
Extrinsic_XYZ: EulerSequence  # value = <EulerSequence.Extrinsic_XYZ: 2>
Extrinsic_XZX: EulerSequence  # value = <EulerSequence.Extrinsic_XZX: 15>
Extrinsic_XZY: EulerSequence  # value = <EulerSequence.Extrinsic_XZY: 3>
Extrinsic_YXY: EulerSequence  # value = <EulerSequence.Extrinsic_YXY: 17>
Extrinsic_YXZ: EulerSequence  # value = <EulerSequence.Extrinsic_YXZ: 5>
Extrinsic_YZX: EulerSequence  # value = <EulerSequence.Extrinsic_YZX: 4>
Extrinsic_YZY: EulerSequence  # value = <EulerSequence.Extrinsic_YZY: 16>
Extrinsic_ZXY: EulerSequence  # value = <EulerSequence.Extrinsic_ZXY: 6>
Extrinsic_ZXZ: EulerSequence  # value = <EulerSequence.Extrinsic_ZXZ: 19>
Extrinsic_ZYX: EulerSequence  # value = <EulerSequence.Extrinsic_ZYX: 7>
Extrinsic_ZYZ: EulerSequence  # value = <EulerSequence.Extrinsic_ZYZ: 18>
Identity: TrsfForm  # value = <TrsfForm.Identity: 0>
Intrinsic_XYX: EulerSequence  # value = <EulerSequence.Intrinsic_XYX: 20>
Intrinsic_XYZ: EulerSequence  # value = <EulerSequence.Intrinsic_XYZ: 8>
Intrinsic_XZX: EulerSequence  # value = <EulerSequence.Intrinsic_XZX: 21>
Intrinsic_XZY: EulerSequence  # value = <EulerSequence.Intrinsic_XZY: 9>
Intrinsic_YXY: EulerSequence  # value = <EulerSequence.Intrinsic_YXY: 23>
Intrinsic_YXZ: EulerSequence  # value = <EulerSequence.Intrinsic_YXZ: 11>
Intrinsic_YZX: EulerSequence  # value = <EulerSequence.Intrinsic_YZX: 10>
Intrinsic_YZY: EulerSequence  # value = <EulerSequence.Intrinsic_YZY: 22>
Intrinsic_ZXY: EulerSequence  # value = <EulerSequence.Intrinsic_ZXY: 12>
Intrinsic_ZXZ: EulerSequence  # value = <EulerSequence.Intrinsic_ZXZ: 24>
Intrinsic_ZYX: EulerSequence  # value = <EulerSequence.Intrinsic_ZYX: 13>
Intrinsic_ZYZ: EulerSequence  # value = <EulerSequence.Intrinsic_ZYZ: 25>
Other: TrsfForm  # value = <TrsfForm.Other: 8>
PntMirror: TrsfForm  # value = <TrsfForm.PntMirror: 3>
Rotation: TrsfForm  # value = <TrsfForm.Rotation: 1>
Scale: TrsfForm  # value = <TrsfForm.Scale: 6>
Translation: TrsfForm  # value = <TrsfForm.Translation: 2>
YawPitchRoll: EulerSequence  # value = <EulerSequence.YawPitchRoll: 1>
