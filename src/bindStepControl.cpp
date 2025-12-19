#include <pybind11/pybind11.h>

#include <STEPControl_Writer.hxx>
#include <STEPControl_StepModelType.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <XSControl_WorkSession.hxx>
#include <StepData_StepModel.hxx>
#include <TopoDS_Shape.hxx>

namespace py = pybind11;

// Declare opencascade::handle as a holder type
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_step_control(py::module_ &m)
{
    // Enumerations
    py::enum_<STEPControl_StepModelType>(m, "StepModelType",
        "Defines the STEP entity type to be output during translation")
        .value("AsIs", STEPControl_AsIs,
            "Translates a shape to its highest possible STEP representation")
        .value("ManifoldSolidBrep", STEPControl_ManifoldSolidBrep,
            "Translates to a manifold_solid_brep or brep_with_voids entity")
        .value("BrepWithVoids", STEPControl_BrepWithVoids,
            "Translates to a brep_with_voids entity")
        .value("FacetedBrep", STEPControl_FacetedBrep,
            "Translates to a faceted_brep entity")
        .value("FacetedBrepAndBrepWithVoids", STEPControl_FacetedBrepAndBrepWithVoids,
            "Translates to faceted_brep and brep_with_voids")
        .value("ShellBasedSurfaceModel", STEPControl_ShellBasedSurfaceModel,
            "Translates to a shell_based_surface_model entity")
        .value("GeometricCurveSet", STEPControl_GeometricCurveSet,
            "Translates to a geometric_curve_set entity")
        .value("Hybrid", STEPControl_Hybrid,
            "Hybrid representation")
        .export_values();

    py::enum_<IFSelect_ReturnStatus>(m, "ReturnStatus",
        "Return status for STEP operations")
        .value("RetVoid", IFSelect_RetVoid, "No result")
        .value("RetDone", IFSelect_RetDone, "Operation succeeded")
        .value("RetError", IFSelect_RetError, "Error occurred")
        .value("RetFail", IFSelect_RetFail, "Operation failed")
        .value("RetStop", IFSelect_RetStop, "Operation stopped")
        .export_values();

    py::class_<STEPControl_Writer>(m, "Writer",
        "Creates and writes STEP files from Open CASCADE models.\n\n"
        "A STEP file can be written to an existing file or a new one.\n"
        "Translation can be performed in one or several operations.\n"
        "Each translation operation outputs a distinct root entity in the STEP file.")
        
        // Constructors
        .def(py::init<>(),
            "Creates a Writer from scratch")
        
        // .def(py::init<const opencascade::handle<XSControl_WorkSession>&, const Standard_Boolean>(),
        //     py::arg("ws"), py::arg("scratch") = true,
        //     "Creates a Writer from an existing Session.\n\n"
        //     "Parameters:\n"
        //     "  ws: Work session to use\n"
        //     "  scratch: If True, clears already recorded data")
        
        // Tolerance methods
        .def("set_tolerance", &STEPControl_Writer::SetTolerance,
            py::arg("tol"),
            "Sets a length-measure value that will be written to uncertainty-measure-with-unit\n"
            "when the next shape is translated")
        
        .def("unset_tolerance", &STEPControl_Writer::UnsetTolerance,
            "Unsets the tolerance formerly forced by set_tolerance")
        
        // // Work session
        // .def_property("ws", &STEPControl_Writer::WS,
        //     &STEPControl_Writer::SetWS,
        //     "Work session used in this writer"
        // )

        // Model access
        .def("model", &STEPControl_Writer::Model,
            py::arg("newone") = false,
            "Returns the produced model. Produces a new one if not yet done or if newone is True.\n"
            "This method allows for instance to edit product or header data before writing")
        
        // Transfer methods
        .def("transfer", [](STEPControl_Writer& self, 
                           const TopoDS_Shape& sh,
                           const STEPControl_StepModelType mode,
                           const Standard_Boolean compgraph) {
            return self.Transfer(sh, mode, compgraph);
        }, py::arg("shape"), 
           py::arg("mode"), 
           py::arg("compgraph") = true,
            "Translates shape to a STEP entity.\n\n"
            "Parameters:\n"
            "  shape: Shape to translate\n"
            "  mode: STEP entity type to output (AsIs, ManifoldSolidBrep, FacetedBrep, etc.)\n"
            "  compgraph: Whether to compute the graph\n\n"
            "Returns:\n"
            "  ReturnStatus indicating success or failure")
        
        // Write methods
        .def("write", &STEPControl_Writer::Write,
            py::arg("filename"),
            "Writes a STEP model to the file identified by filename.\n\n"
            "Returns:\n"
            "  ReturnStatus indicating success or failure")
        
        // Statistics
        .def("print_stats_transfer", &STEPControl_Writer::PrintStatsTransfer,
            py::arg("what"), py::arg("mode") = 0,
            "Displays statistics for the last translation.\n\n"
            "Parameters:\n"
            "  what: Kind of statistics to display:\n"
            "    0 - General statistics (translated roots, warnings, fail messages)\n"
            "    1 - Root results\n"
            "    2 - Statistics for all checked entities\n"
            "    3 - List of translated entities\n"
            "    4 - Warning and fail messages\n"
            "    5 - Fail messages only\n"
            "  mode: Used with what=1,2,3:\n"
            "    0 - List numbers of STEP entities\n"
            "    1 - Number, identifier, type and result type for each entity\n"
            "    2 - Maximum information for each entity (checks)\n"
            "    3 - Number of entities by STEP entity type\n"
            "    4 - Number of entities per result type and/or status\n"
            "    5 - Number of pairs (type and status)\n"
            "    6 - Number of pairs (type and status) AND entity list")
    ;
        
}