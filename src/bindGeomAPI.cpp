#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec.hxx>
#include <gp_Vec2d.hxx>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <Standard_Handle.hxx>
#include <GeomAPI.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include <Geom2dAPI_Interpolate.hxx>
#include <Geom2dAPI_PointsToBSpline.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom2d_BSplineCurve.hxx>
#include <Geom_Surface.hxx>
#include <Geom2d_Curve.hxx>

#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <GeomAPI_ExtremaCurveSurface.hxx>
#include <GeomAPI_ExtremaSurfaceSurface.hxx>
#include <Geom2dAPI_ExtremaCurveCurve.hxx>

#include <GeomAPI_ProjectPointOnCurve.hxx>

#include <Geom2dAPI_InterCurveCurve.hxx>
#include <Geom2dAPI_ProjectPointOnCurve.hxx>

#include <GeomAbs_Shape.hxx>
#include <Approx_ParametrizationType.hxx>

#include "array_utils.hpp"


namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_geom_api(py::module_ &m)
{
    // =========================================================================
    // Free functions
    // =========================================================================
    m.def("to_2d",[](const opencascade::handle<Geom_Curve>& curve3d, const gp_Pln& plane) {
        return GeomAPI::To2d(curve3d, plane);
    }, py::arg("curve3d"), py::arg("plane"),
       "Projects a 3D curve onto a plane, returning a 2D curve");

    m.def("to_3d",[](const opencascade::handle<Geom2d_Curve>& curve2d, const gp_Pln& plane) {
        return GeomAPI::To3d(curve2d, plane);
    }, py::arg("curve2d"), py::arg("plane"),
       "Embeds a 2D curve into 3D space on the given plane");

    // =========================================================================
    // GeomAPI_Interpolate - 3D BSpline interpolation through points
    // =========================================================================
    py::class_<GeomAPI_Interpolate>(m, "Interpolate",
        R"doc(
        Interpolates a BSpline curve passing through an array of 3D points.

        The resulting curve will have:
        - C2 continuity if no tangency is requested
        - C1 continuity at points where tangency is specified
        - C1 continuity if periodic

        Example:
            points = [gp.Pnt(0,0,0), gp.Pnt(1,1,0), gp.Pnt(2,0,0)]
            interp = GeomAPI.Interpolate(points)
            interp.perform()
            if interp.is_done:
                curve = interp.curve
        )doc")
        
        .def(py::init([](const std::vector<gp_Pnt>& points, bool periodic, double tolerance) {
            return new GeomAPI_Interpolate(vector_to_HArray1OfPnt(points), periodic, tolerance);
        }), py::arg("points"), py::arg("periodic") = false, py::arg("tolerance") = 1.0e-6,
            "Creates interpolation from points list.\n"
            "periodic: if True, curve will be closed\n"
            "tolerance: used to check if points are too close")
        
        .def(py::init([](const std::vector<gp_Pnt>& points, const std::vector<double>& parameters, 
                        bool periodic, double tolerance) {
            return new GeomAPI_Interpolate(vector_to_HArray1OfPnt(points), 
                                           vector_to_HArray1OfReal(parameters), 
                                           periodic, tolerance);
        }), py::arg("points"), py::arg("parameters"), py::arg("periodic") = false, py::arg("tolerance") = 1.0e-6,
            "Creates interpolation from points with explicit parameters.\n"
            "parameters: parameter values for each point")

        .def("load", [](GeomAPI_Interpolate& self, const gp_Vec& initial_tangent, 
                       const gp_Vec& final_tangent, bool scale) {
            self.Load(initial_tangent, final_tangent, scale);
        }, py::arg("initial_tangent"), py::arg("final_tangent"), py::arg("scale") = true,
            "Sets tangent constraints at first and last points")
        
        .def("load_tangents", [](GeomAPI_Interpolate& self, const std::vector<gp_Vec>& tangents,
                                const std::vector<bool>& tangent_flags, bool scale) {
            self.Load(vector_to_Array1OfVec(tangents), vector_to_HArray1OfBoolean(tangent_flags), scale);
        }, py::arg("tangents"), py::arg("tangent_flags"), py::arg("scale") = true,
            "Sets tangent constraints at specified points.\n"
            "tangents: list of tangent vectors (parallel to points)\n"
            "tangent_flags: True where tangent should be applied")
        
        .def("perform", &GeomAPI_Interpolate::Perform,
            "Computes the interpolated BSpline curve")
        
        .def_property_readonly("is_done", &GeomAPI_Interpolate::IsDone,
            "True if interpolation was successful")
        
        .def_property_readonly("curve", &GeomAPI_Interpolate::Curve,
            "Returns the computed BSpline curve (raises if not done)")
    ;

    // =========================================================================
    // GeomAPI_PointsToBSpline - 3D BSpline approximation through points
    // =========================================================================
    py::class_<GeomAPI_PointsToBSpline>(m, "PointsToBSpline",
        R"doc(
        Approximates a BSpline curve passing through an array of 3D points.

        Unlike Interpolate, this creates an approximation that may not pass
        exactly through each point, but minimizes the distance while maintaining
        specified continuity and degree constraints.

        Example:
            points = [gp.Pnt(0,0,0), gp.Pnt(1,1,0), gp.Pnt(2,0,0)]
            approx = GeomAPI.PointsToBSpline(points, deg_min=3, deg_max=8)
            if approx.is_done:
                curve = approx.curve
        )doc")
        
        .def(py::init<>(), "Creates an empty approximation object. Use init() to define the curve.")
        
        .def(py::init([](const std::vector<gp_Pnt>& points, int deg_min, int deg_max,
                        GeomAbs_Shape continuity, double tol3d) {
            return new GeomAPI_PointsToBSpline(vector_to_Array1OfPnt(points), deg_min, deg_max, continuity, tol3d);
        }), py::arg("points"), py::arg("deg_min") = 3, py::arg("deg_max") = 8,
            py::arg("continuity") = GeomAbs_C2, py::arg("tol3d") = 1.0e-3,
            "Creates approximation from points with degree and continuity constraints")
        
        .def(py::init([](const std::vector<gp_Pnt>& points, Approx_ParametrizationType par_type,
                        int deg_min, int deg_max, GeomAbs_Shape continuity, double tol3d) {
            return new GeomAPI_PointsToBSpline(vector_to_Array1OfPnt(points), par_type, deg_min, deg_max, continuity, tol3d);
        }), py::arg("points"), py::arg("par_type"), py::arg("deg_min") = 3, py::arg("deg_max") = 8,
            py::arg("continuity") = GeomAbs_C2, py::arg("tol3d") = 1.0e-3,
            "Creates approximation with specified parametrization type")
        
        .def(py::init([](const std::vector<gp_Pnt>& points, const std::vector<double>& parameters,
                        int deg_min, int deg_max, GeomAbs_Shape continuity, double tol3d) {
            return new GeomAPI_PointsToBSpline(vector_to_Array1OfPnt(points), vector_to_Array1OfReal(parameters), 
                                               deg_min, deg_max, continuity, tol3d);
        }), py::arg("points"), py::arg("parameters"), py::arg("deg_min") = 3, py::arg("deg_max") = 8,
            py::arg("continuity") = GeomAbs_C2, py::arg("tol3d") = 1.0e-3,
            "Creates approximation with explicit parameter values")
        
        .def(py::init([](const std::vector<gp_Pnt>& points, double weight1, double weight2, double weight3,
                        int deg_max, GeomAbs_Shape continuity, double tol3d) {
            return new GeomAPI_PointsToBSpline(vector_to_Array1OfPnt(points), weight1, weight2, weight3, 
                                               deg_max, continuity, tol3d);
        }), py::arg("points"), py::arg("weight1"), py::arg("weight2"), py::arg("weight3"),
            py::arg("deg_max") = 8, py::arg("continuity") = GeomAbs_C2, py::arg("tol3d") = 1.0e-3,
            "Creates approximation using variational smoothing.\n"
            "Minimizes: weight1*Length + weight2*Curvature + weight3*Torsion")

        .def("init", [](GeomAPI_PointsToBSpline& self, const std::vector<gp_Pnt>& points,
                       int deg_min, int deg_max, GeomAbs_Shape continuity, double tol3d) {
            self.Init(vector_to_Array1OfPnt(points), deg_min, deg_max, continuity, tol3d);
        }, py::arg("points"), py::arg("deg_min") = 3, py::arg("deg_max") = 8,
            py::arg("continuity") = GeomAbs_C2, py::arg("tol3d") = 1.0e-3,
            "Initializes and computes approximation from points")
        
        .def_property_readonly("is_done", &GeomAPI_PointsToBSpline::IsDone,
            "True if approximation was successful")
        
        .def_property_readonly("curve", &GeomAPI_PointsToBSpline::Curve,
            "Returns the computed BSpline curve (raises if not done)")
    ;

    // =========================================================================
    // Geom2dAPI_Interpolate - 2D BSpline interpolation through points
    // =========================================================================
    py::class_<Geom2dAPI_Interpolate>(m, "Interpolate2d",
        R"doc(
        Interpolates a BSpline curve passing through an array of 2D points.

        The resulting curve will have:
        - C2 continuity if no tangency is requested
        - C1 continuity at points where tangency is specified
        - C1 continuity if periodic

        Example:
            points = [gp.Pnt2d(0,0), gp.Pnt2d(1,1), gp.Pnt2d(2,0)]
            interp = GeomAPI.Interpolate2d(points)
            interp.perform()
            if interp.is_done:
                curve = interp.curve
        )doc")
        
        .def(py::init([](const std::vector<gp_Pnt2d>& points, bool periodic, double tolerance) {
            return new Geom2dAPI_Interpolate(vector_to_HArray1OfPnt2d(points), periodic, tolerance);
        }), py::arg("points"), py::arg("periodic") = false, py::arg("tolerance") = 1.0e-6,
            "Creates interpolation from 2D points list.\n"
            "periodic: if True, curve will be closed\n"
            "tolerance: used to check if points are too close")
        
        .def(py::init([](const std::vector<gp_Pnt2d>& points, const std::vector<double>& parameters, 
                        bool periodic, double tolerance) {
            return new Geom2dAPI_Interpolate(vector_to_HArray1OfPnt2d(points), 
                                             vector_to_HArray1OfReal(parameters), 
                                             periodic, tolerance);
        }), py::arg("points"), py::arg("parameters"), py::arg("periodic") = false, py::arg("tolerance") = 1.0e-6,
            "Creates interpolation from 2D points with explicit parameters")

        .def("load", [](Geom2dAPI_Interpolate& self, const gp_Vec2d& initial_tangent, 
                       const gp_Vec2d& final_tangent, bool scale) {
            self.Load(initial_tangent, final_tangent, scale);
        }, py::arg("initial_tangent"), py::arg("final_tangent"), py::arg("scale") = true,
            "Sets tangent constraints at first and last points")
        
        .def("load_tangents", [](Geom2dAPI_Interpolate& self, const std::vector<gp_Vec2d>& tangents,
                                const std::vector<bool>& tangent_flags, bool scale) {
            self.Load(vector_to_Array1OfVec2d(tangents), vector_to_HArray1OfBoolean(tangent_flags), scale);
        }, py::arg("tangents"), py::arg("tangent_flags"), py::arg("scale") = true,
            "Sets tangent constraints at specified points")
        
        .def("perform", &Geom2dAPI_Interpolate::Perform,
            "Computes the interpolated BSpline curve")
        
        .def_property_readonly("is_done", &Geom2dAPI_Interpolate::IsDone,
            "True if interpolation was successful")
        
        .def_property_readonly("curve", &Geom2dAPI_Interpolate::Curve,
            "Returns the computed 2D BSpline curve (raises if not done)")
    ;

    // =========================================================================
    // Geom2dAPI_PointsToBSpline - 2D BSpline approximation through points
    // =========================================================================
    py::class_<Geom2dAPI_PointsToBSpline>(m, "PointsToBSpline2d",
        R"doc(
        Approximates a BSpline curve passing through an array of 2D points.

        Unlike Interpolate2d, this creates an approximation that may not pass
        exactly through each point, but minimizes the distance while maintaining
        specified continuity and degree constraints.

        Example:
            points = [gp.Pnt2d(0,0), gp.Pnt2d(1,1), gp.Pnt2d(2,0)]
            approx = GeomAPI.PointsToBSpline2d(points)
            if approx.is_done:
                curve = approx.curve
        )doc")
        
        .def(py::init<>(), "Creates an empty approximation object. Use init() to define the curve.")
        
        .def(py::init([](const std::vector<gp_Pnt2d>& points, int deg_min, int deg_max,
                        GeomAbs_Shape continuity, double tol2d) {
            return new Geom2dAPI_PointsToBSpline(vector_to_Array1OfPnt2d(points), deg_min, deg_max, continuity, tol2d);
        }), py::arg("points"), py::arg("deg_min") = 3, py::arg("deg_max") = 8,
            py::arg("continuity") = GeomAbs_C2, py::arg("tol2d") = 1.0e-6,
            "Creates approximation from 2D points with degree and continuity constraints")
        
        .def(py::init([](const std::vector<double>& y_values, double x0, double dx,
                        int deg_min, int deg_max, GeomAbs_Shape continuity, double tol2d) {
            return new Geom2dAPI_PointsToBSpline(vector_to_Array1OfReal(y_values), x0, dx, deg_min, deg_max, continuity, tol2d);
        }), py::arg("y_values"), py::arg("x0"), py::arg("dx"),
            py::arg("deg_min") = 3, py::arg("deg_max") = 8,
            py::arg("continuity") = GeomAbs_C2, py::arg("tol2d") = 1.0e-6,
            "Creates approximation from Y values at uniform X spacing.\n"
            "Points are: (x0 + i*dx, y_values[i])")
        
        .def(py::init([](const std::vector<gp_Pnt2d>& points, Approx_ParametrizationType par_type,
                        int deg_min, int deg_max, GeomAbs_Shape continuity, double tol2d) {
            return new Geom2dAPI_PointsToBSpline(vector_to_Array1OfPnt2d(points), par_type, deg_min, deg_max, continuity, tol2d);
        }), py::arg("points"), py::arg("par_type"), py::arg("deg_min") = 3, py::arg("deg_max") = 8,
            py::arg("continuity") = GeomAbs_C2, py::arg("tol2d") = 1.0e-3,
            "Creates approximation with specified parametrization type")
        
        .def(py::init([](const std::vector<gp_Pnt2d>& points, const std::vector<double>& parameters,
                        int deg_min, int deg_max, GeomAbs_Shape continuity, double tol2d) {
            return new Geom2dAPI_PointsToBSpline(vector_to_Array1OfPnt2d(points), vector_to_Array1OfReal(parameters), 
                                                  deg_min, deg_max, continuity, tol2d);
        }), py::arg("points"), py::arg("parameters"), py::arg("deg_min") = 3, py::arg("deg_max") = 8,
            py::arg("continuity") = GeomAbs_C2, py::arg("tol2d") = 1.0e-3,
            "Creates approximation with explicit parameter values")
        
        .def(py::init([](const std::vector<gp_Pnt2d>& points, double weight1, double weight2, double weight3,
                        int deg_max, GeomAbs_Shape continuity, double tol2d) {
            return new Geom2dAPI_PointsToBSpline(vector_to_Array1OfPnt2d(points), weight1, weight2, weight3, 
                                                  deg_max, continuity, tol2d);
        }), py::arg("points"), py::arg("weight1"), py::arg("weight2"), py::arg("weight3"),
            py::arg("deg_max") = 8, py::arg("continuity") = GeomAbs_C2, py::arg("tol2d") = 1.0e-3,
            "Creates approximation using variational smoothing.\n"
            "Minimizes: weight1*Length + weight2*Curvature + weight3*Torsion")

        .def("init", [](Geom2dAPI_PointsToBSpline& self, const std::vector<gp_Pnt2d>& points,
                       int deg_min, int deg_max, GeomAbs_Shape continuity, double tol2d) {
            self.Init(vector_to_Array1OfPnt2d(points), deg_min, deg_max, continuity, tol2d);
        }, py::arg("points"), py::arg("deg_min") = 3, py::arg("deg_max") = 8,
            py::arg("continuity") = GeomAbs_C2, py::arg("tol2d") = 1.0e-6,
            "Initializes and computes approximation from 2D points")
        
        .def_property_readonly("is_done", &Geom2dAPI_PointsToBSpline::IsDone,
            "True if approximation was successful")
        
        .def_property_readonly("curve", &Geom2dAPI_PointsToBSpline::Curve,
            "Returns the computed 2D BSpline curve (raises if not done)")
    ;

    // =========================================================================
    // GeomAPI_ExtremaCurveCurve - Compute extrema between two 3D curves
    // =========================================================================
    py::class_<GeomAPI_ExtremaCurveCurve>(m, "ExtremaCurveCurve",
        R"doc(
        Computes all extrema (minimum and maximum distances) between two 3D curves.

        An ExtremaCurveCurve algorithm minimizes or maximizes the distance between
        a point on the first curve and a point on the second curve. Thus, it computes
        start and end points of perpendiculars common to the two curves.

        Solutions consist of pairs of points, and an extremum is considered to be
        a segment joining the two points of a solution.

        Warning: In some cases, the nearest points between two curves do not
        correspond to one of the computed extrema. Instead, they may be given by:
        - A limit point of one curve and its orthogonal projection on the other
        - A limit point of both curves
        - An intersection point between the two curves

        Use TotalNearestPoints() to get the true nearest points accounting for
        curve endpoints.

        Example:
            c1 = Geom.Line(gp.Pnt(0,0,0), gp.Dir(1,0,0))
            c2 = Geom.Line(gp.Pnt(0,1,0), gp.Dir(0,0,1))
            extrema = GeomAPI.ExtremaCurveCurve(c1, c2)
            if extrema.nb_extrema > 0:
                dist = extrema.lower_distance
                p1, p2 = extrema.nearest_points()
        )doc")
        
        .def(py::init<>(),
            "Creates an empty algorithm. Use init() to define the curves.")
        
        .def(py::init<const Handle(Geom_Curve)&, const Handle(Geom_Curve)&>(),
            py::arg("c1"), py::arg("c2"),
            "Computes the extrema between curves C1 and C2.")
        
        .def(py::init<const Handle(Geom_Curve)&, const Handle(Geom_Curve)&,
                      Standard_Real, Standard_Real, Standard_Real, Standard_Real>(),
            py::arg("c1"), py::arg("c2"),
            py::arg("u1_min"), py::arg("u1_max"),
            py::arg("u2_min"), py::arg("u2_max"),
            "Computes extrema between portions of the curves.\n\n"
            "Parameters:\n"
            "  c1, c2: The two curves\n"
            "  u1_min, u1_max: Parameter bounds for C1\n"
            "  u2_min, u2_max: Parameter bounds for C2")
        
        .def("init", py::overload_cast<const Handle(Geom_Curve)&, const Handle(Geom_Curve)&>(
            &GeomAPI_ExtremaCurveCurve::Init),
            py::arg("c1"), py::arg("c2"),
            "Initializes and computes extrema between curves C1 and C2.")
        
        .def("init", py::overload_cast<const Handle(Geom_Curve)&, const Handle(Geom_Curve)&,
                                       Standard_Real, Standard_Real, Standard_Real, Standard_Real>(
            &GeomAPI_ExtremaCurveCurve::Init),
            py::arg("c1"), py::arg("c2"),
            py::arg("u1_min"), py::arg("u1_max"),
            py::arg("u2_min"), py::arg("u2_max"),
            "Initializes and computes extrema between portions of curves.")
        
        .def_property_readonly("nb_extrema", &GeomAPI_ExtremaCurveCurve::NbExtrema,
            "Returns the number of extrema computed. Returns 0 if algorithm fails.")
        
        .def("points", [](const GeomAPI_ExtremaCurveCurve& self, Standard_Integer index) {
            gp_Pnt P1, P2;
            self.Points(index, P1, P2);
            return std::make_tuple(P1, P2);
        }, py::arg("index"),
            "Returns points P1 on first curve and P2 on second curve for extremum at index.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_extrema]\n\n"
            "Returns:\n"
            "  Tuple of (P1, P2)")
        
        .def("parameters", [](const GeomAPI_ExtremaCurveCurve& self, Standard_Integer index) {
            Standard_Real U1, U2;
            self.Parameters(index, U1, U2);
            return std::make_tuple(U1, U2);
        }, py::arg("index"),
            "Returns parameters U1 on first curve and U2 on second curve for extremum at index.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_extrema]\n\n"
            "Returns:\n"
            "  Tuple of (U1, U2)")
        
        .def("distance", &GeomAPI_ExtremaCurveCurve::Distance, py::arg("index"),
            "Returns the distance between the endpoints of extremum at index.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_extrema]")
        
        .def_property_readonly("is_parallel", &GeomAPI_ExtremaCurveCurve::IsParallel,
            "Returns True if the two curves are parallel.")
        
        // .def_property_readonly("points", [](const GeomAPI_ExtremaCurveCurve& self) {
        //     std::vector<std::pair<gp_Pnt, gp_Pnt>> result;
        //     Standard_Integer n = self.NbExtrema();
        //     for (Standard_Integer i = 1; i <= n; ++i) {
        //         gp_Pnt P1, P2;
        //         self.Points(i, P1, P2);
        //         result.emplace_back(P1, P2);
        //     }
        //     return result;
        // },
        //     "Returns a list of all extremum point pairs as (P1, P2) tuples.")
        // .def_property_readonly("parameters", [](const GeomAPI_ExtremaCurveCurve& self) {
        //     std::vector<std::pair<Standard_Real, Standard_Real>> result;
        //     Standard_Integer n = self.NbExtrema();
        //     for (Standard_Integer i = 1; i <= n; ++i) {
        //         Standard_Real U1, U2;
        //         self.Parameters(i, U1, U2);
        //         result.emplace_back(U1, U2);
        //     }
        //     return result;
        // },
        //     "Returns a list of all extremum parameter pairs as (U1, U2) tuples.")

        .def_property_readonly("nearest_points", [](const GeomAPI_ExtremaCurveCurve& self) {
            gp_Pnt P1, P2;
            self.NearestPoints(P1, P2);
            return std::make_tuple(P1, P2);
        },
            "Returns points P1 and P2 that are the ends of the shortest extremum.\n\n"
            "Returns:\n"
            "  Tuple of (P1, P2)\n\n"
            "Raises:\n"
            "  StdFail_NotDone if algorithm fails")
        
        .def_property_readonly("lower_distance_parameters", [](const GeomAPI_ExtremaCurveCurve& self) {
            Standard_Real U1, U2;
            self.LowerDistanceParameters(U1, U2);
            return std::make_tuple(U1, U2);
        },
            "Returns parameters U1 and U2 of the shortest extremum endpoints.\n\n"
            "Returns:\n"
            "  Tuple of (U1, U2)\n\n"
            "Raises:\n"
            "  StdFail_NotDone if algorithm fails")
        
        .def_property_readonly("lower_distance", &GeomAPI_ExtremaCurveCurve::LowerDistance,
            "Returns the distance of the shortest extremum.\n\n"
            "Raises:\n"
            "  StdFail_NotDone if algorithm fails")
        
        .def_property_readonly("total_nearest_points", [](GeomAPI_ExtremaCurveCurve& self) {
            gp_Pnt P1, P2;
            bool found = self.TotalNearestPoints(P1, P2);
            if (found) {
                return py::make_tuple(true, P1, P2);
            }
            return py::make_tuple(false, gp_Pnt(), gp_Pnt());
        },
            "Returns the true nearest points considering curve endpoints.\n\n"
            "This method accounts for limit points of the curves, which may be\n"
            "closer than any computed extremum.\n\n"
            "Returns:\n"
            "  Tuple of (found, P1, P2) where found is True if successful")
        
        .def_property_readonly("total_lower_distance_parameters", [](GeomAPI_ExtremaCurveCurve& self) {
            Standard_Real U1, U2;
            bool found = self.TotalLowerDistanceParameters(U1, U2);
            if (found) {
                return py::make_tuple(true, U1, U2);
            }
            return py::make_tuple(false, 0.0, 0.0);
        },
            "Returns parameters of the true nearest points.\n\n"
            "Returns:\n"
            "  Tuple of (found, U1, U2) where found is True if successful")
        
        .def_property_readonly("total_lower_distance", &GeomAPI_ExtremaCurveCurve::TotalLowerDistance,
            "Returns the distance of the true nearest points.\n\n"
            "This accounts for curve endpoints and may be smaller than lower_distance.")

        .def_property_readonly("result",[](GeomAPI_ExtremaCurveCurve& self) {
            bool found = true;
            gp_Pnt P1, P2;
            found = found && self.TotalNearestPoints(P1, P2);
            Standard_Real U1, U2;
            found = found && self.TotalLowerDistanceParameters(U1, U2);
            auto dist = self.TotalLowerDistance();
            if (found) {
                return py::make_tuple(true, P1, P2, U1, U2, dist);
            }
            return py::make_tuple(false, gp_Pnt(), gp_Pnt(), 0.0, 0.0, 0.0);

        },"Returns a complete result of the nearest points computation.\n\n"
        )
    ;

    // =========================================================================
    // GeomAPI_ExtremaCurveSurface - Compute extrema between curve and surface
    // =========================================================================
    py::class_<GeomAPI_ExtremaCurveSurface>(m, "ExtremaCurveSurface",
        R"doc(
        Computes all extrema between a 3D curve and a surface.

        An ExtremaCurveSurface algorithm minimizes or maximizes the distance
        between a point on the curve and a point on the surface. Thus, it
        computes start and end points of perpendiculars common to the curve
        and the surface.

        Solutions consist of pairs of points, and an extremum is considered
        to be a segment joining the two points of a solution.

        Warning: In some cases, the nearest points may be given by:
        - A point of a bounding curve of the surface and its projection
        - A limit point of the curve and its projection on the surface
        - An intersection point between the curve and the surface

        Example:
            curve = Geom.Line(gp.Pnt(0,0,5), gp.Dir(1,0,0))
            surface = Geom.Plane(gp.Ax3())
            extrema = GeomAPI.ExtremaCurveSurface(curve, surface)
            dist = extrema.lower_distance  # Should be 5
        )doc")
        
        .def(py::init<>(),
            "Creates an empty algorithm. Use init() to define curve and surface.")
        
        .def(py::init<const Handle(Geom_Curve)&, const Handle(Geom_Surface)&>(),
            py::arg("curve"), py::arg("surface"),
            "Computes extrema between a curve and a surface.")
        
        .def(py::init<const Handle(Geom_Curve)&, const Handle(Geom_Surface)&,
                      Standard_Real, Standard_Real, Standard_Real, Standard_Real,
                      Standard_Real, Standard_Real>(),
            py::arg("curve"), py::arg("surface"),
            py::arg("w_min"), py::arg("w_max"),
            py::arg("u_min"), py::arg("u_max"),
            py::arg("v_min"), py::arg("v_max"),
            "Computes extrema on portions of curve and surface.\n\n"
            "Parameters:\n"
            "  curve, surface: The curve and surface\n"
            "  w_min, w_max: Parameter bounds for the curve\n"
            "  u_min, u_max, v_min, v_max: Parameter bounds for the surface")
        
        .def("init", py::overload_cast<const Handle(Geom_Curve)&, const Handle(Geom_Surface)&>(
            &GeomAPI_ExtremaCurveSurface::Init),
            py::arg("curve"), py::arg("surface"),
            "Initializes and computes extrema between curve and surface.")
        
        .def("init", py::overload_cast<const Handle(Geom_Curve)&, const Handle(Geom_Surface)&,
                                       Standard_Real, Standard_Real, Standard_Real, Standard_Real,
                                       Standard_Real, Standard_Real>(
            &GeomAPI_ExtremaCurveSurface::Init),
            py::arg("curve"), py::arg("surface"),
            py::arg("w_min"), py::arg("w_max"),
            py::arg("u_min"), py::arg("u_max"),
            py::arg("v_min"), py::arg("v_max"),
            "Initializes and computes extrema on portions of curve and surface.")
        
        .def_property_readonly("nb_extrema", &GeomAPI_ExtremaCurveSurface::NbExtrema,
            "Returns the number of extrema computed. Returns 0 if algorithm fails.")
        
        .def("points", [](const GeomAPI_ExtremaCurveSurface& self, Standard_Integer index) {
            gp_Pnt P1, P2;
            self.Points(index, P1, P2);
            return std::make_tuple(P1, P2);
        }, py::arg("index"),
            "Returns points P1 on the curve and P2 on the surface for extremum at index.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_extrema]\n\n"
            "Returns:\n"
            "  Tuple of (P1, P2)")
        
        .def("parameters", [](const GeomAPI_ExtremaCurveSurface& self, Standard_Integer index) {
            Standard_Real W, U, V;
            self.Parameters(index, W, U, V);
            return std::make_tuple(W, U, V);
        }, py::arg("index"),
            "Returns parameters W on curve and (U,V) on surface for extremum at index.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_extrema]\n\n"
            "Returns:\n"
            "  Tuple of (W, U, V)")
        
        .def("distance", &GeomAPI_ExtremaCurveSurface::Distance, py::arg("index"),
            "Returns the distance between endpoints of extremum at index.")
        
        .def_property_readonly("is_parallel", &GeomAPI_ExtremaCurveSurface::IsParallel,
            "Returns True if the curve lies on a surface parallel to the given surface.")
        
        .def_property_readonly("nearest_points", [](const GeomAPI_ExtremaCurveSurface& self) {
            gp_Pnt PC, PS;
            self.NearestPoints(PC, PS);
            return std::make_tuple(PC, PS);
        },
            "Returns points PC on curve and PS on surface of the shortest extremum.\n\n"
            "Returns:\n"
            "  Tuple of (PC, PS)")
        
        .def_property_readonly("lower_distance_parameters", [](const GeomAPI_ExtremaCurveSurface& self) {
            Standard_Real W, U, V;
            self.LowerDistanceParameters(W, U, V);
            return std::make_tuple(W, U, V);
        },
            "Returns parameters of the shortest extremum endpoints.\n\n"
            "Returns:\n"
            "  Tuple of (W, U, V) - W on curve, (U,V) on surface")
        
        .def_property_readonly("lower_distance", &GeomAPI_ExtremaCurveSurface::LowerDistance,
            "Returns the distance of the shortest extremum.")
        
            // TODO: add is_parallel to GeomAPI_ExtremaCurveSurface
        .def_property_readonly("is_parallel", [](const GeomAPI_ExtremaCurveSurface& self) {
            return self.Extrema().IsParallel();
            },
            "Returns True if the curve lies on a surface parallel to the given surface.")


        .def_property_readonly("result",[](GeomAPI_ExtremaCurveSurface& self) {
            bool found = self.NbExtrema() > 0;
            if (found) {
                gp_Pnt PC, PS;
                self.NearestPoints(PC, PS);
                Standard_Real W, U, V;
                self.LowerDistanceParameters(W, U, V);
                auto dist = self.LowerDistance();
                return py::make_tuple(true, PC, PS, W, U, V, dist);
            }
            return py::make_tuple(false, gp_Pnt(), gp_Pnt(), 0.0, 0.0, 0.0, 0.0);

        },"Returns a complete result of the nearest points computation.\n\n"
        )
    ;

    // =========================================================================
    // GeomAPI_ExtremaSurfaceSurface - Compute extrema between two surfaces
    // =========================================================================
    py::class_<GeomAPI_ExtremaSurfaceSurface>(m, "ExtremaSurfaceSurface",
        R"doc(
        Computes all extrema between two surfaces.

        An ExtremaSurfaceSurface algorithm minimizes or maximizes the distance
        between a point on the first surface and a point on the second surface.
        Results are start and end points of perpendiculars common to the two surfaces.

        Warning: In some cases, the nearest points may be given by:
        - A point of a bounding curve and its projection
        - Any point on intersection curves between the two surfaces

        Example:
            s1 = Geom.Plane(gp.Ax3(gp.Pnt(0,0,0), gp.Dir(0,0,1)))
            s2 = Geom.Plane(gp.Ax3(gp.Pnt(0,0,10), gp.Dir(0,0,1)))
            extrema = GeomAPI.ExtremaSurfaceSurface(s1, s2)
            dist = extrema.lower_distance  # Should be 10
        )doc")
        
        .def(py::init<>(),
            "Creates an empty algorithm. Use init() to define the surfaces.")
        
        .def(py::init<const Handle(Geom_Surface)&, const Handle(Geom_Surface)&>(),
            py::arg("s1"), py::arg("s2"),
            "Computes extrema between surfaces S1 and S2.")
        
        .def(py::init<const Handle(Geom_Surface)&, const Handle(Geom_Surface)&,
                      Standard_Real, Standard_Real, Standard_Real, Standard_Real,
                      Standard_Real, Standard_Real, Standard_Real, Standard_Real>(),
            py::arg("s1"), py::arg("s2"),
            py::arg("u1_min"), py::arg("u1_max"), py::arg("v1_min"), py::arg("v1_max"),
            py::arg("u2_min"), py::arg("u2_max"), py::arg("v2_min"), py::arg("v2_max"),
            "Computes extrema on portions of the surfaces.\n\n"
            "Parameters:\n"
            "  s1, s2: The two surfaces\n"
            "  u1_min, u1_max, v1_min, v1_max: Parameter bounds for S1\n"
            "  u2_min, u2_max, v2_min, v2_max: Parameter bounds for S2")
        
        .def("init", py::overload_cast<const Handle(Geom_Surface)&, const Handle(Geom_Surface)&>(
            &GeomAPI_ExtremaSurfaceSurface::Init),
            py::arg("s1"), py::arg("s2"),
            "Initializes and computes extrema between surfaces S1 and S2.")
        
        .def("init", py::overload_cast<const Handle(Geom_Surface)&, const Handle(Geom_Surface)&,
                                       Standard_Real, Standard_Real, Standard_Real, Standard_Real,
                                       Standard_Real, Standard_Real, Standard_Real, Standard_Real>(
            &GeomAPI_ExtremaSurfaceSurface::Init),
            py::arg("s1"), py::arg("s2"),
            py::arg("u1_min"), py::arg("u1_max"), py::arg("v1_min"), py::arg("v1_max"),
            py::arg("u2_min"), py::arg("u2_max"), py::arg("v2_min"), py::arg("v2_max"),
            "Initializes and computes extrema on portions of surfaces.")
        
        .def_property_readonly("nb_extrema", &GeomAPI_ExtremaSurfaceSurface::NbExtrema,
            "Returns the number of extrema computed. Returns 0 if algorithm fails.")
        
        .def("points", [](const GeomAPI_ExtremaSurfaceSurface& self, Standard_Integer index) {
            gp_Pnt P1, P2;
            self.Points(index, P1, P2);
            return std::make_tuple(P1, P2);
        }, py::arg("index"),
            "Returns points P1 on first surface and P2 on second surface.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_extrema]\n\n"
            "Returns:\n"
            "  Tuple of (P1, P2)")
        
        .def("parameters", [](const GeomAPI_ExtremaSurfaceSurface& self, Standard_Integer index) {
            Standard_Real U1, V1, U2, V2;
            self.Parameters(index, U1, V1, U2, V2);
            return std::make_tuple(U1, V1, U2, V2);
        }, py::arg("index"),
            "Returns parameters (U1,V1) on S1 and (U2,V2) on S2 for extremum at index.\n\n"
            "Returns:\n"
            "  Tuple of (U1, V1, U2, V2)")
        
        .def("distance", &GeomAPI_ExtremaSurfaceSurface::Distance, py::arg("index"),
            "Returns the distance between endpoints of extremum at index.")
        
        .def_property_readonly("is_parallel", &GeomAPI_ExtremaSurfaceSurface::IsParallel,
            "Returns True if the two surfaces are parallel.")
        
        .def_property_readonly("nearest_points", [](const GeomAPI_ExtremaSurfaceSurface& self) {
            gp_Pnt P1, P2;
            self.NearestPoints(P1, P2);
            return std::make_tuple(P1, P2);
        },
            "Returns points P1 and P2 of the shortest extremum.\n\n"
            "Returns:\n"
            "  Tuple of (P1, P2)")
        
        .def_property_readonly("lower_distance_parameters", [](const GeomAPI_ExtremaSurfaceSurface& self) {
            Standard_Real U1, V1, U2, V2;
            self.LowerDistanceParameters(U1, V1, U2, V2);
            return std::make_tuple(U1, V1, U2, V2);
        },
            "Returns parameters of the shortest extremum endpoints.\n\n"
            "Returns:\n"
            "  Tuple of (U1, V1, U2, V2)")
        
        .def_property_readonly("lower_distance", &GeomAPI_ExtremaSurfaceSurface::LowerDistance,
            "Returns the distance of the shortest extremum.")

        .def_property_readonly("result",[](GeomAPI_ExtremaSurfaceSurface& self) {
            bool found = self.NbExtrema() > 0;
            if (found) {
                gp_Pnt P1, P2;
                self.NearestPoints(P1, P2);
                Standard_Real U1, V1, U2, V2;
                self.LowerDistanceParameters(U1, V1, U2, V2);
                auto dist = self.LowerDistance();
                return py::make_tuple(true, P1, P2, U1, V1, U2, V2, dist);
            }
            return py::make_tuple(false, gp_Pnt(), gp_Pnt(), 0.0, 0.0, 0.0, 0.0, 0.0);

        },"Returns a complete result of the nearest points computation.\n\n"
        )
        
    ;

    // =========================================================================
    // Geom2dAPI_ExtremaCurveCurve - Compute extrema between two 2D curves
    // =========================================================================
    py::class_<Geom2dAPI_ExtremaCurveCurve>(m, "ExtremaCurveCurve2d",
        R"doc(
        Computes all extrema between two 2D curves.

        An ExtremaCurveCurve2d algorithm minimizes or maximizes the distance
        between a point on the first curve and a point on the second curve.

        Note: Unlike the 3D version, this class requires parameter bounds.

        Example:
            c1 = Geom2d.Line(gp.Pnt2d(0,0), gp.Dir2d(1,0))
            c2 = Geom2d.Line(gp.Pnt2d(0,5), gp.Dir2d(1,0))
            extrema = GeomAPI.ExtremaCurveCurve2d(c1, c2, 0, 10, 0, 10)
            dist = extrema.lower_distance  # Should be 5
        )doc")
        
        .def(py::init<const Handle(Geom2d_Curve)&, const Handle(Geom2d_Curve)&,
                      Standard_Real, Standard_Real, Standard_Real, Standard_Real>(),
            py::arg("c1"), py::arg("c2"),
            py::arg("u1_min"), py::arg("u1_max"),
            py::arg("u2_min"), py::arg("u2_max"),
            "Computes extrema between portions of curves C1 and C2.\n\n"
            "Parameters:\n"
            "  c1, c2: The two 2D curves\n"
            "  u1_min, u1_max: Parameter bounds for C1\n"
            "  u2_min, u2_max: Parameter bounds for C2")
        
        .def_property_readonly("nb_extrema", &Geom2dAPI_ExtremaCurveCurve::NbExtrema,
            "Returns the number of extrema computed. Returns 0 if algorithm fails.")
        
        .def("points", [](const Geom2dAPI_ExtremaCurveCurve& self, Standard_Integer index) {
            gp_Pnt2d P1, P2;
            self.Points(index, P1, P2);
            return std::make_tuple(P1, P2);
        }, py::arg("index"),
            "Returns points P1 on first curve and P2 on second curve.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_extrema]\n\n"
            "Returns:\n"
            "  Tuple of (P1, P2)")
        
        .def("parameters", [](const Geom2dAPI_ExtremaCurveCurve& self, Standard_Integer index) {
            Standard_Real U1, U2;
            self.Parameters(index, U1, U2);
            return std::make_tuple(U1, U2);
        }, py::arg("index"),
            "Returns parameters U1 on C1 and U2 on C2 for extremum at index.\n\n"
            "Returns:\n"
            "  Tuple of (U1, U2)")
        
        .def("distance", &Geom2dAPI_ExtremaCurveCurve::Distance, py::arg("index"),
            "Returns the distance between endpoints of extremum at index.")
        
        .def("nearest_points", [](const Geom2dAPI_ExtremaCurveCurve& self) {
            gp_Pnt2d P1, P2;
            self.NearestPoints(P1, P2);
            return std::make_tuple(P1, P2);
        },
            "Returns points P1 and P2 of the shortest extremum.\n\n"
            "Returns:\n"
            "  Tuple of (P1, P2)")
        
        .def_property_readonly("lower_distance_parameters", [](const Geom2dAPI_ExtremaCurveCurve& self) {
            Standard_Real U1, U2;
            self.LowerDistanceParameters(U1, U2);
            return std::make_tuple(U1, U2);
        },
            "Returns parameters U1 and U2 of the shortest extremum endpoints.\n\n"
            "Returns:\n"
            "  Tuple of (U1, U2)")
        
        .def_property_readonly("lower_distance", &Geom2dAPI_ExtremaCurveCurve::LowerDistance,
            "Returns the distance of the shortest extremum.")

        .def_property_readonly("is_parallel", [](const Geom2dAPI_ExtremaCurveCurve& self) {
                return self.Extrema().IsParallel();
            },
            "Returns True if the two curves are parallel."
        ) // TODO fix occt

        .def_property_readonly("result",[](Geom2dAPI_ExtremaCurveCurve& self) {
            bool found = self.NbExtrema() > 0;
            if (found) {
                gp_Pnt2d P1, P2;
                self.NearestPoints(P1, P2);
                Standard_Real U1, U2;
                self.LowerDistanceParameters(U1, U2);
                auto dist = self.LowerDistance();
                return py::make_tuple(true, P1, P2, U1, U2, dist);
            }
            return py::make_tuple(false, gp_Pnt2d(), gp_Pnt2d(), 0.0, 0.0, 0.0);

        },"Returns a complete result of the nearest points computation.\n\n"
        )
    ;

    // =========================================================================
    // GeomAPI_ProjectPointOnCurve - Project point onto 3D curve
    // =========================================================================
    py::class_<GeomAPI_ProjectPointOnCurve>(m, "ProjectPointOnCurve",
        R"doc(
        Computes all orthogonal projections of a 3D point onto a 3D curve.

        This class finds all points on the curve where the line from the
        given point to the curve is perpendicular to the curve tangent.

        Example:
            curve = Geom.Circle(gp.Ax2(), 10.0)
            proj = GeomAPI.ProjectPointOnCurve(gp.Pnt(15, 0, 0), curve)
            nearest = proj.nearest_point  # Point on circle closest to (15,0,0)
            dist = proj.lower_distance    # Should be 5 (15 - radius 10)
        )doc")
        
        .def(py::init<>(),
            "Creates an empty projector. Use init() to define point and curve.")
        
        .def(py::init<const gp_Pnt&, const Handle(Geom_Curve)&>(),
            py::arg("point"), py::arg("curve"),
            "Creates a projection of point P onto curve.")
        
        .def(py::init<const gp_Pnt&, const Handle(Geom_Curve)&, Standard_Real, Standard_Real>(),
            py::arg("point"), py::arg("curve"), py::arg("u_min"), py::arg("u_max"),
            "Creates a projection onto a portion of the curve.\n\n"
            "Parameters:\n"
            "  point: The point to project\n"
            "  curve: The curve to project onto\n"
            "  u_min, u_max: Parameter bounds")
        
        .def("init", py::overload_cast<const gp_Pnt&, const Handle(Geom_Curve)&>(
            &GeomAPI_ProjectPointOnCurve::Init),
            py::arg("point"), py::arg("curve"),
            "Initializes projection of point onto curve.")
        
        .def("init", py::overload_cast<const gp_Pnt&, const Handle(Geom_Curve)&,
                                       Standard_Real, Standard_Real>(
            &GeomAPI_ProjectPointOnCurve::Init),
            py::arg("point"), py::arg("curve"), py::arg("u_min"), py::arg("u_max"),
            "Initializes projection onto a portion of the curve.")
        
        .def("init_curve", [](GeomAPI_ProjectPointOnCurve& self, const Handle(Geom_Curve)& curve,
                              Standard_Real u_min, Standard_Real u_max) {
            self.Init(curve, u_min, u_max);
        }, py::arg("curve"), py::arg("u_min"), py::arg("u_max"),
            "Initializes the curve only. Use perform() to project points.")
        
        .def("perform", &GeomAPI_ProjectPointOnCurve::Perform, py::arg("point"),
            "Projects a point onto the current curve.")
        
        .def_property_readonly("nb_points", &GeomAPI_ProjectPointOnCurve::NbPoints,
            "Returns the number of computed projection points. Returns 0 if fails.")
        
        .def("point", &GeomAPI_ProjectPointOnCurve::Point, py::arg("index"),
            "Returns the orthogonal projection point at index.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_points]")
        
        .def("parameter", py::overload_cast<Standard_Integer>(
            &GeomAPI_ProjectPointOnCurve::Parameter, py::const_), py::arg("index"),
            "Returns the parameter on the curve of projection at index.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_points]")
        
        .def("distance", &GeomAPI_ProjectPointOnCurve::Distance, py::arg("index"),
            "Returns the distance from the point to projection at index.")
        
        .def_property_readonly("nearest_point", &GeomAPI_ProjectPointOnCurve::NearestPoint,
            "Returns the nearest orthogonal projection of the point on the curve.")
        
        .def_property_readonly("lower_distance_parameter",
            &GeomAPI_ProjectPointOnCurve::LowerDistanceParameter,
            "Returns the parameter on curve of the nearest projection.")
        
        .def_property_readonly("lower_distance", &GeomAPI_ProjectPointOnCurve::LowerDistance,
            "Returns the distance to the nearest projection.")
        
        .def_property_readonly("result", [](GeomAPI_ProjectPointOnCurve& self) {
            gp_Pnt P = self.NearestPoint();
            Standard_Real U = self.LowerDistanceParameter();
            Standard_Real dist = self.LowerDistance();
            return py::make_tuple(P, U, dist);
        },
            "Returns a complete result of the nearest point projection.\n\n"
            "Returns:\n"
            "  Tuple of (P, U, distance) where P is the nearest point on curve,\n"
            "  U is its parameter, and distance is the distance to the original point.")
    ;

    // =========================================================================
    // Geom2dAPI_ProjectPointOnCurve - Project point onto 2D curve
    // =========================================================================
    py::class_<Geom2dAPI_ProjectPointOnCurve>(m, "ProjectPointOnCurve2d",
        R"doc(
        Computes all orthogonal projections of a 2D point onto a 2D curve.

        This class finds all points on the curve where the line from the
        given point to the curve is perpendicular to the curve tangent.

        Example:
            curve = Geom2d.Circle(gp.Ax2d(), 10.0)
            proj = GeomAPI.ProjectPointOnCurve2d(gp.Pnt2d(15, 0), curve)
            nearest = proj.nearest_point  # Point on circle closest to (15,0)
        )doc")
        
        .def(py::init<>(),
            "Creates an empty projector. Use init() to define point and curve.")
        
        .def(py::init<const gp_Pnt2d&, const Handle(Geom2d_Curve)&>(),
            py::arg("point"), py::arg("curve"),
            "Creates a projection of 2D point onto curve.")
        
        .def(py::init<const gp_Pnt2d&, const Handle(Geom2d_Curve)&, Standard_Real, Standard_Real>(),
            py::arg("point"), py::arg("curve"), py::arg("u_min"), py::arg("u_max"),
            "Creates a projection onto a portion of the curve.\n\n"
            "Parameters:\n"
            "  point: The 2D point to project\n"
            "  curve: The 2D curve to project onto\n"
            "  u_min, u_max: Parameter bounds")
        
        .def("init", py::overload_cast<const gp_Pnt2d&, const Handle(Geom2d_Curve)&>(
            &Geom2dAPI_ProjectPointOnCurve::Init),
            py::arg("point"), py::arg("curve"),
            "Initializes projection of point onto curve.")
        
        .def("init", py::overload_cast<const gp_Pnt2d&, const Handle(Geom2d_Curve)&,
                                       Standard_Real, Standard_Real>(
            &Geom2dAPI_ProjectPointOnCurve::Init),
            py::arg("point"), py::arg("curve"), py::arg("u_min"), py::arg("u_max"),
            "Initializes projection onto a portion of the curve.")
        
        .def_property_readonly("nb_points", &Geom2dAPI_ProjectPointOnCurve::NbPoints,
            "Returns the number of computed projection points. Returns 0 if fails.")
        
        .def("point", &Geom2dAPI_ProjectPointOnCurve::Point, py::arg("index"),
            "Returns the orthogonal projection point at index.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_points]")
        
        .def("parameter", py::overload_cast<Standard_Integer>(
            &Geom2dAPI_ProjectPointOnCurve::Parameter, py::const_), py::arg("index"),
            "Returns the parameter on the curve of projection at index.")
        
        .def("distance", &Geom2dAPI_ProjectPointOnCurve::Distance, py::arg("index"),
            "Returns the distance from the point to projection at index.")
        
        .def_property_readonly("nearest_point", &Geom2dAPI_ProjectPointOnCurve::NearestPoint,
            "Returns the nearest orthogonal projection of the point on the curve.")
        
        .def_property_readonly("lower_distance_parameter",
            &Geom2dAPI_ProjectPointOnCurve::LowerDistanceParameter,
            "Returns the parameter on curve of the nearest projection.")
        
        .def_property_readonly("lower_distance", &Geom2dAPI_ProjectPointOnCurve::LowerDistance,
            "Returns the distance to the nearest projection.")

        .def_property_readonly("result", [](Geom2dAPI_ProjectPointOnCurve& self) {
            gp_Pnt2d P = self.NearestPoint();
            Standard_Real U = self.LowerDistanceParameter();
            Standard_Real dist = self.LowerDistance();
            return py::make_tuple(P, U, dist);
        },
            "Returns a complete result of the nearest point projection.\n\n"
            "Returns:\n"
            "  Tuple of (P, U, distance) where P is the nearest point on curve,\n"
            "  U is its parameter, and distance is the distance to the original point.")
    ;

    // =========================================================================
    // Geom2dAPI_InterCurveCurve - Compute intersections between 2D curves
    // =========================================================================
    py::class_<Geom2dAPI_InterCurveCurve>(m, "InterCurveCurve2d",
        R"doc(
        Computes intersections between two 2D curves, or self-intersections of a curve.

        This class finds:
        - Intersection points in case of cross intersections
        - Intersection segments in case of tangential intersections
        - Nothing if no intersections exist

        Example:
            c1 = Geom2d.Line(gp.Pnt2d(0,0), gp.Dir2d(1,1))
            c2 = Geom2d.Line(gp.Pnt2d(10,0), gp.Dir2d(-1,1))
            inter = GeomAPI.InterCurveCurve2d(c1, c2)
            if inter.nb_points > 0:
                pt = inter.point(1)  # First intersection point
        )doc")
        
        .def(py::init<>(),
            "Creates an empty intersector. Use init() to define curves.")
        
        .def(py::init<const Handle(Geom2d_Curve)&, const Handle(Geom2d_Curve)&, Standard_Real>(),
            py::arg("c1"), py::arg("c2"), py::arg("tol") = 1.0e-6,
            "Creates an intersector and computes intersections between C1 and C2.\n\n"
            "Parameters:\n"
            "  c1, c2: The two curves\n"
            "  tol: Tolerance for intersection computation")
        
        .def(py::init<const Handle(Geom2d_Curve)&, Standard_Real>(),
            py::arg("c1"), py::arg("tol") = 1.0e-6,
            "Creates an intersector and computes self-intersections of C1.\n\n"
            "Parameters:\n"
            "  c1: The curve to check for self-intersections\n"
            "  tol: Tolerance for intersection computation")
        
        .def("init", py::overload_cast<const Handle(Geom2d_Curve)&, const Handle(Geom2d_Curve)&,
                                       Standard_Real>(&Geom2dAPI_InterCurveCurve::Init),
            py::arg("c1"), py::arg("c2"), py::arg("tol") = 1.0e-6,
            "Initializes and computes intersections between C1 and C2.")
        
        .def("init", py::overload_cast<const Handle(Geom2d_Curve)&, Standard_Real>(
            &Geom2dAPI_InterCurveCurve::Init),
            py::arg("c1"), py::arg("tol") = 1.0e-6,
            "Initializes and computes self-intersections of C1.")
        
        .def_property_readonly("nb_points", &Geom2dAPI_InterCurveCurve::NbPoints,
            "Returns the number of intersection points (cross intersections).")
        
        .def("point", &Geom2dAPI_InterCurveCurve::Point, py::arg("index"),
            "Returns intersection point at index.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_points]")
        
        .def_property_readonly("nb_segments", &Geom2dAPI_InterCurveCurve::NbSegments,
            "Returns the number of tangential intersection segments.")
        
        .def("segment", [](const Geom2dAPI_InterCurveCurve& self, Standard_Integer index) {
            Handle(Geom2d_Curve) curve1, curve2;
            self.Segment(index, curve1, curve2);
            return std::make_tuple(curve1, curve2);
        }, py::arg("index"),
            "Returns intersection segment curves for tangential intersections.\n\n"
            "Parameters:\n"
            "  index: Index in range [1, nb_segments]\n\n"
            "Returns:\n"
            "  Tuple of (segment_on_c1, segment_on_c2)")
    ;
}
