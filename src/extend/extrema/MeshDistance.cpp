#include "extend/extrema/MeshDistance.hpp"

#include "extend/render/TessellationUtils.hpp"

#include <BRep_Tool.hxx>
#include <BVH_Distance.hxx>
#include <BVH_Tools.hxx>
#include <NCollection_Vector.hxx>
#include <Poly_Triangulation.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopExp_Explorer.hxx>
#include <TopLoc_Location.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_TShape.hxx>
#include <gp_Trsf.hxx>

#include <cmath>
#include <limits>
#include <stdexcept>

namespace occt::extended::extrema {

namespace {

//! Point-to-triangle-set distance probe over a BRepExtrema_TriangleSet BVH.
class PointToTriangleSetDistance
    : public BVH_Distance<Standard_Real, 3, BVH_Vec3d, BRepExtrema_TriangleSet>
{
public:
    void reset(const BVH_Vec3d& query)
    {
        SetObject(query);
        myDistance = std::numeric_limits<Standard_Real>::max();
        myIsDone = Standard_False;
        myClosestPoint = query;
    }

    const BVH_Vec3d& closest_point() const { return myClosestPoint; }

    Standard_Boolean RejectNode(const BVH_Vec3d& corner_min,
                                const BVH_Vec3d& corner_max,
                                Standard_Real& metric) const Standard_OVERRIDE
    {
        metric = BVH_Tools<Standard_Real, 3>::PointBoxSquareDistance(
            myObject, corner_min, corner_max);
        return metric > myDistance;
    }

    Standard_Boolean Accept(const Standard_Integer triangle_index,
                            const Standard_Real&) Standard_OVERRIDE
    {
        BVH_Vec3d v0, v1, v2;
        myBVHSet->GetVertices(triangle_index, v0, v1, v2);
        const BVH_Vec3d projection = BVH_Tools<Standard_Real, 3>::PointTriangleProjection(
            myObject, v0, v1, v2);
        const BVH_Vec3d delta = projection - myObject;
        const Standard_Real square_distance = delta.Dot(delta);
        if (square_distance < myDistance) {
            myDistance = square_distance;
            myClosestPoint = projection;
            return Standard_True;
        }
        return Standard_False;
    }

private:
    BVH_Vec3d myClosestPoint;
};

BRepExtrema_ShapeList collect_faces(const TopoDS_Shape& shape)
{
    BRepExtrema_ShapeList faces;
    for (TopExp_Explorer explorer(shape, TopAbs_FACE); explorer.More(); explorer.Next()) {
        faces.Append(explorer.Current());
    }
    return faces;
}

Standard_Boolean shape_has_triangulation(const TopoDS_Shape& shape)
{
    Standard_Boolean any_face = Standard_False;
    for (TopExp_Explorer explorer(shape, TopAbs_FACE); explorer.More(); explorer.Next()) {
        any_face = Standard_True;
        TopLoc_Location location;
        const Handle(Poly_Triangulation) triangulation =
            BRep_Tool::Triangulation(TopoDS::Face(explorer.Current()), location);
        if (triangulation.IsNull()) {
            return Standard_False;
        }
    }
    return any_face;
}

} // namespace

MeshDistance::MeshDistance(const TopoDS_Shape& reference,
                           Standard_Real deflection,
                           Standard_Real angle_deflection_deg,
                           Standard_Boolean parallel)
{
    set_reference(reference, deflection, angle_deflection_deg, parallel);
}

void MeshDistance::set_reference(const TopoDS_Shape& reference,
                                 Standard_Real deflection,
                                 Standard_Real angle_deflection_deg,
                                 Standard_Boolean parallel)
{
    if (deflection <= 0.0) {
        throw std::invalid_argument("MeshDistance: deflection must be positive");
    }
    myReference = reference;
    myDeflection = deflection;
    myAngleDeflectionDeg = angle_deflection_deg;
    myParallel = parallel;

    render::build_mesh(reference, deflection, /*is_relative=*/false,
                       angle_deflection_deg, parallel);

    const BRepExtrema_ShapeList faces = collect_faces(reference);
    if (faces.IsEmpty()) {
        throw std::invalid_argument("MeshDistance: reference shape contains no faces");
    }

    myTriangleSet = new BRepExtrema_TriangleSet();
    if (!myTriangleSet->Init(faces) || myTriangleSet->Size() == 0) {
        myTriangleSet.Nullify();
        throw std::runtime_error(
            "MeshDistance: failed to build BVH for reference (no triangles)");
    }

    // The new reference may have a different deflection, so any previous
    // query-side tessellation memo is stale.
    myTessellatedQueries.clear();
}

Standard_Integer MeshDistance::nb_reference_triangles() const
{
    return myTriangleSet.IsNull() ? 0 : myTriangleSet->Size();
}

void MeshDistance::clear_query_cache()
{
    myTessellatedQueries.clear();
}

Standard_Integer MeshDistance::nb_cached_queries() const
{
    return static_cast<Standard_Integer>(myTessellatedQueries.size());
}

MeshDistanceResult MeshDistance::distance_to(const TopoDS_Shape& shape) const
{
    if (myTriangleSet.IsNull() || myTriangleSet->Size() == 0) {
        throw std::runtime_error("MeshDistance: reference not initialized");
    }

    // Fast path: we've already meshed this exact TShape at the current
    // deflection. The Handle stored in the cache keeps the TShape alive so
    // its pointer-identity is stable (otherwise the pointer could be reused
    // after garbage collection and yield false positives).
    //
    // Profiling note: BRepMesh_IncrementalMesh dominates per-call cost when
    // the query shape is fresh. shape_has_triangulation() is cheap but scales
    // with the face count, so on large compound queries the cache lookup
    // still helps.
    Handle(TopoDS_TShape) query_tshape = shape.TShape();
    bool cache_hit = !query_tshape.IsNull()
                   && myTessellatedQueries.count(query_tshape) > 0;

    // Defensive: even on cache hit the triangulation could have been cleared
    // externally (e.g. BRepTools::Clean by user code) since we last meshed.
    // Probe the first face only — cheap, and a stale entry would otherwise
    // surface as a confusing "no triangulation vertices" error below.
    if (cache_hit) {
        TopExp_Explorer first(shape, TopAbs_FACE);
        if (first.More()) {
            TopLoc_Location loc;
            if (BRep_Tool::Triangulation(TopoDS::Face(first.Current()), loc).IsNull()) {
                myTessellatedQueries.erase(query_tshape);
                cache_hit = false;
            }
        }
    }

    if (!cache_hit) {
        if (!shape_has_triangulation(shape)) {
            render::build_mesh(shape, myDeflection, /*is_relative=*/false,
                               myAngleDeflectionDeg, myParallel);
        }
        if (!query_tshape.IsNull()) {
            myTessellatedQueries.insert(query_tshape);
        }
    }

    PointToTriangleSetDistance probe;
    probe.SetBVHSet(myTriangleSet.get());

    MeshDistanceResult result{
        std::numeric_limits<Standard_Real>::max(),
        gp_Pnt(),
        gp_Pnt()
    };

    Standard_Boolean any_query_node = Standard_False;

    for (TopExp_Explorer explorer(shape, TopAbs_FACE); explorer.More(); explorer.Next()) {
        const TopoDS_Face& face = TopoDS::Face(explorer.Current());
        TopLoc_Location location;
        const Handle(Poly_Triangulation) triangulation =
            BRep_Tool::Triangulation(face, location);
        if (triangulation.IsNull()) {
            continue;
        }
        const gp_Trsf transform = location.Transformation();
        const Standard_Boolean apply_transform = !location.IsIdentity();
        const Standard_Integer node_count = triangulation->NbNodes();
        for (Standard_Integer i = 1; i <= node_count; ++i) {
            gp_Pnt node = triangulation->Node(i);
            if (apply_transform) {
                node.Transform(transform);
            }
            const BVH_Vec3d query(node.X(), node.Y(), node.Z());
            probe.reset(query);
            probe.ComputeDistance();
            any_query_node = Standard_True;
            if (probe.IsDone() && probe.Distance() < result.distance) {
                result.distance = probe.Distance();
                const BVH_Vec3d& cp = probe.closest_point();
                result.point_on_reference = gp_Pnt(cp.x(), cp.y(), cp.z());
                result.point_on_shape = node;
            }
        }
    }

    if (!any_query_node) {
        throw std::runtime_error(
            "MeshDistance: query shape produced no triangulation vertices");
    }

    result.distance = std::sqrt(result.distance);
    return result;
}

} // namespace occt::extended::extrema
