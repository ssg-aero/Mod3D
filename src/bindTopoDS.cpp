#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>

namespace py = pybind11;

void bind_shapes(py::module &m);

PYBIND11_MODULE(TopoDS, m)
{

    bind_shapes(m);
}