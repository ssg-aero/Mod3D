#pragma once

#include <BRepExtrema_TriangleSet.hxx>
#include <Standard_Handle.hxx>
#include <Standard_Real.hxx>
#include <Standard_Boolean.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_TShape.hxx>
#include <gp_Pnt.hxx>

#include <cstddef>
#include <unordered_set>

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

    //! Drop the memo of query shapes already tessellated at the current
    //! deflection. Releases the Handle refcounts held on those TShapes —
    //! important in loops that create a fresh query shape every iteration,
    //! otherwise memory grows linearly with the number of distinct queries.
    void clear_query_cache();

    //! Number of distinct query TShapes currently cached as tessellated.
    Standard_Integer nb_cached_queries() const;

private:
    //! Hash + equality for the cache: keyed by the underlying TShape pointer.
    //! The Handle is what we store so the TShape is kept alive (refcounted)
    //! while we cache it — otherwise the pointer could be reused after the
    //! original shape is collected, causing false positives.
    struct TShapeHandleHash {
        std::size_t operator()(const Handle(TopoDS_TShape)& h) const noexcept {
            return std::hash<const void*>{}(h.get());
        }
    };
    struct TShapeHandleEqual {
        bool operator()(const Handle(TopoDS_TShape)& a,
                        const Handle(TopoDS_TShape)& b) const noexcept {
            return a.get() == b.get();
        }
    };

    TopoDS_Shape myReference;
    Standard_Real myDeflection;
    Standard_Real myAngleDeflectionDeg;
    Standard_Boolean myParallel;
    Handle(BRepExtrema_TriangleSet) myTriangleSet;
    mutable std::unordered_set<Handle(TopoDS_TShape),
                               TShapeHandleHash,
                               TShapeHandleEqual> myTessellatedQueries;
};

} // namespace occt::extended::extrema
