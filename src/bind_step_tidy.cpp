#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>

#include <Standard_Version.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

#if OCC_VERSION_HEX >= 0x080000

#include <StepTidy_DuplicateCleaner.hxx>
#include <XSControl_WorkSession.hxx>

void bind_step_tidy(py::module_ &m) {
    // =========================================================================
    // StepTidy - merge equal STEP entities and remove duplicates in a work
    //   session, shrinking the exported file. New in OCCT 8.0.
    // =========================================================================
    m.def("remove_duplicates",
          [](const opencascade::handle<XSControl_WorkSession> &ws) {
              if (ws.IsNull()) {
                  throw py::value_error("work_session must not be None");
              }
              StepTidy_DuplicateCleaner cleaner(ws);
              cleaner.Perform();
          },
          py::arg("work_session"),
          "Merge equal STEP entities and drop duplicates in the given work\n"
          "session (from a StepControl Reader/Writer's .work_session).\n\n"
          "Typical use before export:\n"
          "    writer.transfer(shape, StepControl.StepModelType.AsIs)\n"
          "    StepTidy.remove_duplicates(writer.work_session)\n"
          "    writer.write('out.step')\n\n"
          "New in OCCT 8.0.");
}

#else // OCC_VERSION_HEX < 0x080000

void bind_step_tidy(py::module_ &) {}

#endif
