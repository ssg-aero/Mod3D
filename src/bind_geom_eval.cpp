#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>

#include <Standard_Version.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

#if OCC_VERSION_HEX >= 0x080000

#include <GeomEval_EllipsoidSurface.hxx>
#include <GeomEval_CircularHelixCurve.hxx>
#include <Geom_ElementarySurface.hxx>
#include <Geom_Curve.hxx>
#include <gp_Ax3.hxx>
#include <gp_Ax2.hxx>

void bind_geom_eval(py::module_ &m) {
    // =========================================================================
    // GeomEval - analytic evaluation geometries new in OCCT 8.0. These are
    //   genuine Geom_* subclasses, so they inherit value()/d1()/... from the
    //   bound Geom.ElementarySurface / Geom.Curve bases. We expose a useful
    //   representative subset (more analytic types exist in the package).
    // =========================================================================
    py::class_<GeomEval_EllipsoidSurface,
               opencascade::handle<GeomEval_EllipsoidSurface>,
               Geom_ElementarySurface>(m, "EllipsoidSurface",
        "Triaxial ellipsoid surface defined by a coordinate system and three\n"
        "semi-axes A (XDir), B (YDir), C (ZDir). New in OCCT 8.0.")
        .def(py::init<const gp_Ax3 &, double, double, double>(),
             py::arg("position"), py::arg("a"), py::arg("b"), py::arg("c"),
             "Create a triaxial ellipsoid. All semi-axes must be > 0.")
        .def_property("semi_axis_a", &GeomEval_EllipsoidSurface::SemiAxisA,
                      &GeomEval_EllipsoidSurface::SetSemiAxisA,
                      "Semi-axis along XDir.")
        .def_property("semi_axis_b", &GeomEval_EllipsoidSurface::SemiAxisB,
                      &GeomEval_EllipsoidSurface::SetSemiAxisB,
                      "Semi-axis along YDir.")
        .def_property("semi_axis_c", &GeomEval_EllipsoidSurface::SemiAxisC,
                      &GeomEval_EllipsoidSurface::SetSemiAxisC,
                      "Semi-axis along ZDir.");

    py::class_<GeomEval_CircularHelixCurve,
               opencascade::handle<GeomEval_CircularHelixCurve>,
               Geom_Curve>(m, "CircularHelixCurve",
        "Unbounded circular helix defined by a coordinate system, radius and\n"
        "pitch (axial advance per 2*pi turn). New in OCCT 8.0.")
        .def(py::init<const gp_Ax2 &, double, double>(),
             py::arg("position"), py::arg("radius"), py::arg("pitch"),
             "Create a circular helix. Radius must be > 0; pitch may be "
             "negative.")
        .def_property_readonly("position", &GeomEval_CircularHelixCurve::Position,
                               "Local coordinate system (gp.Ax2).")
        .def_property_readonly("radius", &GeomEval_CircularHelixCurve::Radius,
                               "Helix radius.")
        .def_property_readonly("pitch", &GeomEval_CircularHelixCurve::Pitch,
                               "Axial advance per 2*pi turn.");
}

#else // OCC_VERSION_HEX < 0x080000

void bind_geom_eval(py::module_ &) {}

#endif
