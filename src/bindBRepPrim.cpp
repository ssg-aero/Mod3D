#include <pybind11/pybind11.h>
#include <BRepPrim_Builder.hxx>
#include <BRepPrim_OneAxis.hxx>
#include <BRepPrim_Revolution.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <gp_Pln.hxx>
#include <gp_Lin.hxx>
#include <gp_Circ.hxx>
#include <gp_Lin2d.hxx>
#include <gp_Circ2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Ax2.hxx>
#include <Geom2d_Curve.hxx>

namespace py = pybind11;

// Declare opencascade::handle as a holder type for pybind11
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_brepprim(py::module_ &m)
{
    py::class_<BRepPrim_Builder>(m, "Builder")
        // Constructors
        .def(py::init<>())
        .def(py::init<const BRep_Builder&>(), py::arg("B"))
        
        // Access to underlying builder
        .def("builder", &BRepPrim_Builder::Builder, py::return_value_policy::reference_internal)
        
        // Make empty shapes (return created objects)
        .def("make_shell", [](const BRepPrim_Builder& self) {
            TopoDS_Shell S;
            self.MakeShell(S);
            return S;
        })
        .def("make_wire", [](const BRepPrim_Builder& self) {
            TopoDS_Wire W;
            self.MakeWire(W);
            return W;
        })
        .def("make_degenerated_edge", [](const BRepPrim_Builder& self) {
            TopoDS_Edge E;
            self.MakeDegeneratedEdge(E);
            return E;
        })
        
        // Make shapes from geometry
        .def("make_face", [](const BRepPrim_Builder& self, const gp_Pln& P) {
            TopoDS_Face F;
            self.MakeFace(F, P);
            return F;
        }, py::arg("plane"))
        .def("make_edge", [](const BRepPrim_Builder& self, const gp_Lin& L) {
            TopoDS_Edge E;
            self.MakeEdge(E, L);
            return E;
        }, py::arg("line"))
        .def("make_edge", [](const BRepPrim_Builder& self, const gp_Circ& C) {
            TopoDS_Edge E;
            self.MakeEdge(E, C);
            return E;
        }, py::arg("circle"))
        .def("make_vertex", [](const BRepPrim_Builder& self, const gp_Pnt& P) {
            TopoDS_Vertex V;
            self.MakeVertex(V, P);
            return V;
        }, py::arg("point"))
        
        // Set PCurves
        .def("set_pcurve", [](const BRepPrim_Builder& self, TopoDS_Edge& E, const TopoDS_Face& F, const gp_Lin2d& L) {
            self.SetPCurve(E, F, L);
        }, py::arg("E"), py::arg("F"), py::arg("L"))
        .def("set_pcurve", [](const BRepPrim_Builder& self, TopoDS_Edge& E, const TopoDS_Face& F, const gp_Lin2d& L1, const gp_Lin2d& L2) {
            self.SetPCurve(E, F, L1, L2);
        }, py::arg("E"), py::arg("F"), py::arg("L1"), py::arg("L2"))
        .def("set_pcurve", [](const BRepPrim_Builder& self, TopoDS_Edge& E, const TopoDS_Face& F, const gp_Circ2d& C) {
            self.SetPCurve(E, F, C);
        }, py::arg("E"), py::arg("F"), py::arg("C"))
        
        // Reverse
        .def("reverse_face", [](const BRepPrim_Builder& self, TopoDS_Face& F) {
            self.ReverseFace(F);
        }, py::arg("F"))
        
        // Add vertex to edge
        .def("add_edge_vertex", [](const BRepPrim_Builder& self, TopoDS_Edge& E, const TopoDS_Vertex& V, Standard_Real P, bool direct) {
            self.AddEdgeVertex(E, V, P, direct);
        }, py::arg("E"), py::arg("V"), py::arg("P"), py::arg("direct"))
        .def("add_edge_vertex_closed", [](const BRepPrim_Builder& self, TopoDS_Edge& E, const TopoDS_Vertex& V, Standard_Real P1, Standard_Real P2) {
            self.AddEdgeVertex(E, V, P1, P2);
        }, py::arg("E"), py::arg("V"), py::arg("P1"), py::arg("P2"))
        
        // Set parameters on closed edge
        .def("set_parameters", [](const BRepPrim_Builder& self, TopoDS_Edge& E, const TopoDS_Vertex& V, Standard_Real P1, Standard_Real P2) {
            self.SetParameters(E, V, P1, P2);
        }, py::arg("E"), py::arg("V"), py::arg("P1"), py::arg("P2"))
        
        // Add edge to wire
        .def("add_wire_edge", [](const BRepPrim_Builder& self, TopoDS_Wire& W, const TopoDS_Edge& E, bool direct) {
            self.AddWireEdge(W, E, direct);
        }, py::arg("W"), py::arg("E"), py::arg("direct"))
        
        // Add wire to face
        .def("add_face_wire", [](const BRepPrim_Builder& self, TopoDS_Face& F, const TopoDS_Wire& W) {
            self.AddFaceWire(F, W);
        }, py::arg("F"), py::arg("W"))
        
        // Add face to shell
        .def("add_shell_face", [](const BRepPrim_Builder& self, TopoDS_Shell& Sh, const TopoDS_Face& F) {
            self.AddShellFace(Sh, F);
        }, py::arg("Sh"), py::arg("F"))
        
        // Complete methods (post-processing)
        .def("complete_edge", [](const BRepPrim_Builder& self, TopoDS_Edge& E) {
            self.CompleteEdge(E);
        }, py::arg("E"))
        .def("complete_wire", [](const BRepPrim_Builder& self, TopoDS_Wire& W) {
            self.CompleteWire(W);
        }, py::arg("W"))
        .def("complete_face", [](const BRepPrim_Builder& self, TopoDS_Face& F) {
            self.CompleteFace(F);
        }, py::arg("F"))
        .def("complete_shell", [](const BRepPrim_Builder& self, TopoDS_Shell& S) {
            self.CompleteShell(S);
        }, py::arg("S"))
    ;

    py::class_<BRepPrim_OneAxis>(m, "OneAxis")
        // Note: This is an abstract class - cannot be instantiated directly
        // Use concrete classes like BRepPrimAPI_MakeCone, BRepPrimAPI_MakeCylinder etc.
        
        // Meridian offset
        .def("set_meridian_offset", &BRepPrim_OneAxis::SetMeridianOffset, 
             py::arg("meridian_offset") = 0.0)
        
        // Axis properties
        .def("axes", py::overload_cast<>(&BRepPrim_OneAxis::Axes, py::const_))
        .def("axes", py::overload_cast<const gp_Ax2&>(&BRepPrim_OneAxis::Axes), py::arg("a"))
        .def("angle", py::overload_cast<>(&BRepPrim_OneAxis::Angle, py::const_))
        .def("angle", py::overload_cast<Standard_Real>(&BRepPrim_OneAxis::Angle), py::arg("a"))
        .def("v_min", py::overload_cast<>(&BRepPrim_OneAxis::VMin, py::const_))
        .def("v_min", py::overload_cast<Standard_Real>(&BRepPrim_OneAxis::VMin), py::arg("v"))
        .def("v_max", py::overload_cast<>(&BRepPrim_OneAxis::VMax, py::const_))
        .def("v_max", py::overload_cast<Standard_Real>(&BRepPrim_OneAxis::VMax), py::arg("v"))
        
        // Virtual methods (implemented in derived classes)
        .def("meridian_on_axis", &BRepPrim_OneAxis::MeridianOnAxis, py::arg("v"))
        .def("meridian_closed", &BRepPrim_OneAxis::MeridianClosed)
        .def("v_max_infinite", &BRepPrim_OneAxis::VMaxInfinite)
        .def("v_min_infinite", &BRepPrim_OneAxis::VMinInfinite)
        .def("has_top", &BRepPrim_OneAxis::HasTop)
        .def("has_bottom", &BRepPrim_OneAxis::HasBottom)
        .def("has_sides", &BRepPrim_OneAxis::HasSides)
        
        // Topology access methods
        .def("shell", &BRepPrim_OneAxis::Shell, py::return_value_policy::reference_internal)
        .def("lateral_face", &BRepPrim_OneAxis::LateralFace, py::return_value_policy::reference_internal)
        .def("top_face", &BRepPrim_OneAxis::TopFace, py::return_value_policy::reference_internal)
        .def("bottom_face", &BRepPrim_OneAxis::BottomFace, py::return_value_policy::reference_internal)
        .def("start_face", &BRepPrim_OneAxis::StartFace, py::return_value_policy::reference_internal)
        .def("end_face", &BRepPrim_OneAxis::EndFace, py::return_value_policy::reference_internal)
        
        // Wire access methods
        .def("lateral_wire", &BRepPrim_OneAxis::LateralWire, py::return_value_policy::reference_internal)
        .def("lateral_start_wire", &BRepPrim_OneAxis::LateralStartWire, py::return_value_policy::reference_internal)
        .def("lateral_end_wire", &BRepPrim_OneAxis::LateralEndWire, py::return_value_policy::reference_internal)
        .def("top_wire", &BRepPrim_OneAxis::TopWire, py::return_value_policy::reference_internal)
        .def("bottom_wire", &BRepPrim_OneAxis::BottomWire, py::return_value_policy::reference_internal)
        .def("start_wire", &BRepPrim_OneAxis::StartWire, py::return_value_policy::reference_internal)
        .def("axis_start_wire", &BRepPrim_OneAxis::AxisStartWire, py::return_value_policy::reference_internal)
        .def("end_wire", &BRepPrim_OneAxis::EndWire, py::return_value_policy::reference_internal)
        .def("axis_end_wire", &BRepPrim_OneAxis::AxisEndWire, py::return_value_policy::reference_internal)
        
        // Edge access methods
        .def("axis_edge", &BRepPrim_OneAxis::AxisEdge, py::return_value_policy::reference_internal)
        .def("start_edge", &BRepPrim_OneAxis::StartEdge, py::return_value_policy::reference_internal)
        .def("end_edge", &BRepPrim_OneAxis::EndEdge, py::return_value_policy::reference_internal)
        .def("start_top_edge", &BRepPrim_OneAxis::StartTopEdge, py::return_value_policy::reference_internal)
        .def("start_bottom_edge", &BRepPrim_OneAxis::StartBottomEdge, py::return_value_policy::reference_internal)
        .def("end_top_edge", &BRepPrim_OneAxis::EndTopEdge, py::return_value_policy::reference_internal)
        .def("end_bottom_edge", &BRepPrim_OneAxis::EndBottomEdge, py::return_value_policy::reference_internal)
        .def("top_edge", &BRepPrim_OneAxis::TopEdge, py::return_value_policy::reference_internal)
        .def("bottom_edge", &BRepPrim_OneAxis::BottomEdge, py::return_value_policy::reference_internal)
        
        // Vertex access methods
        .def("axis_top_vertex", &BRepPrim_OneAxis::AxisTopVertex, py::return_value_policy::reference_internal)
        .def("axis_bottom_vertex", &BRepPrim_OneAxis::AxisBottomVertex, py::return_value_policy::reference_internal)
        .def("top_start_vertex", &BRepPrim_OneAxis::TopStartVertex, py::return_value_policy::reference_internal)
        .def("top_end_vertex", &BRepPrim_OneAxis::TopEndVertex, py::return_value_policy::reference_internal)
        .def("bottom_start_vertex", &BRepPrim_OneAxis::BottomStartVertex, py::return_value_policy::reference_internal)
        .def("bottom_end_vertex", &BRepPrim_OneAxis::BottomEndVertex, py::return_value_policy::reference_internal)
    ;

    py::class_<BRepPrim_Revolution, BRepPrim_OneAxis>(m, "Revolution")
        // Main constructor
        .def(py::init<const gp_Ax2&, Standard_Real, Standard_Real, const opencascade::handle<Geom_Curve>&, const opencascade::handle<Geom2d_Curve>&>(),
             py::arg("A"), py::arg("VMin"), py::arg("VMax"), py::arg("M"), py::arg("PM"),
             "Create a revolution body. M is the meridian and must be in the XZ plane of A. PM is the meridian in the XZ plane.")
        
        // // Protected constructor (for derived classes)
        // .def(py::init<const gp_Ax2&, Standard_Real, Standard_Real>(),
        //      py::arg("A"), py::arg("VMin"), py::arg("VMax"),
        //      "Create a revolution body. The meridian is set later. Reserved for derived classes.")
        
        // Virtual methods from BRepPrim_OneAxis interface
        .def("make_empty_lateral_face", &BRepPrim_Revolution::MakeEmptyLateralFace,
             "The surface normal should be directed towards the outside.")
        
        .def("make_empty_meridian_edge", &BRepPrim_Revolution::MakeEmptyMeridianEdge, py::arg("Ang"),
             "Returns an edge with a 3D curve made from the meridian in the XZ plane rotated by Ang around the Z-axis. Ang may be 0 or myAngle.")
        
        .def("meridian_value", &BRepPrim_Revolution::MeridianValue, py::arg("V"),
             "Returns the meridian point at parameter V in the plane XZ.")
        
        .def("set_meridian_pcurve", &BRepPrim_Revolution::SetMeridianPCurve, py::arg("E"), py::arg("F"),
             "Sets the parametric curve of the edge E in the face F to be the 2d representation of the meridian.")
    ;


}