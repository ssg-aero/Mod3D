#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>

#include <Standard_Version.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

#if OCC_VERSION_HEX >= 0x080000

#include <GeomBndLib_Surface.hxx>
#include <Geom_Surface.hxx>
#include <Bnd_Box.hxx>
// NOTE: GeomBndLib_Curve is intentionally NOT bound. Its header transitively
// includes GeomBndLib_Line.hxx, which pulls the private implementation header
// GeomBndLib_InfiniteHelpers.pxx -- not shipped by the conda-forge OCCT 8.0.0
// package, so it cannot be compiled against. The surface dispatcher headers do
// not have this problem. Re-add the Curve binding once the upstream feedstock
// installs the .pxx (or stops including it from a public header).

namespace {
// Bnd_Box is not exposed as a Python type, so report boxes as
// (xmin, ymin, zmin, xmax, ymax, zmax) tuples (None when void).
py::object box_to_tuple(const Bnd_Box &box) {
    if (box.IsVoid()) {
        return py::none();
    }
    double xmin, ymin, zmin, xmax, ymax, zmax;
    box.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    return py::make_tuple(xmin, ymin, zmin, xmax, ymax, zmax);
}
} // namespace

void bind_geom_bndlib(py::module_ &m) {
    // =========================================================================
    // GeomBndLib - bounding boxes computed directly from Geom_* geometry.
    //   New variant-based dispatchers in OCCT 8.0 (auto-detect the geometry
    //   type). Boxes are returned as (xmin, ymin, zmin, xmax, ymax, zmax)
    //   tuples, or None for an empty box.
    // =========================================================================
    py::class_<GeomBndLib_Surface>(m, "Surface",
        "Bounding-box dispatcher for a Geom.Surface (new in OCCT 8.0).")
        .def(py::init<const opencascade::handle<Geom_Surface> &>(),
             py::arg("surface"))
        .def("box",
             [](const GeomBndLib_Surface &self, double tol) {
                 return box_to_tuple(self.Box(tol));
             },
             py::arg("tol"),
             "Bounding box of the whole surface within tolerance `tol`.")
        .def("box",
             [](const GeomBndLib_Surface &self, double u_min, double u_max,
                double v_min, double v_max, double tol) {
                 return box_to_tuple(self.Box(u_min, u_max, v_min, v_max, tol));
             },
             py::arg("u_min"), py::arg("u_max"), py::arg("v_min"),
             py::arg("v_max"), py::arg("tol"),
             "Bounding box of the patch [u_min, u_max] x [v_min, v_max].")
        .def("box_optimal",
             [](const GeomBndLib_Surface &self, double tol) {
                 return box_to_tuple(self.BoxOptimal(tol));
             },
             py::arg("tol"),
             "Tighter (optimal) bounding box of the whole surface.");
}

#else // OCC_VERSION_HEX < 0x080000

void bind_geom_bndlib(py::module_ &) {}

#endif
