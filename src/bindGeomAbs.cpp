#include <pybind11/pybind11.h>

#include <GeomAbs_Shape.hxx>
#include <GeomAbs_JoinType.hxx>

namespace py = pybind11;

void bind_geom_abs(py::module_ &m)
{
    py::enum_<GeomAbs_Shape>(m, "Shape")
        .value("GeomAbs_C0", GeomAbs_C0)
        .value("GeomAbs_C1", GeomAbs_C1)
        .value("GeomAbs_C2", GeomAbs_C2)
        .value("GeomAbs_C3", GeomAbs_C3)
        .value("GeomAbs_G1", GeomAbs_G1)
        .value("GeomAbs_G2", GeomAbs_G2)
        .value("GeomAbs_CN", GeomAbs_CN)
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
}