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
}
