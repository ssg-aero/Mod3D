#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <BRepOffsetAPI_MakeEvolved.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepFill_Evolved.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <GeomAbs_JoinType.hxx>
#include <TopTools_ListOfShape.hxx>
#include <Message_ProgressRange.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <BRepPrimAPI_MakeSweep.hxx>
#include <BRepFill_TypeOfContact.hxx>
#include <BRepBuilderAPI_PipeError.hxx>
#include <BRepBuilderAPI_TransitionMode.hxx>
#include <BRepOffsetAPI_MakeFilling.hxx>
#include <BRepOffsetAPI_MakeOffset.hxx>
#include <BRepOffsetAPI_MakeOffsetShape.hxx>
#include <BRepOffsetAPI_MiddlePath.hxx>
#include <BRepOffsetAPI_NormalProjection.hxx>
#include <BRepOffsetAPI_ThruSections.hxx>
#include <BRepFill_ThruSectionErrorStatus.hxx>
#include <Approx_ParametrizationType.hxx>
#include <BRepOffset_Mode.hxx>
#include <TopoDS_Vertex.hxx>
#include <Law_Function.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>

namespace py = pybind11;

void bind_brep_offset_api(py::module_ &m)
{
    // ==================== BRepOffset_Mode Enum ====================
    py::enum_<BRepOffset_Mode>(m, "OffsetMode",
        R"doc(
        Offset construction modes.

        Defines how the offset algorithm handles the shape:
        - Skin: Offset along the surface of a solid (default)
        - Pipe: Offset of a curve to obtain a pre-surface
        - RectoVerso: Offset along both sides of a surface shell
        )doc")
        .value("Skin", BRepOffset_Skin,
            "Offset along the surface of a solid to obtain a manifold space")
        .value("Pipe", BRepOffset_Pipe,
            "Offset of a curve to obtain a pre-surface")
        .value("RectoVerso", BRepOffset_RectoVerso,
            "Offset along both sides of a surface shell")
        .export_values();

    py::class_<BRepOffsetAPI_MakeEvolved, BRepBuilderAPI_MakeShape>(m, "MakeEvolved",
        "Builds evolved shapes by sweeping a profile along a spine.\n\n"
        "An evolved shape is the unlooped sweep (pipe) of a profile (wire)\n"
        "along a planar spine (face or wire) with self-intersections removed.\n\n"
        "The profile is defined in a reference frame R. The position of the\n"
        "profile at the current point of the spine is given by confusing R\n"
        "and the local referential (D0, D1, and spine normal).\n\n"
        "Coordinate system determination (is_axe_prof argument):\n"
        "- True: R is the global coordinate system\n"
        "- False: R is computed with:\n"
        "  * Origin: point on spine closest to profile\n"
        "  * X Axis: tangent to spine at this point\n"
        "  * Z Axis: normal to plane containing spine")
        
        .def(py::init<>(),
            "Creates an empty evolved shape")
        
        .def(py::init<const TopoDS_Shape&, const TopoDS_Wire&, const GeomAbs_JoinType,
                      const Standard_Boolean, const Standard_Boolean, const Standard_Boolean,
                      const Standard_Real, const Standard_Boolean, const Standard_Boolean>(),
            py::arg("spine"), py::arg("profile"),
            py::arg("join_type") = GeomAbs_Arc,
            py::arg("is_axe_prof") = true,
            py::arg("is_solid") = false,
            py::arg("is_prof_on_spine") = false,
            py::arg("tol") = 0.0000001,
            py::arg("is_volume") = false,
            py::arg("run_in_parallel") = false,
            "Constructs an evolved shape by sweeping profile along spine.\n\n"
            "Parameters:\n"
            "  spine: Wire or face to sweep along (must be planar)\n"
            "  profile: Wire profile to sweep (expected to be planar or a line)\n"
            "  join_type: Type of pipe at salient vertices (default: Arc)\n"
            "  is_axe_prof: True for global coords, False for computed frame\n"
            "  is_solid: True to complete result as solid/compound of solids\n"
            "  is_prof_on_spine: True if profile must connect with spine\n"
            "  tol: Tolerance for construction (default: 1e-7)\n"
            "  is_volume: True to remove self-intersections with MakerVolume\n"
            "  run_in_parallel: True to enable parallel computation\n\n"
            "Note: When is_volume=True, join_type, is_axe_prof, and\n"
            "is_prof_on_spine are not used")
        
        .def("evolved", &BRepOffsetAPI_MakeEvolved::Evolved,
            py::return_value_policy::reference_internal,
            "Returns the internal BRepFill_Evolved object")
        
        .def("build",
            [](BRepOffsetAPI_MakeEvolved& self) {
                self.Build();
            },
            "Builds the resulting evolved shape.\n\n"
            "Performs the sweep operation and removes self-intersections.\n"
            "Use IsDone() to check success and Shape() to retrieve result")
        
        .def("generated_shapes", &BRepOffsetAPI_MakeEvolved::GeneratedShapes,
            py::arg("spine_shape"), py::arg("prof_shape"),
            py::return_value_policy::reference_internal,
            "Returns shapes created from spine and profile subshapes.\n\n"
            "Parameters:\n"
            "  spine_shape: A subshape of the spine\n"
            "  prof_shape: A subshape of the profile\n\n"
            "Returns the list of shapes generated from these subshapes")
        
        .def_property_readonly("top", &BRepOffsetAPI_MakeEvolved::Top,
            "Returns the top face (only valid if is_solid=True in constructor)")
        
        .def_property_readonly("bottom", &BRepOffsetAPI_MakeEvolved::Bottom,
            "Returns the bottom face (only valid if is_solid=True in constructor)")
    ;

    py::class_<BRepOffsetAPI_MakeFilling, BRepBuilderAPI_MakeShape>(m, "MakeFilling",
        "N-Side Filling surface construction.\n\n"
        "This algorithm builds a face from:\n"
        "- A set of edges defining the bounds of the face and some constraints\n"
        "  the surface has to satisfy\n"
        "- A set of edges and points defining constraints the support surface\n"
        "  has to satisfy\n"
        "- An optional initial surface to deform for satisfying the constraints\n"
        "- A set of parameters to control the constraints\n\n"
        "The support surface is computed by deformation of the initial surface\n"
        "(or an automatically computed one) to satisfy the given constraints.\n\n"
        "Constraint orders:\n"
        "- GeomAbs_C0: Surface passes through the 3D representation\n"
        "- GeomAbs_G1: Surface passes through and respects tangency\n"
        "- GeomAbs_G2: Surface passes through and respects tangency and curvature\n\n"
        "Typical usage:\n"
        "  filling = BRepOffsetAPI.MakeFilling()\n"
        "  filling.add(edge1, GeomAbs.C0)  # Boundary constraint\n"
        "  filling.add(edge2, face, GeomAbs.G1)  # Tangent to face\n"
        "  filling.add(point)  # Point constraint\n"
        "  filling.build()\n"
        "  result = filling.shape()")
        
        // Constructor
        .def(py::init<const Standard_Integer, const Standard_Integer, const Standard_Integer,
                      const Standard_Boolean, const Standard_Real, const Standard_Real,
                      const Standard_Real, const Standard_Real, const Standard_Integer,
                      const Standard_Integer>(),
            py::arg("degree") = 3,
            py::arg("nb_pts_on_cur") = 15,
            py::arg("nb_iter") = 2,
            py::arg("anisotropie") = false,
            py::arg("tol_2d") = 0.00001,
            py::arg("tol_3d") = 0.0001,
            py::arg("tol_ang") = 0.01,
            py::arg("tol_curv") = 0.1,
            py::arg("max_deg") = 8,
            py::arg("max_segments") = 9,
            "Constructs a wire filling object.\n\n"
            "Parameters:\n"
            "  degree: Order of energy criterion to minimize (default: 3)\n"
            "          Recommended: i+2 where i is max order of constraints\n"
            "  nb_pts_on_cur: Average number of points for edge discretization (default: 15)\n"
            "  nb_iter: Maximum iterations, discretization points increase each iteration (default: 2)\n"
            "  anisotropie: Better performance when U/V length ratio is very different (default: False)\n"
            "  tol_2d: 2D tolerance (default: 1e-5)\n"
            "  tol_3d: Max distance between surface and constraints (default: 1e-4)\n"
            "  tol_ang: Max angle between surface normal and constraints (default: 0.01)\n"
            "  tol_curv: Max curvature difference between surface and constraints (default: 0.1)\n"
            "  max_deg: Highest polynomial degree for filling surface (default: 8)\n"
            "  max_segments: Greatest number of segments (default: 9)")
        
        // Parameter setting methods
        .def("set_constr_param", &BRepOffsetAPI_MakeFilling::SetConstrParam,
            py::arg("tol_2d") = 0.00001,
            py::arg("tol_3d") = 0.0001,
            py::arg("tol_ang") = 0.01,
            py::arg("tol_curv") = 0.1,
            "Sets tolerance values used to control constraints.\n\n"
            "Parameters:\n"
            "  tol_2d: 2D tolerance\n"
            "  tol_3d: Max distance between surface and constraints\n"
            "  tol_ang: Max angle between surface normal and constraints\n"
            "  tol_curv: Max curvature difference between surface and constraints")
        
        .def("set_resol_param", &BRepOffsetAPI_MakeFilling::SetResolParam,
            py::arg("degree") = 3,
            py::arg("nb_pts_on_cur") = 15,
            py::arg("nb_iter") = 2,
            py::arg("anisotropie") = false,
            "Sets parameters used for resolution.\n\n"
            "Parameters:\n"
            "  degree: Order of energy criterion to minimize\n"
            "  nb_pts_on_cur: Average number of points for edge discretization\n"
            "  nb_iter: Maximum number of iterations\n"
            "  anisotropie: Better performance for surfaces with extreme U/V ratio")
        
        .def("set_approx_param", &BRepOffsetAPI_MakeFilling::SetApproxParam,
            py::arg("max_deg") = 8,
            py::arg("max_segments") = 9,
            "Sets parameters used to approximate the filling surface.\n\n"
            "Parameters:\n"
            "  max_deg: Highest polynomial degree for the surface\n"
            "  max_segments: Greatest number of segments")
        
        // Load initial surface
        .def("load_init_surface", &BRepOffsetAPI_MakeFilling::LoadInitSurface,
            py::arg("surf"),
            "Loads an initial surface to begin construction.\n\n"
            "This is useful if the resulting surface is likely to be complex.\n"
            "The support surface is computed by deforming this surface to\n"
            "satisfy the given constraints.\n\n"
            "Important: The initial surface must have orthogonal local coordinates,\n"
            "i.e., partial derivatives dS/du and dS/dv must be orthogonal at each point.\n"
            "If this condition is not met, distortions may occur.\n\n"
            "If no initial surface is given, the algorithm computes one automatically.")
        
        // Add constraints - edge only
        .def("add",
            py::overload_cast<const TopoDS_Edge&, const GeomAbs_Shape, const Standard_Boolean>(
                &BRepOffsetAPI_MakeFilling::Add),
            py::arg("edge"), py::arg("order"), py::arg("is_bound") = true,
            "Adds an edge constraint which also defines a boundary edge.\n\n"
            "Parameters:\n"
            "  edge: The constraint edge\n"
            "  order: Continuity order (C0, G1, or G2)\n"
            "  is_bound: True if edge is a boundary of the face (default: True)\n\n"
            "Returns the index of the constraint.\n\n"
            "Raises ConstructionError if order is G1/G2 and edge has no face representation")
        
        // Add constraints - edge with support face
        .def("add",
            py::overload_cast<const TopoDS_Edge&, const TopoDS_Face&, const GeomAbs_Shape,
                              const Standard_Boolean>(&BRepOffsetAPI_MakeFilling::Add),
            py::arg("edge"), py::arg("support"), py::arg("order"), py::arg("is_bound") = true,
            "Adds an edge constraint with a support face for tangency/curvature.\n\n"
            "Parameters:\n"
            "  edge: The constraint edge\n"
            "  support: Face to respect tangency/curvature with\n"
            "  order: Continuity order (C0, G1, or G2)\n"
            "  is_bound: True if edge is a boundary of the face (default: True)\n\n"
            "Returns the index of the constraint.\n\n"
            "Raises ConstructionError if edge has no 2D representation on the support face")
        
        // Add constraints - free face constraint
        .def("add",
            py::overload_cast<const TopoDS_Face&, const GeomAbs_Shape>(
                &BRepOffsetAPI_MakeFilling::Add),
            py::arg("support"), py::arg("order"),
            "Adds a free constraint on a face.\n\n"
            "The corresponding edge is automatically computed. It is always a bound.\n\n"
            "Parameters:\n"
            "  support: The support face\n"
            "  order: Continuity order\n\n"
            "Returns the index of the constraint")
        
        // Add constraints - point
        .def("add",
            py::overload_cast<const gp_Pnt&>(&BRepOffsetAPI_MakeFilling::Add),
            py::arg("point"),
            "Adds a punctual constraint.\n\n"
            "The surface will pass through this point.\n\n"
            "Returns the index of the constraint")
        
        // Add constraints - point on face
        .def("add",
            py::overload_cast<const Standard_Real, const Standard_Real, const TopoDS_Face&,
                              const GeomAbs_Shape>(&BRepOffsetAPI_MakeFilling::Add),
            py::arg("u"), py::arg("v"), py::arg("support"), py::arg("order"),
            "Adds a punctual constraint at parametric coordinates on a face.\n\n"
            "Parameters:\n"
            "  u, v: Parametric coordinates on the support face\n"
            "  support: The support face\n"
            "  order: Continuity order at this point\n\n"
            "Returns the index of the constraint")
        
        // Build
        .def("build",
            [](BRepOffsetAPI_MakeFilling& self) {
                self.Build();
            },
            "Builds the resulting filling face.\n\n"
            "Computes the surface that satisfies all constraints and builds the face.\n"
            "Use is_done() to check success and shape() to retrieve the result")
        
        // Generated shapes
        .def("generated", &BRepOffsetAPI_MakeFilling::Generated,
            py::arg("shape"),
            py::return_value_policy::reference_internal,
            "Returns the list of shapes generated from the given shape")
        
        // Error properties (global)
        .def_property_readonly("g0_error",
            py::overload_cast<>(&BRepOffsetAPI_MakeFilling::G0Error, py::const_),
            "Maximum distance between the result and the constraints")
        
        .def_property_readonly("g1_error",
            py::overload_cast<>(&BRepOffsetAPI_MakeFilling::G1Error, py::const_),
            "Maximum angle (radians) between result normal and constraints")
        
        .def_property_readonly("g2_error",
            py::overload_cast<>(&BRepOffsetAPI_MakeFilling::G2Error, py::const_),
            "Maximum curvature difference between result and constraints")
        
        // Error methods (per constraint index)
        .def("g0_error_at", 
            py::overload_cast<const Standard_Integer>(&BRepOffsetAPI_MakeFilling::G0Error),
            py::arg("index"),
            "Returns maximum distance for constraint at given index")
        
        .def("g1_error_at",
            py::overload_cast<const Standard_Integer>(&BRepOffsetAPI_MakeFilling::G1Error),
            py::arg("index"),
            "Returns maximum angle for constraint at given index")
        
        .def("g2_error_at",
            py::overload_cast<const Standard_Integer>(&BRepOffsetAPI_MakeFilling::G2Error),
            py::arg("index"),
            "Returns maximum curvature difference for constraint at given index")
    ;


    py::class_<BRepOffsetAPI_MakeOffset, BRepBuilderAPI_MakeShape>(m, "MakeOffset",
        R"doc(
        Creates 2D offset curves from wires on a planar face.

        This algorithm offsets wires contained in a planar face by a specified
        distance, creating parallel curves. It handles corners using different
        join types and can produce open or closed results.

        The algorithm works with wires lying on a plane. For 3D solid offsets,
        use BRepOffsetAPI_MakeOffsetShape instead.

        Join types at salient vertices:
        - GeomAbs_Arc: Corners are filleted with circular arcs (default)
        - GeomAbs_Tangent: Corners are extended tangentially
        - GeomAbs_Intersection: Edges are extended until they intersect

        Typical usage:
          # From a planar face containing wires
          offset = BRepOffsetAPI.MakeOffset(face, GeomAbs.JoinType.Arc)
          offset.perform(5.0)  # Offset by 5 units
          result = offset.shape()

          # From a single wire
          offset = BRepOffsetAPI.MakeOffset(wire, GeomAbs.JoinType.Arc)
          offset.perform(-2.0)  # Negative offset (inward)
          result = offset.shape()

          # Multiple wires
          offset = BRepOffsetAPI.MakeOffset()
          offset.init(GeomAbs.JoinType.Arc)
          offset.add_wire(wire1)
          offset.add_wire(wire2)
          offset.perform(3.0)
          result = offset.shape()
        )doc")
        
        // Default constructor
        .def(py::init<>(),
            "Creates an empty offset algorithm.\n\n"
            "Use init() to initialize and add_wire() to add wires before calling perform().")
        
        // Constructor from face
        .def(py::init<const TopoDS_Face&, const GeomAbs_JoinType, const Standard_Boolean>(),
            py::arg("spine"),
            py::arg("join") = GeomAbs_Arc,
            py::arg("is_open_result") = false,
            R"doc(
            Creates an offset algorithm from a planar face.

            All wires contained in the face will be offset.

            Parameters:
              spine: Planar face containing the wires to offset
              join: Corner treatment type (default: Arc)
                - Arc: Filleted corners with circular arcs
                - Tangent: Extended tangentially
                - Intersection: Extended until intersection
              is_open_result: If True, allows open wire results (default: False)
            )doc")
        
        // Constructor from wire
        .def(py::init<const TopoDS_Wire&, const GeomAbs_JoinType, const Standard_Boolean>(),
            py::arg("spine"),
            py::arg("join") = GeomAbs_Arc,
            py::arg("is_open_result") = false,
            R"doc(
            Creates an offset algorithm from a single wire.

            The wire should be planar.

            Parameters:
              spine: Wire to offset (should be planar)
              join: Corner treatment type (default: Arc)
              is_open_result: If True, allows open wire results (default: False)
            )doc")
        
        // Init from face
        .def("init",
            py::overload_cast<const TopoDS_Face&, const GeomAbs_JoinType, const Standard_Boolean>(
                &BRepOffsetAPI_MakeOffset::Init),
            py::arg("spine"),
            py::arg("join") = GeomAbs_Arc,
            py::arg("is_open_result") = false,
            R"doc(
            Initializes the algorithm with a planar face.

            Parameters:
              spine: Planar face containing the wires to offset
              join: Corner treatment type (default: Arc)
              is_open_result: If True, allows open wire results (default: False)
            )doc")
        
        // Init without face (for adding wires manually)
        .def("init",
            py::overload_cast<const GeomAbs_JoinType, const Standard_Boolean>(
                &BRepOffsetAPI_MakeOffset::Init),
            py::arg("join") = GeomAbs_Arc,
            py::arg("is_open_result") = false,
            R"doc(
            Initializes the algorithm for manual wire addition.

            Use add_wire() to add wires after calling this method.

            Parameters:
              join: Corner treatment type (default: Arc)
              is_open_result: If True, allows open wire results (default: False)
            )doc")
        
        // Set approximation
        .def("set_approx", &BRepOffsetAPI_MakeOffset::SetApprox,
            py::arg("to_approx"),
            R"doc(
            Sets the approximation flag.

            When enabled, input contours are converted to curves consisting
            only of 2D circular arcs and 2D linear segments. This can improve
            performance and stability for complex input curves.

            Parameters:
              to_approx: True to enable approximation of input contours
            )doc")
        
        // Add wire
        .def("add_wire", &BRepOffsetAPI_MakeOffset::AddWire,
            py::arg("spine"),
            R"doc(
            Adds a wire to be offset.

            Multiple wires can be added. All wires should lie in the same plane.
            Call init() before adding wires if not using the face constructor.

            Parameters:
              spine: Wire to add for offsetting
            )doc")
        
        // Perform offset
        .def("perform", &BRepOffsetAPI_MakeOffset::Perform,
            py::arg("offset"),
            py::arg("alt") = 0.0,
            R"doc(
            Computes the offset at the specified distance.

            Positive offset values expand outward, negative values contract inward.
            The alt parameter allows creating the offset at a different altitude
            from the original plane.

            Parameters:
              offset: Distance to offset (positive=outward, negative=inward)
              alt: Altitude from the plane of the spine (default: 0.0)

            Raises:
              StdFail_NotDone: If the offset cannot be computed
            )doc")
        
        // Build
        .def("build",
            [](BRepOffsetAPI_MakeOffset& self) {
                self.Build();
            },
            "Builds the resulting offset shape.\n\n"
            "This is called automatically by perform(), but can be called\n"
            "explicitly if needed. Use is_done() to check success.")
        
        // Generated shapes
        .def("generated", [](BRepOffsetAPI_MakeOffset& self, const TopoDS_Shape& shape) {
                TopTools_ListOfShape result = self.Generated(shape);
                py::list py_result;
                for (TopTools_ListIteratorOfListOfShape it(result); it.More(); it.Next()) {
                    py_result.append(it.Value());
                }
                return py_result;
            },
            py::arg("shape"),
            R"doc(
            Returns shapes generated from an input shape.

            Parameters:
              shape: A subshape of the original wire/face

            Returns:
              List of shapes created from the input shape
            )doc")
        
        // Static method to convert face
        .def_static("convert_face", &BRepOffsetAPI_MakeOffset::ConvertFace,
            py::arg("face"),
            py::arg("angle_tolerance"),
            R"doc(
            Converts a face's wires to arcs and segments only.

            This utility method converts each wire of the face into contours
            consisting only of circular arcs and linear segments. New 3D curves
            are built as well. This can be useful for preprocessing complex
            faces before offsetting.

            Parameters:
              face: The face to convert
              angle_tolerance: Angular tolerance for the conversion

            Returns:
              A new face with simplified wire geometry
            )doc");
    
    // ==================== MakeOffsetShape ====================
    py::class_<BRepOffsetAPI_MakeOffsetShape, BRepBuilderAPI_MakeShape>(m, "MakeOffsetShape",
        R"doc(
        Creates 3D offset shapes (shells/solids) from a shape.

        This algorithm builds a shell or solid parallel to the source shape
        at a specified offset distance. It handles faces, shells, solids, and
        compounds of these shapes.

        The offset shape is constructed:
        - Outside the shape if offset is positive
        - Inside the shape if offset is negative

        Two algorithms are available:
        - PerformByJoin: Full algorithm with intersection computation (default)
        - PerformBySimple: Simple algorithm without intersection computation

        Join types at edges:
        - GeomAbs_Arc: Pipes between edges, spheres at vertices (default)
        - GeomAbs_Intersection: Adjacent faces enlarged and intersected

        Requirements:
        - All faces must have C1 continuous underlying surfaces
        - Offset value should be small enough to avoid self-intersections
        - Vertices with more than 3 converging edges may cause failures
        - BSpline surfaces must have continuity > C0

        Typical usage:
          # Simple offset
          offset = BRepOffsetAPI.MakeOffsetShape()
          offset.perform_by_join(solid, 2.0, 1e-3)
          result = offset.shape()

          # With options
          offset = BRepOffsetAPI.MakeOffsetShape()
          offset.perform_by_join(solid, -1.0, 1e-3,
                                 mode=BRepOffsetAPI.OffsetMode.Skin,
                                 join=GeomAbs.JoinType.Arc)
          result = offset.shape()
        )doc")
        
        // Default constructor
        .def(py::init<>(),
            "Creates an empty offset shape algorithm.\n\n"
            "Use perform_by_join() or perform_by_simple() to compute the offset.")
        
        // PerformBySimple
        .def("perform_by_simple", &BRepOffsetAPI_MakeOffsetShape::PerformBySimple,
            py::arg("shape"),
            py::arg("offset_value"),
            R"doc(
            Computes offset using a simple algorithm without intersection computation.

            This is faster but may produce self-intersecting results for complex
            shapes or large offset values.

            Parameters:
              shape: The shape to offset (face, shell, solid, or compound)
              offset_value: Offset distance (positive=outward, negative=inward)
            )doc")
        
        // PerformByJoin - main algorithm
        .def("perform_by_join",
            [](BRepOffsetAPI_MakeOffsetShape& self,
               const TopoDS_Shape& shape,
               Standard_Real offset,
               Standard_Real tol,
               BRepOffset_Mode mode,
               Standard_Boolean intersection,
               Standard_Boolean selfInter,
               GeomAbs_JoinType join,
               Standard_Boolean removeIntEdges) {
                self.PerformByJoin(shape, offset, tol, mode, intersection,
                                   selfInter, join, removeIntEdges);
            },
            py::arg("shape"),
            py::arg("offset"),
            py::arg("tol"),
            py::arg("mode") = BRepOffset_Skin,
            py::arg("intersection") = false,
            py::arg("self_inter") = false,
            py::arg("join") = GeomAbs_Arc,
            py::arg("remove_internal_edges") = false,
            R"doc(
            Computes offset using the full algorithm with intersection computation.

            This is the recommended method for most use cases. It handles
            self-intersections and produces clean results.

            Parameters:
              shape: The shape to offset (face, shell, solid, or compound)
              offset: Offset distance (positive=outward, negative=inward)
              tol: Coincidence tolerance for generated shapes
              mode: Construction mode (default: Skin)
                - Skin: Offset along solid surface
                - Pipe: Offset of a curve
                - RectoVerso: Offset on both sides
              intersection: If True, compute intersections with all parallels
                            (more general but not fully implemented, default: False)
              self_inter: If True, eliminate self-intersections
                          (not yet implemented, use False)
              join: How to fill holes between adjacent faces (default: Arc)
                - Arc: Pipes between edges, spheres at vertices
                - Intersection: Enlarge and intersect adjacent faces
              remove_internal_edges: If True, remove internal edges from result
                                     (default: False)

            Warnings:
              - Faces must have C1 continuous underlying surfaces
              - Offset should be small enough to avoid self-intersections
              - Vertices with >3 converging edges may cause failures
            )doc")
        
        // GetJoinType - property
        .def_property_readonly("join_type", &BRepOffsetAPI_MakeOffsetShape::GetJoinType,
            "Returns the join type used for the offset computation.");
    
    // ==================== MiddlePath ====================
    py::class_<BRepOffsetAPI_MiddlePath, BRepBuilderAPI_MakeShape>(m, "MiddlePath",
        R"doc(
        Computes the middle path of a pipe-like shape.

        This algorithm extracts a wire representing the middle path (spine)
        of a pipe-shaped solid or shell. The pipe must have identifiable
        start and end sections.

        The algorithm works by:
        1. Identifying edges on the start and end sections
        2. Finding corresponding paths through the pipe
        3. Computing the middle of those paths

        Requirements:
        - The shape must be pipe-like (constant section along a path)
        - Start and end shapes must be faces or wires on the pipe boundaries

        Typical usage:
          # Extract middle path from a pipe
          pipe = make_pipe(...)  # A pipe-shaped solid
          start_face = ...  # Face at one end
          end_face = ...    # Face at the other end
          
          middle = BRepOffsetAPI.MiddlePath(pipe, start_face, end_face)
          middle.build()
          path_wire = middle.shape()
        )doc")
        
        .def(py::init<const TopoDS_Shape&, const TopoDS_Shape&, const TopoDS_Shape&>(),
            py::arg("shape"),
            py::arg("start_shape"),
            py::arg("end_shape"),
            R"doc(
            Constructs a middle path algorithm.

            Parameters:
              shape: The pipe-like shape to analyze
              start_shape: A wire or face at one end of the pipe
              end_shape: A wire or face at the other end of the pipe
            )doc")
        
        .def("build", [](BRepOffsetAPI_MiddlePath& self) {
                self.Build();
            },
            "Computes the middle path. Check is_done() for success.");
    
    // ==================== NormalProjection ====================
    py::class_<BRepOffsetAPI_NormalProjection, BRepBuilderAPI_MakeShape>(m, "NormalProjection",
        R"doc(
        Projects edges/wires onto a shape along the normal direction.

        This algorithm projects edges or wires onto a target shape (typically
        a face or shell) by projecting each point along the normal to the
        target surface at that point.

        The projection produces new edges that lie on the target surface,
        with associated 2D curves (pcurves) on the surface.

        Features:
        - Can project multiple edges/wires in one operation
        - Results are built as oriented wires when possible
        - Optional 3D curve computation (vs. keeping original curves)
        - Configurable approximation parameters

        Typical usage:
          # Project a wire onto a surface
          proj = BRepOffsetAPI.NormalProjection(target_face)
          proj.add(wire_to_project)
          proj.build()
          projected = proj.shape()
        )doc")
        
        // Default constructor
        .def(py::init<>(),
            "Creates an empty normal projection algorithm.\n\n"
            "Use init() to set the target shape and add() to add shapes to project.")
        
        // Constructor with target shape
        .def(py::init<const TopoDS_Shape&>(),
            py::arg("target"),
            R"doc(
            Creates a normal projection algorithm with a target shape.

            Parameters:
              target: The shape to project onto (typically a face or shell)

            Default parameters:
              Tol3D = 1e-4, Tol2D = sqrt(Tol3D)
              InternalContinuity = C2
              MaxDegree = 14, MaxSeg = 16
            )doc")
        
        // Init
        .def("init", &BRepOffsetAPI_NormalProjection::Init,
            py::arg("target"),
            "Initializes with a target shape to project onto.")
        
        // Add
        .def("add", &BRepOffsetAPI_NormalProjection::Add,
            py::arg("to_project"),
            R"doc(
            Adds a shape to be projected onto the target.

            Parameters:
              to_project: An edge or wire to project

            Raises:
              Standard_ConstructionError: If the shape cannot be added
            )doc")
        
        // SetParams
        .def("set_params", &BRepOffsetAPI_NormalProjection::SetParams,
            py::arg("tol_3d"),
            py::arg("tol_2d"),
            py::arg("continuity"),
            py::arg("max_degree"),
            py::arg("max_seg"),
            R"doc(
            Sets approximation parameters.

            Parameters:
              tol_3d: Required tolerance between 3D curve and 2D representation
              tol_2d: 2D tolerance
              continuity: Order of continuity for approximation (C0, C1, C2, etc.)
              max_degree: Maximum degree for BSpline approximation
              max_seg: Maximum number of segments for BSpline approximation
            )doc")
        
        // SetMaxDistance
        .def("set_max_distance", &BRepOffsetAPI_NormalProjection::SetMaxDistance,
            py::arg("max_dist"),
            R"doc(
            Sets maximum distance between target and source shape.

            If the distance exceeds this value, that part of the projection
            is discarded. Use negative value to disable this check.

            Parameters:
              max_dist: Maximum allowed distance (< 0 to disable)
            )doc")
        
        // SetLimit
        .def("set_limit", &BRepOffsetAPI_NormalProjection::SetLimit,
            py::arg("face_boundaries") = true,
            R"doc(
            Controls limitation of projected edges to face boundaries.

            Parameters:
              face_boundaries: If True, limit projection to face boundaries
            )doc")
        
        // Compute3d
        .def("compute_3d", &BRepOffsetAPI_NormalProjection::Compute3d,
            py::arg("with_3d") = true,
            R"doc(
            Sets whether to compute new 3D curves.

            Parameters:
              with_3d: If True, compute new 3D curves;
                       If False, keep original curves for result edges
            )doc")
        
        // Build
        .def("build", [](BRepOffsetAPI_NormalProjection& self) {
                self.Build();
            },
            "Builds the projection result as a compound of wires.")
        
        // Projection
        .def("projection", &BRepOffsetAPI_NormalProjection::Projection,
            py::return_value_policy::reference,
            "Returns the projection result (same as shape()).")
        
        // Couple
        .def("couple", &BRepOffsetAPI_NormalProjection::Couple,
            py::arg("edge"),
            py::return_value_policy::reference,
            R"doc(
            Returns the target face corresponding to a projected edge.

            Parameters:
              edge: A projected edge from the result

            Returns:
              The face on which the edge was projected
            )doc")
        
        // Ancestor
        .def("ancestor", &BRepOffsetAPI_NormalProjection::Ancestor,
            py::arg("edge"),
            py::return_value_policy::reference,
            R"doc(
            Returns the original edge corresponding to a projected edge.

            Parameters:
              edge: A projected edge from the result

            Returns:
              The original edge that was projected
            )doc")
        
        // BuildWire
        .def("build_wire", [](BRepOffsetAPI_NormalProjection& self) {
                TopTools_ListOfShape liste;
                Standard_Boolean result = self.BuildWire(liste);
                py::list py_result;
                for (TopTools_ListIteratorOfListOfShape it(liste); it.More(); it.Next()) {
                    py_result.append(it.Value());
                }
                return py::make_tuple(result, py_result);
            },
            R"doc(
            Builds the result as a list of wires if possible.

            Returns:
              Tuple of (success, list_of_wires)
              success is True only if there is exactly one wire
            )doc");
    
    // ==================== ThruSections Error Status ====================
    py::enum_<BRepFill_ThruSectionErrorStatus>(m, "ThruSectionStatus",
        "Status codes for ThruSections algorithm.")
        .value("Done", BRepFill_ThruSectionErrorStatus_Done,
            "Operation completed successfully")
        .value("NotDone", BRepFill_ThruSectionErrorStatus_NotDone,
            "Operation not completed")
        .value("NotSameTopology", BRepFill_ThruSectionErrorStatus_NotSameTopology,
            "Profiles have different topology (all must be closed or all open)")
        .value("ProfilesInconsistent", BRepFill_ThruSectionErrorStatus_ProfilesInconsistent,
            "Profiles are inconsistent")
        .value("WrongUsage", BRepFill_ThruSectionErrorStatus_WrongUsage,
            "Wrong usage of punctual sections")
        .value("Null3DCurve", BRepFill_ThruSectionErrorStatus_Null3DCurve,
            "Null 3D curve in edge")
        .value("Failed", BRepFill_ThruSectionErrorStatus_Failed,
            "Algorithm has failed")
        .export_values();
    
    // ==================== Approx_ParametrizationType ====================
    py::enum_<Approx_ParametrizationType>(m, "ParametrizationType",
        "Parametrization types for curve/surface approximation.")
        .value("ChordLength", Approx_ChordLength, "Parametrization by chord length")
        .value("Centripetal", Approx_Centripetal, "Centripetal parametrization")
        .value("IsoParametric", Approx_IsoParametric, "Iso-parametric (uniform) parametrization")
        .export_values();
    
    // ==================== ThruSections ====================
    py::class_<BRepOffsetAPI_ThruSections, BRepBuilderAPI_MakeShape>(m, "ThruSections",
        R"doc(
        Creates a shell or solid by lofting through a set of wire sections.

        This algorithm builds a shape by connecting a sequence of wire profiles.
        The wires are connected either by ruled surfaces (straight lines between
        corresponding points) or by smooth approximated surfaces.

        Features:
        - Can create shells (open) or solids (closed with end caps)
        - First and last sections can be vertices (points)
        - Optional smoothing with configurable continuity
        - Automatic wire compatibility checking and adjustment

        Requirements:
        - At least one wire section must be provided
        - Wires should have compatible orientations
        - Vertices can only be first or last sections

        Typical usage:
          # Create a lofted solid through three circles
          loft = BRepOffsetAPI.ThruSections(is_solid=True)
          loft.add_wire(circle1)
          loft.add_wire(circle2)
          loft.add_wire(circle3)
          loft.build()
          result = loft.shape()

          # Loft from point to circle
          loft = BRepOffsetAPI.ThruSections(is_solid=True)
          loft.add_vertex(apex_vertex)
          loft.add_wire(base_circle)
          loft.build()
          cone = loft.shape()
        )doc")
        
        // Constructor
        .def(py::init<Standard_Boolean, Standard_Boolean, Standard_Real>(),
            py::arg("is_solid") = false,
            py::arg("ruled") = false,
            py::arg("pres3d") = 1.0e-6,
            R"doc(
            Creates a ThruSections algorithm.

            Parameters:
              is_solid: If True, create a solid with end caps;
                        If False, create an open shell (default)
              ruled: If True, create ruled surfaces between sections;
                     If False, create smooth approximated surfaces (default)
              pres3d: Precision for 3D approximation (default: 1e-6)
            )doc")
        
        // Init
        .def("init", &BRepOffsetAPI_ThruSections::Init,
            py::arg("is_solid") = false,
            py::arg("ruled") = false,
            py::arg("pres3d") = 1.0e-6,
            "Reinitializes the algorithm with new parameters.")
        
        // AddWire
        .def("add_wire", &BRepOffsetAPI_ThruSections::AddWire,
            py::arg("wire"),
            R"doc(
            Adds a wire section to the loft.

            Wires are processed in the order they are added.
            Use build() after adding all sections.

            Parameters:
              wire: A wire representing a cross-section
            )doc")
        
        // AddVertex
        .def("add_vertex", &BRepOffsetAPI_ThruSections::AddVertex,
            py::arg("vertex"),
            R"doc(
            Adds a vertex (point) section to the loft.

            A vertex can only be added as the first or last section.
            This creates a pointed end (like a cone apex).

            Parameters:
              vertex: A vertex representing a point section
            )doc")
        
        // CheckCompatibility
        .def("check_compatibility", &BRepOffsetAPI_ThruSections::CheckCompatibility,
            py::arg("check") = true,
            R"doc(
            Enables/disables wire compatibility checking.

            When enabled, the algorithm adjusts wires to have the same
            number of edges and consistent orientations to avoid twisted results.

            Parameters:
              check: If True, enable compatibility checking (default)
            )doc")
        
        // SetSmoothing
        .def("set_smoothing", &BRepOffsetAPI_ThruSections::SetSmoothing,
            py::arg("use_smoothing"),
            "Enables/disables smoothing in the approximation algorithm.")
        
        // SetParType
        .def("set_par_type", &BRepOffsetAPI_ThruSections::SetParType,
            py::arg("par_type"),
            R"doc(
            Sets the parametrization type for approximation.

            Parameters:
              par_type: ChordLength, Centripetal, or IsoParametric
            )doc")
        
        // SetContinuity
        .def("set_continuity", &BRepOffsetAPI_ThruSections::SetContinuity,
            py::arg("continuity"),
            R"doc(
            Sets the continuity for the approximation.

            Parameters:
              continuity: Geometric continuity (C0, C1, C2, etc.)
            )doc")
        
        // SetCriteriumWeight
        .def("set_criterium_weight", &BRepOffsetAPI_ThruSections::SetCriteriumWeight,
            py::arg("w1"),
            py::arg("w2"),
            py::arg("w3"),
            R"doc(
            Sets weights for optimization criteria.

            Each weight must be positive (> 0).

            Parameters:
              w1: Weight for first criterion
              w2: Weight for second criterion
              w3: Weight for third criterion
            )doc")
        
        // SetMaxDegree
        .def("set_max_degree", &BRepOffsetAPI_ThruSections::SetMaxDegree,
            py::arg("max_deg"),
            "Sets the maximum U degree for the result surface.")
        
        // SetMutableInput
        .def("set_mutable_input", &BRepOffsetAPI_ThruSections::SetMutableInput,
            py::arg("is_mutable"),
            R"doc(
            Sets whether input profiles can be modified.

            Parameters:
              is_mutable: If True, input wires may be modified (default);
                          If False, input wires are preserved
            )doc")
        
        // Properties (read-only getters)
        .def_property_readonly("par_type", &BRepOffsetAPI_ThruSections::ParType,
            "Returns the parametrization type used.")
        
        .def_property_readonly("continuity", &BRepOffsetAPI_ThruSections::Continuity,
            "Returns the continuity used in approximation.")
        
        .def_property_readonly("max_degree", &BRepOffsetAPI_ThruSections::MaxDegree,
            "Returns the maximum U degree of result surface.")
        
        .def_property_readonly("use_smoothing", &BRepOffsetAPI_ThruSections::UseSmoothing,
            "Returns True if smoothing is enabled.")
        
        .def_property_readonly("is_mutable_input", &BRepOffsetAPI_ThruSections::IsMutableInput,
            "Returns True if input profiles can be modified.")
        
        .def_property_readonly("status", &BRepOffsetAPI_ThruSections::GetStatus,
            "Returns the status of the thrusections operation.")
        
        // CriteriumWeight
        .def("criterium_weight", [](BRepOffsetAPI_ThruSections& self) {
                Standard_Real w1, w2, w3;
                self.CriteriumWeight(w1, w2, w3);
                return py::make_tuple(w1, w2, w3);
            },
            "Returns the criterion weights as a tuple (w1, w2, w3).")
        
        // Build
        .def("build", [](BRepOffsetAPI_ThruSections& self) {
                self.Build();
            },
            "Builds the lofted shape. Check is_done() for success.")
        
        // FirstShape / LastShape
        .def("first_shape", &BRepOffsetAPI_ThruSections::FirstShape,
            py::return_value_policy::reference,
            "Returns the bottom face of the loft (if solid).")
        
        .def("last_shape", &BRepOffsetAPI_ThruSections::LastShape,
            py::return_value_policy::reference,
            "Returns the top face of the loft (if solid).")
        
        // GeneratedFace
        .def("generated_face", &BRepOffsetAPI_ThruSections::GeneratedFace,
            py::arg("edge"),
            R"doc(
            Returns the face generated by an edge.

            For ruled lofts: returns face generated by edge (except last wire).
            For smoothed lofts: returns face generated by edge of first wire.

            Parameters:
              edge: An edge from one of the input wires

            Returns:
              The face generated by the edge
            )doc")
        
        // Wires
        .def("wires", [](BRepOffsetAPI_ThruSections& self) {
                const TopTools_ListOfShape& wires = self.Wires();
                py::list result;
                for (TopTools_ListIteratorOfListOfShape it(wires); it.More(); it.Next()) {
                    result.append(it.Value());
                }
                return result;
            },
            "Returns the list of original input wires.");
}
