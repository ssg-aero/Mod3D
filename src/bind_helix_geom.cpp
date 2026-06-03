#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>

#include <Standard_Version.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

#if OCC_VERSION_HEX >= 0x080000

#include <HelixGeom_Tools.hxx>
#include <Geom_BSplineCurve.hxx>

#include <stdexcept>
#include <string>

void bind_helix_geom(py::module_ &m) {
    // =========================================================================
    // HelixGeom - analytic helix curves approximated as B-splines.
    //   New package in OCCT 8.0 (TKHelix). We surface the high-value
    //   one-call approximation entry point; the OCCT call already returns a
    //   Geom_BSplineCurve, so it lives in core (no extend/ wrapper needed).
    // =========================================================================
    m.def("appr_helix",
          [](double t1, double t2, double pitch, double r_start,
             double taper_angle, bool cw, double tol) {
              opencascade::handle<Geom_BSplineCurve> bspl;
              double max_error = 0.0;
              const int rc = HelixGeom_Tools::ApprHelix(
                  t1, t2, pitch, r_start, taper_angle, cw, tol, bspl, max_error);
              if (rc != 0 || bspl.IsNull()) {
                  throw std::runtime_error(
                      "HelixGeom appr_helix failed (error code " +
                      std::to_string(rc) + ")");
              }
              return py::make_tuple(bspl, max_error);
          },
          py::arg("t1"), py::arg("t2"), py::arg("pitch"), py::arg("r_start"),
          py::arg("taper_angle") = 0.0, py::arg("cw") = false,
          py::arg("tol") = 1e-6,
          "Approximate an analytic helix as a Geom.BSplineCurve.\n\n"
          "t1, t2      : start/end angular parameters (radians)\n"
          "pitch       : vertical distance per full turn (2*pi radians)\n"
          "r_start     : radius at t1\n"
          "taper_angle : radius variation angle in radians (0 = cylindrical)\n"
          "cw          : True for clockwise, False for counter-clockwise\n"
          "tol         : approximation tolerance\n\n"
          "Returns a tuple (curve, max_error). Raises RuntimeError on failure.\n"
          "New in OCCT 8.0.");
}

#else  // OCC_VERSION_HEX < 0x080000

void bind_helix_geom(py::module_ &) {}

#endif
