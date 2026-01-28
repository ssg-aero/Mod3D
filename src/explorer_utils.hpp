#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

namespace py = pybind11;

// C++20: Use non-type template parameter with if constexpr
template <TopAbs_ShapeEnum E>
auto get_shapes_of_type(const TopoDS_Shape& shape) {
    py::list shapes;
    for (TopExp_Explorer exp(shape, E); exp.More(); exp.Next())
    {
        if constexpr (E == TopAbs_FACE) {
            TopoDS_Face s = TopoDS::Face(exp.Current());
            shapes.append(s);
        } else if constexpr (E == TopAbs_EDGE) {
            TopoDS_Edge s = TopoDS::Edge(exp.Current());
            shapes.append(s);
        } else if constexpr (E == TopAbs_VERTEX) {
            TopoDS_Vertex s = TopoDS::Vertex(exp.Current());
            shapes.append(s);
        } else if constexpr (E == TopAbs_SOLID) {
            TopoDS_Solid s = TopoDS::Solid(exp.Current());
            shapes.append(s);
        }

    }
    return shapes;
}