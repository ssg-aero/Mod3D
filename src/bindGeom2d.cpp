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
    ;

    bind_geom2d_curves(m);
}