#include <pybind11/pybind11.h>

#include <TopAbs_ShapeEnum.hxx>

namespace py = pybind11;

void bind_top_abs(py::module_ &m)
{
    // TopAbs_ShapeEnum enum
    py::enum_<TopAbs_ShapeEnum>(m, "ShapeEnum",
        "Enumeration of topological shape types")
        .value("COMPOUND", TopAbs_COMPOUND, "Compound shape")
        .value("COMPSOLID", TopAbs_COMPSOLID, "Composite solid")
        .value("SOLID", TopAbs_SOLID, "Solid shape")
        .value("SHELL", TopAbs_SHELL, "Shell shape")
        .value("FACE", TopAbs_FACE, "Face shape")
        .value("WIRE", TopAbs_WIRE, "Wire shape")
        .value("EDGE", TopAbs_EDGE, "Edge shape")
        .value("VERTEX", TopAbs_VERTEX, "Vertex shape")
        .value("SHAPE", TopAbs_SHAPE, "Generic shape")
        .export_values();
}