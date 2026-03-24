#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <Standard_Handle.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Vec.hxx>
#include <gp_Lin.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Hypr.hxx>
#include <gp_Parab.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Conic.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_Hyperbola.hxx>
#include <Geom_Parabola.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_geom_curves_conics(py::module_ &m)
{

    py::class_<Geom_Conic, opencascade::handle<Geom_Conic>, Geom_Curve>(m, "Conic",
        "Abstract base class for conic curves in 3D space.\n"
        "Defines common behavior for Circle, Ellipse, Hyperbola and Parabola.\n"
        "A conic is positioned with a right-handed coordinate system (gp_Ax2):\n"
        "- origin is the center (or apex for parabola)\n"
        "- origin, X Direction and Y Direction define the plane of the conic\n"
        "- main Direction is the vector normal to the plane")
        
        // Properties for position and orientation
        .def_property("axis", &Geom_Conic::Axis, &Geom_Conic::SetAxis,
             "Get or set the main Axis of this conic (normal to the plane)")
        .def_property("location", &Geom_Conic::Location, &Geom_Conic::SetLocation,
             "Get or set the location point of the conic")
        .def_property("position", &Geom_Conic::Position, &Geom_Conic::SetPosition,
             "Get or set the local coordinate system of the conic")
        .def_property_readonly("x_axis", &Geom_Conic::XAxis,
             "Returns the XAxis of the conic (defines origin of parametrization)")
        .def_property_readonly("y_axis", &Geom_Conic::YAxis,
             "Returns the YAxis of the conic (perpendicular to XAxis)")
        .def_property_readonly("eccentricity", &Geom_Conic::Eccentricity,
             "Returns the eccentricity value of the conic:\n"
             "e = 0 for a circle\n"
             "0 < e < 1 for an ellipse (e = 0 if MajorRadius = MinorRadius)\n"
             "e > 1 for a hyperbola\n"
             "e = 1 for a parabola\n"
             "Raises exception for hyperbola if major radius is null")
    ;

    py::class_<Geom_Circle, opencascade::handle<Geom_Circle>, Geom_Conic>(m, "Circle",
        "Describes a circle in 3D space.\n"
        "A circle is defined by its radius and positioned with a right-handed coordinate system:\n"
        "- origin is the center of the circle\n"
        "- origin, X Direction and Y Direction define the plane of the circle\n"
        "- main Direction is the vector normal to the plane\n"
        "Parameterization: P(U) = O + R*Cos(U)*XDir + R*Sin(U)*YDir\n"
        "where U is the parameter (angle), O is origin, R is radius.\n"
        "Parameter range: [0, 2*Pi), period = 2*Pi")
        
        // Constructors
        .def(py::init([](const gp_Circ& c) {
            return opencascade::handle<Geom_Circle>(new Geom_Circle(c));
        }), py::arg("c"),
             "Constructs a circle by conversion of the gp_Circ circle")
        
        .def(py::init([](const gp_Ax2& a2, Standard_Real radius) {
            return opencascade::handle<Geom_Circle>(new Geom_Circle(a2, radius));
        }), py::arg("a2"), py::arg("radius"),
             "Constructs a circle of given radius, where a2 locates the circle:\n"
             "- center is the origin of a2\n"
             "- origin, X Direction and Y Direction of a2 define the plane\n"
             "- a2 is the local coordinate system of the circle\n"
             "Note: Radius can be 0.0. Raises exception if radius < 0")
        
        // Properties
        .def_property("circ", &Geom_Circle::Circ, &Geom_Circle::SetCirc,
             "Get or set the gp_Circ with the same geometric properties")
        .def_property("radius", &Geom_Circle::Radius, &Geom_Circle::SetRadius,
             "Get or set the radius of the circle")
    ;

    py::class_<Geom_Ellipse, opencascade::handle<Geom_Ellipse>, Geom_Conic>(m, "Ellipse",
        "Describes an ellipse in 3D space.\n"
        "An ellipse is defined by its major and minor radii and positioned with a\n"
        "right-handed coordinate system where:\n"
        "- origin is the center of the ellipse\n"
        "- X Direction defines the major axis\n"
        "- Y Direction defines the minor axis\n"
        "Parameterization: P(U) = O + MajorRad*Cos(U)*XDir + MinorRad*Sin(U)*YDir\n"
        "Parameter range: [0, 2*Pi), period = 2*Pi")
        
        // Constructors
        .def(py::init([](const gp_Elips& e) {
            return opencascade::handle<Geom_Ellipse>(new Geom_Ellipse(e));
        }), py::arg("e"),
             "Constructs an ellipse by conversion of the gp_Elips ellipse")
        
        .def(py::init([](const gp_Ax2& a2, Standard_Real major_radius, Standard_Real minor_radius) {
            return opencascade::handle<Geom_Ellipse>(new Geom_Ellipse(a2, major_radius, minor_radius));
        }), py::arg("a2"), py::arg("major_radius"), py::arg("minor_radius"),
             "Constructs an ellipse with given major and minor radii, where a2 locates the ellipse:\n"
             "- center is the origin of a2\n"
             "- X Direction defines the major axis\n"
             "- Y Direction defines the minor axis\n"
             "Raises exception if major_radius < minor_radius or minor_radius < 0")
        
        // Properties
        .def_property("elips", &Geom_Ellipse::Elips, &Geom_Ellipse::SetElips,
             "Get or set the gp_Elips with the same geometric properties")
        .def_property("major_radius", &Geom_Ellipse::MajorRadius, &Geom_Ellipse::SetMajorRadius,
             "Get or set the major radius of the ellipse")
        .def_property("minor_radius", &Geom_Ellipse::MinorRadius, &Geom_Ellipse::SetMinorRadius,
             "Get or set the minor radius of the ellipse")
        
        // Geometric properties (read-only)
        .def_property_readonly("directrix1", &Geom_Ellipse::Directrix1,
             "Returns the first directrix of the ellipse.\n"
             "This directrix is normal to the XAxis at distance d = MajorRadius / e from center,\n"
             "where e is the eccentricity. Parallel to YAxis, on positive side of XAxis.\n"
             "Raises exception if eccentricity = 0 (ellipse degenerates into circle)")
        .def_property_readonly("directrix2", &Geom_Ellipse::Directrix2,
             "Returns the second directrix (symmetrical transformation of directrix1 w.r.t. YAxis)")
        .def_property_readonly("focal", &Geom_Ellipse::Focal,
             "Computes the focal distance (distance between the two foci)")
        .def_property_readonly("focus1", &Geom_Ellipse::Focus1,
             "Returns the first focus of the ellipse (on positive side of XAxis)")
        .def_property_readonly("focus2", &Geom_Ellipse::Focus2,
             "Returns the second focus of the ellipse (on negative side of XAxis)")
        .def_property_readonly("parameter", &Geom_Ellipse::Parameter,
             "Returns p = (1 - e²) * MajorRadius where e is the eccentricity")
    ;

    py::class_<Geom_Hyperbola, opencascade::handle<Geom_Hyperbola>, Geom_Conic>(m, "Hyperbola",
        "Describes a branch of a hyperbola in 3D space.\n"
        "A hyperbola is defined by its major and minor radii and positioned with a\n"
        "right-handed coordinate system where:\n"
        "- origin is the center of the hyperbola\n"
        "- X Direction defines the major axis\n"
        "- Y Direction defines the minor axis\n"
        "The branch described is on the positive side of the major axis.\n"
        "Parameterization: P(U) = O + MajRad*Cosh(U)*XDir + MinRad*Sinh(U)*YDir\n"
        "Parameter range: ]-infinite, +infinite[")
        
        // Constructors
        .def(py::init([](const gp_Hypr& h) {
            return opencascade::handle<Geom_Hyperbola>(new Geom_Hyperbola(h));
        }), py::arg("h"),
             "Constructs a hyperbola by conversion of the gp_Hypr hyperbola")
        
        .def(py::init([](const gp_Ax2& a2, Standard_Real major_radius, Standard_Real minor_radius) {
            return opencascade::handle<Geom_Hyperbola>(new Geom_Hyperbola(a2, major_radius, minor_radius));
        }), py::arg("a2"), py::arg("major_radius"), py::arg("minor_radius"),
             "Constructs a hyperbola with given major and minor radii, where a2 locates the hyperbola:\n"
             "- center is the origin of a2\n"
             "- X Direction defines the major axis\n"
             "- Y Direction defines the minor axis\n"
             "Raises exception if major_radius < 0 or minor_radius < 0")
        
        // Properties
        .def_property("hypr", &Geom_Hyperbola::Hypr, &Geom_Hyperbola::SetHypr,
             "Get or set the gp_Hypr with the same geometric properties")
        .def_property("major_radius", &Geom_Hyperbola::MajorRadius, &Geom_Hyperbola::SetMajorRadius,
             "Get or set the major radius of the hyperbola")
        .def_property("minor_radius", &Geom_Hyperbola::MinorRadius, &Geom_Hyperbola::SetMinorRadius,
             "Get or set the minor radius of the hyperbola")
        
        // Geometric properties (read-only)
        .def_property_readonly("asymptote1", &Geom_Hyperbola::Asymptote1,
             "Returns the first asymptote of the hyperbola (equation: Y = (B/A)*X).\n"
             "Raises exception if MajorRadius = 0")
        .def_property_readonly("asymptote2", &Geom_Hyperbola::Asymptote2,
             "Returns the second asymptote of the hyperbola")
        .def_property_readonly("conjugate_branch1", &Geom_Hyperbola::ConjugateBranch1,
             "Returns the conjugate branch on the positive side of the YAxis")
        .def_property_readonly("conjugate_branch2", &Geom_Hyperbola::ConjugateBranch2,
             "Returns the conjugate branch on the negative side of the YAxis")
        .def_property_readonly("directrix1", &Geom_Hyperbola::Directrix1,
             "Returns the first directrix of the hyperbola")
        .def_property_readonly("directrix2", &Geom_Hyperbola::Directrix2,
             "Returns the second directrix of the hyperbola")
        .def_property_readonly("focal", &Geom_Hyperbola::Focal,
             "Computes the focal distance (distance between the two foci)")
        .def_property_readonly("focus1", &Geom_Hyperbola::Focus1,
             "Returns the first focus of the hyperbola (on positive side of XAxis)")
        .def_property_readonly("focus2", &Geom_Hyperbola::Focus2,
             "Returns the second focus of the hyperbola (on negative side of XAxis)")
        .def_property_readonly("other_branch", &Geom_Hyperbola::OtherBranch,
             "Returns the other branch of this hyperbola")
        .def_property_readonly("parameter", &Geom_Hyperbola::Parameter,
             "Returns p = (e² - 1) * MajorRadius where e is the eccentricity")
    ;

    py::class_<Geom_Parabola, opencascade::handle<Geom_Parabola>, Geom_Conic>(m, "Parabola",
        "Describes a parabola in 3D space.\n"
        "A parabola is defined by its focal length and positioned with a\n"
        "right-handed coordinate system where:\n"
        "- origin is the apex of the parabola\n"
        "- X Axis defines the axis of symmetry (parabola on positive side)\n"
        "- Y Direction is parallel to the directrix\n"
        "Parameterization: P(U) = O + U²/(4*F)*XDir + U*YDir\n"
        "where F is the focal length. Parameter range: ]-infinite, +infinite[")
        
        // Constructors
        .def(py::init([](const gp_Parab& prb) {
            return opencascade::handle<Geom_Parabola>(new Geom_Parabola(prb));
        }), py::arg("prb"),
             "Constructs a parabola by conversion of the gp_Parab parabola")
        
        .def(py::init([](const gp_Ax2& a2, Standard_Real focal) {
            return opencascade::handle<Geom_Parabola>(new Geom_Parabola(a2, focal));
        }), py::arg("a2"), py::arg("focal"),
             "Constructs a parabola with its local coordinate system a2 and focal length.\n"
             "The XDirection of a2 defines the axis of symmetry.\n"
             "The YDirection is parallel to the directrix.\n"
             "The Location point of a2 is the vertex (apex) of the parabola.\n"
             "Raises exception if focal < 0")
        
        .def(py::init([](const gp_Ax1& d, const gp_Pnt& f) {
            return opencascade::handle<Geom_Parabola>(new Geom_Parabola(d, f));
        }), py::arg("d"), py::arg("f"),
             "Constructs a parabola from directrix d and focus point f.\n"
             "The symmetry axis (XAxis) is normal to the directrix and passes through focus,\n"
             "but its location point is the vertex of the parabola.\n"
             "The YAxis is parallel to d with location at the vertex")
        
        // Properties
        .def_property("parab", &Geom_Parabola::Parab, &Geom_Parabola::SetParab,
             "Get or set the gp_Parab with the same geometric properties")
        .def_property("focal", &Geom_Parabola::Focal, &Geom_Parabola::SetFocal,
             "Get or set the focal distance")
        
        // Geometric properties (read-only)
        .def_property_readonly("directrix", &Geom_Parabola::Directrix,
             "Returns the directrix of this parabola.\n"
             "This is a line normal to the axis of symmetry, in the plane of the parabola,\n"
             "on the negative side of its axis, at distance from apex equal to focal length.\n"
             "Origin is located on the X Axis of this parabola")
        .def_property_readonly("focus", &Geom_Parabola::Focus,
             "Returns the focus of this parabola (on positive side of X Axis)")
        .def_property_readonly("parameter", &Geom_Parabola::Parameter,
             "Returns the parameter of the parabola (distance between focus and directrix)")
    ;

        
}