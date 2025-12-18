#include <pybind11/pybind11.h>

#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepExtrema_SupportType.hxx>
#include <Extrema_ExtFlag.hxx>
#include <Extrema_ExtAlgo.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Pnt.hxx>

namespace py = pybind11;

void bind_brep_extrema(py::module_ &m)
{
    // Enumerations
    py::enum_<BRepExtrema_SupportType>(m, "SupportType",
        "Type of support where a solution is located")
        .value("IsVertex", BRepExtrema_IsVertex, "Solution is a vertex")
        .value("IsOnEdge", BRepExtrema_IsOnEdge, "Solution is on an edge")
        .value("IsInFace", BRepExtrema_IsInFace, "Solution is inside a face")
        .export_values();

    py::enum_<Extrema_ExtFlag>(m, "ExtFlag")
        .value("MIN", Extrema_ExtFlag_MIN)
        .value("MAX", Extrema_ExtFlag_MAX)
        .value("MINMAX", Extrema_ExtFlag_MINMAX)
        .export_values();

    py::enum_<Extrema_ExtAlgo>(m, "ExtAlgo")
        .value("Grad", Extrema_ExtAlgo_Grad)
        .value("Tree", Extrema_ExtAlgo_Tree)
        .export_values();

    py::class_<BRepExtrema_DistShapeShape>(m, "DistShapeShape",
        "Compute minimum distance between two shapes (Compound, CompSolid, Solid, Shell, Face, Wire, Edge, Vertex)")
        
        // Constructors
        .def(py::init<>(),
            "Create empty distance computation tool")
        
        .def(py::init<const TopoDS_Shape&, const TopoDS_Shape&, const Extrema_ExtFlag, const Extrema_ExtAlgo>(),
            py::arg("shape1"), py::arg("shape2"),
            py::arg("flag") = Extrema_ExtFlag_MINMAX,
            py::arg("algo") = Extrema_ExtAlgo_Grad,
            "Create tool and compute minimum distance using default deflection (Precision::Confusion())\n\n"
            "Parameters:\n"
            "  shape1: First shape for distance computation\n"
            "  shape2: Second shape for distance computation\n"
            "  flag: Extrema flag (obsolete, not used)\n"
            "  algo: Extrema algorithm (obsolete, not used)")
        
        .def(py::init<const TopoDS_Shape&, const TopoDS_Shape&, const Standard_Real, const Extrema_ExtFlag, const Extrema_ExtAlgo>(),
            py::arg("shape1"), py::arg("shape2"), py::arg("deflection"),
            py::arg("flag") = Extrema_ExtFlag_MINMAX,
            py::arg("algo") = Extrema_ExtAlgo_Grad,
            "Create tool and compute minimum distance with specified deflection\n\n"
            "Parameters:\n"
            "  shape1: First shape for distance computation\n"
            "  shape2: Second shape for distance computation\n"
            "  deflection: Precision of distance computation\n"
            "  flag: Extrema flag (obsolete, not used)\n"
            "  algo: Extrema algorithm (obsolete, not used)")
        
        // Configuration
        .def("set_deflection", &BRepExtrema_DistShapeShape::SetDeflection,
            py::arg("deflection"),
            "Set deflection for computation of minimum distance")
        
        .def("load_s1", &BRepExtrema_DistShapeShape::LoadS1,
            py::arg("shape1"),
            "Load first shape into extrema")
        
        .def("load_s2", &BRepExtrema_DistShapeShape::LoadS2,
            py::arg("shape2"),
            "Load second shape into extrema")
        
        // TODO add message support?
        .def("perform", [](BRepExtrema_DistShapeShape& self) {
            return self.Perform();
        },
            "Compute minimum distance (value and pair of points)\n\n"
            "Returns:\n"
            "  True if computation succeeded")
        
        // Properties (read-only)
        .def_property_readonly("is_done", &BRepExtrema_DistShapeShape::IsDone,
            "True if the minimum distance is found")
        
        .def_property_readonly("nb_solution", &BRepExtrema_DistShapeShape::NbSolution,
            "Number of solutions satisfying the minimum distance")
        
        .def_property_readonly("inner_solution", &BRepExtrema_DistShapeShape::InnerSolution,
            "True if one shape is a solid and the other is completely or partially inside")
        
        // Multi-threading property
        .def_property("is_multi_thread",
            &BRepExtrema_DistShapeShape::IsMultiThread,
            &BRepExtrema_DistShapeShape::SetMultiThread,
            "Enable/disable parallel computation (default: False)")
        
        // Results
        .def_property_readonly("value", &BRepExtrema_DistShapeShape::Value,
            "Return the value of the minimum distance")
        
        .def("point_on_shape1", &BRepExtrema_DistShapeShape::PointOnShape1,
            py::arg("n"),
            py::return_value_policy::reference_internal,
            "Return the point corresponding to the Nth solution on the first shape")
        
        .def("point_on_shape2", &BRepExtrema_DistShapeShape::PointOnShape2,
            py::arg("n"),
            py::return_value_policy::reference_internal,
            "Return the point corresponding to the Nth solution on the second shape")
        
        .def("support_type_shape1", &BRepExtrema_DistShapeShape::SupportTypeShape1,
            py::arg("n"),
            "Return the type of support where the Nth solution on the first shape is situated\n\n"
            "Returns:\n"
            "  IsVertex: Solution is a vertex\n"
            "  IsOnEdge: Solution is on an edge\n"
            "  IsInFace: Solution is inside a face")
        
        .def("support_type_shape2", &BRepExtrema_DistShapeShape::SupportTypeShape2,
            py::arg("n"),
            "Return the type of support where the Nth solution on the second shape is situated\n\n"
            "Returns:\n"
            "  IsVertex: Solution is a vertex\n"
            "  IsOnEdge: Solution is on an edge\n"
            "  IsInFace: Solution is inside a face")
        
        .def("support_on_shape1", &BRepExtrema_DistShapeShape::SupportOnShape1,
            py::arg("n"),
            "Return the support (Vertex, Edge or Face) where the Nth solution on the first shape is situated")
        
        .def("support_on_shape2", &BRepExtrema_DistShapeShape::SupportOnShape2,
            py::arg("n"),
            "Return the support (Vertex, Edge or Face) where the Nth solution on the second shape is situated")
        
        // Parameters on edges (with output parameters converted to tuples)
        .def("par_on_edge_s1", [](const BRepExtrema_DistShapeShape& self, Standard_Integer n) {
            Standard_Real t;
            self.ParOnEdgeS1(n, t);
            return t;
        }, py::arg("n"),
            "Return the parameter t if the Nth solution is on an edge of the first shape")
        
        .def("par_on_edge_s2", [](const BRepExtrema_DistShapeShape& self, Standard_Integer n) {
            Standard_Real t;
            self.ParOnEdgeS2(n, t);
            return t;
        }, py::arg("n"),
            "Return the parameter t if the Nth solution is on an edge of the second shape")
        
        // Parameters on faces (with output parameters converted to tuples)
        .def("par_on_face_s1", [](const BRepExtrema_DistShapeShape& self, Standard_Integer n) {
            Standard_Real u, v;
            self.ParOnFaceS1(n, u, v);
            return py::make_tuple(u, v);
        }, py::arg("n"),
            "Return the parameters (u, v) if the Nth solution is on a face of the first shape")
        
        .def("par_on_face_s2", [](const BRepExtrema_DistShapeShape& self, Standard_Integer n) {
            Standard_Real u, v;
            self.ParOnFaceS2(n, u, v);
            return py::make_tuple(u, v);
        }, py::arg("n"),
            "Return the parameters (u, v) if the Nth solution is on a face of the second shape")
        
        // Obsolete methods (kept for compatibility)
        .def("set_flag", &BRepExtrema_DistShapeShape::SetFlag,
            py::arg("flag"),
            "Set extrema flag (obsolete, not used)")
        
        .def("set_algo", &BRepExtrema_DistShapeShape::SetAlgo,
            py::arg("algo"),
            "Set extrema algorithm (obsolete, not used)")
        
        // Debug
        .def("dump", &BRepExtrema_DistShapeShape::Dump,
            py::arg("stream"),
            "Print information on the current state to output stream")
    ;
}
