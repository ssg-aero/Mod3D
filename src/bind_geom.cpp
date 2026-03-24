#include <pybind11/pybind11.h>
// #include <pybind11/operators.h>
#include <pybind11/numpy.h>

#include <Standard_Handle.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Vec.hxx>
#include <gp_Lin.hxx>
#include <gp_Trsf.hxx>
#include <Geom_Geometry.hxx>
#include <GeomAbs_Shape.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

// Forward declarations
void bind_geom_curves(py::module_ &m);
void bind_geom_surfaces(py::module_ &m);

void bind_geom(py::module_ &m)
{

    py::class_<Geom_Geometry, opencascade::handle<Geom_Geometry>>(m, "Geometry")
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&Geom_Geometry::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&Geom_Geometry::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&Geom_Geometry::Mirror), py::arg("a2"))
        .def("rotate", &Geom_Geometry::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &Geom_Geometry::Scale, py::arg("p"), py::arg("s"))
        .def("translate", py::overload_cast<const gp_Vec&>(&Geom_Geometry::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&Geom_Geometry::Translate), 
             py::arg("p1"), py::arg("p2"))
        .def("transform", &Geom_Geometry::Transform, py::arg("t"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&Geom_Geometry::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&Geom_Geometry::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&Geom_Geometry::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &Geom_Geometry::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &Geom_Geometry::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &Geom_Geometry::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&Geom_Geometry::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&Geom_Geometry::Translated, py::const_), 
             py::arg("p1"), py::arg("p2"))
        
        // Copy
        .def("copy", &Geom_Geometry::Copy)
        
        // Debug
        .def("dump_json", &Geom_Geometry::DumpJson, py::arg("stream"), py::arg("depth") = -1)
        
        // Hash and equality (identity-based)
        .def("__hash__", [](const opencascade::handle<Geom_Geometry>& self) {
            return std::hash<Geom_Geometry*>{}(self.get());
        }, "Returns hash value based on object identity")
        .def("__eq__", [](const opencascade::handle<Geom_Geometry>& self, 
                          const opencascade::handle<Geom_Geometry>& other) {
            return self.get() == other.get();
        }, py::arg("other"), "Returns True if both refer to the same geometry object")
    ;

    bind_geom_curves(m);
    bind_geom_surfaces(m);

}
