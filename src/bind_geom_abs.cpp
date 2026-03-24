#include <pybind11/pybind11.h>

#include <GeomAbs_Shape.hxx>
#include <GeomAbs_JoinType.hxx>
#include <GeomAbs_CurveType.hxx>
#include <GeomAbs_IsoType.hxx>
#include <GeomAbs_SurfaceType.hxx>

namespace py = pybind11;

void bind_geom_abs(py::module_ &m)
{
    py::enum_<GeomAbs_Shape>(m, "Shape")
        .value("C0", GeomAbs_C0)
        .value("C1", GeomAbs_C1)
        .value("C2", GeomAbs_C2)
        .value("C3", GeomAbs_C3)
        .value("G1", GeomAbs_G1)
        .value("G2", GeomAbs_G2)
        .value("CN", GeomAbs_CN)
        .export_values();

    // GeomAbs_JoinType enum
    py::enum_<GeomAbs_JoinType>(m, "JoinType")
        .value("Arc", GeomAbs_JoinType::GeomAbs_Arc, 
            "Vertices generate revolved pipes about the axis passing along the vertex")
        .value("Tangent", GeomAbs_JoinType::GeomAbs_Tangent,
            "Tangent join type")
        .value("Intersection", GeomAbs_JoinType::GeomAbs_Intersection,
            "Intersection join type")
        .export_values();

    // GeomAbs_CurveType enum - identifies the type of a curve
    py::enum_<GeomAbs_CurveType>(m, "CurveType",
        "Identifies the type of a curve.")
        .value("Line", GeomAbs_Line, "Straight line")
        .value("Circle", GeomAbs_Circle, "Circle")
        .value("Ellipse", GeomAbs_Ellipse, "Ellipse")
        .value("Hyperbola", GeomAbs_Hyperbola, "Hyperbola")
        .value("Parabola", GeomAbs_Parabola, "Parabola")
        .value("BezierCurve", GeomAbs_BezierCurve, "Bezier curve")
        .value("BSplineCurve", GeomAbs_BSplineCurve, "BSpline curve")
        .value("OffsetCurve", GeomAbs_OffsetCurve, "Offset curve")
        .value("OtherCurve", GeomAbs_OtherCurve, "Other curve type")
        .export_values();

    // GeomAbs_IsoType enum - describes if a curve is a U or V isoparametric
    py::enum_<GeomAbs_IsoType>(m, "IsoType",
        "Describes if a curve is a U or V isoparametric curve on a surface.")
        .value("IsoU", GeomAbs_IsoU, "U isoparametric curve (constant U)")
        .value("IsoV", GeomAbs_IsoV, "V isoparametric curve (constant V)")
        .value("NoneIso", GeomAbs_NoneIso, "Not an isoparametric curve")
        .export_values();

    // GeomAbs_SurfaceType enum - identifies the type of a surface
    py::enum_<GeomAbs_SurfaceType>(m, "SurfaceType",
        "Identifies the type of a surface.")
        .value("Plane", GeomAbs_Plane, "Planar surface")
        .value("Cylinder", GeomAbs_Cylinder, "Cylindrical surface")
        .value("Cone", GeomAbs_Cone, "Conical surface")
        .value("Sphere", GeomAbs_Sphere, "Spherical surface")
        .value("Torus", GeomAbs_Torus, "Toroidal surface")
        .value("BezierSurface", GeomAbs_BezierSurface, "Bezier surface")
        .value("BSplineSurface", GeomAbs_BSplineSurface, "BSpline surface")
        .value("SurfaceOfRevolution", GeomAbs_SurfaceOfRevolution, "Surface of revolution")
        .value("SurfaceOfExtrusion", GeomAbs_SurfaceOfExtrusion, "Surface of extrusion")
        .value("OffsetSurface", GeomAbs_OffsetSurface, "Offset surface")
        .value("OtherSurface", GeomAbs_OtherSurface, "Other surface type")
        .export_values();
}