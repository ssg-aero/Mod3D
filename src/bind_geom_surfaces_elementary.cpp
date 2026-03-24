/**
 * @file bindGeomSurfacesElementary.cpp
 * @brief Pybind11 bindings for OCCT elementary surface classes
 *
 * This file provides Python bindings for:
 * - Geom_Plane
 * - Geom_CylindricalSurface
 * - Geom_ConicalSurface
 * - Geom_SphericalSurface
 * - Geom_ToroidalSurface
 */

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <Geom_ElementarySurface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_CylindricalSurface.hxx>
#include <Geom_ConicalSurface.hxx>
#include <Geom_SphericalSurface.hxx>
#include <Geom_ToroidalSurface.hxx>
#include <gp_Pln.hxx>
#include <gp_Cylinder.hxx>
#include <gp_Cone.hxx>
#include <gp_Sphere.hxx>
#include <gp_Torus.hxx>
#include <gp_Ax3.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_geom_surfaces_elementary(py::module_ &m)
{
    // =========================================================================
    // Geom_Plane
    // =========================================================================
    py::class_<Geom_Plane, opencascade::handle<Geom_Plane>, Geom_ElementarySurface>(m, "Plane",
        R"doc(
        Describes a plane in 3D space.

        A plane is defined by a coordinate system (gp_Ax3) where:
        - The origin is a point on the plane
        - The "Z Direction" is the normal to the plane
        - The "X Direction" and "Y Direction" define the U and V parametric directions

        The parametric equation is:
            P(u, v) = O + u*XDir + v*YDir

        where O is the origin, XDir and YDir are the X and Y directions.

        The plane is infinite in both U and V directions.
        )doc")
        // --- Constructors ---
        .def(py::init<const gp_Ax3 &>(),
            py::arg("a3"),
            R"doc(
            Creates a plane from a coordinate system.

            Parameters
            ----------
            a3 : gp_Ax3
                The coordinate system defining the plane. The Z direction is
                normal to the plane.
            )doc")
        .def(py::init<const gp_Pln &>(),
            py::arg("pl"),
            R"doc(
            Creates a plane from a gp_Pln.

            Parameters
            ----------
            pl : gp_Pln
                The plane definition
            )doc")
        .def(py::init<const gp_Pnt &, const gp_Dir &>(),
            py::arg("point"),
            py::arg("normal"),
            R"doc(
            Creates a plane passing through a point with given normal.

            Parameters
            ----------
            point : gp_Pnt
                A point on the plane
            normal : gp_Dir
                The normal direction to the plane
            )doc")

        // --- Specific properties and methods ---
        .def_property("pln", &Geom_Plane::Pln, &Geom_Plane::SetPln,
            R"doc(
            The non-persistent plane representation.

            Can be both read and modified.

            Returns/Sets
            -----------
            gp_Pln
                The gp_Pln representation
            )doc")
        .def_property_readonly("coefficients", [](const Geom_Plane &self) {
                Standard_Real A, B, C, D;
                self.Coefficients(A, B, C, D);
                return py::make_tuple(A, B, C, D);
            },
            R"doc(
            The coefficients of the plane equation: Ax + By + Cz + D = 0.

            The coefficients are normalized (A² + B² + C² = 1).

            Returns
            -------
            tuple[float, float, float, float]
                Coefficients (A, B, C, D)
            )doc")
        ;

    // =========================================================================
    // Geom_CylindricalSurface
    // =========================================================================
    py::class_<Geom_CylindricalSurface, opencascade::handle<Geom_CylindricalSurface>, Geom_ElementarySurface>(m, "CylindricalSurface",
        R"doc(
        Describes a cylinder in 3D space.

        A cylinder is defined by:
        - A coordinate system (gp_Ax3) whose Z direction is the axis of symmetry
        - A radius

        The parametric equation is:
            P(u, v) = O + R*(cos(u)*XDir + sin(u)*YDir) + v*ZDir

        where:
        - O is the origin of the coordinate system
        - R is the radius
        - XDir, YDir, ZDir are the axis directions
        - u is in [0, 2*PI], v is in (-∞, +∞)

        The cylinder is infinite in the V direction.
        )doc")
        // --- Constructors ---
        .def(py::init<const gp_Ax3 &, Standard_Real>(),
            py::arg("a3"),
            py::arg("radius"),
            R"doc(
            Creates a cylindrical surface.

            Parameters
            ----------
            a3 : gp_Ax3
                The coordinate system. Z direction is the axis of symmetry.
            radius : float
                The cylinder radius (must be >= 0)

            Raises
            ------
            Standard_ConstructionError
                If radius < 0
            )doc")
        .def(py::init<const gp_Cylinder &>(),
            py::arg("c"),
            R"doc(
            Creates a cylindrical surface from a gp_Cylinder.

            Parameters
            ----------
            c : gp_Cylinder
                The cylinder definition
            )doc")

        // --- Specific properties and methods ---
        .def_property("cylinder", &Geom_CylindricalSurface::Cylinder, &Geom_CylindricalSurface::SetCylinder,
            R"doc(
            The non-persistent cylinder representation.

            Can be both read and modified.

            Returns/Sets
            -----------
            gp_Cylinder
                The gp_Cylinder representation
            )doc")
        .def_property("radius", &Geom_CylindricalSurface::Radius, &Geom_CylindricalSurface::SetRadius,
            R"doc(
            The radius of this cylinder.

            Can be both read and modified.

            Returns/Sets
            -----------
            float
                The cylinder radius (must be >= 0)
            )doc")
        .def("coefficients", [](const Geom_CylindricalSurface &self) {
                Standard_Real A1, A2, A3, B1, B2, B3, C1, C2, C3, D;
                self.Coefficients(A1, A2, A3, B1, B2, B3, C1, C2, C3, D);
                return py::make_tuple(A1, A2, A3, B1, B2, B3, C1, C2, C3, D);
            },
            R"doc(
            Returns the coefficients of the implicit quadric equation.

            The equation is:
            A1*X² + A2*Y² + A3*Z² + 2*(B1*X*Y + B2*X*Z + B3*Y*Z) + 2*(C1*X + C2*Y + C3*Z) + D = 0

            The coefficients are normalized.

            Returns
            -------
            tuple[float, ...]
                Coefficients (A1, A2, A3, B1, B2, B3, C1, C2, C3, D)
            )doc")
        ;

    // =========================================================================
    // Geom_ConicalSurface
    // =========================================================================
    py::class_<Geom_ConicalSurface, opencascade::handle<Geom_ConicalSurface>, Geom_ElementarySurface>(m, "ConicalSurface",
        R"doc(
        Describes a cone in 3D space.

        A cone is defined by:
        - A coordinate system (gp_Ax3) whose Z direction is the axis of symmetry
        - A semi-angle (can be negative)
        - A reference radius (radius at the reference plane Z=0)

        The parametric equation is:
            P(u, v) = O + (RefRadius + v*sin(Ang))*(cos(u)*XDir + sin(u)*YDir) + v*cos(Ang)*ZDir

        where:
        - O is the origin of the coordinate system
        - Ang is the semi-angle
        - RefRadius is the reference radius
        - u is in [0, 2*PI], v is in (-∞, +∞)

        The apex is located at distance RefRadius/tan(Ang) from the origin along ZDir.
        )doc")
        // --- Constructors ---
        .def(py::init<const gp_Ax3 &, Standard_Real, Standard_Real>(),
            py::arg("a3"),
            py::arg("angle"),
            py::arg("radius"),
            R"doc(
            Creates a conical surface.

            Parameters
            ----------
            a3 : gp_Ax3
                The coordinate system. Z direction is the axis of symmetry.
            angle : float
                The semi-angle in radians. Absolute value must be in (0, PI/2).
            radius : float
                The reference radius at the placement plane (must be >= 0).
                If radius = 0, the origin is at the apex.

            Raises
            ------
            Standard_ConstructionError
                If radius < 0 or |angle| is not in (0, PI/2)
            )doc")
        .def(py::init<const gp_Cone &>(),
            py::arg("c"),
            R"doc(
            Creates a conical surface from a gp_Cone.

            Parameters
            ----------
            c : gp_Cone
                The cone definition
            )doc")

        // --- Specific properties and methods ---
        .def_property("cone", &Geom_ConicalSurface::Cone, &Geom_ConicalSurface::SetCone,
            R"doc(
            The non-persistent cone representation.

            Can be both read and modified.

            Returns/Sets
            -----------
            gp_Cone
                The gp_Cone representation
            )doc")
        .def_property("ref_radius", &Geom_ConicalSurface::RefRadius, &Geom_ConicalSurface::SetRadius,
            R"doc(
            The reference radius (at the reference plane Z=0).

            Can be both read and modified.

            Returns/Sets
            -----------
            float
                The reference radius (must be >= 0)
            )doc")
        .def_property("semi_angle", &Geom_ConicalSurface::SemiAngle, &Geom_ConicalSurface::SetSemiAngle,
            R"doc(
            The semi-angle at the apex. Can be negative.

            Can be both read and modified.

            Returns/Sets
            -----------
            float
                The semi-angle in radians. Absolute value must be in (0, PI/2).
            )doc")
        .def_property_readonly("apex", &Geom_ConicalSurface::Apex,
            R"doc(
            The apex of the cone.

            The apex is on the negative side of the axis if the semi-angle is positive,
            and on the positive side if the semi-angle is negative.

            Returns
            -------
            gp_Pnt
                The apex point
            )doc")
        .def("coefficients", [](const Geom_ConicalSurface &self) {
                Standard_Real A1, A2, A3, B1, B2, B3, C1, C2, C3, D;
                self.Coefficients(A1, A2, A3, B1, B2, B3, C1, C2, C3, D);
                return py::make_tuple(A1, A2, A3, B1, B2, B3, C1, C2, C3, D);
            },
            R"doc(
            Returns the coefficients of the implicit quadric equation.

            Returns
            -------
            tuple[float, ...]
                Coefficients (A1, A2, A3, B1, B2, B3, C1, C2, C3, D)
            )doc")
        ;

    // =========================================================================
    // Geom_SphericalSurface
    // =========================================================================
    py::class_<Geom_SphericalSurface, opencascade::handle<Geom_SphericalSurface>, Geom_ElementarySurface>(m, "SphericalSurface",
        R"doc(
        Describes a sphere in 3D space.

        A sphere is defined by:
        - A coordinate system (gp_Ax3) whose origin is the center
        - A radius

        The parametric equation is:
            P(u, v) = O + R*sin(v)*ZDir + R*cos(v)*(cos(u)*XDir + sin(u)*YDir)

        where:
        - O is the center (origin of the coordinate system)
        - R is the radius
        - u is in [0, 2*PI] (longitude), v is in [-PI/2, PI/2] (latitude)

        The "north pole" is at v = PI/2, the "south pole" is at v = -PI/2.
        )doc")
        // --- Constructors ---
        .def(py::init<const gp_Ax3 &, Standard_Real>(),
            py::arg("a3"),
            py::arg("radius"),
            R"doc(
            Creates a spherical surface.

            Parameters
            ----------
            a3 : gp_Ax3
                The coordinate system. Origin is the center of the sphere.
            radius : float
                The sphere radius (must be >= 0)

            Raises
            ------
            Standard_ConstructionError
                If radius < 0
            )doc")
        .def(py::init<const gp_Sphere &>(),
            py::arg("s"),
            R"doc(
            Creates a spherical surface from a gp_Sphere.

            Parameters
            ----------
            s : gp_Sphere
                The sphere definition
            )doc")

        // --- Specific properties and methods ---
        .def_property("sphere", &Geom_SphericalSurface::Sphere, &Geom_SphericalSurface::SetSphere,
            R"doc(
            The non-persistent sphere representation.

            Can be both read and modified.

            Returns/Sets
            -----------
            gp_Sphere
                The gp_Sphere representation
            )doc")
        .def_property("radius", &Geom_SphericalSurface::Radius, &Geom_SphericalSurface::SetRadius,
            R"doc(
            The radius of this sphere.

            Can be both read and modified.

            Returns/Sets
            -----------
            float
                The sphere radius (must be >= 0)
            )doc")
        .def_property_readonly("area", &Geom_SphericalSurface::Area,
            R"doc(
            Computes the surface area of the sphere.

            Area = 4 * PI * R²

            Returns
            -------
            float
                The surface area
            )doc")
        .def_property_readonly("volume", &Geom_SphericalSurface::Volume,
            R"doc(
            Computes the volume of the sphere.

            Volume = 4/3 * PI * R³

            Returns
            -------
            float
                The volume
            )doc")
        .def("coefficients", [](const Geom_SphericalSurface &self) {
                Standard_Real A1, A2, A3, B1, B2, B3, C1, C2, C3, D;
                self.Coefficients(A1, A2, A3, B1, B2, B3, C1, C2, C3, D);
                return py::make_tuple(A1, A2, A3, B1, B2, B3, C1, C2, C3, D);
            },
            R"doc(
            Returns the coefficients of the implicit quadric equation.

            Returns
            -------
            tuple[float, ...]
                Coefficients (A1, A2, A3, B1, B2, B3, C1, C2, C3, D)
            )doc")
        ;

    // =========================================================================
    // Geom_ToroidalSurface
    // =========================================================================
    py::class_<Geom_ToroidalSurface, opencascade::handle<Geom_ToroidalSurface>, Geom_ElementarySurface>(m, "ToroidalSurface",
        R"doc(
        Describes a torus in 3D space.

        A torus is defined by:
        - A coordinate system (gp_Ax3) whose origin is the center
        - A major radius (from center to tube center)
        - A minor radius (tube radius)

        The parametric equation is:
            P(u, v) = O + (R + r*cos(v))*(cos(u)*XDir + sin(u)*YDir) + r*sin(v)*ZDir

        where:
        - O is the center (origin of the coordinate system)
        - R is the major radius
        - r is the minor radius
        - u is in [0, 2*PI] (around the axis), v is in [0, 2*PI] (around the tube)

        Note: It is valid to create a torus with R = 0 (spindle torus) or
        r = 0 (degenerate torus).
        )doc")
        // --- Constructors ---
        .def(py::init<const gp_Ax3 &, Standard_Real, Standard_Real>(),
            py::arg("a3"),
            py::arg("major_radius"),
            py::arg("minor_radius"),
            R"doc(
            Creates a toroidal surface.

            Parameters
            ----------
            a3 : gp_Ax3
                The coordinate system. Origin is the center of the torus.
            major_radius : float
                The major radius (must be >= 0)
            minor_radius : float
                The minor radius (must be >= 0)

            Raises
            ------
            Standard_ConstructionError
                If major_radius < 0 or minor_radius < 0
            )doc")
        .def(py::init<const gp_Torus &>(),
            py::arg("t"),
            R"doc(
            Creates a toroidal surface from a gp_Torus.

            Parameters
            ----------
            t : gp_Torus
                The torus definition
            )doc")

        // --- Specific properties and methods ---
        .def_property("torus", &Geom_ToroidalSurface::Torus, &Geom_ToroidalSurface::SetTorus,
            R"doc(
            The non-persistent torus representation.

            Can be both read and modified.

            Returns/Sets
            -----------
            gp_Torus
                The gp_Torus representation
            )doc")
        .def_property("major_radius", &Geom_ToroidalSurface::MajorRadius, &Geom_ToroidalSurface::SetMajorRadius,
            R"doc(
            The major radius of this torus.

            Can be both read and modified.

            Returns/Sets
            -----------
            float
                The major radius (must be >= 0)
            )doc")
        .def_property("minor_radius", &Geom_ToroidalSurface::MinorRadius, &Geom_ToroidalSurface::SetMinorRadius,
            R"doc(
            The minor radius of this torus.

            Can be both read and modified.

            Returns/Sets
            -----------
            float
                The minor radius (must be >= 0)
            )doc")
        .def_property_readonly("area", &Geom_ToroidalSurface::Area,
            R"doc(
            Computes the surface area of the torus.

            Area = 4 * PI² * R * r

            where R is the major radius and r is the minor radius.

            Returns
            -------
            float
                The surface area
            )doc")
        .def_property_readonly("volume", &Geom_ToroidalSurface::Volume,
            R"doc(
            Computes the volume of the torus.

            Volume = 2 * PI² * R * r²

            where R is the major radius and r is the minor radius.

            Returns
            -------
            float
                The volume
            )doc")
        ;
}
