#include <gp.hxx>
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


    py::class_<BRepBuilderAPI_MakeEdge, BRepBuilderAPI_MakeShape>(m, "MakeEdge")
        .def(py::init<const gp_Pnt&, const gp_Pnt&>(), py::arg("Pnt1"), py::arg("Pnt2"))
        .def("edge", &BRepBuilderAPI_MakeEdge::Edge)
        .def("vertex1", &BRepBuilderAPI_MakeEdge::Vertex1)
        .def("vertex2", &BRepBuilderAPI_MakeEdge::Vertex2)
        .def("error_status", &BRepBuilderAPI_MakeEdge::Error)
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
        .def(py::init<const opencascade::handle<Geom_Surface>&, Standard_Real>(), 
             py::arg("S"), py::arg("TolDegen"))
        
        // From elementary surfaces with bounds
        .def(py::init<const gp_Pln&, Standard_Real, Standard_Real, Standard_Real, Standard_Real>(),
             py::arg("P"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"))
        .def(py::init<const gp_Cylinder&, Standard_Real, Standard_Real, Standard_Real, Standard_Real>(),
             py::arg("C"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"))
        .def(py::init<const gp_Cone&, Standard_Real, Standard_Real, Standard_Real, Standard_Real>(),
             py::arg("C"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"))
        .def(py::init<const gp_Sphere&, Standard_Real, Standard_Real, Standard_Real, Standard_Real>(),
             py::arg("S"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"))
        .def(py::init<const gp_Torus&, Standard_Real, Standard_Real, Standard_Real, Standard_Real>(),
             py::arg("T"), py::arg("UMin"), py::arg("UMax"), py::arg("VMin"), py::arg("VMax"))
        
        // From Geom_Surface with bounds
        .def(py::init<const opencascade::handle<Geom_Surface>&, Standard_Real, Standard_Real, Standard_Real, Standard_Real, Standard_Real>(),
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
             py::arg("F"), py::arg("W"))
        
        // Init methods
        .def("init", py::overload_cast<const TopoDS_Face&>(&BRepBuilderAPI_MakeFace::Init), py::arg("F"))
        .def("init", py::overload_cast<const opencascade::handle<Geom_Surface>&, Standard_Boolean, Standard_Real>(&BRepBuilderAPI_MakeFace::Init),
             py::arg("S"), py::arg("Bound"), py::arg("TolDegen"))
        .def("init", py::overload_cast<const opencascade::handle<Geom_Surface>&, Standard_Real, Standard_Real, Standard_Real, Standard_Real, Standard_Real>(&BRepBuilderAPI_MakeFace::Init),
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
        .def(py::init<const opencascade::handle<Geom_Surface>&, Standard_Real, Standard_Real, Standard_Real, Standard_Real, Standard_Boolean>(),
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
        .def(py::init<const Standard_Real, const Standard_Real, const Standard_Real>(),
             py::arg("R1"), py::arg("R2"), py::arg("H"))
        .def(py::init<const Standard_Real, const Standard_Real, const Standard_Real, const Standard_Real>(),
             py::arg("R1"), py::arg("R2"), py::arg("H"), py::arg("angle"))
        .def(py::init<const gp_Ax2&, const Standard_Real, const Standard_Real, const Standard_Real>(),
             py::arg("Axes"), py::arg("R1"), py::arg("R2"), py::arg("H"))
    ;

    py::class_<BRepPrimAPI_MakeCylinder, BRepPrimAPI_MakeOneAxis>(m, "MakeCylinder")
        .def(py::init<const Standard_Real, const Standard_Real>(),
             py::arg("R"), py::arg("H"))
        .def(py::init<const Standard_Real, const Standard_Real, const Standard_Real>(),
             py::arg("R"), py::arg("H"), py::arg("angle"))
        .def(py::init<const gp_Ax2&, const Standard_Real, const Standard_Real>(),
             py::arg("Axes"), py::arg("R"), py::arg("H"))
    ;

    py::class_<BRepPrimAPI_MakeRevolution, BRepPrimAPI_MakeOneAxis>(m, "MakeRevolution")
        // Constructors without axes (uses default Z axis)
        .def(py::init<const opencascade::handle<Geom_Curve>&>(),
             py::arg("meridian"))
        .def(py::init<const opencascade::handle<Geom_Curve>&, const Standard_Real>(),
             py::arg("meridian"), py::arg("angle"))
        .def(py::init<const opencascade::handle<Geom_Curve>&, const Standard_Real, const Standard_Real>(),
             py::arg("meridian"), py::arg("v_min"), py::arg("v_max"))
        .def(py::init<const opencascade::handle<Geom_Curve>&, const Standard_Real, const Standard_Real, const Standard_Real>(),
             py::arg("meridian"), py::arg("v_min"), py::arg("v_max"), py::arg("angle"))
        
        // Constructors with custom axes
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&>(),
             py::arg("axes"), py::arg("meridian"))
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&, const Standard_Real>(),
             py::arg("axes"), py::arg("meridian"), py::arg("angle"))
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&, const Standard_Real, const Standard_Real>(),
             py::arg("axes"), py::arg("meridian"), py::arg("v_min"), py::arg("v_max"))
        .def(py::init<const gp_Ax2&, const opencascade::handle<Geom_Curve>&, const Standard_Real, const Standard_Real, const Standard_Real>(),
             py::arg("axes"), py::arg("meridian"), py::arg("v_min"), py::arg("v_max"), py::arg("angle"))
        
        // Methods
        .def("revolution", &BRepPrimAPI_MakeRevolution::Revolution, py::return_value_policy::reference_internal)
    ;
}