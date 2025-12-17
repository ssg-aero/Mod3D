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
#include <Geom_BSplineCurve.hxx>
#include <Geom_BezierCurve.hxx>

#include "array_utils.hpp"

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_geom_curves_splines(py::module_ &m)
{
    py::class_<Geom_BSplineCurve, opencascade::handle<Geom_BSplineCurve>, Geom_BoundedCurve>(m, "BSplineCurve")
        // Constructors - need to handle complex array construction
        .def(py::init([](const py::handle& poles_container, const py::handle& knots_container, 
                         const py::handle& mults_container, int degree, bool periodic) {
            // Convert containers to OCCT arrays
            auto poles = py_container_to_pnt_array(poles_container);
            auto knots = py_container_to_occt_array<Standard_Real, TColStd_Array1OfReal>(knots_container);
            auto mults = py_container_to_occt_array<Standard_Integer, TColStd_Array1OfInteger>(mults_container);
            
            if (knots.Length() != mults.Length()) {
                throw py::value_error("Knots and multiplicities arrays must have the same length");
            }
            
            return opencascade::handle<Geom_BSplineCurve>(
                new Geom_BSplineCurve(poles, knots, mults, degree, periodic));
        }), py::arg("poles"), py::arg("knots"), py::arg("multiplicities"), 
            py::arg("degree"), py::arg("periodic") = false,
            "Create a non-rational B-spline curve. Poles can be list of gp_Pnt or numpy array of shape (n,3). Knots and multiplicities can be lists or 1D numpy arrays.")
        
        .def(py::init([](const py::handle& poles_container, const py::handle& weights_container,
                         const py::handle& knots_container, const py::handle& mults_container, 
                         int degree, bool periodic, bool check_rational) {
            // Convert containers to OCCT arrays
            auto poles = py_container_to_pnt_array(poles_container);
            auto weights = py_container_to_occt_array<Standard_Real, TColStd_Array1OfReal>(weights_container);
            auto knots = py_container_to_occt_array<Standard_Real, TColStd_Array1OfReal>(knots_container);
            auto mults = py_container_to_occt_array<Standard_Integer, TColStd_Array1OfInteger>(mults_container);
            
            if (poles.Length() != weights.Length()) {
                throw py::value_error("Poles and weights arrays must have the same length");
            }
            if (knots.Length() != mults.Length()) {
                throw py::value_error("Knots and multiplicities arrays must have the same length");
            }
            
            return opencascade::handle<Geom_BSplineCurve>(
                new Geom_BSplineCurve(poles, weights, knots, mults, degree, periodic, check_rational));
        }), py::arg("poles"), py::arg("weights"), py::arg("knots"), py::arg("multiplicities"), 
            py::arg("degree"), py::arg("periodic") = false, py::arg("check_rational") = true,
            "Create a rational B-spline curve. All arrays can be lists or numpy arrays.")
        
        // Modification methods
        .def("increase_degree", &Geom_BSplineCurve::IncreaseDegree, py::arg("degree"),
             "Increases the degree of this BSpline curve to Degree")
        .def("increase_multiplicity", 
             py::overload_cast<const Standard_Integer, const Standard_Integer>(&Geom_BSplineCurve::IncreaseMultiplicity),
             py::arg("index"), py::arg("M"))
        .def("increase_multiplicity", 
             py::overload_cast<const Standard_Integer, const Standard_Integer, const Standard_Integer>(&Geom_BSplineCurve::IncreaseMultiplicity),
             py::arg("I1"), py::arg("I2"), py::arg("M"))
        .def("increment_multiplicity", &Geom_BSplineCurve::IncrementMultiplicity,
             py::arg("I1"), py::arg("I2"), py::arg("M"))
        
        .def("insert_knot", &Geom_BSplineCurve::InsertKnot,
             py::arg("U"), py::arg("M") = 1, py::arg("ParametricTolerance") = 0.0, py::arg("Add") = true)
        .def("remove_knot", &Geom_BSplineCurve::RemoveKnot,
             py::arg("Index"), py::arg("M"), py::arg("Tolerance"))
        
        .def("segment", &Geom_BSplineCurve::Segment,
             py::arg("U1"), py::arg("U2"), py::arg("theTolerance") = 1e-12)
        
        // Knot and pole modification
        .def("set_knot", py::overload_cast<const Standard_Integer, const Standard_Real>(&Geom_BSplineCurve::SetKnot),
             py::arg("Index"), py::arg("K"))
        .def("set_knot", py::overload_cast<const Standard_Integer, const Standard_Real, const Standard_Integer>(&Geom_BSplineCurve::SetKnot),
             py::arg("Index"), py::arg("K"), py::arg("M"))
        .def("set_pole", py::overload_cast<const Standard_Integer, const gp_Pnt&>(&Geom_BSplineCurve::SetPole),
             py::arg("Index"), py::arg("P"))
        .def("set_pole", py::overload_cast<const Standard_Integer, const gp_Pnt&, const Standard_Real>(&Geom_BSplineCurve::SetPole),
             py::arg("Index"), py::arg("P"), py::arg("Weight"))
        .def("set_weight", &Geom_BSplineCurve::SetWeight, py::arg("Index"), py::arg("Weight"))
        
        // Periodic operations
        .def("periodic_normalization", &Geom_BSplineCurve::PeriodicNormalization, py::arg("U"))
        .def("set_periodic", &Geom_BSplineCurve::SetPeriodic)
        .def("set_not_periodic", &Geom_BSplineCurve::SetNotPeriodic)
        .def("set_origin", py::overload_cast<const Standard_Integer>(&Geom_BSplineCurve::SetOrigin), py::arg("Index"))
        .def("set_origin", py::overload_cast<const Standard_Real, const Standard_Real>(&Geom_BSplineCurve::SetOrigin), 
             py::arg("U"), py::arg("Tol"))
        
        // Point modification
        .def("move_point", [](Geom_BSplineCurve& self, Standard_Real U, const gp_Pnt& P, 
                              Standard_Integer Index1, Standard_Integer Index2) {
            Standard_Integer FirstModifiedPole, LastModifiedPole;
            self.MovePoint(U, P, Index1, Index2, FirstModifiedPole, LastModifiedPole);
            return py::make_tuple(FirstModifiedPole, LastModifiedPole);
        }, py::arg("U"), py::arg("P"), py::arg("Index1"), py::arg("Index2"))
        
        .def("move_point_and_tangent", [](Geom_BSplineCurve& self, Standard_Real U, const gp_Pnt& P, 
                                          const gp_Vec& Tangent, Standard_Real Tolerance, 
                                          Standard_Integer StartingCondition, Standard_Integer EndingCondition) {
            Standard_Integer ErrorStatus;
            self.MovePointAndTangent(U, P, Tangent, Tolerance, StartingCondition, EndingCondition, ErrorStatus);
            return ErrorStatus;
        }, py::arg("U"), py::arg("P"), py::arg("Tangent"), py::arg("Tolerance"), 
           py::arg("StartingCondition"), py::arg("EndingCondition"))
        
        // Properties
        .def("degree", &Geom_BSplineCurve::Degree)
        .def("is_rational", &Geom_BSplineCurve::IsRational)
        .def("is_periodic", &Geom_BSplineCurve::IsPeriodic)
        .def("is_closed", &Geom_BSplineCurve::IsClosed)
        .def("continuity", &Geom_BSplineCurve::Continuity)
        .def("is_g1", &Geom_BSplineCurve::IsG1, py::arg("theTf"), py::arg("theTl"), py::arg("theAngTol"))
        
        // Knot information
        .def("nb_knots", &Geom_BSplineCurve::NbKnots)
        .def("nb_poles", &Geom_BSplineCurve::NbPoles)
        .def("first_u_knot_index", &Geom_BSplineCurve::FirstUKnotIndex)
        .def("last_u_knot_index", &Geom_BSplineCurve::LastUKnotIndex)
        .def("knot", &Geom_BSplineCurve::Knot, py::arg("Index"))
        .def("multiplicity", &Geom_BSplineCurve::Multiplicity, py::arg("Index"))
        
        // Access to poles and weights
        .def("pole", &Geom_BSplineCurve::Pole, py::arg("Index"), py::return_value_policy::reference_internal)
        .def("weight", &Geom_BSplineCurve::Weight, py::arg("Index"))
        
        // Access arrays as numpy arrays by default
        .def("knots", [](const Geom_BSplineCurve& self) {
            return occt_array_to_numpy(self.Knots());
        }, "Get knots as numpy array")
        
        .def("multiplicities", [](const Geom_BSplineCurve& self) {
            return occt_array_to_numpy(self.Multiplicities());
        }, "Get multiplicities as numpy array")
        
        .def("poles", [](const Geom_BSplineCurve& self) {
            return pnt_array_to_numpy(self.Poles());
        }, "Get poles as numpy array of shape (n,3)")
        
        .def("weights", [](const Geom_BSplineCurve& self) {
            const TColStd_Array1OfReal* weights_ptr = self.Weights();
            if (weights_ptr) {
                return occt_array_to_numpy(*weights_ptr);
            }
            return py::array_t<double>();
        }, "Get weights as numpy array")
        
        // Knot sequence (with repetitions)
        .def("knot_sequence", [](const Geom_BSplineCurve& self) {
            return occt_array_to_numpy(self.KnotSequence());
        }, "Get knot sequence as numpy array")
        
        // Locate parameter
        .def("locate_u", [](const Geom_BSplineCurve& self, Standard_Real U, Standard_Real ParametricTolerance, bool WithKnotRepetition) {
            Standard_Integer I1, I2;
            self.LocateU(U, ParametricTolerance, I1, I2, WithKnotRepetition);
            return py::make_tuple(I1, I2);
        }, py::arg("U"), py::arg("ParametricTolerance"), py::arg("WithKnotRepetition") = false)
        
        // Local evaluation methods
        .def("local_value", &Geom_BSplineCurve::LocalValue, py::arg("U"), py::arg("FromK1"), py::arg("ToK2"))
        .def("local_d0", [](const Geom_BSplineCurve& self, Standard_Real U, Standard_Integer FromK1, Standard_Integer ToK2) {
            gp_Pnt P;
            self.LocalD0(U, FromK1, ToK2, P);
            return P;
        }, py::arg("U"), py::arg("FromK1"), py::arg("ToK2"))
        .def("local_d1", [](const Geom_BSplineCurve& self, Standard_Real U, Standard_Integer FromK1, Standard_Integer ToK2) {
            gp_Pnt P;
            gp_Vec V1;
            self.LocalD1(U, FromK1, ToK2, P, V1);
            return py::make_tuple(P, V1);
        }, py::arg("U"), py::arg("FromK1"), py::arg("ToK2"))
        .def("local_d2", [](const Geom_BSplineCurve& self, Standard_Real U, Standard_Integer FromK1, Standard_Integer ToK2) {
            gp_Pnt P;
            gp_Vec V1, V2;
            self.LocalD2(U, FromK1, ToK2, P, V1, V2);
            return py::make_tuple(P, V1, V2);
        }, py::arg("U"), py::arg("FromK1"), py::arg("ToK2"))
        .def("local_d3", [](const Geom_BSplineCurve& self, Standard_Real U, Standard_Integer FromK1, Standard_Integer ToK2) {
            gp_Pnt P;
            gp_Vec V1, V2, V3;
            self.LocalD3(U, FromK1, ToK2, P, V1, V2, V3);
            return py::make_tuple(P, V1, V2, V3);
        }, py::arg("U"), py::arg("FromK1"), py::arg("ToK2"))
        .def("local_dn", &Geom_BSplineCurve::LocalDN, py::arg("U"), py::arg("FromK1"), py::arg("ToK2"), py::arg("N"))
        
        // Utility methods
        .def("resolution", [](Geom_BSplineCurve& self, double Tolerance3D) {
            double UTolerance;
            self.Resolution(Tolerance3D, UTolerance);
            return UTolerance;
        }, py::arg("Tolerance3D"))
        
        // Static methods
        .def_static("max_degree", &Geom_BSplineCurve::MaxDegree)
        
        // Comparison
        .def("is_equal", &Geom_BSplineCurve::IsEqual, py::arg("other"), py::arg("precision"))
    ;

    py::class_<Geom_BezierCurve, opencascade::handle<Geom_BezierCurve>, Geom_BoundedCurve>(m, "BezierCurve")
        // Constructors
        .def(py::init([](const py::handle& poles_container) {
            auto poles = py_container_to_pnt_array(poles_container);
            return opencascade::handle<Geom_BezierCurve>(new Geom_BezierCurve(poles));
        }), py::arg("poles"), 
            "Create a non-rational Bezier curve with control points. Poles can be list of gp_Pnt or numpy array of shape (n,3).")
        
        .def(py::init([](const py::handle& poles_container, const py::handle& weights_container) {
            auto poles = py_container_to_pnt_array(poles_container);
            auto weights = py_container_to_occt_array<Standard_Real, TColStd_Array1OfReal>(weights_container);
            
            if (poles.Length() != weights.Length()) {
                throw py::value_error("Poles and weights arrays must have the same length");
            }
            
            return opencascade::handle<Geom_BezierCurve>(new Geom_BezierCurve(poles, weights));
        }), py::arg("poles"), py::arg("weights"),
            "Create a rational Bezier curve with control points and weights. Both can be lists or numpy arrays.")
        
        // Modification methods
        .def("increase", &Geom_BezierCurve::Increase, py::arg("degree"),
             "Increases the degree of the Bezier curve")
        
        .def("insert_pole_after", py::overload_cast<const Standard_Integer, const gp_Pnt&>(&Geom_BezierCurve::InsertPoleAfter),
             py::arg("index"), py::arg("P"), "Insert pole after given index")
        .def("insert_pole_after", py::overload_cast<const Standard_Integer, const gp_Pnt&, const Standard_Real>(&Geom_BezierCurve::InsertPoleAfter),
             py::arg("index"), py::arg("P"), py::arg("weight"), "Insert pole with weight after given index")
        
        .def("insert_pole_before", py::overload_cast<const Standard_Integer, const gp_Pnt&>(&Geom_BezierCurve::InsertPoleBefore),
             py::arg("index"), py::arg("P"), "Insert pole before given index")
        .def("insert_pole_before", py::overload_cast<const Standard_Integer, const gp_Pnt&, const Standard_Real>(&Geom_BezierCurve::InsertPoleBefore),
             py::arg("index"), py::arg("P"), py::arg("weight"), "Insert pole with weight before given index")
        
        .def("remove_pole", &Geom_BezierCurve::RemovePole, py::arg("index"),
             "Remove pole at given index")
        
        .def("set_pole", py::overload_cast<const Standard_Integer, const gp_Pnt&>(&Geom_BezierCurve::SetPole),
             py::arg("index"), py::arg("P"), "Set pole at given index")
        .def("set_pole", py::overload_cast<const Standard_Integer, const gp_Pnt&, const Standard_Real>(&Geom_BezierCurve::SetPole),
             py::arg("index"), py::arg("P"), py::arg("weight"), "Set pole and weight at given index")
        
        .def("set_weight", &Geom_BezierCurve::SetWeight, py::arg("index"), py::arg("weight"),
             "Set weight at given index")
        
        .def("segment", &Geom_BezierCurve::Segment, py::arg("U1"), py::arg("U2"),
             "Segment the curve between parameters U1 and U2")
        
        // Properties
        .def("degree", &Geom_BezierCurve::Degree, "Get the degree of the Bezier curve")
        .def("is_rational", &Geom_BezierCurve::IsRational, "Check if curve is rational")
        .def("is_closed", &Geom_BezierCurve::IsClosed, "Check if curve is closed")
        .def("is_periodic", &Geom_BezierCurve::IsPeriodic, "Check if curve is periodic")
        .def("continuity", &Geom_BezierCurve::Continuity, "Get continuity of the curve")
        .def("is_cn", &Geom_BezierCurve::IsCN, py::arg("N"), "Check if curve has CN continuity")
        
        // Pole and weight information
        .def("nb_poles", &Geom_BezierCurve::NbPoles, "Get number of poles")
        .def("pole", &Geom_BezierCurve::Pole, py::arg("index"), py::return_value_policy::reference_internal,
             "Get pole at given index (1-based)")
        .def("weight", &Geom_BezierCurve::Weight, py::arg("index"), 
             "Get weight at given index (1-based)")
        
        // Access arrays as numpy arrays by default
        .def("poles", [](const Geom_BezierCurve& self) {
            return pnt_array_to_numpy(self.Poles());
        }, "Get all poles as numpy array of shape (n,3)")
        
        .def("weights", [](const Geom_BezierCurve& self) {
            const TColStd_Array1OfReal* weights_ptr = self.Weights();
            if (weights_ptr) {
                return occt_array_to_numpy(*weights_ptr);
            }
            return py::array_t<double>();
        }, "Get all weights as numpy array")
        
        // Utility methods
        .def("resolution", [](Geom_BezierCurve& self, double Tolerance3D) {
            double UTolerance;
            self.Resolution(Tolerance3D, UTolerance);
            return UTolerance;
        }, py::arg("Tolerance3D"), "Compute parametric tolerance for given 3D tolerance")
        
        // Static methods
        .def_static("max_degree", &Geom_BezierCurve::MaxDegree, "Get maximum allowed degree")
    ;
}