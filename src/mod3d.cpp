#include <TopoDS_Shell.hxx>
#include <pybind11/pybind11.h>

#include <BRepBndLib.hxx>
#include <BRepPrimAPI_MakeBox.hxx>
#include <Bnd_Box.hxx>
#include <TopoDS_Shape.hxx>

#include <stdexcept>

namespace py = pybind11;

// Forward declaration from sub binding fonctions

void bind_gp(py::module_ &m);
void bind_geom2d(py::module_ &m);
void bind_geom(py::module_ &m);
void bind_geom_abs(py::module_ &m);
void bind_geom_fill(py::module_ &m);
void bind_topods(py::module_ &m);
void bind_top_abs(py::module_ &m);
void bind_adaptor(py::module_ &m);
void bind_top_exp(py::module_ &m);
void bind_brep(py::module_ &m);
void bind_brepbuilder_api(py::module_ &m);
void bind_brep_offset_api(py::module_ &m);
void bind_brepprim(py::module_ &m);
void bind_brep_fillet(py::module_ &m);
// void bind_brep_fillet2d(py::module_ &m);
void bind_brep_extrema(py::module_ &m);
void bind_brep_boolean_op(py::module_ &m);
void bind_shape_analysis(py::module_ &m);
void bind_step_control(py::module_ &m);
void bind_step_data(py::module_ &m);
void bind_render(py::module_ &m);


static py::dict create_box_summary(double dx, double dy, double dz)
{
    if (dx <= 0.0 || dy <= 0.0 || dz <= 0.0) {
        throw std::invalid_argument("All box extents must be positive");
    }

    BRepPrimAPI_MakeBox box_maker(dx, dy, dz);
    auto solid = box_maker.Solid();

    Bnd_Box bounding_box;
    BRepBndLib::Add(solid, bounding_box);

    Standard_Real xmin, ymin, zmin, xmax, ymax, zmax;
    bounding_box.Get(xmin, ymin, zmin, xmax, ymax, zmax);

    py::dict bounds;
    bounds["min"] = py::make_tuple(xmin, ymin, zmin);
    bounds["max"] = py::make_tuple(xmax, ymax, zmax);

    py::dict summary;
    summary["bounds"] = bounds;
    summary["extent"] = py::make_tuple(dx, dy, dz);
    summary["volume"] = dx * dy * dz;
    summary["shape_type"] = "box";
    return summary;
}

PYBIND11_MODULE(mod3d, m)
{
    m.doc() = "Mod3D - OpenCASCADE bindings";
    
    // Create submodules
    py::module_ gp = m.def_submodule("gp", "Geometric primitives");
    py::module_ Geom2d = m.def_submodule("Geom2d", "Geom2d module");
    py::module_ Geom = m.def_submodule("Geom", "Geom module");
    py::module_ GeomFill = m.def_submodule("GeomFill", "GeomFill module");
    py::module_ GeomAbs = m.def_submodule("GeomAbs", "GeomAbs module");

    py::module_ TopoDS = m.def_submodule("TopoDS", "Topology data structures");
    py::module_ Adaptor = m.def_submodule("Adaptor", "Adaptor module");
    py::module_ TopAbs = m.def_submodule("TopAbs", "Topological shape types");
    py::module_ TopExp = m.def_submodule("TopExp", "Topological exploration module");
    py::module_ BRep = m.def_submodule("BRep", "Boundary representation module");
    py::module_ BRepBuilderAPI = m.def_submodule("BRepBuilderAPI", "BRep builder API");
    py::module_ BRepPrim = m.def_submodule("BRepPrim", "BRep primitive shapes");
    py::module_ BRepOffsetAPI = m.def_submodule("BRepOffsetAPI", "BRep offset API");
    py::module_ BRepExtrema = m.def_submodule("BRepExtrema", "BRep extrema computations");
    py::module_ BRepFillet = m.def_submodule("BRepFillet", "BRep fillet module");
    py::module_ BooleanOp = m.def_submodule("BooleanOp", "BRep boolean operations module");
    // py::module_ BRepFillet2d = m.def_submodule("BRepFillet2d", "BRep 2D fillet module");

    py::module_ ShapeAnalysis = m.def_submodule("ShapeAnalysis", "Shape analysis module");

    py::module_ StepControl = m.def_submodule("StepControl", "STEP control module");
    py::module_ StepData = m.def_submodule("StepData", "STEP data module");

    py::module_ Render = m.def_submodule("Render", "Rendering and tessellation module");


    m.doc() = "Open Cascade helpers exposed via pybind11 as part of the mod3d package.";

    // m.def("occt_version", &occt_version_string, "Return the Open Cascade runtime version.");
    m.def("create_box_summary",
          &create_box_summary,
          py::arg("dx") = 10.0,
          py::arg("dy") = 10.0,
          py::arg("dz") = 10.0,
          "Build a box primitve and return its bounding information.");

    m.def(
        "create_box",
        [](double dx, double dy, double dz) {
            if (dx <= 0.0 || dy <= 0.0 || dz <= 0.0) {
                throw std::invalid_argument("All box extents must be positive");
            }
            BRepPrimAPI_MakeBox box_maker(dx, dy, dz);
            return box_maker.Solid();
        },
        py::arg("dx") = 10.0,
        py::arg("dy") = 10.0,
        py::arg("dz") = 10.0,
        "Create and return a box shape with the given dimensions.   "
    );

    // Bind submodules
    bind_gp(gp);
    bind_geom_abs(GeomAbs);
    bind_geom_fill(GeomFill);
    bind_geom(Geom);
    bind_geom2d(Geom2d);
    bind_topods(TopoDS);
    bind_adaptor(Adaptor);
    bind_top_abs(TopAbs);
    bind_top_exp(TopExp);
    bind_brep(BRep);
    bind_brepprim(BRepPrim);
    bind_brepbuilder_api(BRepBuilderAPI);
    bind_brep_offset_api(BRepOffsetAPI);
    bind_brep_extrema(BRepExtrema);
    bind_brep_fillet(BRepFillet);
    bind_brep_boolean_op(BooleanOp);
    // bind_brep_fillet2d(BRepFillet2d);

    bind_shape_analysis(ShapeAnalysis);

    bind_step_control(StepControl);
    bind_step_data(StepData);
    
    // Bind rendering/tessellation functions to main module
    bind_render(Render);
}
