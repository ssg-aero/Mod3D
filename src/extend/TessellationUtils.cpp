#include "TessellationUtils.hpp"

#include <BRepAdaptor_CompCurve.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepMesh_IncrementalMesh.hxx>
#include <BRepTools.hxx>
#include <BRep_Tool.hxx>
#include <GCPnts_UniformDeflection.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <Geom_Curve.hxx>
#include <Poly_Triangulation.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <TopLoc_Location.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Pnt.hxx>
#include <gp_Trsf.hxx>

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace occt::extended::render {

namespace {

constexpr double degrees_to_radians(double angle)
{
    return angle * std::acos(-1.0) / 180.0;
}

FaceTessellation extract_face_tessellation(const TopoDS_Face& face, bool compute_normals)
{
    TopLoc_Location location;
    Handle(Poly_Triangulation) triangulation = BRep_Tool::Triangulation(face, location);

    if (triangulation.IsNull()) {
        return {};
    }

    if (compute_normals) {
        triangulation->ComputeNormals();
    }

    const int node_count = triangulation->NbNodes();
    const int triangle_count = triangulation->NbTriangles();
    const bool has_normals = triangulation->HasNormals();
    const bool has_uvs = triangulation->HasUVNodes();
    const bool reverse_orientation = (face.Orientation() == TopAbs_INTERNAL);

    FaceTessellation result;
    result.triangles.resize(static_cast<std::size_t>(triangle_count) * 3);
    result.vertices.resize(static_cast<std::size_t>(node_count) * 3);
    if (has_normals) {
        result.normals.emplace(static_cast<std::size_t>(node_count) * 3);
    }
    if (has_uvs) {
        result.uvs.emplace(static_cast<std::size_t>(node_count) * 2);
    }

    const auto vertex_transform = location.Transformation();
    for (int i = 0; i < node_count; ++i) {
        const auto point = triangulation->Node(i + 1).Transformed(vertex_transform);
        const auto index = static_cast<std::size_t>(i) * 3;
        result.vertices[index] = point.X();
        result.vertices[index + 1] = point.Y();
        result.vertices[index + 2] = point.Z();
    }

    if (result.normals) {
        auto& normals = *result.normals;
        for (int i = 0; i < node_count; ++i) {
            auto normal = triangulation->Normal(i + 1);
            if (reverse_orientation) {
                normal.Reverse();
            }
            const auto index = static_cast<std::size_t>(i) * 3;
            normals[index] = normal.X();
            normals[index + 1] = normal.Y();
            normals[index + 2] = normal.Z();
        }
    }

    if (result.uvs) {
        auto& uvs = *result.uvs;
        for (int i = 0; i < node_count; ++i) {
            const auto uv = triangulation->UVNode(i + 1);
            const auto index = static_cast<std::size_t>(i) * 2;
            uvs[index] = uv.X();
            uvs[index + 1] = uv.Y();
        }
    }

    for (int i = 0; i < triangle_count; ++i) {
        int node1 = 0;
        int node2 = 0;
        int node3 = 0;
        triangulation->Triangle(i + 1).Get(node1, node2, node3);
        const auto index = static_cast<std::size_t>(i) * 3;
        result.triangles[index] = node1 - 1;
        result.triangles[index + 1] = node2 - 1;
        result.triangles[index + 2] = node3 - 1;
    }

    return result;
}

EdgeTessellation extract_edge_tessellation(
    const TopoDS_Edge& edge,
    const TopTools_IndexedDataMapOfShapeListOfShape& edge_face_map)
{
    TopLoc_Location location;
    gp_Trsf transform;
    EdgeTessellation result;

    const auto polygon_3d = BRep_Tool::Polygon3D(edge, location);
    if (!polygon_3d.IsNull()) {
        if (!location.IsIdentity()) {
            transform = location.Transformation();
        }

        const auto& nodes = polygon_3d->Nodes();
        const auto node_count = polygon_3d->NbNodes();
        result.vertices.resize(static_cast<std::size_t>(node_count) * 3);
        result.indices.resize(static_cast<std::size_t>(node_count));

        for (int i = 0; i < node_count; ++i) {
            auto vertex = nodes(i + 1);
            vertex.Transform(transform);
            const auto index = static_cast<std::size_t>(i) * 3;
            result.vertices[index] = vertex.X();
            result.vertices[index + 1] = vertex.Y();
            result.vertices[index + 2] = vertex.Z();
            result.indices[static_cast<std::size_t>(i)] = i;
        }
        return result;
    }

    const auto& first_face = TopoDS::Face(edge_face_map.FindFromKey(edge).First());
    const auto face_triangulation = BRep_Tool::Triangulation(first_face, location);
    if (face_triangulation.IsNull()) {
        return result;
    }

    const auto polygon_on_triangulation = BRep_Tool::PolygonOnTriangulation(edge, face_triangulation, location);
    if (polygon_on_triangulation.IsNull()) {
        return result;
    }

    if (!location.IsIdentity()) {
        transform = location.Transformation();
    }

    const auto& indices = polygon_on_triangulation->Nodes();
    const auto node_count = polygon_on_triangulation->NbNodes();
    result.vertices.resize(static_cast<std::size_t>(node_count) * 3);
    result.indices.resize(static_cast<std::size_t>(node_count));

    for (Standard_Integer i = 1; i <= node_count; ++i) {
        auto vertex = face_triangulation->Node(indices(i));
        vertex.Transform(transform);
        const auto vertex_index = static_cast<std::size_t>(i - 1) * 3;
        result.vertices[vertex_index] = vertex.X();
        result.vertices[vertex_index + 1] = vertex.Y();
        result.vertices[vertex_index + 2] = vertex.Z();
        result.indices[static_cast<std::size_t>(i - 1)] = indices(i) - 1;
    }

    return result;
}

} // namespace

ShapeTessellation extract_tessellation(const TopoDS_Shape& shape, bool compute_normals)
{
    std::vector<TopoDS_Face> faces;
    for (TopExp_Explorer explorer(shape, TopAbs_FACE); explorer.More(); explorer.Next()) {
        faces.push_back(TopoDS::Face(explorer.Current()));
    }

    ShapeTessellation result;
    result.faces.resize(faces.size());
    std::transform(faces.begin(), faces.end(), result.faces.begin(), [compute_normals](const TopoDS_Face& face) {
        return extract_face_tessellation(face, compute_normals);
    });

    TopTools_IndexedMapOfShape edge_map;
    TopExp::MapShapes(shape, TopAbs_EDGE, edge_map);

    TopTools_IndexedDataMapOfShapeListOfShape edge_face_map;
    TopExp::MapShapesAndAncestors(shape, TopAbs_EDGE, TopAbs_FACE, edge_face_map);

    std::vector<TopoDS_Edge> edges;
    for (Standard_Integer i = 1; i <= edge_face_map.Extent(); ++i) {
        const auto& faces_for_edge = edge_face_map.FindFromIndex(i);
        if (!faces_for_edge.IsEmpty()) {
            edges.push_back(TopoDS::Edge(edge_map(i)));
        }
    }

    result.edges.resize(edges.size());
    std::transform(edges.begin(), edges.end(), result.edges.begin(), [&edge_face_map](const TopoDS_Edge& edge) {
        return extract_edge_tessellation(edge, edge_face_map);
    });

    return result;
}

void build_mesh(const TopoDS_Shape& shape,
                double linear_deflection,
                bool is_relative,
                double angle_deflection,
                bool parallel)
{
    BRepTools::Clean(shape);
    BRepMesh_IncrementalMesh mesher(shape, linear_deflection, is_relative, degrees_to_radians(angle_deflection), parallel);
}

ShapeTessellation extract_tessellation(const TopoDS_Shape& shape,
                                       double linear_deflection,
                                       bool is_relative,
                                       double angle_deflection,
                                       bool parallel,
                                       bool compute_normals)
{
    build_mesh(shape, linear_deflection, is_relative, angle_deflection, parallel);
    return extract_tessellation(shape, compute_normals);
}

ShapeTessellation extract_tessellation(const TopoDS_Shape& shape,
                                       const IMeshTools_Parameters& params,
                                       bool compute_normals)
{
    BRepTools::Clean(shape);
    BRepMesh_IncrementalMesh mesher(shape, params);
    return extract_tessellation(shape, compute_normals);
}

IMeshTools_Parameters make_delaunay_parameters(std::optional<double> element_size,
                                               std::optional<double> surface_deflection,
                                               IMeshTools_MeshAlgoType mesh_algo,
                                               bool is_relative,
                                               double angle_deflection,
                                               bool parallel,
                                               bool control_surface_deflection)
{
    const auto deflection = element_size.value_or(surface_deflection.value_or(0.0));
    if (deflection <= 0.0) {
        throw std::invalid_argument("element_size or surface_deflection must be positive");
    }

    const auto interior_deflection = surface_deflection.value_or(deflection);
    if (interior_deflection <= 0.0) {
        throw std::invalid_argument("surface_deflection must be positive");
    }

    const auto angle_in_radians = degrees_to_radians(angle_deflection);

    IMeshTools_Parameters params;
    params.MeshAlgo = mesh_algo;
    params.Deflection = deflection;
    params.DeflectionInterior = interior_deflection;
    params.Angle = angle_in_radians;
    params.AngleInterior = angle_in_radians;
    params.InParallel = parallel;
    params.Relative = is_relative;
    params.ControlSurfaceDeflection = control_surface_deflection || surface_deflection.has_value();
    params.EnableControlSurfaceDeflectionAllSurfaces = params.ControlSurfaceDeflection;

    return params;
}

ShapeTessellation extract_delaunay_tessellation(const TopoDS_Shape& shape,
                                                std::optional<double> element_size,
                                                std::optional<double> surface_deflection,
                                                IMeshTools_MeshAlgoType mesh_algo,
                                                bool is_relative,
                                                double angle_deflection,
                                                bool parallel,
                                                bool compute_normals,
                                                bool control_surface_deflection)
{
    const auto params = make_delaunay_parameters(
        element_size,
        surface_deflection,
        mesh_algo,
        is_relative,
        angle_deflection,
        parallel,
        control_surface_deflection
    );
    return extract_tessellation(shape, params, compute_normals);
}

EdgeTessellation extract_curve_tessellation(const Adaptor3d_Curve& curve, double linear_deflection)
{
    GCPnts_UniformDeflection discretization(curve, linear_deflection);
    const auto point_count = static_cast<int>(discretization.NbPoints());

    EdgeTessellation result;
    result.indices.resize(static_cast<std::size_t>(point_count));
    result.vertices.resize(static_cast<std::size_t>(point_count) * 3);
    std::iota(result.indices.begin(), result.indices.end(), 0);

    for (int i = 1; i <= point_count; ++i) {
        const gp_Pnt point = discretization.Value(i);
        const auto index = static_cast<std::size_t>(i - 1) * 3;
        result.vertices[index] = point.X();
        result.vertices[index + 1] = point.Y();
        result.vertices[index + 2] = point.Z();
    }

    return result;
}

EdgeTessellation extract_curve_tessellation(const opencascade::handle<Geom_Curve>& curve, double linear_deflection)
{
    GeomAdaptor_Curve adaptor_curve(curve);
    return extract_curve_tessellation(adaptor_curve, linear_deflection);
}

EdgeTessellation extract_curve_tessellation(const TopoDS_Edge& edge, double linear_deflection)
{
    BRepAdaptor_Curve adaptor_curve(edge);
    return extract_curve_tessellation(adaptor_curve, linear_deflection);
}

EdgeTessellation extract_curve_tessellation(const TopoDS_Wire& wire, double linear_deflection)
{
    BRepAdaptor_CompCurve adaptor_curve(wire);
    return extract_curve_tessellation(adaptor_curve, linear_deflection);
}

} // namespace occt::extended::render