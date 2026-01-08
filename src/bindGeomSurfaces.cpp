#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

#include <Geom_Surface.hxx>
#include <Geom_BoundedSurface.hxx>
#include <Geom_ElementarySurface.hxx>
#include <Geom_Curve.hxx>
#include <GeomAbs_Shape.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Trsf.hxx>
#include <gp_GTrsf2d.hxx>
#include <Geom_BSplineSurface.hxx>

#include <gbs/bssurf.h>

namespace py = pybind11;
// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

// Helper functions to convert gbs surfaces to OCCT
inline opencascade::handle<Geom_BSplineSurface> gbs_bssurface_to_occt(const gbs::BSSurface<double,3>& surf) {
    const auto& poles_ = surf.poles();
    size_t nu = surf.nPolesU();
    size_t nv = surf.nPolesV();
    Standard_Integer u_size = static_cast<Standard_Integer>(nu);
    Standard_Integer v_size = static_cast<Standard_Integer>(nv);
    
    TColgp_Array2OfPnt poles(1, u_size, 1, v_size);
    for (size_t i = 0; i < nu; ++i) {
        for (size_t j = 0; j < nv; ++j) {
            size_t idx = j + i * nu;
            const auto& pt = poles_[idx];
            poles.SetValue(static_cast<Standard_Integer>(i + 1), 
                          static_cast<Standard_Integer>(j + 1), 
                          gp_Pnt(pt[0], pt[1], pt[2]));
        }
    }
    
    auto [knots_u, mults_u] = gbs::knots_and_mults(surf.knotsFlatsU());
    auto [knots_v, mults_v] = gbs::knots_and_mults(surf.knotsFlatsV());
    
    TColStd_Array1OfReal occt_knots_u(1, static_cast<Standard_Integer>(knots_u.size()));
    TColStd_Array1OfInteger occt_mults_u(1, static_cast<Standard_Integer>(mults_u.size()));
    for (size_t i = 0; i < knots_u.size(); ++i) {
        occt_knots_u.SetValue(static_cast<Standard_Integer>(i + 1), knots_u[i]);
        occt_mults_u.SetValue(static_cast<Standard_Integer>(i + 1), static_cast<Standard_Integer>(mults_u[i]));
    }
    
    TColStd_Array1OfReal occt_knots_v(1, static_cast<Standard_Integer>(knots_v.size()));
    TColStd_Array1OfInteger occt_mults_v(1, static_cast<Standard_Integer>(mults_v.size()));
    for (size_t i = 0; i < knots_v.size(); ++i) {
        occt_knots_v.SetValue(static_cast<Standard_Integer>(i + 1), knots_v[i]);
        occt_mults_v.SetValue(static_cast<Standard_Integer>(i + 1), static_cast<Standard_Integer>(mults_v[i]));
    }
    
    return new Geom_BSplineSurface(poles, occt_knots_u, occt_knots_v, 
                                   occt_mults_u, occt_mults_v, 
                                   surf.degreeU(), surf.degreeV());
}

inline opencascade::handle<Geom_BSplineSurface> gbs_bssurface_rational_to_occt(const gbs::BSSurfaceRational<double,3>& surf) {
    auto projected_poles = surf.polesProjected();
    size_t nu = surf.nPolesU();
    size_t nv = surf.nPolesV();
    Standard_Integer u_size = static_cast<Standard_Integer>(nu);
    Standard_Integer v_size = static_cast<Standard_Integer>(nv);
    
    TColgp_Array2OfPnt poles(1, u_size, 1, v_size);
    for (size_t i = 0; i < nu; ++i) {
        for (size_t j = 0; j < nv; ++j) {
            size_t idx = j + i * nu;
            const auto& pt = projected_poles[idx];
            poles.SetValue(static_cast<Standard_Integer>(i + 1), 
                          static_cast<Standard_Integer>(j + 1), 
                          gp_Pnt(pt[0], pt[1], pt[2]));
        }
    }
    
    const auto& weights_ = surf.weights();
    TColStd_Array2OfReal weights(1, u_size, 1, v_size);
    for (size_t i = 0; i < nu; ++i) {
        for (size_t j = 0; j < nv; ++j) {
            size_t idx = j + i * nu;
            weights.SetValue(static_cast<Standard_Integer>(i + 1), 
                           static_cast<Standard_Integer>(j + 1), 
                           weights_[idx]);
        }
    }
    
    auto [knots_u, mults_u] = gbs::knots_and_mults(surf.knotsFlatsU());
    auto [knots_v, mults_v] = gbs::knots_and_mults(surf.knotsFlatsV());
    
    TColStd_Array1OfReal occt_knots_u(1, static_cast<Standard_Integer>(knots_u.size()));
    TColStd_Array1OfInteger occt_mults_u(1, static_cast<Standard_Integer>(mults_u.size()));
    for (size_t i = 0; i < knots_u.size(); ++i) {
        occt_knots_u.SetValue(static_cast<Standard_Integer>(i + 1), knots_u[i]);
        occt_mults_u.SetValue(static_cast<Standard_Integer>(i + 1), static_cast<Standard_Integer>(mults_u[i]));
    }
    
    TColStd_Array1OfReal occt_knots_v(1, static_cast<Standard_Integer>(knots_v.size()));
    TColStd_Array1OfInteger occt_mults_v(1, static_cast<Standard_Integer>(mults_v.size()));
    for (size_t i = 0; i < knots_v.size(); ++i) {
        occt_knots_v.SetValue(static_cast<Standard_Integer>(i + 1), knots_v[i]);
        occt_mults_v.SetValue(static_cast<Standard_Integer>(i + 1), static_cast<Standard_Integer>(mults_v[i]));
    }
    
    return new Geom_BSplineSurface(poles, weights, occt_knots_u, occt_knots_v, 
                                   occt_mults_u, occt_mults_v, 
                                   surf.degreeU(), surf.degreeV());
}

// Forward declaration
extern void bind_geom_surfaces_splines(py::module_ &m);
extern void bind_geom_surfaces_elementary(py::module_ &m);

void bind_geom_surfaces(py::module_ &m)
{
    py::class_<Geom_Surface, opencascade::handle<Geom_Surface>, Geom_Geometry>(m, "Surface",
        R"doc(
        Abstract base class for all surfaces in 3D space.

        Geom_Surface describes the common behavior of surfaces in 3D space.
        Surfaces are parameterized by (U, V) parameters.

        Key characteristics:
        - All surfaces must have at least C0 geometric continuity
        - Surfaces can be closed and/or periodic in U and/or V directions
        - Provides methods for point evaluation, derivatives, and iso-curves

        Derived classes include planes, cylinders, cones, spheres, tori,
        surfaces of linear extrusion, surfaces of revolution, Bezier and
        BSpline surfaces.
        )doc")
        // Constructors from gbs types (for implicit conversion support)
        .def(py::init([](const gbs::BSSurface<double,3>& surf) {
            return opencascade::handle<Geom_Surface>(gbs_bssurface_to_occt(surf));
        }), py::arg("bssurface"),
            "Create a Geom_Surface from a gbs::BSSurface object (implicit conversion).")
        .def(py::init([](const gbs::BSSurfaceRational<double,3>& surf) {
            return opencascade::handle<Geom_Surface>(gbs_bssurface_rational_to_occt(surf));
        }), py::arg("bssurface_rational"),
            "Create a Geom_Surface from a gbs::BSSurfaceRational object (implicit conversion).")
        // --- Reversal methods ---
        .def("u_reverse", &Geom_Surface::UReverse,
            R"doc(
            Reverse the U direction of parametrization in-place.

            The bounds of the surface are not modified.
            )doc")

        .def("v_reverse", &Geom_Surface::VReverse,
            R"doc(
            Reverse the V direction of parametrization in-place.

            The bounds of the surface are not modified.
            )doc")

        .def("u_reversed", &Geom_Surface::UReversed,
            R"doc(
            Return a copy of this surface with U direction reversed.

            The bounds of the surface are not modified.

            Returns:
                Surface: A new surface with reversed U parametrization.
            )doc")

        .def("v_reversed", &Geom_Surface::VReversed,
            R"doc(
            Return a copy of this surface with V direction reversed.

            The bounds of the surface are not modified.

            Returns:
                Surface: A new surface with reversed V parametrization.
            )doc")

        .def("u_reversed_parameter", &Geom_Surface::UReversedParameter, py::arg("u"),
            R"doc(
            Compute the U parameter on the U-reversed surface.

            For a point at parameter U on this surface, returns the corresponding
            parameter on the U-reversed surface.

            Args:
                u: The U parameter on this surface.

            Returns:
                float: The corresponding U parameter on the reversed surface.
            )doc")

        .def("v_reversed_parameter", &Geom_Surface::VReversedParameter, py::arg("v"),
            R"doc(
            Compute the V parameter on the V-reversed surface.

            For a point at parameter V on this surface, returns the corresponding
            parameter on the V-reversed surface.

            Args:
                v: The V parameter on this surface.

            Returns:
                float: The corresponding V parameter on the reversed surface.
            )doc")

        // --- Bounds ---
        .def("bounds", [](const Geom_Surface& self) {
                Standard_Real u1, u2, v1, v2;
                self.Bounds(u1, u2, v1, v2);
                return py::make_tuple(u1, u2, v1, v2);
            },
            R"doc(
            Return the parametric bounds of this surface.

            Returns:
                tuple: (u1, u2, v1, v2) parametric bounds.
                       For infinite surfaces, values may be very large.
            )doc")

        // --- Closure and periodicity properties ---
        .def_property_readonly("is_u_closed", &Geom_Surface::IsUClosed,
            R"doc(
            Check if the surface is closed in the U direction.

            A surface is closed in U if for each V, the points at U_first and
            U_last are coincident within tolerance.
            )doc")

        .def_property_readonly("is_v_closed", &Geom_Surface::IsVClosed,
            R"doc(
            Check if the surface is closed in the V direction.

            A surface is closed in V if for each U, the points at V_first and
            V_last are coincident within tolerance.
            )doc")

        .def_property_readonly("is_u_periodic", &Geom_Surface::IsUPeriodic,
            R"doc(
            Check if the surface is periodic in the U direction.

            A surface is U-periodic if it is U-closed and P(U, V) = P(U + T, V)
            for some period T.
            )doc")

        .def_property_readonly("is_v_periodic", &Geom_Surface::IsVPeriodic,
            R"doc(
            Check if the surface is periodic in the V direction.

            A surface is V-periodic if it is V-closed and P(U, V) = P(U, V + T)
            for some period T.
            )doc")

        .def_property_readonly("u_period", [](const Geom_Surface& self) -> py::object {
                if (self.IsUPeriodic()) {
                    return py::cast(self.UPeriod());
                }
                return py::none();
            },
            R"doc(
            The period in the U direction, or None if not U-periodic.
            )doc")

        .def_property_readonly("v_period", [](const Geom_Surface& self) -> py::object {
                if (self.IsVPeriodic()) {
                    return py::cast(self.VPeriod());
                }
                return py::none();
            },
            R"doc(
            The period in the V direction, or None if not V-periodic.
            )doc")

        // --- Continuity ---
        .def_property_readonly("continuity", &Geom_Surface::Continuity,
            R"doc(
            The global continuity of the surface.

            Returns the minimum continuity between U and V directions.
            Possible values: C0, C1, C2, C3, CN, G1, G2.
            )doc")

        .def("is_cn_u", &Geom_Surface::IsCNu, py::arg("n"),
            R"doc(
            Check if the surface has Cn continuity in the U direction.

            Args:
                n: The order of continuity to check (n >= 0).

            Returns:
                bool: True if the surface is at least Cn continuous in U.
            )doc")

        .def("is_cn_v", &Geom_Surface::IsCNv, py::arg("n"),
            R"doc(
            Check if the surface has Cn continuity in the V direction.

            Args:
                n: The order of continuity to check (n >= 0).

            Returns:
                bool: True if the surface is at least Cn continuous in V.
            )doc")

        // --- Iso-curves ---
        .def("u_iso", &Geom_Surface::UIso, py::arg("u"),
            R"doc(
            Compute the U isoparametric curve.

            Returns the curve at constant U parameter.

            Args:
                u: The U parameter value.

            Returns:
                Curve: The isoparametric curve in the V direction at U.
            )doc")

        .def("v_iso", &Geom_Surface::VIso, py::arg("v"),
            R"doc(
            Compute the V isoparametric curve.

            Returns the curve at constant V parameter.

            Args:
                v: The V parameter value.

            Returns:
                Curve: The isoparametric curve in the U direction at V.
            )doc")

        // --- Point and derivative evaluation ---
        .def("value", &Geom_Surface::Value, py::arg("u"), py::arg("v"),
            R"doc(
            Compute the point at parameters (U, V).

            Args:
                u: The U parameter.
                v: The V parameter.

            Returns:
                gp.Pnt: The 3D point on the surface.
            )doc")

        .def("d0", [](const Geom_Surface& self, double u, double v) {
                gp_Pnt p;
                self.D0(u, v, p);
                return p;
            }, py::arg("u"), py::arg("v"),
            R"doc(
            Compute the point at parameters (U, V).

            Equivalent to value(u, v).

            Args:
                u: The U parameter.
                v: The V parameter.

            Returns:
                gp.Pnt: The 3D point on the surface.
            )doc")

        .def("d1", [](const Geom_Surface& self, double u, double v) {
                gp_Pnt p;
                gp_Vec d1u, d1v;
                self.D1(u, v, p, d1u, d1v);
                return py::make_tuple(p, d1u, d1v);
            }, py::arg("u"), py::arg("v"),
            R"doc(
            Compute the point and first derivatives at (U, V).

            Requires C1 continuity.

            Args:
                u: The U parameter.
                v: The V parameter.

            Returns:
                tuple: (point, d1u, d1v) where d1u and d1v are the first
                       partial derivatives in U and V directions.
            )doc")

        .def("d2", [](const Geom_Surface& self, double u, double v) {
                gp_Pnt p;
                gp_Vec d1u, d1v, d2u, d2v, d2uv;
                self.D2(u, v, p, d1u, d1v, d2u, d2v, d2uv);
                return py::make_tuple(p, d1u, d1v, d2u, d2v, d2uv);
            }, py::arg("u"), py::arg("v"),
            R"doc(
            Compute the point, first and second derivatives at (U, V).

            Requires C2 continuity.

            Args:
                u: The U parameter.
                v: The V parameter.

            Returns:
                tuple: (point, d1u, d1v, d2u, d2v, d2uv) containing the point,
                       first derivatives, second derivatives, and mixed derivative.
            )doc")

        .def("d3", [](const Geom_Surface& self, double u, double v) {
                gp_Pnt p;
                gp_Vec d1u, d1v, d2u, d2v, d2uv, d3u, d3v, d3uuv, d3uvv;
                self.D3(u, v, p, d1u, d1v, d2u, d2v, d2uv, d3u, d3v, d3uuv, d3uvv);
                return py::make_tuple(p, d1u, d1v, d2u, d2v, d2uv, d3u, d3v, d3uuv, d3uvv);
            }, py::arg("u"), py::arg("v"),
            R"doc(
            Compute the point and derivatives up to third order at (U, V).

            Requires C3 continuity.

            Args:
                u: The U parameter.
                v: The V parameter.

            Returns:
                tuple: (point, d1u, d1v, d2u, d2v, d2uv, d3u, d3v, d3uuv, d3uvv)
                       containing all derivatives up to order 3.
            )doc")

        .def("dn", &Geom_Surface::DN, py::arg("u"), py::arg("v"), py::arg("nu"), py::arg("nv"),
            R"doc(
            Compute a specific partial derivative at (U, V).

            Returns the derivative of order Nu in U and Nv in V.
            Requires CNu continuity in U and CNv continuity in V.

            Args:
                u: The U parameter.
                v: The V parameter.
                nu: The derivative order in U (nu >= 0).
                nv: The derivative order in V (nv >= 0).

            Returns:
                gp.Vec: The partial derivative vector.

            Raises:
                Standard_RangeError: If nu + nv < 1 or nu < 0 or nv < 0.
            )doc")

        // --- Parameter transformation ---
        .def("transform_parameters", [](const Geom_Surface& self, double u, double v, const gp_Trsf& t) {
                Standard_Real u_out = u, v_out = v;
                self.TransformParameters(u_out, v_out, t);
                return py::make_tuple(u_out, v_out);
            }, py::arg("u"), py::arg("v"), py::arg("transform"),
            R"doc(
            Compute parameters on a transformed surface.

            For a point P(U, V) on this surface, returns the parameters (U', V')
            such that transformed_surface.Value(U', V') == P(U, V).Transformed(T).

            Args:
                u: The U parameter on this surface.
                v: The V parameter on this surface.
                transform: The geometric transformation.

            Returns:
                tuple: (u', v') parameters on the transformed surface.
            )doc")

        .def("parametric_transformation", &Geom_Surface::ParametricTransformation, py::arg("transform"),
            R"doc(
            Return a 2D transformation for parameter space.

            Returns a 2D transformation that maps (U, V) parameters on this
            surface to (U', V') parameters on the transformed surface.

            Args:
                transform: The 3D geometric transformation.

            Returns:
                gp.GTrsf2d: The 2D parameter transformation.
            )doc")

        // --- Callable interface ---
        .def("__call__", &Geom_Surface::Value, py::arg("u"), py::arg("v"),
            R"doc(
            Evaluate the surface at parameters (U, V).

            Allows using the surface as a callable: surface(u, v).

            Args:
                u: The U parameter.
                v: The V parameter.

            Returns:
                gp.Pnt: The 3D point on the surface.
            )doc")
        ;
    // Register implicit conversions from gbs types to Geom_Surface
    py::implicitly_convertible<gbs::BSSurface<double,3>, Geom_Surface>();
    py::implicitly_convertible<gbs::BSSurfaceRational<double,3>, Geom_Surface>();

    py::class_<Geom_BoundedSurface, opencascade::handle<Geom_BoundedSurface>, Geom_Surface>(m, "BoundedSurface",
        R"doc(
        Abstract base class for bounded surfaces in 3D space.

        A bounded surface is defined by a finite rectangle in its 2D parametric space:
        - U parameter ranges between u0 ("First u parameter") and u1 ("Last u parameter")
        - V parameter ranges between v0 ("First v parameter") and v1 ("Last v parameter")

        The surface is limited by four boundary curves:
        - u0 and u1 isoparametric curves in the U direction
        - v0 and v1 isoparametric curves in the V direction

        A bounded surface is always finite.

        Concrete implementations:
        - BezierSurface: Bezier polynomial surface
        - BSplineSurface: B-spline surface
        - RectangularTrimmedSurface: Surface trimmed by isoparametric curves
        )doc")
        // No additional methods - this is an abstract intermediate class
        ;


    // Bind spline surfaces from dedicated file
    bind_geom_surfaces_splines(m);

    py::class_<Geom_ElementarySurface, opencascade::handle<Geom_ElementarySurface>, Geom_Surface>(m, "ElementarySurface",
        R"doc(
        Abstract base class for elementary surfaces in 3D space.

        Elementary surfaces are defined by simple geometric shapes such as:
        - Planes
        - Cylinders
        - Cones
        - Spheres
        - Tori

        These surfaces have well-known mathematical representations and properties.
        All elementary surfaces are positioned in space by a coordinate system (gp_Ax3).
        )doc")
        // --- Position and axis properties ---
        .def_property("axis", &Geom_ElementarySurface::Axis, &Geom_ElementarySurface::SetAxis,
            R"doc(
            The main axis (Z-axis) of the local coordinate system.

            This defines the axis of symmetry for the surface. Can be both read and modified.

            Returns/Sets
            -----------
            gp_Ax1
                The main axis of symmetry for the surface
            )doc")
        .def_property("location", &Geom_ElementarySurface::Location, &Geom_ElementarySurface::SetLocation,
            R"doc(
            The location (origin) of the local coordinate system.

            Can be both read and modified.

            Returns/Sets
            -----------
            gp_Pnt
                The origin point of the local coordinate system
            )doc")
        .def_property("position", &Geom_ElementarySurface::Position, &Geom_ElementarySurface::SetPosition,
            R"doc(
            The local coordinate system of the surface.

            Can be both read and modified.

            Returns/Sets
            -----------
            gp_Ax3
                The complete coordinate system (origin, X, Y, Z axes)
            )doc")
        ;

    // Bind elementary surfaces from dedicated file
    bind_geom_surfaces_elementary(m);
}