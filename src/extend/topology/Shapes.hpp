#pragma once

#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Ax1.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>

namespace occt::extended::shapes {

void translate(TopoDS_Shape& shape, const gp_Vec& vector);
TopoDS_Shape translated(const TopoDS_Shape& shape, const gp_Vec& vector);

void rotate(TopoDS_Shape& shape, const gp_Ax1& axis, double angle);
TopoDS_Shape rotated(const TopoDS_Shape& shape, const gp_Ax1& axis, double angle);

void scale(TopoDS_Shape& shape, const gp_Pnt& center, double factor);
TopoDS_Shape scaled(const TopoDS_Shape& shape, const gp_Pnt& center, double factor);

class Shape : public TopoDS_Shape {
public:
    Shape();
    Shape(const TopoDS_Shape& shape);
    Shape(const TopoDS_Face& face);
    Shape(const TopoDS_Wire& wire);
    Shape(const TopoDS_Edge& edge);
    Shape(const TopoDS_Vertex& vertex);

    void translate(const gp_Vec& vector);
    Shape translated(const gp_Vec& vector) const;

    void rotate(const gp_Ax1& axis, double angle);
    Shape rotated(const gp_Ax1& axis, double angle) const;

    void scale(const gp_Pnt& center, double factor);
    Shape scaled(const gp_Pnt& center, double factor) const;
};

} // namespace occt::extended::shapes