#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>
#include <ShapeUpgrade_UnifySameDomain.hxx>
#include <TopoDS_Shape.hxx>
#include <BRepTools_History.hxx>
#include <heal.hxx>

namespace py = pybind11;
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_shape_upgrade(py::module_ &m)
{
    m.def("union_faces", &union_faces,
          py::arg("shape"),
          "Unifies faces of a shape that lie on the same geometry.\n\n"
          "Faces are considered 'same-domain' if a group of neighbouring\n"
          "faces lie on coincident surfaces. In this case these faces can be\n"
          "unified into one face.\n\n"
          "This is a healing operation that can be used to clean up imported\n"
          "models or to prepare shapes for downstream operations.\n\n");
    // =========================================================================
    // ShapeUpgrade_UnifySameDomain - Unify faces/edges on same geometry
    // =========================================================================
    py::class_<ShapeUpgrade_UnifySameDomain, opencascade::handle<ShapeUpgrade_UnifySameDomain>>(
        m, "UnifySameDomain",
        R"doc(
        Unifies faces and edges of a shape that lie on the same geometry.

        Faces/edges are considered 'same-domain' if a group of neighbouring
        faces/edges lie on coincident surfaces/curves. In this case these
        faces/edges can be unified into one face/edge.

        Parameters (constructor or initialize):
            shape: The input shape to process
            unify_edges: Try to unify all possible edges (default: True)
            unify_faces: Try to unify all possible faces (default: True)  
            concat_bsplines: Merge neighbouring edges on BSpline/Bezier curves
                with C1 continuity (default: False)

        Example:
            unifier = ShapeUpgrade.UnifySameDomain(shape)
            unifier.build()
            result = unifier.shape
        )doc")
        
        .def(py::init<>(),
            "Creates an empty unifier.")
        
        .def(py::init<const TopoDS_Shape&, Standard_Boolean, Standard_Boolean, Standard_Boolean>(),
            py::arg("shape"),
            py::arg("unify_edges") = true,
            py::arg("unify_faces") = true,
            py::arg("concat_bsplines") = false,
            "Creates a unifier with the given shape and options.")
        
        .def("initialize", &ShapeUpgrade_UnifySameDomain::Initialize,
            py::arg("shape"),
            py::arg("unify_edges") = true,
            py::arg("unify_faces") = true,
            py::arg("concat_bsplines") = false,
            "Initializes with a shape and options. Does not perform unification.")
        
        .def("allow_internal_edges", &ShapeUpgrade_UnifySameDomain::AllowInternalEdges,
            py::arg("allow"),
            "Sets whether internal edges can be created inside merged faces\n"
            "in non-manifold topology. Default is False.")
        
        .def("keep_shape", &ShapeUpgrade_UnifySameDomain::KeepShape,
            py::arg("shape"),
            "Marks a shape (vertex or edge) to avoid merging.\n"
            "If vertex: forbids merging of connected edges.\n"
            "If edge: forbids merging of connected faces.")
        
        .def("set_safe_input_mode", &ShapeUpgrade_UnifySameDomain::SetSafeInputMode,
            py::arg("safe"),
            "Sets whether the original shape can be modified. Default is True (safe).")
        
        .def("set_linear_tolerance", &ShapeUpgrade_UnifySameDomain::SetLinearTolerance,
            py::arg("tolerance"),
            "Sets the linear tolerance (chord error for merging decisions).\n"
            "Default is Precision::Confusion().")
        
        .def("set_angular_tolerance", &ShapeUpgrade_UnifySameDomain::SetAngularTolerance,
            py::arg("tolerance"),
            "Sets the angular tolerance. Shapes with connection angle greater\n"
            "than this value will not be merged. Default is Precision::Angular().")
        
        .def("build", &ShapeUpgrade_UnifySameDomain::Build,
            "Performs unification and builds the resulting shape.")
        
        .def_property_readonly("shape", &ShapeUpgrade_UnifySameDomain::Shape,
            py::return_value_policy::reference_internal,
            "Returns the resulting unified shape.")
    ;
}