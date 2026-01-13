#include <Adaptor3d_Curve.hxx>
#include <Adaptor3d_CurveOnSurface.hxx>
#include <Adaptor3d_IsoCurve.hxx>
#include <Adaptor3d_Surface.hxx>
#include <Adaptor2d_Curve2d.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_CompCurve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <GeomAdaptor_Surface.hxx>

#include <Geom_Curve.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_OffsetCurve.hxx>
#include <Geom_BezierSurface.hxx>
#include <Geom_BSplineSurface.hxx>

#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>

#include <GeomAbs_Shape.hxx>
#include <GeomAbs_CurveType.hxx>
#include <GeomAbs_IsoType.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <TColStd_Array1OfReal.hxx>

#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Lin.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Hypr.hxx>
#include <gp_Parab.hxx>
#include <gp_Trsf.hxx>
#include <gp_Pln.hxx>
#include <gp_Cylinder.hxx>
#include <gp_Cone.hxx>
#include <gp_Sphere.hxx>
#include <gp_Torus.hxx>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>, true);

void bind_adaptor(py::module& m) {
    
    // =========================================================================
    // Adaptor3d_Curve - Root class for 3D curves on which geometric algorithms work
    // =========================================================================
    py::class_<Adaptor3d_Curve, Handle(Adaptor3d_Curve)>(m, "Curve",
        "Root class for 3D curves on which geometric algorithms work.\n\n"
        "An adapted curve is an interface between the services provided by a curve\n"
        "and those required of the curve by algorithms which use it.\n\n"
        "Derived concrete classes:\n"
        "- GeomAdaptor_Curve for a curve from the Geom package\n"
        "- Adaptor3d_CurveOnSurface for a curve lying on a surface\n"
        "- BRepAdaptor_Curve for an edge geometry as a 3D curve\n\n"
        "Note: Polynomial coefficients of BSpline curves used for their evaluation\n"
        "are cached for better performance. Therefore these evaluations are not\n"
        "thread-safe and parallel evaluations need to be prevented.")

        .def(py::init<>([](const opencascade::handle<Geom_Curve>& geom_curve) {
            return new GeomAdaptor_Curve(geom_curve);
        }), py::arg("geom_curve"),
            "Constructor from a Geom_Curve.")
        .def(py::init<>([](const TopoDS_Edge& edge) {
            return new BRepAdaptor_Curve(edge);
        }), py::arg("edge"),
            "Constructor from a TopoDS_Edge.")
        .def(py::init<>([](const TopoDS_Wire& wire) {
            return new BRepAdaptor_CompCurve(wire);
        }), py::arg("wire"),
            "Constructor from a TopoDS_Wire.")
        
        // Parameter domain
        .def_property_readonly("first_parameter", &Adaptor3d_Curve::FirstParameter,
            "Returns the first parameter of the curve.")
        .def_property_readonly("last_parameter", &Adaptor3d_Curve::LastParameter,
            "Returns the last parameter of the curve.")
        
        // Continuity
        .def_property_readonly("continuity", &Adaptor3d_Curve::Continuity,
            "Returns the global continuity of the curve.")
        
        .def("nb_intervals", &Adaptor3d_Curve::NbIntervals, py::arg("continuity"),
            "Returns the number of intervals for continuity S.\n\n"
            "Parameters:\n"
            "  continuity: The desired continuity (GeomAbs.Shape)\n\n"
            "Returns:\n"
            "  Number of intervals. May be one if Continuity(me) >= S")
        
        .def("intervals", [](const Adaptor3d_Curve& self, GeomAbs_Shape S) {
            Standard_Integer n = self.NbIntervals(S);
            TColStd_Array1OfReal T(1, n + 1);
            self.Intervals(T, S);
            std::vector<Standard_Real> result;
            for (Standard_Integer i = 1; i <= n + 1; ++i) {
                result.push_back(T(i));
            }
            return result;
        }, py::arg("continuity"),
            "Returns the parameters bounding the intervals of continuity S.\n\n"
            "Parameters:\n"
            "  continuity: The desired continuity (GeomAbs.Shape)\n\n"
            "Returns:\n"
            "  List of parameter values bounding the intervals")
        
        .def("trim", &Adaptor3d_Curve::Trim, 
            py::arg("first"), py::arg("last"), py::arg("tol"),
            "Returns a curve equivalent of this curve between parameters First and Last.\n\n"
            "Parameters:\n"
            "  first: First parameter\n"
            "  last: Last parameter\n"
            "  tol: Tolerance for 3D points confusion")
        
        // Periodicity
        .def_property_readonly("is_closed", &Adaptor3d_Curve::IsClosed,
            "Returns True if the curve is closed.")
        .def_property_readonly("is_periodic", &Adaptor3d_Curve::IsPeriodic,
            "Returns True if the curve is periodic.")
        .def_property_readonly("period", &Adaptor3d_Curve::Period,
            "Returns the period of the curve (raises if not periodic).")
        
        // Point evaluation
        .def("value", &Adaptor3d_Curve::Value, py::arg("u"),
            "Computes the point of parameter U on the curve.\n\n"
            "Parameters:\n"
            "  u: Parameter value\n\n"
            "Returns:\n"
            "  gp_Pnt at parameter u")
        
        .def("d0", [](const Adaptor3d_Curve& self, Standard_Real U) {
            gp_Pnt P;
            self.D0(U, P);
            return P;
        }, py::arg("u"),
            "Computes the point of parameter U on the curve.\n\n"
            "Parameters:\n"
            "  u: Parameter value\n\n"
            "Returns:\n"
            "  gp_Pnt at parameter u")
        
        .def("d1", [](const Adaptor3d_Curve& self, Standard_Real U) {
            gp_Pnt P;
            gp_Vec V;
            self.D1(U, P, V);
            return std::make_tuple(P, V);
        }, py::arg("u"),
            "Computes the point and first derivative at parameter U.\n\n"
            "Parameters:\n"
            "  u: Parameter value\n\n"
            "Returns:\n"
            "  Tuple of (point, first_derivative)\n\n"
            "Raises:\n"
            "  If continuity of current interval is not C1")
        
        .def("d2", [](const Adaptor3d_Curve& self, Standard_Real U) {
            gp_Pnt P;
            gp_Vec V1, V2;
            self.D2(U, P, V1, V2);
            return std::make_tuple(P, V1, V2);
        }, py::arg("u"),
            "Computes the point, first and second derivatives at parameter U.\n\n"
            "Parameters:\n"
            "  u: Parameter value\n\n"
            "Returns:\n"
            "  Tuple of (point, first_derivative, second_derivative)\n\n"
            "Raises:\n"
            "  If continuity of current interval is not C2")
        
        .def("d3", [](const Adaptor3d_Curve& self, Standard_Real U) {
            gp_Pnt P;
            gp_Vec V1, V2, V3;
            self.D3(U, P, V1, V2, V3);
            return std::make_tuple(P, V1, V2, V3);
        }, py::arg("u"),
            "Computes the point and first three derivatives at parameter U.\n\n"
            "Parameters:\n"
            "  u: Parameter value\n\n"
            "Returns:\n"
            "  Tuple of (point, d1, d2, d3)\n\n"
            "Raises:\n"
            "  If continuity of current interval is not C3")
        
        .def("dn", &Adaptor3d_Curve::DN, py::arg("u"), py::arg("n"),
            "Returns the derivative of order N at parameter U.\n\n"
            "Parameters:\n"
            "  u: Parameter value\n"
            "  n: Order of derivation (>= 1)\n\n"
            "Returns:\n"
            "  gp_Vec derivative\n\n"
            "Raises:\n"
            "  If continuity of current interval is not CN\n"
            "  If N < 1")
        
        .def("resolution", &Adaptor3d_Curve::Resolution, py::arg("r3d"),
            "Returns the parametric resolution corresponding to the real space resolution R3d.\n\n"
            "Parameters:\n"
            "  r3d: Real space resolution\n\n"
            "Returns:\n"
            "  Parametric resolution")
        
        // Curve type
        .def("get_type", &Adaptor3d_Curve::GetType,
            "Returns the type of the curve in the current interval.\n\n"
            "Returns:\n"
            "  GeomAbs.CurveType: Line, Circle, Ellipse, Hyperbola, Parabola,\n"
            "  BezierCurve, BSplineCurve, OffsetCurve, or OtherCurve")
        
        // Specific curve types
        .def("line", &Adaptor3d_Curve::Line,
            "Returns the line if GetType() == Line.\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if curve is not a line")
        .def("circle", &Adaptor3d_Curve::Circle,
            "Returns the circle if GetType() == Circle.\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if curve is not a circle")
        .def("ellipse", &Adaptor3d_Curve::Ellipse,
            "Returns the ellipse if GetType() == Ellipse.\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if curve is not an ellipse")
        .def("hyperbola", &Adaptor3d_Curve::Hyperbola,
            "Returns the hyperbola if GetType() == Hyperbola.\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if curve is not a hyperbola")
        .def("parabola", &Adaptor3d_Curve::Parabola,
            "Returns the parabola if GetType() == Parabola.\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if curve is not a parabola")
        
        // BSpline/Bezier properties
        .def_property_readonly("degree", &Adaptor3d_Curve::Degree,
            "Returns the degree for BSpline/Bezier curves.")
        .def_property_readonly("is_rational", &Adaptor3d_Curve::IsRational,
            "Returns True if the curve is rational (BSpline/Bezier).")
        .def_property_readonly("nb_poles", &Adaptor3d_Curve::NbPoles,
            "Returns the number of poles for BSpline/Bezier curves.")
        .def_property_readonly("nb_knots", &Adaptor3d_Curve::NbKnots,
            "Returns the number of knots for BSpline curves.")
        
        .def("bezier", &Adaptor3d_Curve::Bezier,
            "Returns the Bezier curve if GetType() == BezierCurve.\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if curve is not a Bezier curve")
        .def("bspline", &Adaptor3d_Curve::BSpline,
            "Returns the BSpline curve if GetType() == BSplineCurve.\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if curve is not a BSpline curve")
        .def("offset_curve", &Adaptor3d_Curve::OffsetCurve,
            "Returns the offset curve if GetType() == OffsetCurve.\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if curve is not an offset curve")
        
        .def("shallow_copy", &Adaptor3d_Curve::ShallowCopy,
            "Returns a shallow copy of this adaptor.");
    

    // implicit constructor registrations
    py::implicitly_convertible<opencascade::handle<Geom_Curve>, Adaptor3d_Curve>();
    py::implicitly_convertible<TopoDS_Edge, Adaptor3d_Curve>();
    py::implicitly_convertible<TopoDS_Wire, Adaptor3d_Curve>();        
    // =========================================================================
    // GeomAdaptor_Curve - Adaptor for Geom_Curve
    // =========================================================================
    py::class_<GeomAdaptor_Curve, Adaptor3d_Curve, Handle(GeomAdaptor_Curve)>(m, "GeomCurve",
        "Adaptor interface between Geom_Curve and algorithms that use adapted curves.\n\n"
        "This class provides an interface between the services provided by any curve\n"
        "from the package Geom and those required of the curve by algorithms which use it.\n"
        "Creation of the loaded curve the curve is C1 by piece.\n\n"
        "Note: Polynomial coefficients of BSpline curves used for their evaluation are\n"
        "cached for better performance. Therefore these evaluations are not thread-safe.\n\n"
        "Example:\n"
        "  curve = Geom.BSplineCurve(...)  # Create a Geom curve\n"
        "  adaptor = Adaptor.GeomCurve(curve)  # Adapt it\n"
        "  pt = adaptor.value(0.5)  # Evaluate at parameter 0.5")
        
        .def(py::init<>(),
            "Creates an undefined curve with no Geom_Curve loaded.")
        
        .def(py::init<const Handle(Geom_Curve)&>(), py::arg("curve"),
            "Creates an adaptor for the given Geom_Curve.\n\n"
            "Parameters:\n"
            "  curve: The Geom_Curve to adapt\n\n"
            "Raises:\n"
            "  Standard_NullObject if curve is null")
        
        .def(py::init<const Handle(Geom_Curve)&, Standard_Real, Standard_Real>(),
            py::arg("curve"), py::arg("u_first"), py::arg("u_last"),
            "Creates an adaptor for a portion of the Geom_Curve.\n\n"
            "Parameters:\n"
            "  curve: The Geom_Curve to adapt\n"
            "  u_first: First parameter\n"
            "  u_last: Last parameter\n\n"
            "Raises:\n"
            "  Standard_NullObject if curve is null\n"
            "  Standard_ConstructionError if u_first > u_last")
        
        .def("load", py::overload_cast<const Handle(Geom_Curve)&>(&GeomAdaptor_Curve::Load),
            py::arg("curve"),
            "Loads a Geom_Curve.\n\n"
            "Parameters:\n"
            "  curve: The Geom_Curve to load\n\n"
            "Raises:\n"
            "  Standard_NullObject if curve is null")
        
        .def("load", py::overload_cast<const Handle(Geom_Curve)&, Standard_Real, Standard_Real>(
            &GeomAdaptor_Curve::Load),
            py::arg("curve"), py::arg("u_first"), py::arg("u_last"),
            "Loads a portion of a Geom_Curve.\n\n"
            "Parameters:\n"
            "  curve: The Geom_Curve to load\n"
            "  u_first: First parameter\n"
            "  u_last: Last parameter\n\n"
            "Raises:\n"
            "  Standard_NullObject if curve is null\n"
            "  Standard_ConstructionError if u_first > u_last")
        
        .def("reset", &GeomAdaptor_Curve::Reset,
            "Reset the currently loaded curve (undoes Load()).")
        
        .def_property_readonly("curve", &GeomAdaptor_Curve::Curve,
            "Returns the underlying Geom_Curve.");
    
    // =========================================================================
    // BRepAdaptor_Curve - Adaptor for Edge geometry
    // =========================================================================
    py::class_<BRepAdaptor_Curve, Adaptor3d_Curve, Handle(BRepAdaptor_Curve)>(m, "BRepCurve",
        "Adaptor to use an Edge of BRep topology like a 3D curve.\n\n"
        "The Curve from BRepAdaptor allows to use an Edge of the BRep topology\n"
        "like a 3D curve. It has the methods of the class Curve from Adaptor3d.\n\n"
        "It is created or initialized with an Edge. It takes into account local\n"
        "coordinate systems. If the Edge has a 3D curve it is used with priority.\n"
        "If the edge has no 3D curve one of the curves on surface is used.\n"
        "It is possible to enforce using a curve on surface by creating or\n"
        "initializing with an Edge and a Face.\n\n"
        "Example:\n"
        "  edge = ...  # Get an edge from a shape\n"
        "  adaptor = Adaptor.BRepCurve(edge)\n"
        "  pt = adaptor.value(0.5)  # Evaluate at mid-parameter")
        
        .def(py::init<>(),
            "Creates an undefined Curve with no Edge loaded.")
        
        .def(py::init<const TopoDS_Edge&>(), py::arg("edge"),
            "Creates a Curve to access the geometry of edge E.\n\n"
            "Parameters:\n"
            "  edge: The edge to adapt")
        
        .def(py::init<const TopoDS_Edge&, const TopoDS_Face&>(),
            py::arg("edge"), py::arg("face"),
            "Creates a Curve to access the geometry of edge E on face F.\n\n"
            "The geometry will be computed using the parametric curve of E on F.\n\n"
            "Parameters:\n"
            "  edge: The edge to adapt\n"
            "  face: The face containing the edge\n\n"
            "Raises:\n"
            "  Standard_DomainError if the edge does not have a pcurve on the face")
        
        .def("initialize", py::overload_cast<const TopoDS_Edge&>(&BRepAdaptor_Curve::Initialize),
            py::arg("edge"),
            "Sets the Curve to access the geometry of edge E.\n\n"
            "Parameters:\n"
            "  edge: The edge to adapt")
        
        .def("initialize", py::overload_cast<const TopoDS_Edge&, const TopoDS_Face&>(
            &BRepAdaptor_Curve::Initialize),
            py::arg("edge"), py::arg("face"),
            "Sets the Curve to access the geometry of edge E on face F.\n\n"
            "Parameters:\n"
            "  edge: The edge to adapt\n"
            "  face: The face containing the edge\n\n"
            "Raises:\n"
            "  Standard_DomainError if the edge does not have a pcurve on the face")
        
        .def("reset", &BRepAdaptor_Curve::Reset,
            "Reset the currently loaded curve (undoes Initialize()).")
        
        .def_property_readonly("trsf", &BRepAdaptor_Curve::Trsf,
            "Returns the coordinate system of the curve.")
        
        .def_property_readonly("is_3d_curve", &BRepAdaptor_Curve::Is3DCurve,
            "Returns True if the edge geometry is computed from a 3D curve.")
        
        .def_property_readonly("is_curve_on_surface", &BRepAdaptor_Curve::IsCurveOnSurface,
            "Returns True if the edge geometry is computed from a pcurve on a surface.")
        
        .def_property_readonly("geom_curve", &BRepAdaptor_Curve::Curve,
            "Returns the GeomAdaptor_Curve of the edge.")
        
        .def_property_readonly("curve_on_surface", &BRepAdaptor_Curve::CurveOnSurface,
            "Returns the Adaptor3d_CurveOnSurface of the edge.")
        
        .def_property_readonly("edge", &BRepAdaptor_Curve::Edge,
            "Returns the edge.")
        
        .def_property_readonly("tolerance", &BRepAdaptor_Curve::Tolerance,
            "Returns the edge tolerance.");
    
    // =========================================================================
    // BRepAdaptor_CompCurve - Adaptor for Wire as a 3D curve
    // =========================================================================
    py::class_<BRepAdaptor_CompCurve, Adaptor3d_Curve, Handle(BRepAdaptor_CompCurve)>(m, "CompCurve",
        "Adaptor to use a Wire of BRep topology like a 3D curve.\n\n"
        "The Curve from BRepAdaptor allows to use a Wire of the BRep topology\n"
        "like a 3D curve.\n\n"
        "Warning: With this class of curve, C0 and C1 continuities are not assumed.\n"
        "So be careful with some algorithms!\n\n"
        "Please note that BRepAdaptor_CompCurve cannot be a periodic curve at all\n"
        "(even if it contains a single periodic edge).\n\n"
        "BRepAdaptor_CompCurve can only work on valid wires where all edges are\n"
        "connected to each other to make a chain.\n\n"
        "Example:\n"
        "  wire = ...  # Get a wire from a shape\n"
        "  adaptor = Adaptor.CompCurve(wire)\n"
        "  pt = adaptor.value(0.5)  # Evaluate at mid-parameter")
        
        .def(py::init<>(),
            "Creates an undefined Curve with no Wire loaded.")
        
        .def(py::init<const TopoDS_Wire&, Standard_Boolean>(),
            py::arg("wire"), py::arg("knot_by_curvilinear_abscissa") = false,
            "Creates a Curve to access the geometry of wire W.\n\n"
            "Parameters:\n"
            "  wire: The wire to adapt\n"
            "  knot_by_curvilinear_abscissa: If True, use curvilinear abscissa for knots")
        
        .def(py::init<const TopoDS_Wire&, Standard_Boolean, Standard_Real, Standard_Real, Standard_Real>(),
            py::arg("wire"), py::arg("knot_by_curvilinear_abscissa"),
            py::arg("first"), py::arg("last"), py::arg("tol"),
            "Creates a Curve to access a trimmed portion of wire W.\n\n"
            "Parameters:\n"
            "  wire: The wire to adapt\n"
            "  knot_by_curvilinear_abscissa: If True, use curvilinear abscissa for knots\n"
            "  first: First parameter\n"
            "  last: Last parameter\n"
            "  tol: Tolerance")
        
        .def("initialize", py::overload_cast<const TopoDS_Wire&, Standard_Boolean>(
            &BRepAdaptor_CompCurve::Initialize),
            py::arg("wire"), py::arg("knot_by_curvilinear_abscissa"),
            "Sets the wire W.\n\n"
            "Parameters:\n"
            "  wire: The wire to adapt\n"
            "  knot_by_curvilinear_abscissa: If True, use curvilinear abscissa for knots")
        
        .def("initialize", py::overload_cast<const TopoDS_Wire&, Standard_Boolean, Standard_Real, Standard_Real, Standard_Real>(
            &BRepAdaptor_CompCurve::Initialize),
            py::arg("wire"), py::arg("knot_by_curvilinear_abscissa"),
            py::arg("first"), py::arg("last"), py::arg("tol"),
            "Sets a trimmed portion of wire W.\n\n"
            "Parameters:\n"
            "  wire: The wire to adapt\n"
            "  knot_by_curvilinear_abscissa: If True, use curvilinear abscissa for knots\n"
            "  first: First parameter\n"
            "  last: Last parameter\n"
            "  tol: Tolerance")
        
        .def_property_readonly("wire", &BRepAdaptor_CompCurve::Wire,
            "Returns the wire.")
        
        .def("edge", [](const BRepAdaptor_CompCurve& self, Standard_Real U) {
            TopoDS_Edge E;
            Standard_Real UonE;
            self.Edge(U, E, UonE);
            return std::make_tuple(E, UonE);
        }, py::arg("u"),
            "Returns an edge and one parameter on it corresponding to parameter U.\n\n"
            "Parameters:\n"
            "  u: Parameter on the composite curve\n\n"
            "Returns:\n"
            "  Tuple of (edge, parameter_on_edge)");
    
    // =========================================================================
    // Adaptor3d_IsoCurve - Isoparametric curve on a surface
    // =========================================================================
    py::class_<Adaptor3d_IsoCurve, Adaptor3d_Curve, Handle(Adaptor3d_IsoCurve)>(m, "IsoCurve",
        "Defines an isoparametric curve on a surface.\n\n"
        "The type of isoparametric curve (U or V) is defined with the enumeration\n"
        "IsoType from GeomAbs. If NoneIso is given an error is raised.\n\n"
        "Example:\n"
        "  surface_adaptor = ...  # Get a surface adaptor\n"
        "  iso_u = Adaptor.IsoCurve(surface_adaptor, GeomAbs.IsoU, 0.5)\n"
        "  pt = iso_u.value(0.0)  # Point at V=0 on the U=0.5 isocurve")
        
        .def(py::init<>(),
            "Creates an undefined IsoCurve. The iso is set to NoneIso.")
        
        .def(py::init<const Handle(Adaptor3d_Surface)&>(), py::arg("surface"),
            "Creates an IsoCurve on a surface. The iso is set to NoneIso.\n\n"
            "Parameters:\n"
            "  surface: The surface adaptor")
        
        .def(py::init<const Handle(Adaptor3d_Surface)&, GeomAbs_IsoType, Standard_Real>(),
            py::arg("surface"), py::arg("iso"), py::arg("param"),
            "Creates an IsoCurve on a surface.\n\n"
            "Parameters:\n"
            "  surface: The surface adaptor\n"
            "  iso: The type of iso curve (IsoU or IsoV)\n"
            "  param: The parameter value defining the iso curve\n\n"
            "The bounds of the iso curve are the bounds of the surface.")
        
        .def(py::init<const Handle(Adaptor3d_Surface)&, GeomAbs_IsoType, Standard_Real, Standard_Real, Standard_Real>(),
            py::arg("surface"), py::arg("iso"), py::arg("param"),
            py::arg("w_first"), py::arg("w_last"),
            "Creates an IsoCurve on a surface with specified bounds.\n\n"
            "Parameters:\n"
            "  surface: The surface adaptor\n"
            "  iso: The type of iso curve (IsoU or IsoV)\n"
            "  param: The parameter value defining the iso curve\n"
            "  w_first: First bound\n"
            "  w_last: Last bound")
        
        .def("load", py::overload_cast<const Handle(Adaptor3d_Surface)&>(&Adaptor3d_IsoCurve::Load),
            py::arg("surface"),
            "Changes the surface. The iso is reset to NoneIso.\n\n"
            "Parameters:\n"
            "  surface: The new surface adaptor")
        
        .def("load", py::overload_cast<GeomAbs_IsoType, Standard_Real>(&Adaptor3d_IsoCurve::Load),
            py::arg("iso"), py::arg("param"),
            "Changes the iso on the current surface.\n\n"
            "Parameters:\n"
            "  iso: The type of iso curve (IsoU or IsoV)\n"
            "  param: The parameter value")
        
        .def("load", py::overload_cast<GeomAbs_IsoType, Standard_Real, Standard_Real, Standard_Real>(
            &Adaptor3d_IsoCurve::Load),
            py::arg("iso"), py::arg("param"), py::arg("w_first"), py::arg("w_last"),
            "Changes the iso on the current surface with specified bounds.\n\n"
            "Parameters:\n"
            "  iso: The type of iso curve (IsoU or IsoV)\n"
            "  param: The parameter value\n"
            "  w_first: First bound\n"
            "  w_last: Last bound")
        
        .def_property_readonly("surface", &Adaptor3d_IsoCurve::Surface,
            "Returns the surface adaptor.")
        
        .def_property_readonly("iso", &Adaptor3d_IsoCurve::Iso,
            "Returns the type of iso (IsoU, IsoV, or NoneIso).")
        
        .def_property_readonly("parameter", &Adaptor3d_IsoCurve::Parameter,
            "Returns the iso parameter value.");
    
    // =========================================================================
    // Adaptor3d_CurveOnSurface - Curve lying on a surface
    // =========================================================================
    py::class_<Adaptor3d_CurveOnSurface, Adaptor3d_Curve, Handle(Adaptor3d_CurveOnSurface)>(m, "CurveOnSurface",
        "An interface between the services provided by a curve lying on a surface\n"
        "from the package Geom and those required of the curve by algorithms which use it.\n\n"
        "The curve is defined as a 2D curve from the Geom2d package, in the parametric\n"
        "space of the surface.\n\n"
        "Example:\n"
        "  curve_2d = ...  # A 2D curve adaptor in (u,v) space\n"
        "  surface = ...   # A surface adaptor\n"
        "  curve_on_surf = Adaptor.CurveOnSurface(curve_2d, surface)")
        
        .def(py::init<>(),
            "Creates an undefined CurveOnSurface.")
        
        .def(py::init<const Handle(Adaptor3d_Surface)&>(), py::arg("surface"),
            "Creates a CurveOnSurface with only a surface defined.\n\n"
            "Parameters:\n"
            "  surface: The surface adaptor")
        
        .def(py::init<const Handle(Adaptor2d_Curve2d)&, const Handle(Adaptor3d_Surface)&>(),
            py::arg("curve_2d"), py::arg("surface"),
            "Creates a CurveOnSurface from the 2D curve and the surface.\n\n"
            "Parameters:\n"
            "  curve_2d: The 2D curve adaptor in parametric space\n"
            "  surface: The surface adaptor")
        
        .def("load", py::overload_cast<const Handle(Adaptor3d_Surface)&>(&Adaptor3d_CurveOnSurface::Load),
            py::arg("surface"),
            "Changes the surface.\n\n"
            "Parameters:\n"
            "  surface: The new surface adaptor")
        
        .def("load", py::overload_cast<const Handle(Adaptor2d_Curve2d)&>(&Adaptor3d_CurveOnSurface::Load),
            py::arg("curve_2d"),
            "Changes the 2D curve.\n\n"
            "Parameters:\n"
            "  curve_2d: The new 2D curve adaptor")
        
        .def("load", py::overload_cast<const Handle(Adaptor2d_Curve2d)&, const Handle(Adaptor3d_Surface)&>(
            &Adaptor3d_CurveOnSurface::Load),
            py::arg("curve_2d"), py::arg("surface"),
            "Loads both curve and surface.\n\n"
            "Parameters:\n"
            "  curve_2d: The 2D curve adaptor\n"
            "  surface: The surface adaptor")
        
        .def_property_readonly("get_curve", &Adaptor3d_CurveOnSurface::GetCurve,
            "Returns the 2D curve adaptor.")
        
        .def_property_readonly("get_surface", &Adaptor3d_CurveOnSurface::GetSurface,
            "Returns the surface adaptor.");
    
    // =========================================================================
    // Adaptor3d_Surface - Root class for surfaces on which geometric algorithms work
    // =========================================================================
    py::class_<Adaptor3d_Surface, Handle(Adaptor3d_Surface)>(m, "Surface",
        "Root class for surfaces on which geometric algorithms work.\n\n"
        "An adapted surface is an interface between the services provided by a surface\n"
        "and those required of the surface by algorithms which use it.\n\n"
        "Derived concrete classes:\n"
        "- GeomAdaptor_Surface for a surface from the Geom package\n"
        "- BRepAdaptor_Surface for a face geometry as a surface")
        
        // Parameter domain
        .def_property_readonly("first_u_parameter", &Adaptor3d_Surface::FirstUParameter,
            "Returns the first parameter in U direction.")
        .def_property_readonly("last_u_parameter", &Adaptor3d_Surface::LastUParameter,
            "Returns the last parameter in U direction.")
        .def_property_readonly("first_v_parameter", &Adaptor3d_Surface::FirstVParameter,
            "Returns the first parameter in V direction.")
        .def_property_readonly("last_v_parameter", &Adaptor3d_Surface::LastVParameter,
            "Returns the last parameter in V direction.")
        
        // Continuity
        .def_property_readonly("u_continuity", &Adaptor3d_Surface::UContinuity,
            "Returns the continuity in U direction.")
        .def_property_readonly("v_continuity", &Adaptor3d_Surface::VContinuity,
            "Returns the continuity in V direction.")
        
        .def("nb_u_intervals", &Adaptor3d_Surface::NbUIntervals, py::arg("continuity"),
            "Returns the number of intervals for continuity S in U direction.")
        .def("nb_v_intervals", &Adaptor3d_Surface::NbVIntervals, py::arg("continuity"),
            "Returns the number of intervals for continuity S in V direction.")
        
        // Point evaluation
        .def("value", &Adaptor3d_Surface::Value, py::arg("u"), py::arg("v"),
            "Computes the point at parameters (U, V).")
        
        .def("d0", [](const Adaptor3d_Surface& self, Standard_Real U, Standard_Real V) {
            gp_Pnt P;
            self.D0(U, V, P);
            return P;
        }, py::arg("u"), py::arg("v"),
            "Computes the point at parameters (U, V).")
        
        .def("d1", [](const Adaptor3d_Surface& self, Standard_Real U, Standard_Real V) {
            gp_Pnt P;
            gp_Vec D1U, D1V;
            self.D1(U, V, P, D1U, D1V);
            return std::make_tuple(P, D1U, D1V);
        }, py::arg("u"), py::arg("v"),
            "Computes the point and first derivatives at parameters (U, V).\n\n"
            "Returns:\n"
            "  Tuple of (point, d1u, d1v)")
        
        .def("d2", [](const Adaptor3d_Surface& self, Standard_Real U, Standard_Real V) {
            gp_Pnt P;
            gp_Vec D1U, D1V, D2U, D2V, D2UV;
            self.D2(U, V, P, D1U, D1V, D2U, D2V, D2UV);
            return std::make_tuple(P, D1U, D1V, D2U, D2V, D2UV);
        }, py::arg("u"), py::arg("v"),
            "Computes the point and first and second derivatives at parameters (U, V).\n\n"
            "Returns:\n"
            "  Tuple of (point, d1u, d1v, d2u, d2v, d2uv)")
        
        .def("dn", &Adaptor3d_Surface::DN, py::arg("u"), py::arg("v"), py::arg("nu"), py::arg("nv"),
            "Returns the derivative of order (Nu, Nv) at parameters (U, V).")
        
        // Surface type
        .def("get_type", &Adaptor3d_Surface::GetType,
            "Returns the type of the surface.")
        
        // Periodicity
        .def_property_readonly("is_u_closed", &Adaptor3d_Surface::IsUClosed,
            "Returns True if the surface is closed in U direction.")
        .def_property_readonly("is_v_closed", &Adaptor3d_Surface::IsVClosed,
            "Returns True if the surface is closed in V direction.")
        .def_property_readonly("is_u_periodic", &Adaptor3d_Surface::IsUPeriodic,
            "Returns True if the surface is periodic in U direction.")
        .def_property_readonly("is_v_periodic", &Adaptor3d_Surface::IsVPeriodic,
            "Returns True if the surface is periodic in V direction.")
        .def_property_readonly("u_period", &Adaptor3d_Surface::UPeriod,
            "Returns the U period (raises if not periodic).")
        .def_property_readonly("v_period", &Adaptor3d_Surface::VPeriod,
            "Returns the V period (raises if not periodic).")
        
        // Specific surface types
        .def("plane", &Adaptor3d_Surface::Plane,
            "Returns the plane if GetType() == Plane.")
        .def("cylinder", &Adaptor3d_Surface::Cylinder,
            "Returns the cylinder if GetType() == Cylinder.")
        .def("cone", &Adaptor3d_Surface::Cone,
            "Returns the cone if GetType() == Cone.")
        .def("sphere", &Adaptor3d_Surface::Sphere,
            "Returns the sphere if GetType() == Sphere.")
        .def("torus", &Adaptor3d_Surface::Torus,
            "Returns the torus if GetType() == Torus.")
        
        .def("u_resolution", &Adaptor3d_Surface::UResolution, py::arg("r3d"),
            "Returns the parametric U resolution for the real space resolution R3d.")
        .def("v_resolution", &Adaptor3d_Surface::VResolution, py::arg("r3d"),
            "Returns the parametric V resolution for the real space resolution R3d.")
        
        .def("shallow_copy", &Adaptor3d_Surface::ShallowCopy,
            "Returns a shallow copy of this adaptor.");
    
    // =========================================================================
    // GeomAdaptor_Surface - Adaptor for Geom_Surface
    // =========================================================================
    py::class_<GeomAdaptor_Surface, Adaptor3d_Surface, Handle(GeomAdaptor_Surface)>(m, "GeomSurface",
        "Adaptor interface between Geom_Surface and algorithms that use adapted surfaces.\n\n"
        "This class provides an interface between the services provided by any surface\n"
        "from the package Geom and those required of the surface by algorithms which use it.\n\n"
        "Example:\n"
        "  surface = Geom.Plane(...)  # Create a Geom surface\n"
        "  adaptor = Adaptor.GeomSurface(surface)  # Adapt it\n"
        "  pt = adaptor.value(0.5, 0.5)  # Evaluate at parameters (0.5, 0.5)")
        
        .def(py::init<>(),
            "Creates an undefined surface with no Geom_Surface loaded.")
        
        .def(py::init<const Handle(Geom_Surface)&>(), py::arg("surface"),
            "Creates an adaptor for the given Geom_Surface.\n\n"
            "Parameters:\n"
            "  surface: The Geom_Surface to adapt")
        
        .def(py::init<const Handle(Geom_Surface)&, Standard_Real, Standard_Real, Standard_Real, Standard_Real, Standard_Real, Standard_Real>(),
            py::arg("surface"), py::arg("u_first"), py::arg("u_last"),
            py::arg("v_first"), py::arg("v_last"),
            py::arg("tol_u") = 0.0, py::arg("tol_v") = 0.0,
            "Creates an adaptor for a portion of the Geom_Surface.\n\n"
            "Parameters:\n"
            "  surface: The Geom_Surface to adapt\n"
            "  u_first, u_last: U parameter bounds\n"
            "  v_first, v_last: V parameter bounds\n"
            "  tol_u, tol_v: Tolerances")
        
        .def("load", py::overload_cast<const Handle(Geom_Surface)&>(&GeomAdaptor_Surface::Load),
            py::arg("surface"),
            "Loads a Geom_Surface.")
        
        .def("load", py::overload_cast<const Handle(Geom_Surface)&, Standard_Real, Standard_Real, Standard_Real, Standard_Real, Standard_Real, Standard_Real>(
            &GeomAdaptor_Surface::Load),
            py::arg("surface"), py::arg("u_first"), py::arg("u_last"),
            py::arg("v_first"), py::arg("v_last"),
            py::arg("tol_u") = 0.0, py::arg("tol_v") = 0.0,
            "Loads a portion of a Geom_Surface with specified bounds.")
        
        .def_property_readonly("surface", &GeomAdaptor_Surface::Surface,
            "Returns the underlying Geom_Surface.");
    
    // =========================================================================
    // BRepAdaptor_Surface - Adaptor for Face geometry
    // =========================================================================
    py::class_<BRepAdaptor_Surface, Adaptor3d_Surface, Handle(BRepAdaptor_Surface)>(m, "BRepSurface",
        "Adaptor to use a Face of BRep topology like a 3D surface.\n\n"
        "The Surface from BRepAdaptor allows to use a Face of the BRep topology\n"
        "like a 3D surface. It has the methods of the class Surface from Adaptor3d.\n\n"
        "It is created or initialized with a Face. It takes into account the local\n"
        "coordinate system.\n\n"
        "The u,v parameter range is the minmax value for the restriction, unless\n"
        "the flag restriction is set to false.\n\n"
        "Example:\n"
        "  face = ...  # Get a face from a shape\n"
        "  adaptor = Adaptor.BRepSurface(face)\n"
        "  pt = adaptor.value(0.5, 0.5)  # Evaluate at parameters")
        
        .def(py::init<>(),
            "Creates an undefined surface with no face loaded.")
        
        .def(py::init<const TopoDS_Face&, Standard_Boolean>(),
            py::arg("face"), py::arg("restriction") = true,
            "Creates a surface to access the geometry of F.\n\n"
            "Parameters:\n"
            "  face: The face to adapt\n"
            "  restriction: If True, the parameter range is the restriction range")
        
        .def("initialize", &BRepAdaptor_Surface::Initialize,
            py::arg("face"), py::arg("restriction") = true,
            "Sets the surface to the geometry of F.\n\n"
            "Parameters:\n"
            "  face: The face to adapt\n"
            "  restriction: If True, the parameter range is the restriction range")
        
        .def_property_readonly("geom_surface", &BRepAdaptor_Surface::Surface,
            "Returns the GeomAdaptor_Surface of the face.")
        
        .def_property_readonly("trsf", &BRepAdaptor_Surface::Trsf,
            "Returns the surface coordinate system.")
        
        .def_property_readonly("face", &BRepAdaptor_Surface::Face,
            "Returns the face.")
        
        .def_property_readonly("tolerance", &BRepAdaptor_Surface::Tolerance,
            "Returns the face tolerance.");
}