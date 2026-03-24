#include <pybind11/pybind11.h>

#include "array_utils.hpp"

#include <Standard_Handle.hxx>
#include <Geom2d_BSplineCurve.hxx>
#include <Geom2d_BezierCurve.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_geom2d_curves_splines(py::module_ &m)
{
        // =========================================================================
    // Geom2d_BSplineCurve
    // =========================================================================
    py::class_<Geom2d_BSplineCurve, opencascade::handle<Geom2d_BSplineCurve>, Geom2d_BoundedCurve>(m, "BSplineCurve2d",
        "Describes a B-spline curve in 2D space.\n"
        "A B-spline curve is defined by poles, knots, multiplicities and degree.")
        
        // Constructors
        .def(py::init([](const py::handle& poles_container, const py::handle& knots_container,
                         const py::handle& mults_container, int degree, bool periodic) {
            auto poles = py_container_to_pnt2d_array(poles_container);
            auto knots = py_container_to_occt_array<double, TColStd_Array1OfReal>(knots_container);
            auto mults = py_container_to_occt_array<int, TColStd_Array1OfInteger>(mults_container);
            return opencascade::handle<Geom2d_BSplineCurve>(
                new Geom2d_BSplineCurve(poles, knots, mults, degree, periodic));
        }), py::arg("poles"), py::arg("knots"), py::arg("multiplicities"),
            py::arg("degree"), py::arg("periodic") = false,
            "Create a non-rational B-spline curve")
        
        .def(py::init([](const py::handle& poles_container, const py::handle& weights_container,
                         const py::handle& knots_container, const py::handle& mults_container,
                         int degree, bool periodic) {
            auto poles = py_container_to_pnt2d_array(poles_container);
            auto weights = py_container_to_occt_array<double, TColStd_Array1OfReal>(weights_container);
            auto knots = py_container_to_occt_array<double, TColStd_Array1OfReal>(knots_container);
            auto mults = py_container_to_occt_array<int, TColStd_Array1OfInteger>(mults_container);
            return opencascade::handle<Geom2d_BSplineCurve>(
                new Geom2d_BSplineCurve(poles, weights, knots, mults, degree, periodic));
        }), py::arg("poles"), py::arg("weights"), py::arg("knots"), py::arg("multiplicities"),
            py::arg("degree"), py::arg("periodic") = false,
            "Create a rational B-spline curve")
        
        // Modification methods
        .def("increase_degree", &Geom2d_BSplineCurve::IncreaseDegree, py::arg("degree"),
             "Increases the degree of this curve")
        .def("insert_knot", &Geom2d_BSplineCurve::InsertKnot,
             py::arg("U"), py::arg("M") = 1, py::arg("ParametricTolerance") = 0.0,
             "Inserts a knot")
        .def("insert_knots", [](Geom2d_BSplineCurve& self,
                                 const py::handle& knots_container,
                                 const py::handle& mults_container,
                                 double parametric_tolerance = 0.0,
                                 bool add = false) {
               auto knots = py_container_to_occt_array<double, TColStd_Array1OfReal>(knots_container);
               auto mults = py_container_to_occt_array<int, TColStd_Array1OfInteger>(mults_container);
               self.InsertKnots(knots, mults, parametric_tolerance, add);
            },
             py::arg("Knots"), py::arg("Multiplicities"), py::arg("ParametricTolerance") = 0.0, py::arg("Add") = false,
             "Inserts multiple knots")
        .def("remove_knot", &Geom2d_BSplineCurve::RemoveKnot,
             py::arg("Index"), py::arg("M"), py::arg("Tolerance"),
             "Removes a knot")
        .def("segment", &Geom2d_BSplineCurve::Segment,
             py::arg("U1"), py::arg("U2"), py::arg("theTolerance") = 1e-12,
             "Modifies this B-spline curve by segmenting it between U1 and U2")
        
        // Knot and pole modification
        .def("set_knot", py::overload_cast<const int, const double>(&Geom2d_BSplineCurve::SetKnot),
             py::arg("Index"), py::arg("K"))
        .def("set_pole", py::overload_cast<const int, const gp_Pnt2d&>(&Geom2d_BSplineCurve::SetPole),
             py::arg("Index"), py::arg("P"))
        .def("set_pole", py::overload_cast<const int, const gp_Pnt2d&, const double>(&Geom2d_BSplineCurve::SetPole),
             py::arg("Index"), py::arg("P"), py::arg("Weight"))
        .def("set_weight", &Geom2d_BSplineCurve::SetWeight, py::arg("Index"), py::arg("Weight"))
        
        // Periodic operations
        .def("set_periodic", &Geom2d_BSplineCurve::SetPeriodic)
        .def("set_not_periodic", &Geom2d_BSplineCurve::SetNotPeriodic)
        .def("set_origin", py::overload_cast<const int>(&Geom2d_BSplineCurve::SetOrigin), py::arg("Index"))
        
        // Properties
        .def_property_readonly("degree", &Geom2d_BSplineCurve::Degree)
        .def_property_readonly("is_rational", &Geom2d_BSplineCurve::IsRational)
        // .def_property_readonly("is_periodic", &Geom2d_BSplineCurve::IsPeriodic)
        // .def_property_readonly("is_closed", &Geom2d_BSplineCurve::IsClosed)
        // .def_property_readonly("continuity", &Geom2d_BSplineCurve::Continuity)
        
        // Knot information
        .def_property_readonly("nb_knots", &Geom2d_BSplineCurve::NbKnots)
        .def_property_readonly("nb_poles", &Geom2d_BSplineCurve::NbPoles)
        .def_property_readonly("first_u_knot_index", &Geom2d_BSplineCurve::FirstUKnotIndex)
        .def_property_readonly("last_u_knot_index", &Geom2d_BSplineCurve::LastUKnotIndex)
        .def("knot", &Geom2d_BSplineCurve::Knot, py::arg("Index"))
        .def("multiplicity", &Geom2d_BSplineCurve::Multiplicity, py::arg("Index"))
        
        // Access to poles and weights
        .def("pole", &Geom2d_BSplineCurve::Pole, py::arg("Index"))
        .def("weight", &Geom2d_BSplineCurve::Weight, py::arg("Index"))
        
        // Access arrays as numpy arrays
        .def_property_readonly("knots", [](const Geom2d_BSplineCurve& self) {
            return occt_array_to_numpy(self.Knots());
        }, "Get knots as numpy array")
        .def_property_readonly("multiplicities", [](const Geom2d_BSplineCurve& self) {
            return occt_array_to_numpy(self.Multiplicities());
        }, "Get multiplicities as numpy array")
        .def_property_readonly("poles", [](const Geom2d_BSplineCurve& self) {
            return pnt_array_to_numpy(self.Poles());
        }, "Get poles as numpy array of shape (n,2)")
        .def_property_readonly("weights", [](const Geom2d_BSplineCurve& self) {
            const TColStd_Array1OfReal* weights_ptr = self.Weights();
            if (weights_ptr) {
                return occt_array_to_numpy(*weights_ptr);
            }
            return py::array_t<double>();
        }, "Get weights as numpy array")
        .def_property_readonly("knot_sequence", [](const Geom2d_BSplineCurve& self) {
            return occt_array_to_numpy(self.KnotSequence());
        }, "Get knot sequence as numpy array")
        
        // Locate parameter
        .def("locate_u", [](const Geom2d_BSplineCurve& self, double U, double ParametricTolerance, bool WithKnotRepetition) {
            int I1, I2;
            self.LocateU(U, ParametricTolerance, I1, I2, WithKnotRepetition);
            return py::make_tuple(I1, I2);
        }, py::arg("U"), py::arg("ParametricTolerance"), py::arg("WithKnotRepetition") = false)
    ;

    // =========================================================================
    // Geom2d_BezierCurve
    // =========================================================================
    py::class_<Geom2d_BezierCurve, opencascade::handle<Geom2d_BezierCurve>, Geom2d_BoundedCurve>(m, "BezierCurve2d",
        "Describes a Bezier curve in 2D space.\n"
        "A Bezier curve is defined by control points (poles).\n"
        "Parameter range: [0, 1]")
        
        // Constructors
        .def(py::init([](const py::handle& poles_container) {
            auto poles = py_container_to_pnt2d_array(poles_container);
            return opencascade::handle<Geom2d_BezierCurve>(new Geom2d_BezierCurve(poles));
        }), py::arg("poles"),
            "Create a non-rational Bezier curve from poles")
        
        .def(py::init([](const py::handle& poles_container, const py::handle& weights_container) {
            auto poles = py_container_to_pnt2d_array(poles_container);
            auto weights = py_container_to_occt_array<double, TColStd_Array1OfReal>(weights_container);
            return opencascade::handle<Geom2d_BezierCurve>(new Geom2d_BezierCurve(poles, weights));
        }), py::arg("poles"), py::arg("weights"),
            "Create a rational Bezier curve from poles and weights")
        
        // Modification
        .def("increase", &Geom2d_BezierCurve::Increase, py::arg("degree"),
             "Increases the degree of this Bezier curve")
        .def("insert_pole_after", py::overload_cast<const int, const gp_Pnt2d&, const double>(&Geom2d_BezierCurve::InsertPoleAfter),
             py::arg("Index"), py::arg("P"), py::arg("Weight") = 1.0)
        .def("insert_pole_before", py::overload_cast<const int, const gp_Pnt2d&, const double>(&Geom2d_BezierCurve::InsertPoleBefore),
             py::arg("Index"), py::arg("P"), py::arg("Weight") = 1.0)
        .def("remove_pole", &Geom2d_BezierCurve::RemovePole, py::arg("Index"))
        .def("segment", &Geom2d_BezierCurve::Segment, py::arg("U1"), py::arg("U2"))
        
        // Pole and weight modification
        .def("set_pole", py::overload_cast<const int, const gp_Pnt2d&>(&Geom2d_BezierCurve::SetPole),
             py::arg("Index"), py::arg("P"))
        .def("set_pole", py::overload_cast<const int, const gp_Pnt2d&, const double>(&Geom2d_BezierCurve::SetPole),
             py::arg("Index"), py::arg("P"), py::arg("Weight"))
        .def("set_weight", &Geom2d_BezierCurve::SetWeight, py::arg("Index"), py::arg("Weight"))
        
        // Properties
        .def_property_readonly("degree", &Geom2d_BezierCurve::Degree)
        .def_property_readonly("is_rational", &Geom2d_BezierCurve::IsRational)
        .def_property_readonly("nb_poles", &Geom2d_BezierCurve::NbPoles)
        .def_static("max_degree", &Geom2d_BezierCurve::MaxDegree,
                    "Returns the maximum degree a Bezier curve can have")
        
        // Access poles and weights
        .def("pole", &Geom2d_BezierCurve::Pole, py::arg("Index"))
        .def("weight", &Geom2d_BezierCurve::Weight, py::arg("Index"))
        .def_property_readonly("poles", [](const Geom2d_BezierCurve& self) {
            return pnt_array_to_numpy(self.Poles());
        }, "Get poles as numpy array of shape (n,2)")
        .def_property_readonly("weights", [](const Geom2d_BezierCurve& self) {
            const TColStd_Array1OfReal* weights_ptr = self.Weights();
            if (weights_ptr) {
                return occt_array_to_numpy(*weights_ptr);
            }
            return py::array_t<double>();
        }, "Get weights as numpy array")
        
        // Resolution
        .def("resolution", [](Geom2d_BezierCurve& self, double ToleranceUV) {
               double UTolerance;
               self.Resolution(ToleranceUV, UTolerance);
               return UTolerance;
        }, py::arg("ToleranceUV"),
             "Computes the parametric resolution corresponding to a 2D resolution")
    ;
}