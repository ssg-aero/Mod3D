#include <pybind11/pybind11.h>

#include <ShapeAnalysis_Edge.hxx>
#include <ShapeAnalysis_Surface.hxx>
#include <ShapeAnalysis_Wire.hxx>
#include <ShapeExtend_Status.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Vertex.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Surface.hxx>
#include <Geom2d_Curve.hxx>
#include <GeomAdaptor_Surface.hxx>
#include <TopLoc_Location.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Pnt.hxx>
#include <Bnd_Box.hxx>

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

    // =========================================================================
    // ShapeAnalysis_Surface - Surface analyzer (singularities, closure,
    //                                            UV-from-3D projection)
    // =========================================================================
    py::class_<ShapeAnalysis_Surface, opencascade::handle<ShapeAnalysis_Surface>>(m, "Surface",
        R"doc(
        Surface analyzer complementing Geom_Surface.

        Detects surface singularities, checks spatial closure, and computes
        projections of 3D points onto a surface. Caches intermediate results
        across calls, so reuse a single instance when querying multiple
        properties of the same surface.

        Example:
            sas = ShapeAnalysis.Surface(my_surface)
            if sas.is_u_closed():
                ...
            uv = sas.value_of_uv(p3d, 1e-6)
        )doc")

        .def(py::init<const opencascade::handle<Geom_Surface>&>(),
            py::arg("surface"),
            "Creates an analyzer for the given Geom_Surface.")

        .def("init",
            py::overload_cast<const opencascade::handle<Geom_Surface>&>(&ShapeAnalysis_Surface::Init),
            py::arg("surface"),
            "Loads a new surface and resets cached results.")

        .def("init_from",
            py::overload_cast<const opencascade::handle<ShapeAnalysis_Surface>&>(&ShapeAnalysis_Surface::Init),
            py::arg("other"),
            "Copies analyzer state from another instance without recomputing.")

        .def("set_domain", &ShapeAnalysis_Surface::SetDomain,
            py::arg("u1"), py::arg("u2"), py::arg("v1"), py::arg("v2"),
            "Restricts the analyzer to a sub-domain of the surface.")

        .def_property_readonly("surface", &ShapeAnalysis_Surface::Surface,
            "Returns the underlying Geom_Surface.")

        .def_property_readonly("adaptor3d", &ShapeAnalysis_Surface::Adaptor3d,
            "Returns the GeomAdaptor_Surface (creating it on first access).")

        .def_property_readonly("true_adaptor3d", &ShapeAnalysis_Surface::TrueAdaptor3d,
            "Returns the cached GeomAdaptor_Surface, or a null handle if\n"
            "adaptor3d has never been called.")

        .def_property_readonly("gap", &ShapeAnalysis_Surface::Gap,
            "Returns the 3D distance recorded by the last query\n"
            "(IsDegenerated, IsUClosed/IsVClosed, ValueOfUV, ...).")

        // Value computation
        .def("value",
            py::overload_cast<const Standard_Real, const Standard_Real>(&ShapeAnalysis_Surface::Value),
            py::arg("u"), py::arg("v"),
            "Returns the 3D point at parameters (u, v).")

        .def("value_at",
            py::overload_cast<const gp_Pnt2d&>(&ShapeAnalysis_Surface::Value),
            py::arg("uv"),
            "Returns the 3D point at the parametric point uv.")

        // Closure
        .def("is_u_closed", &ShapeAnalysis_Surface::IsUClosed,
            py::arg("preci") = -1.0,
            "Returns True if the surface is spatially closed in U with the\n"
            "given precision. preci < 0 falls back to Precision::Confusion.")

        .def("is_v_closed", &ShapeAnalysis_Surface::IsVClosed,
            py::arg("preci") = -1.0,
            "Returns True if the surface is spatially closed in V with the\n"
            "given precision. preci < 0 falls back to Precision::Confusion.")

        .def_property_readonly("u_close_val", &ShapeAnalysis_Surface::UCloseVal,
            "Minimum precision at which the surface is considered U-closed.")

        .def_property_readonly("v_close_val", &ShapeAnalysis_Surface::VCloseVal,
            "Minimum precision at which the surface is considered V-closed.")

        // Bounds and isos
        .def("bounds",
            [](const ShapeAnalysis_Surface& self) {
                Standard_Real uf, ul, vf, vl;
                self.Bounds(uf, ul, vf, vl);
                return py::make_tuple(uf, ul, vf, vl);
            },
            "Returns the buffered (u_first, u_last, v_first, v_last) bounds.")

        .def("compute_bound_isos", &ShapeAnalysis_Surface::ComputeBoundIsos,
            "Pre-computes the four boundary iso-curves, protecting against\n"
            "exceptions raised by the underlying Geom_Surface.")

        .def("u_iso", &ShapeAnalysis_Surface::UIso,
            py::arg("u"),
            "Returns the U=const iso-curve, or a null handle on failure.\n"
            "Boundary isos are cached.")

        .def("v_iso", &ShapeAnalysis_Surface::VIso,
            py::arg("v"),
            "Returns the V=const iso-curve, or a null handle on failure.\n"
            "Boundary isos are cached.")

        // Singularities
        .def("has_singularities", &ShapeAnalysis_Surface::HasSingularities,
            py::arg("preci"),
            "Returns True if at least one degenerated iso has size <= preci.")

        .def("nb_singularities", &ShapeAnalysis_Surface::NbSingularities,
            py::arg("preci"),
            "Returns the count of degenerated isos with size <= preci\n"
            "(always between 0 and 4).")

        .def("singularity",
            [](ShapeAnalysis_Surface& self, Standard_Integer num) {
                Standard_Real preci, firstpar, lastpar;
                gp_Pnt p3d;
                gp_Pnt2d firstP2d, lastP2d;
                Standard_Boolean uisodeg;
                Standard_Boolean ok = self.Singularity(num, preci, p3d, firstP2d, lastP2d,
                                                       firstpar, lastpar, uisodeg);
                return py::make_tuple(ok, preci, p3d, firstP2d, lastP2d,
                                      firstpar, lastpar, uisodeg);
            },
            py::arg("num"),
            "Returns tuple (ok, preci, p3d, first_p2d, last_p2d,\n"
            "               first_par, last_par, u_iso_deg) for singularity num.\n\n"
            "num is 1-indexed. Returns ok=False if num is out of range.")

        .def("is_degenerated",
            [](ShapeAnalysis_Surface& self, const gp_Pnt& p3d, Standard_Real preci) {
                return static_cast<bool>(self.IsDegenerated(p3d, preci));
            },
            py::arg("p3d"), py::arg("preci"),
            "Returns True if some iso is degenerated with size <= preci\n"
            "and p3d is within preci of the corresponding singular point.")

        .def("is_degenerated_pcurve",
            [](ShapeAnalysis_Surface& self, const gp_Pnt2d& p2d1, const gp_Pnt2d& p2d2,
               Standard_Real tol, Standard_Real ratio) {
                return static_cast<bool>(self.IsDegenerated(p2d1, p2d2, tol, ratio));
            },
            py::arg("p2d1"), py::arg("p2d2"), py::arg("tol"), py::arg("ratio"),
            "Returns True if the straight 2D segment p2d1->p2d2 lies in a\n"
            "surface singularity (3D length below tol while 2D length is\n"
            "ratio times the corresponding 2D Resolution).")

        .def("degenerated_values",
            [](ShapeAnalysis_Surface& self, const gp_Pnt& p3d, Standard_Real preci,
               Standard_Boolean forward) {
                gp_Pnt2d firstP2d, lastP2d;
                Standard_Real firstpar, lastpar;
                Standard_Boolean ok = self.DegeneratedValues(p3d, preci, firstP2d, lastP2d,
                                                             firstpar, lastpar, forward);
                return py::make_tuple(ok, firstP2d, lastP2d, firstpar, lastpar);
            },
            py::arg("p3d"), py::arg("preci"), py::arg("forward") = true,
            "Returns tuple (ok, first_p2d, last_p2d, first_par, last_par) for\n"
            "the first degenerated iso matching p3d within preci.")

        .def("project_degenerated",
            [](ShapeAnalysis_Surface& self, const gp_Pnt& p3d, Standard_Real preci,
               const gp_Pnt2d& neighbour) {
                gp_Pnt2d result;
                Standard_Boolean ok = self.ProjectDegenerated(p3d, preci, neighbour, result);
                return py::make_tuple(ok, result);
            },
            py::arg("p3d"), py::arg("preci"), py::arg("neighbour"),
            "Returns tuple (ok, uv) where uv has its indeterminate coordinate\n"
            "filled from neighbour when p3d lies on a degenerated iso.")

        // Projection
        .def("value_of_uv", &ShapeAnalysis_Surface::ValueOfUV,
            py::arg("p3d"), py::arg("preci"),
            "Projects p3d onto the surface, returning its parametric uv.\n\n"
            "Robust at boundaries/singularities where the standard\n"
            "GeomAPI_ProjectPointOnSurface tool tends to fail.")

        .def("next_value_of_uv", &ShapeAnalysis_Surface::NextValueOfUV,
            py::arg("p2d_prev"), py::arg("p3d"), py::arg("preci"),
            py::arg("maxpreci") = -1.0,
            "Projects p3d onto the surface using p2d_prev as a seed.\n\n"
            "Faster than value_of_uv when called repeatedly along a curve;\n"
            "falls back to value_of_uv on failure or when the result is\n"
            "farther than maxpreci from p3d (if maxpreci > 0).")

        .def("uv_from_iso",
            [](ShapeAnalysis_Surface& self, const gp_Pnt& p3d, Standard_Real preci,
               Standard_Real u, Standard_Real v) {
                Standard_Real refinedU = u;
                Standard_Real refinedV = v;
                Standard_Real dist = self.UVFromIso(p3d, preci, refinedU, refinedV);
                return py::make_tuple(dist, refinedU, refinedV);
            },
            py::arg("p3d"), py::arg("preci"), py::arg("u"), py::arg("v"),
            "Returns tuple (distance, refined_u, refined_v) by refining (u, v)\n"
            "through projections onto surface iso-lines. distance is the best\n"
            "achieved |p3d - Value(refined_u, refined_v)|, or a very large value\n"
            "if no refinement succeeded.")

        // Bounding boxes around each boundary iso
        .def_property_readonly("box_u_first", &ShapeAnalysis_Surface::GetBoxUF,
            py::return_value_policy::reference_internal,
            "Bounding box of the U=u_first iso-curve.")

        .def_property_readonly("box_u_last", &ShapeAnalysis_Surface::GetBoxUL,
            py::return_value_policy::reference_internal,
            "Bounding box of the U=u_last iso-curve.")

        .def_property_readonly("box_v_first", &ShapeAnalysis_Surface::GetBoxVF,
            py::return_value_policy::reference_internal,
            "Bounding box of the V=v_first iso-curve.")

        .def_property_readonly("box_v_last", &ShapeAnalysis_Surface::GetBoxVL,
            py::return_value_policy::reference_internal,
            "Bounding box of the V=v_last iso-curve.")
    ;

    // =========================================================================
    // ShapeAnalysis_Wire - Wire analyzer (order, connectivity, gaps,
    //                                      self-intersection, lacking edges,
    //                                      closure)
    // =========================================================================
    py::class_<ShapeAnalysis_Wire, opencascade::handle<ShapeAnalysis_Wire>>(m, "Wire",
        R"doc(
        Wire analyzer.

        Inspects a wire-on-face for the issues that ShapeFix_Wire repairs:
        edge ordering, gaps in 3D and 2D, lacking edges (gap in 2D while
        connected in 3D), self-intersection, degenerated edges, closure,
        small edges. Each Check<X>() method runs the analysis and stores
        a status flag queryable via Status<X>(status).

        Two usage shapes:
        - Quick one-shot: perform() runs the standard battery in a fixed
          order and returns True if anything was flagged.
        - Step-by-step: call individual Check<X>() methods, then inspect
          Status<X>(ShapeExtendStatus.DoneN) for the detailed verdict.

        Distance metrics (min_distance_3d, max_distance_3d, ...) are
        populated by the most recent Check<X>() that records them.

        Example:
            saw = ShapeAnalysis.Wire(wire, face, 1e-6)
            if saw.check_connected():
                if saw.status_connected(ShapeAnalysis.ShapeExtendStatus.Done1):
                    ...  # edges are confused with starting precision
        )doc")

        .def(py::init<>(),
            "Creates an empty wire analyzer.")

        .def(py::init<const TopoDS_Wire&, const TopoDS_Face&, Standard_Real>(),
            py::arg("wire"), py::arg("face"), py::arg("precision"),
            "Creates a wire analyzer loaded with wire/face/precision.")

        .def("init",
            py::overload_cast<const TopoDS_Wire&, const TopoDS_Face&, Standard_Real>(
                &ShapeAnalysis_Wire::Init),
            py::arg("wire"), py::arg("face"), py::arg("precision"),
            "Loads wire/face/precision, resetting cached statuses.")

        .def("load",
            py::overload_cast<const TopoDS_Wire&>(&ShapeAnalysis_Wire::Load),
            py::arg("wire"),
            "Loads the wire, resetting cached statuses.")

        .def("set_face",
            py::overload_cast<const TopoDS_Face&>(&ShapeAnalysis_Wire::SetFace),
            py::arg("face"),
            "Sets the working face for the wire.")

        .def("set_face_with_surface",
            py::overload_cast<const TopoDS_Face&, const opencascade::handle<ShapeAnalysis_Surface>&>(
                &ShapeAnalysis_Wire::SetFace),
            py::arg("face"), py::arg("surface_analysis"),
            "Sets the working face plus a pre-built ShapeAnalysis.Surface.\n"
            "Useful when the same surface is reused for many wires.")

        .def("set_surface_analysis",
            py::overload_cast<const opencascade::handle<ShapeAnalysis_Surface>&>(
                &ShapeAnalysis_Wire::SetSurface),
            py::arg("surface_analysis"),
            "Sets the working surface from a pre-built ShapeAnalysis.Surface.")

        .def("set_surface",
            py::overload_cast<const opencascade::handle<Geom_Surface>&>(
                &ShapeAnalysis_Wire::SetSurface),
            py::arg("surface"),
            "Sets the working surface from a raw Geom_Surface.")

        .def("set_surface",
            py::overload_cast<const opencascade::handle<Geom_Surface>&, const TopLoc_Location&>(
                &ShapeAnalysis_Wire::SetSurface),
            py::arg("surface"), py::arg("location"),
            "Sets the working surface from a Geom_Surface + location.")

        .def("set_precision", &ShapeAnalysis_Wire::SetPrecision,
            py::arg("precision"),
            "Sets the working precision.")

        .def("clear_statuses", &ShapeAnalysis_Wire::ClearStatuses,
            "Clears every cached status (wire/face/precision are kept).")

        // State
        .def_property_readonly("is_loaded", &ShapeAnalysis_Wire::IsLoaded,
            "True when the wire has at least one edge.")

        .def_property_readonly("is_ready", &ShapeAnalysis_Wire::IsReady,
            "True when both wire and face are loaded.")

        .def_property_readonly("precision", &ShapeAnalysis_Wire::Precision,
            "Current working precision.")

        .def_property_readonly("nb_edges", &ShapeAnalysis_Wire::NbEdges,
            "Number of edges in the working wire (0 if not loaded).")

        .def_property_readonly("face", &ShapeAnalysis_Wire::Face,
            py::return_value_policy::reference_internal,
            "Working face.")

        .def_property_readonly("surface", &ShapeAnalysis_Wire::Surface,
            "Working ShapeAnalysis.Surface (built lazily from face/surface).")

        // High-level checks
        .def("perform", [](ShapeAnalysis_Wire& self) { return self.Perform(); },
            "Runs the standard analysis battery in OCCT's default order:\n"
            "CheckOrder, CheckSmall, CheckConnected, CheckEdgeCurves,\n"
            "CheckDegenerated, CheckSelfIntersection, CheckLacking,\n"
            "CheckClosed.\n\n"
            "Returns True if at least one check flagged an issue.")

        .def("check_order",
            py::overload_cast<const Standard_Boolean, const Standard_Boolean>(
                &ShapeAnalysis_Wire::CheckOrder),
            py::arg("is_closed") = true, py::arg("mode_3d") = true,
            "Checks tail-to-head ordering. Returns False if already ordered.")

        .def("check_connected",
            py::overload_cast<const Standard_Real>(&ShapeAnalysis_Wire::CheckConnected),
            py::arg("prec") = 0.0,
            "Checks vertex sharing between adjacent edges across the wire.")

        .def("check_connected_at",
            py::overload_cast<const Standard_Integer, const Standard_Real>(
                &ShapeAnalysis_Wire::CheckConnected),
            py::arg("num"), py::arg("prec") = 0.0,
            "Checks edges num-1 and num for shared vertex / closeness.")

        .def("check_small",
            py::overload_cast<const Standard_Real>(&ShapeAnalysis_Wire::CheckSmall),
            py::arg("prec_small") = 0.0,
            "Checks for edges shorter than min(precision, prec_small).")

        .def("check_small_at",
            py::overload_cast<const Standard_Integer, const Standard_Real>(
                &ShapeAnalysis_Wire::CheckSmall),
            py::arg("num"), py::arg("prec_small") = 0.0,
            "Checks if the num-th edge is small.")

        .def("check_edge_curves", &ShapeAnalysis_Wire::CheckEdgeCurves,
            "Runs all per-edge curve consistency checks (3D vs pcurve,\n"
            "vertices vs pcurve, vertices vs 3D, seam, gaps, same parameter).")

        .def("check_degenerated",
            py::overload_cast<>(&ShapeAnalysis_Wire::CheckDegenerated),
            "Checks for badly-marked degenerated edges across the wire.")

        .def("check_degenerated_at",
            py::overload_cast<const Standard_Integer>(&ShapeAnalysis_Wire::CheckDegenerated),
            py::arg("num"),
            "Checks for a degenerated edge between edges num-1 and num.")

        .def("check_degenerated_values",
            [](ShapeAnalysis_Wire& self, Standard_Integer num) {
                gp_Pnt2d dgnr1, dgnr2;
                Standard_Boolean flagged = self.CheckDegenerated(num, dgnr1, dgnr2);
                return py::make_tuple(flagged, dgnr1, dgnr2);
            },
            py::arg("num"),
            "Returns tuple (flagged, dgnr1_uv, dgnr2_uv) — the two 2D points\n"
            "in parametric space that span the singularity between edges\n"
            "num-1 and num.")

        .def("check_closed", &ShapeAnalysis_Wire::CheckClosed,
            py::arg("prec") = 0.0,
            "Checks closure between the last and first edges.")

        .def("check_self_intersection", &ShapeAnalysis_Wire::CheckSelfIntersection,
            "Looks for self-intersecting edges and intersecting adjacent\n"
            "edges (does not check every pair, only adjacent ones).")

        .def("check_self_intersecting_edge",
            py::overload_cast<const Standard_Integer>(&ShapeAnalysis_Wire::CheckSelfIntersectingEdge),
            py::arg("num"),
            "Checks if the num-th edge is self-intersecting.")

        .def("check_intersecting_edges",
            py::overload_cast<const Standard_Integer>(&ShapeAnalysis_Wire::CheckIntersectingEdges),
            py::arg("num"),
            "Checks edges num-1 and num for intersection outside the common\n"
            "vertex.")

        .def("check_intersecting_edges_pair",
            py::overload_cast<const Standard_Integer, const Standard_Integer>(
                &ShapeAnalysis_Wire::CheckIntersectingEdges),
            py::arg("num1"), py::arg("num2"),
            "Checks edges num1 and num2 for intersection.")

        .def("check_lacking",
            py::overload_cast<>(&ShapeAnalysis_Wire::CheckLacking),
            "Checks for lacking edges (2D gap while connected in 3D).")

        .def("check_lacking_at",
            py::overload_cast<const Standard_Integer, const Standard_Real>(
                &ShapeAnalysis_Wire::CheckLacking),
            py::arg("num"), py::arg("tolerance") = 0.0,
            "Checks if a lacking edge is needed between edges num-1 and num.")

        .def("check_lacking_values",
            [](ShapeAnalysis_Wire& self, Standard_Integer num, Standard_Real tol) {
                gp_Pnt2d p2d1, p2d2;
                Standard_Boolean flagged = self.CheckLacking(num, tol, p2d1, p2d2);
                return py::make_tuple(flagged, p2d1, p2d2);
            },
            py::arg("num"), py::arg("tolerance"),
            "Returns tuple (flagged, p2d1, p2d2) — the 2D endpoints of the\n"
            "gap detected between edges num-1 and num.")

        .def("check_gaps_3d", &ShapeAnalysis_Wire::CheckGaps3d,
            "Checks 3D-curve gaps between adjacent edges across the wire.")

        .def("check_gap_3d",
            py::overload_cast<const Standard_Integer>(&ShapeAnalysis_Wire::CheckGap3d),
            py::arg("num") = 0,
            "Checks 3D-curve gap between edges num-1 and num\n"
            "(num=0 means check the whole wire).")

        .def("check_gaps_2d", &ShapeAnalysis_Wire::CheckGaps2d,
            "Checks pcurve gaps between adjacent edges across the wire.")

        .def("check_gap_2d",
            py::overload_cast<const Standard_Integer>(&ShapeAnalysis_Wire::CheckGap2d),
            py::arg("num") = 0,
            "Checks pcurve gap between edges num-1 and num.")

        .def("check_curve_gaps", &ShapeAnalysis_Wire::CheckCurveGaps,
            "Checks the gap between 3D curve and the curve generated by the\n"
            "pcurve on the surface, summed over the wire.")

        .def("check_curve_gap",
            py::overload_cast<const Standard_Integer>(&ShapeAnalysis_Wire::CheckCurveGap),
            py::arg("num") = 0,
            "Checks the 3D-vs-pcurve gap for the num-th edge.")

        .def("check_outer_bound", &ShapeAnalysis_Wire::CheckOuterBound,
            py::arg("api_make") = true,
            "Returns True if the wire bounds an outer region of the face.\n"
            "api_make=True uses BRepAPI_MakeWire (handles unshared vertices),\n"
            "False uses BRep_Builder.")

        .def("check_small_area", &ShapeAnalysis_Wire::CheckSmallArea,
            py::arg("wire"),
            "Returns True if the wire's parametric area is below precision.")

        .def("check_shape_connect",
            py::overload_cast<const TopoDS_Shape&, const Standard_Real>(
                &ShapeAnalysis_Wire::CheckShapeConnect),
            py::arg("shape"), py::arg("prec") = 0.0,
            "Checks how the given edge/wire can be connected to this wire.\n"
            "Use last_check_status(...) on Done1..Done6 to interpret\n"
            "(see OCCT documentation for the table).")

        .def("check_shape_connect_distances",
            [](ShapeAnalysis_Wire& self, const TopoDS_Shape& shape, Standard_Real prec) {
                Standard_Real tailhead, tailtail, headtail, headhead;
                Standard_Boolean flagged = self.CheckShapeConnect(
                    tailhead, tailtail, headtail, headhead, shape, prec);
                return py::make_tuple(flagged, tailhead, tailtail, headtail, headhead);
            },
            py::arg("shape"), py::arg("prec") = 0.0,
            "Returns tuple (flagged, tail_head, tail_tail, head_tail, head_head)\n"
            "with the four end-to-end distances between this wire and the\n"
            "given shape (edge or wire).")

        .def("check_notched_edges",
            [](ShapeAnalysis_Wire& self, Standard_Integer num, Standard_Real tol) {
                Standard_Integer shortNum = 0;
                Standard_Real param = 0.0;
                Standard_Boolean flagged = self.CheckNotchedEdges(num, shortNum, param, tol);
                return py::make_tuple(flagged, shortNum, param);
            },
            py::arg("num"), py::arg("tolerance") = 0.0,
            "Returns tuple (flagged, short_edge_num, parameter) when a notch\n"
            "is detected between edges num-1 and num.")

        // Status accessors
        .def("status_order",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusOrder(s); },
            py::arg("status"),
            "Returns True if the last check_order recorded the given status.")

        .def("status_connected",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusConnected(s); },
            py::arg("status"),
            "Returns True if the last check_connected recorded the given status.")

        .def("status_edge_curves",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusEdgeCurves(s); },
            py::arg("status"),
            "Returns True if the last check_edge_curves recorded the given status.")

        .def("status_degenerated",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusDegenerated(s); },
            py::arg("status"),
            "Returns True if the last check_degenerated recorded the given status.")

        .def("status_closed",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusClosed(s); },
            py::arg("status"),
            "Returns True if the last check_closed recorded the given status.")

        .def("status_small",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusSmall(s); },
            py::arg("status"),
            "Returns True if the last check_small recorded the given status.")

        .def("status_self_intersection",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusSelfIntersection(s); },
            py::arg("status"),
            "Returns True if the last check_self_intersection recorded the given status.")

        .def("status_lacking",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusLacking(s); },
            py::arg("status"),
            "Returns True if the last check_lacking recorded the given status.")

        .def("status_gaps_3d",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusGaps3d(s); },
            py::arg("status"),
            "Returns True if the last check_gaps_3d recorded the given status.")

        .def("status_gaps_2d",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusGaps2d(s); },
            py::arg("status"),
            "Returns True if the last check_gaps_2d recorded the given status.")

        .def("status_curve_gaps",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusCurveGaps(s); },
            py::arg("status"),
            "Returns True if the last check_curve_gaps recorded the given status.")

        .def("status_loop",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.StatusLoop(s); },
            py::arg("status"),
            "Returns True if the last loop check recorded the given status.")

        .def("last_check_status",
            [](const ShapeAnalysis_Wire& self, ShapeExtend_Status s) { return self.LastCheckStatus(s); },
            py::arg("status"),
            "Returns True if the most recent advanced check (e.g.\n"
            "check_shape_connect) recorded the given status.")

        // Distance metrics
        .def_property_readonly("min_distance_3d", &ShapeAnalysis_Wire::MinDistance3d,
            "Lowest 3D distance recorded by the last distance-aware check.")

        .def_property_readonly("min_distance_2d", &ShapeAnalysis_Wire::MinDistance2d,
            "Lowest 2D-UV distance recorded by the last distance-aware check.")

        .def_property_readonly("max_distance_3d", &ShapeAnalysis_Wire::MaxDistance3d,
            "Highest 3D distance recorded by the last distance-aware check.")

        .def_property_readonly("max_distance_2d", &ShapeAnalysis_Wire::MaxDistance2d,
            "Highest 2D-UV distance recorded by the last distance-aware check.")
    ;
}