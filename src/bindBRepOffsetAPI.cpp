#include <pybind11/pybind11.h>

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
#include <TopoDS_Vertex.hxx>
#include <Law_Function.hxx>
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>

namespace py = pybind11;

void bind_brep_offset_api(py::module_ &m)
{

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
}