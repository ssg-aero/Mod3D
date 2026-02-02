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
#include <Geom_BSplineCurve.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <GeomLProp_CLProps.hxx>

#include <gbs/bscurve.h>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

// Helper functions to convert gbs curves to OCCT
inline opencascade::handle<Geom_BSplineCurve> gbs_bscurve_to_occt(const gbs::BSCurve<double,3>& crv) {
    auto poles_ = crv.poles();
    TColgp_Array1OfPnt poles(1, static_cast<Standard_Integer>(poles_.size()));
    for (size_t i = 0; i < poles_.size(); ++i) {
        poles.SetValue(static_cast<Standard_Integer>(i + 1), 
                       gp_Pnt(poles_[i][0], poles_[i][1], poles_[i][2]));
    }
    auto [knots, mults] = gbs::knots_and_mults(crv.knotsFlats());
    TColStd_Array1OfReal occt_knots(1, static_cast<Standard_Integer>(knots.size()));
    TColStd_Array1OfInteger occt_mults(1, static_cast<Standard_Integer>(mults.size()));
    for (size_t i = 0; i < knots.size(); ++i) {
        occt_knots.SetValue(static_cast<Standard_Integer>(i + 1), knots[i]);
        occt_mults.SetValue(static_cast<Standard_Integer>(i + 1), static_cast<Standard_Integer>(mults[i]));
    }
    return new Geom_BSplineCurve(poles, occt_knots, occt_mults, crv.degree());
}

inline opencascade::handle<Geom_BSplineCurve> gbs_bscurve_rational_to_occt(const gbs::BSCurveRational<double,3>& crv) {
    auto poles_ = crv.polesProjected();
    TColgp_Array1OfPnt poles(1, static_cast<Standard_Integer>(poles_.size()));
    for (size_t i = 0; i < poles_.size(); ++i) {
        poles.SetValue(static_cast<Standard_Integer>(i + 1), 
                       gp_Pnt(poles_[i][0], poles_[i][1], poles_[i][2]));
    }
    auto weights_ = crv.weights();
    TColStd_Array1OfReal weights(1, static_cast<Standard_Integer>(weights_.size()));
    for (size_t i = 0; i < weights_.size(); ++i) {
        weights.SetValue(static_cast<Standard_Integer>(i + 1), weights_[i]);
    }
    auto [knots, mults] = gbs::knots_and_mults(crv.knotsFlats());
    TColStd_Array1OfReal occt_knots(1, static_cast<Standard_Integer>(knots.size()));
    TColStd_Array1OfInteger occt_mults(1, static_cast<Standard_Integer>(mults.size()));
    for (size_t i = 0; i < knots.size(); ++i) {
        occt_knots.SetValue(static_cast<Standard_Integer>(i + 1), knots[i]);
        occt_mults.SetValue(static_cast<Standard_Integer>(i + 1), static_cast<Standard_Integer>(mults[i]));
    }
    return new Geom_BSplineCurve(poles, weights, occt_knots, occt_mults, crv.degree());
}

// Forward declaration
void bind_geom_curves_conics(py::module_ &m);
void bind_geom_curves_splines(py::module_ &m);

void bind_geom_curves(py::module_ &m)
{


    py::class_<Geom_Curve, opencascade::handle<Geom_Curve>, Geom_Geometry>(m, "Curve")
        // Constructors from gbs types (for implicit conversion support)
        .def(py::init([](const gbs::BSCurve<double,3>& crv) {
            return opencascade::handle<Geom_Curve>(gbs_bscurve_to_occt(crv));
        }), py::arg("bscurve"),
            "Create a Geom_Curve from a gbs::BSCurve object (implicit conversion).")
        .def(py::init([](const gbs::BSCurveRational<double,3>& crv) {
            return opencascade::handle<Geom_Curve>(gbs_bscurve_rational_to_occt(crv));
        }), py::arg("bscurve_rational"),
            "Create a Geom_Curve from a gbs::BSCurveRational object (implicit conversion).")
        
        // Parametrization
        .def("reverse", &Geom_Curve::Reverse)
        .def("reversed", &Geom_Curve::Reversed)
        .def("reversed_parameter", &Geom_Curve::ReversedParameter, py::arg("u"))
        .def("transformed_parameter", &Geom_Curve::TransformedParameter, py::arg("u"), py::arg("t"))
        .def("parametric_transformation", &Geom_Curve::ParametricTransformation, py::arg("t"))
        
        // Parameter bounds
        .def_property_readonly("first_parameter", &Geom_Curve::FirstParameter)
        .def_property_readonly("last_parameter", &Geom_Curve::LastParameter)
        .def_property_readonly("bounds", [](const Geom_Curve& self) {
            return py::make_tuple(self.FirstParameter(), self.LastParameter());
        })
        // Properties
        .def_property_readonly("is_closed", &Geom_Curve::IsClosed)
        .def_property_readonly("is_periodic", &Geom_Curve::IsPeriodic)
        .def_property_readonly("period", &Geom_Curve::Period)
        .def_property_readonly("continuity", &Geom_Curve::Continuity)
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
        .def("curvature", [](const Geom_Curve& self, double u) {
          const opencascade::handle<Geom_Curve> h_curve(&self);
          GeomLProp_CLProps props(h_curve, u, 2, Precision::Confusion());
          return props.Curvature();
        }, py::arg("u"), "Returns (curvature, first derivative of curvature) at parameter U")
        .def("centter_of_curvature", [](const Geom_Curve& self, double u) {
          const opencascade::handle<Geom_Curve> h_curve(&self);
          GeomLProp_CLProps props(h_curve, u, 2, Precision::Confusion());
          gp_Pnt center;
          props.CentreOfCurvature(center);
          return center;
        }, py::arg("u"), "Returns the center of curvature at parameter U")
    ;

    // Register implicit conversions from gbs types to Geom_Curve
    py::implicitly_convertible<gbs::BSCurve<double, 3>, Geom_Curve>();
    py::implicitly_convertible<gbs::BSCurveRational<double, 3>, Geom_Curve>();

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
        .def_property_readonly("start_point", &Geom_BoundedCurve::StartPoint)
        .def_property_readonly("end_point", &Geom_BoundedCurve::EndPoint)
    ;

    py::class_<Geom_TrimmedCurve, opencascade::handle<Geom_TrimmedCurve>, Geom_BoundedCurve>(m, "TrimmedCurve")
        .def(py::init([](const opencascade::handle<Geom_Curve>& c, Standard_Real u1, Standard_Real u2, 
                         Standard_Boolean sense, Standard_Boolean adjust_periodic) {
            return opencascade::handle<Geom_TrimmedCurve>(new Geom_TrimmedCurve(c, u1, u2, sense, adjust_periodic));
        }), py::arg("c"), py::arg("u1"), py::arg("u2"), 
            py::arg("sense") = true, py::arg("adjust_periodic") = true)
        
        // Basis curve
        .def_property_readonly("basis_curve", &Geom_TrimmedCurve::BasisCurve)
        
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