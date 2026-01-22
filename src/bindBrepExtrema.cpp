#include <pybind11/pybind11.h>

#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepExtrema_SupportType.hxx>
#include <Extrema_ExtFlag.hxx>
#include <Extrema_ExtAlgo.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <gp_Pnt.hxx>
#include <BRepExtrema_ExtCC.hxx>
#include <BRepExtrema_ExtCF.hxx>
#include <BRepExtrema_ExtFF.hxx>
#include <BRepExtrema_ExtPC.hxx>
#include <BRepExtrema_ExtPF.hxx>


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

    // =========================================================================
    // BRepExtrema_ExtPC - Extrema between a vertex and an edge
    // =========================================================================
    py::class_<BRepExtrema_ExtPC>(m, "ExtPC",
        "Computes extrema distances between a vertex (point) and an edge (curve).\n\n"
        "This class finds all local extremum distances between a point and a curve,\n"
        "including both minima and maxima.")
        
        .def(py::init<>(), "Creates an empty ExtPC object")
        
        .def(py::init<const TopoDS_Vertex&, const TopoDS_Edge&>(),
            py::arg("vertex"), py::arg("edge"),
            "Creates and computes all extremum distances between vertex and edge")
        
        .def("initialize", &BRepExtrema_ExtPC::Initialize,
            py::arg("edge"),
            "Initializes the algorithm with an edge")
        
        .def("perform", &BRepExtrema_ExtPC::Perform,
            py::arg("vertex"),
            "Computes extrema for the given vertex")
        
        .def_property_readonly("is_done", &BRepExtrema_ExtPC::IsDone,
            "True if the distances are found")
        
        .def_property_readonly("nb_ext", &BRepExtrema_ExtPC::NbExt,
            "Returns the number of extremum distances")
        
        .def("is_min", &BRepExtrema_ExtPC::IsMin,
            py::arg("n"),
            "Returns True if the N-th extremum distance is a minimum")
        
        .def("square_distance", &BRepExtrema_ExtPC::SquareDistance,
            py::arg("n"),
            "Returns the value of the N-th extremum square distance")
        
        .def("parameter", &BRepExtrema_ExtPC::Parameter,
            py::arg("n"),
            "Returns the parameter on the edge of the N-th extremum distance")
        
        .def("point", &BRepExtrema_ExtPC::Point,
            py::arg("n"),
            "Returns the point of the N-th extremum distance")
        .def("points",[](const BRepExtrema_ExtPC& self) {
            std::vector<gp_Pnt> pts;
            Standard_Integer n = self.NbExt();
            for (Standard_Integer i = 1; i <= n; i++) {
                pts.push_back(self.Point(i));
            }
            return pts;
        },
            "Returns all extremum points as a list")
        
        .def("trimmed_square_distances", [](const BRepExtrema_ExtPC& self) {
            Standard_Real dist1, dist2;
            gp_Pnt pnt1, pnt2;
            self.TrimmedSquareDistances(dist1, dist2, pnt1, pnt2);
            return py::make_tuple(dist1, dist2, pnt1, pnt2);
        },
            "Returns (dist1, dist2, pnt1, pnt2) for trimmed curve endpoints.\n"
            "dist1: square distance to first parameter point\n"
            "dist2: square distance to last parameter point")
    ;

    // =========================================================================
    // BRepExtrema_ExtPF - Extrema between a vertex and a face
    // =========================================================================
    py::class_<BRepExtrema_ExtPF>(m, "ExtPF",
        "Computes extrema distances between a vertex (point) and a face (surface).\n\n"
        "This class finds all local extremum distances between a point and a surface.")
        
        .def(py::init<>(), "Creates an empty ExtPF object")
        
        .def(py::init<const TopoDS_Vertex&, const TopoDS_Face&, const Extrema_ExtFlag, const Extrema_ExtAlgo>(),
            py::arg("vertex"), py::arg("face"),
            py::arg("flag") = Extrema_ExtFlag_MINMAX,
            py::arg("algo") = Extrema_ExtAlgo_Grad,
            "Creates and computes all extremum distances between vertex and face")
        
        .def("initialize", &BRepExtrema_ExtPF::Initialize,
            py::arg("face"),
            py::arg("flag") = Extrema_ExtFlag_MINMAX,
            py::arg("algo") = Extrema_ExtAlgo_Grad,
            "Initializes the algorithm with a face")
        
        .def("perform", &BRepExtrema_ExtPF::Perform,
            py::arg("vertex"), py::arg("face"),
            "Computes extrema for the given vertex and face")
        
        .def_property_readonly("is_done", &BRepExtrema_ExtPF::IsDone,
            "True if the distances are found")
        
        .def_property_readonly("nb_ext", &BRepExtrema_ExtPF::NbExt,
            "Returns the number of extremum distances")
        
        .def("square_distance", &BRepExtrema_ExtPF::SquareDistance,
            py::arg("n"),
            "Returns the value of the N-th extremum square distance")
        
        .def("parameter", [](const BRepExtrema_ExtPF& self, Standard_Integer n) {
            Standard_Real u, v;
            self.Parameter(n, u, v);
            return py::make_tuple(u, v);
        }, py::arg("n"),
            "Returns the (u, v) parameters on the face of the N-th extremum distance")
        .def("parameters",[](const BRepExtrema_ExtPF& self) {
            std::vector<py::tuple> params;
            Standard_Integer n = self.NbExt();
            for (Standard_Integer i = 1; i <= n; i++) {
                Standard_Real u, v;
                self.Parameter(i, u, v);
                params.push_back(py::make_tuple(u, v));
            }
            return params;
        },
            "Returns all extremum parameters as a list of (u, v) tuples")
        
        .def("point", &BRepExtrema_ExtPF::Point,
            py::arg("n"),
            "Returns the point of the N-th extremum distance")
        .def("points",[](const BRepExtrema_ExtPF& self) {
            std::vector<gp_Pnt> pts;
            Standard_Integer n = self.NbExt();
            for (Standard_Integer i = 1; i <= n; i++) {
                pts.push_back(self.Point(i));
            }
            return pts;
        },
            "Returns all extremum points as a list")
        
        .def("set_flag", &BRepExtrema_ExtPF::SetFlag,
            py::arg("flag"),
            "Sets the extrema flag (MIN, MAX, or MINMAX)")
        
        .def("set_algo", &BRepExtrema_ExtPF::SetAlgo,
            py::arg("algo"),
            "Sets the extrema algorithm (Grad or Tree)")
    ;

    // =========================================================================
    // BRepExtrema_ExtCC - Extrema between two edges
    // =========================================================================
    py::class_<BRepExtrema_ExtCC>(m, "ExtCC",
        "Computes extrema distances between two edges (curves).\n\n"
        "This class finds all local extremum distances between two curves.")
        
        .def(py::init<>(), "Creates an empty ExtCC object")
        
        .def(py::init<const TopoDS_Edge&, const TopoDS_Edge&>(),
            py::arg("edge1"), py::arg("edge2"),
            "Creates and computes all extremum distances between two edges")
        
        .def("initialize", &BRepExtrema_ExtCC::Initialize,
            py::arg("edge2"),
            "Initializes the algorithm with the second edge")
        
        .def("perform", &BRepExtrema_ExtCC::Perform,
            py::arg("edge1"),
            "Computes extrema for the given first edge")
        
        .def_property_readonly("is_done", &BRepExtrema_ExtCC::IsDone,
            "True if the distances are found")
        
        .def_property_readonly("nb_ext", &BRepExtrema_ExtCC::NbExt,
            "Returns the number of extremum distances")
        
        .def_property_readonly("is_parallel", &BRepExtrema_ExtCC::IsParallel,
            "Returns True if the two edges are parallel")
        
        .def("square_distance", &BRepExtrema_ExtCC::SquareDistance,
            py::arg("n"),
            "Returns the value of the N-th extremum square distance")
        .def("square_distances",[](const BRepExtrema_ExtCC& self) {
            std::vector<Standard_Real> dists;
            Standard_Integer n = self.NbExt();
            for (Standard_Integer i = 1; i <= n; i++) {
                dists.push_back(self.SquareDistance(i));
            }
            return dists;
        },
            "Returns all extremum square distances as a list")
        
        .def("parameter_on_e1", &BRepExtrema_ExtCC::ParameterOnE1,
            py::arg("n"),
            "Returns the parameter on the first edge of the N-th extremum distance")
        
        .def("parameter_on_e2", &BRepExtrema_ExtCC::ParameterOnE2,
            py::arg("n"),
            "Returns the parameter on the second edge of the N-th extremum distance")
        
        .def("point_on_e1", &BRepExtrema_ExtCC::PointOnE1,
            py::arg("n"),
            "Returns the point on the first edge of the N-th extremum distance")
        
        .def("point_on_e2", &BRepExtrema_ExtCC::PointOnE2,
            py::arg("n"),
            "Returns the point on the second edge of the N-th extremum distance")
        
        .def("pamameters_and_points", [](const BRepExtrema_ExtCC& self){
            auto n = self.NbExt();
            std::vector<py::tuple> results;
            for (Standard_Integer i = 1; i <= n; i++) {
                auto t1 = self.ParameterOnE1(i);
                auto t2 = self.ParameterOnE2(i);
                auto p1 = self.PointOnE1(i);
                auto p2 = self.PointOnE2(i);
                auto d = self.SquareDistance(i);
                results.push_back(py::make_tuple(py::make_tuple(t1, p1), py::make_tuple(t2, p2), d));
            }
            return results;
        })
        
        .def("trimmed_square_distances", [](const BRepExtrema_ExtCC& self) {
            Standard_Real dist11, dist12, dist21, dist22;
            gp_Pnt p11, p12, p21, p22;
            self.TrimmedSquareDistances(dist11, dist12, dist21, dist22, p11, p12, p21, p22);
            return py::make_tuple(
                py::make_tuple(dist11, dist12, dist21, dist22),
                py::make_tuple(p11, p12, p21, p22)
            );
        },
            "Returns ((dist11, dist12, dist21, dist22), (p11, p12, p21, p22)).\n"
            "Square distances between trimmed curve endpoints:\n"
            "  dist11: E1.First to E2.First\n"
            "  dist12: E1.First to E2.Last\n"
            "  dist21: E1.Last to E2.First\n"
            "  dist22: E1.Last to E2.Last")
    ;

    // =========================================================================
    // BRepExtrema_ExtCF - Extrema between an edge and a face
    // =========================================================================
    py::class_<BRepExtrema_ExtCF>(m, "ExtCF",
        "Computes extrema distances between an edge (curve) and a face (surface).\n\n"
        "This class finds all local extremum distances between a curve and a surface.")
        
        .def(py::init<>(), "Creates an empty ExtCF object")
        
        .def(py::init<const TopoDS_Edge&, const TopoDS_Face&>(),
            py::arg("edge"), py::arg("face"),
            "Creates and computes all extremum distances between edge and face")
        
        .def("initialize", &BRepExtrema_ExtCF::Initialize,
            py::arg("edge"), py::arg("face"),
            "Initializes the algorithm with an edge and face")
        
        .def("perform", &BRepExtrema_ExtCF::Perform,
            py::arg("edge"), py::arg("face"),
            "Computes extrema for the given edge and face")
        
        .def_property_readonly("is_done", &BRepExtrema_ExtCF::IsDone,
            "True if the distances are found")
        
        .def_property_readonly("nb_ext", &BRepExtrema_ExtCF::NbExt,
            "Returns the number of extremum distances")
        
        .def_property_readonly("is_parallel", &BRepExtrema_ExtCF::IsParallel,
            "Returns True if the curve lies on a parallel surface")
        
        .def("square_distance", &BRepExtrema_ExtCF::SquareDistance,
            py::arg("n"),
            "Returns the value of the N-th extremum square distance")
        
        .def("parameter_on_edge", &BRepExtrema_ExtCF::ParameterOnEdge,
            py::arg("n"),
            "Returns the parameter on the edge of the N-th extremum distance")
        
        .def("parameter_on_face", [](const BRepExtrema_ExtCF& self, Standard_Integer n) {
            Standard_Real u, v;
            self.ParameterOnFace(n, u, v);
            return py::make_tuple(u, v);
        }, py::arg("n"),
            "Returns the (u, v) parameters on the face of the N-th extremum distance")
        
        .def("point_on_edge", &BRepExtrema_ExtCF::PointOnEdge,
            py::arg("n"),
            "Returns the point on the edge of the N-th extremum distance")
        
        .def("point_on_face", &BRepExtrema_ExtCF::PointOnFace,
            py::arg("n"),
            "Returns the point on the face of the N-th extremum distance")
        .def("points_and_parameters", [](const BRepExtrema_ExtCF& self){
            auto n = self.NbExt();
            std::vector<py::tuple> results;
            for (Standard_Integer i = 1; i <= n; i++) {
                auto t = self.ParameterOnEdge(i);
                Standard_Real u, v;
                self.ParameterOnFace(i, u, v);
                auto p1 = self.PointOnEdge(i);
                auto p2 = self.PointOnFace(i);
                auto d = self.SquareDistance(i);
                results.push_back(py::make_tuple(py::make_tuple(t, p1), py::make_tuple(u, v, p2), d));
            }
            return results;
        })
    ;

    // =========================================================================
    // BRepExtrema_ExtFF - Extrema between two faces
    // =========================================================================
    py::class_<BRepExtrema_ExtFF>(m, "ExtFF",
        "Computes extrema distances between two faces (surfaces).\n\n"
        "This class finds all local extremum distances between two surfaces.")
        
        .def(py::init<>(), "Creates an empty ExtFF object")
        
        .def(py::init<const TopoDS_Face&, const TopoDS_Face&>(),
            py::arg("face1"), py::arg("face2"),
            "Creates and computes all extremum distances between two faces")
        
        .def("initialize", &BRepExtrema_ExtFF::Initialize,
            py::arg("face2"),
            "Initializes the algorithm with the second face")
        
        .def("perform", &BRepExtrema_ExtFF::Perform,
            py::arg("face1"), py::arg("face2"),
            "Computes extrema for the given faces")
        
        .def_property_readonly("is_done", &BRepExtrema_ExtFF::IsDone,
            "True if the distances are found")
        
        .def_property_readonly("nb_ext", &BRepExtrema_ExtFF::NbExt,
            "Returns the number of extremum distances")
        
        .def_property_readonly("is_parallel", &BRepExtrema_ExtFF::IsParallel,
            "Returns True if the two surfaces are parallel")
        
        .def("square_distance", &BRepExtrema_ExtFF::SquareDistance,
            py::arg("n"),
            "Returns the value of the N-th extremum square distance")
        
        .def("parameter_on_face1", [](const BRepExtrema_ExtFF& self, Standard_Integer n) {
            Standard_Real u, v;
            self.ParameterOnFace1(n, u, v);
            return py::make_tuple(u, v);
        }, py::arg("n"),
            "Returns the (u, v) parameters on face1 of the N-th extremum distance")
        
        .def("parameter_on_face2", [](const BRepExtrema_ExtFF& self, Standard_Integer n) {
            Standard_Real u, v;
            self.ParameterOnFace2(n, u, v);
            return py::make_tuple(u, v);
        }, py::arg("n"),
            "Returns the (u, v) parameters on face2 of the N-th extremum distance")
        
        .def("point_on_face1", &BRepExtrema_ExtFF::PointOnFace1,
            py::arg("n"),
            "Returns the point on face1 of the N-th extremum distance")
        
        .def("point_on_face2", &BRepExtrema_ExtFF::PointOnFace2,
            py::arg("n"),
            "Returns the point on face2 of the N-th extremum distance")
        .def("points_and_parameters", [](const BRepExtrema_ExtFF& self){
            auto n = self.NbExt();
            std::vector<py::tuple> results;
            for (Standard_Integer i = 1; i <= n; i++) {
                Standard_Real u1, v1;
                self.ParameterOnFace1(i, u1, v1);
                Standard_Real u2, v2;
                self.ParameterOnFace2(i, u2, v2);
                auto p1 = self.PointOnFace1(i);
                auto p2 = self.PointOnFace2(i);
                auto d = self.SquareDistance(i);
                results.push_back(py::make_tuple(py::make_tuple(u1, v1, p1), py::make_tuple(u2, v2, p2), d));
            }
            return results;
        })
    ;
    
}
