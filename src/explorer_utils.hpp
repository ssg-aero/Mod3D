#include <TopoDS_Compound.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopoDS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>

#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>

namespace py = pybind11;

// Downcast a TopoDS_Shape to the proper Python subtype based on its ShapeType
inline py::object downcast_shape(const TopoDS_Shape& shape) {
    switch (shape.ShapeType()) {
        case TopAbs_COMPOUND:
            return py::cast(TopoDS::Compound(shape));
        case TopAbs_COMPSOLID:
            return py::cast(TopoDS::CompSolid(shape));
        case TopAbs_SOLID:
            return py::cast(TopoDS::Solid(shape));
        case TopAbs_SHELL:
            return py::cast(TopoDS::Shell(shape));
        case TopAbs_FACE:
            return py::cast(TopoDS::Face(shape));
        case TopAbs_WIRE:
            return py::cast(TopoDS::Wire(shape));
        case TopAbs_EDGE:
            return py::cast(TopoDS::Edge(shape));
        case TopAbs_VERTEX:
            return py::cast(TopoDS::Vertex(shape));
        default:
            return py::cast(shape);
    }
}

// C++20: Use non-type template parameter with if constexpr
template <TopAbs_ShapeEnum E>
auto get_shapes_of_type(const TopoDS_Shape& shape) {
    // Use TopExp::MapShapes for unique shapes (avoids duplicates, e.g. shared vertices)
    TopTools_IndexedMapOfShape map;
    TopExp::MapShapes(shape, E, map);
    py::list shapes;
    for (int i = 1; i <= map.Extent(); ++i)
    {
        if constexpr (E == TopAbs_FACE) {
            shapes.append(TopoDS::Face(map(i)));
        } else if constexpr (E == TopAbs_EDGE) {
            shapes.append(TopoDS::Edge(map(i)));
        } else if constexpr (E == TopAbs_VERTEX) {
            shapes.append(TopoDS::Vertex(map(i)));
        } else if constexpr (E == TopAbs_SOLID) {
            shapes.append(TopoDS::Solid(map(i)));
        }
    }
    return shapes;
}