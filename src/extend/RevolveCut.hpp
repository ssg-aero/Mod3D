#pragma once

#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Ax1.hxx>

namespace occt::extended::brep {

TopoDS_Shape revolve_cut(const TopoDS_Shape& shape,
                         const TopoDS_Face& profile,
                         const gp_Ax1& axis);

TopoDS_Shape revolve_cut(const TopoDS_Shape& shape,
                         const TopoDS_Face& profile,
                         const gp_Ax1& axis,
                         double angle);

TopoDS_Shape revolve_cut(const TopoDS_Shape& shape,
                         const TopoDS_Wire& profile,
                         const gp_Ax1& axis);

TopoDS_Shape revolve_cut(const TopoDS_Shape& shape,
                         const TopoDS_Wire& profile,
                         const gp_Ax1& axis,
                         double angle);

} // namespace occt::extended::brep