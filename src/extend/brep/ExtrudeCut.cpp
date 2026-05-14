#include "extend/brep/ExtrudeCut.hpp"

#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepBndLib.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepPrimAPI_MakeHalfSpace.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <Bnd_Box.hxx>
#include <gp_Pnt.hxx>

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

gp_Pnt reference_point(const TopoDS_Shape& profile) {
    Bnd_Box box;
    BRepBndLib::Add(profile, box);
    if (box.IsVoid()) {
        throw std::runtime_error("Cannot determine extrude cut profile reference point");
    }

    double xmin = 0.0;
    double ymin = 0.0;
    double zmin = 0.0;
    double xmax = 0.0;
    double ymax = 0.0;
    double zmax = 0.0;
    box.Get(xmin, ymin, zmin, xmax, ymax, zmax);
    return gp_Pnt(
        0.5 * (xmin + xmax),
        0.5 * (ymin + ymax),
        0.5 * (zmin + zmax)
    );
}

TopoDS_Shape limit_prism(const TopoDS_Shape& profile,
                         const gp_Vec& vector,
                         const TopoDS_Face& limiting_face) {
    const auto prism = BRepPrimAPI_MakePrism(profile, vector).Shape();
    const auto half_space = BRepPrimAPI_MakeHalfSpace(limiting_face, reference_point(profile)).Solid();

    BRepAlgoAPI_Common common_algo(prism, half_space);
    common_algo.Build();
    if (!common_algo.IsDone()) {
        throw std::runtime_error("Limited extrude cut tool construction failed");
    }
    return common_algo.Shape();
}

TopoDS_Shape limit_prism(const TopoDS_Shape& profile,
                         const gp_Vec& vector,
                         const gp_Pln& limiting_plane) {
    return limit_prism(profile, vector, BRepBuilderAPI_MakeFace(limiting_plane).Face());
}

} // namespace

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Face& profile,
                         const gp_Vec& vector) {
    return build_cut(shape, BRepPrimAPI_MakePrism(profile, vector).Shape());
}

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Face& profile,
                         const gp_Vec& vector,
                         const gp_Pln& limiting_plane) {
    return build_cut(shape, limit_prism(profile, vector, limiting_plane));
}

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Face& profile,
                         const gp_Vec& vector,
                         const TopoDS_Face& limiting_face) {
    return build_cut(shape, limit_prism(profile, vector, limiting_face));
}

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Wire& profile,
                         const gp_Vec& vector) {
    const auto face = BRepBuilderAPI_MakeFace(profile).Face();
    return extrude_cut(shape, face, vector);
}

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Wire& profile,
                         const gp_Vec& vector,
                         const gp_Pln& limiting_plane) {
    const auto face = BRepBuilderAPI_MakeFace(profile).Face();
    return extrude_cut(shape, face, vector, limiting_plane);
}

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Wire& profile,
                         const gp_Vec& vector,
                         const TopoDS_Face& limiting_face) {
    const auto face = BRepBuilderAPI_MakeFace(profile).Face();
    return extrude_cut(shape, face, vector, limiting_face);
}

} // namespace occt::extended::brep