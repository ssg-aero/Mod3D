#include <pybind11/pybind11.h>

#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <GProp_PrincipalProps.hxx>
#include <gp_Pnt.hxx>
#include <gp_Mat.hxx>
#include <gp_Ax1.hxx>
#include <TopoDS_Shape.hxx>

namespace py = pybind11;

void bind_brep_gprop(py::module_ &m)
{
    // ---------------------------------------------------------------------------
    // GProp_GProps - Global properties of a geometric system
    // ---------------------------------------------------------------------------
    py::class_<GProp_GProps>(m, "GProps",
        R"(Implements a general mechanism to compute the global properties of a
compound geometric system in 3D space. The global properties computed are:
- the mass (length, area, or volume depending on the type)
- the centre of mass
- the moments of inertia (static moments and quadratic moments)
- the moment about an axis
- the radius of gyration about an axis
- the principal properties of inertia)")
        // Constructors
        .def(py::init<>(),
            "Creates a GProps with the origin (0,0,0) as reference point")
        .def(py::init<const gp_Pnt&>(),
            py::arg("system_location"),
            R"(Creates a GProps with a specified reference point.
For more accuracy, the reference point should be close to the system location,
e.g., near the center of mass.)")

        // Methods
        .def("add", &GProp_GProps::Add,
            py::arg("item"),
            py::arg("density") = 1.0,
            R"(Adds the global properties of item to this framework.
The density parameter (default 1.0) is used as the density of the system
analyzed by item.)")

        .def_property_readonly("mass", &GProp_GProps::Mass,
            R"(Returns the mass of the current system.
If no density is attached to the components, the returned value corresponds to:
- the total length of edges (for LinearProperties)
- the total area of faces (for SurfaceProperties)
- the total volume of solids (for VolumeProperties))")

        .def_property_readonly("centre_of_mass", &GProp_GProps::CentreOfMass,
            R"(Returns the center of mass (or center of gravity in a uniform field).
The coordinates are expressed in the absolute Cartesian coordinate system.)")

        .def_property_readonly("matrix_of_inertia", &GProp_GProps::MatrixOfInertia,
            R"(Returns the matrix of inertia (symmetrical).
The coefficients are the quadratic moments of inertia:
| Ixx  Ixy  Ixz |
| Ixy  Iyy  Iyz |
| Ixz  Iyz  Izz |
The matrix is returned in the central coordinate system (G, Gx, Gy, Gz)
where G is the centre of mass.)")

        .def("static_moments", [](const GProp_GProps& self) {
            Standard_Real Ix, Iy, Iz;
            self.StaticMoments(Ix, Iy, Iz);
            return py::make_tuple(Ix, Iy, Iz);
        },
            "Returns (Ix, Iy, Iz), the static moments of inertia about the three Cartesian axes.")

        .def("moment_of_inertia", &GProp_GProps::MomentOfInertia,
            py::arg("axis"),
            "Computes the moment of inertia about the given axis.")

        .def("radius_of_gyration", &GProp_GProps::RadiusOfGyration,
            py::arg("axis"),
            "Returns the radius of gyration about the given axis.")

        .def_property_readonly("principal_properties", &GProp_GProps::PrincipalProperties,
            R"(Computes and returns the principal properties of inertia.
The principal axes are axes for which the products of inertia are zero
(i.e., the matrix of inertia is diagonal). Their origin coincides with
the center of mass.)");

    // ---------------------------------------------------------------------------
    // GProp_PrincipalProps - Principal properties of inertia
    // ---------------------------------------------------------------------------
    py::class_<GProp_PrincipalProps>(m, "PrincipalProps",
        "A framework for presenting the principal properties of inertia of a system.")

        .def("has_symmetry_axis", [](const GProp_PrincipalProps& self) {
            return self.HasSymmetryAxis();
        },
            "Returns true if the geometric system has an axis of symmetry.")

        .def("has_symmetry_point", [](const GProp_PrincipalProps& self) {
            return self.HasSymmetryPoint();
        },
            "Returns true if the geometric system has a point of symmetry.")

        .def("moments", [](const GProp_PrincipalProps& self) {
            Standard_Real Ixx, Iyy, Izz;
            self.Moments(Ixx, Iyy, Izz);
            return py::make_tuple(Ixx, Iyy, Izz);
        },
            "Returns (Ixx, Iyy, Izz), the principal moments of inertia.")

        .def("first_axis_of_inertia", &GProp_PrincipalProps::FirstAxisOfInertia,
            "Returns the first principal axis of inertia.")

        .def("second_axis_of_inertia", &GProp_PrincipalProps::SecondAxisOfInertia,
            "Returns the second principal axis of inertia.")

        .def("third_axis_of_inertia", &GProp_PrincipalProps::ThirdAxisOfInertia,
            "Returns the third principal axis of inertia.")

        .def("radii_of_gyration", [](const GProp_PrincipalProps& self) {
            Standard_Real Rxx, Ryy, Rzz;
            self.RadiusOfGyration(Rxx, Ryy, Rzz);
            return py::make_tuple(Rxx, Ryy, Rzz);
        },
            "Returns (Rxx, Ryy, Rzz), the principal radii of gyration.");

    // ---------------------------------------------------------------------------
    // BRepGProp - Static methods for computing global properties of shapes
    // ---------------------------------------------------------------------------
    py::class_<BRepGProp>(m, "BRepGProp",
        R"(Provides static methods to compute global properties of BRep shapes:
- LinearProperties: computes properties from edges (length)
- SurfaceProperties: computes properties from faces (area)
- VolumeProperties: computes properties from solids (volume))")

        // LinearProperties
        .def_static("linear_properties", [](const TopoDS_Shape& shape, bool skip_shared) {
            GProp_GProps props;
            BRepGProp::LinearProperties(shape, props, skip_shared);
            return props;
        },
            py::arg("shape"),
            py::arg("skip_shared") = false,
            R"(Computes the linear properties (length, center of mass, inertia) from the edges.
If skip_shared is true, edges shared by two or more faces are skipped.
Returns a GProps object containing the computed properties.)")

        // SurfaceProperties (basic)
        .def_static("surface_properties", [](const TopoDS_Shape& shape, bool skip_shared) {
            GProp_GProps props;
            BRepGProp::SurfaceProperties(shape, props, skip_shared);
            return props;
        },
            py::arg("shape"),
            py::arg("skip_shared") = false,
            R"(Computes the surface properties (area, center of mass, inertia) from the faces.
If skip_shared is true, faces shared by two or more shells are skipped.
Returns a GProps object containing the computed properties.)")

        // SurfaceProperties with precision
        .def_static("surface_properties_eps", [](const TopoDS_Shape& shape, Standard_Real eps, bool skip_shared) {
            GProp_GProps props;
            Standard_Real error = BRepGProp::SurfaceProperties(shape, props, eps, skip_shared);
            return py::make_tuple(props, error);
        },
            py::arg("shape"),
            py::arg("eps"),
            py::arg("skip_shared") = false,
            R"(Computes surface properties with a specified precision.
Args:
    shape: The shape to analyze
    eps: Relative precision for integration
    skip_shared: If true, skip shared faces
Returns:
    tuple: (GProps, error) where error is the relative computation error.)")

        // VolumeProperties (basic)
        .def_static("volume_properties", [](const TopoDS_Shape& shape, bool only_closed) {
            GProp_GProps props;
            BRepGProp::VolumeProperties(shape, props, only_closed);
            return props;
        },
            py::arg("shape"),
            py::arg("only_closed") = false,
            R"(Computes the volume properties (volume, center of mass, inertia) from the solids.
If only_closed is true, only closed shells are considered.
Returns a GProps object containing the computed properties.)")

        // VolumeProperties with precision
        .def_static("volume_properties_eps", [](const TopoDS_Shape& shape, Standard_Real eps, bool only_closed) {
            GProp_GProps props;
            Standard_Real error = BRepGProp::VolumeProperties(shape, props, eps, only_closed);
            return py::make_tuple(props, error);
        },
            py::arg("shape"),
            py::arg("eps"),
            py::arg("only_closed") = false,
            R"(Computes volume properties with a specified precision.
Args:
    shape: The shape to analyze
    eps: Relative precision for integration
    only_closed: If true, only closed shells are considered
Returns:
    tuple: (GProps, error) where error is the relative computation error.)")

        // VolumePropertiesGK (Gauss-Kronrod integration)
        .def_static("volume_properties_gk", [](const TopoDS_Shape& shape, Standard_Real eps, bool only_closed, bool skip_shared) {
            GProp_GProps props;
            Standard_Real error = BRepGProp::VolumePropertiesGK(shape, props, eps, only_closed, skip_shared);
            return py::make_tuple(props, error);
        },
            py::arg("shape"),
            py::arg("eps") = 0.001,
            py::arg("only_closed") = false,
            py::arg("skip_shared") = false,
            R"(Computes volume properties using Gauss-Kronrod integration.
This method provides adaptive integration with better error estimation.
Args:
    shape: The shape to analyze
    eps: Relative precision (default 0.001)
    only_closed: If true, only closed shells are considered
    skip_shared: If true, skip shared faces
Returns:
    tuple: (GProps, error) where error is the relative computation error.)");

    m.def("length", [](const TopoDS_Shape& shape) {
        GProp_GProps props;
        BRepGProp::LinearProperties(shape, props, false);
        return props.Mass();
    },
        py::arg("shape"),
        "Compute and return the total length of the given shape.");
    m.def("area", [](const TopoDS_Shape& shape) {
        GProp_GProps props;
        BRepGProp::SurfaceProperties(shape, props, false);
        return props.Mass();
    },
        py::arg("shape"),
        "Compute and return the surface area of the given shape.");
    m.def("volume", [](const TopoDS_Shape& shape) {
        GProp_GProps props;
        BRepGProp::VolumeProperties(shape, props, false);
        return props.Mass();
    },
        py::arg("shape"),
        "Compute and return the volume of the given shape.");
    
}