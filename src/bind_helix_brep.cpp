#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Standard_Handle.hxx>

#include <Standard_Version.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

#if OCC_VERSION_HEX >= 0x080000

#include <HelixBRep_BuilderHelix.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Ax3.hxx>
#include <GeomAbs_Shape.hxx>
#include <NCollection_Array1.hxx>

#include <vector>

namespace {
NCollection_Array1<double> to_array(const std::vector<double> &v,
                                    const char *name) {
    if (v.empty()) {
        throw py::value_error(std::string(name) + " must not be empty");
    }
    NCollection_Array1<double> a(1, static_cast<int>(v.size()));
    for (std::size_t i = 0; i < v.size(); ++i) {
        a.SetValue(static_cast<int>(i) + 1, v[i]);
    }
    return a;
}
} // namespace

void bind_helix_brep(py::module_ &m) {
    // =========================================================================
    // HelixBRep - build a helix / spiral as a BRep wire. New in OCCT 8.0.
    //   Per-segment parameters are passed as parallel lists (one entry per
    //   coil/segment). Useful for threads, springs and turbomachinery.
    // =========================================================================
    py::class_<HelixBRep_BuilderHelix>(m, "BuilderHelix",
        "Builds a helix or spiral as a TopoDS wire (new in OCCT 8.0).\n\n"
        "Workflow:\n"
        "    b = HelixBRep.BuilderHelix()\n"
        "    b.set_pure_helix(axis, diameter, pitches=[p], nb_turns=[n])\n"
        "    b.perform()\n"
        "    if b.error_status() == 0:\n"
        "        wire = b.shape()")
        .def(py::init<>())
        .def("set_pure_helix",
             [](HelixBRep_BuilderHelix &self, const gp_Ax3 &axis, double diameter,
                const std::vector<double> &pitches,
                const std::vector<double> &nb_turns) {
                 self.SetParameters(axis, diameter, to_array(pitches, "pitches"),
                                    to_array(nb_turns, "nb_turns"));
             },
             py::arg("axis"), py::arg("diameter"), py::arg("pitches"),
             py::arg("nb_turns"),
             "Pure (constant-diameter) helix: one (pitch, nb_turns) pair per "
             "coil segment.")
        .def("set_pure_spiral",
             [](HelixBRep_BuilderHelix &self, const gp_Ax3 &axis, double diameter1,
                double diameter2, const std::vector<double> &pitches,
                const std::vector<double> &nb_turns) {
                 self.SetParameters(axis, diameter1, diameter2,
                                    to_array(pitches, "pitches"),
                                    to_array(nb_turns, "nb_turns"));
             },
             py::arg("axis"), py::arg("diameter1"), py::arg("diameter2"),
             py::arg("pitches"), py::arg("nb_turns"),
             "Pure spiral tapering from diameter1 to diameter2.")
        .def("set_composite_helix",
             [](HelixBRep_BuilderHelix &self, const gp_Ax3 &axis,
                const std::vector<double> &diameters,
                const std::vector<double> &pitches,
                const std::vector<double> &nb_turns) {
                 self.SetParameters(axis, to_array(diameters, "diameters"),
                                    to_array(pitches, "pitches"),
                                    to_array(nb_turns, "nb_turns"));
             },
             py::arg("axis"), py::arg("diameters"), py::arg("pitches"),
             py::arg("nb_turns"),
             "General composite helix: per-segment diameter, pitch and turns.")
        .def("set_approx_parameters", &HelixBRep_BuilderHelix::SetApproxParameters,
             py::arg("tolerance"), py::arg("max_degree"), py::arg("continuity"),
             "Approximation tolerance, max B-spline degree and continuity "
             "(GeomAbs.Shape).")
        .def("perform", &HelixBRep_BuilderHelix::Perform,
             "Run the construction.")
        .def("tolerance_reached", &HelixBRep_BuilderHelix::ToleranceReached,
             "Tolerance actually reached by the approximation.")
        .def("error_status", &HelixBRep_BuilderHelix::ErrorStatus,
             "Error status (0 = success).")
        .def("warning_status", &HelixBRep_BuilderHelix::WarningStatus,
             "Warning status (0 = none).")
        .def("shape", &HelixBRep_BuilderHelix::Shape,
             "The resulting helix wire (TopoDS.Shape).");
}

#else // OCC_VERSION_HEX < 0x080000

void bind_helix_brep(py::module_ &) {}

#endif
