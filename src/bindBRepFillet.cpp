#include <pybind11/pybind11.h>

#include <BRepFilletAPI_LocalOperation.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <ChFiDS_SecHArray1.hxx>
#include <ChFiDS_ChamfMode.hxx>
#include <BRepFilletAPI_MakeChamfer.hxx>
#include <TopOpeBRepBuild_HBuilder.hxx>
#include <TopTools_ListOfShape.hxx>
#include <Message_ProgressRange.hxx>
#include <BRepFilletAPI_MakeFillet.hxx>
#include <ChFi3d_FilletShape.hxx>
#include <ChFiDS_ErrorStatus.hxx>
#include <Law_Function.hxx>
#include <TColgp_Array1OfPnt2d.hxx>
#include <Geom_Surface.hxx>
#include <GeomAbs_Shape.hxx>
#include <BRepFilletAPI_MakeFillet2d.hxx>
#include <ChFi2d_ConstructionError.hxx>
#include <TopTools_SequenceOfShape.hxx>

namespace py = pybind11;


void bind_brep_fillet(py::module_ &m)
{
    // ChFi3d_FilletShape enum
    py::enum_<ChFi3d_FilletShape>(m, "FilletShape")
        .value("Rational", ChFi3d_FilletShape::ChFi3d_Rational, 
            "Classical NURBS representation of circles")
        .value("QuasiAngular", ChFi3d_FilletShape::ChFi3d_QuasiAngular,
            "NURBS representation with parameterization matching the circle")
        .value("Polynomial", ChFi3d_FilletShape::ChFi3d_Polynomial,
            "Polynomial representation of circles")
        .export_values();

    // ChFiDS_ErrorStatus enum
    py::enum_<ChFiDS_ErrorStatus>(m, "ErrorStatus")
        .value("Ok", ChFiDS_ErrorStatus::ChFiDS_Ok, "Computation is Ok")
        .value("Error", ChFiDS_ErrorStatus::ChFiDS_Error, "General error")
        .value("WalkingFailure", ChFiDS_ErrorStatus::ChFiDS_WalkingFailure, 
            "Problem in the walking")
        .value("StartsolFailure", ChFiDS_ErrorStatus::ChFiDS_StartsolFailure,
            "Computation can't start, perhaps radius is too big")
        .value("TwistedSurface", ChFiDS_ErrorStatus::ChFiDS_TwistedSurface,
            "Computation failed because of a twisted surface")
        .export_values();

    // ChFi2d_ConstructionError enum
    py::enum_<ChFi2d_ConstructionError>(m, "ConstructionError")
        .value("IsDone", ChFi2d_ConstructionError::ChFi2d_IsDone, "Construction successful")
        .value("NotPlanar", ChFi2d_ConstructionError::ChFi2d_NotPlanar, "Face is not planar")
        .value("NoFace", ChFi2d_ConstructionError::ChFi2d_NoFace, "Face is null")
        .value("InitialisationError", ChFi2d_ConstructionError::ChFi2d_InitialisationError, "Initialization error")
        .value("ParametersError", ChFi2d_ConstructionError::ChFi2d_ParametersError, "Invalid parameters")
        .value("Ready", ChFi2d_ConstructionError::ChFi2d_Ready, "Ready for construction")
        .value("IsDoneNoError", ChFi2d_ConstructionError::ChFi2d_IsDone, "Done without error")
        .value("ComputationError", ChFi2d_ConstructionError::ChFi2d_ComputationError, "Computation error")
        .value("ConnexionError", ChFi2d_ConstructionError::ChFi2d_ConnexionError, "Connection error")
        .value("TangencyError", ChFi2d_ConstructionError::ChFi2d_TangencyError, "Tangency error")
        .value("FirstEdgeDegenerated", ChFi2d_ConstructionError::ChFi2d_FirstEdgeDegenerated, "First edge is degenerated")
        .value("LastEdgeDegenerated", ChFi2d_ConstructionError::ChFi2d_LastEdgeDegenerated, "Last edge is degenerated")
        .value("BothEdgesDegenerated", ChFi2d_ConstructionError::ChFi2d_BothEdgesDegenerated, "Both edges are degenerated")
        .value("NotAuthorized", ChFi2d_ConstructionError::ChFi2d_NotAuthorized, "Operation not authorized")
        .export_values();

    // ChFiDS_ChamfMode enum
    py::enum_<ChFiDS_ChamfMode>(m, "ChamfMode")
        .value("ClassicChamfer", ChFiDS_ChamfMode::ChFiDS_ClassicChamfer, 
            "Chamfer with constant distance from spine to one of the two surfaces")
        .value("ConstThroatChamfer", ChFiDS_ChamfMode::ChFiDS_ConstThroatChamfer, 
            "Symmetric chamfer with constant throat (height of isosceles triangle in section)")
        .value("ConstThroatWithPenetrationChamfer", ChFiDS_ChamfMode::ChFiDS_ConstThroatWithPenetrationChamfer,
            "Chamfer with constant throat: right-angled triangle section with penetration")
        .export_values();

    py::class_<BRepFilletAPI_LocalOperation, BRepBuilderAPI_MakeShape>(m, "FilletLocalOperation",
        "Abstract base class for local operations (fillets, chamfers) on edges.\n\n"
        "Manages contours of tangent edges and provides methods for querying\n"
        "and manipulating these contours.")
        
        // Contour building methods
        .def("add", &BRepFilletAPI_LocalOperation::Add,
            py::arg("edge"),
            "Adds a contour in the builder (builds a contour of tangent edges)")
        
        .def("reset_contour", &BRepFilletAPI_LocalOperation::ResetContour,
            py::arg("ic"),
            "Resets the contour of index ic, removing all information in the contour")
        
        .def("remove", &BRepFilletAPI_LocalOperation::Remove,
            py::arg("edge"),
            "Removes the contour containing the edge")
        
        .def("reset", &BRepFilletAPI_LocalOperation::Reset,
            "Resets all fields updated by Build operation.\n\n"
            "Leaves the algorithm in the same state as before build call.\n"
            "Allows contours and radius modifications to build the result another time")
        
        // Contour query methods
        .def_property_readonly("nb_contours", &BRepFilletAPI_LocalOperation::NbContours,
            "Number of contours")
        
        .def("contour", &BRepFilletAPI_LocalOperation::Contour,
            py::arg("edge"),
            "Returns the index of the contour containing the edge.\n"
            "Returns 0 if edge doesn't belong to any contour")
        
        .def("nb_edges", &BRepFilletAPI_LocalOperation::NbEdges,
            py::arg("i"),
            "Returns the number of edges in contour i")
        
        .def("edge", &BRepFilletAPI_LocalOperation::Edge,
            py::arg("i"), py::arg("j"),
            py::return_value_policy::reference_internal,
            "Returns the edge j in contour i")
        
        // Contour geometry methods
        .def("length", &BRepFilletAPI_LocalOperation::Length,
            py::arg("ic"),
            "Returns the length of the contour of index ic")
        
        .def("first_vertex", &BRepFilletAPI_LocalOperation::FirstVertex,
            py::arg("ic"),
            "Returns the first vertex of the contour of index ic")
        
        .def("last_vertex", &BRepFilletAPI_LocalOperation::LastVertex,
            py::arg("ic"),
            "Returns the last vertex of the contour of index ic")
        
        .def("abscissa", &BRepFilletAPI_LocalOperation::Abscissa,
            py::arg("ic"), py::arg("vertex"),
            "Returns the abscissa of the vertex on the contour of index ic")
        
        .def("relative_abscissa", &BRepFilletAPI_LocalOperation::RelativeAbscissa,
            py::arg("ic"), py::arg("vertex"),
            "Returns the relative abscissa ([0, 1]) of the vertex on contour ic")
        
        // Contour topology methods
        .def("closed_and_tangent", &BRepFilletAPI_LocalOperation::ClosedAndTangent,
            py::arg("ic"),
            "Returns True if the contour of index ic is closed and tangent")
        
        .def("closed", &BRepFilletAPI_LocalOperation::Closed,
            py::arg("ic"),
            "Returns True if the contour of index ic is closed")
        
        // Simulation methods
        .def("simulate", &BRepFilletAPI_LocalOperation::Simulate,
            py::arg("ic"),
            "Simulates the operation on contour ic")
        
        .def("nb_surf", &BRepFilletAPI_LocalOperation::NbSurf,
            py::arg("ic"),
            "Returns the number of surfaces for contour ic")
        
        .def("sect", &BRepFilletAPI_LocalOperation::Sect,
            py::arg("ic"), py::arg("is"),
            "Returns the section array for contour ic, surface is")
    ;

    py::class_<BRepFilletAPI_MakeChamfer, BRepFilletAPI_LocalOperation>(m, "MakeChamfer",
        "Builds chamfers on edges of a shell or solid.\n\n"
        "Provides a framework for:\n"
        "- Initializing the construction algorithm with a shape\n"
        "- Defining chamfer parameters (symmetric, two distances, distance+angle)\n"
        "- Building the chamfers and constructing the resulting shape\n"
        "- Consulting the result")
        
        .def(py::init<const TopoDS_Shape&>(),
            py::arg("shape"),
            "Initializes an algorithm for computing chamfers on the shape.\n"
            "The edges on which chamfers are built are defined using Add")
        
        // Add methods - multiple overloads
        .def("add",
            py::overload_cast<const TopoDS_Edge&>(&BRepFilletAPI_MakeChamfer::Add),
            py::arg("edge"),
            "Adds edge to the table of edges (parameters set later)")
        
        .def("add",
            py::overload_cast<const Standard_Real, const TopoDS_Edge&>(&BRepFilletAPI_MakeChamfer::Add),
            py::arg("dis"), py::arg("edge"),
            "Adds edge with symmetric chamfer distance.\n\n"
            "Builds a contour by propagation from the edge (contour contains\n"
            "at least this edge). The contour is composed of edges tangential\n"
            "to one another, delimiting two series of tangential faces")
        
        .def("add",
            py::overload_cast<const Standard_Real, const Standard_Real, const TopoDS_Edge&, const TopoDS_Face&>(&BRepFilletAPI_MakeChamfer::Add),
            py::arg("dis1"), py::arg("dis2"), py::arg("edge"), py::arg("face"),
            "Adds edge with two distances chamfer.\n\n"
            "Parameters:\n"
            "  dis1: distance measured on face\n"
            "  dis2: distance measured on the other face\n"
            "  edge: edge to chamfer\n"
            "  face: face where dis1 is measured\n\n"
            "Builds a contour by propagation from the edge")
        
        .def("add_da", &BRepFilletAPI_MakeChamfer::AddDA,
            py::arg("dis"), py::arg("angle"), py::arg("edge"), py::arg("face"),
            "Adds a chamfer contour with distance and angle parameters.\n\n"
            "Parameters:\n"
            "  dis: distance from the edge\n"
            "  angle: angle of the chamfer\n"
            "  edge: edge to chamfer\n"
            "  face: reference face")
        
        // Set/Get distance methods
        .def("set_dist", &BRepFilletAPI_MakeChamfer::SetDist,
            py::arg("dis"), py::arg("ic"), py::arg("face"),
            "Sets the symmetric distance for contour ic.\n\n"
            "Parameters:\n"
            "  dis: symmetric distance\n"
            "  ic: contour index\n"
            "  face: reference face")
        
        .def("get_dist",
            [](const BRepFilletAPI_MakeChamfer& self, Standard_Integer ic) {
                Standard_Real dis;
                self.GetDist(ic, dis);
                return dis;
            },
            py::arg("ic"),
            "Returns the symmetric distance for contour ic")
        
        .def("set_dists", &BRepFilletAPI_MakeChamfer::SetDists,
            py::arg("dis1"), py::arg("dis2"), py::arg("ic"), py::arg("face"),
            "Sets the two distances for contour ic.\n\n"
            "Parameters:\n"
            "  dis1: distance measured on face\n"
            "  dis2: distance measured on the other face\n"
            "  ic: contour index\n"
            "  face: face where dis1 is measured")
        
        .def("dists",
            [](const BRepFilletAPI_MakeChamfer& self, Standard_Integer ic) {
                Standard_Real dis1, dis2;
                self.Dists(ic, dis1, dis2);
                return py::make_tuple(dis1, dis2);
            },
            py::arg("ic"),
            "Returns tuple (dis1, dis2) for contour ic.\n"
            "Returns (-1, -1) if ic is outside bounds")
        
        // Distance and angle methods
        .def("set_dist_angle", &BRepFilletAPI_MakeChamfer::SetDistAngle,
            py::arg("dis"), py::arg("angle"), py::arg("ic"), py::arg("face"),
            "Sets distance and angle for contour ic.\n\n"
            "Parameters:\n"
            "  dis: distance from edge\n"
            "  angle: chamfer angle\n"
            "  ic: contour index\n"
            "  face: reference face")
        
        .def("get_dist_angle",
            [](const BRepFilletAPI_MakeChamfer& self, Standard_Integer ic) {
                Standard_Real dis, angle;
                self.GetDistAngle(ic, dis, angle);
                return py::make_tuple(dis, angle);
            },
            py::arg("ic"),
            "Returns tuple (distance, angle) for contour ic")
        
        // Mode setting
        .def("set_mode", &BRepFilletAPI_MakeChamfer::SetMode,
            py::arg("mode"),
            "Sets the chamfer mode (Sym, TwoDist, or DistAngle)")
        
        // Query chamfer type
        .def("is_symetric", &BRepFilletAPI_MakeChamfer::IsSymetric,
            py::arg("ic"),
            "Returns True if contour ic is a symmetric chamfer")
        
        .def("is_two_distances", &BRepFilletAPI_MakeChamfer::IsTwoDistances,
            py::arg("ic"),
            "Returns True if contour ic is made with two distances")
        
        .def("is_distance_angle", &BRepFilletAPI_MakeChamfer::IsDistanceAngle,
            py::arg("ic"),
            "Returns True if contour ic is made with distance and angle")
        
        // Build and reset
        .def("build", 
            [](BRepFilletAPI_MakeChamfer& self) { 
                self.Build(); 
            },
            "Builds the chamfers on all contours.\n\n"
            "Constructs the resulting shape with chamfers applied.\n"
            "Use IsDone() to verify success and Shape() to retrieve result.\n\n"
            "Warning: Complex construction may fail. Current limitations:\n"
            "- End point is intersection of 4+ edges\n"
            "- Chamfer intersection with limiting face not fully contained")
        
        .def("reset", &BRepFilletAPI_MakeChamfer::Reset,
            "Reinitializes the algorithm, canceling Build effects.\n"
            "Allows modifications to contours and parameters to rebuild")
        
        // Builder access
        .def("builder", &BRepFilletAPI_MakeChamfer::Builder,
            "Returns the internal filleting algorithm builder")
        
        // Shape modification queries
        .def("generated", &BRepFilletAPI_MakeChamfer::Generated,
            py::arg("shape"),
            py::return_value_policy::reference_internal,
            "Returns the list of shapes generated from the shape")
        
        .def("modified", &BRepFilletAPI_MakeChamfer::Modified,
            py::arg("shape"),
            py::return_value_policy::reference_internal,
            "Returns the list of shapes modified from the shape")
        
        .def("is_deleted", &BRepFilletAPI_MakeChamfer::IsDeleted,
            py::arg("shape"),
            "Returns True if the shape was deleted during chamfer operation")
    ;

    py::class_<BRepFilletAPI_MakeFillet, BRepFilletAPI_LocalOperation>(m, "MakeFillet",
        "Builds fillets on edges of a shell or solid.\n\n"
        "Provides a framework for:\n"
        "- Initializing the construction algorithm with a shape\n"
        "- Defining fillet parameters (constant, linear evolution, law-based)\n"
        "- Building the fillets and constructing the resulting shape\n"
        "- Consulting the result and error information")
        
        .def(py::init<const TopoDS_Shape&, const ChFi3d_FilletShape>(),
            py::arg("shape"), py::arg("fshape") = ChFi3d_Rational,
            "Initializes the computation of fillets.\n\n"
            "fshape sets the type of fillet surface:\n"
            "- Rational (default): classical NURBS representation of circles\n"
            "- QuasiAngular: NURBS with parameterization matching the circle\n"
            "- Polynomial: polynomial representation of circles")
        
        // Parameter setting
        .def("set_params", &BRepFilletAPI_MakeFillet::SetParams,
            py::arg("tang"), py::arg("tesp"), py::arg("t2d"), 
            py::arg("tapp3d"), py::arg("tolapp2d"), py::arg("fleche"),
            "Sets the computation parameters")
        
        .def("set_continuity", &BRepFilletAPI_MakeFillet::SetContinuity,
            py::arg("internal_continuity"), py::arg("angular_tolerance"),
            "Changes the parameters of continuity.\n\n"
            "Produces fillet surfaces with continuity Ci (i=0,1 or 2).\n"
            "Default InternalContinuity = GeomAbs_C1.\n"
            "AngularTolerance is the G1 tolerance between fillet and support faces")
        
        // Add methods - multiple overloads
        .def("add",
            py::overload_cast<const TopoDS_Edge&>(&BRepFilletAPI_MakeFillet::Add),
            py::arg("edge"),
            "Adds a fillet contour (builds contour of tangent edges).\n"
            "The radius must be set after")
        
        .def("add",
            py::overload_cast<const Standard_Real, const TopoDS_Edge&>(&BRepFilletAPI_MakeFillet::Add),
            py::arg("radius"), py::arg("edge"),
            "Adds a fillet with constant radius.\n\n"
            "Builds a contour of tangent edges and sets the radius")
        
        .def("add",
            py::overload_cast<const Standard_Real, const Standard_Real, const TopoDS_Edge&>(&BRepFilletAPI_MakeFillet::Add),
            py::arg("r1"), py::arg("r2"), py::arg("edge"),
            "Adds a fillet with linear radius evolution.\n\n"
            "Builds a contour of tangent edges and sets linear radius evolution\n"
            "law between the first and last vertex of the spine")
        
        .def("add",
            py::overload_cast<const opencascade::handle<Law_Function>&, const TopoDS_Edge&>(&BRepFilletAPI_MakeFillet::Add),
            py::arg("law"), py::arg("edge"),
            "Adds a fillet with radius evolution law.\n\n"
            "Builds a contour of tangent edges and sets the radius evolution law")
        
        .def("add",
            py::overload_cast<const TColgp_Array1OfPnt2d&, const TopoDS_Edge&>(&BRepFilletAPI_MakeFillet::Add),
            py::arg("uandr"), py::arg("edge"),
            "Adds a fillet with interpolated radius values.\n\n"
            "Builds a contour of tangent edges and sets radius evolution law\n"
            "interpolating the values given in the array UandR:\n"
            "  p2d.X() = relative parameter on the spine [0,1]\n"
            "  p2d.Y() = value of the radius")
        
        // SetRadius methods - multiple overloads
        .def("set_radius",
            py::overload_cast<const Standard_Real, const Standard_Integer, const Standard_Integer>(&BRepFilletAPI_MakeFillet::SetRadius),
            py::arg("radius"), py::arg("ic"), py::arg("iinc"),
            "Sets constant radius for contour ic at edge iinc")
        
        .def("set_radius",
            py::overload_cast<const Standard_Real, const Standard_Real, const Standard_Integer, const Standard_Integer>(&BRepFilletAPI_MakeFillet::SetRadius),
            py::arg("r1"), py::arg("r2"), py::arg("ic"), py::arg("iinc"),
            "Sets linear radius evolution from r1 to r2 for contour ic")
        
        .def("set_radius",
            py::overload_cast<const opencascade::handle<Law_Function>&, const Standard_Integer, const Standard_Integer>(&BRepFilletAPI_MakeFillet::SetRadius),
            py::arg("law"), py::arg("ic"), py::arg("iinc"),
            "Sets radius evolution law for contour ic")
        
        .def("set_radius",
            py::overload_cast<const TColgp_Array1OfPnt2d&, const Standard_Integer, const Standard_Integer>(&BRepFilletAPI_MakeFillet::SetRadius),
            py::arg("uandr"), py::arg("ic"), py::arg("iinc"),
            "Sets radius evolution by interpolating UandR values for contour ic")
        
        .def("set_radius_on_edge",
            py::overload_cast<const Standard_Real, const Standard_Integer, const TopoDS_Edge&>(&BRepFilletAPI_MakeFillet::SetRadius),
            py::arg("radius"), py::arg("ic"), py::arg("edge"),
            "Assigns radius as the radius of the fillet on edge")
        
        .def("set_radius_on_vertex",
            py::overload_cast<const Standard_Real, const Standard_Integer, const TopoDS_Vertex&>(&BRepFilletAPI_MakeFillet::SetRadius),
            py::arg("radius"), py::arg("ic"), py::arg("vertex"),
            "Sets radius at vertex for contour ic")
        
        // Query radius methods
        .def("is_constant",
            py::overload_cast<const Standard_Integer>(&BRepFilletAPI_MakeFillet::IsConstant),
            py::arg("ic"),
            "Returns True if the radius along contour ic is constant.\n"
            "Returns False if ic is outside bounds")
        
        .def("is_constant_on_edge",
            py::overload_cast<const Standard_Integer, const TopoDS_Edge&>(&BRepFilletAPI_MakeFillet::IsConstant),
            py::arg("ic"), py::arg("edge"),
            "Returns True if radius along edge E of contour ic is constant.\n"
            "Returns False if ic is outside bounds or edge not in contour")
        
        .def("radius",
            py::overload_cast<const Standard_Integer>(&BRepFilletAPI_MakeFillet::Radius),
            py::arg("ic"),
            "Returns the radius of the fillet along contour ic.\n"
            "Use only if radius is constant. Returns -1 if ic outside bounds")
        
        .def("radius_on_edge",
            py::overload_cast<const Standard_Integer, const TopoDS_Edge&>(&BRepFilletAPI_MakeFillet::Radius),
            py::arg("ic"), py::arg("edge"),
            "Returns the radius of the fillet along edge of contour ic.\n"
            "Use only if radius is constant. Returns -1 if invalid")
        
        // Law methods
        .def("get_bounds",
            [](BRepFilletAPI_MakeFillet& self, Standard_Integer ic, const TopoDS_Edge& e) {
                Standard_Real f, l;
                Standard_Boolean result = self.GetBounds(ic, e, f, l);
                return py::make_tuple(result, f, l);
            },
            py::arg("ic"), py::arg("edge"),
            "Returns tuple (success, first, last) bounds for edge in contour ic")
        
        .def("get_law", &BRepFilletAPI_MakeFillet::GetLaw,
            py::arg("ic"), py::arg("edge"),
            "Returns the radius evolution law for edge in contour ic")
        
        .def("set_law", &BRepFilletAPI_MakeFillet::SetLaw,
            py::arg("ic"), py::arg("edge"), py::arg("law"),
            "Sets the radius evolution law for edge in contour ic")
        
        // Fillet shape
        .def("set_fillet_shape", &BRepFilletAPI_MakeFillet::SetFilletShape,
            py::arg("fshape"),
            "Assigns fshape as the type of fillet shape built by this algorithm")
        
        .def_property("fillet_shape",
            &BRepFilletAPI_MakeFillet::GetFilletShape,
            &BRepFilletAPI_MakeFillet::SetFilletShape,
            "Type of fillet shape (Rational, QuasiAngular, or Polynomial)")
        
        // Build and reset
        .def("build", 
            [](BRepFilletAPI_MakeFillet& self) { 
                self.Build(); 
            },
            "Builds the fillets on all contours.\n\n"
            "Constructs the resulting shape with fillets applied.\n"
            "Use IsDone() to verify success and Shape() to retrieve result.\n\n"
            "Warning: Complex construction may fail. Current limitations:\n"
            "- End point is intersection of 4+ edges\n"
            "- Fillet intersection with limiting face not fully contained")
        
        .def("reset", &BRepFilletAPI_MakeFillet::Reset,
            "Reinitializes the algorithm, canceling Build effects.\n"
            "Allows modifications to contours and parameters to rebuild")
        
        // Builder access
        .def("builder", &BRepFilletAPI_MakeFillet::Builder,
            "Returns the internal topology building algorithm")
        
        // Shape modification queries
        .def("generated", &BRepFilletAPI_MakeFillet::Generated,
            py::arg("shape"),
            py::return_value_policy::reference_internal,
            "Returns the list of shapes generated from the shape")
        
        .def("modified", &BRepFilletAPI_MakeFillet::Modified,
            py::arg("shape"),
            py::return_value_policy::reference_internal,
            "Returns the list of shapes modified from the shape")
        
        .def("is_deleted", &BRepFilletAPI_MakeFillet::IsDeleted,
            py::arg("shape"),
            "Returns True if the shape was deleted during fillet operation")
        
        // Surface queries
        .def_property_readonly("nb_surfaces", &BRepFilletAPI_MakeFillet::NbSurfaces,
            "Number of surfaces after shape creation")
        
        .def("new_faces", &BRepFilletAPI_MakeFillet::NewFaces,
            py::arg("i"),
            py::return_value_policy::reference_internal,
            "Returns the faces created for surface i")
        
        // Error information
        .def_property_readonly("nb_faulty_contours", &BRepFilletAPI_MakeFillet::NbFaultyContours,
            "Number of contours where computation failed")
        
        .def("faulty_contour", &BRepFilletAPI_MakeFillet::FaultyContour,
            py::arg("i"),
            "Returns the index of the i-th faulty contour.\n"
            "Use NbEdges(ic) and Edge(ic,ie) to get contour details")
        
        .def("nb_computed_surfaces", &BRepFilletAPI_MakeFillet::NbComputedSurfaces,
            py::arg("ic"),
            "Returns the number of surfaces computed on contour ic")
        
        .def("computed_surface", &BRepFilletAPI_MakeFillet::ComputedSurface,
            py::arg("ic"), py::arg("is"),
            "Returns surface number is for contour ic")
        
        .def_property_readonly("nb_faulty_vertices", &BRepFilletAPI_MakeFillet::NbFaultyVertices,
            "Number of vertices where computation failed")
        
        .def("faulty_vertex", &BRepFilletAPI_MakeFillet::FaultyVertex,
            py::arg("iv"),
            "Returns the vertex where computation failed")
        
        .def("has_result", &BRepFilletAPI_MakeFillet::HasResult,
            "Returns True if a part of the result has been computed.\n"
            "If filling in a corner failed, a shape with a hole is returned")
        
        .def("bad_shape", &BRepFilletAPI_MakeFillet::BadShape,
            "Returns the partial result if HasResult() is True")
        
        .def("stripe_status", &BRepFilletAPI_MakeFillet::StripeStatus,
            py::arg("ic"),
            "Returns the error status for contour ic:\n"
            "- Ok: computation successful\n"
            "- StartsolFailure: can't start, radius may be too big\n"
            "- TwistedSurface: failed due to twisted surface\n"
            "- WalkingFailure: problem in the walking\n"
            "- Error: other error")
    ;

    py::class_<BRepFilletAPI_MakeFillet2d, BRepBuilderAPI_MakeShape>(m, "MakeFillet2d",
        "Builds fillets and chamfers on vertices of a planar face.\n\n"
        "Provides a framework for:\n"
        "- Initializing the construction with a planar face\n"
        "- Adding/modifying/removing fillets at vertices\n"
        "- Adding/modifying/removing chamfers between edges\n"
        "- Building the result and consulting modifications\n\n"
        "Warning: Only segments of straight lines and arcs of circles are treated.\n"
        "BSplines are not processed.")
        
        .def(py::init<>(),
            "Initializes an empty algorithm for computing fillets and chamfers.\n"
            "The face must be defined using Init function.\n"
            "Check status with Status() method after initialization")
        
        .def(py::init<const TopoDS_Face&>(),
            py::arg("face"),
            "Initializes the algorithm for computing fillets and chamfers on face F.\n\n"
            "Status after initialization can be:\n"
            "- Ready: initialization correct\n"
            "- NotPlanar: F is not planar\n"
            "- NoFace: F is a null face")
        
        // Init methods
        .def("init",
            py::overload_cast<const TopoDS_Face&>(&BRepFilletAPI_MakeFillet2d::Init),
            py::arg("face"),
            "Initializes this algorithm with face F.\n\n"
            "Status after initialization can be:\n"
            "- Ready: initialization correct\n"
            "- NotPlanar: F is not planar\n"
            "- NoFace: F is a null face")
        
        .def("init",
            py::overload_cast<const TopoDS_Face&, const TopoDS_Face&>(&BRepFilletAPI_MakeFillet2d::Init),
            py::arg("ref_face"), py::arg("mod_face"),
            "Initializes from reference face and modified face.\n\n"
            "This is useful to modify a fillet or chamfer already created on ModFace\n"
            "which derives from RefFace")
        
        // Fillet methods
        .def("add_fillet", &BRepFilletAPI_MakeFillet2d::AddFillet,
            py::arg("vertex"), py::arg("radius"),
            "Adds a fillet of radius between two edges adjacent to vertex V.\n\n"
            "The two edges do not need to be rectilinear.\n"
            "Returns the fillet edge and builds the resulting face.\n\n"
            "Status can be:\n"
            "- IsDone: fillet built successfully\n"
            "- ConnexionError: V doesn't belong to initial face\n"
            "- ComputationError: Radius too large\n"
            "- NotAuthorized: edge connected to V is fillet/chamfer or not straight/arc\n\n"
            "Raises Standard_NegativeValue if Radius <= 0")
        
        .def("modify_fillet", &BRepFilletAPI_MakeFillet2d::ModifyFillet,
            py::arg("fillet"), py::arg("radius"),
            "Assigns new radius to existing fillet.\n\n"
            "Returns the new fillet edge and modifies the existing face.\n\n"
            "Status can be:\n"
            "- IsDone: new fillet built\n"
            "- ConnexionError: Fillet doesn't belong to face\n"
            "- ComputationError: Radius too large\n\n"
            "Raises Standard_NegativeValue if Radius <= 0")
        
        .def("remove_fillet", &BRepFilletAPI_MakeFillet2d::RemoveFillet,
            py::arg("fillet"),
            "Removes the fillet and modifies the existing face.\n\n"
            "Returns the vertex connecting the two adjacent edges.\n"
            "Vertex is only valid if Status returns IsDone.\n"
            "Returns null vertex if edge doesn't belong to initial face")
        
        // Chamfer methods
        .def("add_chamfer",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Edge&, const Standard_Real, const Standard_Real>(&BRepFilletAPI_MakeFillet2d::AddChamfer),
            py::arg("e1"), py::arg("e2"), py::arg("d1"), py::arg("d2"),
            "Adds a chamfer between adjacent edges E1 and E2.\n\n"
            "Chamfer extremities are on E1 and E2 at distances D1 and D2 respectively.\n"
            "For non-rectilinear edges, distances measured using curvilinear abscissa.\n"
            "Returns the chamfer edge and builds the resulting face")
        
        .def("add_chamfer",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Vertex&, const Standard_Real, const Standard_Real>(&BRepFilletAPI_MakeFillet2d::AddChamfer),
            py::arg("edge"), py::arg("vertex"), py::arg("d"), py::arg("ang"),
            "Adds a chamfer between two edges connected by vertex V.\n\n"
            "E is one of the two edges. Chamfer makes angle Ang with E\n"
            "and one extremity is on E at distance D from V.\n"
            "Angle Ang is given in radians.\n"
            "Returns the chamfer edge and builds the resulting face.\n\n"
            "Status can be:\n"
            "- IsDone: chamfer built\n"
            "- ParametersError: D or Ang <= 0\n"
            "- ConnexionError: E doesn't belong to face or V not on E\n"
            "- ComputationError: parameters too large\n"
            "- NotAuthorized: edge is fillet/chamfer or not straight/arc")
        
        .def("modify_chamfer",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Edge&, const TopoDS_Edge&, const Standard_Real, const Standard_Real>(&BRepFilletAPI_MakeFillet2d::ModifyChamfer),
            py::arg("chamfer"), py::arg("e1"), py::arg("e2"), py::arg("d1"), py::arg("d2"),
            "Modifies existing chamfer with new distances.\n\n"
            "E1 and E2 are the two adjacent edges on which Chamfer is built.\n"
            "New chamfer extremities are on E1 and E2 at distances D1 and D2.\n"
            "Returns the new chamfer edge")
        
        .def("modify_chamfer",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Edge&, const Standard_Real, const Standard_Real>(&BRepFilletAPI_MakeFillet2d::ModifyChamfer),
            py::arg("chamfer"), py::arg("edge"), py::arg("d"), py::arg("ang"),
            "Modifies existing chamfer with new distance and angle.\n\n"
            "E is one of the two adjacent edges on which Chamfer is built.\n"
            "New chamfer makes angle Ang with E and one extremity is\n"
            "on E at distance D from the vertex.\n"
            "Angle Ang is given in radians.\n"
            "Returns the new chamfer edge and modifies the existing face.\n\n"
            "Status can be:\n"
            "- IsDone: chamfer built\n"
            "- ParametersError: D or Ang <= 0\n"
            "- ConnexionError: edge or chamfer doesn't belong to face\n"
            "- ComputationError: parameters too large\n"
            "- NotAuthorized: E1 or E2 is fillet/chamfer")
        
        .def("remove_chamfer", &BRepFilletAPI_MakeFillet2d::RemoveChamfer,
            py::arg("chamfer"),
            "Removes the chamfer and modifies the existing face.\n\n"
            "Returns the vertex connecting the two adjacent edges.\n"
            "Vertex is only valid if Status returns IsDone.\n"
            "Returns null vertex if edge doesn't belong to initial face")
        
        // Query methods
        .def("is_modified", &BRepFilletAPI_MakeFillet2d::IsModified,
            py::arg("edge"),
            "Returns True if edge E is chamfered or filleted.\n"
            "Returns False if E doesn't belong to the face")
        
        .def_property_readonly("fillet_edges", 
            [](const BRepFilletAPI_MakeFillet2d& self) -> const TopTools_SequenceOfShape& {
                return self.FilletEdges();
            },
            py::return_value_policy::reference_internal,
            "Table of fillets on the modified face")
        
        .def_property_readonly("nb_fillet",
            [](const BRepFilletAPI_MakeFillet2d& self) { return self.NbFillet(); },
            "Number of fillets on the modified face")
        
        .def_property_readonly("chamfer_edges",
            [](const BRepFilletAPI_MakeFillet2d& self) -> const TopTools_SequenceOfShape& {
                return self.ChamferEdges();
            },
            py::return_value_policy::reference_internal,
            "Table of chamfers on the modified face")
        
        .def_property_readonly("nb_chamfer",
            [](const BRepFilletAPI_MakeFillet2d& self) { return self.NbChamfer(); },
            "Number of chamfers on the modified face")
        
        // Shape modification
        .def("modified", &BRepFilletAPI_MakeFillet2d::Modified,
            py::arg("shape"),
            py::return_value_policy::reference_internal,
            "Returns the list of shapes modified from the shape")
        
        .def_property_readonly("nb_curves", &BRepFilletAPI_MakeFillet2d::NbCurves,
            "Number of new curves after shape creation")
        
        .def("new_edges", &BRepFilletAPI_MakeFillet2d::NewEdges,
            py::arg("i"),
            py::return_value_policy::reference_internal,
            "Returns the edges created for curve i")
        
        // Edge relationships
        .def("has_descendant", &BRepFilletAPI_MakeFillet2d::HasDescendant,
            py::arg("edge"),
            "Returns True if edge E has a descendant (modified edge)")
        
        .def("descendant_edge", &BRepFilletAPI_MakeFillet2d::DescendantEdge,
            py::arg("edge"),
            py::return_value_policy::reference_internal,
            "Returns the chamfered or filleted edge built from edge E.\n"
            "If E has not been modified, returns E.\n\n"
            "Raises Standard_NoSuchObject if E doesn't belong to initial face")
        
        .def("basis_edge", &BRepFilletAPI_MakeFillet2d::BasisEdge,
            py::arg("edge"),
            py::return_value_policy::reference_internal,
            "Returns the basis edge from which chamfered/filleted edge E is built.\n"
            "If E has not been modified, returns E.\n"
            "Returns E if it doesn't belong to initial face")
        
        // Status
        .def_property_readonly("status", &BRepFilletAPI_MakeFillet2d::Status,
            "Returns the construction status:\n"
            "- IsDone/Ready: successful\n"
            "- NotPlanar: face not planar\n"
            "- NoFace: null face\n"
            "- ParametersError: invalid parameters\n"
            "- ConnexionError: connectivity error\n"
            "- ComputationError: computation failed\n"
            "- NotAuthorized: operation not allowed")
        
        // Build
        .def("build",
            [](BRepFilletAPI_MakeFillet2d& self) {
                self.Build();
            },
            "Updates the result and sets the Done flag")
    ;
}