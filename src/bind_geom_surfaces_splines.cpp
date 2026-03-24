#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <Standard_Handle.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>
#include <Geom_Surface.hxx>
#include <Geom_BoundedSurface.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_BezierSurface.hxx>
#include <Geom_RectangularTrimmedSurface.hxx>
#include <GeomAbs_BSplKnotDistribution.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array2OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <TColStd_Array2OfInteger.hxx>

#include "array_utils.hpp"

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

// Forward declarations
extern void bind_geom_surfaces_splines(py::module_ &m);
extern void bind_geom_surfaces_elementary(py::module_ &m);
#if HAS_GBS
    #include <gbs/bssurf.h>
    // Helper functions to convert gbs surfaces to OCCT
    extern opencascade::handle<Geom_BSplineSurface> gbs_bssurface_to_occt(const gbs::BSSurface<double,3>& surf);
    extern opencascade::handle<Geom_BSplineSurface> gbs_bssurface_rational_to_occt(const gbs::BSSurfaceRational<double,3>& surf);
#endif
void bind_geom_surfaces_splines(py::module_ &m)
{
    // ========== Geom_BezierSurface ==========
    py::class_<Geom_BezierSurface, opencascade::handle<Geom_BezierSurface>, Geom_BoundedSurface>(m, "BezierSurface",
        R"doc(
        A rational or non-rational Bezier surface.

        A non-rational Bezier surface is defined by a table of poles (control points).
        A rational Bezier surface is defined by poles with varying associated weights.

        Key characteristics:
        - Parameter range is [0, 1] in both U and V directions
        - Degree = number of poles - 1 in each direction (max 25)
        - Infinite continuity (C∞) in both parametric directions
        - Corner poles are the four parametric corners of the surface
        - First/last rows and columns define boundary Bezier curves

        The poles form a rectangular network that shapes the surface. If all weights
        are equal, the surface is polynomial (non-rational).
        )doc")

        // --- Constructors ---
        .def(py::init([](py::array_t<double> poles) {
            auto arr = py_array_to_pnt_array2(poles);
            return new Geom_BezierSurface(arr);
        }), py::arg("poles"),
            R"doc(
            Create a non-rational Bezier surface from a 2D array of poles.

            Args:
                poles: 2D array of 3D points with shape [nu, nv, 3].
                       nu and nv must be >= 2 and <= MaxDegree + 1.

            The surface degrees will be (nu-1) in U and (nv-1) in V.
            )doc")

        .def(py::init([](py::array_t<double> poles, py::array_t<double> weights) {
            auto parr = py_array_to_pnt_array2(poles);
            auto warr = py_array_to_real_array2(weights);
            
            if (parr.ColLength() != warr.ColLength() || parr.RowLength() != warr.RowLength()) {
                throw py::value_error("Weights shape must match poles shape");
            }
            return new Geom_BezierSurface(parr, warr);
        }), py::arg("poles"), py::arg("weights"),
            R"doc(
            Create a rational Bezier surface from poles and weights.

            Args:
                poles: 2D array of 3D points with shape [nu, nv, 3].
                weights: 2D array of weights with shape [nu, nv].
                         All weights must be positive.

            If all weights are identical, the surface is treated as non-rational.
            )doc")

        // --- Properties ---
        .def_property_readonly("u_degree", &Geom_BezierSurface::UDegree,
            "The degree in the U parametric direction.")

        .def_property_readonly("v_degree", &Geom_BezierSurface::VDegree,
            "The degree in the V parametric direction.")

        .def_property_readonly("nb_u_poles", &Geom_BezierSurface::NbUPoles,
            "The number of poles in the U direction.")

        .def_property_readonly("nb_v_poles", &Geom_BezierSurface::NbVPoles,
            "The number of poles in the V direction.")

        .def_property_readonly("is_u_rational", &Geom_BezierSurface::IsURational,
            "True if the surface is rational in the U direction.")

        .def_property_readonly("is_v_rational", &Geom_BezierSurface::IsVRational,
            "True if the surface is rational in the V direction.")

        .def_property_readonly_static("max_degree", [](py::object) {
            return Geom_BezierSurface::MaxDegree();
        }, "The maximum allowed degree (25).")

        // --- Pole access ---
        .def("pole", &Geom_BezierSurface::Pole, py::arg("u_index"), py::arg("v_index"),
            R"doc(
            Return the pole at indices (u_index, v_index).

            Args:
                u_index: 1-based index in U direction (1 to nb_u_poles).
                v_index: 1-based index in V direction (1 to nb_v_poles).

            Returns:
                gp.Pnt: The control point.
            )doc")

        .def("poles", [](const Geom_BezierSurface& self) {
            TColgp_Array2OfPnt arr(1, self.NbUPoles(), 1, self.NbVPoles());
            self.Poles(arr);
            return pnt_array2_to_numpy(arr);
        }, "Return all poles as a numpy array of shape [nb_u_poles, nb_v_poles, 3].")

        .def("weight", &Geom_BezierSurface::Weight, py::arg("u_index"), py::arg("v_index"),
            R"doc(
            Return the weight at indices (u_index, v_index).

            Args:
                u_index: 1-based index in U direction.
                v_index: 1-based index in V direction.

            Returns:
                float: The weight value (1.0 for non-rational surfaces).
            )doc")

        .def("weights", [](const Geom_BezierSurface& self) {
            int nu = self.NbUPoles();
            int nv = self.NbVPoles();
            TColStd_Array2OfReal arr(1, nu, 1, nv);
            self.Weights(arr);
            return real_array2_to_numpy(arr);
        }, "Return all weights as a numpy array of shape [nb_u_poles, nb_v_poles].")

        // --- Modification ---
        .def("set_pole", py::overload_cast<const Standard_Integer, const Standard_Integer, const gp_Pnt&>(
            &Geom_BezierSurface::SetPole), py::arg("u_index"), py::arg("v_index"), py::arg("p"),
            "Set the pole at (u_index, v_index) to point p.")

        .def("set_pole", py::overload_cast<const Standard_Integer, const Standard_Integer, const gp_Pnt&, const Standard_Real>(
            &Geom_BezierSurface::SetPole), py::arg("u_index"), py::arg("v_index"), py::arg("p"), py::arg("weight"),
            "Set the pole and weight at (u_index, v_index).")

        .def("set_weight", &Geom_BezierSurface::SetWeight, py::arg("u_index"), py::arg("v_index"), py::arg("weight"),
            "Set the weight at (u_index, v_index).")

        .def("increase", &Geom_BezierSurface::Increase, py::arg("u_degree"), py::arg("v_degree"),
            R"doc(
            Increase the degree to (u_degree, v_degree).

            The new degrees must be >= current degrees and <= max_degree.
            The surface shape is preserved.
            )doc")

        .def("segment", &Geom_BezierSurface::Segment,
            py::arg("u1"), py::arg("u2"), py::arg("v1"), py::arg("v2"),
            R"doc(
            Segment the surface to the parameter range [u1, u2] x [v1, v2].

            The poles are modified so the new parameter range becomes [0, 1] x [0, 1].
            )doc")

        .def("exchange_uv", &Geom_BezierSurface::ExchangeUV,
            "Exchange U and V parametric directions (transposes poles/weights).")
        ;

    // ========== Geom_BSplineSurface ==========
    py::class_<Geom_BSplineSurface, opencascade::handle<Geom_BSplineSurface>, Geom_BoundedSurface>(m, "BSplineSurface",
        R"doc(
        A B-spline surface (rational or non-rational).

        In each parametric direction, a B-spline surface can be:
        - Uniform or non-uniform
        - Rational or non-rational
        - Periodic or non-periodic

        Defined by:
        - Degrees in U and V (max 25)
        - Table of poles (control points)
        - Table of weights (for rational surfaces)
        - Knot vectors with multiplicities in U and V

        The surface is infinitely differentiable except at knots, where
        continuity = degree - multiplicity.
        )doc")

        // --- Constructors ---
        .def(py::init([](py::array_t<double> poles, 
                         const py::handle& u_knots_container, const py::handle& v_knots_container,
                         const py::handle& u_mults_container, const py::handle& v_mults_container,
                         int u_degree, int v_degree, 
                         bool u_periodic, bool v_periodic) {
            auto parr = py_array_to_pnt_array2(poles);
            auto uknots = py_container_to_occt_array<Standard_Real, TColStd_Array1OfReal>(u_knots_container);
            auto vknots = py_container_to_occt_array<Standard_Real, TColStd_Array1OfReal>(v_knots_container);
            auto umults = py_container_to_occt_array<Standard_Integer, TColStd_Array1OfInteger>(u_mults_container);
            auto vmults = py_container_to_occt_array<Standard_Integer, TColStd_Array1OfInteger>(v_mults_container);
            
            return opencascade::handle<Geom_BSplineSurface>(
                new Geom_BSplineSurface(parr, uknots, vknots, umults, vmults, 
                                        u_degree, v_degree, u_periodic, v_periodic));
        }), py::arg("poles"), py::arg("u_knots"), py::arg("v_knots"), 
            py::arg("u_multiplicities"), py::arg("v_multiplicities"),
            py::arg("u_degree"), py::arg("v_degree"),
            py::arg("u_periodic") = false, py::arg("v_periodic") = false,
            R"doc(
            Create a non-rational B-spline surface.

            Args:
                poles: 2D array of 3D points with shape [nu, nv, 3].
                u_knots, v_knots: Knot vectors (list or 1D numpy array).
                u_multiplicities, v_multiplicities: Multiplicities (list or 1D numpy array).
                u_degree, v_degree: Degrees in U and V directions.
                u_periodic, v_periodic: Whether the surface is periodic in U/V.
            )doc")

        .def(py::init([](py::array_t<double> poles, py::array_t<double> weights,
                         const py::handle& u_knots_container, const py::handle& v_knots_container,
                         const py::handle& u_mults_container, const py::handle& v_mults_container,
                         int u_degree, int v_degree, 
                         bool u_periodic, bool v_periodic) {
            auto parr = py_array_to_pnt_array2(poles);
            auto warr = py_array_to_real_array2(weights);
            auto uknots = py_container_to_occt_array<Standard_Real, TColStd_Array1OfReal>(u_knots_container);
            auto vknots = py_container_to_occt_array<Standard_Real, TColStd_Array1OfReal>(v_knots_container);
            auto umults = py_container_to_occt_array<Standard_Integer, TColStd_Array1OfInteger>(u_mults_container);
            auto vmults = py_container_to_occt_array<Standard_Integer, TColStd_Array1OfInteger>(v_mults_container);
            
            if (parr.ColLength() != warr.ColLength() || parr.RowLength() != warr.RowLength()) {
                throw py::value_error("Weights shape must match poles shape");
            }
            
            return opencascade::handle<Geom_BSplineSurface>(
                new Geom_BSplineSurface(parr, warr, uknots, vknots, umults, vmults, 
                                        u_degree, v_degree, u_periodic, v_periodic));
        }), py::arg("poles"), py::arg("weights"), py::arg("u_knots"), py::arg("v_knots"), 
            py::arg("u_multiplicities"), py::arg("v_multiplicities"),
            py::arg("u_degree"), py::arg("v_degree"),
            py::arg("u_periodic") = false, py::arg("v_periodic") = false,
            R"doc(
            Create a rational B-spline surface.

            Args:
                poles: 2D array of 3D points with shape [nu, nv, 3].
                weights: 2D array of weights with shape [nu, nv].
                u_knots, v_knots: Knot vectors (list or 1D numpy array).
                u_multiplicities, v_multiplicities: Multiplicities (list or 1D numpy array).
                u_degree, v_degree: Degrees in U and V directions.
                u_periodic, v_periodic: Whether the surface is periodic in U/V.
            )doc")
#if HAS_GBS
        .def(py::init([](const gbs::BSSurface<double,3>& surf) {
            return gbs_bssurface_to_occt(surf);
        }), py::arg("bssurface"),
            "Create a B-spline surface from a gbs::BSSurface object.")

        .def(py::init([](const gbs::BSSurfaceRational<double,3>& surf) {
            return gbs_bssurface_rational_to_occt(surf);
        }), py::arg("bssurface_rational"),
            "Create a rational B-spline surface from a gbs::BSSurfaceRational object.")
#endif
        // --- Properties ---
        .def_property_readonly("u_degree", &Geom_BSplineSurface::UDegree,
            "The degree in the U parametric direction.")

        .def_property_readonly("v_degree", &Geom_BSplineSurface::VDegree,
            "The degree in the V parametric direction.")

        .def_property_readonly("nb_u_poles", &Geom_BSplineSurface::NbUPoles,
            "The number of poles in the U direction.")

        .def_property_readonly("nb_v_poles", &Geom_BSplineSurface::NbVPoles,
            "The number of poles in the V direction.")

        .def_property_readonly("nb_u_knots", &Geom_BSplineSurface::NbUKnots,
            "The number of distinct knots in the U direction.")

        .def_property_readonly("nb_v_knots", &Geom_BSplineSurface::NbVKnots,
            "The number of distinct knots in the V direction.")

        .def_property_readonly("is_u_rational", &Geom_BSplineSurface::IsURational,
            "True if the surface is rational in the U direction.")

        .def_property_readonly("is_v_rational", &Geom_BSplineSurface::IsVRational,
            "True if the surface is rational in the V direction.")

        .def_property_readonly("first_u_knot_index", &Geom_BSplineSurface::FirstUKnotIndex,
            "Index of the first U knot (start of parameter range).")

        .def_property_readonly("last_u_knot_index", &Geom_BSplineSurface::LastUKnotIndex,
            "Index of the last U knot (end of parameter range).")

        .def_property_readonly("first_v_knot_index", &Geom_BSplineSurface::FirstVKnotIndex,
            "Index of the first V knot (start of parameter range).")

        .def_property_readonly("last_v_knot_index", &Geom_BSplineSurface::LastVKnotIndex,
            "Index of the last V knot (end of parameter range).")

        .def_property_readonly("u_knot_distribution", &Geom_BSplineSurface::UKnotDistribution,
            "Knot distribution type in U: Uniform, QuasiUniform, PiecewiseBezier, or NonUniform.")

        .def_property_readonly("v_knot_distribution", &Geom_BSplineSurface::VKnotDistribution,
            "Knot distribution type in V: Uniform, QuasiUniform, PiecewiseBezier, or NonUniform.")

        .def_property_readonly_static("max_degree", [](py::object) {
            return Geom_BSplineSurface::MaxDegree();
        }, "The maximum allowed degree (25).")

        // --- Pole access ---
        .def("pole", &Geom_BSplineSurface::Pole, py::arg("u_index"), py::arg("v_index"),
            "Return the pole at (u_index, v_index). Indices are 1-based.")

        .def("poles", [](const Geom_BSplineSurface& self) {
            return pnt_array2_to_numpy(self.Poles());
        }, "Return all poles as a numpy array of shape [nb_u_poles, nb_v_poles, 3].")

        .def("weight", &Geom_BSplineSurface::Weight, py::arg("u_index"), py::arg("v_index"),
            "Return the weight at (u_index, v_index). Indices are 1-based.")

        .def("weights", [](const Geom_BSplineSurface& self) {
            const TColStd_Array2OfReal* weights = self.Weights();
            if (weights) {
                return real_array2_to_numpy(*weights);
            }
            // Return array of 1.0 for non-rational surfaces
            int nu = self.NbUPoles();
            int nv = self.NbVPoles();
            py::array_t<double> result({nu, nv});
            auto buf = result.mutable_unchecked<2>();
            for (int i = 0; i < nu; ++i) {
                for (int j = 0; j < nv; ++j) {
                    buf(i, j) = 1.0;
                }
            }
            return result;
        }, "Return all weights as a numpy array of shape [nb_u_poles, nb_v_poles].")

        // --- Knot access ---
        .def("u_knot", &Geom_BSplineSurface::UKnot, py::arg("index"),
            "Return the U knot at index (1-based).")

        .def("v_knot", &Geom_BSplineSurface::VKnot, py::arg("index"),
            "Return the V knot at index (1-based).")

        .def("u_knots", [](const Geom_BSplineSurface& self) {
            return occt_array_to_numpy(self.UKnots());
        }, "Return all distinct U knots as a numpy array.")

        .def("v_knots", [](const Geom_BSplineSurface& self) {
            return occt_array_to_numpy(self.VKnots());
        }, "Return all distinct V knots as a numpy array.")

        .def("u_multiplicity", &Geom_BSplineSurface::UMultiplicity, py::arg("index"),
            "Return the multiplicity of U knot at index (1-based).")

        .def("v_multiplicity", &Geom_BSplineSurface::VMultiplicity, py::arg("index"),
            "Return the multiplicity of V knot at index (1-based).")

        .def("u_multiplicities", [](const Geom_BSplineSurface& self) {
            return occt_array_to_numpy(self.UMultiplicities());
        }, "Return all U knot multiplicities as a numpy array.")

        .def("v_multiplicities", [](const Geom_BSplineSurface& self) {
            return occt_array_to_numpy(self.VMultiplicities());
        }, "Return all V knot multiplicities as a numpy array.")

        .def("u_knot_sequence", [](const Geom_BSplineSurface& self) {
            return occt_array_to_numpy(self.UKnotSequence());
        }, "Return U knots with repetitions as a numpy array.")

        .def("v_knot_sequence", [](const Geom_BSplineSurface& self) {
            return occt_array_to_numpy(self.VKnotSequence());
        }, "Return V knots with repetitions as a numpy array.")

        // --- Modification ---
        .def("set_pole", py::overload_cast<const Standard_Integer, const Standard_Integer, const gp_Pnt&>(
            &Geom_BSplineSurface::SetPole), py::arg("u_index"), py::arg("v_index"), py::arg("p"),
            "Set the pole at (u_index, v_index) to point p.")

        .def("set_pole", py::overload_cast<const Standard_Integer, const Standard_Integer, const gp_Pnt&, const Standard_Real>(
            &Geom_BSplineSurface::SetPole), py::arg("u_index"), py::arg("v_index"), py::arg("p"), py::arg("weight"),
            "Set the pole and weight at (u_index, v_index).")

        .def("set_weight", &Geom_BSplineSurface::SetWeight, py::arg("u_index"), py::arg("v_index"), py::arg("weight"),
            "Set the weight at (u_index, v_index).")

        .def("increase_degree", &Geom_BSplineSurface::IncreaseDegree, py::arg("u_degree"), py::arg("v_degree"),
            "Increase the degrees to (u_degree, v_degree). Preserves surface shape.")

        .def("segment", &Geom_BSplineSurface::Segment,
            py::arg("u1"), py::arg("u2"), py::arg("v1"), py::arg("v2"),
            py::arg("u_tolerance") = 1e-7, py::arg("v_tolerance") = 1e-7,
            "Segment the surface to the parameter range [u1, u2] x [v1, v2].")

        .def("exchange_uv", &Geom_BSplineSurface::ExchangeUV,
            "Exchange U and V parametric directions.")

        .def("set_u_periodic", &Geom_BSplineSurface::SetUPeriodic,
            "Make the surface periodic in U (must be closed first).")

        .def("set_v_periodic", &Geom_BSplineSurface::SetVPeriodic,
            "Make the surface periodic in V (must be closed first).")

        .def("set_u_not_periodic", &Geom_BSplineSurface::SetUNotPeriodic,
            "Make the surface non-periodic in U.")

        .def("set_v_not_periodic", &Geom_BSplineSurface::SetVNotPeriodic,
            "Make the surface non-periodic in V.")

        .def("resolution", [](Geom_BSplineSurface& self, double tol3d) { // Should use const
            Standard_Real utol, vtol;
            self.Resolution(tol3d, utol, vtol);
            return py::make_tuple(utol, vtol);
        }, py::arg("tolerance_3d"),
            R"doc(
            Compute parametric tolerances from a 3D tolerance.

            Returns (u_tolerance, v_tolerance) such that if |du| < u_tolerance
            and |dv| < v_tolerance, then |S(u+du,v+dv) - S(u,v)| < tolerance_3d.
            )doc")

        .def("locate_u", [](const Geom_BSplineSurface& self, double u, double tol) {
            Standard_Integer i1, i2;
            self.LocateU(u, tol, i1, i2);
            return py::make_tuple(i1, i2);
        }, py::arg("u"), py::arg("tolerance"),
            "Locate U in the knot sequence. Returns (i1, i2) where UKnot(i1) <= U <= UKnot(i2).")

        .def("locate_v", [](const Geom_BSplineSurface& self, double v, double tol) {
            Standard_Integer i1, i2;
            self.LocateV(v, tol, i1, i2);
            return py::make_tuple(i1, i2);
        }, py::arg("v"), py::arg("tolerance"),
            "Locate V in the knot sequence. Returns (i1, i2) where VKnot(i1) <= V <= VKnot(i2).")
        ;
#if HAS_GBS
    // Register implicit conversions from gbs types to Geom_BSplineSurface
    py::implicitly_convertible<gbs::BSSurface<double,3>, Geom_BSplineSurface>();
    py::implicitly_convertible<gbs::BSSurfaceRational<double,3>, Geom_BSplineSurface>();
#endif
    // ========== Geom_RectangularTrimmedSurface ==========
    py::class_<Geom_RectangularTrimmedSurface, opencascade::handle<Geom_RectangularTrimmedSurface>, Geom_BoundedSurface>(
        m, "RectangularTrimmedSurface",
        R"doc(
        A surface trimmed by isoparametric curves.

        Describes a portion of a surface limited by parameter ranges:
        - [u1, u2] in the U direction
        - [v1, v2] in the V direction

        The trimmed surface is built from a copy of the basis surface, so
        modifications to the basis surface don't affect the trimmed surface.

        The trimmed surface may not have the same orientation as the basis surface.
        )doc")

        // --- Constructors ---
        .def(py::init<const opencascade::handle<Geom_Surface>&, Standard_Real, Standard_Real,
                      Standard_Real, Standard_Real, Standard_Boolean, Standard_Boolean>(),
            py::arg("surface"), py::arg("u1"), py::arg("u2"), py::arg("v1"), py::arg("v2"),
            py::arg("u_sense") = true, py::arg("v_sense") = true,
            R"doc(
            Create a rectangular trimmed surface.

            Args:
                surface: The basis surface to trim.
                u1, u2: Parameter range in U direction.
                v1, v2: Parameter range in V direction.
                u_sense: If True, U direction goes from u1 to u2.
                v_sense: If True, V direction goes from v1 to v2.

            For periodic surfaces, u_sense and v_sense control which part is used.
            )doc")

        .def(py::init<const opencascade::handle<Geom_Surface>&, Standard_Real, Standard_Real,
                      Standard_Boolean, Standard_Boolean>(),
            py::arg("surface"), py::arg("param1"), py::arg("param2"),
            py::arg("u_trim"), py::arg("sense") = true,
            R"doc(
            Create a surface trimmed in one direction only.

            Args:
                surface: The basis surface to trim.
                param1, param2: Parameter range for trimming.
                u_trim: If True, trim in U direction; else trim in V.
                sense: Direction sense for periodic surfaces.
            )doc")

        // --- Properties ---
        .def_property_readonly("basis_surface", &Geom_RectangularTrimmedSurface::BasisSurface,
            "The underlying basis surface.")

        // --- Methods ---
        .def("set_trim", py::overload_cast<const Standard_Real, const Standard_Real, const Standard_Real,
                                           const Standard_Real, const Standard_Boolean, const Standard_Boolean>(
            &Geom_RectangularTrimmedSurface::SetTrim),
            py::arg("u1"), py::arg("u2"), py::arg("v1"), py::arg("v2"),
            py::arg("u_sense") = true, py::arg("v_sense") = true,
            "Modify the trim values in both directions.")

        .def("set_trim", py::overload_cast<const Standard_Real, const Standard_Real,
                                           const Standard_Boolean, const Standard_Boolean>(
            &Geom_RectangularTrimmedSurface::SetTrim),
            py::arg("param1"), py::arg("param2"), py::arg("u_trim"), py::arg("sense") = true,
            "Modify the trim values in one direction only.")
        ;

}
