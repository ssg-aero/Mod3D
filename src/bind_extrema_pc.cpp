#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <Standard_Handle.hxx>

#include <Standard_Version.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

#if OCC_VERSION_HEX >= 0x080000

#include <ExtremaPC.hxx>
#include <ExtremaPC_Curve.hxx>
#include <Geom_Curve.hxx>
#include <gp_Pnt.hxx>

#include <cmath>

namespace {
// Convert an ExtremaPC::Result (non-copyable, internal storage) into a plain
// Python dict so callers never touch the OCCT result struct directly.
py::dict result_to_dict(const ExtremaPC::Result &res) {
    py::list extrema;
    for (int i = 0; i < res.NbExt(); ++i) {
        const ExtremaPC::ExtremumResult &e = res[i];
        py::dict d;
        d["parameter"] = e.Parameter;
        d["point"] = e.Point;
        d["square_distance"] = e.SquareDistance;
        d["distance"] = std::sqrt(e.SquareDistance);
        d["is_minimum"] = e.IsMinimum;
        extrema.append(std::move(d));
    }

    py::dict out;
    out["status"] = res.Status;
    out["is_done"] = res.IsDone();
    out["is_infinite"] = res.IsInfinite();
    out["extrema"] = std::move(extrema);
    out["min_index"] = res.MinIndex();
    out["min_distance"] =
        res.NbExt() > 0 ? std::sqrt(res.MinSquareDistance()) : py::none();
    out["max_distance"] =
        res.NbExt() > 0 ? py::cast(std::sqrt(res.MaxSquareDistance()))
                        : py::none();
    return out;
}
} // namespace

void bind_extrema_pc(py::module_ &m) {
    // =========================================================================
    // ExtremaPC - modern point-to-curve extrema (closest / farthest points).
    //   New in OCCT 8.0 (variant-based dispatch, analytic + BVH-numerical).
    //   Perform() is Pythonified to a dict; the OCCT Result struct (move-only,
    //   NCollection-backed) is not exposed directly.
    // =========================================================================
    py::enum_<ExtremaPC::Status>(m, "Status",
        "Status of an ExtremaPC computation.")
        .value("OK", ExtremaPC::Status::OK)
        .value("NotDone", ExtremaPC::Status::NotDone)
        .value("InfiniteSolutions", ExtremaPC::Status::InfiniteSolutions)
        .value("NoSolution", ExtremaPC::Status::NoSolution)
        .value("NumericalError", ExtremaPC::Status::NumericalError)
        .export_values();

    py::enum_<ExtremaPC::SearchMode>(m, "SearchMode",
        "Which extrema to search for (enables early-out optimizations).")
        .value("MinMax", ExtremaPC::SearchMode::MinMax)
        .value("Min", ExtremaPC::SearchMode::Min)
        .value("Max", ExtremaPC::SearchMode::Max)
        .export_values();

    py::class_<ExtremaPC_Curve>(m, "Curve",
        "Point-to-curve extrema dispatcher for a Geom.Curve (new in OCCT 8.0).\n"
        "Build once for a curve, then call perform() for many query points.")
        .def(py::init<const opencascade::handle<Geom_Curve> &>(),
             py::arg("curve"),
             "Build from a Geom.Curve (natural/unbounded parameter domain, or\n"
             "the trimmed bounds for a trimmed curve).")
        .def(py::init<const opencascade::handle<Geom_Curve> &, double, double>(),
             py::arg("curve"), py::arg("u_min"), py::arg("u_max"),
             "Build from a Geom.Curve restricted to [u_min, u_max].")
        .def("is_initialized", &ExtremaPC_Curve::IsInitialized,
             "True if the internal evaluator is ready.")
        .def("perform",
             [](const ExtremaPC_Curve &self, const gp_Pnt &p, double tol,
                ExtremaPC::SearchMode mode, bool include_endpoints) {
                 const ExtremaPC::Result &res =
                     include_endpoints
                         ? self.PerformWithEndpoints(p, tol, mode)
                         : self.Perform(p, tol, mode);
                 return result_to_dict(res);
             },
             py::arg("point"), py::arg("tol") = 1e-9,
             py::arg("mode") = ExtremaPC::SearchMode::MinMax,
             py::arg("include_endpoints") = false,
             "Compute extrema between `point` and the curve.\n\n"
             "Returns a dict with keys:\n"
             "  status            : ExtremaPC.Status\n"
             "  is_done           : bool (status == OK)\n"
             "  is_infinite       : bool (infinitely many solutions)\n"
             "  extrema           : list of dicts, each with parameter, point,\n"
             "                      distance, square_distance, is_minimum\n"
             "  min_index         : index of the closest extremum (-1 if none)\n"
             "  min_distance      : distance to the closest extremum (or None)\n"
             "  max_distance      : distance to the farthest extremum (or None)\n\n"
             "Set include_endpoints=True to also consider the curve endpoints.");
}

#else // OCC_VERSION_HEX < 0x080000

void bind_extrema_pc(py::module_ &) {}

#endif
