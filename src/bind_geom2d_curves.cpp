#include <Geom_Curve.hxx>
#include <Precision.hxx>
#include <gp.hxx>
#include <pybind11/cast.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <Standard_Handle.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Ax22d.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Lin2d.hxx>

#include <gp_Trsf2d.hxx>

#include <Geom2d_Geometry.hxx>
#include <Geom2d_Curve.hxx>
#include <Geom2d_Line.hxx>
#include <Geom2d_BoundedCurve.hxx>
#include <Geom2d_TrimmedCurve.hxx>
#include <Geom2d_OffsetCurve.hxx>

#include <TColgp_Array1OfPnt2d.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <GeomAbs_Shape.hxx>

#include <Geom2dLProp_CLProps2d.hxx>
#include <Geom2dAdaptor_Curve.hxx>
#include <GCPnts_AbscissaPoint.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);


extern void bind_geom2d_curves_conics(py::module_ &m);
extern void bind_geom2d_curves_splines(py::module_ &m);

void bind_geom2d_curves(py::module_ &m)
{
    // =========================================================================
    // Geom2d_Curve - Base class for all 2D curves
    // =========================================================================
    py::class_<Geom2d_Curve, opencascade::handle<Geom2d_Curve>, Geom2d_Geometry>(m, "Curve2d",
        "Abstract base class for 2D curves.\n"
        "Provides parametric curve operations in 2D space.")
        
        // Parametrization
        .def("reverse", &Geom2d_Curve::Reverse,
             "Changes the direction of parametrization of the curve")
        .def("reversed", &Geom2d_Curve::Reversed,
             "Returns a copy of this curve with reversed parametrization")
        .def("reversed_parameter", &Geom2d_Curve::ReversedParameter, py::arg("u"),
             "Returns the parameter on the reversed curve for the point of parameter U")
        .def("transformed_parameter", &Geom2d_Curve::TransformedParameter, py::arg("u"), py::arg("t"),
             "Returns the parameter on the transformed curve for the transform T")
        .def("parametric_transformation", &Geom2d_Curve::ParametricTransformation, py::arg("t"),
             "Returns a coefficient to compute the parameter on the transformed curve")
        
        // Parameter bounds
        .def_property_readonly("first_parameter", &Geom2d_Curve::FirstParameter,
             "Returns the first parameter of this curve")
        .def_property_readonly("last_parameter", &Geom2d_Curve::LastParameter,
             "Returns the last parameter of this curve")
        .def_property_readonly("bounds", [](const Geom2d_Curve& self) {
            return py::make_tuple(self.FirstParameter(), self.LastParameter());
        }, "Returns (first_parameter, last_parameter)")
        
        // Properties
        .def_property_readonly("is_closed", &Geom2d_Curve::IsClosed,
             "Returns True if the curve is closed")
        .def_property_readonly("is_periodic", &Geom2d_Curve::IsPeriodic,
             "Returns True if the curve is periodic")
        .def_property_readonly("period", &Geom2d_Curve::Period,
             "Returns the period of this curve. Raises if not periodic")
        .def_property_readonly("continuity", &Geom2d_Curve::Continuity,
             "Returns the global continuity of the curve")
        .def("is_cn", &Geom2d_Curve::IsCN, py::arg("n"),
             "Returns True if the curve has CN continuity")
        
        // Evaluation
        .def("value", &Geom2d_Curve::Value, py::arg("u"),
             "Returns the point at parameter U")
        .def("d0", [](const Geom2d_Curve& self, double u) {
            gp_Pnt2d p;
            self.D0(u, p);
            return p;
        }, py::arg("u"), "Returns the point at parameter U")
        .def("d1", [](const Geom2d_Curve& self, double u) {
            gp_Pnt2d p;
            gp_Vec2d v1;
            self.D1(u, p, v1);
            return py::make_tuple(p, v1);
        }, py::arg("u"), "Returns (point, first derivative) at parameter U")
        .def("d2", [](const Geom2d_Curve& self, double u) {
            gp_Pnt2d p;
            gp_Vec2d v1, v2;
            self.D2(u, p, v1, v2);
            return py::make_tuple(p, v1, v2);
        }, py::arg("u"), "Returns (point, first derivative, second derivative) at parameter U")
        .def("d3", [](const Geom2d_Curve& self, double u) {
            gp_Pnt2d p;
            gp_Vec2d v1, v2, v3;
            self.D3(u, p, v1, v2, v3);
            return py::make_tuple(p, v1, v2, v3);
        }, py::arg("u"), "Returns (point, d1, d2, d3) at parameter U")
        .def("dn", &Geom2d_Curve::DN, py::arg("u"), py::arg("n"),
             "Returns the Nth derivative vector at parameter U")
        .def("length", [](const Geom2d_Curve& self, Standard_Real u1, Standard_Real u2) {
            const opencascade::handle<Geom2d_Curve> h_curve(&self);
            Geom2dAdaptor_Curve adaptor(h_curve);
            return GCPnts_AbscissaPoint::Length(adaptor, u1, u2);
        }, py::arg("u1"), py::arg("u2"),
             "Compute the length of the curve between parameters U1 and U2")
        .def("length", [](const Geom2d_Curve& self) {
            const opencascade::handle<Geom2d_Curve> h_curve(&self);
            Geom2dAdaptor_Curve adaptor(h_curve);
            return GCPnts_AbscissaPoint::Length(adaptor);
        }, 
             "Compute the length of the curve")
        .def("curvature", [](const Geom2d_Curve& self, double u) {
          const opencascade::handle<Geom2d_Curve> h_curve(&self);
          Geom2dLProp_CLProps2d props(h_curve, u, 2, Precision::Confusion());
          return props.Curvature();
        }, py::arg("u"), "Returns (curvature, first derivative of curvature) at parameter U")
        .def("center_of_curvature", [](const Geom2d_Curve& self, double u) {
          const opencascade::handle<Geom2d_Curve> h_curve(&self);
          Geom2dLProp_CLProps2d props(h_curve, u, 2, Precision::Confusion());
          gp_Pnt2d center;
          props.CentreOfCurvature(center);
          return center;
        }, py::arg("u"), "Returns the center of curvature at parameter U")
        .def("normal", [](const Geom2d_Curve& self, double u) {
          const opencascade::handle<Geom2d_Curve> h_curve(&self);
          Geom2dLProp_CLProps2d props(h_curve, u, 2, Precision::Confusion());
          gp_Dir2d normal;
          props.Normal(normal);
          return normal;
        }, py::arg("u"), "Returns the normal direction at parameter U ")
        .def("tangent", [](const Geom2d_Curve& self, double u) {
          const opencascade::handle<Geom2d_Curve> h_curve(&self);
          Geom2dLProp_CLProps2d props(h_curve, u, 1, Precision::Confusion());
          gp_Dir2d tangent;
          props.Tangent(tangent);
          return tangent;
        }, py::arg("u"), "Returns the tangent direction at parameter U ")

    ;

    // =========================================================================
    // Geom2d_Line
    // =========================================================================
    py::class_<Geom2d_Line, opencascade::handle<Geom2d_Line>, Geom2d_Curve>(m, "Line2d",
        "Describes a line in 2D space.\n"
        "A line is positioned with an axis (gp_Ax2d) defining origin and direction.\n"
        "Parameterization: P(U) = O + U*Dir where O is origin, Dir is direction.\n"
        "Parameter range: ]-infinite, +infinite[")
        
        // Constructors
        .def(py::init([](const gp_Ax2d& a) {
            return opencascade::handle<Geom2d_Line>(new Geom2d_Line(a));
        }), py::arg("a"),
             "Constructs a line from an axis")
        .def(py::init([](const gp_Lin2d& l) {
            return opencascade::handle<Geom2d_Line>(new Geom2d_Line(l));
        }), py::arg("l"),
             "Constructs a line by conversion of gp_Lin2d")
        .def(py::init([](const gp_Pnt2d& p, const gp_Dir2d& v) {
            return opencascade::handle<Geom2d_Line>(new Geom2d_Line(p, v));
        }), py::arg("p"), py::arg("v"),
             "Constructs a line passing through point P with direction V")
        
        // Distance
        .def("distance", &Geom2d_Line::Distance, py::arg("p"),
             "Computes the distance between a point and this line")
        
        // Properties
        .def_property("direction", &Geom2d_Line::Direction, &Geom2d_Line::SetDirection)
        .def_property("location", &Geom2d_Line::Location, &Geom2d_Line::SetLocation)
        .def_property("position", &Geom2d_Line::Position, &Geom2d_Line::SetPosition)
        .def_property("line", &Geom2d_Line::Lin2d, &Geom2d_Line::SetLin2d)
    ;

    bind_geom2d_curves_conics(m);



    // =========================================================================
    // Geom2d_BoundedCurve - Base class for bounded curves
    // =========================================================================
    py::class_<Geom2d_BoundedCurve, opencascade::handle<Geom2d_BoundedCurve>, Geom2d_Curve>(m, "BoundedCurve2d",
        "Abstract base class for bounded curves in 2D space.\n"
        "A bounded curve has a finite extent defined by start and end points.")
        
        .def_property_readonly("start_point", &Geom2d_BoundedCurve::StartPoint,
             "Returns the start point of the curve")
        .def_property_readonly("end_point", &Geom2d_BoundedCurve::EndPoint,
             "Returns the end point of the curve")
    ;

    // =========================================================================
    // Geom2d_TrimmedCurve
    // =========================================================================
    py::class_<Geom2d_TrimmedCurve, opencascade::handle<Geom2d_TrimmedCurve>, Geom2d_BoundedCurve>(m, "TrimmedCurve2d",
        "Describes a portion of a curve limited by two parameter values.\n"
        "The trimmed curve has the same parametric properties as the basis curve.")
        
        // Constructors
        .def(py::init([](const opencascade::handle<Geom2d_Curve>& c, double u1, double u2,
                         bool sense, bool adjust_periodic) {
            return opencascade::handle<Geom2d_TrimmedCurve>(
                new Geom2d_TrimmedCurve(c, u1, u2, sense, adjust_periodic));
        }), py::arg("c"), py::arg("u1"), py::arg("u2"),
            py::arg("sense") = true, py::arg("adjust_periodic") = true,
             "Creates a trimmed curve from curve C between parameters U1 and U2")
        
        // Methods
        .def_property_readonly("basis_curve", &Geom2d_TrimmedCurve::BasisCurve,
             "Returns the basis curve")
        .def("set_trim", &Geom2d_TrimmedCurve::SetTrim,
             py::arg("u1"), py::arg("u2"),
             py::arg("sense") = true, py::arg("adjust_periodic") = true,
             "Changes the trim parameters")
    ;

     bind_geom2d_curves_splines(m);

    // =========================================================================
    // Geom2d_OffsetCurve
    // =========================================================================
    py::class_<Geom2d_OffsetCurve, opencascade::handle<Geom2d_OffsetCurve>, Geom2d_Curve>(m, "OffsetCurve2d",
        "Describes an offset curve in 2D space.\n"
        "An offset curve is at constant distance (offset) from a basis curve.\n"
        "The offset is perpendicular to the basis curve.")
        
        // Constructor
        .def(py::init([](const opencascade::handle<Geom2d_Curve>& c, double offset, bool is_not_check_c0) {
            return opencascade::handle<Geom2d_OffsetCurve>(
                new Geom2d_OffsetCurve(c, offset, is_not_check_c0));
        }), py::arg("basis_curve"), py::arg("offset"), py::arg("is_not_check_c0") = false,
             "Creates an offset curve from a basis curve.\n"
             "Offset is the distance (positive = left side of curve direction)")
        
        // Setters
        .def("set_basis_curve", &Geom2d_OffsetCurve::SetBasisCurve,
             py::arg("c"), py::arg("is_not_check_c0") = false,
             "Changes the basis curve")
        .def("set_offset_value", &Geom2d_OffsetCurve::SetOffsetValue, py::arg("d"),
             "Changes the offset value")
        
        // Getters
        .def("basis_curve", &Geom2d_OffsetCurve::BasisCurve,
             "Returns the basis curve")
        .def("offset", &Geom2d_OffsetCurve::Offset,
             "Returns the offset value")
        
        // Properties
        .def_property("offset", &Geom2d_OffsetCurve::Offset, &Geom2d_OffsetCurve::SetOffsetValue)
    ;
}