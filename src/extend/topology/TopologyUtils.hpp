#pragma once

#include <TopAbs_ShapeEnum.hxx>
#include <TopoDS_Shape.hxx>

#include <vector>

namespace occt::extended::topology {

struct ShapeAncestors {
    TopoDS_Shape shape;
    std::vector<TopoDS_Shape> ancestors;
};

std::vector<ShapeAncestors>
map_shapes_and_ancestors(const TopoDS_Shape& shape,
                         TopAbs_ShapeEnum shape_type,
                         TopAbs_ShapeEnum ancestor_type);

std::vector<ShapeAncestors>
map_shapes_and_unique_ancestors(const TopoDS_Shape& shape,
                                TopAbs_ShapeEnum shape_type,
                                TopAbs_ShapeEnum ancestor_type,
                                bool use_orientation = false);

} // namespace occt::extended::topology