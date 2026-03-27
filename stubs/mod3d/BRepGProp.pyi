"""
BRep geometric properties module
"""
from __future__ import annotations
import mod3d.TopoDS
import mod3d.gp
import typing
__all__: list[str] = ['BRepGProp', 'GProps', 'PrincipalProps', 'area', 'length', 'volume']
class BRepGProp:
    """
    Provides static methods to compute global properties of BRep shapes:
    - LinearProperties: computes properties from edges (length)
    - SurfaceProperties: computes properties from faces (area)
    - VolumeProperties: computes properties from solids (volume)
    """
    @staticmethod
    def linear_properties(shape: mod3d.TopoDS.Shape, skip_shared: bool = False) -> GProps:
        """
        Computes the linear properties (length, center of mass, inertia) from the edges.
        If skip_shared is true, edges shared by two or more faces are skipped.
        Returns a GProps object containing the computed properties.
        """
    @staticmethod
    def surface_properties(shape: mod3d.TopoDS.Shape, skip_shared: bool = False) -> GProps:
        """
        Computes the surface properties (area, center of mass, inertia) from the faces.
        If skip_shared is true, faces shared by two or more shells are skipped.
        Returns a GProps object containing the computed properties.
        """
    @staticmethod
    def surface_properties_eps(shape: mod3d.TopoDS.Shape, eps: typing.SupportsFloat, skip_shared: bool = False) -> tuple:
        """
        Computes surface properties with a specified precision.
        Args:
            shape: The shape to analyze
            eps: Relative precision for integration
            skip_shared: If true, skip shared faces
        Returns:
            tuple: (GProps, error) where error is the relative computation error.
        """
    @staticmethod
    def volume_properties(shape: mod3d.TopoDS.Shape, only_closed: bool = False) -> GProps:
        """
        Computes the volume properties (volume, center of mass, inertia) from the solids.
        If only_closed is true, only closed shells are considered.
        Returns a GProps object containing the computed properties.
        """
    @staticmethod
    def volume_properties_eps(shape: mod3d.TopoDS.Shape, eps: typing.SupportsFloat, only_closed: bool = False) -> tuple:
        """
        Computes volume properties with a specified precision.
        Args:
            shape: The shape to analyze
            eps: Relative precision for integration
            only_closed: If true, only closed shells are considered
        Returns:
            tuple: (GProps, error) where error is the relative computation error.
        """
    @staticmethod
    def volume_properties_gk(shape: mod3d.TopoDS.Shape, eps: typing.SupportsFloat = 0.001, only_closed: bool = False, skip_shared: bool = False) -> tuple:
        """
        Computes volume properties using Gauss-Kronrod integration.
        This method provides adaptive integration with better error estimation.
        Args:
            shape: The shape to analyze
            eps: Relative precision (default 0.001)
            only_closed: If true, only closed shells are considered
            skip_shared: If true, skip shared faces
        Returns:
            tuple: (GProps, error) where error is the relative computation error.
        """
class GProps:
    """
    Implements a general mechanism to compute the global properties of a
    compound geometric system in 3D space. The global properties computed are:
    - the mass (length, area, or volume depending on the type)
    - the centre of mass
    - the moments of inertia (static moments and quadratic moments)
    - the moment about an axis
    - the radius of gyration about an axis
    - the principal properties of inertia
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates a GProps with the origin (0,0,0) as reference point
        """
    @typing.overload
    def __init__(self, system_location: mod3d.gp.Pnt) -> None:
        """
        Creates a GProps with a specified reference point.
        For more accuracy, the reference point should be close to the system location,
        e.g., near the center of mass.
        """
    def add(self, item: GProps, density: typing.SupportsFloat = 1.0) -> None:
        """
        Adds the global properties of item to this framework.
        The density parameter (default 1.0) is used as the density of the system
        analyzed by item.
        """
    def moment_of_inertia(self, axis: mod3d.gp.Ax1) -> float:
        """
        Computes the moment of inertia about the given axis.
        """
    def radius_of_gyration(self, axis: mod3d.gp.Ax1) -> float:
        """
        Returns the radius of gyration about the given axis.
        """
    def static_moments(self) -> tuple:
        """
        Returns (Ix, Iy, Iz), the static moments of inertia about the three Cartesian axes.
        """
    @property
    def centre_of_mass(self) -> mod3d.gp.Pnt:
        """
        Returns the center of mass (or center of gravity in a uniform field).
        The coordinates are expressed in the absolute Cartesian coordinate system.
        """
    @property
    def mass(self) -> float:
        """
        Returns the mass of the current system.
        If no density is attached to the components, the returned value corresponds to:
        - the total length of edges (for LinearProperties)
        - the total area of faces (for SurfaceProperties)
        - the total volume of solids (for VolumeProperties)
        """
    @property
    def matrix_of_inertia(self) -> mod3d.gp.Mat:
        """
        Returns the matrix of inertia (symmetrical).
        The coefficients are the quadratic moments of inertia:
        | Ixx  Ixy  Ixz |
        | Ixy  Iyy  Iyz |
        | Ixz  Iyz  Izz |
        The matrix is returned in the central coordinate system (G, Gx, Gy, Gz)
        where G is the centre of mass.
        """
    @property
    def principal_properties(self) -> GProp_PrincipalProps:
        """
        Computes and returns the principal properties of inertia.
        The principal axes are axes for which the products of inertia are zero
        (i.e., the matrix of inertia is diagonal). Their origin coincides with
        the center of mass.
        """
class PrincipalProps:
    """
    A framework for presenting the principal properties of inertia of a system.
    """
    def first_axis_of_inertia(self) -> mod3d.gp.Vec:
        """
        Returns the first principal axis of inertia.
        """
    def has_symmetry_axis(self) -> bool:
        """
        Returns true if the geometric system has an axis of symmetry.
        """
    def has_symmetry_point(self) -> bool:
        """
        Returns true if the geometric system has a point of symmetry.
        """
    def moments(self) -> tuple:
        """
        Returns (Ixx, Iyy, Izz), the principal moments of inertia.
        """
    def radii_of_gyration(self) -> tuple:
        """
        Returns (Rxx, Ryy, Rzz), the principal radii of gyration.
        """
    def second_axis_of_inertia(self) -> mod3d.gp.Vec:
        """
        Returns the second principal axis of inertia.
        """
    def third_axis_of_inertia(self) -> mod3d.gp.Vec:
        """
        Returns the third principal axis of inertia.
        """
def area(shape: mod3d.TopoDS.Shape) -> float:
    """
    Compute and return the surface area of the given shape.
    """
def length(shape: mod3d.TopoDS.Shape) -> float:
    """
    Compute and return the total length of the given shape.
    """
def volume(shape: mod3d.TopoDS.Shape) -> float:
    """
    Compute and return the volume of the given shape.
    """
