#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Standard_Handle.hxx>

#include <Standard_Version.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

#if OCC_VERSION_HEX >= 0x080000

#include <GeomGridEval_Surface.hxx>
#include <GeomGridEval_Curve.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Curve.hxx>
#include <gp_Pnt.hxx>
#include <NCollection_Array1.hxx>
#include <NCollection_Array2.hxx>

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

void bind_geom_grid_eval(py::module_ &m) {
    // =========================================================================
    // GeomGridEval - fast batch evaluation of a curve/surface over a grid of
    //   parameters (variant-based dispatch, new in OCCT 8.0). evaluate_grid
    //   returns nested lists of gp.Pnt.
    // =========================================================================
    py::class_<GeomGridEval_Surface>(m, "Surface",
        "Batch grid evaluator for a Geom.Surface (new in OCCT 8.0).")
        .def(py::init<const opencascade::handle<Geom_Surface> &>(),
             py::arg("surface"))
        .def("evaluate_grid",
             [](const GeomGridEval_Surface &self,
                const std::vector<double> &u_params,
                const std::vector<double> &v_params) {
                 NCollection_Array2<gp_Pnt> grid = self.EvaluateGrid(
                     to_array(u_params, "u_params"),
                     to_array(v_params, "v_params"));
                 py::list rows;
                 for (int i = grid.LowerRow(); i <= grid.UpperRow(); ++i) {
                     py::list row;
                     for (int j = grid.LowerCol(); j <= grid.UpperCol(); ++j) {
                         row.append(grid.Value(i, j));
                     }
                     rows.append(std::move(row));
                 }
                 return rows;
             },
             py::arg("u_params"), py::arg("v_params"),
             "Evaluate the surface at every (u, v) in the parameter grid.\n"
             "Returns a list of rows (one per u) of gp.Pnt (one per v).");

    py::class_<GeomGridEval_Curve>(m, "Curve",
        "Batch grid evaluator for a Geom.Curve (new in OCCT 8.0).")
        .def(py::init<const opencascade::handle<Geom_Curve> &>(),
             py::arg("curve"))
        .def("evaluate_grid",
             [](const GeomGridEval_Curve &self,
                const std::vector<double> &params) {
                 NCollection_Array1<gp_Pnt> pts =
                     self.EvaluateGrid(to_array(params, "params"));
                 py::list out;
                 for (int i = pts.Lower(); i <= pts.Upper(); ++i) {
                     out.append(pts.Value(i));
                 }
                 return out;
             },
             py::arg("params"),
             "Evaluate the curve at every parameter. Returns a list of gp.Pnt.");
}

#else // OCC_VERSION_HEX < 0x080000

void bind_geom_grid_eval(py::module_ &) {}

#endif
