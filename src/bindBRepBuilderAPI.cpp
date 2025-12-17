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
#include <BRepPrimAPI_MakeOneAxis.hxx>
#include <BRepPrimAPI_MakeCone.hxx>
#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakeRevolution.hxx>
#include <BRepPrimAPI_MakeSweep.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepOffsetAPI_MakePipe.hxx>
#include <BRepOffsetAPI_MakePipeShell.hxx>
#include <GeomFill_Trihedron.hxx>
#include <BRepFill_TypeOfContact.hxx>
#include <BRepBuilderAPI_PipeError.hxx>
#include <BRepBuilderAPI_TransitionMode.hxx>
#include <BRepPrimAPI_MakePrism.hxx>
#include <BRepPrimAPI_MakeRevol.hxx>
#include <Law_Function.hxx>
#include <BRepSweep_Prism.hxx>
#include <BRepSweep_Revol.hxx>


namespace py = pybind11;

// Declare opencascade::handle as a holder type for pybind11
// Required to process opencascade::handle<T> as arguments/return types
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

template<typename T>
auto process_container(const T& container) {
    auto py_list = py::list();
    for (const auto& item : container) {
        py_list.append(item);
    }
    return py_list;
};

void bind_brepbuilderapi(py::module_ &m)
{

    py::class_<BRepBuilderAPI_Command>(m, "Command")
        .def("is_done", &BRepBuilderAPI_Command::IsDone)
        .def("check", &BRepBuilderAPI_Command::Check)
    ;

    py::class_<BRepBuilderAPI_MakeShape, BRepBuilderAPI_Command>(m, "MakeShape")
        .def("build", &BRepBuilderAPI_MakeShape::Build) // TODO add progess indicator?
        .def("shape", &BRepBuilderAPI_MakeShape::Shape)
        .def("generated", [](BRepBuilderAPI_MakeShape& self, const TopoDS_Shape &theOriginal) {
            return process_container(self.Generated(theOriginal));
        })
        .def("modified", [](BRepBuilderAPI_MakeShape& self, const TopoDS_Shape &theOriginal) {
            return process_container(self.Modified(theOriginal));
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

    ////////////////////

    py::class_<BRepPrimAPI_MakeOneAxis, BRepBuilderAPI_MakeShape>(m, "MakeOneAxis")
        // Note: This is an abstract base class - cannot be instantiated directly
        // Use concrete classes like MakeCylinder, MakeCone, etc.
        .def("face", &BRepPrimAPI_MakeOneAxis::Face)
        .def("shell", &BRepPrimAPI_MakeOneAxis::Shell)
        .def("solid", &BRepPrimAPI_MakeOneAxis::Solid)
    ;

    py::class_<BRepPrimAPI_MakeCone, BRepPrimAPI_MakeOneAxis>(m, "MakeCone")
        .def(py::init<const double, const double, const double>(),
             py::arg("R1"), py::arg("R2"), py::arg("H"))
        .def(py::init<const double, const double, const double, const double>(),
             py::arg("R1"), py::arg("R2"), py::arg("H"), py::arg("angle"))
        .def(py::init<const gp_Ax2&, const double, const double, const double>(),
             py::arg("Axes"), py::arg("R1"), py::arg("R2"), py::arg("H"))
    ;

    py::class_<BRepPrimAPI_MakeCylinder, BRepPrimAPI_MakeOneAxis>(m, "MakeCylinder")
        .def(py::init<const double, const double>(),
             py::arg("R"), py::arg("H"))
        .def(py::init<const double, const double, const double>(),
             py::arg("R"), py::arg("H"), py::arg("angle"))
        .def(py::init<const gp_Ax2&, const double, const double>(),
             py::arg("Axes"), py::arg("R"), py::arg("H"))
    ;

    py::class_<BRepPrimAPI_MakeRevolution, BRepPrimAPI_MakeOneAxis>(m, "MakeRevolution")
        // Constructors without axes (uses default Z axis)
        .def(py::init<const opencascade::handle<Geom_Curve>&>(),
             py::arg("meridian"))
        .def(py::init<const opencascade::handle<Geom_Curve>&, const double>(),
             py::arg("meridian"), py::arg("angle"))
        .def(py::init<const opencascade::handle<Geom_Curve>&, const double, const double>(),
             py::arg("meridian"), py::arg("v_min"), py::arg("v_max"))
        .def(py::init<const opencascade::handle<Geom_Curve>&, const double, const double, const double>(),
             py::arg("meridian"), py::arg("v_min"), py::arg("v_max"), py::arg("angle"))
        
        // Constructors with custom axes
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&>(),
             py::arg("axes"), py::arg("meridian"))
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&, const double>(),
             py::arg("axes"), py::arg("meridian"), py::arg("angle"))
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&, const double, const double>(),
             py::arg("axes"), py::arg("meridian"), py::arg("v_min"), py::arg("v_max"))
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&, const double, const double, const double>(),
             py::arg("axes"), py::arg("meridian"), py::arg("v_min"), py::arg("v_max"), py::arg("angle"))
        
        // Methods
        .def("revolution", &BRepPrimAPI_MakeRevolution::Revolution, py::return_value_policy::reference_internal)
    ;

    py::class_<BRepPrimAPI_MakeSweep, BRepBuilderAPI_MakeShape>(m, "MakeSweep")
        // Note: This is an abstract base class for swept primitives
        // Concrete classes like MakeRevolution inherit from this
        .def("first_shape", &BRepPrimAPI_MakeSweep::FirstShape, py::return_value_policy::reference_internal,
             "Returns the TopoDS Shape of the bottom of the sweep")
        .def("last_shape", &BRepPrimAPI_MakeSweep::LastShape, py::return_value_policy::reference_internal,
             "Returns the TopoDS Shape of the top of the sweep")
    ;

    py::class_<BRepOffsetAPI_MakePipe, BRepPrimAPI_MakeSweep>(m, "MakePipe")
        // Constructors
        .def(py::init<const TopoDS_Wire&, const TopoDS_Shape&>(),
             py::arg("spine"), py::arg("profile"),
             "Constructs a pipe by sweeping the shape Profile along the wire Spine")
        .def(py::init<const TopoDS_Wire&, const TopoDS_Shape&, const GeomFill_Trihedron, const Standard_Boolean>(),
             py::arg("spine"), py::arg("profile"), py::arg("mode"), py::arg("force_approx_c1") = false,
             "Same as previous but with setting of mode of sweep and C1 approximation flag")
        
        // Methods
        .def("pipe", &BRepOffsetAPI_MakePipe::Pipe, py::return_value_policy::reference_internal,
             "Returns the internal BRepFill_Pipe object")
        .def("generated", py::overload_cast<const TopoDS_Shape&, const TopoDS_Shape&>(&BRepOffsetAPI_MakePipe::Generated),
             py::arg("spine_shape"), py::arg("profile_shape"),
             "Returns the shape generated from the given spine and profile shapes")
        .def("error_on_surface", &BRepOffsetAPI_MakePipe::ErrorOnSurface,
             "Returns the maximum error on the generated surface")
    ;

    py::enum_<BRepFill_TypeOfContact>(m, "BRepFill_TypeOfContact")
        .value("NoContact", BRepFill_NoContact)
        .value("Contact", BRepFill_Contact)
        .value("ContactOnBorder", BRepFill_ContactOnBorder)
        .export_values();

    py::enum_<BRepBuilderAPI_PipeError>(m, "PipeError")
        .value("PipeDone", BRepBuilderAPI_PipeDone)
        .value("PipeNotDone", BRepBuilderAPI_PipeNotDone)
        .value("PlaneNotIntersectGuide", BRepBuilderAPI_PlaneNotIntersectGuide)
        .value("ImpossibleContact", BRepBuilderAPI_ImpossibleContact)
        .export_values();

    py::enum_<BRepBuilderAPI_TransitionMode>(m, "TransitionMode")
        .value("Transformed", BRepBuilderAPI_Transformed)
        .value("RightCorner", BRepBuilderAPI_RightCorner)
        .value("RoundCorner", BRepBuilderAPI_RoundCorner)
        .export_values();

    py::class_<BRepOffsetAPI_MakePipeShell, BRepPrimAPI_MakeSweep>(m, "MakePipeShell")
        // Constructor
        .def(py::init<const TopoDS_Wire&>(), py::arg("spine"),
             "Constructs the shell-generating framework defined by the wire Spine")
        
        // Mode setting methods
        .def("set_mode", py::overload_cast<const Standard_Boolean>(&BRepOffsetAPI_MakePipeShell::SetMode),
             py::arg("is_frenet") = false,
             "Sets a Frenet or CorrectedFrenet trihedron. If IsFrenet is false, a corrected Frenet trihedron is used")
        .def("set_discrete_mode", &BRepOffsetAPI_MakePipeShell::SetDiscreteMode,
             "Sets a Discrete trihedron to perform the sweeping")
        .def("set_mode", py::overload_cast<const gp_Ax2&>(&BRepOffsetAPI_MakePipeShell::SetMode),
             py::arg("axe"), "Sets a fixed trihedron - all sections will be parallel")
        .def("set_mode", py::overload_cast<const gp_Dir&>(&BRepOffsetAPI_MakePipeShell::SetMode),
             py::arg("binormal"), "Sets a fixed BiNormal direction - angular relations will be constant")
        .def("set_mode", py::overload_cast<const TopoDS_Shape&>(&BRepOffsetAPI_MakePipeShell::SetMode),
             py::arg("spine_support"), "Sets support to the spine to define the BiNormal")
        .def("set_mode", py::overload_cast<const TopoDS_Wire&, const Standard_Boolean, const BRepFill_TypeOfContact>(&BRepOffsetAPI_MakePipeShell::SetMode),
             py::arg("auxiliary_spine"), py::arg("curvilinear_equivalence"), py::arg("keep_contact") = BRepFill_NoContact,
             "Sets an auxiliary spine to define the Normal")
        
        // Profile methods
        .def("add", py::overload_cast<const TopoDS_Shape&, const Standard_Boolean, const Standard_Boolean>(&BRepOffsetAPI_MakePipeShell::Add),
             py::arg("profile"), py::arg("with_contact") = false, py::arg("with_correction") = false,
             "Adds a section Profile. Point on spine computed automatically")
        .def("add", py::overload_cast<const TopoDS_Shape&, const TopoDS_Vertex&, const Standard_Boolean, const Standard_Boolean>(&BRepOffsetAPI_MakePipeShell::Add),
             py::arg("profile"), py::arg("location"), py::arg("with_contact") = false, py::arg("with_correction") = false,
             "Adds a section Profile at the given Location on spine")
        .def("set_law", py::overload_cast<const TopoDS_Shape&, const opencascade::handle<Law_Function>&, const Standard_Boolean, const Standard_Boolean>(&BRepOffsetAPI_MakePipeShell::SetLaw),
             py::arg("profile"), py::arg("law"), py::arg("with_contact") = false, py::arg("with_correction") = false,
             "Sets evolution law with homotetic law defined by function")
        .def("set_law", py::overload_cast<const TopoDS_Shape&, const opencascade::handle<Law_Function>&, const TopoDS_Vertex&, const Standard_Boolean, const Standard_Boolean>(&BRepOffsetAPI_MakePipeShell::SetLaw),
             py::arg("profile"), py::arg("law"), py::arg("location"), py::arg("with_contact") = false, py::arg("with_correction") = false,
             "Sets evolution law with homotetic law defined by function at given location")
        .def("delete", &BRepOffsetAPI_MakePipeShell::Delete, py::arg("profile"),
             "Removes the section Profile")
        
        // Status and configuration
        .def("is_ready", &BRepOffsetAPI_MakePipeShell::IsReady,
             "Returns true if ready to build (has profile definition)")
        .def("get_status", &BRepOffsetAPI_MakePipeShell::GetStatus,
             "Get status when Simulate or Build failed")
        .def("set_tolerance", &BRepOffsetAPI_MakePipeShell::SetTolerance,
             py::arg("tol_3d") = 1.0e-4, py::arg("bound_tol") = 1.0e-4, py::arg("tol_angular") = 1.0e-2,
             "Sets tolerance values")
        .def("set_max_degree", &BRepOffsetAPI_MakePipeShell::SetMaxDegree, py::arg("new_max_degree"),
             "Define maximum V degree of resulting surface")
        .def("set_max_segments", &BRepOffsetAPI_MakePipeShell::SetMaxSegments, py::arg("new_max_segments"),
             "Define maximum number of spans in V-direction")
        .def("set_force_approx_c1", &BRepOffsetAPI_MakePipeShell::SetForceApproxC1, py::arg("force_approx_c1"),
             "Set flag to attempt C1-continuous surface approximation")
        .def("set_transition_mode", &BRepOffsetAPI_MakePipeShell::SetTransitionMode,
             py::arg("mode") = BRepBuilderAPI_Transformed,
             "Sets transition mode for discontinuities on spine")
        
        // Simulation and building
        .def("simulate", [](BRepOffsetAPI_MakePipeShell& self, const Standard_Integer number_of_section) {
            TopTools_ListOfShape result;
            self.Simulate(number_of_section, result);
            return process_container(result);
        }, py::arg("number_of_section"),
        "Simulates the resulting shape by calculating cross-sections")
        .def("make_solid", &BRepOffsetAPI_MakePipeShell::MakeSolid,
             "Transforms the sweeping Shell into Solid")
        .def("error_on_surface", &BRepOffsetAPI_MakePipeShell::ErrorOnSurface,
             "Returns maximum error on generated surface")
        .def("profiles", [](BRepOffsetAPI_MakePipeShell& self) {
            TopTools_ListOfShape profiles;
            self.Profiles(profiles);
            return process_container(profiles);
        }, "Returns the list of original profiles")
        .def("spine", &BRepOffsetAPI_MakePipeShell::Spine, py::return_value_policy::reference_internal,
             "Returns the spine")
    ;

    
    py::class_<BRepPrimAPI_MakePrism, BRepPrimAPI_MakeSweep>(m, "MakePrism")
        // Constructors
        .def(py::init<const TopoDS_Shape&, const gp_Vec&, const Standard_Boolean, const Standard_Boolean>(),
             py::arg("shape"), py::arg("vector"), py::arg("copy") = false, py::arg("canonize") = true,
             "Builds the prism of base S and vector V. If Copy is true, S is copied")
        .def(py::init<const TopoDS_Shape&, const gp_Dir&, const Standard_Boolean, const Standard_Boolean, const Standard_Boolean>(),
             py::arg("shape"), py::arg("direction"), py::arg("infinite") = true, py::arg("copy") = false, py::arg("canonize") = true,
             "Builds a semi-infinite or infinite prism. If Inf is true the prism is infinite")
        
        // Methods
        .def("prism", &BRepPrimAPI_MakePrism::Prism, py::return_value_policy::reference_internal,
             "Returns the internal sweeping algorithm")
        .def("first_shape", py::overload_cast<>(&BRepPrimAPI_MakePrism::FirstShape),
             "Returns the TopoDS Shape of the bottom of the prism")
        .def("last_shape", py::overload_cast<>(&BRepPrimAPI_MakePrism::LastShape),
             "Returns the TopoDS Shape of the top of the prism")
        .def("first_shape", py::overload_cast<const TopoDS_Shape&>(&BRepPrimAPI_MakePrism::FirstShape),
             py::arg("the_shape"), "Returns the bottom shape generated with theShape")
        .def("last_shape", py::overload_cast<const TopoDS_Shape&>(&BRepPrimAPI_MakePrism::LastShape),
             py::arg("the_shape"), "Returns the top shape generated with theShape")
        .def("is_deleted", &BRepPrimAPI_MakePrism::IsDeleted, py::arg("shape"),
             "Returns true if the shape S has been deleted")
    ;

    py::class_<BRepPrimAPI_MakeRevol, BRepPrimAPI_MakeSweep>(m, "MakeRevol")
        // Constructors
        .def(py::init<const TopoDS_Shape&, const gp_Ax1&, const double, const Standard_Boolean>(),
             py::arg("shape"), py::arg("axis"), py::arg("angle"), py::arg("copy") = false,
             "Builds the Revol of base S, axis A and angle D. If Copy is true, S is copied")
        .def(py::init<const TopoDS_Shape&, const gp_Ax1&, const Standard_Boolean>(),
             py::arg("shape"), py::arg("axis"), py::arg("copy") = false,
             "Builds the Revol of base S, axis A and angle 2*Pi. If Copy is true, S is copied")
        
        // Methods
        .def("revol", &BRepPrimAPI_MakeRevol::Revol, py::return_value_policy::reference_internal,
             "Returns the internal sweeping algorithm")
        .def("first_shape", py::overload_cast<>(&BRepPrimAPI_MakeRevol::FirstShape),
             "Returns the first shape of the revol (coinciding with the generating shape)")
        .def("last_shape", py::overload_cast<>(&BRepPrimAPI_MakeRevol::LastShape),
             "Returns the TopoDS Shape of the end of the revol")
        .def("first_shape", py::overload_cast<const TopoDS_Shape&>(&BRepPrimAPI_MakeRevol::FirstShape),
             py::arg("the_shape"), "Returns the first shape generated with theShape")
        .def("last_shape", py::overload_cast<const TopoDS_Shape&>(&BRepPrimAPI_MakeRevol::LastShape),
             py::arg("the_shape"), "Returns the last shape generated with theShape")
        .def("is_deleted", &BRepPrimAPI_MakeRevol::IsDeleted, py::arg("shape"),
             "Returns true if the shape S has been deleted")
        .def("has_degenerated", &BRepPrimAPI_MakeRevol::HasDegenerated,
             "Check if there are degenerated edges in the result")
        .def("degenerated", [](BRepPrimAPI_MakeRevol& self) {
            return process_container(self.Degenerated());
        }, "Returns the list of degenerated edges")
    ;



}