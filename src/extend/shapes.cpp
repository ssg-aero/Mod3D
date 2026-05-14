#include "extend/Shapes.hpp"

#include <gp_Trsf.hxx>
#include <TopLoc_Location.hxx>

namespace occt::extended::shapes {

namespace {

TopLoc_Location location_from_translation(const gp_Vec& vector) {
    gp_Trsf transform;
    transform.SetTranslation(vector);
    return TopLoc_Location(transform);
}

TopLoc_Location location_from_rotation(const gp_Ax1& axis, double angle) {
    gp_Trsf transform;
    transform.SetRotation(axis, angle);
    return TopLoc_Location(transform);
}

TopLoc_Location location_from_scale(const gp_Pnt& center, double factor) {
    gp_Trsf transform;
    transform.SetScale(center, factor);
    return TopLoc_Location(transform);
}

} // namespace

void translate(TopoDS_Shape& shape, const gp_Vec& vector) {
    shape.Move(location_from_translation(vector));
}

TopoDS_Shape translated(const TopoDS_Shape& shape, const gp_Vec& vector) {
    return shape.Moved(location_from_translation(vector));
}

void rotate(TopoDS_Shape& shape, const gp_Ax1& axis, double angle) {
    shape.Move(location_from_rotation(axis, angle));
}

TopoDS_Shape rotated(const TopoDS_Shape& shape, const gp_Ax1& axis, double angle) {
    return shape.Moved(location_from_rotation(axis, angle));
}

void scale(TopoDS_Shape& shape, const gp_Pnt& center, double factor) {
    shape.Move(location_from_scale(center, factor));
}

TopoDS_Shape scaled(const TopoDS_Shape& shape, const gp_Pnt& center, double factor) {
    return shape.Moved(location_from_scale(center, factor));
}

Shape::Shape() : TopoDS_Shape() {}
Shape::Shape(const TopoDS_Shape& shape) : TopoDS_Shape(shape) {}
Shape::Shape(const TopoDS_Face& face) : TopoDS_Shape(face) {}
Shape::Shape(const TopoDS_Wire& wire) : TopoDS_Shape(wire) {}
Shape::Shape(const TopoDS_Edge& edge) : TopoDS_Shape(edge) {}
Shape::Shape(const TopoDS_Vertex& vertex) : TopoDS_Shape(vertex) {}

void Shape::translate(const gp_Vec& vector) {
    shapes::translate(*this, vector);
}

Shape Shape::translated(const gp_Vec& vector) const {
    return Shape(shapes::translated(*this, vector));
}

void Shape::rotate(const gp_Ax1& axis, double angle) {
    shapes::rotate(*this, axis, angle);
}

Shape Shape::rotated(const gp_Ax1& axis, double angle) const {
    return Shape(shapes::rotated(*this, axis, angle));
}

void Shape::scale(const gp_Pnt& center, double factor) {
    shapes::scale(*this, center, factor);
}

Shape Shape::scaled(const gp_Pnt& center, double factor) const {
    return Shape(shapes::scaled(*this, center, factor));
}

} // namespace occt::extended::shapes