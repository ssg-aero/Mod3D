#include <pybind11/pybind11.h>

#include <BRepOffsetAPI_MakeEvolved.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepFill_Evolved.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <GeomAbs_JoinType.hxx>
#include <TopTools_ListOfShape.hxx>
#include <Message_ProgressRange.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <BRepPrimAPI_MakeSweep.hxx>
#include <BRepFill_TypeOfContact.hxx>
#include <BRepBuilderAPI_PipeError.hxx>
#include <BRepBuilderAPI_TransitionMode.hxx>
#include <TopoDS_Vertex.hxx>
#include <Law_Function.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>

namespace py = pybind11;

void bind_brep_offset_api(py::module_ &m)
{

    py::class_<BRepOffsetAPI_MakeEvolved, BRepBuilderAPI_MakeShape>(m, "MakeEvolved",
        "Builds evolved shapes by sweeping a profile along a spine.\n\n"
        "An evolved shape is the unlooped sweep (pipe) of a profile (wire)\n"
        "along a planar spine (face or wire) with self-intersections removed.\n\n"
        "The profile is defined in a reference frame R. The position of the\n"
        "profile at the current point of the spine is given by confusing R\n"
        "and the local referential (D0, D1, and spine normal).\n\n"
        "Coordinate system determination (is_axe_prof argument):\n"
        "- True: R is the global coordinate system\n"
        "- False: R is computed with:\n"
        "  * Origin: point on spine closest to profile\n"
        "  * X Axis: tangent to spine at this point\n"
        "  * Z Axis: normal to plane containing spine")
        
        .def(py::init<>(),
            "Creates an empty evolved shape")
        
        .def(py::init<const TopoDS_Shape&, const TopoDS_Wire&, const GeomAbs_JoinType,
                      const Standard_Boolean, const Standard_Boolean, const Standard_Boolean,
                      const Standard_Real, const Standard_Boolean, const Standard_Boolean>(),
            py::arg("spine"), py::arg("profile"),
            py::arg("join_type") = GeomAbs_Arc,
            py::arg("is_axe_prof") = true,
            py::arg("is_solid") = false,
            py::arg("is_prof_on_spine") = false,
            py::arg("tol") = 0.0000001,
            py::arg("is_volume") = false,
            py::arg("run_in_parallel") = false,
            "Constructs an evolved shape by sweeping profile along spine.\n\n"
            "Parameters:\n"
            "  spine: Wire or face to sweep along (must be planar)\n"
            "  profile: Wire profile to sweep (expected to be planar or a line)\n"
            "  join_type: Type of pipe at salient vertices (default: Arc)\n"
            "  is_axe_prof: True for global coords, False for computed frame\n"
            "  is_solid: True to complete result as solid/compound of solids\n"
            "  is_prof_on_spine: True if profile must connect with spine\n"
            "  tol: Tolerance for construction (default: 1e-7)\n"
            "  is_volume: True to remove self-intersections with MakerVolume\n"
            "  run_in_parallel: True to enable parallel computation\n\n"
            "Note: When is_volume=True, join_type, is_axe_prof, and\n"
            "is_prof_on_spine are not used")
        
        .def("evolved", &BRepOffsetAPI_MakeEvolved::Evolved,
            py::return_value_policy::reference_internal,
            "Returns the internal BRepFill_Evolved object")
        
        .def("build",
            [](BRepOffsetAPI_MakeEvolved& self) {
                self.Build();
            },
            "Builds the resulting evolved shape.\n\n"
            "Performs the sweep operation and removes self-intersections.\n"
            "Use IsDone() to check success and Shape() to retrieve result")
        
        .def("generated_shapes", &BRepOffsetAPI_MakeEvolved::GeneratedShapes,
            py::arg("spine_shape"), py::arg("prof_shape"),
            py::return_value_policy::reference_internal,
            "Returns shapes created from spine and profile subshapes.\n\n"
            "Parameters:\n"
            "  spine_shape: A subshape of the spine\n"
            "  prof_shape: A subshape of the profile\n\n"
            "Returns the list of shapes generated from these subshapes")
        
        .def_property_readonly("top", &BRepOffsetAPI_MakeEvolved::Top,
            "Returns the top face (only valid if is_solid=True in constructor)")
        
        .def_property_readonly("bottom", &BRepOffsetAPI_MakeEvolved::Bottom,
            "Returns the bottom face (only valid if is_solid=True in constructor)")
    ;

}