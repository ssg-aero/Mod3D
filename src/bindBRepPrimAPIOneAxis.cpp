#include <pybind11/pybind11.h>


#include <BRepPrimAPI_MakeOneAxis.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevolution.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax2.hxx>
#include <Standard_Handle.hxx>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_brep_prim_api_one_axis(py::module_ &m)
{
       py::class_<BRepPrimAPI_MakeOneAxis, BRepBuilderAPI_MakeShape>(m, "MakeOneAxis")
        // Note: This is an abstract base class - cannot be instantiated directly
        // Use concrete classes like MakeCylinder, MakeCone, etc.
        .def("face", &BRepPrimAPI_MakeOneAxis::Face)
        .def("shell", &BRepPrimAPI_MakeOneAxis::Shell)
        .def("solid", &BRepPrimAPI_MakeOneAxis::Solid)
    ;

    py::class_<BRepPrimAPI_MakeCone, BRepPrimAPI_MakeOneAxis>(m, "MakeCone")
        .def(py::init<const double, const double, const double>(),
             py::arg("r1"), py::arg("r2"), py::arg("h"))
        .def(py::init<const double, const double, const double, const double>(),
             py::arg("r1"), py::arg("r2"), py::arg("h"), py::arg("angle"))
        .def(py::init<const gp_Ax2&, const double, const double, const double>(),
             py::arg("Axes"), py::arg("r1"), py::arg("r2"), py::arg("h"))
    ;

    py::class_<BRepPrimAPI_MakeCylinder, BRepPrimAPI_MakeOneAxis>(m, "MakeCylinder")
        .def(py::init<const double, const double>(),
             py::arg("r"), py::arg("h"))
        .def(py::init<const double, const double, const double>(),
             py::arg("r"), py::arg("h"), py::arg("angle"))
        .def(py::init<const gp_Ax2&, const double, const double>(),
             py::arg("Axes"), py::arg("r"), py::arg("h"))
    ;

    py::class_<BRepPrimAPI_MakeRevolution, BRepPrimAPI_MakeOneAxis>(m, "MakeRevolution")
        // Constructors without axes (uses default Z axis)
        .def(py::init<const opencascade::handle<Geom_Curve>&>(),
             py::arg("meridian"))
        .def(py::init<const opencascade::handle<Geom_Curve>&, const double>(),
             py::arg("meridian"), py::arg("angle"))
        .def(py::init<const opencascade::handle<Geom_Curve>&, const double, const double>(),
             py::arg("meridian"), py::arg("v_min"), py::arg("v_max"))
        .def(py::init<const opencascade::handle<Geom_Curve>&, const double, const double, const double>(),
             py::arg("meridian"), py::arg("v_min"), py::arg("v_max"), py::arg("angle"))
        
        // Constructors with custom axes
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&>(),
             py::arg("axes"), py::arg("meridian"))
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&, const double>(),
             py::arg("axes"), py::arg("meridian"), py::arg("angle"))
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&, const double, const double>(),
             py::arg("axes"), py::arg("meridian"), py::arg("v_min"), py::arg("v_max"))
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&, const double, const double, const double>(),
             py::arg("axes"), py::arg("meridian"), py::arg("v_min"), py::arg("v_max"), py::arg("angle"))
        
        // Methods
        .def("revolution", &BRepPrimAPI_MakeRevolution::Revolution, py::return_value_policy::reference_internal)
    ;

    py::class_<BRepPrimAPI_MakeSphere, BRepPrimAPI_MakeOneAxis>(m, "MakeSphere",
        "Describes functions to build spheres or portions of spheres.\n\n"
        "A MakeSphere object provides a framework for:\n"
        "- defining the construction of a sphere,\n"
        "- implementing the construction algorithm, and\n"
        "- consulting the result.\n\n"
        "The resulting shape is composed of:\n"
        "- a lateral spherical face,\n"
        "- two planar faces parallel to the plane z=0 if truncated in v direction,\n"
        "- and in case of a portion, two planar faces to close the shape (in planes u=0 and u=angle)")
        
        // Basic constructors (origin at 0,0,0, axis along Z)
        .def(py::init<const double>(),
            py::arg("r"),
            "Make a sphere of radius R\n\n"
            "Parameters:\n"
            "  r: Sphere radius")
        
        .def(py::init<const double, const double>(),
            py::arg("r"), py::arg("angle"),
            "Make a sphere (spherical wedge)\n\n"
            "Parameters:\n"
            "  r: Sphere radius\n"
            "  angle: Angle between the radii lying within the bounding semidisks")
        
        .def(py::init<const double, const double, const double>(),
            py::arg("r"), py::arg("angle1"), py::arg("angle2"),
            "Make a sphere (spherical segment)\n\n"
            "Parameters:\n"
            "  r: Sphere radius\n"
            "  angle1: First angle defining a spherical segment\n"
            "  angle2: Second angle defining a spherical segment")
        
        .def(py::init<const double, const double, const double, const double>(),
            py::arg("r"), py::arg("angle1"), py::arg("angle2"), py::arg("angle3"),
            "Make a sphere (spherical segment with wedge)\n\n"
            "Parameters:\n"
            "  r: Sphere radius\n"
            "  angle1: First angle defining a spherical segment\n"
            "  angle2: Second angle defining a spherical segment\n"
            "  angle3: Angle between the radii lying within the bounding semidisks")
        
        // Constructors with center point
        .def(py::init<const gp_Pnt&, const double>(),
            py::arg("center"), py::arg("r"),
            "Make a sphere at given center point\n\n"
            "Parameters:\n"
            "  center: Sphere center coordinates\n"
            "  r: Sphere radius")
        
        .def(py::init<const gp_Pnt&, const double, const double>(),
            py::arg("center"), py::arg("r"), py::arg("angle"),
            "Make a sphere (spherical wedge) at given center\n\n"
            "Parameters:\n"
            "  center: Sphere center coordinates\n"
            "  r: Sphere radius\n"
            "  angle: Angle between the radii lying within the bounding semidisks")
        
        .def(py::init<const gp_Pnt&, const double, const double, const double>(),
            py::arg("center"), py::arg("r"), py::arg("angle1"), py::arg("angle2"),
            "Make a sphere (spherical segment) at given center\n\n"
            "Parameters:\n"
            "  center: Sphere center coordinates\n"
            "  r: Sphere radius\n"
            "  angle1: First angle defining a spherical segment\n"
            "  angle2: Second angle defining a spherical segment")
        
        .def(py::init<const gp_Pnt&, const double, const double, const double, const double>(),
            py::arg("center"), py::arg("r"), py::arg("angle1"), py::arg("angle2"), py::arg("angle3"),
            "Make a sphere (spherical segment with wedge) at given center\n\n"
            "Parameters:\n"
            "  center: Sphere center coordinates\n"
            "  r: Sphere radius\n"
            "  angle1: First angle defining a spherical segment\n"
            "  angle2: Second angle defining a spherical segment\n"
            "  angle3: Angle between the radii lying within the bounding semidisks")
        
        // Constructors with custom coordinate system
        .def(py::init<const gp_Ax2&, const double>(),
            py::arg("axes"), py::arg("r"),
            "Make a sphere with custom coordinate system\n\n"
            "Parameters:\n"
            "  axes: Coordinate system for the construction of the sphere\n"
            "  r: Sphere radius")
        
        .def(py::init<const gp_Ax2&, const double, const double>(),
            py::arg("axes"), py::arg("r"), py::arg("angle"),
            "Make a sphere (spherical wedge) with custom coordinate system\n\n"
            "Parameters:\n"
            "  axes: Coordinate system for the construction of the sphere\n"
            "  r: Sphere radius\n"
            "  angle: Angle between the radii lying within the bounding semidisks")
        
        .def(py::init<const gp_Ax2&, const double, const double, const double>(),
            py::arg("axes"), py::arg("r"), py::arg("angle1"), py::arg("angle2"),
            "Make a sphere (spherical segment) with custom coordinate system\n\n"
            "Parameters:\n"
            "  axes: Coordinate system for the construction of the sphere\n"
            "  r: Sphere radius\n"
            "  angle1: First angle defining a spherical segment\n"
            "  angle2: Second angle defining a spherical segment")
        
        .def(py::init<const gp_Ax2&, const double, const double, const double, const double>(),
            py::arg("axes"), py::arg("r"), py::arg("angle1"), py::arg("angle2"), py::arg("angle3"),
            "Make a sphere (spherical segment with wedge) with custom coordinate system\n\n"
            "Parameters:\n"
            "  axes: Coordinate system for the construction of the sphere\n"
            "  r: Sphere radius\n"
            "  angle1: First angle defining a spherical segment\n"
            "  angle2: Second angle defining a spherical segment\n"
            "  angle3: Angle between the radii lying within the bounding semidisks")
        
        // Internal algorithm as property
        .def_property_readonly("sphere", &BRepPrimAPI_MakeSphere::Sphere,
            py::return_value_policy::reference_internal,
            "Returns the internal BRepPrim_Sphere algorithm")
    ;
}