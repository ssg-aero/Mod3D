#include <pybind11/pybind11.h>

#include <gp_Ax2.hxx>
#include <BRepPrimAPI_MakeSweep.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <GeomFill_Trihedron.hxx>
#include <BRepFill_TypeOfContact.hxx>
#include <BRepBuilderAPI_PipeError.hxx>
#include <BRepBuilderAPI_TransitionMode.hxx>
#include <Law_Function.hxx>
#include <BRepSweep_Prism.hxx>
#include <BRepSweep_Revol.hxx>

#include "array_utils.hpp"

namespace py = pybind11;

void bind_brep_prim_api_sweep(py::module_ &m)
{
    
    py::class_<BRepPrimAPI_MakeSweep, BRepBuilderAPI_MakeShape>(m, "MakeSweep")
    // Note: This is an abstract base class for swept primitives
        // Concrete classes like MakeRevolution inherit from this
        .def("first_shape", &BRepPrimAPI_MakeSweep::FirstShape, py::return_value_policy::reference_internal,
             "Returns the TopoDS Shape of the bottom of the sweep")
        .def("last_shape", &BRepPrimAPI_MakeSweep::LastShape, py::return_value_policy::reference_internal,
          "Returns the TopoDS Shape of the top of the sweep")
          ;

    py::class_<BRepOffsetAPI_MakePipe, BRepPrimAPI_MakeSweep>(m, "MakePipe")
        // Constructors
        .def(py::init<const TopoDS_Wire&, const TopoDS_Shape&>(),
             py::arg("spine"), py::arg("profile"),
             "Constructs a pipe by sweeping the shape Profile along the wire Spine")
        .def(py::init<const TopoDS_Wire&, const TopoDS_Shape&, const GeomFill_Trihedron, const Standard_Boolean>(),
             py::arg("spine"), py::arg("profile"), py::arg("mode"), py::arg("force_approx_c1") = false,
             "Same as previous but with setting of mode of sweep and C1 approximation flag")
        
        // Methods
        .def("pipe", &BRepOffsetAPI_MakePipe::Pipe, py::return_value_policy::reference_internal,
             "Returns the internal BRepFill_Pipe object")
        .def("generated", py::overload_cast<const TopoDS_Shape&, const TopoDS_Shape&>(&BRepOffsetAPI_MakePipe::Generated),
             py::arg("spine_shape"), py::arg("profile_shape"),
             "Returns the shape generated from the given spine and profile shapes")
        .def("error_on_surface", &BRepOffsetAPI_MakePipe::ErrorOnSurface,
             "Returns the maximum error on the generated surface")
    ;

    py::enum_<BRepFill_TypeOfContact>(m, "TypeOfContact")
        .value("NoContact", BRepFill_NoContact)
        .value("Contact", BRepFill_Contact)
        .value("ContactOnBorder", BRepFill_ContactOnBorder)
        .export_values();

    py::enum_<BRepBuilderAPI_PipeError>(m, "PipeError")
        .value("PipeDone", BRepBuilderAPI_PipeDone)
        .value("PipeNotDone", BRepBuilderAPI_PipeNotDone)
        .value("PlaneNotIntersectGuide", BRepBuilderAPI_PlaneNotIntersectGuide)
        .value("ImpossibleContact", BRepBuilderAPI_ImpossibleContact)
        .export_values();

    py::enum_<BRepBuilderAPI_TransitionMode>(m, "TransitionMode")
        .value("Transformed", BRepBuilderAPI_Transformed)
        .value("RightCorner", BRepBuilderAPI_RightCorner)
        .value("RoundCorner", BRepBuilderAPI_RoundCorner)
        .export_values();

    py::class_<BRepOffsetAPI_MakePipeShell, BRepPrimAPI_MakeSweep>(m, "MakePipeShell")
        // Constructor
        .def(py::init<const TopoDS_Wire&>(), py::arg("spine"),
             "Constructs the shell-generating framework defined by the wire Spine")
        
        // Mode setting methods
        .def("set_mode", py::overload_cast<const Standard_Boolean>(&BRepOffsetAPI_MakePipeShell::SetMode),
             py::arg("is_frenet") = false,
             "Sets a Frenet or CorrectedFrenet trihedron. If IsFrenet is false, a corrected Frenet trihedron is used")
        .def("set_discrete_mode", &BRepOffsetAPI_MakePipeShell::SetDiscreteMode,
             "Sets a Discrete trihedron to perform the sweeping")
        .def("set_mode", py::overload_cast<const gp_Ax2&>(&BRepOffsetAPI_MakePipeShell::SetMode),
             py::arg("axe"), "Sets a fixed trihedron - all sections will be parallel")
        .def("set_mode", py::overload_cast<const gp_Dir&>(&BRepOffsetAPI_MakePipeShell::SetMode),
             py::arg("binormal"), "Sets a fixed BiNormal direction - angular relations will be constant")
        .def("set_mode", py::overload_cast<const TopoDS_Shape&>(&BRepOffsetAPI_MakePipeShell::SetMode),
             py::arg("spine_support"), "Sets support to the spine to define the BiNormal")
        .def("set_mode", py::overload_cast<const TopoDS_Wire&, const Standard_Boolean, const BRepFill_TypeOfContact>(&BRepOffsetAPI_MakePipeShell::SetMode),
             py::arg("auxiliary_spine"), py::arg("curvilinear_equivalence"), py::arg("keep_contact") = BRepFill_NoContact,
             "Sets an auxiliary spine to define the Normal")
        
        // Profile methods
        .def("add", py::overload_cast<const TopoDS_Shape&, const Standard_Boolean, const Standard_Boolean>(&BRepOffsetAPI_MakePipeShell::Add),
             py::arg("profile"), py::arg("with_contact") = false, py::arg("with_correction") = false,
             "Adds a section Profile. Point on spine computed automatically")
        .def("add", py::overload_cast<const TopoDS_Shape&, const TopoDS_Vertex&, const Standard_Boolean, const Standard_Boolean>(&BRepOffsetAPI_MakePipeShell::Add),
             py::arg("profile"), py::arg("location"), py::arg("with_contact") = false, py::arg("with_correction") = false,
             "Adds a section Profile at the given Location on spine")
        .def("set_law", py::overload_cast<const TopoDS_Shape&, const opencascade::handle<Law_Function>&, const Standard_Boolean, const Standard_Boolean>(&BRepOffsetAPI_MakePipeShell::SetLaw),
             py::arg("profile"), py::arg("law"), py::arg("with_contact") = false, py::arg("with_correction") = false,
             "Sets evolution law with homotetic law defined by function")
        .def("set_law", py::overload_cast<const TopoDS_Shape&, const opencascade::handle<Law_Function>&, const TopoDS_Vertex&, const Standard_Boolean, const Standard_Boolean>(&BRepOffsetAPI_MakePipeShell::SetLaw),
             py::arg("profile"), py::arg("law"), py::arg("location"), py::arg("with_contact") = false, py::arg("with_correction") = false,
             "Sets evolution law with homotetic law defined by function at given location")
        .def("delete", &BRepOffsetAPI_MakePipeShell::Delete, py::arg("profile"),
             "Removes the section Profile")
        
        // Status and configuration
        .def("is_ready", &BRepOffsetAPI_MakePipeShell::IsReady,
             "Returns true if ready to build (has profile definition)")
        .def("get_status", &BRepOffsetAPI_MakePipeShell::GetStatus,
             "Get status when Simulate or Build failed")
        .def("set_tolerance", &BRepOffsetAPI_MakePipeShell::SetTolerance,
             py::arg("tol_3d") = 1.0e-4, py::arg("bound_tol") = 1.0e-4, py::arg("tol_angular") = 1.0e-2,
             "Sets tolerance values")
        .def("set_max_degree", &BRepOffsetAPI_MakePipeShell::SetMaxDegree, py::arg("new_max_degree"),
             "Define maximum V degree of resulting surface")
        .def("set_max_segments", &BRepOffsetAPI_MakePipeShell::SetMaxSegments, py::arg("new_max_segments"),
             "Define maximum number of spans in V-direction")
        .def("set_force_approx_c1", &BRepOffsetAPI_MakePipeShell::SetForceApproxC1, py::arg("force_approx_c1"),
             "Set flag to attempt C1-continuous surface approximation")
        .def("set_transition_mode", &BRepOffsetAPI_MakePipeShell::SetTransitionMode,
             py::arg("mode") = BRepBuilderAPI_Transformed,
             "Sets transition mode for discontinuities on spine")
        
        // Simulation and building
        .def("simulate", [](BRepOffsetAPI_MakePipeShell& self, const Standard_Integer number_of_section) {
            TopTools_ListOfShape result;
            self.Simulate(number_of_section, result);
            return occt_container_to_py_list(result);
        }, py::arg("number_of_section"),
        "Simulates the resulting shape by calculating cross-sections")
        .def("make_solid", &BRepOffsetAPI_MakePipeShell::MakeSolid,
             "Transforms the sweeping Shell into Solid")
        .def("error_on_surface", &BRepOffsetAPI_MakePipeShell::ErrorOnSurface,
             "Returns maximum error on generated surface")
        .def("profiles", [](BRepOffsetAPI_MakePipeShell& self) {
            TopTools_ListOfShape profiles;
            self.Profiles(profiles);
            return occt_container_to_py_list(profiles);
        }, "Returns the list of original profiles")
        .def("spine", &BRepOffsetAPI_MakePipeShell::Spine, py::return_value_policy::reference_internal,
             "Returns the spine")
    ;

    
    py::class_<BRepPrimAPI_MakePrism, BRepPrimAPI_MakeSweep>(m, "MakePrism")
        // Constructors
        .def(py::init<const TopoDS_Shape&, const gp_Vec&, const Standard_Boolean, const Standard_Boolean>(),
             py::arg("shape"), py::arg("vector"), py::arg("copy") = false, py::arg("canonize") = true,
             "Builds the prism of base S and vector V. If Copy is true, S is copied")
        .def(py::init<const TopoDS_Shape&, const gp_Dir&, const Standard_Boolean, const Standard_Boolean, const Standard_Boolean>(),
             py::arg("shape"), py::arg("direction"), py::arg("infinite") = true, py::arg("copy") = false, py::arg("canonize") = true,
             "Builds a semi-infinite or infinite prism. If Inf is true the prism is infinite")
        
        // Methods
        .def("prism", &BRepPrimAPI_MakePrism::Prism, py::return_value_policy::reference_internal,
             "Returns the internal sweeping algorithm")
        .def("first_shape", py::overload_cast<>(&BRepPrimAPI_MakePrism::FirstShape),
             "Returns the TopoDS Shape of the bottom of the prism")
        .def("last_shape", py::overload_cast<>(&BRepPrimAPI_MakePrism::LastShape),
             "Returns the TopoDS Shape of the top of the prism")
        .def("first_shape", py::overload_cast<const TopoDS_Shape&>(&BRepPrimAPI_MakePrism::FirstShape),
             py::arg("the_shape"), "Returns the bottom shape generated with theShape")
        .def("last_shape", py::overload_cast<const TopoDS_Shape&>(&BRepPrimAPI_MakePrism::LastShape),
             py::arg("the_shape"), "Returns the top shape generated with theShape")
        .def("is_deleted", &BRepPrimAPI_MakePrism::IsDeleted, py::arg("shape"),
             "Returns true if the shape S has been deleted")
    ;

    py::class_<BRepPrimAPI_MakeRevol, BRepPrimAPI_MakeSweep>(m, "MakeRevol")
        // Constructors
        .def(py::init<const TopoDS_Shape&, const gp_Ax1&, const double, const Standard_Boolean>(),
             py::arg("shape"), py::arg("axis"), py::arg("angle"), py::arg("copy") = false,
             "Builds the Revol of base S, axis A and angle D. If Copy is true, S is copied")
        .def(py::init<const TopoDS_Shape&, const gp_Ax1&, const Standard_Boolean>(),
             py::arg("shape"), py::arg("axis"), py::arg("copy") = false,
             "Builds the Revol of base S, axis A and angle 2*Pi. If Copy is true, S is copied")
        
        // Methods
        .def("revol", &BRepPrimAPI_MakeRevol::Revol, py::return_value_policy::reference_internal,
             "Returns the internal sweeping algorithm")
        .def("first_shape", py::overload_cast<>(&BRepPrimAPI_MakeRevol::FirstShape),
             "Returns the first shape of the revol (coinciding with the generating shape)")
        .def("last_shape", py::overload_cast<>(&BRepPrimAPI_MakeRevol::LastShape),
             "Returns the TopoDS Shape of the end of the revol")
        .def("first_shape", py::overload_cast<const TopoDS_Shape&>(&BRepPrimAPI_MakeRevol::FirstShape),
             py::arg("the_shape"), "Returns the first shape generated with theShape")
        .def("last_shape", py::overload_cast<const TopoDS_Shape&>(&BRepPrimAPI_MakeRevol::LastShape),
             py::arg("the_shape"), "Returns the last shape generated with theShape")
        .def("is_deleted", &BRepPrimAPI_MakeRevol::IsDeleted, py::arg("shape"),
             "Returns true if the shape S has been deleted")
        .def("has_degenerated", &BRepPrimAPI_MakeRevol::HasDegenerated,
             "Check if there are degenerated edges in the result")
        .def("degenerated", [](BRepPrimAPI_MakeRevol& self) {
            return occt_container_to_py_list(self.Degenerated());
        }, "Returns the list of degenerated edges")
    ;
}