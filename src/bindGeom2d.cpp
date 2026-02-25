#include <pybind11/pybind11.h>


#include <Geom2d_Geometry.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

// Forward declarations
extern void bind_geom2d_curves(py::module_ &m);

void bind_geom2d(py::module_ &m)
{
    py::class_<Geom2d_Geometry, opencascade::handle<Geom2d_Geometry>>(m, "Geometry2d")
        // Hash and equality (identity-based)
        .def("__hash__", [](const opencascade::handle<Geom2d_Geometry>& self) {
            return std::hash<Geom2d_Geometry*>{}(self.get());
        }, "Returns hash value based on object identity")
        .def("__eq__", [](const opencascade::handle<Geom2d_Geometry>& self, 
                          const opencascade::handle<Geom2d_Geometry>& other) {
            return self.get() == other.get();
        }, py::arg("other"), "Returns True if both refer to the same geometry object")
    ;

    bind_geom2d_curves(m);
}