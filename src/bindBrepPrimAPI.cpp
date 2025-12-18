#include <pybind11/pybind11.h>

#include <BRepPrimAPI_MakeBox.hxx>

namespace py = pybind11;
// forward declaration
void bind_brep_prim_api_one_axis(py::module_ &m);
void bind_brep_prim_api_sweep(py::module_ &m);

void bind_brep_prim_api(py::module_ &m)
{
    py::class_<BRepPrimAPI_MakeBox, BRepBuilderAPI_MakeShape>(m, "MakeBox",
        "Describes functions to build parallelepiped boxes.\n\n"
        "A MakeBox object provides a framework for:\n"
        "- defining the construction of a box,\n"
        "- implementing the construction algorithm, and\n"
        "- consulting the result.\n\n"
        "Constructs a box such that its sides are parallel to the axes of\n"
        "- the global coordinate system, or\n"
        "- the local coordinate system Axis, and\n"
        "- with a corner at (0, 0, 0) and of size (dx, dy, dz), or\n"
        "- with a corner at point P and of size (dx, dy, dz), or\n"
        "- with corners at points P1 and P2.")
        
        // Constructors
        .def(py::init<>(),
            "Default constructor")
        
        .def(py::init<const double, const double, const double>(),
            py::arg("dx"), py::arg("dy"), py::arg("dz"),
            "Make a box with a corner at (0,0,0) and the other at (dx, dy, dz)\n\n"
            "Parameters:\n"
            "  dx: Size along X axis\n"
            "  dy: Size along Y axis\n"
            "  dz: Size along Z axis\n\n"
            "Raises:\n"
            "  Standard_DomainError if dx, dy, or dz <= Precision::Confusion()")
        
        .def(py::init<const gp_Pnt&, const double, const double, const double>(),
            py::arg("p"), py::arg("dx"), py::arg("dy"), py::arg("dz"),
            "Make a box with a corner at P and size dx, dy, dz\n\n"
            "Parameters:\n"
            "  p: Corner point of the box\n"
            "  dx: Size along X axis\n"
            "  dy: Size along Y axis\n"
            "  dz: Size along Z axis\n\n"
            "Raises:\n"
            "  Standard_DomainError if dx, dy, or dz <= Precision::Confusion()")
        
        .def(py::init<const gp_Pnt&, const gp_Pnt&>(),
            py::arg("p1"), py::arg("p2"),
            "Make a box with corners P1 and P2\n\n"
            "Parameters:\n"
            "  p1: First corner point\n"
            "  p2: Second corner point (opposite corner)\n\n"
            "Raises:\n"
            "  Standard_DomainError if any component of the vector P1-P2 <= Precision::Confusion()")
        
        .def(py::init<const gp_Ax2&, const double, const double, const double>(),
            py::arg("axes"), py::arg("dx"), py::arg("dy"), py::arg("dz"),
            "Make a box with Ax2 (the left corner and the axis) and size dx, dy, dz\n\n"
            "Parameters:\n"
            "  axes: Local coordinate system (origin is the corner)\n"
            "  dx: Size along local X axis\n"
            "  dy: Size along local Y axis\n"
            "  dz: Size along local Z axis\n\n"
            "Raises:\n"
            "  Standard_DomainError if dx, dy, or dz <= Precision::Confusion()")
        
        // Init methods
        .def("init", py::overload_cast<const double, const double, const double>(&BRepPrimAPI_MakeBox::Init),
            py::arg("dx"), py::arg("dy"), py::arg("dz"),
            "Initialize a box with a corner at (0,0,0) and the other at (dx, dy, dz)")
        
        .def("init", py::overload_cast<const gp_Pnt&, const double, const double, const double>(&BRepPrimAPI_MakeBox::Init),
            py::arg("pnt"), py::arg("dx"), py::arg("dy"), py::arg("dz"),
            "Initialize a box with a corner at pnt and size dx, dy, dz")
        
        .def("init", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&BRepPrimAPI_MakeBox::Init),
            py::arg("pnt1"), py::arg("pnt2"),
            "Initialize a box with corners pnt1 and pnt2")
        
        .def("init", py::overload_cast<const gp_Ax2&, const double, const double, const double>(&BRepPrimAPI_MakeBox::Init),
            py::arg("axes"), py::arg("dx"), py::arg("dy"), py::arg("dz"),
            "Initialize a box with Ax2 (the left corner and the axes) and size dx, dy, dz")
        
        // Internal algorithm
        .def_property_readonly("wedge", &BRepPrimAPI_MakeBox::Wedge,
            py::return_value_policy::reference_internal,
            "Returns the internal wedge algorithm")
        
        // Build (inherited but documented) - use lambda to handle default argument
        // TODO: Add message support?
        .def("build", [](BRepPrimAPI_MakeBox& self) {
            self.Build();
        }, "Builds the box and stores the result")
        
        // Shape access
        .def("shell", &BRepPrimAPI_MakeBox::Shell,
            py::return_value_policy::reference_internal,
            "Returns the constructed box as a shell")
        
        .def("solid", &BRepPrimAPI_MakeBox::Solid,
            py::return_value_policy::reference_internal,
            "Returns the constructed box as a solid")
        
        // Face access as properties (read-only)
        .def_property_readonly("bottom_face", &BRepPrimAPI_MakeBox::BottomFace,
            py::return_value_policy::reference_internal,
            "Returns ZMin face (bottom face)")
        
        .def_property_readonly("back_face", &BRepPrimAPI_MakeBox::BackFace,
            py::return_value_policy::reference_internal,
            "Returns XMin face (back face)")
        
        .def_property_readonly("front_face", &BRepPrimAPI_MakeBox::FrontFace,
            py::return_value_policy::reference_internal,
            "Returns XMax face (front face)")
        
        .def_property_readonly("left_face", &BRepPrimAPI_MakeBox::LeftFace,
            py::return_value_policy::reference_internal,
            "Returns YMin face (left face)")
        
        .def_property_readonly("right_face", &BRepPrimAPI_MakeBox::RightFace,
            py::return_value_policy::reference_internal,
            "Returns YMax face (right face)")
        
        .def_property_readonly("top_face", &BRepPrimAPI_MakeBox::TopFace,
            py::return_value_policy::reference_internal,
            "Returns ZMax face (top face)")
    ;

    bind_brep_prim_api_one_axis(m);

    bind_brep_prim_api_sweep(m);
}