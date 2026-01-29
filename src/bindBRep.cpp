#include <pybind11/pybind11.h>

#include <BRep_Tool.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopLoc_Location.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Curve.hxx>
#include <Geom2d_Curve.hxx>
#include <Poly_Triangulation.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <GeomAbs_Shape.hxx>

namespace py = pybind11;

// Declare opencascade::handle as a holder type
#include <Standard_Handle.hxx>
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_brep(py::module_ &m)
{
    py::class_<BRep_Tool>(m, "Tool",
        "Provides class methods to access the geometry of BRep shapes.\n\n"
        "This is a utility class with static methods for extracting geometric\n"
        "information from topological shapes (faces, edges, vertices).")
        
        // General methods
        .def_static("is_closed", 
            py::overload_cast<const TopoDS_Shape&>(&BRep_Tool::IsClosed),
            py::arg("shape"),
            "Returns True if the shape is closed.\n\n"
            "- For Shell: True if it has no free boundaries (edges)\n"
            "- For Wire: True if it has no free ends (vertices)\n"
            "- For Edge: True if its vertices are the same\n"
            "- For other types: returns shape.Closed()")
        
        // Face methods
        .def_static("surface", 
            py::overload_cast<const TopoDS_Face&>(&BRep_Tool::Surface),
            py::arg("face"),
            "Returns the geometric surface of the face.\n"
            "May be a copy if there is a location")
        
        .def_static("surface_with_location",
            [](const TopoDS_Face& f) {
                TopLoc_Location loc;
                auto surf = BRep_Tool::Surface(f, loc);
                return py::make_tuple(surf, loc);
            },
            py::arg("face"),
            "Returns tuple (surface, location) for the face")
        
        .def_static("triangulation",
            [](const TopoDS_Face& f) {
                TopLoc_Location loc;
                auto tri = BRep_Tool::Triangulation(f, loc);
                return py::make_tuple(tri, loc);
            },
            py::arg("face"),
            "Returns tuple (triangulation, location) for the face")
        
        .def_static("tolerance", 
            py::overload_cast<const TopoDS_Face&>(&BRep_Tool::Tolerance),
            py::arg("face"),
            "Returns the tolerance of the face")
        
        .def_static("natural_restriction", &BRep_Tool::NaturalRestriction,
            py::arg("face"),
            "Returns the NaturalRestriction flag of the face")
        
        .def_static("is_geometric", 
            py::overload_cast<const TopoDS_Face&>(&BRep_Tool::IsGeometric),
            py::arg("face"),
            "Returns True if the face has a surface")
        
        // Edge methods - 3D curve
        .def_static("curve",
            [](const TopoDS_Edge& e) {
                Standard_Real first, last;
                auto curve = BRep_Tool::Curve(e, first, last);
                return py::make_tuple(curve, first, last);
            },
            py::arg("edge"),
            "Returns tuple (curve, first, last) for the edge's 3D curve")
        
        .def_static("curve_with_location",
            [](const TopoDS_Edge& e) {
                TopLoc_Location loc;
                Standard_Real first, last;
                auto curve = BRep_Tool::Curve(e, loc, first, last);
                return py::make_tuple(curve, loc, first, last);
            },
            py::arg("edge"),
            "Returns tuple (curve, location, first, last) for the edge's 3D curve")
        
        .def_static("is_geometric_edge", 
            py::overload_cast<const TopoDS_Edge&>(&BRep_Tool::IsGeometric),
            py::arg("edge"),
            "Returns True if edge is a 3D curve or curve on surface")
        
        // Edge methods - 2D curve on surface
        .def_static("curve_on_surface",
            [](const TopoDS_Edge& e, const TopoDS_Face& f) {
                Standard_Real first, last;
                auto curve = BRep_Tool::CurveOnSurface(e, f, first, last);
                return py::make_tuple(curve, first, last);
            },
            py::arg("edge"), py::arg("face"),
            "Returns tuple (2d_curve, first, last) for the edge on the face")
        
        // Edge properties
        .def_static("tolerance_edge", 
            py::overload_cast<const TopoDS_Edge&>(&BRep_Tool::Tolerance),
            py::arg("edge"),
            "Returns the tolerance of the edge")
        
        .def_static("same_parameter", &BRep_Tool::SameParameter,
            py::arg("edge"),
            "Returns the SameParameter flag for the edge")
        
        .def_static("same_range", &BRep_Tool::SameRange,
            py::arg("edge"),
            "Returns the SameRange flag for the edge")
        
        .def_static("degenerated", &BRep_Tool::Degenerated,
            py::arg("edge"),
            "Returns True if the edge is degenerated")
        
        .def_static("range",
            py::overload_cast<const TopoDS_Edge&, Standard_Real&, Standard_Real&>(&BRep_Tool::Range),
            py::arg("edge"), py::arg("first"), py::arg("last"),
            "Gets the range of the 3D curve (outputs first and last parameters)")
        
        .def_static("range_on_face",
            [](const TopoDS_Edge& e, const TopoDS_Face& f) {
                Standard_Real first, last;
                BRep_Tool::Range(e, f, first, last);
                return py::make_tuple(first, last);
            },
            py::arg("edge"), py::arg("face"),
            "Returns tuple (first, last) for the edge's range on the face")
        
        .def_static("uv_points",
            [](const TopoDS_Edge& e, const TopoDS_Face& f) {
                gp_Pnt2d p_first, p_last;
                BRep_Tool::UVPoints(e, f, p_first, p_last);
                return py::make_tuple(p_first, p_last);
            },
            py::arg("edge"), py::arg("face"),
            "Returns tuple (uv_first, uv_last) for the edge extremities on the face")
        
        .def_static("set_uv_points",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Face&, const gp_Pnt2d&, const gp_Pnt2d&>(&BRep_Tool::SetUVPoints),
            py::arg("edge"), py::arg("face"), py::arg("p_first"), py::arg("p_last"),
            "Sets the UV locations of the edge extremities on the face")
        
        // Edge continuity
        .def_static("has_continuity",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Face&, const TopoDS_Face&>(&BRep_Tool::HasContinuity),
            py::arg("edge"), py::arg("face1"), py::arg("face2"),
            "Returns True if the edge is on the surfaces of the two faces")
        
        .def_static("continuity",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Face&, const TopoDS_Face&>(&BRep_Tool::Continuity),
            py::arg("edge"), py::arg("face1"), py::arg("face2"),
            "Returns the continuity of the edge between two faces")
        
        .def_static("has_continuity_edge",
            py::overload_cast<const TopoDS_Edge&>(&BRep_Tool::HasContinuity),
            py::arg("edge"),
            "Returns True if the edge has regularity on some two surfaces")
        
        .def_static("max_continuity", &BRep_Tool::MaxContinuity,
            py::arg("edge"),
            "Returns the max continuity of edge between surfaces or GeomAbs_C0")
        
        .def_static("is_closed_edge_on_face",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Face&>(&BRep_Tool::IsClosed),
            py::arg("edge"), py::arg("face"),
            "Returns True if edge has two PCurves on the face (closed surface)")
        
        // Vertex methods
        .def_static("pnt", &BRep_Tool::Pnt,
            py::arg("vertex"),
            "Returns the 3D point of the vertex")
        
        .def_static("tolerance_vertex", 
            py::overload_cast<const TopoDS_Vertex&>(&BRep_Tool::Tolerance),
            py::arg("vertex"),
            "Returns the tolerance of the vertex")
        
        .def_static("parameter",
            [](const TopoDS_Vertex& v, const TopoDS_Edge& e) {
                Standard_Real param;
                Standard_Boolean found = BRep_Tool::Parameter(v, e, param);
                if (found) {
                    return py::make_tuple(true, param);
                }
                return py::make_tuple(false, 0.0);
            },
            py::arg("vertex"), py::arg("edge"),
            "Returns tuple (found, parameter) for the vertex on the edge")
        
        .def_static("parameter_on_edge",
            py::overload_cast<const TopoDS_Vertex&, const TopoDS_Edge&>(&BRep_Tool::Parameter),
            py::arg("vertex"), py::arg("edge"),
            "Returns the parameter of the vertex on the edge.\n"
            "Raises exception if no parameter exists")
        
        .def_static("parameter_on_face",
            py::overload_cast<const TopoDS_Vertex&, const TopoDS_Edge&, const TopoDS_Face&>(&BRep_Tool::Parameter),
            py::arg("vertex"), py::arg("edge"), py::arg("face"),
            "Returns the parameter of the vertex on the edge's pcurve on the face")
        
        .def_static("parameters", &BRep_Tool::Parameters,
            py::arg("vertex"), py::arg("face"),
            "Returns the UV parameters of the vertex on the face")
        
        // General utility
        .def_static("max_tolerance", &BRep_Tool::MaxTolerance,
            py::arg("shape"), py::arg("sub_shape"),
            "Returns the maximum tolerance of shape's subshapes.\n\n"
            "Parameters:\n"
            "  shape: Shape to search tolerance in\n"
            "  sub_shape: Type of subshape (Face, Edge, or Vertex)")
    ;
}