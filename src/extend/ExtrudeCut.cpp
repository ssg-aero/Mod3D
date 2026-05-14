#include "extend/ExtrudeCut.hpp"

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

#include <stdexcept>

namespace occt::extended::brep {

namespace {

TopoDS_Shape build_cut(const TopoDS_Shape& shape, const TopoDS_Shape& tool) {
    BRepAlgoAPI_Cut cut_algo(shape, tool);
    cut_algo.Build();
    if (!cut_algo.IsDone()) {
        throw std::runtime_error("Extrude cut operation failed");
    }
    return cut_algo.Shape();
}

} // namespace

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Face& profile,
                         const gp_Vec& vector) {
    return build_cut(shape, BRepPrimAPI_MakePrism(profile, vector).Shape());
}

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Wire& profile,
                         const gp_Vec& vector) {
    const auto face = BRepBuilderAPI_MakeFace(profile).Face();
    return extrude_cut(shape, face, vector);
}

} // namespace occt::extended::brep