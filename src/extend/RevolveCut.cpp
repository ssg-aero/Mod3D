#include "extend/RevolveCut.hpp"

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>

#include <stdexcept>

namespace occt::extended::brep {

namespace {

TopoDS_Shape build_cut(const TopoDS_Shape& shape, const TopoDS_Shape& tool) {
    BRepAlgoAPI_Cut cut_algo(shape, tool);
    cut_algo.Build();
    if (!cut_algo.IsDone()) {
        throw std::runtime_error("Revolve cut operation failed");
    }
    return cut_algo.Shape();
}

} // namespace

TopoDS_Shape revolve_cut(const TopoDS_Shape& shape,
                         const TopoDS_Face& profile,
                         const gp_Ax1& axis) {
    return build_cut(shape, BRepPrimAPI_MakeRevol(profile, axis).Shape());
}

TopoDS_Shape revolve_cut(const TopoDS_Shape& shape,
                         const TopoDS_Face& profile,
                         const gp_Ax1& axis,
                         double angle) {
    return build_cut(shape, BRepPrimAPI_MakeRevol(profile, axis, angle).Shape());
}

TopoDS_Shape revolve_cut(const TopoDS_Shape& shape,
                         const TopoDS_Wire& profile,
                         const gp_Ax1& axis) {
    const auto face = BRepBuilderAPI_MakeFace(profile).Face();
    return revolve_cut(shape, face, axis);
}

TopoDS_Shape revolve_cut(const TopoDS_Shape& shape,
                         const TopoDS_Wire& profile,
                         const gp_Ax1& axis,
                         double angle) {
    const auto face = BRepBuilderAPI_MakeFace(profile).Face();
    return revolve_cut(shape, face, axis, angle);
}

} // namespace occt::extended::brep