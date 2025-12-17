#include <gp.hxx>
#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>
#include <Geom_Curve.hxx>
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