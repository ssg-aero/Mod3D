"""
GeomFill module
"""
from __future__ import annotations
import typing
__all__: list[str] = ['ConstantNormal', 'CorrectedFrenet', 'Darboux', 'DiscreteTrihedron', 'Fixed', 'Frenet', 'GuideAC', 'GuideACWithContact', 'GuidePlan', 'GuidePlanWithContact', 'Trihedron']
class Trihedron:
    """
    Members:
    
      CorrectedFrenet
    
      Fixed
    
      Frenet
    
      ConstantNormal
    
      Darboux
    
      GuideAC
    
      GuidePlan
    
      GuideACWithContact
    
      GuidePlanWithContact
    
      DiscreteTrihedron
    """
    ConstantNormal: typing.ClassVar[Trihedron]  # value = <Trihedron.ConstantNormal: 3>
    CorrectedFrenet: typing.ClassVar[Trihedron]  # value = <Trihedron.CorrectedFrenet: 0>
    Darboux: typing.ClassVar[Trihedron]  # value = <Trihedron.Darboux: 4>
    DiscreteTrihedron: typing.ClassVar[Trihedron]  # value = <Trihedron.DiscreteTrihedron: 9>
    Fixed: typing.ClassVar[Trihedron]  # value = <Trihedron.Fixed: 1>
    Frenet: typing.ClassVar[Trihedron]  # value = <Trihedron.Frenet: 2>
    GuideAC: typing.ClassVar[Trihedron]  # value = <Trihedron.GuideAC: 5>
    GuideACWithContact: typing.ClassVar[Trihedron]  # value = <Trihedron.GuideACWithContact: 7>
    GuidePlan: typing.ClassVar[Trihedron]  # value = <Trihedron.GuidePlan: 6>
    GuidePlanWithContact: typing.ClassVar[Trihedron]  # value = <Trihedron.GuidePlanWithContact: 8>
    __members__: typing.ClassVar[dict[str, Trihedron]]  # value = {'CorrectedFrenet': <Trihedron.CorrectedFrenet: 0>, 'Fixed': <Trihedron.Fixed: 1>, 'Frenet': <Trihedron.Frenet: 2>, 'ConstantNormal': <Trihedron.ConstantNormal: 3>, 'Darboux': <Trihedron.Darboux: 4>, 'GuideAC': <Trihedron.GuideAC: 5>, 'GuidePlan': <Trihedron.GuidePlan: 6>, 'GuideACWithContact': <Trihedron.GuideACWithContact: 7>, 'GuidePlanWithContact': <Trihedron.GuidePlanWithContact: 8>, 'DiscreteTrihedron': <Trihedron.DiscreteTrihedron: 9>}
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
ConstantNormal: Trihedron  # value = <Trihedron.ConstantNormal: 3>
CorrectedFrenet: Trihedron  # value = <Trihedron.CorrectedFrenet: 0>
Darboux: Trihedron  # value = <Trihedron.Darboux: 4>
DiscreteTrihedron: Trihedron  # value = <Trihedron.DiscreteTrihedron: 9>
Fixed: Trihedron  # value = <Trihedron.Fixed: 1>
Frenet: Trihedron  # value = <Trihedron.Frenet: 2>
GuideAC: Trihedron  # value = <Trihedron.GuideAC: 5>
GuideACWithContact: Trihedron  # value = <Trihedron.GuideACWithContact: 7>
GuidePlan: Trihedron  # value = <Trihedron.GuidePlan: 6>
GuidePlanWithContact: Trihedron  # value = <Trihedron.GuidePlanWithContact: 8>
