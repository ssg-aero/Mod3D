#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <BRepLib.hxx>
#include <Geom_Plane.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopTools_ListOfShape.hxx>
#include <BRepTools_ReShape.hxx>
#include <Adaptor3d_Curve.hxx>
#include <gp_Pnt.hxx>
#include <GeomAbs_Shape.hxx>

namespace py = pybind11;
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_brep_lib(py::module_ &m)
{
    // Precision getter/setter
    m.def("set_precision", static_cast<void (*)(const Standard_Real)>(&BRepLib::Precision),
          py::arg("p"),
          "Sets the default precision.");

    m.def("precision", static_cast<Standard_Real (*)()>(&BRepLib::Precision),
          "Returns the default precision.");

    // Plane getter/setter
    m.def("set_plane", static_cast<void (*)(const Handle(Geom_Plane)&)>(&BRepLib::Plane),
          py::arg("p"),
          "Sets the current plane.");

    m.def("plane", static_cast<const Handle(Geom_Plane)& (*)()>(&BRepLib::Plane),
          "Returns the current plane.");

    // CheckSameRange
    m.def("check_same_range", &BRepLib::CheckSameRange,
          py::arg("edge"),
          py::arg("confusion") = 1.0e-12,
          "Checks if the Edge is same range ignoring the same range flag.");

    // SameRange for edge
    m.def("same_range", static_cast<void (*)(const TopoDS_Edge&, const Standard_Real)>(&BRepLib::SameRange),
          py::arg("edge"),
          py::arg("tolerance") = 1.0e-5,
          "Makes all curve representations have the same range domain.");

    // BuildCurve3d
    m.def("build_curve_3d", &BRepLib::BuildCurve3d,
          py::arg("edge"),
          py::arg("tolerance") = 1.0e-5,
          py::arg("continuity") = GeomAbs_C1,
          py::arg("max_degree") = 14,
          py::arg("max_segment") = 0,
          "Computes the 3d curve for the edge if it does not exist.");

    // BuildCurves3d overloads
    m.def("build_curves_3d", static_cast<Standard_Boolean (*)(const TopoDS_Shape&, const Standard_Real, const GeomAbs_Shape, const Standard_Integer, const Standard_Integer)>(&BRepLib::BuildCurves3d),
          py::arg("shape"),
          py::arg("tolerance"),
          py::arg("continuity") = GeomAbs_C1,
          py::arg("max_degree") = 14,
          py::arg("max_segment") = 0,
          "Computes 3d curves for all edges of the shape with tolerance.");

    m.def("build_curves_3d_with_defaults", static_cast<Standard_Boolean (*)(const TopoDS_Shape&)>(&BRepLib::BuildCurves3d),
          py::arg("shape"),
          "Computes 3d curves for all edges of the shape with default parameters.");

    // BuildPCurveForEdgeOnPlane (updates edge)
    m.def("build_pcurve_for_edge_on_plane", static_cast<void (*)(const TopoDS_Edge&, const TopoDS_Face&)>(&BRepLib::BuildPCurveForEdgeOnPlane),
          py::arg("edge"),
          py::arg("face"),
          "Builds pcurve of edge on face if the surface is plane, and updates the edge.");

    // UpdateEdgeTol
    m.def("update_edge_tol", &BRepLib::UpdateEdgeTol,
          py::arg("edge"),
          py::arg("min_tolerance_request"),
          py::arg("max_tolerance_to_check"),
          "Updates edge tolerance if needed. Returns True if updated.");

    // UpdateEdgeTolerance
    m.def("update_edge_tolerance", &BRepLib::UpdateEdgeTolerance,
          py::arg("shape"),
          py::arg("min_tolerance_request"),
          py::arg("max_tolerance_to_check"),
          "Updates tolerances for all edges of the shape.");

    // SameParameter for edge
    m.def("same_parameter", static_cast<void (*)(const TopoDS_Edge&, const Standard_Real)>(&BRepLib::SameParameter),
          py::arg("edge"),
          py::arg("tolerance") = 1.0e-5,
          "Computes new 2d curve(s) for the edge to have the same parameter as the 3d curve.");

    // SameParameter for shape
    m.def("same_parameter_shape", static_cast<void (*)(const TopoDS_Shape&, const Standard_Real, const Standard_Boolean)>(&BRepLib::SameParameter),
          py::arg("shape"),
          py::arg("tolerance") = 1.0e-5,
          py::arg("forced") = false,
          "Computes new 2d curve(s) for all edges of the shape.");

    // SameParameter for shape with reshaper
    m.def("same_parameter_reshape", static_cast<void (*)(const TopoDS_Shape&, BRepTools_ReShape&, const Standard_Real, const Standard_Boolean)>(&BRepLib::SameParameter),
          py::arg("shape"),
          py::arg("reshaper"),
          py::arg("tolerance") = 1.0e-5,
          py::arg("forced") = false,
          "Computes new 2d curve(s) for all edges, recording modifications in reshaper.");

    // UpdateTolerances
    m.def("update_tolerances", static_cast<void (*)(const TopoDS_Shape&, const Standard_Boolean)>(&BRepLib::UpdateTolerances),
          py::arg("shape"),
          py::arg("verify_face_tolerance") = false,
          "Replaces tolerance of FACE EDGE VERTEX by the tolerance Max of their connected shapes.");

    // UpdateTolerances with reshaper
    m.def("update_tolerances_reshape", static_cast<void (*)(const TopoDS_Shape&, BRepTools_ReShape&, const Standard_Boolean)>(&BRepLib::UpdateTolerances),
          py::arg("shape"),
          py::arg("reshaper"),
          py::arg("verify_face_tolerance") = false,
          "Replaces tolerances, recording modifications in reshaper.");

    // UpdateInnerTolerances
    m.def("update_inner_tolerances", &BRepLib::UpdateInnerTolerances,
          py::arg("shape"),
          "Checks tolerances of edges and vertices and updates them for SameParameter condition.");

    // OrientClosedSolid
    m.def("orient_closed_solid", &BRepLib::OrientClosedSolid,
          py::arg("solid"),
          "Orients the solid forward and shell to have matter in the solid.");

    // ContinuityOfFaces
    m.def("continuity_of_faces", &BRepLib::ContinuityOfFaces,
          py::arg("edge"),
          py::arg("face1"),
          py::arg("face2"),
          py::arg("angle_tol"),
          "Returns the order of continuity between two faces connected by an edge.");

    // EncodeRegularity overloads
    m.def("encode_regularity", static_cast<void (*)(const TopoDS_Shape&, const Standard_Real)>(&BRepLib::EncodeRegularity),
          py::arg("shape"),
          py::arg("tol_ang") = 1.0e-10,
          "Encodes the Regularity of edges on a Shape.");

    m.def("encode_regularity_edge", static_cast<void (*)(TopoDS_Edge&, const TopoDS_Face&, const TopoDS_Face&, const Standard_Real)>(&BRepLib::EncodeRegularity),
          py::arg("edge"),
          py::arg("face1"),
          py::arg("face2"),
          py::arg("tol_ang") = 1.0e-10,
          "Encodes the Regularity between face1 and face2 by edge.");

    // SortFaces
    m.def("sort_faces", [](const TopoDS_Shape& S) {
              TopTools_ListOfShape LF;
              BRepLib::SortFaces(S, LF);
              std::vector<TopoDS_Face> result;
              for (TopTools_ListIteratorOfListOfShape it(LF); it.More(); it.Next()) {
                  result.push_back(TopoDS::Face(it.Value()));
              }
              return result;
          },
          py::arg("shape"),
          "Sorts faces by surface complexity (Plane, Cylinder, Cone, Sphere, Torus, other).");

    // ReverseSortFaces
    m.def("reverse_sort_faces", [](const TopoDS_Shape& S) {
              TopTools_ListOfShape LF;
              BRepLib::ReverseSortFaces(S, LF);
              std::vector<TopoDS_Face> result;
              for (TopTools_ListIteratorOfListOfShape it(LF); it.More(); it.Next()) {
                  result.push_back(TopoDS::Face(it.Value()));
              }
              return result;
          },
          py::arg("shape"),
          "Sorts faces by reverse surface complexity.");

    // EnsureNormalConsistency
    m.def("ensure_normal_consistency", &BRepLib::EnsureNormalConsistency,
          py::arg("shape"),
          py::arg("ang_tol") = 0.001,
          py::arg("force_compute_normals") = false,
          "Corrects normals in Poly_Triangulation of faces along smooth edges.");

    // UpdateDeflection
    m.def("update_deflection", &BRepLib::UpdateDeflection,
          py::arg("shape"),
          "Updates value of deflection in Poly_Triangulation by the maximum deviation.");

    // FindValidRange for edge
    m.def("find_valid_range", static_cast<Standard_Boolean (*)(const TopoDS_Edge&, Standard_Real&, Standard_Real&)>(&BRepLib::FindValidRange),
          py::arg("edge"),
          py::arg("first"),
          py::arg("last"),
          "Finds a range of 3d curve of the edge not covered by vertices tolerances.");

    // ExtendFace
    m.def("extend_face", [](const TopoDS_Face& theF, Standard_Real theExtVal,
                            Standard_Boolean theExtUMin, Standard_Boolean theExtUMax,
                            Standard_Boolean theExtVMin, Standard_Boolean theExtVMax) {
              TopoDS_Face theFExtended;
              BRepLib::ExtendFace(theF, theExtVal, theExtUMin, theExtUMax, theExtVMin, theExtVMax, theFExtended);
              return theFExtended;
          },
          py::arg("face"),
          py::arg("ext_val"),
          py::arg("ext_u_min"),
          py::arg("ext_u_max"),
          py::arg("ext_v_min"),
          py::arg("ext_v_max"),
          "Enlarges the face on the given value in specified directions.");
}