#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <StepData_StepModel.hxx>
#include <Interface_InterfaceModel.hxx>
#include <TCollection_HAsciiString.hxx>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_step_data(py::module_ &m)
{
    // -------------------------------------------------------------------------
    // StepData_StepModel
    // -------------------------------------------------------------------------
    py::class_<StepData_StepModel, opencascade::handle<StepData_StepModel>>(m, "StepModel",
        "Provides access to the entities and header of a loaded STEP file.\n\n"
        "Obtained from :meth:`mod3d.StepControl.Reader.step_model` or\n"
        ":meth:`mod3d.StepControl.Writer.model`.")

        .def("nb_entities", &StepData_StepModel::NbEntities,
            "Returns the total number of entities in the model.\n\n"
            "Returns\n"
            "-------\n"
            "int")

        .def("__repr__", [](const StepData_StepModel& self) {
            return "<StepModel entities=" + std::to_string(self.NbEntities()) + ">";
        })
        ;
}