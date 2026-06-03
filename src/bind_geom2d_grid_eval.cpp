#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Standard_Handle.hxx>

#include <Standard_Version.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

#if OCC_VERSION_HEX >= 0x080000

#include <Geom2dGridEval_Curve.hxx>
#include <Geom2d_Curve.hxx>
#include <gp_Pnt2d.hxx>
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

void bind_geom2d_grid_eval(py::module_ &m) {
    // =========================================================================
    // Geom2dGridEval - batch grid evaluation of a Geom2d.Curve (the 2D
    //   counterpart of GeomGridEval, new in OCCT 8.0). evaluate_grid returns a
    //   list of gp.Pnt2d.
    // =========================================================================
    py::class_<Geom2dGridEval_Curve>(m, "Curve",
        "Batch grid evaluator for a Geom2d.Curve (new in OCCT 8.0).")
        .def(py::init<const opencascade::handle<Geom2d_Curve> &>(),
             py::arg("curve"))
        .def("evaluate_grid",
             [](const Geom2dGridEval_Curve &self,
                const std::vector<double> &params) {
                 NCollection_Array1<gp_Pnt2d> pts =
                     self.EvaluateGrid(to_array(params, "params"));
                 py::list out;
                 for (int i = pts.Lower(); i <= pts.Upper(); ++i) {
                     out.append(pts.Value(i));
                 }
                 return out;
             },
             py::arg("params"),
             "Evaluate the curve at every parameter. Returns a list of "
             "gp.Pnt2d.");
}

#else // OCC_VERSION_HEX < 0x080000

void bind_geom2d_grid_eval(py::module_ &) {}

#endif
