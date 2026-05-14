#pragma once

#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Pln.hxx>
#include <gp_Vec.hxx>

namespace occt::extended::brep {

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Face& profile,
                         const gp_Vec& vector);

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Face& profile,
                         const gp_Vec& vector,
                         const gp_Pln& limiting_plane);

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Face& profile,
                         const gp_Vec& vector,
                         const TopoDS_Face& limiting_face);

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Wire& profile,
                         const gp_Vec& vector);

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Wire& profile,
                         const gp_Vec& vector,
                         const gp_Pln& limiting_plane);

TopoDS_Shape extrude_cut(const TopoDS_Shape& shape,
                         const TopoDS_Wire& profile,
                         const gp_Vec& vector,
                         const TopoDS_Face& limiting_face);

} // namespace occt::extended::brep