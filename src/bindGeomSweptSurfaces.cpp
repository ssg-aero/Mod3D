#include <pybind11/pybind11.h>
#include <Geom_SweptSurface.hxx>
#include <Geom_SurfaceOfRevolution.hxx>
#include <Geom_SurfaceOfLinearExtrusion.hxx>

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_geom_swept_surfaces(py::module_ &m)
{
    // ==================== SweptSurface ====================
    // Abstract base class - no constructor
    py::class_<Geom_SweptSurface, Geom_Surface, opencascade::handle<Geom_SweptSurface>>(m, "SweptSurface",
        "Abstract base class for surfaces created by sweeping a curve with another curve.\n"
        "Concrete derived classes are SurfaceOfRevolution and SurfaceOfLinearExtrusion.")
        .def_property_readonly("direction", &Geom_SweptSurface::Direction,
            "Returns the reference direction of the swept surface.\n"
            "For a surface of revolution it is the direction of the revolution axis,\n"
            "for a surface of linear extrusion it is the direction of extrusion.")
        .def_property_readonly("basis_curve", &Geom_SweptSurface::BasisCurve,
            "Returns the referenced curve of the surface.\n"
            "For a surface of revolution it is the revolution curve,\n"
            "for a surface of linear extrusion it is the extruded curve.");


    // ==================== SurfaceOfRevolution ====================
    py::class_<Geom_SurfaceOfRevolution, Geom_SweptSurface, 
               opencascade::handle<Geom_SurfaceOfRevolution>>(m, "SurfaceOfRevolution",
        "Describes a surface of revolution.\n"
        "A surface of revolution is created by rotating a curve about an axis.")
        .def(py::init<const opencascade::handle<Geom_Curve>&, const gp_Ax1&>(),
            py::arg("basis_curve"),
            py::arg("axis"),
            "Constructor:\n"
            "  basis_curve: The curve to revolve\n"
            "  axis: The axis of revolution")
        .def_property_readonly("axis", &Geom_SurfaceOfRevolution::Axis,
            "Returns the axis of revolution.");


    // ==================== SurfaceOfLinearExtrusion ====================
    py::class_<Geom_SurfaceOfLinearExtrusion, Geom_SweptSurface,
               opencascade::handle<Geom_SurfaceOfLinearExtrusion>>(m, "SurfaceOfLinearExtrusion",
        "Describes a surface of linear extrusion.\n"
        "A surface of linear extrusion is created by extruding a curve along a direction.")
        .def(py::init<const opencascade::handle<Geom_Curve>&, const gp_Dir&>(),
            py::arg("basis_curve"),
            py::arg("direction"),
            "Constructor:\n"
            "  basis_curve: The curve to extrude\n"
            "  direction: The extrusion direction");
}
