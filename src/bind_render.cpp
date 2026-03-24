#include <Adaptor2d_Curve2d.hxx>
#include <Adaptor3d_Curve.hxx>
#include <Geom_Curve.hxx>
#include <Standard_Handle.hxx>
#include <numeric>
#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include <utility>

namespace py = pybind11;

#include <BRepMesh_IncrementalMesh.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <Poly_Triangulation.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS.hxx>
#include <algorithm>
#include <cstddef>
#include <gp_Dir.hxx>
#include <gp_Pnt.hxx>
#include <gp_Quaternion.hxx>
#include <gp_Trsf.hxx>
#include <vector>
#include <numbers>
#include <GCPnts_UniformDeflection.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepAdaptor_CompCurve.hxx>
#include <IMeshTools_Parameters.hxx>
#include <IMeshTools_MeshAlgoType.hxx>
#include <BRepTools.hxx>

using edge_tess_info = std::tuple<
    py::array_t<int>, 
    py::array_t<double>
>;

using face_tess_info = std::tuple<
    py::array_t<int>, 
    py::array_t<double>, 
    std::optional<py::array_t<double>>, 
    std::optional<py::array_t<double>>
>;

auto extract_tessellation(const TopoDS_Shape& shape, bool compute_normals)
{
    std::vector<TopoDS_Face> faces;
    for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next())
    {
        faces.push_back(TopoDS::Face(exp.Current()));
    }

    std::vector<face_tess_info> result_faces(faces.size());
    std::transform(faces.begin(), faces.end(), std::begin(result_faces),
        [compute_normals](const TopoDS_Face& face)
    {
        TopLoc_Location loc;
        Handle(Poly_Triangulation) tri = BRep_Tool::Triangulation(face, loc);
        
        if (compute_normals)
            tri->ComputeNormals();

        if (tri.IsNull())
            return face_tess_info{};

        const int n_vertices = tri->NbNodes();
        const int n_triangles = tri->NbTriangles();

        const bool had_normals = tri->HasNormals();
        const bool had_uvs = tri->HasUVNodes();
        const bool reverse_orientation = (face.Orientation() == TopAbs_INTERNAL);

        // Create numpy arrays directly with proper shapes
        auto tri_array = py::array_t<int>({n_triangles, 3});
        auto vert_array = py::array_t<double>({n_vertices, 3});
        auto norm_array = had_normals ? std::optional<py::array_t<double>>(py::array_t<double>({n_vertices, 3})) : std::optional<py::array_t<double>>{};
        auto uv_array = had_uvs ? std::optional<py::array_t<double>>(py::array_t<double>({n_vertices, 2})) : std::optional<py::array_t<double>>{};
        
        // Get mutable pointers to write data directly
        auto tri_ptr = tri_array.mutable_data();
        auto vert_ptr = vert_array.mutable_data();
        auto norm_ptr = norm_array ? norm_array->mutable_data() : nullptr;
        auto uv_ptr = uv_array ? uv_array->mutable_data() : nullptr;

        auto vertice_trfm = loc.Transformation();

        auto normal_trfm = gp_Trsf();
        normal_trfm.SetRotation(loc.Transformation().GetRotation());

        // Process vertices - split loops to avoid branch prediction overhead
        for(size_t i = 0; i < n_vertices; ++i)
        {
            auto p = tri->Node(i + 1).Transformed(vertice_trfm);
            vert_ptr[3 * i]     = p.X();
            vert_ptr[3 * i + 1] = p.Y();
            vert_ptr[3 * i + 2] = p.Z();
        }
        
        if (had_normals)
        {
            for(size_t i = 0; i < n_vertices; ++i)
            {
                // auto n = tri->Normal(i + 1).Transformed(normal_trfm);
                auto n = tri->Normal(i + 1);
                if (reverse_orientation)
                    n.Reverse();
                
                const int idx = 3 * i;

                norm_ptr[idx]     = n.X();
                norm_ptr[idx + 1] = n.Y();
                norm_ptr[idx + 2] = n.Z();
            }
        }
        
        if (had_uvs)
        {
            for(size_t i = 0; i < n_vertices; ++i)
            {
                auto uv = tri->UVNode(i + 1);
                const int idx = 2 * i;
                uv_ptr[idx]     = uv.X();
                uv_ptr[idx + 1] = uv.Y();
            }
        }
        
        for(size_t i = 0; i < n_triangles; ++i)
        {
            int n1, n2, n3;
            tri->Triangle(i + 1).Get(n1, n2, n3);
            const int idx = 3 * i;
            tri_ptr[idx]     = n1 - 1;
            tri_ptr[idx + 1] = n2 - 1;
            tri_ptr[idx + 2] = n3 - 1;
        }
        
        return face_tess_info{ tri_array, vert_array, norm_array, uv_array };
    });


    
    // Get unique edges only
    std::vector<TopoDS_Edge> edges;
    TopTools_IndexedMapOfShape edge_map;
    TopExp::MapShapes(shape, TopAbs_EDGE, edge_map);
    
    TopTools_IndexedDataMapOfShapeListOfShape edge_face_map;
    TopExp::MapShapesAndAncestors(shape, TopAbs_EDGE, TopAbs_FACE, edge_face_map);
    
    
    for (Standard_Integer i = 1; i <= edge_face_map.Extent(); ++i) {
        const auto& face_list_for_edge = edge_face_map.FindFromIndex(i);
        
        if (face_list_for_edge.IsEmpty()) {

            continue;  // Skip free edges
        }
        
        edges.push_back(TopoDS::Edge(edge_map(i)));
    }
    std::vector<edge_tess_info> result_edges(edges.size());
    
    std::transform(edges.begin(), edges.end(), std::begin(result_edges),
        [&edge_face_map](const TopoDS_Edge& edge)
        {
        TopLoc_Location location;
        gp_Trsf transform;
        
        py::array_t<double> vert_array;
        py::array_t<int> index_array;

        // Try direct 3D triangulation first
        auto poly_3d = BRep_Tool::Polygon3D(edge, location);
    
        if (!poly_3d.IsNull()) {
            if (!location.IsIdentity()) {
                transform = location.Transformation();

                const auto& nodes = poly_3d->Nodes();
                const auto nb_nodes = poly_3d->NbNodes();

                // Pre-allocate exact size
                vert_array.resize({nb_nodes, 3});
                auto vert_ptr = vert_array.mutable_data();
                index_array.resize({nb_nodes});
                auto index_ptr = index_array.mutable_data();
                
                for (int i = 0; i < nb_nodes; ++i) {
                    auto vertex = nodes(i+1);
                    vertex.Transform(transform);

                    const auto idx = i * 3;
                    vert_ptr[idx]     = vertex.X();
                    vert_ptr[idx + 1] = vertex.Y();
                    vert_ptr[idx + 2] = vertex.Z();
                    index_ptr[i] = i;
                }
                
            }
        }

        else {
            const auto& first_face = TopoDS::Face(edge_face_map.FindFromKey(edge).First());
            auto face_triangulation = BRep_Tool::Triangulation(first_face, location);
            if (face_triangulation.IsNull()) {
                return edge_tess_info{ index_array, vert_array };
            }
            
            auto poly_on_tri = BRep_Tool::PolygonOnTriangulation(edge, face_triangulation, location);
            if (poly_on_tri.IsNull()) {
                return edge_tess_info{ index_array, vert_array };
            }
            
            if (!location.IsIdentity()) {
                transform = location.Transformation();
            }

            const auto& indices = poly_on_tri->Nodes();
            const auto nb_nodes = poly_on_tri->NbNodes();

            // Pre-allocate exact size
            vert_array.resize({nb_nodes, 3});
            auto vert_ptr = vert_array.mutable_data();
            index_array.resize({nb_nodes});
            auto index_ptr = index_array.mutable_data();

            for (Standard_Integer i = 1; i <= nb_nodes; ++i) {
                auto vertex = face_triangulation->Node(indices(i));
                vertex.Transform(transform);

                const auto idx = (i - 1) * 3;
                vert_ptr[idx] = vertex.X();
                vert_ptr[idx + 1] = vertex.Y();
                vert_ptr[idx + 2] = vertex.Z();
                index_ptr[i - 1] = indices(i) - 1;
            }
        }

        return edge_tess_info{ index_array, vert_array };
    });
    // } 

    return std::make_pair(result_faces, result_edges);
}

auto build_mesh(const TopoDS_Shape& shape, double linear_deflection,
                           bool is_relative = false,
                           double angle_deflection = 30.0,
                           bool parallel = true,
                           bool compute_normals = true)
{
    angle_deflection *= std::numbers::pi / 180.0;
    BRepTools::Clean(shape);
    BRepMesh_IncrementalMesh mesher(shape, linear_deflection, is_relative, angle_deflection, parallel);
}

auto extract_tessellation( const TopoDS_Shape& shape, double linear_deflection,
                           bool is_relative = false,
                           double angle_deflection = 30.0,
                           bool parallel = true,
                           bool compute_normals = true)
{
    build_mesh(shape, linear_deflection, is_relative, angle_deflection, parallel, compute_normals);
    return extract_tessellation(shape, compute_normals);
}

auto extract_tessellation( const TopoDS_Shape& shape, const IMeshTools_Parameters& params, bool compute_normals = true)
{
    BRepTools::Clean(shape);
    BRepMesh_IncrementalMesh mesher(shape, params);
    return extract_tessellation(shape, compute_normals);
}

auto extract_curve_tessellation( const Adaptor3d_Curve& curve, double linear_deflection)
{
    
    GCPnts_UniformDeflection discretization(
        curve, linear_deflection
    );

    edge_tess_info edge_info;
    std::get<0>(edge_info) = py::array_t<int>(static_cast<int>(discretization.NbPoints()));
    std::get<1>(edge_info) = py::array_t<double>({static_cast<int>(discretization.NbPoints()), 3});
    
    auto& indices = std::get<0>(edge_info);
    auto& vertices = std::get<1>(edge_info);
    auto index_ptr = indices.mutable_data();
    auto vert_ptr = vertices.mutable_data();

    const auto point_count = static_cast<int>(discretization.NbPoints());
    std::iota(index_ptr, index_ptr + point_count, 0);

    for (int i = 1; i <= point_count; ++i) {
        gp_Pnt p = discretization.Value(i);
        const auto idx = (i - 1) * 3;
        vert_ptr[idx]     = p.X();
        vert_ptr[idx + 1] = p.Y();
        vert_ptr[idx + 2] = p.Z();
    }

    return edge_info;

}

auto extract_curve_tessellation( const opencascade::handle<Geom_Curve>& curve, double linear_deflection)
{
    GeomAdaptor_Curve adaptor_curve(curve);
    return extract_curve_tessellation(adaptor_curve, linear_deflection);
}

auto extract_curve_tessellation( const TopoDS_Edge& edge, double linear_deflection)
{

    BRepAdaptor_Curve adaptor_curve(edge);
    return extract_curve_tessellation(adaptor_curve, linear_deflection);
}

auto extract_curve_tessellation( const TopoDS_Wire& wire, double linear_deflection)
{
    BRepAdaptor_CompCurve adaptor_curve(wire);
    return extract_curve_tessellation(adaptor_curve, linear_deflection);
}

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_render(py::module_ &m)
{
    // Bind IMeshTools_MeshAlgoType enum
    py::enum_<IMeshTools_MeshAlgoType>(m, "MeshAlgoType")
        .value("DEFAULT", IMeshTools_MeshAlgoType_DEFAULT, "Use global default (Watson or CSF_MeshAlgo)")
        .value("Watson", IMeshTools_MeshAlgoType_Watson, "2D Delaunay triangulation based on Watson algorithm")
        .value("Delabella", IMeshTools_MeshAlgoType_Delabella, "2D Delaunay triangulation based on Delabella algorithm")
        .export_values();

    // Bind IMeshTools_Parameters struct with all properties
    py::class_<IMeshTools_Parameters>(m, "MeshParameters", "Structure storing meshing parameters")
        .def(py::init<>())
        .def_readwrite("MeshAlgo", &IMeshTools_Parameters::MeshAlgo,
            "2D Delaunay triangulation algorithm factory to use")
        .def_readwrite("Angle", &IMeshTools_Parameters::Angle,
            "Angular deflection used to tessellate the boundary edges")
        .def_readwrite("Deflection", &IMeshTools_Parameters::Deflection,
            "Linear deflection used to tessellate the boundary edges")
        .def_readwrite("AngleInterior", &IMeshTools_Parameters::AngleInterior,
            "Angular deflection used to tessellate the face interior")
        .def_readwrite("DeflectionInterior", &IMeshTools_Parameters::DeflectionInterior,
            "Linear deflection used to tessellate the face interior")
        .def_readwrite("MinSize", &IMeshTools_Parameters::MinSize,
            "Minimum size parameter limiting size of triangle's edges")
        .def_readwrite("InParallel", &IMeshTools_Parameters::InParallel,
            "Switches on/off multi-thread computation")
        .def_readwrite("Relative", &IMeshTools_Parameters::Relative,
            "Switches on/off relative computation of edge tolerance")
        .def_readwrite("InternalVerticesMode", &IMeshTools_Parameters::InternalVerticesMode,
            "Mode to take or not to take internal face vertices into account")
        .def_readwrite("ControlSurfaceDeflection", &IMeshTools_Parameters::ControlSurfaceDeflection,
            "Parameter to check the deviation of triangulation and interior of the face")
        .def_readwrite("EnableControlSurfaceDeflectionAllSurfaces", &IMeshTools_Parameters::EnableControlSurfaceDeflectionAllSurfaces,
            "Enables/disables check triggered by ControlSurfaceDeflection for all surface types")
        .def_readwrite("CleanModel", &IMeshTools_Parameters::CleanModel,
            "Cleans temporary data model when algorithm is finished")
        .def_readwrite("AdjustMinSize", &IMeshTools_Parameters::AdjustMinSize,
            "Enables/disables local adjustment of min size depending on edge size")
        .def_readwrite("ForceFaceDeflection", &IMeshTools_Parameters::ForceFaceDeflection,
            "Enables/disables usage of shape tolerances for computing face deflection")
        .def_readwrite("AllowQualityDecrease", &IMeshTools_Parameters::AllowQualityDecrease,
            "Allows/forbids the decrease of the quality of the generated mesh over existing one")
        .def_static("RelMinSize", &IMeshTools_Parameters::RelMinSize,
            "Returns factor used to compute default value of MinSize from deflection");

    m.def("extract_tessellation", py::overload_cast<
            const TopoDS_Shape&, double, bool, double, bool, bool
        >(&extract_tessellation),
        py::arg("shape"),
        py::arg("linear_deflection"),
        py::arg("is_relative") = false,
        py::arg("angle_deflection") = 30.,
        py::arg("parallel") = true,
        py::arg("compute_normals") = true,
        "Extracts tessellation data from the given shape. Previous tessellation data will be cleared.\n\n"
        "Parameters:\n"
        "  shape: The TopoDS_Shape to tessellate\n"
        "  linear_deflection: The linear deflection value for meshing\n"
        "  is_relative: If True, linear deflection is relative to shape size (default: False)\n"
        "  angle_deflection: The angular deflection in degrees (default: 30.0)\n"
        "  parallel: If True, use parallel processing (default: True)\n"
        "  compute_normals: If True, compute vertex normals (default: True)\n\n"
        "Returns:\n"
        "  A list of tuples for each face, each containing:\n"
        "    - triangle indices (numpy array of int, shape (n_triangles, 3))\n"
        "    - vertex positions (numpy array of float, shape (n_vertices, 3))\n"
        "    - vertex normals (numpy array of float, shape (n_vertices, 3))\n"
        "    - vertex UV coordinates (numpy array of float, shape (n_vertices, 2))"
    );

    m.def("extract_tessellation", py::overload_cast<
            const TopoDS_Shape&, const IMeshTools_Parameters&, bool
        >(&extract_tessellation),
        py::arg("shape"),
        py::arg("params"),
        py::arg("compute_normals") = true,
        "Extracts tessellation data from the given shape using specified meshing parameters.\n\n"
        "Parameters:\n"
        "  shape: The TopoDS_Shape to tessellate\n"
        "  params: An instance of MeshParameters containing meshing settings\n"
        "  compute_normals: If True, compute vertex normals (default: True)\n\n"
        "Returns:\n"
        "  A list of tuples for each face, each containing:\n"
        "    - triangle indices (numpy array of int, shape (n_triangles, 3))\n"
        "    - vertex positions (numpy array of float, shape (n_vertices, 3))\n"
        "    - vertex normals (numpy array of float, shape (n_vertices, 3))\n"
        "    - vertex UV coordinates (numpy array of float, shape (n_vertices, 2))"
    );

    m.def("extract_curve_tessellation", py::overload_cast<const opencascade::handle<Geom_Curve>&, double>(&extract_curve_tessellation),
        py::arg("curve"),
        py::arg("linear_deflection"),
        "Extracts tessellation data from the given curve. Previous tessellation data will be cleared.\n\n"
        "Parameters:\n"
        "  curve: The Geom_Curve to tessellate\n"
        "  linear_deflection: The linear deflection value for meshing\n\n"
        "Returns:\n"
        "  A tuple containing:\n"
        "    - vertex indices (numpy array of int, shape (n_points,))\n"
        "    - vertex positions (numpy array of float, shape (n_points, 3))"
    );

    m.def("extract_curve_tessellation", py::overload_cast<const TopoDS_Edge&, double>(&extract_curve_tessellation),
        py::arg("edge"),
        py::arg("linear_deflection"),
        "Extracts tessellation data from the given edge.\n\n"
        "Parameters:\n"
        "  edge: The TopoDS_Edge to tessellate\n"
        "  linear_deflection: The linear deflection value for meshing\n\n"
        "Returns:\n"
        "  A tuple containing:\n"
        "    - vertex indices (numpy array of int, shape (n_points,))\n"
        "    - vertex positions (numpy array of float, shape (n_points, 3))"
    );

    m.def("extract_curve_tessellation", py::overload_cast<const TopoDS_Wire&, double>(&extract_curve_tessellation),
        py::arg("wire"),
        py::arg("linear_deflection"),
        "Extracts tessellation data from the given wire.\n\n"
        "Parameters:\n"
        "  wire: The TopoDS_Wire to tessellate\n"
        "  linear_deflection: The linear deflection value for meshing\n\n"
        "Returns:\n"
        "  A tuple containing:\n"
        "    - vertex indices (numpy array of int, shape (n_points,))\n"
        "    - vertex positions (numpy array of float, shape (n_points, 3))"
    );
}