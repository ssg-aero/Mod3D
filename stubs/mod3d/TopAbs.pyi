"""
Topological shape types
"""
from __future__ import annotations
import typing
__all__: list[str] = ['COMPOUND', 'COMPSOLID', 'EDGE', 'FACE', 'SHAPE', 'SHELL', 'SOLID', 'ShapeEnum', 'VERTEX', 'WIRE']
class ShapeEnum:
    """
    Enumeration of topological shape types
    
    Members:
    
      COMPOUND : Compound shape
    
      COMPSOLID : Composite solid
    
      SOLID : Solid shape
    
      SHELL : Shell shape
    
      FACE : Face shape
    
      WIRE : Wire shape
    
      EDGE : Edge shape
    
      VERTEX : Vertex shape
    
      SHAPE : Generic shape
    """
    COMPOUND: typing.ClassVar[ShapeEnum]  # value = <ShapeEnum.COMPOUND: 0>
    COMPSOLID: typing.ClassVar[ShapeEnum]  # value = <ShapeEnum.COMPSOLID: 1>
    EDGE: typing.ClassVar[ShapeEnum]  # value = <ShapeEnum.EDGE: 6>
    FACE: typing.ClassVar[ShapeEnum]  # value = <ShapeEnum.FACE: 4>
    SHAPE: typing.ClassVar[ShapeEnum]  # value = <ShapeEnum.SHAPE: 8>
    SHELL: typing.ClassVar[ShapeEnum]  # value = <ShapeEnum.SHELL: 3>
    SOLID: typing.ClassVar[ShapeEnum]  # value = <ShapeEnum.SOLID: 2>
    VERTEX: typing.ClassVar[ShapeEnum]  # value = <ShapeEnum.VERTEX: 7>
    WIRE: typing.ClassVar[ShapeEnum]  # value = <ShapeEnum.WIRE: 5>
    __members__: typing.ClassVar[dict[str, ShapeEnum]]  # value = {'COMPOUND': <ShapeEnum.COMPOUND: 0>, 'COMPSOLID': <ShapeEnum.COMPSOLID: 1>, 'SOLID': <ShapeEnum.SOLID: 2>, 'SHELL': <ShapeEnum.SHELL: 3>, 'FACE': <ShapeEnum.FACE: 4>, 'WIRE': <ShapeEnum.WIRE: 5>, 'EDGE': <ShapeEnum.EDGE: 6>, 'VERTEX': <ShapeEnum.VERTEX: 7>, 'SHAPE': <ShapeEnum.SHAPE: 8>}
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
COMPOUND: ShapeEnum  # value = <ShapeEnum.COMPOUND: 0>
COMPSOLID: ShapeEnum  # value = <ShapeEnum.COMPSOLID: 1>
EDGE: ShapeEnum  # value = <ShapeEnum.EDGE: 6>
FACE: ShapeEnum  # value = <ShapeEnum.FACE: 4>
SHAPE: ShapeEnum  # value = <ShapeEnum.SHAPE: 8>
SHELL: ShapeEnum  # value = <ShapeEnum.SHELL: 3>
SOLID: ShapeEnum  # value = <ShapeEnum.SOLID: 2>
VERTEX: ShapeEnum  # value = <ShapeEnum.VERTEX: 7>
WIRE: ShapeEnum  # value = <ShapeEnum.WIRE: 5>
