#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <Standard_Handle.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Vec.hxx>
#include <gp_Lin.hxx>
#include <gp_Trsf.hxx>
#include <Geom_Geometry.hxx>
#include <Geom_Curve.hxx>
// #include <Geom_Conic.hxx>
// #include <Geom_Circle.hxx>
#include <Geom_Line.hxx>
#include <Geom_BoundedCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
// #include <Geom_BSplineCurve.hxx>
// #include <Geom_BezierCurve.hxx>
#include <GeomAbs_Shape.hxx>

// #include "array_utils.hpp"

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

// Forward declaration
void bind_geom_curves_conics(py::module_ &m);
void bind_geom_curves_splines(py::module_ &m);

void bind_geom_curves(py::module_ &m)
{


    py::class_<Geom_Curve, opencascade::handle<Geom_Curve>, Geom_Geometry>(m, "Curve")
        // Parametrization
        .def("reverse", &Geom_Curve::Reverse)
        .def("reversed", &Geom_Curve::Reversed)
        .def("reversed_parameter", &Geom_Curve::ReversedParameter, py::arg("u"))
        .def("transformed_parameter", &Geom_Curve::TransformedParameter, py::arg("u"), py::arg("t"))
        .def("parametric_transformation", &Geom_Curve::ParametricTransformation, py::arg("t"))
        
        // Parameter bounds
        .def("first_parameter", &Geom_Curve::FirstParameter)
        .def("last_parameter", &Geom_Curve::LastParameter)
        
        // Properties
        .def("is_closed", &Geom_Curve::IsClosed)
        .def("is_periodic", &Geom_Curve::IsPeriodic)
        .def("period", &Geom_Curve::Period)
        .def("continuity", &Geom_Curve::Continuity)
        .def("is_cn", &Geom_Curve::IsCN, py::arg("n"))
        
        // Evaluation
        .def("value", &Geom_Curve::Value, py::arg("u"))
        .def("d0", [](const Geom_Curve& self, Standard_Real u) {
            gp_Pnt p;
            self.D0(u, p);
            return p;
        }, py::arg("u"))
        .def("d1", [](const Geom_Curve& self, Standard_Real u) {
            gp_Pnt p;
            gp_Vec v1;
            self.D1(u, p, v1);
            return py::make_tuple(p, v1);
        }, py::arg("u"))
        .def("d2", [](const Geom_Curve& self, Standard_Real u) {
            gp_Pnt p;
            gp_Vec v1, v2;
            self.D2(u, p, v1, v2);
            return py::make_tuple(p, v1, v2);
        }, py::arg("u"))
        .def("d3", [](const Geom_Curve& self, Standard_Real u) {
            gp_Pnt p;
            gp_Vec v1, v2, v3;
            self.D3(u, p, v1, v2, v3);
            return py::make_tuple(p, v1, v2, v3);
        }, py::arg("u"))
        .def("dn", &Geom_Curve::DN, py::arg("u"), py::arg("n"))
    ;

    // py::class_<Geom_Line, opencascade::handle<Geom_Line>, Geom_Curve>(m, "Line")
    py::class_<Geom_Line, opencascade::handle<Geom_Line>, Geom_Curve>(m, "Line")
        // Constructors
        .def(py::init([](const gp_Ax1& a1)-> opencascade::handle<Geom_Line> { return new Geom_Line(a1); }), py::arg("a1"))
        .def(py::init([](const gp_Lin& line)-> opencascade::handle<Geom_Line> { return new Geom_Line(line); }), py::arg("line"))
        .def(py::init([](const gp_Pnt& p, const gp_Dir& v)-> opencascade::handle<Geom_Line> { return new Geom_Line(p, v); }), py::arg("p"), py::arg("v"))
        
        // Getters
        .def_property("line", &Geom_Line::Lin, &Geom_Line::SetLin)
        .def_property("position", &Geom_Line::Position, &Geom_Line::SetPosition)
        .def_property("direction", [](const Geom_Line& self) { return self.Lin().Direction(); }, &Geom_Line::SetDirection)

    ;

    bind_geom_curves_conics(m);

    py::class_<Geom_BoundedCurve, opencascade::handle<Geom_BoundedCurve>, Geom_Curve>(m, "BoundedCurve")
        .def("start_point", &Geom_BoundedCurve::StartPoint)
        .def("end_point", &Geom_BoundedCurve::EndPoint)
    ;

    py::class_<Geom_TrimmedCurve, opencascade::handle<Geom_TrimmedCurve>, Geom_BoundedCurve>(m, "TrimmedCurve")
        .def(py::init([](const opencascade::handle<Geom_Curve>& c, Standard_Real u1, Standard_Real u2, 
                         Standard_Boolean sense, Standard_Boolean adjust_periodic) {
            return opencascade::handle<Geom_TrimmedCurve>(new Geom_TrimmedCurve(c, u1, u2, sense, adjust_periodic));
        }), py::arg("c"), py::arg("u1"), py::arg("u2"), 
            py::arg("sense") = true, py::arg("adjust_periodic") = true)
        
        // Basis curve
        .def("basis_curve", &Geom_TrimmedCurve::BasisCurve)
        
        // Set trim
        .def("set_trim", &Geom_TrimmedCurve::SetTrim, 
             py::arg("u1"), py::arg("u2"), 
             py::arg("sense") = true, py::arg("adjust_periodic") = true)
    ;

    bind_geom_curves_splines(m);

}