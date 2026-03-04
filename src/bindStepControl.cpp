#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <STEPControl_Writer.hxx>
#include <STEPControl_Reader.hxx>
#include <STEPControl_StepModelType.hxx>
#include <IFSelect_ReturnStatus.hxx>
#include <XSControl_WorkSession.hxx>
#include <StepData_StepModel.hxx>
#include <TopoDS_Shape.hxx>
#include <TColStd_SequenceOfAsciiString.hxx>
#include <Message_ProgressRange.hxx>

namespace py = pybind11;

// Declare opencascade::handle as a holder type
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_step_control(py::module_ &m)
{
    // -------------------------------------------------------------------------
    // Enumerations
    // -------------------------------------------------------------------------
    py::enum_<STEPControl_StepModelType>(m, "StepModelType",
        "Defines the STEP entity type to be output during translation.")
        .value("AsIs", STEPControl_AsIs,
            "Translates a shape to its highest possible STEP representation.")
        .value("ManifoldSolidBrep", STEPControl_ManifoldSolidBrep,
            "Translates to a manifold_solid_brep or brep_with_voids entity.")
        .value("BrepWithVoids", STEPControl_BrepWithVoids,
            "Translates to a brep_with_voids entity.")
        .value("FacetedBrep", STEPControl_FacetedBrep,
            "Translates to a faceted_brep entity.")
        .value("FacetedBrepAndBrepWithVoids", STEPControl_FacetedBrepAndBrepWithVoids,
            "Translates to faceted_brep and brep_with_voids.")
        .value("ShellBasedSurfaceModel", STEPControl_ShellBasedSurfaceModel,
            "Translates to a shell_based_surface_model entity.")
        .value("GeometricCurveSet", STEPControl_GeometricCurveSet,
            "Translates to a geometric_curve_set entity.")
        .value("Hybrid", STEPControl_Hybrid,
            "Hybrid representation.")
        .export_values();

    py::enum_<IFSelect_ReturnStatus>(m, "ReturnStatus",
        "Return status for STEP I/O operations.")
        .value("RetVoid", IFSelect_RetVoid, "No result.")
        .value("RetDone", IFSelect_RetDone, "Operation succeeded.")
        .value("RetError", IFSelect_RetError, "Error occurred.")
        .value("RetFail", IFSelect_RetFail, "Operation failed.")
        .value("RetStop", IFSelect_RetStop, "Operation stopped.")
        .export_values();

    // -------------------------------------------------------------------------
    // STEPControl_Writer
    // -------------------------------------------------------------------------
    py::class_<STEPControl_Writer>(m, "Writer",
        "Creates and writes STEP files from Open CASCADE models.\n\n"
        "A STEP file can be written to an existing file or a new one.\n"
        "Translation can be performed in one or several operations.\n"
        "Each operation produces a distinct root entity in the STEP file.\n\n"
        "Typical use::\n\n"
        "    writer = mod3d.Writer()\n"
        "    writer.transfer(shape, mod3d.StepModelType.AsIs)\n"
        "    writer.write('output.step')\n")

        // Constructors
        .def(py::init<>(),
            "Creates a Writer from scratch.")

        // Tolerance
        .def("set_tolerance", &STEPControl_Writer::SetTolerance,
            py::arg("tol"),
            "Sets a length-measure value written to uncertainty-measure-with-unit\n"
            "when the next shape is translated.\n\n"
            "Parameters\n"
            "----------\n"
            "tol : float\n"
            "    Tolerance value in current length unit.")

        .def("unset_tolerance", &STEPControl_Writer::UnsetTolerance,
            "Unsets the tolerance previously forced by :meth:`set_tolerance`.")

        // Model access
        .def("model",
            [](STEPControl_Writer& self, bool newone) {
                return self.Model(newone ? Standard_True : Standard_False);
            },
            py::arg("newone") = false,
            "Returns the produced STEP model.\n\n"
            "Produces a new model if none exists yet or if *newone* is ``True``.\n"
            "This allows editing product or header data before writing.\n\n"
            "Parameters\n"
            "----------\n"
            "newone : bool, optional\n"
            "    If ``True``, force creation of a fresh model. Default is ``False``.")

        // Transfer
        .def("transfer",
            [](STEPControl_Writer& self,
               const TopoDS_Shape& sh,
               const STEPControl_StepModelType mode,
               const Standard_Boolean compgraph) {
                return self.Transfer(sh, mode, compgraph);
            },
            py::arg("shape"),
            py::arg("mode"),
            py::arg("compgraph") = true,
            "Translates a shape to a STEP entity and adds it to the model.\n\n"
            "Parameters\n"
            "----------\n"
            "shape : TopoDS_Shape\n"
            "    The shape to translate.\n"
            "mode : StepModelType\n"
            "    STEP representation type (``AsIs``, ``ManifoldSolidBrep``, etc.).\n"
            "compgraph : bool, optional\n"
            "    Whether to compute the graph. Default is ``True``.\n\n"
            "Returns\n"
            "-------\n"
            "ReturnStatus\n"
            "    ``RetDone`` on success.")

        // Write to file
        .def("write", &STEPControl_Writer::Write,
            py::arg("filename"),
            "Writes the STEP model to a file.\n\n"
            "Parameters\n"
            "----------\n"
            "filename : str\n"
            "    Path of the output STEP file.\n\n"
            "Returns\n"
            "-------\n"
            "ReturnStatus\n"
            "    ``RetDone`` on success.")

        // Statistics
        .def("print_stats_transfer", &STEPControl_Writer::PrintStatsTransfer,
            py::arg("what"), py::arg("mode") = 0,
            "Displays statistics for the last translation.\n\n"
            "Parameters\n"
            "----------\n"
            "what : int\n"
            "    Kind of statistics:\n\n"
            "    - 0 — General statistics (number of translated roots, warnings, fails).\n"
            "    - 1 — Root results.\n"
            "    - 2 — Statistics for all checked entities.\n"
            "    - 3 — List of translated entities.\n"
            "    - 4 — Warning and fail messages.\n"
            "    - 5 — Fail messages only.\n"
            "mode : int, optional\n"
            "    Detail level when *what* is 1–3:\n\n"
            "    - 0 — List numbers of STEP entities.\n"
            "    - 1 — Number, identifier, type and result type per entity.\n"
            "    - 2 — Maximum information per entity (checks).\n"
            "    - 3 — Number of entities per STEP entity type.")
        ;

    // -------------------------------------------------------------------------
    // STEPControl_Reader
    // -------------------------------------------------------------------------
    py::class_<STEPControl_Reader>(m, "Reader",
        "Reads STEP files and translates their contents into Open CASCADE shapes.\n\n"
        "Inherits from XSControl_Reader. The workflow is:\n\n"
        "1. :meth:`read_file` — load a STEP file into memory.\n"
        "2. :meth:`transfer_roots` or :meth:`transfer_root` — translate entities to shapes.\n"
        "3. :meth:`shape` / :meth:`one_shape` — retrieve translated shapes.\n\n"
        "Typical use::\n\n"
        "    reader = mod3d.Reader()\n"
        "    status = reader.read_file('model.step')\n"
        "    reader.transfer_roots()\n"
        "    shape = reader.one_shape()\n")

        // Constructors
        .def(py::init<>(),
            "Creates a Reader with an empty STEP model.")

        // File I/O
        .def("read_file",
            [](STEPControl_Reader& self, const std::string& filename) {
                return self.ReadFile(filename.c_str());
            },
            py::arg("filename"),
            "Loads a STEP file into memory.\n\n"
            "Parameters\n"
            "----------\n"
            "filename : str\n"
            "    Path to the STEP file.\n\n"
            "Returns\n"
            "-------\n"
            "ReturnStatus\n"
            "    ``RetDone`` on success, ``RetError`` / ``RetFail`` otherwise.")

        // Transfer
        .def("transfer_root",
            [](STEPControl_Reader& self, int num) {
                return self.TransferRoot(num);
            },
            py::arg("num") = 1,
            "Translates a single root entity identified by its rank.\n\n"
            "Parameters\n"
            "----------\n"
            "num : int, optional\n"
            "    Rank of the root to translate (1-based). Default is 1.\n\n"
            "Returns\n"
            "-------\n"
            "bool\n"
            "    ``True`` if a shape was produced.")

        .def("transfer_roots",
            [](STEPControl_Reader& self) {
                return self.TransferRoots();
            },
            "Translates all transferable root entities.\n\n"
            "Clears existing output shapes first.\n\n"
            "Returns\n"
            "-------\n"
            "int\n"
            "    Number of successfully translated roots.")

        .def_property_readonly("nb_roots_for_transfer", &STEPControl_Reader::NbRootsForTransfer,
            "Returns the number of root entities that are candidates for translation.\n\n"
            "Returns\n"
            "-------\n"
            "int")

        // Shape retrieval
        .def_property_readonly("nb_shapes", &STEPControl_Reader::NbShapes,
            "Returns the number of shapes produced by translation.\n\n"
            "Returns\n"
            "-------\n"
            "int")

        .def("shape",
            [](STEPControl_Reader& self, int num) {
                return self.Shape(num);
            },
            py::arg("num") = 1,
            "Returns the shape produced at rank *num*.\n\n"
            "Parameters\n"
            "----------\n"
            "num : int, optional\n"
            "    Rank of the result shape (1-based). Default is 1.\n\n"
            "Returns\n"
            "-------\n"
            "TopoDS_Shape")

        .def_property_readonly("one_shape", &STEPControl_Reader::OneShape,
            "Returns all results as a single compound shape.\n\n"
            "Returns a null shape if nothing was translated, the shape directly if\n"
            "there is exactly one result, or a compound if there are multiple results.\n\n"
            "Returns\n"
            "-------\n"
            "TopoDS_Shape")

        .def("clear_shapes", &STEPControl_Reader::ClearShapes,
            "Clears the list of accumulated result shapes.\n\n"
            "Call this before starting a new batch of translations.")

        // Unit information
        .def_property_readonly("file_units",
            [](STEPControl_Reader& self) {
                TColStd_SequenceOfAsciiString lengthNames, angleNames, solidAngleNames;
                self.FileUnits(lengthNames, angleNames, solidAngleNames);

                auto to_list = [](const TColStd_SequenceOfAsciiString& seq) {
                    std::vector<std::string> result;
                    result.reserve(seq.Size());
                    for (int i = 1; i <= seq.Size(); ++i)
                        result.push_back(seq.Value(i).ToCString());
                    return result;
                };

                return py::make_tuple(
                    to_list(lengthNames),
                    to_list(angleNames),
                    to_list(solidAngleNames)
                );
            },
            "Returns the unit names found in the STEP file.\n\n"
            "Returns\n"
            "-------\n"
            "tuple[list[str], list[str], list[str]]\n"
            "    A triple ``(length_units, angle_units, solid_angle_units)``.")

        .def("set_system_length_unit", &STEPControl_Reader::SetSystemLengthUnit,
            py::arg("length_unit"),
            "Sets the system length unit used during transfer.\n\n"
            "Parameters\n"
            "----------\n"
            "length_unit : float\n"
            "    Length unit value.")

        .def("system_length_unit", &STEPControl_Reader::SystemLengthUnit,
            "Returns the system length unit used during transfer.\n\n"
            "Returns\n"
            "-------\n"
            "float")

        // Model access
        .def_property_readonly("step_model", &STEPControl_Reader::StepModel,
            "Returns the loaded STEP model, or ``None`` if no file has been read.\n\n"
            "Returns\n"
            "-------\n"
            "StepData_StepModel | None")
        ;
}