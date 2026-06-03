#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>

#include <Standard_Version.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

#if OCC_VERSION_HEX >= 0x080000

#include <Geom2dEval_ArchimedeanSpiralCurve.hxx>
#include <Geom2dEval_LogarithmicSpiralCurve.hxx>
#include <Geom2dEval_CircleInvoluteCurve.hxx>
#include <Geom2dEval_SineWaveCurve.hxx>
#include <Geom2d_Curve.hxx>
#include <gp_Ax2d.hxx>

void bind_geom2d_eval(py::module_ &m) {
    // =========================================================================
    // Geom2dEval - analytic 2D evaluation curves new in OCCT 8.0. Bound as
    //   genuine Geom2d_Curve subclasses (they inherit value()/d1()/... from the
    //   bound Geom2d.Curve2d base). Representative subset.
    // =========================================================================
    py::class_<Geom2dEval_ArchimedeanSpiralCurve,
               opencascade::handle<Geom2dEval_ArchimedeanSpiralCurve>,
               Geom2d_Curve>(m, "ArchimedeanSpiralCurve",
        "Archimedean spiral r(t) = initial_radius + growth_rate * t,\n"
        "in the frame `position`. New in OCCT 8.0.")
        .def(py::init<const gp_Ax2d &, double, double>(),
             py::arg("position"), py::arg("initial_radius"),
             py::arg("growth_rate"))
        .def_property_readonly("position",
                               &Geom2dEval_ArchimedeanSpiralCurve::Position)
        .def_property_readonly("initial_radius",
                               &Geom2dEval_ArchimedeanSpiralCurve::InitialRadius)
        .def_property_readonly("growth_rate",
                               &Geom2dEval_ArchimedeanSpiralCurve::GrowthRate);

    py::class_<Geom2dEval_LogarithmicSpiralCurve,
               opencascade::handle<Geom2dEval_LogarithmicSpiralCurve>,
               Geom2d_Curve>(m, "LogarithmicSpiralCurve",
        "Logarithmic spiral r(t) = scale * exp(growth_exponent * t),\n"
        "in the frame `position`. New in OCCT 8.0.")
        .def(py::init<const gp_Ax2d &, double, double>(),
             py::arg("position"), py::arg("scale"), py::arg("growth_exponent"))
        .def_property_readonly("position",
                               &Geom2dEval_LogarithmicSpiralCurve::Position)
        .def_property_readonly("scale", &Geom2dEval_LogarithmicSpiralCurve::Scale)
        .def_property_readonly("growth_exponent",
                               &Geom2dEval_LogarithmicSpiralCurve::GrowthExponent);

    py::class_<Geom2dEval_CircleInvoluteCurve,
               opencascade::handle<Geom2dEval_CircleInvoluteCurve>,
               Geom2d_Curve>(m, "CircleInvoluteCurve",
        "Involute of a circle of the given base `radius`, in the frame\n"
        "`position`. New in OCCT 8.0.")
        .def(py::init<const gp_Ax2d &, double>(),
             py::arg("position"), py::arg("radius"))
        .def_property_readonly("position",
                               &Geom2dEval_CircleInvoluteCurve::Position)
        .def_property_readonly("radius",
                               &Geom2dEval_CircleInvoluteCurve::Radius);

    py::class_<Geom2dEval_SineWaveCurve,
               opencascade::handle<Geom2dEval_SineWaveCurve>,
               Geom2d_Curve>(m, "SineWaveCurve",
        "2D sine wave y(x) = amplitude * sin(omega * x + phase), in the\n"
        "frame `position`. New in OCCT 8.0.")
        .def(py::init<const gp_Ax2d &, double, double, double>(),
             py::arg("position"), py::arg("amplitude"), py::arg("omega"),
             py::arg("phase") = 0.0)
        .def_property_readonly("position", &Geom2dEval_SineWaveCurve::Position)
        .def_property_readonly("amplitude", &Geom2dEval_SineWaveCurve::Amplitude)
        .def_property_readonly("omega", &Geom2dEval_SineWaveCurve::Omega)
        .def_property_readonly("phase", &Geom2dEval_SineWaveCurve::Phase);
}

#else // OCC_VERSION_HEX < 0x080000

void bind_geom2d_eval(py::module_ &) {}

#endif
