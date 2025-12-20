#include <pybind11/pybind11.h>

#include <ShapeAnalysis_Edge.hxx>
#include <ShapeExtend_Status.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Vertex.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Surface.hxx>
#include <Geom2d_Curve.hxx>
#include <TopLoc_Location.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Pnt.hxx>

namespace py = pybind11;
// Declare opencascade::handle as a holder type
#include <Standard_Handle.hxx>
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_shape_analysis(py::module_ &m)
{
    // ShapeExtend_Status enum
    py::enum_<ShapeExtend_Status>(m, "ShapeExtendStatus")
        .value("OK", ShapeExtend_Status::ShapeExtend_OK)
        .value("Done1", ShapeExtend_Status::ShapeExtend_DONE1)
        .value("Done2", ShapeExtend_Status::ShapeExtend_DONE2)
        .value("Done3", ShapeExtend_Status::ShapeExtend_DONE3)
        .value("Done4", ShapeExtend_Status::ShapeExtend_DONE4)
        .value("Done5", ShapeExtend_Status::ShapeExtend_DONE5)
        .value("Done6", ShapeExtend_Status::ShapeExtend_DONE6)
        .value("Done7", ShapeExtend_Status::ShapeExtend_DONE7)
        .value("Done8", ShapeExtend_Status::ShapeExtend_DONE8)
        .value("Fail1", ShapeExtend_Status::ShapeExtend_FAIL1)
        .value("Fail2", ShapeExtend_Status::ShapeExtend_FAIL2)
        .value("Fail3", ShapeExtend_Status::ShapeExtend_FAIL3)
        .value("Fail4", ShapeExtend_Status::ShapeExtend_FAIL4)
        .value("Fail5", ShapeExtend_Status::ShapeExtend_FAIL5)
        .value("Fail6", ShapeExtend_Status::ShapeExtend_FAIL6)
        .value("Fail7", ShapeExtend_Status::ShapeExtend_FAIL7)
        .value("Fail8", ShapeExtend_Status::ShapeExtend_FAIL8)
        .export_values();

    py::class_<ShapeAnalysis_Edge>(m, "Edge",
        "Tool for analyzing edges.\n\n"
        "Queries geometrical representations of the edge (3d curve, pcurve\n"
        "on the given face or surface) and topological sub-shapes (bounding\n"
        "vertices).\n"
        "Provides methods for analyzing geometry and topology consistency\n"
        "(3d and pcurve(s) consistency, their adjacency to the vertices).")
        
        .def(py::init<>(),
            "Empty constructor; initializes Status to OK")
        
        // 3D curve methods
        .def("has_curve3d", &ShapeAnalysis_Edge::HasCurve3d,
            py::arg("edge"),
            "Returns True if the edge has a 3d curve")
        
        .def("curve3d",
            [](ShapeAnalysis_Edge& self, const TopoDS_Edge& edge, bool orient) {
                opencascade::handle<Geom_Curve> c3d;
                Standard_Real cf, cl;
                Standard_Boolean result = self.Curve3d(edge, c3d, cf, cl, orient);
                return py::make_tuple(result, c3d, cf, cl);
            },
            py::arg("edge"), py::arg("orient") = true,
            "Returns tuple (success, curve, first, last) for the edge's 3d curve.\n\n"
            "If orient is True (default), takes orientation into account:\n"
            "if the edge is reversed, first and last are toggled")
        
        .def("is_closed3d", &ShapeAnalysis_Edge::IsClosed3d,
            py::arg("edge"),
            "Returns True if the edge has a 3d curve, this curve is closed,\n"
            "and the edge has the same vertex at start and end")
        
        // PCurve methods
        .def("has_pcurve",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Face&>(&ShapeAnalysis_Edge::HasPCurve, py::const_),
            py::arg("edge"), py::arg("face"),
            "Returns True if the edge has a pcurve on the face")
        
        .def("has_pcurve_on_surface",
            py::overload_cast<const TopoDS_Edge&, const opencascade::handle<Geom_Surface>&, const TopLoc_Location&>(&ShapeAnalysis_Edge::HasPCurve, py::const_),
            py::arg("edge"), py::arg("surface"), py::arg("location"),
            "Returns True if the edge has a pcurve on the surface (with location)")
        
        .def("pcurve",
            [](const ShapeAnalysis_Edge& self, const TopoDS_Edge& edge, const TopoDS_Face& face, bool orient) {
                opencascade::handle<Geom2d_Curve> c2d;
                Standard_Real cf, cl;
                Standard_Boolean result = self.PCurve(edge, face, c2d, cf, cl, orient);
                return py::make_tuple(result, c2d, cf, cl);
            },
            py::arg("edge"), py::arg("face"), py::arg("orient") = true,
            "Returns tuple (success, pcurve, first, last) for the edge on the face.\n\n"
            "If orient is True (default), takes orientation into account:\n"
            "if the edge is reversed, first and last are toggled")
        
        .def("pcurve_on_surface",
            [](const ShapeAnalysis_Edge& self, const TopoDS_Edge& edge, 
               const opencascade::handle<Geom_Surface>& surface, const TopLoc_Location& location, bool orient) {
                opencascade::handle<Geom2d_Curve> c2d;
                Standard_Real cf, cl;
                Standard_Boolean result = self.PCurve(edge, surface, location, c2d, cf, cl, orient);
                return py::make_tuple(result, c2d, cf, cl);
            },
            py::arg("edge"), py::arg("surface"), py::arg("location"), py::arg("orient") = true,
            "Returns tuple (success, pcurve, first, last) for the edge on the surface.\n\n"
            "If orient is True (default), takes orientation into account:\n"
            "if the edge is reversed, first and last are toggled")
        
        // Boundary UV methods
        .def("bound_uv",
            [](const ShapeAnalysis_Edge& self, const TopoDS_Edge& edge, const TopoDS_Face& face) {
                gp_Pnt2d first, last;
                Standard_Boolean result = self.BoundUV(edge, face, first, last);
                return py::make_tuple(result, first, last);
            },
            py::arg("edge"), py::arg("face"),
            "Returns tuple (success, first_uv, last_uv) for the pcurve ends.\n"
            "Calls PCurve with orient=True")
        
        .def("bound_uv_on_surface",
            [](const ShapeAnalysis_Edge& self, const TopoDS_Edge& edge,
               const opencascade::handle<Geom_Surface>& surface, const TopLoc_Location& location) {
                gp_Pnt2d first, last;
                Standard_Boolean result = self.BoundUV(edge, surface, location, first, last);
                return py::make_tuple(result, first, last);
            },
            py::arg("edge"), py::arg("surface"), py::arg("location"),
            "Returns tuple (success, first_uv, last_uv) for the pcurve ends on surface")
        
        // Seam checking
        .def("is_seam",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Face&>(&ShapeAnalysis_Edge::IsSeam, py::const_),
            py::arg("edge"), py::arg("face"),
            "Returns True if the edge is a seam on the face")
        
        .def("is_seam_on_surface",
            py::overload_cast<const TopoDS_Edge&, const opencascade::handle<Geom_Surface>&, const TopLoc_Location&>(&ShapeAnalysis_Edge::IsSeam, py::const_),
            py::arg("edge"), py::arg("surface"), py::arg("location"),
            "Returns True if the edge has two pcurves on the surface (seam)")
        
        // Vertex methods
        .def("first_vertex", &ShapeAnalysis_Edge::FirstVertex,
            py::arg("edge"),
            "Returns start vertex of the edge (taking edge orientation into account)")
        
        .def("last_vertex", &ShapeAnalysis_Edge::LastVertex,
            py::arg("edge"),
            "Returns end vertex of the edge (taking edge orientation into account)")
        
        // Tangent methods
        .def("get_end_tangent2d",
            [](const ShapeAnalysis_Edge& self, const TopoDS_Edge& edge, const TopoDS_Face& face,
               bool at_end, Standard_Real dparam) {
                gp_Pnt2d pos;
                gp_Vec2d tang;
                Standard_Boolean result = self.GetEndTangent2d(edge, face, at_end, pos, tang, dparam);
                return py::make_tuple(result, pos, tang);
            },
            py::arg("edge"), py::arg("face"), py::arg("at_end"), py::arg("dparam") = 0.0,
            "Returns tuple (success, position, tangent) for the edge pcurve end.\n\n"
            "at_end: False for start, True for end of edge\n"
            "dparam: parameter increment for tangent computation\n\n"
            "Returns tangent at start (at_end=False) or end (at_end=True),\n"
            "regarding edge orientation. If edge is REVERSED, tangent is reversed.\n"
            "Returns True if pcurve exists and tangent is computed and non-null")
        
        .def("get_end_tangent2d_on_surface",
            [](const ShapeAnalysis_Edge& self, const TopoDS_Edge& edge,
               const opencascade::handle<Geom_Surface>& surface, const TopLoc_Location& location,
               bool at_end, Standard_Real dparam) {
                gp_Pnt2d pos;
                gp_Vec2d tang;
                Standard_Boolean result = self.GetEndTangent2d(edge, surface, location, at_end, pos, tang, dparam);
                return py::make_tuple(result, pos, tang);
            },
            py::arg("edge"), py::arg("surface"), py::arg("location"), py::arg("at_end"), py::arg("dparam") = 0.0,
            "Returns tuple (success, position, tangent) for the edge pcurve end on surface")
        
        // Vertex checking methods
        .def("check_vertices_with_curve3d", &ShapeAnalysis_Edge::CheckVerticesWithCurve3d,
            py::arg("edge"), py::arg("preci") = -1, py::arg("vtx") = 0,
            "Checks start/end vertex of edge for matching with 3d curve.\n\n"
            "Parameters:\n"
            "  vtx: 0 = both vertices (default), 1 = start only, 2 = end only\n"
            "  preci: precision to use (< 0: use vertex tolerances)")
        
        .def("check_vertices_with_pcurve",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Face&, const Standard_Real, const Standard_Integer>(&ShapeAnalysis_Edge::CheckVerticesWithPCurve),
            py::arg("edge"), py::arg("face"), py::arg("preci") = -1, py::arg("vtx") = 0,
            "Checks start/end vertex of edge for matching with pcurve on face.\n\n"
            "Parameters:\n"
            "  vtx: 0 = both vertices (default), 1 = start only, 2 = end only\n"
            "  preci: precision to use (< 0: use vertex tolerances)")
        
        .def("check_vertices_with_pcurve_on_surface",
            py::overload_cast<const TopoDS_Edge&, const opencascade::handle<Geom_Surface>&, const TopLoc_Location&, const Standard_Real, const Standard_Integer>(&ShapeAnalysis_Edge::CheckVerticesWithPCurve),
            py::arg("edge"), py::arg("surface"), py::arg("location"), py::arg("preci") = -1, py::arg("vtx") = 0,
            "Checks start/end vertex of edge for matching with pcurve on surface.\n\n"
            "Parameters:\n"
            "  vtx: 0 = both vertices (default), 1 = start only, 2 = end only\n"
            "  preci: precision to use (< 0: use vertex tolerances)")
        
        // Vertex tolerance checking
        .def("check_vertex_tolerance_on_face",
            [](ShapeAnalysis_Edge& self, const TopoDS_Edge& edge, const TopoDS_Face& face) {
                Standard_Real toler1, toler2;
                Standard_Boolean result = self.CheckVertexTolerance(edge, face, toler1, toler2);
                return py::make_tuple(result, toler1, toler2);
            },
            py::arg("edge"), py::arg("face"),
            "Returns tuple (needs_update, toler1, toler2).\n\n"
            "Checks if it's necessary to increase tolerances of edge vertices\n"
            "to comprise the ends of 3d curve and pcurve on the face.\n"
            "toler1: necessary tolerance for first vertex\n"
            "toler2: necessary tolerance for last vertex")
        
        .def("check_vertex_tolerance",
            [](ShapeAnalysis_Edge& self, const TopoDS_Edge& edge) {
                Standard_Real toler1, toler2;
                Standard_Boolean result = self.CheckVertexTolerance(edge, toler1, toler2);
                return py::make_tuple(result, toler1, toler2);
            },
            py::arg("edge"),
            "Returns tuple (needs_update, toler1, toler2).\n\n"
            "Checks if it's necessary to increase tolerances of edge vertices\n"
            "to comprise the ends of 3d curve and all pcurves stored in the edge.\n"
            "toler1: necessary tolerance for first vertex\n"
            "toler2: necessary tolerance for last vertex")
        
        // Curve orientation checking
        .def("check_curve3d_with_pcurve",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Face&>(&ShapeAnalysis_Edge::CheckCurve3dWithPCurve),
            py::arg("edge"), py::arg("face"),
            "Checks mutual orientation of 3d curve and pcurve on face.\n"
            "Analysis is based on curves' bounding points")
        
        .def("check_curve3d_with_pcurve_on_surface",
            py::overload_cast<const TopoDS_Edge&, const opencascade::handle<Geom_Surface>&, const TopLoc_Location&>(&ShapeAnalysis_Edge::CheckCurve3dWithPCurve),
            py::arg("edge"), py::arg("surface"), py::arg("location"),
            "Checks mutual orientation of 3d curve and pcurve on surface.\n"
            "Analysis is based on curves' bounding points")
        
        // Status checking
        .def("status", &ShapeAnalysis_Edge::Status,
            py::arg("status"),
            "Returns the status (as True/False) of the last check operation")
        
        // SameParameter checking
        .def("check_same_parameter",
            [](ShapeAnalysis_Edge& self, const TopoDS_Edge& edge, Standard_Integer nb_control) {
                Standard_Real maxdev;
                Standard_Boolean result = self.CheckSameParameter(edge, maxdev, nb_control);
                return py::make_tuple(result, maxdev);
            },
            py::arg("edge"), py::arg("nb_control") = 23,
            "Returns tuple (is_valid, max_deviation).\n\n"
            "Checks the edge to be SameParameter.\n"
            "Calculates the maximal deviation between 3d curve and each pcurve\n"
            "of the edge on nb_control equidistant points (default 23, as in BRepCheck).\n"
            "Returns True if deviation <= edge tolerance, False otherwise")
        
        .def("check_same_parameter_on_face",
            [](ShapeAnalysis_Edge& self, const TopoDS_Edge& edge, const TopoDS_Face& face, Standard_Integer nb_control) {
                Standard_Real maxdev;
                Standard_Boolean result = self.CheckSameParameter(edge, face, maxdev, nb_control);
                return py::make_tuple(result, maxdev);
            },
            py::arg("edge"), py::arg("face"), py::arg("nb_control") = 23,
            "Returns tuple (is_valid, max_deviation).\n\n"
            "Checks the edge to be SameParameter on the specific face.\n"
            "Calculates the maximal deviation between 3d curve and pcurve\n"
            "on nb_control equidistant points (default 23, as in BRepCheck).\n"
            "Returns True if deviation <= edge tolerance, False otherwise")
        
        // PCurve range checking
        .def("check_pcurve_range", &ShapeAnalysis_Edge::CheckPCurveRange,
            py::arg("first"), py::arg("last"), py::arg("pcurve"),
            "Checks possibility for pcurve to have range [first, last] (edge range)\n"
            "with respect to real first, last parameters of the pcurve")
        
        // Overlapping checking
        .def("check_overlapping",
            [](ShapeAnalysis_Edge& self, const TopoDS_Edge& edge1, const TopoDS_Edge& edge2, Standard_Real domain_dist) {
                Standard_Real tol_overlap = 0.0;
                Standard_Boolean result = self.CheckOverlapping(edge1, edge2, tol_overlap, domain_dist);
                return py::make_tuple(result, tol_overlap);
            },
            py::arg("edge1"), py::arg("edge2"), py::arg("domain_dist") = 0.0,
            "Returns tuple (is_overlapped, tolerance).\n\n"
            "Checks if the first edge is overlapped with second edge.\n"
            "If distance between edges < tolerance, edges are overlapped.\n"
            "domain_dist: length of part of edges on which edges are overlapped")
    ;
}