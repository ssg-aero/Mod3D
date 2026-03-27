"""
STEP control module
"""
from __future__ import annotations
import mod3d.TopoDS
import typing
__all__: list[str] = ['AsIs', 'BrepWithVoids', 'FacetedBrep', 'FacetedBrepAndBrepWithVoids', 'GeometricCurveSet', 'Hybrid', 'ManifoldSolidBrep', 'Reader', 'RetDone', 'RetError', 'RetFail', 'RetStop', 'RetVoid', 'ReturnStatus', 'ShellBasedSurfaceModel', 'StepModelType', 'Writer']
class Reader:
    """
    Reads STEP files and translates their contents into Open CASCADE shapes.
    
    Inherits from XSControl_Reader. The workflow is:
    
    1. :meth:`read_file` — load a STEP file into memory.
    2. :meth:`transfer_roots` or :meth:`transfer_root` — translate entities to shapes.
    3. :meth:`shape` / :meth:`one_shape` — retrieve translated shapes.
    
    Typical use::
    
        reader = mod3d.Reader()
        status = reader.read_file('model.step')
        reader.transfer_roots()
        shape = reader.one_shape()
    """
    def __init__(self) -> None:
        """
        Creates a Reader with an empty STEP model.
        """
    def clear_shapes(self) -> None:
        """
        Clears the list of accumulated result shapes.
        
        Call this before starting a new batch of translations.
        """
    def read_file(self, filename: str) -> ReturnStatus:
        """
        Loads a STEP file into memory.
        
        Parameters
        ----------
        filename : str
            Path to the STEP file.
        
        Returns
        -------
        ReturnStatus
            ``RetDone`` on success, ``RetError`` / ``RetFail`` otherwise.
        """
    def set_system_length_unit(self, length_unit: typing.SupportsFloat) -> None:
        """
        Sets the system length unit used during transfer.
        
        Parameters
        ----------
        length_unit : float
            Length unit value.
        """
    def shape(self, num: typing.SupportsInt = 1) -> mod3d.TopoDS.Shape:
        """
        Returns the shape produced at rank *num*.
        
        Parameters
        ----------
        num : int, optional
            Rank of the result shape (1-based). Default is 1.
        
        Returns
        -------
        TopoDS_Shape
        """
    def system_length_unit(self) -> float:
        """
        Returns the system length unit used during transfer.
        
        Returns
        -------
        float
        """
    def transfer_root(self, num: typing.SupportsInt = 1) -> bool:
        """
        Translates a single root entity identified by its rank.
        
        Parameters
        ----------
        num : int, optional
            Rank of the root to translate (1-based). Default is 1.
        
        Returns
        -------
        bool
            ``True`` if a shape was produced.
        """
    def transfer_roots(self) -> int:
        """
        Translates all transferable root entities.
        
        Clears existing output shapes first.
        
        Returns
        -------
        int
            Number of successfully translated roots.
        """
    @property
    def file_units(self) -> tuple:
        """
        Returns the unit names found in the STEP file.
        
        Returns
        -------
        tuple[list[str], list[str], list[str]]
            A triple ``(length_units, angle_units, solid_angle_units)``.
        """
    @property
    def nb_roots_for_transfer(self) -> int:
        """
        Returns the number of root entities that are candidates for translation.
        
        Returns
        -------
        int
        """
    @property
    def nb_shapes(self) -> int:
        """
        Returns the number of shapes produced by translation.
        
        Returns
        -------
        int
        """
    @property
    def one_shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns all results as a single compound shape.
        
        Returns a null shape if nothing was translated, the shape directly if
        there is exactly one result, or a compound if there are multiple results.
        
        Returns
        -------
        TopoDS_Shape
        """
    @property
    def step_model(self) -> StepData_StepModel:
        """
        Returns the loaded STEP model, or ``None`` if no file has been read.
        
        Returns
        -------
        StepData_StepModel | None
        """
class ReturnStatus:
    """
    Return status for STEP I/O operations.
    
    Members:
    
      RetVoid : No result.
    
      RetDone : Operation succeeded.
    
      RetError : Error occurred.
    
      RetFail : Operation failed.
    
      RetStop : Operation stopped.
    """
    RetDone: typing.ClassVar[ReturnStatus]  # value = <ReturnStatus.RetDone: 1>
    RetError: typing.ClassVar[ReturnStatus]  # value = <ReturnStatus.RetError: 2>
    RetFail: typing.ClassVar[ReturnStatus]  # value = <ReturnStatus.RetFail: 3>
    RetStop: typing.ClassVar[ReturnStatus]  # value = <ReturnStatus.RetStop: 4>
    RetVoid: typing.ClassVar[ReturnStatus]  # value = <ReturnStatus.RetVoid: 0>
    __members__: typing.ClassVar[dict[str, ReturnStatus]]  # value = {'RetVoid': <ReturnStatus.RetVoid: 0>, 'RetDone': <ReturnStatus.RetDone: 1>, 'RetError': <ReturnStatus.RetError: 2>, 'RetFail': <ReturnStatus.RetFail: 3>, 'RetStop': <ReturnStatus.RetStop: 4>}
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
class StepModelType:
    """
    Defines the STEP entity type to be output during translation.
    
    Members:
    
      AsIs : Translates a shape to its highest possible STEP representation.
    
      ManifoldSolidBrep : Translates to a manifold_solid_brep or brep_with_voids entity.
    
      BrepWithVoids : Translates to a brep_with_voids entity.
    
      FacetedBrep : Translates to a faceted_brep entity.
    
      FacetedBrepAndBrepWithVoids : Translates to faceted_brep and brep_with_voids.
    
      ShellBasedSurfaceModel : Translates to a shell_based_surface_model entity.
    
      GeometricCurveSet : Translates to a geometric_curve_set entity.
    
      Hybrid : Hybrid representation.
    """
    AsIs: typing.ClassVar[StepModelType]  # value = <StepModelType.AsIs: 0>
    BrepWithVoids: typing.ClassVar[StepModelType]  # value = <StepModelType.BrepWithVoids: 2>
    FacetedBrep: typing.ClassVar[StepModelType]  # value = <StepModelType.FacetedBrep: 3>
    FacetedBrepAndBrepWithVoids: typing.ClassVar[StepModelType]  # value = <StepModelType.FacetedBrepAndBrepWithVoids: 4>
    GeometricCurveSet: typing.ClassVar[StepModelType]  # value = <StepModelType.GeometricCurveSet: 6>
    Hybrid: typing.ClassVar[StepModelType]  # value = <StepModelType.Hybrid: 7>
    ManifoldSolidBrep: typing.ClassVar[StepModelType]  # value = <StepModelType.ManifoldSolidBrep: 1>
    ShellBasedSurfaceModel: typing.ClassVar[StepModelType]  # value = <StepModelType.ShellBasedSurfaceModel: 5>
    __members__: typing.ClassVar[dict[str, StepModelType]]  # value = {'AsIs': <StepModelType.AsIs: 0>, 'ManifoldSolidBrep': <StepModelType.ManifoldSolidBrep: 1>, 'BrepWithVoids': <StepModelType.BrepWithVoids: 2>, 'FacetedBrep': <StepModelType.FacetedBrep: 3>, 'FacetedBrepAndBrepWithVoids': <StepModelType.FacetedBrepAndBrepWithVoids: 4>, 'ShellBasedSurfaceModel': <StepModelType.ShellBasedSurfaceModel: 5>, 'GeometricCurveSet': <StepModelType.GeometricCurveSet: 6>, 'Hybrid': <StepModelType.Hybrid: 7>}
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
class Writer:
    """
    Creates and writes STEP files from Open CASCADE models.
    
    A STEP file can be written to an existing file or a new one.
    Translation can be performed in one or several operations.
    Each operation produces a distinct root entity in the STEP file.
    
    Typical use::
    
        writer = mod3d.Writer()
        writer.transfer(shape, mod3d.StepModelType.AsIs)
        writer.write('output.step')
    """
    def __init__(self) -> None:
        """
        Creates a Writer from scratch.
        """
    def model(self, newone: bool = False) -> StepData_StepModel:
        """
        Returns the produced STEP model.
        
        Produces a new model if none exists yet or if *newone* is ``True``.
        This allows editing product or header data before writing.
        
        Parameters
        ----------
        newone : bool, optional
            If ``True``, force creation of a fresh model. Default is ``False``.
        """
    def print_stats_transfer(self, what: typing.SupportsInt, mode: typing.SupportsInt = 0) -> None:
        """
        Displays statistics for the last translation.
        
        Parameters
        ----------
        what : int
            Kind of statistics:
        
            - 0 — General statistics (number of translated roots, warnings, fails).
            - 1 — Root results.
            - 2 — Statistics for all checked entities.
            - 3 — List of translated entities.
            - 4 — Warning and fail messages.
            - 5 — Fail messages only.
        mode : int, optional
            Detail level when *what* is 1–3:
        
            - 0 — List numbers of STEP entities.
            - 1 — Number, identifier, type and result type per entity.
            - 2 — Maximum information per entity (checks).
            - 3 — Number of entities per STEP entity type.
        """
    def set_tolerance(self, tol: typing.SupportsFloat) -> None:
        """
        Sets a length-measure value written to uncertainty-measure-with-unit
        when the next shape is translated.
        
        Parameters
        ----------
        tol : float
            Tolerance value in current length unit.
        """
    def transfer(self, shape: mod3d.TopoDS.Shape, mode: StepModelType, compgraph: bool = True) -> ReturnStatus:
        """
        Translates a shape to a STEP entity and adds it to the model.
        
        Parameters
        ----------
        shape : TopoDS_Shape
            The shape to translate.
        mode : StepModelType
            STEP representation type (``AsIs``, ``ManifoldSolidBrep``, etc.).
        compgraph : bool, optional
            Whether to compute the graph. Default is ``True``.
        
        Returns
        -------
        ReturnStatus
            ``RetDone`` on success.
        """
    def unset_tolerance(self) -> None:
        """
        Unsets the tolerance previously forced by :meth:`set_tolerance`.
        """
    def write(self, filename: str) -> ReturnStatus:
        """
        Writes the STEP model to a file.
        
        Parameters
        ----------
        filename : str
            Path of the output STEP file.
        
        Returns
        -------
        ReturnStatus
            ``RetDone`` on success.
        """
AsIs: StepModelType  # value = <StepModelType.AsIs: 0>
BrepWithVoids: StepModelType  # value = <StepModelType.BrepWithVoids: 2>
FacetedBrep: StepModelType  # value = <StepModelType.FacetedBrep: 3>
FacetedBrepAndBrepWithVoids: StepModelType  # value = <StepModelType.FacetedBrepAndBrepWithVoids: 4>
GeometricCurveSet: StepModelType  # value = <StepModelType.GeometricCurveSet: 6>
Hybrid: StepModelType  # value = <StepModelType.Hybrid: 7>
ManifoldSolidBrep: StepModelType  # value = <StepModelType.ManifoldSolidBrep: 1>
RetDone: ReturnStatus  # value = <ReturnStatus.RetDone: 1>
RetError: ReturnStatus  # value = <ReturnStatus.RetError: 2>
RetFail: ReturnStatus  # value = <ReturnStatus.RetFail: 3>
RetStop: ReturnStatus  # value = <ReturnStatus.RetStop: 4>
RetVoid: ReturnStatus  # value = <ReturnStatus.RetVoid: 0>
ShellBasedSurfaceModel: StepModelType  # value = <StepModelType.ShellBasedSurfaceModel: 5>
