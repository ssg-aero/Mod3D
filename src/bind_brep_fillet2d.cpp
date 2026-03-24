#include <pybind11/pybind11.h>
#include <BRepFilletAPI_MakeFillet2d.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <ChFi2d_ConstructionError.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_SequenceOfShape.hxx>
#include <TopTools_ListOfShape.hxx>
#include <Message_ProgressRange.hxx>

namespace py = pybind11;

void bind_brep_fillet2d(py::module_ &m)
{

    py::class_<BRepFilletAPI_MakeFillet2d, BRepBuilderAPI_MakeShape>(m, "MakeFillet2d",
        "Constructs 2D fillets and chamfers on vertices of a planar face.\n\n"
        "This algorithm modifies a planar face by adding fillets (rounded corners)\n"
        "or chamfers (beveled corners) at specified vertices.\n\n"
        "Workflow:\n"
        "1. Initialize with a planar face using constructor or init()\n"
        "2. Add fillets or chamfers using add_fillet() or add_chamfer()\n"
        "3. Optionally modify or remove fillets/chamfers\n"
        "4. Build the result using build()\n"
        "5. Retrieve the modified face using shape()\n\n"
        "Limitations:\n"
        "- Only straight lines and circular arcs are supported\n"
        "- BSplines are not processed\n"
        "- Face must be planar")
        
        // Constructors
        .def(py::init<>(),
            "Creates an empty algorithm for computing 2D fillets and chamfers.\n\n"
            "The face must be defined using init() before adding fillets/chamfers")
        
        .def(py::init<const TopoDS_Face&>(),
            py::arg("face"),
            "Initializes the algorithm with a planar face.\n\n"
            "Parameters:\n"
            "  face: The planar face to modify\n\n"
            "Status after initialization (check with status property):\n"
            "- Ready: initialization successful\n"
            "- NotPlanar: face is not planar\n"
            "- NoFace: face is null")
        
        // Init methods
        .def("init",
            py::overload_cast<const TopoDS_Face&>(&BRepFilletAPI_MakeFillet2d::Init),
            py::arg("face"),
            "Initializes the algorithm with a planar face.\n\n"
            "Parameters:\n"
            "  face: The planar face to modify\n\n"
            "Status after initialization (check with status property):\n"
            "- Ready: initialization successful\n"
            "- NotPlanar: face is not planar\n"
            "- NoFace: face is null")
        
        .def("init",
            py::overload_cast<const TopoDS_Face&, const TopoDS_Face&>(&BRepFilletAPI_MakeFillet2d::Init),
            py::arg("ref_face"), py::arg("mod_face"),
            "Initializes from a reference face and a modified face.\n\n"
            "This is useful to modify a fillet or chamfer already created on mod_face.\n\n"
            "Parameters:\n"
            "  ref_face: Original reference face\n"
            "  mod_face: Modified face derived from ref_face")
        
        // Fillet operations
        .def("add_fillet", &BRepFilletAPI_MakeFillet2d::AddFillet,
            py::arg("vertex"), py::arg("radius"),
            "Adds a fillet of specified radius at a vertex.\n\n"
            "Creates a circular arc of radius 'radius' connecting the two edges\n"
            "adjacent to the vertex. The edges do not need to be rectilinear.\n\n"
            "Parameters:\n"
            "  vertex: The vertex where the fillet is added\n"
            "  radius: The fillet radius (must be > 0)\n\n"
            "Returns:\n"
            "  The fillet edge (only valid if status is IsDone)\n\n"
            "Status after operation (check with status property):\n"
            "- IsDone: fillet created successfully\n"
            "- ConnexionError: vertex does not belong to face\n"
            "- ComputationError: radius too large for adjacent edges\n"
            "- NotAuthorized: one adjacent edge is already a fillet/chamfer,\n"
            "  or uses unsupported curve type\n\n"
            "Raises:\n"
            "  Standard_NegativeValue if radius <= 0")
        
        .def("modify_fillet", &BRepFilletAPI_MakeFillet2d::ModifyFillet,
            py::arg("fillet"), py::arg("radius"),
            "Modifies the radius of an existing fillet.\n\n"
            "Parameters:\n"
            "  fillet: The existing fillet edge to modify\n"
            "  radius: The new fillet radius (must be > 0)\n\n"
            "Returns:\n"
            "  The new fillet edge (only valid if status is IsDone)\n\n"
            "Status after operation (check with status property):\n"
            "- IsDone: fillet modified successfully\n"
            "- ConnexionError: fillet does not belong to face\n"
            "- ComputationError: new radius too large\n\n"
            "Raises:\n"
            "  Standard_NegativeValue if radius <= 0")
        
        .def("remove_fillet", &BRepFilletAPI_MakeFillet2d::RemoveFillet,
            py::arg("fillet"),
            "Removes an existing fillet from the face.\n\n"
            "Parameters:\n"
            "  fillet: The fillet edge to remove\n\n"
            "Returns:\n"
            "  The vertex connecting the two adjacent edges\n"
            "  (only valid if status is IsDone, null if fillet not on face)")
        
        // Chamfer operations
        .def("add_chamfer",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Edge&, const Standard_Real, const Standard_Real>(&BRepFilletAPI_MakeFillet2d::AddChamfer),
            py::arg("E1"), py::arg("E2"), py::arg("D1"), py::arg("D2"),
            "Adds a chamfer between two adjacent edges with specified distances.\n\n"
            "The chamfer extremities are on E1 and E2 at distances D1 and D2\n"
            "respectively from their common vertex.\n\n"
            "For non-rectilinear edges, distances are measured using curvilinear\n"
            "abscissa along the edges.\n\n"
            "Parameters:\n"
            "  E1: First edge\n"
            "  E2: Second edge (must be adjacent to E1)\n"
            "  D1: Distance from common vertex along E1 (must be > 0)\n"
            "  D2: Distance from common vertex along E2 (must be > 0)\n\n"
            "Returns:\n"
            "  The chamfer edge (only valid if status is IsDone)\n\n"
            "Status after operation (check with status property):\n"
            "- IsDone: chamfer created successfully\n"
            "- ParametersError: D1 or D2 <= 0\n"
            "- ConnexionError: E1 or E2 not on face, or not adjacent\n"
            "- ComputationError: parameters too large\n"
            "- NotAuthorized: E1 or E2 is a fillet/chamfer, or uses unsupported curve")
        
        .def("add_chamfer",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Vertex&, const Standard_Real, const Standard_Real>(&BRepFilletAPI_MakeFillet2d::AddChamfer),
            py::arg("E"), py::arg("V"), py::arg("D"), py::arg("Ang"),
            "Adds a chamfer at a vertex with distance and angle.\n\n"
            "The chamfer makes an angle Ang with edge E, with one extremity\n"
            "on E at distance D from vertex V.\n\n"
            "For non-rectilinear edges, distance is measured using curvilinear\n"
            "abscissa and angle is measured relative to the tangent.\n\n"
            "Parameters:\n"
            "  E: The reference edge (must pass through V)\n"
            "  V: The vertex where chamfer is added\n"
            "  D: Distance from V along E (must be > 0)\n"
            "  Ang: Chamfer angle in radians (must be > 0)\n\n"
            "Returns:\n"
            "  The chamfer edge (only valid if status is IsDone)\n\n"
            "Status: Same as add_chamfer(E1, E2, D1, D2)")
        
        .def("modify_chamfer",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Edge&, const TopoDS_Edge&, const Standard_Real, const Standard_Real>(&BRepFilletAPI_MakeFillet2d::ModifyChamfer),
            py::arg("chamfer"), py::arg("E1"), py::arg("E2"), py::arg("D1"), py::arg("D2"),
            "Modifies an existing chamfer with new distances.\n\n"
            "Parameters:\n"
            "  chamfer: The existing chamfer edge to modify\n"
            "  E1: First adjacent edge\n"
            "  E2: Second adjacent edge\n"
            "  D1: New distance from common vertex along E1 (must be > 0)\n"
            "  D2: New distance from common vertex along E2 (must be > 0)\n\n"
            "Returns:\n"
            "  The new chamfer edge (only valid if status is IsDone)\n\n"
            "Status after operation (check with status property):\n"
            "- IsDone: chamfer modified successfully\n"
            "- ParametersError: D1 or D2 <= 0\n"
            "- ConnexionError: chamfer, E1 or E2 not on face, or E1/E2 not adjacent\n"
            "- ComputationError: new parameters too large\n"
            "- NotAuthorized: E1 or E2 is a fillet/chamfer")
        
        .def("modify_chamfer",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Edge&, const Standard_Real, const Standard_Real>(&BRepFilletAPI_MakeFillet2d::ModifyChamfer),
            py::arg("chamfer"), py::arg("E"), py::arg("D"), py::arg("Ang"),
            "Modifies an existing chamfer with new distance and angle.\n\n"
            "Parameters:\n"
            "  chamfer: The existing chamfer edge to modify\n"
            "  E: The reference adjacent edge\n"
            "  D: New distance from vertex along E (must be > 0)\n"
            "  Ang: New chamfer angle in radians (must be > 0)\n\n"
            "Returns:\n"
            "  The new chamfer edge (only valid if status is IsDone)\n\n"
            "Status: Same as modify_chamfer(chamfer, E1, E2, D1, D2)")
        
        .def("remove_chamfer", &BRepFilletAPI_MakeFillet2d::RemoveChamfer,
            py::arg("chamfer"),
            "Removes an existing chamfer from the face.\n\n"
            "Parameters:\n"
            "  chamfer: The chamfer edge to remove\n\n"
            "Returns:\n"
            "  The vertex connecting the two adjacent edges\n"
            "  (only valid if status is IsDone, null if chamfer not on face)")
        
        // Query methods
        .def("is_modified", &BRepFilletAPI_MakeFillet2d::IsModified,
            py::arg("edge"),
            "Returns True if the edge is chamfered or filleted.\n\n"
            "Returns False if edge does not belong to the modified face")
        
        .def("has_descendant", &BRepFilletAPI_MakeFillet2d::HasDescendant,
            py::arg("edge"),
            "Returns True if the edge has a descendant (chamfered/filleted version)")
        
        .def("descendant_edge", &BRepFilletAPI_MakeFillet2d::DescendantEdge,
            py::arg("edge"),
            py::return_value_policy::reference_internal,
            "Returns the chamfered or filleted edge built from edge E.\n\n"
            "If E has not been modified, returns E.\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if E does not belong to initial face")
        
        .def("basis_edge", &BRepFilletAPI_MakeFillet2d::BasisEdge,
            py::arg("edge"),
            py::return_value_policy::reference_internal,
            "Returns the basis edge from which chamfered/filleted edge E was built.\n\n"
            "If E has not been modified, returns E.\n"
            "Returns E if it does not belong to the initial face")
        
        .def("new_edges", &BRepFilletAPI_MakeFillet2d::NewEdges,
            py::arg("index"),
            py::return_value_policy::reference_internal,
            "Returns the edges created for curve I.\n\n"
            "Parameters:\n"
            "  index: Curve index (1 to nb_curves)")
        
        // Properties
        .def_property_readonly("fillet_edges", &BRepFilletAPI_MakeFillet2d::FilletEdges,
            py::return_value_policy::reference_internal,
            "Returns the sequence of all fillet edges on the modified face")
        
        .def_property_readonly("nb_fillet", &BRepFilletAPI_MakeFillet2d::NbFillet,
            "Returns the number of fillets on the modified face")
        
        .def_property_readonly("chamfer_edges", &BRepFilletAPI_MakeFillet2d::ChamferEdges,
            py::return_value_policy::reference_internal,
            "Returns the sequence of all chamfer edges on the modified face")
        
        .def_property_readonly("nb_chamfer", &BRepFilletAPI_MakeFillet2d::NbChamfer,
            "Returns the number of chamfers on the modified face")
        
        .def_property_readonly("nb_curves", &BRepFilletAPI_MakeFillet2d::NbCurves,
            "Returns the number of new curves after shape creation")
        
        .def_property_readonly("status", &BRepFilletAPI_MakeFillet2d::Status,
            "Returns the current construction status.\n\n"
            "Possible values:\n"
            "- IsDone: operation successful\n"
            "- Ready: algorithm ready (after init)\n"
            "- NotPlanar: face is not planar\n"
            "- NoFace: face is null\n"
            "- ParametersError: invalid parameters\n"
            "- ConnexionError: edge/vertex not on face or not adjacent\n"
            "- ComputationError: parameters too large\n"
            "- NotAuthorized: unsupported operation\n"
            "- TangencyError: tangency error\n"
            "- Unknown: unknown error")
        
        // Build method
        .def("build",
            [](BRepFilletAPI_MakeFillet2d& self) {
                self.Build();
            },
            "Updates the result and sets the Done flag.\n\n"
            "Call this after adding/modifying fillets and chamfers.\n"
            "Use is_done() to check success and shape() to retrieve result")
    ;
}