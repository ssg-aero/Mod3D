#include <gp.hxx>
#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>
#include <gp_Vec.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Lin.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Hypr.hxx>
#include <gp_Parab.hxx>
#include <gp_Cylinder.hxx>
#include <gp_Cone.hxx>
#include <gp_Sphere.hxx>
#include <gp_Torus.hxx>
#include <gp_Pln.hxx>
#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>
#include <Geom_Curve.hxx>
#include <Geom_Surface.hxx>
#include <TopTools_ListOfShape.hxx>
#include <BRepBuilderAPI_Command.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepTools_ReShape.hxx>
#include <BRepBuilderAPI_Sewing.hxx>

#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>

#include "array_utils.hpp"

namespace py = pybind11;

// Declare opencascade::handle as a holder type for pybind11
// Required to process opencascade::handle<T> as arguments/return types
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);


void bind_brep_prim_api(py::module_ &m);

void bind_brepbuilder_api(py::module_ &m)
{

    py::class_<BRepBuilderAPI_Command>(m, "Command")
        .def("is_done", &BRepBuilderAPI_Command::IsDone)
        .def("check", &BRepBuilderAPI_Command::Check)
    ;

    py::class_<BRepBuilderAPI_MakeShape, BRepBuilderAPI_Command>(m, "MakeShape")
        .def("build", [](BRepBuilderAPI_MakeShape& self) { self.Build(); }) // TODO add progess indicator?
        .def("shape", &BRepBuilderAPI_MakeShape::Shape)
        .def("generated", [](BRepBuilderAPI_MakeShape& self, const TopoDS_Shape &theOriginal) {
            return occt_container_to_py_list(self.Generated(theOriginal));
        })
        .def("modified", [](BRepBuilderAPI_MakeShape& self, const TopoDS_Shape &theOriginal) {
            return occt_container_to_py_list(self.Modified(theOriginal));
        })
        .def("deleted", [](BRepBuilderAPI_MakeShape& self, const TopoDS_Shape &theOriginal) {
            return self.IsDeleted(theOriginal);
        })
    ;

    py::class_<BRepBuilderAPI_MakeVertex, BRepBuilderAPI_MakeShape>(m, "MakeVertex")
        .def(py::init<const gp_Pnt&>(), py::arg("Pnt"))
        .def(py::init([](double x, double y, double z){
            return BRepBuilderAPI_MakeVertex(gp_Pnt(x, y, z));
            }), 
            py::arg("x"), py::arg("y"), py::arg("z")
        )
        .def("vertex", &BRepBuilderAPI_MakeVertex::Vertex)
    ;

    py::enum_<BRepBuilderAPI_EdgeError>(m, "EdgeError")
        .value("EdgeDone", BRepBuilderAPI_EdgeDone)
        .value("PointProjectionFailed", BRepBuilderAPI_PointProjectionFailed)
        .value("ParameterOutOfRange", BRepBuilderAPI_ParameterOutOfRange)
        .value("DifferentPointsOnClosedCurve", BRepBuilderAPI_DifferentPointsOnClosedCurve)
        .value("PointWithInfiniteParameter", BRepBuilderAPI_PointWithInfiniteParameter)
        .value("DifferentsPointAndParameter", BRepBuilderAPI_DifferentsPointAndParameter)
        .value("LineThroughIdenticPoints", BRepBuilderAPI_LineThroughIdenticPoints)
        .export_values();


    py::class_<BRepBuilderAPI_MakeEdge, BRepBuilderAPI_MakeShape>(m, "MakeEdge",
        "Provides methods to build edges.\n"
        "An edge can be created from:\n"
        "- Two vertices or two points\n"
        "- A curve (gp_Lin, gp_Circ, gp_Elips, gp_Hypr, gp_Parab, or Geom_Curve)\n"
        "- A curve with parameter bounds or vertex/point bounds\n"
        "- A 2D curve on a surface")
        
        // Empty constructor
        .def(py::init<>(), "Creates an empty edge")
        
        // From vertices
        .def(py::init<const TopoDS_Vertex&, const TopoDS_Vertex&>(), 
             py::arg("V1"), py::arg("V2"),
             "Creates a straight edge between two vertices")
        
        // From points
        .def(py::init<const gp_Pnt&, const gp_Pnt&>(), 
             py::arg("P1"), py::arg("P2"),
             "Creates a straight edge between two points")
        
        // From gp_Lin (line)
        .def(py::init<const gp_Lin&>(), py::arg("L"),
             "Creates an infinite edge on the line L")
        .def(py::init<const gp_Lin&, const double, const double>(),
             py::arg("L"), py::arg("p1"), py::arg("p2"),
             "Creates an edge on the line L between parameters p1 and p2")
        .def(py::init<const gp_Lin&, const gp_Pnt&, const gp_Pnt&>(),
             py::arg("L"), py::arg("P1"), py::arg("P2"),
             "Creates an edge on the line L between points P1 and P2")
        .def(py::init<const gp_Lin&, const TopoDS_Vertex&, const TopoDS_Vertex&>(),
             py::arg("L"), py::arg("V1"), py::arg("V2"),
             "Creates an edge on the line L between vertices V1 and V2")
        
        // From gp_Circ (circle)
        .def(py::init<const gp_Circ&>(), py::arg("L"),
             "Creates a closed edge on the circle L")
        .def(py::init<const gp_Circ&, const double, const double>(),
             py::arg("L"), py::arg("p1"), py::arg("p2"),
             "Creates an edge on the circle L between parameters p1 and p2")
        .def(py::init<const gp_Circ&, const gp_Pnt&, const gp_Pnt&>(),
             py::arg("L"), py::arg("P1"), py::arg("P2"),
             "Creates an edge on the circle L between points P1 and P2")
        .def(py::init<const gp_Circ&, const TopoDS_Vertex&, const TopoDS_Vertex&>(),
             py::arg("L"), py::arg("V1"), py::arg("V2"),
             "Creates an edge on the circle L between vertices V1 and V2")
        
        // From gp_Elips (ellipse)
        .def(py::init<const gp_Elips&>(), py::arg("L"),
             "Creates a closed edge on the ellipse L")
        .def(py::init<const gp_Elips&, const double, const double>(),
             py::arg("L"), py::arg("p1"), py::arg("p2"),
             "Creates an edge on the ellipse L between parameters p1 and p2")
        .def(py::init<const gp_Elips&, const gp_Pnt&, const gp_Pnt&>(),
             py::arg("L"), py::arg("P1"), py::arg("P2"),
             "Creates an edge on the ellipse L between points P1 and P2")
        .def(py::init<const gp_Elips&, const TopoDS_Vertex&, const TopoDS_Vertex&>(),
             py::arg("L"), py::arg("V1"), py::arg("V2"),
             "Creates an edge on the ellipse L between vertices V1 and V2")
        
        // From gp_Hypr (hyperbola)
        .def(py::init<const gp_Hypr&>(), py::arg("L"),
             "Creates an infinite edge on the hyperbola L")
        .def(py::init<const gp_Hypr&, const double, const double>(),
             py::arg("L"), py::arg("p1"), py::arg("p2"),
             "Creates an edge on the hyperbola L between parameters p1 and p2")
        .def(py::init<const gp_Hypr&, const gp_Pnt&, const gp_Pnt&>(),
             py::arg("L"), py::arg("P1"), py::arg("P2"),
             "Creates an edge on the hyperbola L between points P1 and P2")
        .def(py::init<const gp_Hypr&, const TopoDS_Vertex&, const TopoDS_Vertex&>(),
             py::arg("L"), py::arg("V1"), py::arg("V2"),
             "Creates an edge on the hyperbola L between vertices V1 and V2")
        
        // From gp_Parab (parabola)
        .def(py::init<const gp_Parab&>(), py::arg("L"),
             "Creates an infinite edge on the parabola L")
        .def(py::init<const gp_Parab&, const double, const double>(),
             py::arg("L"), py::arg("p1"), py::arg("p2"),
             "Creates an edge on the parabola L between parameters p1 and p2")
        .def(py::init<const gp_Parab&, const gp_Pnt&, const gp_Pnt&>(),
             py::arg("L"), py::arg("P1"), py::arg("P2"),
             "Creates an edge on the parabola L between points P1 and P2")
        .def(py::init<const gp_Parab&, const TopoDS_Vertex&, const TopoDS_Vertex&>(),
             py::arg("L"), py::arg("V1"), py::arg("V2"),
             "Creates an edge on the parabola L between vertices V1 and V2")
        
        // From Geom_Curve
        .def(py::init<const opencascade::handle<Geom_Curve>&>(), py::arg("L"),
             "Creates an edge on the curve L with natural bounds")
        .def(py::init<const opencascade::handle<Geom_Curve>&, const double, const double>(),
             py::arg("L"), py::arg("p1"), py::arg("p2"),
             "Creates an edge on the curve L between parameters p1 and p2")
        .def(py::init<const opencascade::handle<Geom_Curve>&, const gp_Pnt&, const gp_Pnt&>(),
             py::arg("L"), py::arg("P1"), py::arg("P2"),
             "Creates an edge on the curve L between points P1 and P2")
        .def(py::init<const opencascade::handle<Geom_Curve>&, const TopoDS_Vertex&, const TopoDS_Vertex&>(),
             py::arg("L"), py::arg("V1"), py::arg("V2"),
             "Creates an edge on the curve L between vertices V1 and V2")
        .def(py::init<const opencascade::handle<Geom_Curve>&, const gp_Pnt&, const gp_Pnt&, const double, const double>(),
             py::arg("L"), py::arg("P1"), py::arg("P2"), py::arg("p1"), py::arg("p2"),
             "Creates an edge on the curve L between points P1, P2 with parameters p1, p2")
        .def(py::init<const opencascade::handle<Geom_Curve>&, const TopoDS_Vertex&, const TopoDS_Vertex&, const double, const double>(),
             py::arg("L"), py::arg("V1"), py::arg("V2"), py::arg("p1"), py::arg("p2"),
             "Creates an edge on the curve L between vertices V1, V2 with parameters p1, p2")
        
        // Init methods for reinitialization
        .def("init", py::overload_cast<const opencascade::handle<Geom_Curve>&>(&BRepBuilderAPI_MakeEdge::Init),
             py::arg("C"), "Reinitializes with curve C")
        .def("init", py::overload_cast<const opencascade::handle<Geom_Curve>&, const double, const double>(&BRepBuilderAPI_MakeEdge::Init),
             py::arg("C"), py::arg("p1"), py::arg("p2"),
             "Reinitializes with curve C between parameters p1 and p2")
        .def("init", py::overload_cast<const opencascade::handle<Geom_Curve>&, const gp_Pnt&, const gp_Pnt&>(&BRepBuilderAPI_MakeEdge::Init),
             py::arg("C"), py::arg("P1"), py::arg("P2"),
             "Reinitializes with curve C between points P1 and P2")
        .def("init", py::overload_cast<const opencascade::handle<Geom_Curve>&, const TopoDS_Vertex&, const TopoDS_Vertex&>(&BRepBuilderAPI_MakeEdge::Init),
             py::arg("C"), py::arg("V1"), py::arg("V2"),
             "Reinitializes with curve C between vertices V1 and V2")
        .def("init", py::overload_cast<const opencascade::handle<Geom_Curve>&, const gp_Pnt&, const gp_Pnt&, const double, const double>(&BRepBuilderAPI_MakeEdge::Init),
             py::arg("C"), py::arg("P1"), py::arg("P2"), py::arg("p1"), py::arg("p2"),
             "Reinitializes with curve C between points P1, P2 with parameters p1, p2")
        .def("init", py::overload_cast<const opencascade::handle<Geom_Curve>&, const TopoDS_Vertex&, const TopoDS_Vertex&, const double, const double>(&BRepBuilderAPI_MakeEdge::Init),
             py::arg("C"), py::arg("V1"), py::arg("V2"), py::arg("p1"), py::arg("p2"),
             "Reinitializes with curve C between vertices V1, V2 with parameters p1, p2")
        
        // Result methods
        .def("edge", &BRepBuilderAPI_MakeEdge::Edge,
             "Returns the constructed edge")
        .def("vertex1", &BRepBuilderAPI_MakeEdge::Vertex1,
             "Returns the first vertex of the edge (may be null)")
        .def("vertex2", &BRepBuilderAPI_MakeEdge::Vertex2,
             "Returns the second vertex of the edge (may be null)")
        
        // Status
        .def("error", &BRepBuilderAPI_MakeEdge::Error,
             "Returns the construction status")
    ;

    m.def("make_edge", [](const gp_Pnt& Pnt1, const gp_Pnt& Pnt2) {
        BRepBuilderAPI_MakeEdge edge_maker(Pnt1, Pnt2);
        if (!edge_maker.IsDone()) {
            throw std::runtime_error("Failed to create edge between points");
        }
        return edge_maker.Edge();
    }, py::arg("Pnt1"), py::arg("Pnt2"));

    py::enum_<BRepBuilderAPI_WireError>(m, "WireError")
        .value("WireDone", BRepBuilderAPI_WireDone)
        .value("EmptyWire", BRepBuilderAPI_EmptyWire)
        .value("DisconnectedWire", BRepBuilderAPI_DisconnectedWire)
        .value("NonManifoldWire", BRepBuilderAPI_NonManifoldWire)
        .export_values();

    py::class_<BRepBuilderAPI_MakeWire, BRepBuilderAPI_MakeShape>(m, "MakeWire")
        // Constructors
        .def(py::init<>())
        .def(py::init<const TopoDS_Edge&>(), py::arg("E"))
        .def(py::init<const TopoDS_Edge&, const TopoDS_Edge&>(), 
             py::arg("E1"), py::arg("E2"))
        .def(py::init<const TopoDS_Edge&, const TopoDS_Edge&, const TopoDS_Edge&>(), 
             py::arg("E1"), py::arg("E2"), py::arg("E3"))
        .def(py::init<const TopoDS_Edge&, const TopoDS_Edge&, const TopoDS_Edge&, const TopoDS_Edge&>(), 
             py::arg("E1"), py::arg("E2"), py::arg("E3"), py::arg("E4"))
        .def(py::init<const TopoDS_Wire&>(), py::arg("W"))
        .def(py::init<const TopoDS_Wire&, const TopoDS_Edge&>(), 
             py::arg("W"), py::arg("E"))
        
        // Add methods
        .def("add", py::overload_cast<const TopoDS_Edge&>(&BRepBuilderAPI_MakeWire::Add), py::arg("E"))
        .def("add", py::overload_cast<const TopoDS_Wire&>(&BRepBuilderAPI_MakeWire::Add), py::arg("W"))
        .def("add", py::overload_cast<const TopTools_ListOfShape&>(&BRepBuilderAPI_MakeWire::Add), py::arg("L"))
        
        // Status
        .def("error", &BRepBuilderAPI_MakeWire::Error)
        
        // Results
        .def("wire", &BRepBuilderAPI_MakeWire::Wire, py::return_value_policy::reference_internal)
        .def("edge", &BRepBuilderAPI_MakeWire::Edge, py::return_value_policy::reference_internal)
        .def("vertex", &BRepBuilderAPI_MakeWire::Vertex, py::return_value_policy::reference_internal)
    ;

    py::class_<BRepBuilderAPI_MakePolygon, BRepBuilderAPI_MakeShape>(m, "MakePolygon",
        "Constructs polygonal wires from points or vertices.\n\n"
        "A polygonal wire consists of a sequence of connected rectilinear edges.\n"
        "When a point or vertex is added, if it is identical to the previous point,\n"
        "no edge is built. Use the added() method to test if the vertex was added.\n\n"
        "Construction approaches:\n"
        "1. Complete polygon by defining all points/vertices (up to 4)\n"
        "2. Empty polygon and add points/vertices in sequence (unlimited)\n\n"
        "Warning: If the sequence p1-p2-p1 appears among the points, you will\n"
        "create a polygonal wire with two consecutive coincident edges")
        
        // Constructors - empty
        .def(py::init<>(),
            "Initializes an empty polygonal wire.\n\n"
            "Points or vertices are added using the add() method.\n"
            "As soon as the wire contains vertices, it can be consulted using wire()")
        
        // Constructors - from points
        .def(py::init<const gp_Pnt&, const gp_Pnt&>(),
            py::arg("P1"), py::arg("P2"),
            "Constructs a polygonal wire from 2 points.\n"
            "Vertices are automatically created on the given points")
        
        .def(py::init<const gp_Pnt&, const gp_Pnt&, const gp_Pnt&, const Standard_Boolean>(),
            py::arg("P1"), py::arg("P2"), py::arg("P3"), py::arg("close") = false,
            "Constructs a polygonal wire from 3 points.\n\n"
            "Parameters:\n"
            "  close: If True, creates a closed polygon")
        
        .def(py::init<const gp_Pnt&, const gp_Pnt&, const gp_Pnt&, const gp_Pnt&, const Standard_Boolean>(),
            py::arg("P1"), py::arg("P2"), py::arg("P3"), py::arg("P4"), py::arg("close") = false,
            "Constructs a polygonal wire from 4 points.\n\n"
            "Parameters:\n"
            "  close: If True, creates a closed polygon")
        
        // Constructors - from vertices
        .def(py::init<const TopoDS_Vertex&, const TopoDS_Vertex&>(),
            py::arg("V1"), py::arg("V2"),
            "Constructs a polygonal wire from 2 vertices")
        
        .def(py::init<const TopoDS_Vertex&, const TopoDS_Vertex&, const TopoDS_Vertex&, const Standard_Boolean>(),
            py::arg("V1"), py::arg("V2"), py::arg("V3"), py::arg("close") = false,
            "Constructs a polygonal wire from 3 vertices.\n\n"
            "Parameters:\n"
            "  close: If True, creates a closed polygon")
        
        .def(py::init<const TopoDS_Vertex&, const TopoDS_Vertex&, const TopoDS_Vertex&, const TopoDS_Vertex&, const Standard_Boolean>(),
            py::arg("V1"), py::arg("V2"), py::arg("V3"), py::arg("V4"), py::arg("close") = false,
            "Constructs a polygonal wire from 4 vertices.\n\n"
            "Parameters:\n"
            "  close: If True, creates a closed polygon")
        
        // Add methods
        .def("add",
            py::overload_cast<const gp_Pnt&>(&BRepBuilderAPI_MakePolygon::Add),
            py::arg("P"),
            "Adds the point P at the end of the polygonal wire.\n\n"
            "A vertex is automatically created on the point P.\n\n"
            "Warning:\n"
            "- When P is coincident to the previous vertex, no edge is built\n"
            "- P is not checked against other vertices except the last one\n"
            "- You can add vertices to a closed polygon\n"
            "- This may create consecutive coincident edges or non-manifold wires")
        
        .def("add",
            py::overload_cast<const TopoDS_Vertex&>(&BRepBuilderAPI_MakePolygon::Add),
            py::arg("V"),
            "Adds the vertex V at the end of the polygonal wire.\n\n"
            "Warning:\n"
            "- When V is coincident to the previous vertex, no edge is built\n"
            "- V is not checked against other vertices except the last one\n"
            "- You can add vertices to a closed polygon\n"
            "- This may create consecutive coincident edges or non-manifold wires")
        
        // Status methods
        .def("added", &BRepBuilderAPI_MakePolygon::Added,
            "Returns True if the last vertex added was not coincident with the previous one.\n\n"
            "Use this method after add() to check if an edge was actually created")
        
        .def("close", &BRepBuilderAPI_MakePolygon::Close,
            "Closes the polygonal wire under construction.\n\n"
            "This is equivalent to adding the first vertex to the polygon")
        
        .def("is_done", &BRepBuilderAPI_MakePolygon::IsDone,
            "Returns True if this algorithm contains a valid polygonal wire.\n\n"
            "A valid wire has at least one edge (i.e., at least two vertices)")
        
        // Result methods and properties
        .def_property_readonly("first_vertex", &BRepBuilderAPI_MakePolygon::FirstVertex,
            py::return_value_policy::reference_internal,
            "Returns the first vertex of the polygonal wire.\n"
            "If the polygon is closed, first and last vertices are identical")
        
        .def_property_readonly("last_vertex", &BRepBuilderAPI_MakePolygon::LastVertex,
            py::return_value_policy::reference_internal,
            "Returns the last vertex of the polygonal wire.\n"
            "If the polygon is closed, first and last vertices are identical")
        
        .def_property_readonly("edge",
            py::overload_cast<>(&BRepBuilderAPI_MakePolygon::Edge, py::const_),
            py::return_value_policy::reference_internal,
            "Returns the edge built between the last two points or vertices added.\n\n"
            "Warning: If there is only one vertex in the polygon, the result is a null edge")
        
        .def("wire",
            py::overload_cast<>(&BRepBuilderAPI_MakePolygon::Wire),
            py::return_value_policy::reference_internal,
            "Returns the constructed polygonal wire.\n\n"
            "Or the already built part of the polygonal wire under construction.\n\n"
            "Raises StdFail_NotDone if fewer than two vertices have been added")
    ;

    py::enum_<BRepBuilderAPI_FaceError>(m, "FaceError")
        .value("FaceDone", BRepBuilderAPI_FaceDone)
        .value("NoFace", BRepBuilderAPI_NoFace)
        .value("NotPlanar", BRepBuilderAPI_NotPlanar)
        .value("CurveProjectionFailed", BRepBuilderAPI_CurveProjectionFailed)
        .value("ParametersOutOfRange", BRepBuilderAPI_ParametersOutOfRange)
        .export_values();

    py::class_<BRepBuilderAPI_MakeFace, BRepBuilderAPI_MakeShape>(m, "MakeFace")
        // Constructors - empty / from face
        .def(py::init<>())
        .def(py::init<const TopoDS_Face&>(), py::arg("F"))
        
        // From elementary surfaces
        .def(py::init<const gp_Pln&>(), py::arg("P"))
        .def(py::init<const gp_Cylinder&>(), py::arg("C"))
        .def(py::init<const gp_Cone&>(), py::arg("C"))
        .def(py::init<const gp_Sphere&>(), py::arg("S"))
        .def(py::init<const gp_Torus&>(), py::arg("T"))
        
        // From Geom_Surface
        .def(py::init<const opencascade::handle<Geom_Surface>&, double>(), 
             py::arg("S"), py::arg("TolDegen"))
        
        // From elementary surfaces with bounds
        .def(py::init<const gp_Pln&, double, double, double, double>(),
             py::arg("P"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"))
        .def(py::init<const gp_Cylinder&, double, double, double, double>(),
             py::arg("C"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"))
        .def(py::init<const gp_Cone&, double, double, double, double>(),
             py::arg("C"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"))
        .def(py::init<const gp_Sphere&, double, double, double, double>(),
             py::arg("S"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"))
        .def(py::init<const gp_Torus&, double, double, double, double>(),
             py::arg("T"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"))
        
        // From Geom_Surface with bounds
        .def(py::init<const opencascade::handle<Geom_Surface>&, double, double, double, double, double>(),
             py::arg("S"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"), py::arg("TolDegen"))
        
        // From wire (find surface automatically)
        .def(py::init<const TopoDS_Wire&, Standard_Boolean>(), 
             py::arg("W"), py::arg("OnlyPlane") = false)
        
        // From surface + wire
        .def(py::init<const gp_Pln&, const TopoDS_Wire&, Standard_Boolean>(),
             py::arg("P"), py::arg("W"), py::arg("Inside") = true)
        .def(py::init<const gp_Cylinder&, const TopoDS_Wire&, Standard_Boolean>(),
             py::arg("C"), py::arg("W"), py::arg("Inside") = true)
        .def(py::init<const gp_Cone&, const TopoDS_Wire&, Standard_Boolean>(),
             py::arg("C"), py::arg("W"), py::arg("Inside") = true)
        .def(py::init<const gp_Sphere&, const TopoDS_Wire&, Standard_Boolean>(),
             py::arg("S"), py::arg("W"), py::arg("Inside") = true)
        .def(py::init<const gp_Torus&, const TopoDS_Wire&, Standard_Boolean>(),
             py::arg("T"), py::arg("W"), py::arg("Inside") = true)
        .def(py::init<const opencascade::handle<Geom_Surface>&, const TopoDS_Wire&, Standard_Boolean>(),
             py::arg("S"), py::arg("W"), py::arg("Inside") = true)
        
        // From face + wire (add hole)
        .def(py::init<const TopoDS_Face&, const TopoDS_Wire&>(),
             py::arg("F"), py::arg("W"), "From face + wire (add hole)")
        
        // Init methods
        .def("init", py::overload_cast<const TopoDS_Face&>(&BRepBuilderAPI_MakeFace::Init), py::arg("F"))
        .def("init", py::overload_cast<const opencascade::handle<Geom_Surface>&, Standard_Boolean, double>(&BRepBuilderAPI_MakeFace::Init),
             py::arg("S"), py::arg("Bound"), py::arg("TolDegen"))
        .def("init", py::overload_cast<const opencascade::handle<Geom_Surface>&, double, double, double, double, double>(&BRepBuilderAPI_MakeFace::Init),
             py::arg("S"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"), py::arg("TolDegen"))
        
        // Add wire (hole)
        .def("add", &BRepBuilderAPI_MakeFace::Add, py::arg("W"))
        
        // Status
        .def("error", &BRepBuilderAPI_MakeFace::Error)
        
        // Result
        .def("face", &BRepBuilderAPI_MakeFace::Face, py::return_value_policy::reference_internal)
    ;

    py::class_<BRepBuilderAPI_MakeSolid, BRepBuilderAPI_MakeShape>(m, "MakeSolid")
        // Constructors
        .def(py::init<>())
        .def(py::init<const TopoDS_CompSolid&>(), py::arg("S"))
        .def(py::init<const TopoDS_Shell&>(), py::arg("S"))
        .def(py::init<const TopoDS_Shell&, const TopoDS_Shell&>(), 
             py::arg("S1"), py::arg("S2"))
        .def(py::init<const TopoDS_Shell&, const TopoDS_Shell&, const TopoDS_Shell&>(), 
             py::arg("S1"), py::arg("S2"), py::arg("S3"))
        .def(py::init<const TopoDS_Solid&>(), py::arg("So"))
        .def(py::init<const TopoDS_Solid&, const TopoDS_Shell&>(), 
             py::arg("So"), py::arg("S"))
        
        // Add shell
        .def("add", &BRepBuilderAPI_MakeSolid::Add, py::arg("S"))
        
        // Result
        .def("solid", &BRepBuilderAPI_MakeSolid::Solid, py::return_value_policy::reference_internal)
    ;

    py::enum_<BRepBuilderAPI_ShellError>(m, "ShellError")
        .value("ShellDone", BRepBuilderAPI_ShellDone)
        .value("EmptyShell", BRepBuilderAPI_EmptyShell)
        .value("DisconnectedShell", BRepBuilderAPI_DisconnectedShell)
        .value("ShellParametersOutOfRange", BRepBuilderAPI_ShellParametersOutOfRange)
        .export_values();

    py::class_<BRepBuilderAPI_MakeShell, BRepBuilderAPI_MakeShape>(m, "MakeShell")
        // Constructors
        .def(py::init<>())
        .def(py::init<const opencascade::handle<Geom_Surface>&, Standard_Boolean>(),
             py::arg("S"), py::arg("Segment") = false)
        .def(py::init<const opencascade::handle<Geom_Surface>&, double, double, double, double, Standard_Boolean>(),
             py::arg("S"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"), py::arg("Segment") = false)
        
        // Init
        .def("init", &BRepBuilderAPI_MakeShell::Init,
             py::arg("S"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"), py::arg("Segment") = false)
        
        // Status
        .def("error", &BRepBuilderAPI_MakeShell::Error)
        
        // Result
        .def("shell", &BRepBuilderAPI_MakeShell::Shell, py::return_value_policy::reference_internal)
    ;

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
        .def("history", &BRepTools_ReShape::History,
            "Returns the history of the substituted shapes")
    ;

    py::class_<BRepBuilderAPI_Sewing, opencascade::handle<BRepBuilderAPI_Sewing>>(m, "Sewing",
        "Provides methods to identify possible contiguous boundaries and\n"
        "assemble contiguous shapes into one shape.\n\n"
        "Only manifold shapes will be found. Sewing will not be done in case\n"
        "of multiple edges.\n\n"
        "Typical usage:\n"
        "  sewing = BRepBuilderAPI.Sewing(tolerance=1e-6)\n"
        "  sewing.add(shape1)\n"
        "  sewing.add(shape2)\n"
        "  sewing.perform()\n"
        "  result = sewing.sewed_shape()")
        
        // Constructors
        .def(py::init<const Standard_Real, const Standard_Boolean, const Standard_Boolean, const Standard_Boolean, const Standard_Boolean>(),
            py::arg("tolerance") = 1.0e-06,
            py::arg("option1") = true,
            py::arg("option2") = true,
            py::arg("option3") = true,
            py::arg("option4") = false,
            "Creates a sewing object.\n\n"
            "Parameters:\n"
            "  tolerance: Tolerance of connexity\n"
            "  option1: Option for sewing (if False only control)\n"
            "  option2: Option for analysis of degenerated shapes\n"
            "  option3: Option for cutting of free edges\n"
            "  option4: Option for non manifold processing")
        
        // Initialization
        .def("init", &BRepBuilderAPI_Sewing::Init,
            py::arg("tolerance") = 1.0e-06,
            py::arg("option1") = true,
            py::arg("option2") = true,
            py::arg("option3") = true,
            py::arg("option4") = false,
            "Initialize the parameters if necessary")
        
        .def("load", &BRepBuilderAPI_Sewing::Load,
            py::arg("shape"),
            "Loads the context shape")
        
        // Add shapes
        .def("add", &BRepBuilderAPI_Sewing::Add,
            py::arg("shape"),
            "Defines the shapes to be sewed or controlled")
        
        // Perform sewing
        .def("perform", 
            [](BRepBuilderAPI_Sewing& self) {
                self.Perform();
            },
            "Computing - performs the sewing operation")
        
        // Results
        .def("sewed_shape", &BRepBuilderAPI_Sewing::SewedShape,
            py::return_value_policy::reference_internal,
            "Gives the sewed shape.\n\n"
            "Returns a null shape if nothing constructed.\n"
            "May be a face, a shell, a solid or a compound")
        
        // Context property
        .def_property("context",
            &BRepBuilderAPI_Sewing::GetContext,
            &BRepBuilderAPI_Sewing::SetContext,
            "Gets/sets the context")
        
        // Free edges
        .def_property_readonly("nb_free_edges", &BRepBuilderAPI_Sewing::NbFreeEdges,
            "Gives the number of free edges (edge shared by one face)")
        
        .def("free_edge", &BRepBuilderAPI_Sewing::FreeEdge,
            py::arg("index"),
            py::return_value_policy::reference_internal,
            "Gives each free edge")
        
        // Multiple edges
        .def_property_readonly("nb_multiple_edges", &BRepBuilderAPI_Sewing::NbMultipleEdges,
            "Gives the number of multiple edges (edge shared by more than two faces)")
        
        .def("multiple_edge", &BRepBuilderAPI_Sewing::MultipleEdge,
            py::arg("index"),
            py::return_value_policy::reference_internal,
            "Gives each multiple edge")
        
        // Contiguous edges
        .def_property_readonly("nb_contigous_edges", &BRepBuilderAPI_Sewing::NbContigousEdges,
            "Gives the number of contiguous edges (edge shared by two faces)")
        
        .def("contigous_edge", &BRepBuilderAPI_Sewing::ContigousEdge,
            py::arg("index"),
            py::return_value_policy::reference_internal,
            "Gives each contiguous edge")
        
        .def("contigous_edge_couple", &BRepBuilderAPI_Sewing::ContigousEdgeCouple,
            py::arg("index"),
            py::return_value_policy::reference_internal,
            "Gives the sections (edge) belonging to a contiguous edge")
        
        // Section queries
        .def("is_section_bound", &BRepBuilderAPI_Sewing::IsSectionBound,
            py::arg("section"),
            "Indicates if a section is bound (before use section_to_boundary)")
        
        .def("section_to_boundary", &BRepBuilderAPI_Sewing::SectionToBoundary,
            py::arg("section"),
            py::return_value_policy::reference_internal,
            "Gives the original edge (free boundary) which becomes the section.\n\n"
            "Remember that sections constitute common edges.\n"
            "This information is important for control because with\n"
            "original edge we can find the surface to which the section is attached")
        
        // Degenerated shapes
        .def_property_readonly("nb_degenerated_shapes", &BRepBuilderAPI_Sewing::NbDegeneratedShapes,
            "Gives the number of degenerated shapes")
        
        .def("degenerated_shape", &BRepBuilderAPI_Sewing::DegeneratedShape,
            py::arg("index"),
            py::return_value_policy::reference_internal,
            "Gives each degenerated shape")
        
        .def("is_degenerated", &BRepBuilderAPI_Sewing::IsDegenerated,
            py::arg("shape"),
            "Indicates if an input shape is degenerated")
        
        // Modified shapes
        .def("is_modified", &BRepBuilderAPI_Sewing::IsModified,
            py::arg("shape"),
            "Indicates if an input shape has been modified")
        
        .def("modified", &BRepBuilderAPI_Sewing::Modified,
            py::arg("shape"),
            py::return_value_policy::reference_internal,
            "Gives a modified shape")
        
        .def("is_modified_subshape", &BRepBuilderAPI_Sewing::IsModifiedSubShape,
            py::arg("shape"),
            "Indicates if an input subshape has been modified")
        
        .def("modified_subshape", &BRepBuilderAPI_Sewing::ModifiedSubShape,
            py::arg("shape"),
            "Gives a modified subshape")
        
        // Deleted faces
        .def_property_readonly("nb_deleted_faces", &BRepBuilderAPI_Sewing::NbDeletedFaces,
            "Gives the number of deleted faces (faces smaller than tolerance)")
        
        .def("deleted_face", &BRepBuilderAPI_Sewing::DeletedFace,
            py::arg("index"),
            py::return_value_policy::reference_internal,
            "Gives each deleted face")
        
        .def("which_face", &BRepBuilderAPI_Sewing::WhichFace,
            py::arg("edge"), py::arg("index") = 1,
            "Gives a modified shape")
        
        // Properties - same parameter mode
        .def_property("same_parameter_mode",
            &BRepBuilderAPI_Sewing::SameParameterMode,
            &BRepBuilderAPI_Sewing::SetSameParameterMode,
            "Gets/sets same parameter mode")
        
        // Properties - tolerances
        .def_property("tolerance",
            &BRepBuilderAPI_Sewing::Tolerance,
            &BRepBuilderAPI_Sewing::SetTolerance,
            "Gets/sets tolerance")
        
        .def_property("min_tolerance",
            &BRepBuilderAPI_Sewing::MinTolerance,
            &BRepBuilderAPI_Sewing::SetMinTolerance,
            "Gets/sets min tolerance")
        
        .def_property("max_tolerance",
            &BRepBuilderAPI_Sewing::MaxTolerance,
            &BRepBuilderAPI_Sewing::SetMaxTolerance,
            "Gets/sets max tolerance")
        
        // Properties - modes
        .def_property("face_mode",
            &BRepBuilderAPI_Sewing::FaceMode,
            &BRepBuilderAPI_Sewing::SetFaceMode,
            "Gets/sets mode for sewing faces (default: True)")
        
        .def_property("floating_edges_mode",
            &BRepBuilderAPI_Sewing::FloatingEdgesMode,
            &BRepBuilderAPI_Sewing::SetFloatingEdgesMode,
            "Gets/sets mode for sewing floating edges (default: False)")
        
        .def_property("local_tolerances_mode",
            &BRepBuilderAPI_Sewing::LocalTolerancesMode,
            &BRepBuilderAPI_Sewing::SetLocalTolerancesMode,
            "Gets/sets mode for accounting of local tolerances.\n\n"
            "During merging: WorkTolerance = tolerance + tolEdge1 + tolEdge2")
        
        .def_property("non_manifold_mode",
            &BRepBuilderAPI_Sewing::NonManifoldMode,
            &BRepBuilderAPI_Sewing::SetNonManifoldMode,
            "Gets/sets mode for non-manifold sewing")
        
        // Debug
        .def("dump", &BRepBuilderAPI_Sewing::Dump,
            "Print the information")
    ;

     bind_brep_prim_api(m);

     
}