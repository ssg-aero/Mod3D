#include <pybind11/pybind11.h>
#include <TopoDS_Shape.hxx>
#include <Standard_Handle.hxx>
#include <BRepTools.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <BRepTools_Quilt.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopTools_FormatVersion.hxx>
#include <BRep_Tool.hxx>

namespace py = pybind11;
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_brep_tools(py::module_ &m)
{
    py::class_<BRepTools_ReShape, opencascade::handle<BRepTools_ReShape>>(m, "ReShape",
        "Rebuilds a shape by making pre-defined substitutions on some of its components.\n\n"
        "In a first phase, it records requests to replace or remove some individual shapes.\n"
        "For each shape, the last given request is recorded.\n\n"
        "Then, these requests may be applied to any shape which may contain one or more\n"
        "of these individual shapes.\n\n"
        "Typical usage:\n"
        "  reshape = BRepTools.ReShape()\n"
        "  reshape.replace(old_edge, new_edge)\n"
        "  reshape.remove(face_to_remove)\n"
        "  result = reshape.apply(original_shape)")
        
        // Constructor
        .def(py::init<>(),
            "Creates an empty ReShape object")
        
        // Clear
        .def("clear", &BRepTools_ReShape::Clear,
            "Clears all substitution requests")
        
        // Replace and Remove
        .def("replace", &BRepTools_ReShape::Replace,
            py::arg("shape"), py::arg("new_shape"),
            "Sets a request to replace a shape by a new one.\n\n"
            "The orientation of the replacing shape respects that of the original one")
        
        .def("remove", &BRepTools_ReShape::Remove,
            py::arg("shape"),
            "Sets a request to remove a shape whatever the orientation")
        
        // Query
        .def("is_recorded", &BRepTools_ReShape::IsRecorded,
            py::arg("shape"),
            "Tells if a shape is recorded for Replace/Remove")
        
        .def("value", &BRepTools_ReShape::Value,
            py::arg("shape"),
            "Returns the new value for an individual shape.\n\n"
            "If not recorded, returns the original shape itself.\n"
            "If to be removed, returns a null shape.\n"
            "Else, returns the replacing item")
        
        .def("status", 
            [](BRepTools_ReShape& self, const TopoDS_Shape& shape, const Standard_Boolean last) {
                TopoDS_Shape newsh;
                Standard_Integer status = self.Status(shape, newsh, last);
                return py::make_tuple(status, newsh);
            },
            py::arg("shape"), py::arg("last") = false,
            "Returns a complete substitution status for a shape.\n\n"
            "Returns: (status, new_shape) where:\n"
            "  status = 0:  not recorded, new_shape = original shape\n"
            "  status < 0:  to be removed, new_shape is null\n"
            "  status > 0:  to be replaced, new_shape is the replacement\n\n"
            "Parameters:\n"
            "  last: If False, returns status recorded directly for the shape.\n"
            "        If True and status > 0, recursively searches for last status")
        
        // Apply
        .def("apply", &BRepTools_ReShape::Apply,
            py::arg("shape"), py::arg("until") = TopAbs_SHAPE,
            "Applies the substitution requests to a shape.\n\n"
            "The 'until' parameter gives the level of type until which requests\n"
            "are taken into account. For subshapes of the type 'until', no rebuild\n"
            "and further exploring are done.\n\n"
            "Note: Each subshape can be replaced by a shape of the same type or by\n"
            "a shape containing only shapes of that type. If incompatible shape type\n"
            "is encountered, it is ignored and flag FAIL1 is set in status")
        
        // Properties
        .def_property("mode_consider_location",
            [](const BRepTools_ReShape& self) -> Standard_Boolean {
                return const_cast<BRepTools_ReShape&>(self).ModeConsiderLocation();
            },
            [](BRepTools_ReShape& self, Standard_Boolean value) {
                self.ModeConsiderLocation() = value;
            },
            "Gets/sets whether location of shape is taken into account during replacing shapes")
        
        // Vertex operations
        .def("copy_vertex",
            py::overload_cast<const TopoDS_Vertex&, const Standard_Real>(
                &BRepTools_ReShape::CopyVertex),
            py::arg("vertex"), py::arg("tolerance") = -1.0,
            "Returns modified copy of vertex if original one is not recorded,\n"
            "or returns modified original vertex otherwise.\n\n"
            "Parameters:\n"
            "  tolerance: New tolerance of vertex (optional, -1.0 keeps original)")
        
        .def("copy_vertex",
            py::overload_cast<const TopoDS_Vertex&, const gp_Pnt&, const Standard_Real>(
                &BRepTools_ReShape::CopyVertex),
            py::arg("vertex"), py::arg("new_position"), py::arg("tolerance"),
            "Returns modified copy of vertex if original one is not recorded,\n"
            "or returns modified original vertex otherwise.\n\n"
            "Parameters:\n"
            "  new_position: New position for vertex copy\n"
            "  tolerance: New tolerance of vertex")
        
        // Shape status
        .def("is_new_shape", &BRepTools_ReShape::IsNewShape,
            py::arg("shape"),
            "Checks if shape has been recorded by reshaper as a value")
        
        // History
        .def_property_readonly("history", &BRepTools_ReShape::History,
            "Returns the history of the substituted shapes")
    ;

    // =========================================================================
    // BRepTools_WireExplorer - Explore edges of a wire in connection order
    // =========================================================================
    py::class_<BRepTools_WireExplorer>(m, "WireExplorer",
        R"doc(
        Explores the edges of a wire in connection order.

        Each edge is connected to the previous one by its origin vertex.
        This is useful for traversing wire edges in their natural sequence.

        If the wire is not closed, returns only a segment of edges depending
        on the starting edge.

        Note: The algorithm assumes the wire is valid. Defects like loops,
        wrong edge orientations, or branching may cause incomplete traversal.

        Example:
            wire = ...  # some TopoDS_Wire
            explorer = BRepTools.WireExplorer(wire)
            while explorer.more:
                edge = explorer.current
                vertex = explorer.current_vertex
                # process edge...
                explorer.next()

        Or using iteration:
            for edge in BRepTools.WireExplorer(wire):
                # process edge...
        )doc")
        
        .def(py::init<>(),
            "Creates an empty explorer. Use init() to set the wire.")
        
        .def(py::init<const TopoDS_Wire&>(),
            py::arg("wire"),
            "Creates an explorer for the given wire.")
        
        .def(py::init<const TopoDS_Wire&, const TopoDS_Face&>(),
            py::arg("wire"), py::arg("face"),
            "Creates an explorer for the wire on a face.\n\n"
            "The face is used to select the edge connected to the previous\n"
            "in the parametric representation of the face.")
        
        .def("init", py::overload_cast<const TopoDS_Wire&>(
            &BRepTools_WireExplorer::Init),
            py::arg("wire"),
            "Initializes exploration of the wire.")
        
        .def("init", py::overload_cast<const TopoDS_Wire&, const TopoDS_Face&>(
            &BRepTools_WireExplorer::Init),
            py::arg("wire"), py::arg("face"),
            "Initializes exploration of the wire on a face.")
        
        .def("init", py::overload_cast<const TopoDS_Wire&, const TopoDS_Face&,
                                       Standard_Real, Standard_Real,
                                       Standard_Real, Standard_Real>(
            &BRepTools_WireExplorer::Init),
            py::arg("wire"), py::arg("face"),
            py::arg("u_min"), py::arg("u_max"),
            py::arg("v_min"), py::arg("v_max"),
            "Initializes exploration with explicit UV bounds of the face.")
        
        .def_property_readonly("more", &BRepTools_WireExplorer::More,
            "Returns True if there is a current edge to explore.")
        
        .def("next", &BRepTools_WireExplorer::Next,
            "Proceeds to the next edge in the wire.")
        
        .def_property_readonly("current", &BRepTools_WireExplorer::Current,
            py::return_value_policy::reference_internal,
            "Returns the current edge being explored.")
        
        .def_property_readonly("orientation", &BRepTools_WireExplorer::Orientation,
            "Returns the orientation of the current edge.")
        
        .def_property_readonly("current_vertex", &BRepTools_WireExplorer::CurrentVertex,
            py::return_value_policy::reference_internal,
            "Returns the vertex connecting the current edge to the previous one.")
        
        .def("clear", &BRepTools_WireExplorer::Clear,
            "Clears the content of the explorer.")
        
        // Make it iterable
        .def("__iter__", [](BRepTools_WireExplorer& self) -> BRepTools_WireExplorer& {
            return self;
        }, py::return_value_policy::reference_internal)
        
        .def("__next__", [](BRepTools_WireExplorer& self) {
            if (!self.More()) {
                throw py::stop_iteration();
            }
            TopoDS_Edge edge = self.Current();
            self.Next();
            return edge;
        }, "Returns the next edge in the wire.")
    ;

    // =========================================================================
    // BRepTools_Quilt - Glue faces at common edges and reconstruct shells
    // =========================================================================
    py::class_<BRepTools_Quilt>(m, "Quilt",
        R"doc(
        A tool to glue faces at common edges and reconstruct shells.

        The user designates pairs of common edges using the bind() method.
        One edge is designated as the edge to use in place of the other one
        (they are supposed to be geometrically confused, but this is not checked).
        They can be of opposite directions, specified by the orientations.

        The user can add shapes with the add() method. All faces are registered
        and copies of faces and edges are made to glue at the bound edges.

        The user can call shells() to compute a compound of shells from the
        current set of faces.

        If no binding is made, this class can be used to make shells from faces
        already sharing their edges.

        Example:
            quilt = BRepTools.Quilt()
            quilt.bind(edge_old, edge_new)  # bind edges to be merged
            quilt.add(face1)
            quilt.add(face2)
            result = quilt.shells()  # get the resulting shell compound
        )doc")
        
        .def(py::init<>(),
            "Creates an empty Quilt object.")
        
        .def("bind", py::overload_cast<const TopoDS_Edge&, const TopoDS_Edge&>(
            &BRepTools_Quilt::Bind),
            py::arg("edge_old"), py::arg("edge_new"),
            "Binds edge_new to be the new edge instead of edge_old.\n\n"
            "The faces of the added shape containing edge_old will be copied\n"
            "to substitute edge_old by edge_new.\n\n"
            "The vertices of edge_old will be bound to the vertices of edge_new\n"
            "with the same orientation.\n\n"
            "If edge_old and edge_new have different orientations, the curves\n"
            "are considered opposite and the pcurves of edge_old will be copied\n"
            "and reversed in the new faces.\n\n"
            "Note: edge_old must belong to the next added shape, edge_new must\n"
            "belong to a shape added before.")
        
        .def("bind", py::overload_cast<const TopoDS_Vertex&, const TopoDS_Vertex&>(
            &BRepTools_Quilt::Bind),
            py::arg("vertex_old"), py::arg("vertex_new"),
            "Binds vertex_new to be a new vertex instead of vertex_old.\n\n"
            "The faces of the added shape containing vertex_old will be copied\n"
            "to substitute vertex_old by vertex_new.")
        
        .def("add", &BRepTools_Quilt::Add,
            py::arg("shape"),
            "Adds the faces of shape to the Quilt.\n\n"
            "Faces containing bound edges are copied.")
        
        .def("is_copied", &BRepTools_Quilt::IsCopied,
            py::arg("shape"),
            "Returns True if shape has been copied.\n\n"
            "Shape can be a vertex, an edge, or a face.")
        
        .def("copy", &BRepTools_Quilt::Copy,
            py::arg("shape"),
            py::return_value_policy::reference_internal,
            "Returns the shape substituted to the given shape in the Quilt.")
        
        .def("shells", &BRepTools_Quilt::Shells,
            "Returns a compound of shells made from the current set of faces.\n\n"
            "The shells will be flagged as closed or not closed.")
    ;

    // =========================================================================
    m.def("is_realy_closed", [](const TopoDS_Edge &E, const TopoDS_Face &F) {
            return BRepTools::IsReallyClosed(E, F);
        }, py::arg("edge"), py::arg("face"),
        "Checks if an edge is really closed on a face.\n\n"
        "Returns True if the edge is closed and the first and last vertices are\n"
    );

    m.def("outer_wire", [](const TopoDS_Face& face) {
            return BRepTools::OuterWire(face);
        }, py::arg("face"),
        "Returns the outer wire of a face.\n\n"
        "The outer wire is the one with the largest area among the wires of the face.\n"
        "If there are no wires, returns a null wire.");

    m.def("write", [](TopoDS_Shape& shape, const std::string& filename) {
            BRepTools::Write(shape, filename.c_str());
        }, py::arg("shape"), py::arg("filename"),
        "Writes a shape to a file in BRep format.\n\n"
        "The file is created if it does not exist, or overwritten if it exists.\n"
        "Raises StdFail_InfiniteLoop if the shape contains an infinite loop of\n" 
        "references (e.g., a face referencing itself).");

    m.def("write", [](TopoDS_Shape& shape, const std::string& filename, bool with_triangles, bool with_normals)
        {
            BRepTools::Write(shape, filename.c_str(), with_triangles, with_normals, TopTools_FormatVersion_CURRENT);
        }, py::arg("shape"), py::arg("filename"), py::arg("with_triangles") = false, py::arg("with_normals") = false,
        "Writes a shape to a file in BRep format with options for triangles and normals.\n\n"
        "Parameters:\n"
        "  shape: The shape to write\n"
        "  filename: The name of the file to write to\n"
        "  with_triangles: If True, includes triangulation data in the file (default: False)\n"
        "  with_normals: If True, includes normal vectors in the file (default: False)\n\n"
        "The file is created if it does not exist, or overwritten if it exists.\n"
        "Raises StdFail_InfiniteLoop if the shape contains an infinite loop of\n"
        "references (e.g., a face referencing itself).");


    m.def("is_closed", [](const TopoDS_Shape& shape) {
            return BRep_Tool::IsClosed(shape);
        }, py::arg("wire"),
        "Checks if a wire is really closed.\n\n"
        "Returns True if the wire is closed and the first and last vertices are\n"
    );
}