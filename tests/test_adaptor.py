"""Tests for Adaptor module - curve and surface adaptors."""
import pytest
import math
import mod3d
from mod3d import gp, Geom, GeomAbs, TopoDS, BRepBuilderAPI, BRepPrim, TopExp, Adaptor


# =============================================================================
# GeomAbs enum tests
# =============================================================================

def test_curve_type_enum():
    """Test GeomAbs.CurveType enum values."""
    assert hasattr(GeomAbs, 'CurveType')
    assert hasattr(GeomAbs.CurveType, 'Line')
    assert hasattr(GeomAbs.CurveType, 'Circle')
    assert hasattr(GeomAbs.CurveType, 'Ellipse')
    assert hasattr(GeomAbs.CurveType, 'Hyperbola')
    assert hasattr(GeomAbs.CurveType, 'Parabola')
    assert hasattr(GeomAbs.CurveType, 'BezierCurve')
    assert hasattr(GeomAbs.CurveType, 'BSplineCurve')
    assert hasattr(GeomAbs.CurveType, 'OffsetCurve')
    assert hasattr(GeomAbs.CurveType, 'OtherCurve')


def test_iso_type_enum():
    """Test GeomAbs.IsoType enum values."""
    assert hasattr(GeomAbs, 'IsoType')
    assert hasattr(GeomAbs.IsoType, 'IsoU')
    assert hasattr(GeomAbs.IsoType, 'IsoV')
    assert hasattr(GeomAbs.IsoType, 'NoneIso')


# =============================================================================
# GeomAdaptor_Curve (Adaptor.GeomCurve) tests
# =============================================================================

def test_geom_curve_default_constructor():
    """Test GeomCurve default constructor."""
    adaptor = Adaptor.GeomCurve()
    # Should be created but undefined


def test_geom_curve_from_line():
    """Test GeomCurve adaptor for a line curve."""
    # Create a line from two points
    p1 = gp.Pnt(0, 0, 0)
    p2 = gp.Pnt(10, 0, 0)
    line = Geom.TrimmedCurve(Geom.Line(p1, gp.Dir(1, 0, 0)), 0, 10)
    
    adaptor = Adaptor.GeomCurve(line)
    
    # Check parameter bounds
    assert adaptor.first_parameter == pytest.approx(0.0)
    assert adaptor.last_parameter == pytest.approx(10.0)
    
    # Check curve type
    assert adaptor.get_type() == GeomAbs.CurveType.Line
    
    # Evaluate at midpoint
    pt = adaptor.value(5.0)
    assert pt.x == pytest.approx(5.0)
    assert pt.y == pytest.approx(0.0)
    assert pt.z == pytest.approx(0.0)


def test_geom_curve_from_circle():
    """Test GeomCurve adaptor for a circle curve."""
    # Create a circle centered at origin in XY plane
    ax2 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle = Geom.Circle(ax2, 5.0)
    
    adaptor = Adaptor.GeomCurve(circle)
    
    # Check parameter bounds (full circle is 0 to 2*pi)
    assert adaptor.first_parameter == pytest.approx(0.0)
    assert adaptor.last_parameter == pytest.approx(2.0 * math.pi)
    
    # Check curve type
    assert adaptor.get_type() == GeomAbs.CurveType.Circle
    
    # Check periodicity
    assert adaptor.is_closed
    assert adaptor.is_periodic
    assert adaptor.period == pytest.approx(2.0 * math.pi)
    
    # Evaluate at start point (should be at (5, 0, 0))
    pt = adaptor.value(0.0)
    assert pt.x == pytest.approx(5.0)
    assert pt.y == pytest.approx(0.0)
    assert pt.z == pytest.approx(0.0)
    
    # Get the underlying circle
    circ = adaptor.circle()
    assert circ.radius == pytest.approx(5.0)


def test_geom_curve_with_bounds():
    """Test GeomCurve adaptor with specified parameter bounds."""
    # Create a full circle
    ax2 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle = Geom.Circle(ax2, 5.0)
    
    # Adapt only quarter of the circle
    adaptor = Adaptor.GeomCurve(circle, 0.0, math.pi / 2)
    
    assert adaptor.first_parameter == pytest.approx(0.0)
    assert adaptor.last_parameter == pytest.approx(math.pi / 2)


def test_geom_curve_load():
    """Test GeomCurve load method."""
    adaptor = Adaptor.GeomCurve()
    
    # Load a line
    line = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
    adaptor.load(line)
    
    assert adaptor.get_type() == GeomAbs.CurveType.Line
    
    # Load with bounds
    adaptor.load(line, 0.0, 5.0)
    assert adaptor.last_parameter == pytest.approx(5.0)


def test_geom_curve_reset():
    """Test GeomCurve reset method."""
    line = Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0))
    adaptor = Adaptor.GeomCurve(line)
    
    # Reset should work
    adaptor.reset()


def test_geom_curve_derivatives():
    """Test GeomCurve derivative methods."""
    # Create a line along X axis
    line = Geom.TrimmedCurve(Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)), 0, 10)
    adaptor = Adaptor.GeomCurve(line)
    
    # D0 - just point
    pt = adaptor.d0(5.0)
    assert pt.x == pytest.approx(5.0)
    
    # D1 - point and first derivative
    pt, v1 = adaptor.d1(5.0)
    assert pt.x == pytest.approx(5.0)
    assert v1.x == pytest.approx(1.0)  # Tangent along X
    assert v1.y == pytest.approx(0.0)
    
    # D2 - point, first and second derivatives
    pt, v1, v2 = adaptor.d2(5.0)
    assert v2.x == pytest.approx(0.0)  # Line has zero second derivative
    
    # DN - nth derivative
    dn = adaptor.dn(5.0, 1)
    assert dn.x == pytest.approx(1.0)


def test_geom_curve_intervals():
    """Test GeomCurve interval methods."""
    line = Geom.TrimmedCurve(Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)), 0, 10)
    adaptor = Adaptor.GeomCurve(line)
    
    # Line has continuous derivatives everywhere
    assert adaptor.continuity == GeomAbs.Shape.CN
    
    # Should have 1 interval for any continuity
    n = adaptor.nb_intervals(GeomAbs.Shape.C0)
    assert n >= 1
    
    # Get the intervals
    intervals = adaptor.intervals(GeomAbs.Shape.C0)
    assert len(intervals) >= 2
    assert intervals[0] == pytest.approx(0.0)
    assert intervals[-1] == pytest.approx(10.0)


def test_geom_curve_resolution():
    """Test GeomCurve resolution method."""
    line = Geom.TrimmedCurve(Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)), 0, 10)
    adaptor = Adaptor.GeomCurve(line)
    
    # Get parametric resolution for 1.0 distance
    res = adaptor.resolution(1.0)
    assert res > 0


def test_geom_curve_trim():
    """Test GeomCurve trim method."""
    line = Geom.TrimmedCurve(Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)), 0, 10)
    adaptor = Adaptor.GeomCurve(line)
    
    # Trim to [2, 8]
    trimmed = adaptor.trim(2.0, 8.0, 1e-7)
    
    assert trimmed.first_parameter == pytest.approx(2.0)
    assert trimmed.last_parameter == pytest.approx(8.0)


def test_geom_curve_bspline():
    """Test GeomCurve adaptor for a BSpline curve."""
    # Create a simple BSpline through some points
    poles = [
        gp.Pnt(0, 0, 0),
        gp.Pnt(1, 2, 0),
        gp.Pnt(3, 1, 0),
        gp.Pnt(4, 0, 0),
    ]
    
    # Knots and multiplicities for degree 3 curve
    knots = [0.0, 1.0]
    
    mults = [4, 4]
    
    bspline = Geom.BSplineCurve(poles, knots, mults, 3)
    adaptor = Adaptor.GeomCurve(bspline)
    
    assert adaptor.get_type() == GeomAbs.CurveType.BSplineCurve
    assert adaptor.degree == 3
    assert adaptor.nb_poles == 4
    
    # Get the underlying BSpline
    bs = adaptor.bspline()
    assert bs is not None


def test_geom_curve_shallow_copy():
    """Test GeomCurve shallow copy."""
    line = Geom.TrimmedCurve(Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)), 0, 10)
    adaptor = Adaptor.GeomCurve(line)
    
    copy = adaptor.shallow_copy()
    assert copy is not None
    assert copy.first_parameter == adaptor.first_parameter


# =============================================================================
# BRepAdaptor_Curve (Adaptor.BRepCurve) tests
# =============================================================================

def test_brep_curve_default_constructor():
    """Test BRepCurve default constructor."""
    adaptor = Adaptor.BRepCurve()
    # Should be created but undefined


def test_brep_curve_from_edge():
    """Test BRepCurve adaptor for an edge."""
    # Create a simple edge (line segment)
    p1 = gp.Pnt(0, 0, 0)
    p2 = gp.Pnt(10, 0, 0)
    edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    
    adaptor = Adaptor.BRepCurve(edge)
    
    # Check that it's a 3D curve
    assert adaptor.is_3d_curve
    assert not adaptor.is_curve_on_surface
    
    # Check parameter bounds (edge should preserve curve parameters)
    assert adaptor.first_parameter == pytest.approx(0.0)
    assert adaptor.last_parameter == pytest.approx(10.0)
    
    # Evaluate at midpoint
    pt = adaptor.value(5.0)
    assert pt.x == pytest.approx(5.0)
    assert pt.y == pytest.approx(0.0)
    
    # Get edge and tolerance
    assert adaptor.edge is not None
    assert adaptor.tolerance > 0


def test_brep_curve_circular_edge():
    """Test BRepCurve adaptor for a circular edge."""
    # Create a circular edge
    ax2 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    circle = gp.Circ(ax2, 5.0)
    edge = BRepBuilderAPI.MakeEdge(circle).edge()
    
    adaptor = Adaptor.BRepCurve(edge)
    
    # Check curve type
    assert adaptor.get_type() == GeomAbs.CurveType.Circle
    
    # Get the circle
    circ = adaptor.circle()
    assert circ.radius == pytest.approx(5.0)


def test_brep_curve_initialize():
    """Test BRepCurve initialize method."""
    adaptor = Adaptor.BRepCurve()
    
    # Create an edge
    edge = BRepBuilderAPI.MakeEdge(gp.Pnt(0, 0, 0), gp.Pnt(5, 5, 5)).edge()
    
    adaptor.initialize(edge)
    assert adaptor.edge is not None


def test_brep_curve_reset():
    """Test BRepCurve reset method."""
    edge = BRepBuilderAPI.MakeEdge(gp.Pnt(0, 0, 0), gp.Pnt(5, 5, 5)).edge()
    adaptor = Adaptor.BRepCurve(edge)
    
    adaptor.reset()


def test_brep_curve_trsf():
    """Test BRepCurve transformation property."""
    edge = BRepBuilderAPI.MakeEdge(gp.Pnt(0, 0, 0), gp.Pnt(5, 5, 5)).edge()
    adaptor = Adaptor.BRepCurve(edge)
    
    trsf = adaptor.trsf
    assert trsf is not None


def test_brep_curve_derivatives():
    """Test BRepCurve derivative methods."""
    edge = BRepBuilderAPI.MakeEdge(gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0)).edge()
    adaptor = Adaptor.BRepCurve(edge)
    
    # D1 - point and tangent
    u = (adaptor.first_parameter + adaptor.last_parameter) / 2
    pt, v1 = adaptor.d1(u)
    
    # Tangent should be along X
    v1_normalized = gp.Dir(v1)
    assert abs(v1_normalized.x) == pytest.approx(1.0, abs=0.01)


# =============================================================================
# BRepAdaptor_CompCurve (Adaptor.CompCurve) tests
# =============================================================================

def test_comp_curve_default_constructor():
    """Test CompCurve default constructor."""
    adaptor = Adaptor.CompCurve()


def test_comp_curve_from_wire():
    """Test CompCurve adaptor for a wire."""
    # Create a simple rectangular wire
    p1, p2, p3, p4 = gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0), gp.Pnt(10, 5, 0), gp.Pnt(0, 5, 0)
    
    wire_maker = BRepBuilderAPI.MakeWire()
    wire_maker.add(BRepBuilderAPI.MakeEdge(p1, p2).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p2, p3).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p3, p4).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p4, p1).edge())
    wire = wire_maker.wire()
    
    adaptor = Adaptor.CompCurve(wire)
    
    # Check the wire
    assert adaptor.wire is not None
    
    # The wire is closed
    assert adaptor.is_closed
    
    # Evaluate at some parameter
    u = (adaptor.first_parameter + adaptor.last_parameter) / 2
    pt = adaptor.value(u)
    assert pt is not None


def test_comp_curve_with_curvilinear_abscissa():
    """Test CompCurve with curvilinear abscissa knots."""
    p1, p2, p3 = gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0), gp.Pnt(10, 5, 0)
    
    wire_maker = BRepBuilderAPI.MakeWire()
    wire_maker.add(BRepBuilderAPI.MakeEdge(p1, p2).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p2, p3).edge())
    wire = wire_maker.wire()
    
    adaptor = Adaptor.CompCurve(wire, True)  # Use curvilinear abscissa
    
    assert adaptor.wire is not None


def test_comp_curve_edge_at_parameter():
    """Test CompCurve edge method to get edge at parameter."""
    p1, p2, p3 = gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0), gp.Pnt(10, 5, 0)
    
    wire_maker = BRepBuilderAPI.MakeWire()
    wire_maker.add(BRepBuilderAPI.MakeEdge(p1, p2).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p2, p3).edge())
    wire = wire_maker.wire()
    
    adaptor = Adaptor.CompCurve(wire, True)
    
    # Get edge at midpoint of first edge
    u = adaptor.first_parameter + 1.0
    edge, u_on_edge = adaptor.edge(u)
    
    assert edge is not None


def test_comp_curve_trimmed():
    """Test CompCurve with trimmed wire."""
    p1, p2, p3 = gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0), gp.Pnt(20, 0, 0)
    
    wire_maker = BRepBuilderAPI.MakeWire()
    wire_maker.add(BRepBuilderAPI.MakeEdge(p1, p2).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p2, p3).edge())
    wire = wire_maker.wire()
    
    # Create trimmed adaptor
    adaptor = Adaptor.CompCurve(wire, True, 2.0, 18.0, 1e-6)
    
    assert adaptor.first_parameter == pytest.approx(2.0)
    assert adaptor.last_parameter == pytest.approx(18.0)


def test_comp_curve_initialize():
    """Test CompCurve initialize method."""
    p1, p2, p3 = gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0), gp.Pnt(10, 5, 0)
    
    wire_maker = BRepBuilderAPI.MakeWire()
    wire_maker.add(BRepBuilderAPI.MakeEdge(p1, p2).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p2, p3).edge())
    wire = wire_maker.wire()
    
    adaptor = Adaptor.CompCurve()
    adaptor.initialize(wire, False)
    
    assert adaptor.wire is not None


# =============================================================================
# Adaptor3d_IsoCurve (Adaptor.IsoCurve) tests
# =============================================================================

def test_iso_curve_default_constructor():
    """Test IsoCurve default constructor."""
    adaptor = Adaptor.IsoCurve()
    assert adaptor.iso == GeomAbs.IsoType.NoneIso


# Note: IsoCurve requires an Adaptor3d_Surface, which we'd need to add
# For now, basic constructor test is sufficient


# =============================================================================
# Adaptor3d_CurveOnSurface (Adaptor.CurveOnSurface) tests
# =============================================================================

def test_curve_on_surface_default_constructor():
    """Test CurveOnSurface default constructor."""
    adaptor = Adaptor.CurveOnSurface()


# =============================================================================
# Adaptor3d_Curve base class tests
# =============================================================================

def test_adaptor_curve_properties():
    """Test that Adaptor.Curve base class properties work through derived classes."""
    # Use GeomCurve as a concrete implementation
    line = Geom.TrimmedCurve(Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)), 0, 10)
    adaptor = Adaptor.GeomCurve(line)
    
    # All these should work as properties
    assert adaptor.first_parameter is not None
    assert adaptor.last_parameter is not None
    assert adaptor.continuity is not None
    assert adaptor.is_closed is not None
    assert adaptor.is_periodic is not None


def test_adaptor_polymorphism():
    """Test that adaptors can be used polymorphically."""
    # Create different adaptor types
    adaptors = []
    
    # GeomCurve
    line = Geom.TrimmedCurve(Geom.Line(gp.Pnt(0, 0, 0), gp.Dir(1, 0, 0)), 0, 10)
    adaptors.append(Adaptor.GeomCurve(line))
    
    # BRepCurve
    edge = BRepBuilderAPI.MakeEdge(gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0)).edge()
    adaptors.append(Adaptor.BRepCurve(edge))
    
    # All should support the base class interface
    for adaptor in adaptors:
        assert adaptor.first_parameter is not None
        assert adaptor.last_parameter is not None
        pt = adaptor.value((adaptor.first_parameter + adaptor.last_parameter) / 2)
        assert pt is not None


# =============================================================================
# Edge case and error handling tests
# =============================================================================

def test_geom_curve_ellipse():
    """Test GeomCurve with ellipse."""
    ax2 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    ellipse = Geom.Ellipse(ax2, 10.0, 5.0)
    
    adaptor = Adaptor.GeomCurve(ellipse)
    
    assert adaptor.get_type() == GeomAbs.CurveType.Ellipse
    
    ell = adaptor.ellipse()
    assert ell.major_radius == pytest.approx(10.0)
    assert ell.minor_radius == pytest.approx(5.0)


def test_brep_curve_from_bspline_edge():
    """Test BRepCurve from a BSpline edge."""
    # Create a BSpline edge

    poles = [
        gp.Pnt(0, 0, 0),
        gp.Pnt(1, 2, 0),
        gp.Pnt(3, 1, 0),
        gp.Pnt(4, 0, 0),
    ]
    
    knots = [0.0, 1.0]
    
    mults = [4, 4]
    
    bspline = Geom.BSplineCurve(poles, knots, mults, 3)
    edge = BRepBuilderAPI.MakeEdge(bspline).edge()
    
    adaptor = Adaptor.BRepCurve(edge)
    
    assert adaptor.get_type() == GeomAbs.CurveType.BSplineCurve


# =============================================================================
# GeomAbs.SurfaceType enum tests
# =============================================================================

def test_surface_type_enum():
    """Test GeomAbs.SurfaceType enum values."""
    assert hasattr(GeomAbs, 'SurfaceType')
    assert hasattr(GeomAbs.SurfaceType, 'Plane')
    assert hasattr(GeomAbs.SurfaceType, 'Cylinder')
    assert hasattr(GeomAbs.SurfaceType, 'Cone')
    assert hasattr(GeomAbs.SurfaceType, 'Sphere')
    assert hasattr(GeomAbs.SurfaceType, 'Torus')
    assert hasattr(GeomAbs.SurfaceType, 'BezierSurface')
    assert hasattr(GeomAbs.SurfaceType, 'BSplineSurface')
    assert hasattr(GeomAbs.SurfaceType, 'SurfaceOfRevolution')
    assert hasattr(GeomAbs.SurfaceType, 'SurfaceOfExtrusion')
    assert hasattr(GeomAbs.SurfaceType, 'OffsetSurface')
    assert hasattr(GeomAbs.SurfaceType, 'OtherSurface')


# =============================================================================
# GeomAdaptor_Surface (Adaptor.GeomSurface) tests
# =============================================================================

def test_geom_surface_default_constructor():
    """Test GeomSurface default constructor."""
    adaptor = Adaptor.GeomSurface()


def test_geom_surface_from_plane():
    """Test GeomSurface adaptor for a plane."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    adaptor = Adaptor.GeomSurface(plane)
    
    # Check surface type
    assert adaptor.get_type() == GeomAbs.SurfaceType.Plane
    
    # Evaluate at some point
    pt = adaptor.value(1.0, 2.0)
    assert pt.x == pytest.approx(1.0)
    assert pt.y == pytest.approx(2.0)
    assert pt.z == pytest.approx(0.0)
    
    # Get the plane
    pln = adaptor.plane()
    assert pln is not None


def test_geom_surface_from_cylinder():
    """Test GeomSurface adaptor for a cylinder."""
    ax3 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    cylinder = Geom.CylindricalSurface(ax3, 5.0)
    
    adaptor = Adaptor.GeomSurface(cylinder)
    
    assert adaptor.get_type() == GeomAbs.SurfaceType.Cylinder
    assert adaptor.is_u_periodic  # Periodic in U (angle)
    
    cyl = adaptor.cylinder()
    assert cyl.radius == pytest.approx(5.0)


def test_geom_surface_from_sphere():
    """Test GeomSurface adaptor for a sphere."""
    ax3 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    sphere = Geom.SphericalSurface(ax3, 10.0)
    
    adaptor = Adaptor.GeomSurface(sphere)
    
    assert adaptor.get_type() == GeomAbs.SurfaceType.Sphere
    
    sph = adaptor.sphere()
    assert sph.radius == pytest.approx(10.0)


def test_geom_surface_derivatives():
    """Test GeomSurface derivative methods."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    adaptor = Adaptor.GeomSurface(plane)
    
    # D0 - just point
    pt = adaptor.d0(1.0, 2.0)
    assert pt.x == pytest.approx(1.0)
    assert pt.y == pytest.approx(2.0)
    
    # D1 - point and first derivatives
    pt, du, dv = adaptor.d1(1.0, 2.0)
    assert du.x == pytest.approx(1.0)  # dP/dU is along X for XY plane
    assert du.y == pytest.approx(0.0)
    assert dv.x == pytest.approx(0.0)
    assert dv.y == pytest.approx(1.0)  # dP/dV is along Y
    
    # D2 - point and first and second derivatives
    pt, du, dv, d2u, d2v, d2uv = adaptor.d2(1.0, 2.0)
    # Plane has zero second derivatives
    assert d2u.magnitude == pytest.approx(0.0, abs=1e-10)


def test_geom_surface_load():
    """Test GeomSurface load method."""
    adaptor = Adaptor.GeomSurface()
    
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    adaptor.load(plane)
    
    assert adaptor.get_type() == GeomAbs.SurfaceType.Plane


def test_geom_surface_with_bounds():
    """Test GeomSurface with specified parameter bounds."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    adaptor = Adaptor.GeomSurface(plane, 0.0, 10.0, 0.0, 20.0)
    
    assert adaptor.first_u_parameter == pytest.approx(0.0)
    assert adaptor.last_u_parameter == pytest.approx(10.0)
    assert adaptor.first_v_parameter == pytest.approx(0.0)
    assert adaptor.last_v_parameter == pytest.approx(20.0)


def test_geom_surface_resolution():
    """Test GeomSurface resolution methods."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    adaptor = Adaptor.GeomSurface(plane)
    
    u_res = adaptor.u_resolution(1.0)
    v_res = adaptor.v_resolution(1.0)
    assert u_res > 0
    assert v_res > 0


def test_geom_surface_shallow_copy():
    """Test GeomSurface shallow copy."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    adaptor = Adaptor.GeomSurface(plane)
    
    copy = adaptor.shallow_copy()
    assert copy is not None


# =============================================================================
# BRepAdaptor_Surface (Adaptor.BRepSurface) tests
# =============================================================================

def test_brep_surface_default_constructor():
    """Test BRepSurface default constructor."""
    adaptor = Adaptor.BRepSurface()


def test_brep_surface_from_planar_face():
    """Test BRepSurface adaptor for a planar face."""
    # Create a planar face from a rectangular wire
    p1, p2, p3, p4 = gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0), gp.Pnt(10, 5, 0), gp.Pnt(0, 5, 0)
    
    wire_maker = BRepBuilderAPI.MakeWire()
    wire_maker.add(BRepBuilderAPI.MakeEdge(p1, p2).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p2, p3).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p3, p4).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p4, p1).edge())
    wire = wire_maker.wire()
    
    face = BRepBuilderAPI.MakeFace(wire, True).face()
    
    adaptor = Adaptor.BRepSurface(face)
    
    # Check properties
    assert adaptor.face is not None
    assert adaptor.tolerance > 0
    
    # Should be a plane
    assert adaptor.get_type() == GeomAbs.SurfaceType.Plane


def test_brep_surface_from_cylindrical_face():
    """Test BRepSurface adaptor for a cylindrical face."""
    # Create a cylinder
    ax2 = gp.Ax2(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    cylinder = BRepBuilderAPI.MakeCylinder(ax2, 5.0, 10.0).shape()
    
    # Get a face (the cylindrical surface)
    faces = TopExp.get_faces(cylinder)
    
    # Find the cylindrical face
    for face in faces:
        adaptor = Adaptor.BRepSurface(face)
        if adaptor.get_type() == GeomAbs.SurfaceType.Cylinder:
            cyl = adaptor.cylinder()
            assert cyl.radius == pytest.approx(5.0)
            break
    else:
        pytest.fail("No cylindrical face found")


def test_brep_surface_initialize():
    """Test BRepSurface initialize method."""
    p1, p2, p3, p4 = gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0), gp.Pnt(10, 5, 0), gp.Pnt(0, 5, 0)
    
    wire_maker = BRepBuilderAPI.MakeWire()
    wire_maker.add(BRepBuilderAPI.MakeEdge(p1, p2).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p2, p3).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p3, p4).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p4, p1).edge())
    wire = wire_maker.wire()
    
    face = BRepBuilderAPI.MakeFace(wire, True).face()
    
    adaptor = Adaptor.BRepSurface()
    adaptor.initialize(face)
    
    assert adaptor.face is not None


def test_brep_surface_trsf():
    """Test BRepSurface transformation property."""
    p1, p2, p3, p4 = gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0), gp.Pnt(10, 5, 0), gp.Pnt(0, 5, 0)
    
    wire_maker = BRepBuilderAPI.MakeWire()
    wire_maker.add(BRepBuilderAPI.MakeEdge(p1, p2).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p2, p3).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p3, p4).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p4, p1).edge())
    wire = wire_maker.wire()
    
    face = BRepBuilderAPI.MakeFace(wire, True).face()
    
    adaptor = Adaptor.BRepSurface(face)
    trsf = adaptor.trsf
    assert trsf is not None


def test_brep_surface_with_restriction():
    """Test BRepSurface with/without restriction flag."""
    p1, p2, p3, p4 = gp.Pnt(0, 0, 0), gp.Pnt(10, 0, 0), gp.Pnt(10, 5, 0), gp.Pnt(0, 5, 0)
    
    wire_maker = BRepBuilderAPI.MakeWire()
    wire_maker.add(BRepBuilderAPI.MakeEdge(p1, p2).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p2, p3).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p3, p4).edge())
    wire_maker.add(BRepBuilderAPI.MakeEdge(p4, p1).edge())
    wire = wire_maker.wire()
    
    face = BRepBuilderAPI.MakeFace(wire, True).face()
    
    # With restriction (default)
    adaptor1 = Adaptor.BRepSurface(face, True)
    
    # Without restriction
    adaptor2 = Adaptor.BRepSurface(face, False)
    
    # Both should work
    pt1 = adaptor1.value(5.0, 2.5)
    assert pt1 is not None


# =============================================================================
# IsoCurve with surface adaptor tests
# =============================================================================

def test_iso_curve_on_geom_surface():
    """Test IsoCurve on a GeomSurface adaptor."""
    # Create a plane surface adaptor
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    surface_adaptor = Adaptor.GeomSurface(plane, -10, 10, -10, 10)
    
    # Create a U isocurve at U=5
    iso_u = Adaptor.IsoCurve(surface_adaptor, GeomAbs.IsoType.IsoU, 5.0)
    
    assert iso_u.iso == GeomAbs.IsoType.IsoU
    assert iso_u.parameter == pytest.approx(5.0)
    
    # The iso curve should be a line (V varies)
    pt = iso_u.value(-5.0)
    assert pt.x == pytest.approx(5.0)  # U is fixed at 5
    assert pt.y == pytest.approx(-5.0)  # V varies
    assert pt.z == pytest.approx(0.0)


def test_iso_curve_on_cylinder():
    """Test IsoCurve on a cylindrical surface."""
    ax3 = gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1))
    cylinder = Geom.CylindricalSurface(ax3, 5.0)
    surface_adaptor = Adaptor.GeomSurface(cylinder)
    
    # Create a V isocurve (constant height, forms a circle)
    iso_v = Adaptor.IsoCurve(surface_adaptor, GeomAbs.IsoType.IsoV, 2.0)
    
    assert iso_v.iso == GeomAbs.IsoType.IsoV
    assert iso_v.parameter == pytest.approx(2.0)
    
    # The iso curve at constant V on a cylinder is a circle
    pt = iso_v.value(0.0)
    # At U=0, V=2 on cylinder with radius 5
    assert pt.x == pytest.approx(5.0)
    assert pt.y == pytest.approx(0.0)
    assert pt.z == pytest.approx(2.0)


def test_iso_curve_load():
    """Test IsoCurve load methods."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    surface_adaptor = Adaptor.GeomSurface(plane, -10, 10, -10, 10)
    
    iso = Adaptor.IsoCurve()
    iso.load(surface_adaptor)
    
    assert iso.iso == GeomAbs.IsoType.NoneIso
    
    # Load the iso parameters
    iso.load(GeomAbs.IsoType.IsoU, 3.0)
    assert iso.iso == GeomAbs.IsoType.IsoU
    assert iso.parameter == pytest.approx(3.0)


def test_iso_curve_with_bounds():
    """Test IsoCurve with specified parameter bounds."""
    plane = Geom.Plane(gp.Ax3(gp.Pnt(0, 0, 0), gp.Dir(0, 0, 1)))
    surface_adaptor = Adaptor.GeomSurface(plane, -10, 10, -10, 10)
    
    # Create iso curve with bounds
    iso = Adaptor.IsoCurve(surface_adaptor, GeomAbs.IsoType.IsoU, 5.0, -5.0, 5.0)
    
    assert iso.first_parameter == pytest.approx(-5.0)
    assert iso.last_parameter == pytest.approx(5.0)


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
