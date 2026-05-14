#include <Geom_Curve.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Wire.hxx>

#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <algorithm>
#include <initializer_list>
#include <optional>
#include <tuple>
#include <vector>

#include <IMeshTools_MeshAlgoType.hxx>
#include <IMeshTools_Parameters.hxx>

#include "extend/TessellationUtils.hpp"

namespace py = pybind11;
namespace render = occt::extended::render;

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

template <typename T>
py::array_t<T> to_array(const std::vector<T>& values, std::initializer_list<py::ssize_t> shape)
{
    const std::vector<py::ssize_t> dimensions(shape);
    py::array_t<T> array(dimensions);
    std::copy(values.begin(), values.end(), array.mutable_data());
    return array;
}

edge_tess_info to_python(const render::EdgeTessellation& tessellation)
{
    const auto point_count = static_cast<py::ssize_t>(tessellation.indices.size());
    return {
        to_array(tessellation.indices, {point_count}),
        to_array(tessellation.vertices, {point_count, 3})
    };
}

face_tess_info to_python(const render::FaceTessellation& tessellation)
{
    const auto vertex_count = static_cast<py::ssize_t>(tessellation.vertices.size() / 3);
    const auto triangle_count = static_cast<py::ssize_t>(tessellation.triangles.size() / 3);
    std::optional<py::array_t<double>> normals;
    std::optional<py::array_t<double>> uvs;

    if (tessellation.normals) {
        normals = to_array(*tessellation.normals, {vertex_count, 3});
    }
    if (tessellation.uvs) {
        uvs = to_array(*tessellation.uvs, {vertex_count, 2});
    }

    return {
        to_array(tessellation.triangles, {triangle_count, 3}),
        to_array(tessellation.vertices, {vertex_count, 3}),
        normals,
        uvs
    };
}

auto to_python(const render::ShapeTessellation& tessellation)
{
    std::vector<face_tess_info> faces;
    faces.reserve(tessellation.faces.size());
    for (const auto& face : tessellation.faces) {
        faces.push_back(to_python(face));
    }

    std::vector<edge_tess_info> edges;
    edges.reserve(tessellation.edges.size());
    for (const auto& edge : tessellation.edges) {
        edges.push_back(to_python(edge));
    }

    return std::make_pair(faces, edges);
}

auto extract_tessellation(const TopoDS_Shape& shape,
                          double linear_deflection,
                          bool is_relative = false,
                          double angle_deflection = 30.0,
                          bool parallel = true,
                          bool compute_normals = true)
{
    return to_python(render::extract_tessellation(
        shape,
        linear_deflection,
        is_relative,
        angle_deflection,
        parallel,
        compute_normals
    ));
}

auto extract_tessellation(const TopoDS_Shape& shape,
                          const IMeshTools_Parameters& params,
                          bool compute_normals = true)
{
    return to_python(render::extract_tessellation(shape, params, compute_normals));
}

auto extract_delaunay_tessellation(const TopoDS_Shape& shape,
                                   std::optional<double> element_size,
                                   std::optional<double> surface_deflection,
                                   IMeshTools_MeshAlgoType mesh_algo = IMeshTools_MeshAlgoType_DEFAULT,
                                   bool is_relative = false,
                                   double angle_deflection = 30.0,
                                   bool parallel = true,
                                   bool compute_normals = true,
                                   bool control_surface_deflection = true)
{
    return to_python(render::extract_delaunay_tessellation(
        shape,
        element_size,
        surface_deflection,
        mesh_algo,
        is_relative,
        angle_deflection,
        parallel,
        compute_normals,
        control_surface_deflection
    ));
}

auto extract_curve_tessellation(const opencascade::handle<Geom_Curve>& curve, double linear_deflection)
{
    return to_python(render::extract_curve_tessellation(curve, linear_deflection));
}

auto extract_curve_tessellation(const TopoDS_Edge& edge, double linear_deflection)
{
    return to_python(render::extract_curve_tessellation(edge, linear_deflection));
}

auto extract_curve_tessellation(const TopoDS_Wire& wire, double linear_deflection)
{
    return to_python(render::extract_curve_tessellation(wire, linear_deflection));
}

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

    m.def("extract_delaunay_tessellation", &extract_delaunay_tessellation,
        py::arg("shape"),
        py::arg("element_size") = std::nullopt,
        py::arg("surface_deflection") = std::nullopt,
        py::arg("mesh_algo") = IMeshTools_MeshAlgoType_DEFAULT,
        py::arg("is_relative") = false,
        py::arg("angle_deflection") = 30.,
        py::arg("parallel") = true,
        py::arg("compute_normals") = true,
        py::arg("control_surface_deflection") = true,
        "Extracts Delaunay tessellation data from the given shape while preserving the same output format as extract_tessellation.\n\n"
        "Parameters:\n"
        "  shape: The TopoDS_Shape to tessellate\n"
        "  element_size: Target element deflection for boundary edges and face interiors\n"
        "  surface_deflection: Optional face interior deflection; if omitted, element_size is used\n"
        "  mesh_algo: Delaunay triangulation algorithm to use (default: MeshAlgoType.DEFAULT)\n"
        "  is_relative: If True, deflection is relative to shape size (default: False)\n"
        "  angle_deflection: The angular deflection in degrees (default: 30.0)\n"
        "  parallel: If True, use parallel processing (default: True)\n"
        "  compute_normals: If True, compute vertex normals (default: True)\n"
        "  control_surface_deflection: If True, check triangulation deviation from surfaces (default: True)\n\n"
        "Returns:\n"
        "  A pair (faces, edges) using the same tuple and numpy array layout as extract_tessellation"
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