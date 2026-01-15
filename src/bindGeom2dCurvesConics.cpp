#include <pybind11/pybind11.h>

#include <Standard_Handle.hxx>
#include <Geom2d_Conic.hxx>
#include <Geom2d_Circle.hxx>
#include <Geom2d_Ellipse.hxx>
#include <Geom2d_Hyperbola.hxx>
#include <Geom2d_Parabola.hxx>

#include <gp_Circ2d.hxx>
#include <gp_Elips2d.hxx>
#include <gp_Hypr2d.hxx>
#include <gp_Parab2d.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);


void bind_geom2d_curves_conics(py::module_ &m)
{
     // =========================================================================
    // Geom2d_Conic - Base class for conics
    // =========================================================================
    py::class_<Geom2d_Conic, opencascade::handle<Geom2d_Conic>, Geom2d_Curve>(m, "Conic2d",
        "Abstract base class for conic curves in 2D space.\n"
        "Defines common behavior for Circle, Ellipse, Hyperbola and Parabola.")

        .def_property("location", &Geom2d_Conic::Location, &Geom2d_Conic::SetLocation,
             "Get or set the location point (center or apex)")
        .def_property("position", &Geom2d_Conic::Position, &Geom2d_Conic::SetAxis,
                "Get or set the local coordinate system of the conic")
        .def_property("x_axis", &Geom2d_Conic::XAxis, &Geom2d_Conic::SetXAxis,
                "Get or set the X axis of the conic")
        .def_property("y_axis", &Geom2d_Conic::YAxis, &Geom2d_Conic::SetYAxis,
                "Get or set the Y axis of the conic")
    
        // Properties
        .def_property_readonly("eccentricity", &Geom2d_Conic::Eccentricity,
             "Returns the eccentricity of the conic")
    ;

    // =========================================================================
    // Geom2d_Circle
    // =========================================================================
    py::class_<Geom2d_Circle, opencascade::handle<Geom2d_Circle>, Geom2d_Conic>(m, "Circle2d",
        "Describes a circle in 2D space.\n"
        "Parameterization: P(U) = O + R*Cos(U)*XDir + R*Sin(U)*YDir\n"
        "Parameter range: [0, 2*Pi), period = 2*Pi")
        
        // Constructors
        .def(py::init([](const gp_Circ2d& c) {
            return opencascade::handle<Geom2d_Circle>(new Geom2d_Circle(c));
        }), py::arg("c"),
             "Constructs a circle by conversion of gp_Circ2d")
        .def(py::init([](const gp_Ax2d& a, double radius, bool sense) {
            return opencascade::handle<Geom2d_Circle>(new Geom2d_Circle(a, radius, sense));
        }), py::arg("a"), py::arg("radius"), py::arg("sense") = true,
             "Constructs a circle with axis A and radius.\n"
             "sense=True for counter-clockwise, False for clockwise")
        .def(py::init([](const gp_Ax22d& a, double radius) {
            return opencascade::handle<Geom2d_Circle>(new Geom2d_Circle(a, radius));
        }), py::arg("a"), py::arg("radius"),
             "Constructs a circle with coordinate system A and radius")
        
        // Properties
        .def_property("circ2d", &Geom2d_Circle::Circ2d, &Geom2d_Circle::SetCirc2d)
        .def_property("radius", &Geom2d_Circle::Radius, &Geom2d_Circle::SetRadius)
        .def_property("radius", &Geom2d_Circle::Radius, &Geom2d_Circle::SetRadius)
    ;

    // =========================================================================
    // Geom2d_Ellipse
    // =========================================================================
    py::class_<Geom2d_Ellipse, opencascade::handle<Geom2d_Ellipse>, Geom2d_Conic>(m, "Ellipse2d",
        "Describes an ellipse in 2D space.\n"
        "Parameterization: P(U) = O + MajorRad*Cos(U)*XDir + MinorRad*Sin(U)*YDir\n"
        "Parameter range: [0, 2*Pi), period = 2*Pi")
        
        // Constructors
        .def(py::init([](const gp_Elips2d& e) {
            return opencascade::handle<Geom2d_Ellipse>(new Geom2d_Ellipse(e));
        }), py::arg("e"),
             "Constructs an ellipse by conversion of gp_Elips2d")
        .def(py::init([](const gp_Ax2d& a, double major_radius, double minor_radius, bool sense) {
            return opencascade::handle<Geom2d_Ellipse>(new Geom2d_Ellipse(a, major_radius, minor_radius, sense));
        }), py::arg("a"), py::arg("major_radius"), py::arg("minor_radius"), py::arg("sense") = true,
             "Constructs an ellipse with given axis and radii")
        .def(py::init([](const gp_Ax22d& a, double major_radius, double minor_radius) {
            return opencascade::handle<Geom2d_Ellipse>(new Geom2d_Ellipse(a, major_radius, minor_radius));
        }), py::arg("a"), py::arg("major_radius"), py::arg("minor_radius"),
             "Constructs an ellipse with coordinate system A and radii")
        
        // Geometric properties
        .def_property_readonly("directrix1", &Geom2d_Ellipse::Directrix1,
             "Returns the first directrix")
        .def_property_readonly("directrix2", &Geom2d_Ellipse::Directrix2,
             "Returns the second directrix")
        .def_property_readonly("focal", &Geom2d_Ellipse::Focal,
             "Returns the focal distance")
        .def_property_readonly("focus1", &Geom2d_Ellipse::Focus1,
             "Returns the first focus")
        .def_property_readonly("focus2", &Geom2d_Ellipse::Focus2,
             "Returns the second focus")
        .def_property_readonly("parameter", &Geom2d_Ellipse::Parameter,
             "Returns the parameter p = (1 - e²) * MajorRadius")
        
        // Properties
        .def_property("elips2d", &Geom2d_Ellipse::Elips2d, &Geom2d_Ellipse::SetElips2d)
        .def_property("major_radius", &Geom2d_Ellipse::MajorRadius, &Geom2d_Ellipse::SetMajorRadius)
        .def_property("minor_radius", &Geom2d_Ellipse::MinorRadius, &Geom2d_Ellipse::SetMinorRadius)
    ;

    // =========================================================================
    // Geom2d_Hyperbola
    // =========================================================================
    py::class_<Geom2d_Hyperbola, opencascade::handle<Geom2d_Hyperbola>, Geom2d_Conic>(m, "Hyperbola2d",
        "Describes a branch of a hyperbola in 2D space.\n"
        "Parameterization: P(U) = O + MajRad*Cosh(U)*XDir + MinRad*Sinh(U)*YDir\n"
        "Parameter range: ]-infinite, +infinite[")
        
        // Constructors
        .def(py::init([](const gp_Hypr2d& h) {
            return opencascade::handle<Geom2d_Hyperbola>(new Geom2d_Hyperbola(h));
        }), py::arg("h"),
             "Constructs a hyperbola by conversion of gp_Hypr2d")
        .def(py::init([](const gp_Ax2d& a, double major_radius, double minor_radius, bool sense) {
            return opencascade::handle<Geom2d_Hyperbola>(new Geom2d_Hyperbola(a, major_radius, minor_radius, sense));
        }), py::arg("a"), py::arg("major_radius"), py::arg("minor_radius"), py::arg("sense") = true,
             "Constructs a hyperbola with given axis and radii")
        .def(py::init([](const gp_Ax22d& a, double major_radius, double minor_radius) {
            return opencascade::handle<Geom2d_Hyperbola>(new Geom2d_Hyperbola(a, major_radius, minor_radius));
        }), py::arg("a"), py::arg("major_radius"), py::arg("minor_radius"),
             "Constructs a hyperbola with coordinate system A and radii")

        // Geometric properties
        .def_property_readonly("asymptote1", &Geom2d_Hyperbola::Asymptote1,
             "Returns the first asymptote")
        .def_property_readonly("asymptote2", &Geom2d_Hyperbola::Asymptote2,
             "Returns the second asymptote")
        .def_property_readonly("conjugate_branch1", &Geom2d_Hyperbola::ConjugateBranch1,
             "Returns the first conjugate branch")
        .def_property_readonly("conjugate_branch2", &Geom2d_Hyperbola::ConjugateBranch2,
             "Returns the second conjugate branch")
        .def_property_readonly("directrix1", &Geom2d_Hyperbola::Directrix1,
             "Returns the first directrix")
        .def_property_readonly("directrix2", &Geom2d_Hyperbola::Directrix2,
             "Returns the second directrix")
        .def_property_readonly("focal", &Geom2d_Hyperbola::Focal,
             "Returns the focal distance")
        .def_property_readonly("focus1", &Geom2d_Hyperbola::Focus1,
             "Returns the first focus")
        .def_property_readonly("focus2", &Geom2d_Hyperbola::Focus2,
             "Returns the second focus")
        .def_property_readonly("other_branch", &Geom2d_Hyperbola::OtherBranch,
             "Returns the other branch")
        .def_property_readonly("parameter", &Geom2d_Hyperbola::Parameter,
             "Returns the parameter")
        
        // Properties
        
        .def_property("major_radius", &Geom2d_Hyperbola::MajorRadius, &Geom2d_Hyperbola::SetMajorRadius)
        .def_property("minor_radius", &Geom2d_Hyperbola::MinorRadius, &Geom2d_Hyperbola::SetMinorRadius)
    ;

    // =========================================================================
    // Geom2d_Parabola
    // =========================================================================
    py::class_<Geom2d_Parabola, opencascade::handle<Geom2d_Parabola>, Geom2d_Conic>(m, "Parabola2d",
        "Describes a parabola in 2D space.\n"
        "Parameterization: P(U) = O + U²/(4*F)*XDir + U*YDir\n"
        "Parameter range: ]-infinite, +infinite[")
        
        // Constructors
        .def(py::init([](const gp_Parab2d& prb) {
            return opencascade::handle<Geom2d_Parabola>(new Geom2d_Parabola(prb));
        }), py::arg("prb"),
             "Constructs a parabola by conversion of gp_Parab2d")
        .def(py::init([](const gp_Ax22d& a, double focal) {
            return opencascade::handle<Geom2d_Parabola>(new Geom2d_Parabola(a, focal));
        }), py::arg("a"), py::arg("focal"),
             "Constructs a parabola with coordinate system A and focal length")
        .def(py::init([](const gp_Ax2d& a, double focal, bool sense) {
            return opencascade::handle<Geom2d_Parabola>(new Geom2d_Parabola(a, focal, sense));
        }), py::arg("a"), py::arg("focal"), py::arg("sense") = true,
             "Constructs a parabola with axis A and focal length")
        .def(py::init([](const gp_Ax2d& d, double f, bool sense) {
            return opencascade::handle<Geom2d_Parabola>(new Geom2d_Parabola(d, f, sense));
        }), py::arg("d"), py::arg("f"), py::arg("sense") = true,
             "Constructs a parabola from directrix D and focus F")
        
        
        // Geometric properties
        .def_property_readonly("directrix", &Geom2d_Parabola::Directrix,
             "Returns the directrix of this parabola")
        .def_property_readonly("focus", &Geom2d_Parabola::Focus,
             "Returns the focus of this parabola")
        .def_property_readonly("parameter", &Geom2d_Parabola::Parameter,
             "Returns the parameter (twice the focal length)")
        
        // Properties
        .def_property("parab2d", &Geom2d_Parabola::Parab2d, &Geom2d_Parabola::SetParab2d)
        .def_property("focal", &Geom2d_Parabola::Focal, &Geom2d_Parabola::SetFocal)
    ;

}