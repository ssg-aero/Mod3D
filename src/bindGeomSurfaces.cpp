#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "array_utils.hpp"

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_geom_surfaces(py::module_ &m)
{

}