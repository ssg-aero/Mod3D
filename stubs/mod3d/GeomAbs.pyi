"""
GeomAbs module
"""
from __future__ import annotations
import typing
__all__: list[str] = ['Arc', 'BSplineCurve', 'BSplineSurface', 'BezierCurve', 'BezierSurface', 'C0', 'C1', 'C2', 'C3', 'CN', 'Circle', 'Cone', 'CurveType', 'Cylinder', 'Ellipse', 'G1', 'G2', 'Hyperbola', 'Intersection', 'IsoType', 'IsoU', 'IsoV', 'JoinType', 'Line', 'NoneIso', 'OffsetCurve', 'OffsetSurface', 'OtherCurve', 'OtherSurface', 'Parabola', 'Plane', 'Shape', 'Sphere', 'SurfaceOfExtrusion', 'SurfaceOfRevolution', 'SurfaceType', 'Tangent', 'Torus']
class CurveType:
    """
    Identifies the type of a curve.
    
    Members:
    
      Line : Straight line
    
      Circle : Circle
    
      Ellipse : Ellipse
    
      Hyperbola : Hyperbola
    
      Parabola : Parabola
    
      BezierCurve : Bezier curve
    
      BSplineCurve : BSpline curve
    
      OffsetCurve : Offset curve
    
      OtherCurve : Other curve type
    """
    BSplineCurve: typing.ClassVar[CurveType]  # value = <CurveType.BSplineCurve: 6>
    BezierCurve: typing.ClassVar[CurveType]  # value = <CurveType.BezierCurve: 5>
    Circle: typing.ClassVar[CurveType]  # value = <CurveType.Circle: 1>
    Ellipse: typing.ClassVar[CurveType]  # value = <CurveType.Ellipse: 2>
    Hyperbola: typing.ClassVar[CurveType]  # value = <CurveType.Hyperbola: 3>
    Line: typing.ClassVar[CurveType]  # value = <CurveType.Line: 0>
    OffsetCurve: typing.ClassVar[CurveType]  # value = <CurveType.OffsetCurve: 7>
    OtherCurve: typing.ClassVar[CurveType]  # value = <CurveType.OtherCurve: 8>
    Parabola: typing.ClassVar[CurveType]  # value = <CurveType.Parabola: 4>
    __members__: typing.ClassVar[dict[str, CurveType]]  # value = {'Line': <CurveType.Line: 0>, 'Circle': <CurveType.Circle: 1>, 'Ellipse': <CurveType.Ellipse: 2>, 'Hyperbola': <CurveType.Hyperbola: 3>, 'Parabola': <CurveType.Parabola: 4>, 'BezierCurve': <CurveType.BezierCurve: 5>, 'BSplineCurve': <CurveType.BSplineCurve: 6>, 'OffsetCurve': <CurveType.OffsetCurve: 7>, 'OtherCurve': <CurveType.OtherCurve: 8>}
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
class IsoType:
    """
    Describes if a curve is a U or V isoparametric curve on a surface.
    
    Members:
    
      IsoU : U isoparametric curve (constant U)
    
      IsoV : V isoparametric curve (constant V)
    
      NoneIso : Not an isoparametric curve
    """
    IsoU: typing.ClassVar[IsoType]  # value = <IsoType.IsoU: 0>
    IsoV: typing.ClassVar[IsoType]  # value = <IsoType.IsoV: 1>
    NoneIso: typing.ClassVar[IsoType]  # value = <IsoType.NoneIso: 2>
    __members__: typing.ClassVar[dict[str, IsoType]]  # value = {'IsoU': <IsoType.IsoU: 0>, 'IsoV': <IsoType.IsoV: 1>, 'NoneIso': <IsoType.NoneIso: 2>}
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
class JoinType:
    """
    Members:
    
      Arc : Vertices generate revolved pipes about the axis passing along the vertex
    
      Tangent : Tangent join type
    
      Intersection : Intersection join type
    """
    Arc: typing.ClassVar[JoinType]  # value = <JoinType.Arc: 0>
    Intersection: typing.ClassVar[JoinType]  # value = <JoinType.Intersection: 2>
    Tangent: typing.ClassVar[JoinType]  # value = <JoinType.Tangent: 1>
    __members__: typing.ClassVar[dict[str, JoinType]]  # value = {'Arc': <JoinType.Arc: 0>, 'Tangent': <JoinType.Tangent: 1>, 'Intersection': <JoinType.Intersection: 2>}
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
class Shape:
    """
    Members:
    
      C0
    
      C1
    
      C2
    
      C3
    
      G1
    
      G2
    
      CN
    """
    C0: typing.ClassVar[Shape]  # value = <Shape.C0: 0>
    C1: typing.ClassVar[Shape]  # value = <Shape.C1: 2>
    C2: typing.ClassVar[Shape]  # value = <Shape.C2: 4>
    C3: typing.ClassVar[Shape]  # value = <Shape.C3: 5>
    CN: typing.ClassVar[Shape]  # value = <Shape.CN: 6>
    G1: typing.ClassVar[Shape]  # value = <Shape.G1: 1>
    G2: typing.ClassVar[Shape]  # value = <Shape.G2: 3>
    __members__: typing.ClassVar[dict[str, Shape]]  # value = {'C0': <Shape.C0: 0>, 'C1': <Shape.C1: 2>, 'C2': <Shape.C2: 4>, 'C3': <Shape.C3: 5>, 'G1': <Shape.G1: 1>, 'G2': <Shape.G2: 3>, 'CN': <Shape.CN: 6>}
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
class SurfaceType:
    """
    Identifies the type of a surface.
    
    Members:
    
      Plane : Planar surface
    
      Cylinder : Cylindrical surface
    
      Cone : Conical surface
    
      Sphere : Spherical surface
    
      Torus : Toroidal surface
    
      BezierSurface : Bezier surface
    
      BSplineSurface : BSpline surface
    
      SurfaceOfRevolution : Surface of revolution
    
      SurfaceOfExtrusion : Surface of extrusion
    
      OffsetSurface : Offset surface
    
      OtherSurface : Other surface type
    """
    BSplineSurface: typing.ClassVar[SurfaceType]  # value = <SurfaceType.BSplineSurface: 6>
    BezierSurface: typing.ClassVar[SurfaceType]  # value = <SurfaceType.BezierSurface: 5>
    Cone: typing.ClassVar[SurfaceType]  # value = <SurfaceType.Cone: 2>
    Cylinder: typing.ClassVar[SurfaceType]  # value = <SurfaceType.Cylinder: 1>
    OffsetSurface: typing.ClassVar[SurfaceType]  # value = <SurfaceType.OffsetSurface: 9>
    OtherSurface: typing.ClassVar[SurfaceType]  # value = <SurfaceType.OtherSurface: 10>
    Plane: typing.ClassVar[SurfaceType]  # value = <SurfaceType.Plane: 0>
    Sphere: typing.ClassVar[SurfaceType]  # value = <SurfaceType.Sphere: 3>
    SurfaceOfExtrusion: typing.ClassVar[SurfaceType]  # value = <SurfaceType.SurfaceOfExtrusion: 8>
    SurfaceOfRevolution: typing.ClassVar[SurfaceType]  # value = <SurfaceType.SurfaceOfRevolution: 7>
    Torus: typing.ClassVar[SurfaceType]  # value = <SurfaceType.Torus: 4>
    __members__: typing.ClassVar[dict[str, SurfaceType]]  # value = {'Plane': <SurfaceType.Plane: 0>, 'Cylinder': <SurfaceType.Cylinder: 1>, 'Cone': <SurfaceType.Cone: 2>, 'Sphere': <SurfaceType.Sphere: 3>, 'Torus': <SurfaceType.Torus: 4>, 'BezierSurface': <SurfaceType.BezierSurface: 5>, 'BSplineSurface': <SurfaceType.BSplineSurface: 6>, 'SurfaceOfRevolution': <SurfaceType.SurfaceOfRevolution: 7>, 'SurfaceOfExtrusion': <SurfaceType.SurfaceOfExtrusion: 8>, 'OffsetSurface': <SurfaceType.OffsetSurface: 9>, 'OtherSurface': <SurfaceType.OtherSurface: 10>}
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
Arc: JoinType  # value = <JoinType.Arc: 0>
BSplineCurve: CurveType  # value = <CurveType.BSplineCurve: 6>
BSplineSurface: SurfaceType  # value = <SurfaceType.BSplineSurface: 6>
BezierCurve: CurveType  # value = <CurveType.BezierCurve: 5>
BezierSurface: SurfaceType  # value = <SurfaceType.BezierSurface: 5>
C0: Shape  # value = <Shape.C0: 0>
C1: Shape  # value = <Shape.C1: 2>
C2: Shape  # value = <Shape.C2: 4>
C3: Shape  # value = <Shape.C3: 5>
CN: Shape  # value = <Shape.CN: 6>
Circle: CurveType  # value = <CurveType.Circle: 1>
Cone: SurfaceType  # value = <SurfaceType.Cone: 2>
Cylinder: SurfaceType  # value = <SurfaceType.Cylinder: 1>
Ellipse: CurveType  # value = <CurveType.Ellipse: 2>
G1: Shape  # value = <Shape.G1: 1>
G2: Shape  # value = <Shape.G2: 3>
Hyperbola: CurveType  # value = <CurveType.Hyperbola: 3>
Intersection: JoinType  # value = <JoinType.Intersection: 2>
IsoU: IsoType  # value = <IsoType.IsoU: 0>
IsoV: IsoType  # value = <IsoType.IsoV: 1>
Line: CurveType  # value = <CurveType.Line: 0>
NoneIso: IsoType  # value = <IsoType.NoneIso: 2>
OffsetCurve: CurveType  # value = <CurveType.OffsetCurve: 7>
OffsetSurface: SurfaceType  # value = <SurfaceType.OffsetSurface: 9>
OtherCurve: CurveType  # value = <CurveType.OtherCurve: 8>
OtherSurface: SurfaceType  # value = <SurfaceType.OtherSurface: 10>
Parabola: CurveType  # value = <CurveType.Parabola: 4>
Plane: SurfaceType  # value = <SurfaceType.Plane: 0>
Sphere: SurfaceType  # value = <SurfaceType.Sphere: 3>
SurfaceOfExtrusion: SurfaceType  # value = <SurfaceType.SurfaceOfExtrusion: 8>
SurfaceOfRevolution: SurfaceType  # value = <SurfaceType.SurfaceOfRevolution: 7>
Tangent: JoinType  # value = <JoinType.Tangent: 1>
Torus: SurfaceType  # value = <SurfaceType.Torus: 4>
