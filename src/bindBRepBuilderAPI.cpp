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
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <BRepBuilderAPI_MakeShell.hxx>

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
        .def("build", &BRepBuilderAPI_MakeShape::Build) // TODO add progess indicator?
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

     bind_brep_prim_api(m);
}