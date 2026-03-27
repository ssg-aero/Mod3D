"""
Rendering and tessellation module
"""
from __future__ import annotations
import mod3d.Geom
import mod3d.TopoDS
import numpy
import numpy.typing
import typing
__all__: list[str] = ['DEFAULT', 'Delabella', 'MeshAlgoType', 'MeshParameters', 'Watson', 'extract_curve_tessellation', 'extract_tessellation']
class MeshAlgoType:
    """
    Members:
    
      DEFAULT : Use global default (Watson or CSF_MeshAlgo)
    
      Watson : 2D Delaunay triangulation based on Watson algorithm
    
      Delabella : 2D Delaunay triangulation based on Delabella algorithm
    """
    DEFAULT: typing.ClassVar[MeshAlgoType]  # value = <MeshAlgoType.DEFAULT: -1>
    Delabella: typing.ClassVar[MeshAlgoType]  # value = <MeshAlgoType.Delabella: 1>
    Watson: typing.ClassVar[MeshAlgoType]  # value = <MeshAlgoType.Watson: 0>
    __members__: typing.ClassVar[dict[str, MeshAlgoType]]  # value = {'DEFAULT': <MeshAlgoType.DEFAULT: -1>, 'Watson': <MeshAlgoType.Watson: 0>, 'Delabella': <MeshAlgoType.Delabella: 1>}
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
class MeshParameters:
    """
    Structure storing meshing parameters
    """
    @staticmethod
    def RelMinSize() -> float:
        """
        Returns factor used to compute default value of MinSize from deflection
        """
    def __init__(self) -> None:
        ...
    @property
    def AdjustMinSize(self) -> bool:
        """
        Enables/disables local adjustment of min size depending on edge size
        """
    @AdjustMinSize.setter
    def AdjustMinSize(self, arg0: bool) -> None:
        ...
    @property
    def AllowQualityDecrease(self) -> bool:
        """
        Allows/forbids the decrease of the quality of the generated mesh over existing one
        """
    @AllowQualityDecrease.setter
    def AllowQualityDecrease(self, arg0: bool) -> None:
        ...
    @property
    def Angle(self) -> float:
        """
        Angular deflection used to tessellate the boundary edges
        """
    @Angle.setter
    def Angle(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def AngleInterior(self) -> float:
        """
        Angular deflection used to tessellate the face interior
        """
    @AngleInterior.setter
    def AngleInterior(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def CleanModel(self) -> bool:
        """
        Cleans temporary data model when algorithm is finished
        """
    @CleanModel.setter
    def CleanModel(self, arg0: bool) -> None:
        ...
    @property
    def ControlSurfaceDeflection(self) -> bool:
        """
        Parameter to check the deviation of triangulation and interior of the face
        """
    @ControlSurfaceDeflection.setter
    def ControlSurfaceDeflection(self, arg0: bool) -> None:
        ...
    @property
    def Deflection(self) -> float:
        """
        Linear deflection used to tessellate the boundary edges
        """
    @Deflection.setter
    def Deflection(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def DeflectionInterior(self) -> float:
        """
        Linear deflection used to tessellate the face interior
        """
    @DeflectionInterior.setter
    def DeflectionInterior(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def EnableControlSurfaceDeflectionAllSurfaces(self) -> bool:
        """
        Enables/disables check triggered by ControlSurfaceDeflection for all surface types
        """
    @EnableControlSurfaceDeflectionAllSurfaces.setter
    def EnableControlSurfaceDeflectionAllSurfaces(self, arg0: bool) -> None:
        ...
    @property
    def ForceFaceDeflection(self) -> bool:
        """
        Enables/disables usage of shape tolerances for computing face deflection
        """
    @ForceFaceDeflection.setter
    def ForceFaceDeflection(self, arg0: bool) -> None:
        ...
    @property
    def InParallel(self) -> bool:
        """
        Switches on/off multi-thread computation
        """
    @InParallel.setter
    def InParallel(self, arg0: bool) -> None:
        ...
    @property
    def InternalVerticesMode(self) -> bool:
        """
        Mode to take or not to take internal face vertices into account
        """
    @InternalVerticesMode.setter
    def InternalVerticesMode(self, arg0: bool) -> None:
        ...
    @property
    def MeshAlgo(self) -> MeshAlgoType:
        """
        2D Delaunay triangulation algorithm factory to use
        """
    @MeshAlgo.setter
    def MeshAlgo(self, arg0: MeshAlgoType) -> None:
        ...
    @property
    def MinSize(self) -> float:
        """
        Minimum size parameter limiting size of triangle's edges
        """
    @MinSize.setter
    def MinSize(self, arg0: typing.SupportsFloat) -> None:
        ...
    @property
    def Relative(self) -> bool:
        """
        Switches on/off relative computation of edge tolerance
        """
    @Relative.setter
    def Relative(self, arg0: bool) -> None:
        ...
@typing.overload
def extract_curve_tessellation(curve: mod3d.Geom.Curve, linear_deflection: typing.SupportsFloat) -> tuple[numpy.typing.NDArray[numpy.int32], numpy.typing.NDArray[numpy.float64]]:
    """
    Extracts tessellation data from the given curve. Previous tessellation data will be cleared.
    
    Parameters:
      curve: The Geom_Curve to tessellate
      linear_deflection: The linear deflection value for meshing
    
    Returns:
      A tuple containing:
        - vertex indices (numpy array of int, shape (n_points,))
        - vertex positions (numpy array of float, shape (n_points, 3))
    """
@typing.overload
def extract_curve_tessellation(edge: mod3d.TopoDS.Edge, linear_deflection: typing.SupportsFloat) -> tuple[numpy.typing.NDArray[numpy.int32], numpy.typing.NDArray[numpy.float64]]:
    """
    Extracts tessellation data from the given edge.
    
    Parameters:
      edge: The TopoDS_Edge to tessellate
      linear_deflection: The linear deflection value for meshing
    
    Returns:
      A tuple containing:
        - vertex indices (numpy array of int, shape (n_points,))
        - vertex positions (numpy array of float, shape (n_points, 3))
    """
@typing.overload
def extract_curve_tessellation(wire: mod3d.TopoDS.Wire, linear_deflection: typing.SupportsFloat) -> tuple[numpy.typing.NDArray[numpy.int32], numpy.typing.NDArray[numpy.float64]]:
    """
    Extracts tessellation data from the given wire.
    
    Parameters:
      wire: The TopoDS_Wire to tessellate
      linear_deflection: The linear deflection value for meshing
    
    Returns:
      A tuple containing:
        - vertex indices (numpy array of int, shape (n_points,))
        - vertex positions (numpy array of float, shape (n_points, 3))
    """
@typing.overload
def extract_tessellation(shape: mod3d.TopoDS.Shape, linear_deflection: typing.SupportsFloat, is_relative: bool = False, angle_deflection: typing.SupportsFloat = 30.0, parallel: bool = True, compute_normals: bool = True) -> tuple[list[tuple[numpy.typing.NDArray[numpy.int32], numpy.typing.NDArray[numpy.float64], numpy.typing.NDArray[numpy.float64] | None, numpy.typing.NDArray[numpy.float64] | None]], list[tuple[numpy.typing.NDArray[numpy.int32], numpy.typing.NDArray[numpy.float64]]]]:
    """
    Extracts tessellation data from the given shape. Previous tessellation data will be cleared.
    
    Parameters:
      shape: The TopoDS_Shape to tessellate
      linear_deflection: The linear deflection value for meshing
      is_relative: If True, linear deflection is relative to shape size (default: False)
      angle_deflection: The angular deflection in degrees (default: 30.0)
      parallel: If True, use parallel processing (default: True)
      compute_normals: If True, compute vertex normals (default: True)
    
    Returns:
      A list of tuples for each face, each containing:
        - triangle indices (numpy array of int, shape (n_triangles, 3))
        - vertex positions (numpy array of float, shape (n_vertices, 3))
        - vertex normals (numpy array of float, shape (n_vertices, 3))
        - vertex UV coordinates (numpy array of float, shape (n_vertices, 2))
    """
@typing.overload
def extract_tessellation(shape: mod3d.TopoDS.Shape, params: MeshParameters, compute_normals: bool = True) -> tuple[list[tuple[numpy.typing.NDArray[numpy.int32], numpy.typing.NDArray[numpy.float64], numpy.typing.NDArray[numpy.float64] | None, numpy.typing.NDArray[numpy.float64] | None]], list[tuple[numpy.typing.NDArray[numpy.int32], numpy.typing.NDArray[numpy.float64]]]]:
    """
    Extracts tessellation data from the given shape using specified meshing parameters.
    
    Parameters:
      shape: The TopoDS_Shape to tessellate
      params: An instance of MeshParameters containing meshing settings
      compute_normals: If True, compute vertex normals (default: True)
    
    Returns:
      A list of tuples for each face, each containing:
        - triangle indices (numpy array of int, shape (n_triangles, 3))
        - vertex positions (numpy array of float, shape (n_vertices, 3))
        - vertex normals (numpy array of float, shape (n_vertices, 3))
        - vertex UV coordinates (numpy array of float, shape (n_vertices, 2))
    """
DEFAULT: MeshAlgoType  # value = <MeshAlgoType.DEFAULT: -1>
Delabella: MeshAlgoType  # value = <MeshAlgoType.Delabella: 1>
Watson: MeshAlgoType  # value = <MeshAlgoType.Watson: 0>
