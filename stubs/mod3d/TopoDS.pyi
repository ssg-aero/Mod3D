"""
Topology data structures
"""
from __future__ import annotations
import collections.abc
import mod3d.Geom
import mod3d.gp
import typing
__all__: list[str] = ['CompSolid', 'Compound', 'Edge', 'Face', 'Shape', 'Shell', 'Solid', 'TopLoc_Location', 'Vertex', 'Wire']
class CompSolid(Shape):
    """
    A composite solid
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty composite solid
        """
    @typing.overload
    def __init__(self, shape: Shape) -> None:
        """
        Creates a composite solid from a shape (casts to compsolid type)
        """
class Compound(Shape):
    """
    A compound is a collection of shapes
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty compound
        """
    @typing.overload
    def __init__(self, shape: Shape) -> None:
        """
        Creates a compound from a shape (casts to compound type)
        """
    @typing.overload
    def __init__(self, shapes: collections.abc.Sequence[Shape]) -> None:
        """
        Creates a compound from a list of shapes
        """
class Edge(Shape):
    """
    An edge (curve bounded by vertices)
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty edge
        """
    @typing.overload
    def __init__(self, shape: Shape) -> None:
        """
        Creates an edge from a shape (casts to edge type)
        """
    def curve(self) -> typing.Any:
        ...
class Face(Shape):
    """
    A face (surface bounded by wires)
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty face
        """
    @typing.overload
    def __init__(self, shape: Shape) -> None:
        """
        Creates a face from a shape (casts to face type)
        """
    def bound(self) -> TopoDS_Wire:
        ...
    def holes(self) -> list[TopoDS_Wire]:
        ...
    def surface(self) -> mod3d.Geom.Surface:
        ...
class Shape:
    """
    Base class for all topological shapes.
    
    A shape references an underlying shape with the potential to be given
    a location and an orientation. It consists of:
    - An underlying TShape with geometry
    - A Location for placement in local coordinate system
    - An Orientation in terms of geometry
    
    Note: A Shape is empty if it references an underlying shape which
    has an empty list of shapes
    """
    def __eq__(self, other: Shape) -> bool:
        """
        Equality operator (same as is_equal)
        """
    def __hash__(self) -> int:
        """
        Returns hash value for this shape
        """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates a null shape referring to nothing
        """
    @typing.overload
    def __init__(self, other: Shape) -> None:
        """
        Creates a copy of another shape.
        
        The copy shares the same TShape, location, and orientation
        """
    def __ne__(self, other: Shape) -> bool:
        """
        Inequality operator (same as is_not_equal)
        """
    def complement(self) -> None:
        """
        Complements the orientation.
        
        Uses TopAbs::Complement. Modifies this shape in place
        """
    def complemented(self) -> Shape:
        """
        Returns a copy with complemented orientation
        """
    def compose(self, orientation: TopAbs_Orientation) -> None:
        """
        Updates orientation by composition.
        
        Uses TopAbs::Compose. Modifies this shape in place
        """
    def composed(self, orientation: TopAbs_Orientation) -> Shape:
        """
        Returns a copy with orientation composed with the given orientation
        """
    def dump_json(self, stream: ..., std: ..., depth: typing.SupportsInt = -1) -> None:
        """
        Dumps the content of this shape to a stream in JSON format
        """
    def empty_copied(self) -> Shape:
        """
        Returns a new shape with no sub-shapes.
        
        The new shape has the same orientation and location,
        and a new TShape with the same geometry but no sub-shapes
        """
    def empty_copy(self) -> None:
        """
        Replaces this shape with a new shape.
        
        The new shape has the same orientation and location,
        and a new TShape with the same geometry but no sub-shapes
        """
    def is_equal(self, other: Shape) -> bool:
        """
        Returns True if two shapes are equal.
        
        Equal shapes share the same TShape, location, and orientation
        """
    def is_not_equal(self, other: Shape) -> bool:
        """
        Returns True if two shapes are not equal
        """
    def is_null(self) -> bool:
        """
        Returns True if this shape is null.
        
        A null shape references no underlying shape
        """
    def is_partner(self, other: Shape) -> bool:
        """
        Returns True if two shapes are partners.
        
        Partners share the same TShape but may have different
        locations and orientations
        """
    def is_same(self, other: Shape) -> bool:
        """
        Returns True if two shapes are the same.
        
        Same shapes share the same TShape and location but
        may have different orientations
        """
    def located(self, location: TopLoc_Location, raise_exc: bool = False) -> Shape:
        """
        Returns a copy of this shape with a new location.
        
        Parameters:
          location: The new local coordinate system
          raise_exc: If True, raises exception for transformations with scale
        
        Returns:
          New shape with the specified location
        """
    @typing.overload
    def move(self, position: TopLoc_Location, raise_exc: bool = False) -> None:
        """
        Multiplies the shape location by position.
        
        Modifies this shape in place
        """
    @typing.overload
    def move(self, transformation: mod3d.gp.Trsf, raise_exc: bool = False) -> None:
        """
        Multiplies the shape location by transformation.
        
        Modifies this shape in place
        """
    @typing.overload
    def moved(self, position: TopLoc_Location, raise_exc: bool = False) -> Shape:
        """
        Returns a copy with location multiplied by position
        """
    @typing.overload
    def moved(self, transformation: mod3d.gp.Trsf, raise_exc: bool = False) -> Shape:
        """
        Returns a copy with location multiplied by transformation
        """
    def nullify(self) -> None:
        """
        Destroys the reference to the underlying shape.
        
        As a result, this shape becomes null
        """
    def oriented(self, orientation: TopAbs_Orientation) -> Shape:
        """
        Returns a copy of this shape with a new orientation.
        
        Returns:
          New shape with the specified orientation
        """
    def reverse(self) -> None:
        """
        Reverses the orientation.
        
        Uses TopAbs::Reverse. Modifies this shape in place
        """
    def reversed(self) -> Shape:
        """
        Returns a copy with reversed orientation
        """
    def rotate(self, axis: mod3d.gp.Ax1, angle: typing.SupportsFloat) -> None:
        """
        Rotates the shape around the given axis by the given angle (in radians)
        """
    def rotated(self, axis: mod3d.gp.Ax1, angle: typing.SupportsFloat) -> Shape:
        """
        Returns a rotated copy of the shape around the given axis by the given angle (in radians)
        """
    def scale(self, center: mod3d.gp.Pnt, factor: typing.SupportsFloat) -> None:
        """
        Scales the shape by the given factor with respect to the given center point
        """
    def scaled(self, center: mod3d.gp.Pnt, factor: typing.SupportsFloat) -> Shape:
        """
        Returns a scaled copy of the shape by the given factor with respect to the given center point)
        """
    def set_location(self, location: TopLoc_Location, raise_exc: bool = False) -> None:
        """
        Sets the shape local coordinate system.
        
        Parameters:
          location: The new local coordinate system
          raise_exc: If True, raises exception for transformations with scale
        
        Raises:
          Standard_DomainError if transformation contains scaling and raise_exc=True
        """
    def translate(self, vector: mod3d.gp.Vec) -> None:
        """
        Translates the shape by the given vector
        """
    def translated(self, vector: mod3d.gp.Vec) -> Shape:
        """
        Returns a translated copy of the shape by the given vector
        """
    @property
    def checked(self) -> bool:
        """
        The checked flag
        """
    @checked.setter
    def checked(self, arg1: bool) -> None:
        ...
    @property
    def closed(self) -> bool:
        """
        The closedness flag
        """
    @closed.setter
    def closed(self, arg1: bool) -> None:
        ...
    @property
    def convex(self) -> bool:
        """
        The convexness flag
        """
    @convex.setter
    def convex(self, arg1: bool) -> None:
        ...
    @property
    def free(self) -> bool:
        """
        The free flag
        """
    @free.setter
    def free(self, arg1: bool) -> None:
        ...
    @property
    def infinite(self) -> bool:
        """
        The infinity flag
        """
    @infinite.setter
    def infinite(self, arg1: bool) -> None:
        ...
    @property
    def location(self) -> TopLoc_Location:
        """
        The shape's local coordinate system.
        
        Get/set the transformation applied to this shape
        """
    @location.setter
    def location(self, arg1: TopLoc_Location) -> None:
        ...
    @property
    def locked(self) -> bool:
        """
        The locked flag
        """
    @locked.setter
    def locked(self, arg1: bool) -> None:
        ...
    @property
    def modified(self) -> bool:
        """
        The modification flag
        """
    @modified.setter
    def modified(self, arg1: bool) -> None:
        ...
    @property
    def nb_children(self) -> int:
        """
        Returns the number of direct sub-shapes (children).
        
        Use TopoDS.Iterator to access the sub-shapes
        """
    @property
    def orientable(self) -> bool:
        """
        The orientability flag
        """
    @orientable.setter
    def orientable(self, arg1: bool) -> None:
        ...
    @property
    def orientation(self) -> TopAbs_Orientation:
        """
        The shape's orientation.
        
        Get/set the orientation (FORWARD, REVERSED, INTERNAL, EXTERNAL)
        """
    @orientation.setter
    def orientation(self, arg1: TopAbs_Orientation) -> None:
        ...
    @property
    def shape_type(self) -> TopAbs_ShapeEnum:
        """
        Returns the type of this shape.
        
        Possible values: COMPOUND, COMPSOLID, SOLID, SHELL, FACE,
        WIRE, EDGE, VERTEX, SHAPE
        """
    @property
    def tshape(self) -> TopoDS_TShape:
        """
        Returns a handle to the actual shape implementation
        """
class Shell(Shape):
    """
    A shell (collection of connected faces)
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty shell
        """
    @typing.overload
    def __init__(self, shape: Shape) -> None:
        """
        Creates a shell from a shape (casts to shell type)
        """
class Solid(Shape):
    """
    A solid shape
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty solid
        """
    @typing.overload
    def __init__(self, shape: Shape) -> None:
        """
        Creates a solid from a shape (casts to solid type)
        """
class TopLoc_Location:
    """
    A composite local coordinate system.
    
    Location represents a coordinate transformation comprised of a series
    of elementary reference coordinates (TopLoc_Datum3D objects) and the
    powers to which these objects are raised. It is used to define the
    placement of shapes in 3D space.
    
    Example:
      loc = TopLoc.Location(gp.Trsf())  # Identity location
      loc2 = loc * other_loc  # Compose transformations
    """
    def __eq__(self, other: TopLoc_Location) -> bool:
        """
        Equality operator (same as is_equal)
        """
    def __hash__(self) -> int:
        """
        Returns a hash value for this location.
        
        Used with map tables to store and retrieve the object
        """
    @typing.overload
    def __init__(self) -> None:
        """
        Constructs an empty local coordinate system (identity transformation).
        
        Note: A Location constructed from a default datum is said to be 'empty'
        """
    @typing.overload
    def __init__(self, transformation: mod3d.gp.Trsf) -> None:
        """
        Constructs a location from a transformation.
        
        Parameters:
          transformation: The gp_Trsf defining the coordinate system
        
        The transformation T invokes in turn a TopLoc_Datum3D object.
        
        Raises:
          Standard_ConstructionError if T does not represent a 3D coordinate system
        """
    def __mul__(self, arg0: TopLoc_Location) -> TopLoc_Location:
        """
        Multiply two locations (composition of transformations)
        """
    def __ne__(self, other: TopLoc_Location) -> bool:
        """
        Inequality operator (same as is_different)
        """
    def __pow__(self, power: typing.SupportsInt) -> TopLoc_Location:
        """
        Raise location to a power (supports negative exponents)
        """
    def __truediv__(self, arg0: TopLoc_Location) -> TopLoc_Location:
        """
        Divide two locations (equivalent to self * other.inverted())
        """
    def clear(self) -> None:
        """
        Clears the internal items, resetting to empty state
        """
    def divided(self, other: TopLoc_Location) -> TopLoc_Location:
        """
        Returns self / other.
        
        Equivalent to self * other.inverted()
        """
    def dump_json(self, stream: ..., std: ..., depth: typing.SupportsInt = -1) -> None:
        """
        Dumps the content of this location to a stream in JSON format
        """
    def identity(self) -> None:
        """
        Resets this location to the identity transformation
        """
    def inverted(self) -> TopLoc_Location:
        """
        Returns the inverse of this location.
        
        The result satisfies: self * self.inverted() == Identity
        """
    def is_different(self, other: TopLoc_Location) -> bool:
        """
        Returns True if locations have different elementary data
        """
    def is_equal(self, other: TopLoc_Location) -> bool:
        """
        Returns True if locations have the same elementary data.
        
        Checks if both contain the same series of TopLoc_Datum3D
        and respective powers
        """
    def is_identity(self) -> bool:
        """
        Returns True if this location equals the identity transformation
        """
    def multiplied(self, other: TopLoc_Location) -> TopLoc_Location:
        """
        Returns self * other.
        
        The elementary datums are concatenated.
        
        Parameters:
          other: The location to multiply with
        """
    def powered(self, power: typing.SupportsInt) -> TopLoc_Location:
        """
        Returns this location raised to the given power.
        
        Parameters:
          power: The exponent (can be negative or zero)
        
        If power is zero, returns Identity.
        Negative powers return the inverse raised to |power|
        """
    def predivided(self, other: TopLoc_Location) -> TopLoc_Location:
        """
        Returns other.inverted() * self.
        
        Pre-division operation
        """
    def shallow_dump(self, stream: ..., std: ...) -> None:
        """
        Prints the contents of this location on the stream
        """
    @property
    def first_datum(self) -> TopLoc_Datum3D:
        """
        Returns the first elementary datum.
        
        Use next_location recursively to access other data.
        
        Raises:
          Standard_NoSuchObject if this location is empty
        """
    @property
    def first_power(self) -> int:
        """
        Returns the power elevation of the first elementary datum.
        
        Raises:
          Standard_NoSuchObject if this location is empty
        """
    @property
    def next_location(self) -> TopLoc_Location:
        """
        Returns this location without the first datum.
        
        Relation: self = next_location * first_datum ^ first_power
        
        Raises:
          Standard_NoSuchObject if this location is empty
        """
    @property
    def transformation(self) -> mod3d.gp.Trsf:
        """
        The transformation associated with this coordinate system.
        
        Returns the gp_Trsf representation of this location
        """
class Vertex(Shape):
    """
    A vertex (topological point)
    """
    point: mod3d.gp.Pnt
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty vertex
        """
    @typing.overload
    def __init__(self, shape: Shape) -> None:
        """
        Creates a vertex from a shape (casts to vertex type)
        """
    @typing.overload
    def __init__(self, point: mod3d.gp.Pnt, tolerance: typing.SupportsFloat = 1e-07) -> None:
        ...
    @typing.overload
    def __init__(self, x: typing.SupportsFloat, y: typing.SupportsFloat, z: typing.SupportsFloat, tolerance: typing.SupportsFloat = 1e-07) -> None:
        ...
    def x(self) -> float:
        ...
    def y(self) -> float:
        ...
    def z(self) -> float:
        ...
class Wire(Shape):
    """
    A wire (sequence of connected edges)
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty wire
        """
    @typing.overload
    def __init__(self, shape: Shape) -> None:
        """
        Creates a wire from a shape (casts to wire type)
        """
