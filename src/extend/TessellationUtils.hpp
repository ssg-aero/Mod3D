#pragma once

#include <IMeshTools_MeshAlgoType.hxx>
#include <IMeshTools_Parameters.hxx>
#include <Standard_Handle.hxx>

#include <optional>
#include <vector>

class Adaptor3d_Curve;
class Geom_Curve;
class TopoDS_Edge;
class TopoDS_Shape;
class TopoDS_Wire;

namespace occt::extended::render {

struct EdgeTessellation {
    std::vector<int> indices;
    std::vector<double> vertices;
};

struct FaceTessellation {
    std::vector<int> triangles;
    std::vector<double> vertices;
    std::optional<std::vector<double>> normals;
    std::optional<std::vector<double>> uvs;
};

struct ShapeTessellation {
    std::vector<FaceTessellation> faces;
    std::vector<EdgeTessellation> edges;
};

ShapeTessellation extract_tessellation(const TopoDS_Shape& shape, bool compute_normals);

void build_mesh(const TopoDS_Shape& shape,
                double linear_deflection,
                bool is_relative = false,
                double angle_deflection = 30.0,
                bool parallel = true);

ShapeTessellation extract_tessellation(const TopoDS_Shape& shape,
                                       double linear_deflection,
                                       bool is_relative = false,
                                       double angle_deflection = 30.0,
                                       bool parallel = true,
                                       bool compute_normals = true);

ShapeTessellation extract_tessellation(const TopoDS_Shape& shape,
                                       const IMeshTools_Parameters& params,
                                       bool compute_normals = true);

IMeshTools_Parameters make_delaunay_parameters(std::optional<double> element_size,
                                               std::optional<double> surface_deflection,
                                               IMeshTools_MeshAlgoType mesh_algo,
                                               bool is_relative,
                                               double angle_deflection,
                                               bool parallel,
                                               bool control_surface_deflection);

ShapeTessellation extract_delaunay_tessellation(const TopoDS_Shape& shape,
                                                std::optional<double> element_size,
                                                std::optional<double> surface_deflection,
                                                IMeshTools_MeshAlgoType mesh_algo = IMeshTools_MeshAlgoType_DEFAULT,
                                                bool is_relative = false,
                                                double angle_deflection = 30.0,
                                                bool parallel = true,
                                                bool compute_normals = true,
                                                bool control_surface_deflection = true);

EdgeTessellation extract_curve_tessellation(const Adaptor3d_Curve& curve, double linear_deflection);
EdgeTessellation extract_curve_tessellation(const opencascade::handle<Geom_Curve>& curve, double linear_deflection);
EdgeTessellation extract_curve_tessellation(const TopoDS_Edge& edge, double linear_deflection);
EdgeTessellation extract_curve_tessellation(const TopoDS_Wire& wire, double linear_deflection);

} // namespace occt::extended::render