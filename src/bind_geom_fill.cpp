#include <pybind11/pybind11.h>
#include <GeomFill_Trihedron.hxx>

namespace py = pybind11;


void bind_geom_fill(py::module_ &m) {
     py::enum_<GeomFill_Trihedron>(m, "Trihedron")
        .value("CorrectedFrenet", GeomFill_IsCorrectedFrenet)
        .value("Fixed", GeomFill_IsFixed)
        .value("Frenet", GeomFill_IsFrenet)
        .value("ConstantNormal", GeomFill_IsConstantNormal)
        .value("Darboux", GeomFill_IsDarboux)
        .value("GuideAC", GeomFill_IsGuideAC)
        .value("GuidePlan", GeomFill_IsGuidePlan)
        .value("GuideACWithContact", GeomFill_IsGuideACWithContact)
        .value("GuidePlanWithContact", GeomFill_IsGuidePlanWithContact)
        .value("DiscreteTrihedron", GeomFill_IsDiscreteTrihedron)
        .export_values();
}