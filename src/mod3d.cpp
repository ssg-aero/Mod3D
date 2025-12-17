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
void bind_geom(py::module_ &m);
void bind_topods(py::module_ &m);
void bind_brepbuilderapi(py::module_ &m);
void bind_brepprim(py::module_ &m);

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
    py::module_ Geom = m.def_submodule("Geom", "Geom module");
    py::module_ TopoDS = m.def_submodule("TopoDS", "Topology data structures");
    py::module_ BRepBuilderAPI = m.def_submodule("BRepBuilderAPI", "BRep builder API");
    py::module_ BRepPrim = m.def_submodule("BRepPrim", "BRep primitive shapes");

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
    bind_geom(Geom);
    bind_topods(TopoDS);
    bind_brepprim(BRepPrim);
    bind_brepbuilderapi(BRepBuilderAPI);
}
