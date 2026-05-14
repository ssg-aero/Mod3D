#include "extend/TopologyUtils.hpp"

#include "extend/OcctContainers.hpp"

#include <TopExp.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>

namespace occt::extended::topology {

namespace {

std::vector<ShapeAncestors>
to_vector(const TopTools_IndexedDataMapOfShapeListOfShape& map) {
    std::vector<ShapeAncestors> result;
    result.reserve(static_cast<size_t>(map.Extent()));
    for (Standard_Integer i = 1; i <= map.Extent(); ++i) {
        result.push_back({map.FindKey(i), containers::to_vector(map.FindFromIndex(i))});
    }
    return result;
}

} // namespace

std::vector<ShapeAncestors>
map_shapes_and_ancestors(const TopoDS_Shape& shape,
                         TopAbs_ShapeEnum shape_type,
                         TopAbs_ShapeEnum ancestor_type) {
    TopTools_IndexedDataMapOfShapeListOfShape map;
    TopExp::MapShapesAndAncestors(shape, shape_type, ancestor_type, map);
    return to_vector(map);
}

std::vector<ShapeAncestors>
map_shapes_and_unique_ancestors(const TopoDS_Shape& shape,
                                TopAbs_ShapeEnum shape_type,
                                TopAbs_ShapeEnum ancestor_type,
                                bool use_orientation) {
    TopTools_IndexedDataMapOfShapeListOfShape map;
    TopExp::MapShapesAndUniqueAncestors(shape, shape_type, ancestor_type, map, use_orientation);
    return to_vector(map);
}

} // namespace occt::extended::topology