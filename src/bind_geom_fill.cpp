#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Standard_Handle.hxx>
#include <GeomFill_Trihedron.hxx>

#include <Standard_Version.hxx>
#if OCC_VERSION_HEX >= 0x080000
#include <GeomFill_Gordon.hxx>
#include <Geom_Curve.hxx>
#include <Geom_BSplineSurface.hxx>
#include <NCollection_Array1.hxx>
#include "extend/geometry/GordonSurface.hpp"
#endif

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

#if OCC_VERSION_HEX >= 0x080000
namespace {
// Build an OCCT array of curve handles from a Python sequence of Geom.Curve.
NCollection_Array1<opencascade::handle<Geom_Curve>>
curves_to_array(const py::sequence &curves, const char *name)
{
    const int n = static_cast<int>(py::len(curves));
    if (n < 2) {
        throw py::value_error(std::string(name) + " must contain at least 2 curves");
    }
    NCollection_Array1<opencascade::handle<Geom_Curve>> array(1, n);
    int i = 1;
    for (const auto &item : curves) {
        array.SetValue(i++, py::cast<opencascade::handle<Geom_Curve>>(item));
    }
    return array;
}
} // namespace
#endif

void bind_geom_fill(py::module_ &m) {
     py::enum_<GeomFill_Trihedron>(m, "Trihedron")
        .value("CorrectedFrenet", GeomFill_IsCorrectedFrenet)
        .value("Fixed", GeomFill_IsFixed)
        .value("Frenet", GeomFill_IsFrenet)
        .value("ConstantNormal", GeomFill_IsConstantNormal)
        .value("Darboux", GeomFill_IsDarboux)
        .value("GuideAC", GeomFill_IsGuideAC)
        .value("GuidePlan", GeomFill_IsGuidePlan)
        .value("GuideACWithContact", GeomFill_IsGuideACWithContact)
        .value("GuidePlanWithContact", GeomFill_IsGuidePlanWithContact)
        .value("DiscreteTrihedron", GeomFill_IsDiscreteTrihedron)
        .export_values();

#if OCC_VERSION_HEX >= 0x080000
    // =========================================================================
    // GeomFill_Gordon - Gordon surface from a network of intersecting curves
    //   (new in OCCT 8.0; generalizes GeomFill_Coons to N x M curve networks).
    // =========================================================================
    py::class_<GeomFill_Gordon>(m, "Gordon",
        "High-level Gordon surface construction from a network of intersecting\n"
        "profile (V-direction) and guide (U-direction) curves.\n\n"
        "Transfinite interpolation via the Boolean-sum formula\n"
        "    S = S_profiles + S_guides - S_tensor\n"
        "generalizing GeomFill_Coons (a 4-boundary patch) to arbitrary N x M\n"
        "curve grids. Accepts arbitrary Geom.Curve inputs (non-rational only).\n\n"
        "New in OCCT 8.0.\n\n"
        "Usage:\n"
        "    g = GeomFill.Gordon()\n"
        "    g.init(profiles, guides, tolerance)\n"
        "    g.perform()\n"
        "    if g.is_done():\n"
        "        surf = g.surface()")
        .def(py::init<>(), "Creates an empty Gordon surface algorithm.")
        .def("init",
             [](GeomFill_Gordon &self, const py::sequence &profiles,
                const py::sequence &guides, double tolerance) {
                 self.Init(curves_to_array(profiles, "profiles"),
                           curves_to_array(guides, "guides"), tolerance);
             },
             py::arg("profiles"), py::arg("guides"), py::arg("tolerance"),
             "Initializes the algorithm with profile and guide curves.\n\n"
             "profiles : sequence of >= 2 Geom.Curve (V-direction sections)\n"
             "guides   : sequence of >= 2 Geom.Curve (U-direction sections)\n"
             "tolerance: geometric tolerance for intersection detection")
        .def("perform", &GeomFill_Gordon::Perform,
             "Performs the Gordon surface construction.")
        .def("set_parallel_mode", &GeomFill_Gordon::SetParallelMode,
             py::arg("use_parallel"),
             "Enables/disables internal parallel processing (off by default).")
        .def("is_parallel_mode", &GeomFill_Gordon::IsParallelMode,
             "Returns True if internal parallel processing is enabled.")
        .def("is_done", &GeomFill_Gordon::IsDone,
             "Returns True if the surface was successfully constructed.")
        .def("surface", &GeomFill_Gordon::Surface,
             "Returns the resulting Gordon B-spline surface.");

    // Ergonomic one-call helper (extend/ layer): list[Curve] in, B-spline
    // surface out, raising on failure instead of returning a null handle.
    m.def("gordon_surface", &occt::extended::geometry::gordon_surface,
          py::arg("profiles"), py::arg("guides"), py::arg("tolerance"),
          py::arg("parallel") = false,
          "Build a Gordon surface from a network of intersecting profile and\n"
          "guide curves and return the resulting Geom.BSplineSurface.\n\n"
          "profiles : sequence of >= 2 Geom.Curve (V-direction sections)\n"
          "guides   : sequence of >= 2 Geom.Curve (U-direction sections)\n"
          "tolerance: geometric tolerance for intersection detection\n"
          "parallel : enable OCCT internal parallel processing (default False)\n\n"
          "Raises RuntimeError if the curve network is incompatible.\n"
          "New in OCCT 8.0.");
#endif
}
