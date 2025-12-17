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
#include <Geom_Line.hxx>
#include <Geom_BoundedCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GeomAbs_Shape.hxx>
#include <Geom_OffsetCurve.hxx>

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

    py::class_<Geom_OffsetCurve, opencascade::handle<Geom_OffsetCurve>, Geom_Curve>(m, "OffsetCurve",
        "Describes an offset curve in 3D space.\n"
        "An offset curve is a curve at constant distance (Offset) from a basis curve\n"
        "in a reference direction V. The offset curve takes its parametrization from\n"
        "the basis curve. The offset direction is N = T ^ V / ||T ^ V||, where T is\n"
        "the first derivative of the basis curve.\n"
        "Formula: Value(U) = BasisCurve.Value(U) + (Offset * (T ^ V)) / ||T ^ V||\n"
        "Note: The offset curve can be self-intersecting even if the basis curve is not.\n"
        "Continuity is one degree less than the basis curve.")
        
        // Constructor
        .def(py::init([](const opencascade::handle<Geom_Curve>& c, 
                         double offset, 
                         const gp_Dir& v,
                         bool is_not_check_c0) {
            return opencascade::handle<Geom_OffsetCurve>(
                new Geom_OffsetCurve(c, offset, v, is_not_check_c0));
        }), py::arg("basis_curve"), py::arg("offset"), py::arg("direction"),
            py::arg("is_not_check_c0") = false,
            "Creates an offset curve from a basis curve.\n"
            "basis_curve: The basis curve (must be at least C1)\n"
            "offset: Distance from the basis curve (positive or negative)\n"
            "direction: Fixed reference direction V for offset computation\n"
            "is_not_check_c0: If True, skip C0 continuity check\n"
            "Raises exception if basis curve is not at least C1.\n"
            "Warning: No check is done that ||T^V|| != 0 at any point")
        
        // Method with optional parameter (cannot be a simple property)
        .def("set_basis_curve", &Geom_OffsetCurve::SetBasisCurve,
             py::arg("basis_curve"), py::arg("is_not_check_c0") = false,
             "Changes the basis curve of this offset curve.\n"
             "Raises exception if the curve is not at least C1 continuous")
        
        // Informational method
        .def("get_basis_curve_continuity", &Geom_OffsetCurve::GetBasisCurveContinuity,
             "Returns the continuity of the basis curve")
        
        // Properties
        .def_property("offset", &Geom_OffsetCurve::Offset, &Geom_OffsetCurve::SetOffsetValue,
                      "Offset distance from the basis curve")
        .def_property("direction", &Geom_OffsetCurve::Direction, &Geom_OffsetCurve::SetDirection,
                      "Reference direction used to compute the offset direction")
        .def_property_readonly("basis_curve", &Geom_OffsetCurve::BasisCurve,
                               "Basis curve of this offset curve (can itself be an offset curve)")
    ;

}