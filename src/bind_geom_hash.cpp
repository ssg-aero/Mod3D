#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>

#include <Standard_Version.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

#if OCC_VERSION_HEX >= 0x080000

#include <GeomHash_SurfaceHasher.hxx>
#include <GeomHash_CurveHasher.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Curve.hxx>
#include <Precision.hxx>

void bind_geom_hash(py::module_ &m) {
    // =========================================================================
    // GeomHash - polymorphic hashing / fuzzy equality for Geom_* geometry,
    //   for deduplication. New in OCCT 8.0.
    // =========================================================================
    py::class_<GeomHash_SurfaceHasher>(m, "SurfaceHasher",
        "Polymorphic hasher and fuzzy-equality test for Geom.Surface\n"
        "(new in OCCT 8.0). Useful for geometry deduplication.")
        .def(py::init<double, double>(),
             py::arg("comp_tolerance") = Precision::Angular(),
             py::arg("hash_tolerance") = Precision::Confusion())
        .def("hash",
             [](const GeomHash_SurfaceHasher &self,
                const opencascade::handle<Geom_Surface> &s) {
                 return self(s);
             },
             py::arg("surface"),
             "Hash a surface (dispatches on the concrete surface type).")
        .def("equal",
             [](const GeomHash_SurfaceHasher &self,
                const opencascade::handle<Geom_Surface> &s1,
                const opencascade::handle<Geom_Surface> &s2) {
                 return self(s1, s2);
             },
             py::arg("surface1"), py::arg("surface2"),
             "True if two surfaces are equal within tolerance.");

    py::class_<GeomHash_CurveHasher>(m, "CurveHasher",
        "Polymorphic hasher and fuzzy-equality test for Geom.Curve\n"
        "(new in OCCT 8.0). Useful for geometry deduplication.")
        .def(py::init<double, double>(),
             py::arg("comp_tolerance") = Precision::Angular(),
             py::arg("hash_tolerance") = Precision::Confusion())
        .def("hash",
             [](const GeomHash_CurveHasher &self,
                const opencascade::handle<Geom_Curve> &c) {
                 return self(c);
             },
             py::arg("curve"),
             "Hash a curve (dispatches on the concrete curve type).")
        .def("equal",
             [](const GeomHash_CurveHasher &self,
                const opencascade::handle<Geom_Curve> &c1,
                const opencascade::handle<Geom_Curve> &c2) {
                 return self(c1, c2);
             },
             py::arg("curve1"), py::arg("curve2"),
             "True if two curves are equal within tolerance.");
}

#else // OCC_VERSION_HEX < 0x080000

void bind_geom_hash(py::module_ &) {}

#endif
