#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>

namespace py = pybind11;

void bind_shapes(py::module &m);


void bind_topods(py::module_ &m)
{

    bind_shapes(m);
    
}