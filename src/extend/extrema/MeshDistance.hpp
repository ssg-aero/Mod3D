#pragma once

#include <BRepExtrema_TriangleSet.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Real.hxx>
#include <Standard_Boolean.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>

namespace occt::extended::extrema {

struct MeshDistanceResult {
    Standard_Real distance;
    gp_Pnt point_on_reference;
    gp_Pnt point_on_shape;
};

//! Approximate minimum-distance query against a fixed reference shape, using a
//! BVH built once over the reference's triangulation.
//!
//! Intended for "same reference, many queries" workflows. Build cost is paid
//! once (mesh + BVH); each `distance_to` is a single-tree BVH descent per
//! query-shape vertex.
//!
//! Accuracy is bounded by the tessellation deflection — use this when an
//! approximate result is acceptable. For exact NURBS distance, use
//! `BRepExtrema_DistShapeShape`.
class MeshDistance {
public:
    MeshDistance(const TopoDS_Shape& reference,
                 Standard_Real deflection,
                 Standard_Real angle_deflection_deg = 30.0,
                 Standard_Boolean parallel = Standard_True);

    void set_reference(const TopoDS_Shape& reference,
                       Standard_Real deflection,
                       Standard_Real angle_deflection_deg = 30.0,
                       Standard_Boolean parallel = Standard_True);

    //! Tessellates the input shape (at the current deflection) if needed,
    //! then queries each of its triangulation vertices against the reference
    //! BVH. Returns the minimum distance and the supporting points.
    MeshDistanceResult distance_to(const TopoDS_Shape& shape) const;

    const TopoDS_Shape& reference() const { return myReference; }
    Standard_Real deflection() const { return myDeflection; }
    Standard_Integer nb_reference_triangles() const;

private:
    TopoDS_Shape myReference;
    Standard_Real myDeflection;
    Standard_Real myAngleDeflectionDeg;
    Standard_Boolean myParallel;
    Handle(BRepExtrema_TriangleSet) myTriangleSet;
};

} // namespace occt::extended::extrema
