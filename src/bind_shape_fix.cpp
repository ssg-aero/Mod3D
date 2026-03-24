#include <pybind11/pybind11.h>
#include <Standard_Handle.hxx>

#include <ShapeFix.hxx>
#include <ShapeFix_Shape.hxx>
#include <ShapeFix_Wire.hxx>
#include <ShapeFix_Root.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Face.hxx>
#include <ShapeExtend_WireData.hxx>
#include <ShapeAnalysis_Wire.hxx>

namespace py = pybind11;
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_shape_fix(py::module_ &m)
{
    // =========================================================================
    // ShapeFix_Shape - Tools for repairing shapes
    // =========================================================================
    py::class_<ShapeFix_Shape, opencascade::handle<ShapeFix_Shape>>(m, "Shape",
        R"doc(
        A tool for repairing shapes in general.

        ShapeFix_Shape iterates on sub-shapes and performs fixes on each level:
        solids, shells, faces, wires, and edges.

        The fixes can be controlled via mode properties (fix_solid_mode, etc.).
        Use perform() to run all applicable fixes.

        Example:
            fixer = ShapeFix.Shape(shape)
            fixer.perform()
            fixed_shape = fixer.shape

        Or with configuration:
            fixer = ShapeFix.Shape()
            fixer.init(shape)
            fixer.set_precision(0.001)
            fixer.fix_solid_mode = 1  # Force
            fixer.perform()
            fixed_shape = fixer.shape
        )doc")
        
        .def(py::init<>(),
            "Creates an empty shape fixer.")
        
        .def(py::init<const TopoDS_Shape&>(),
            py::arg("shape"),
            "Creates a shape fixer initialized with the given shape.")
        
        .def("init", &ShapeFix_Shape::Init,
            py::arg("shape"),
            "Initializes with a shape.")
        
        .def("perform", [](ShapeFix_Shape& self) { return self.Perform(); },
            "Iterates on sub-shapes and performs fixes.\n\n"
            "Returns True if any fix was applied.")
        
        .def_property_readonly("shape", &ShapeFix_Shape::Shape,
            "Returns the resulting fixed shape.")
        
        .def("set_precision", &ShapeFix_Shape::SetPrecision,
            py::arg("precision"),
            "Sets the basic precision value.")
        
        .def("set_min_tolerance", &ShapeFix_Shape::SetMinTolerance,
            py::arg("min_tolerance"),
            "Sets minimal allowed tolerance.")
        
        .def("set_max_tolerance", &ShapeFix_Shape::SetMaxTolerance,
            py::arg("max_tolerance"),
            "Sets maximal allowed tolerance.")
        
        // Mode properties (read-write)
        .def_property("fix_solid_mode",
            [](const ShapeFix_Shape& self) { return const_cast<ShapeFix_Shape&>(self).FixSolidMode(); },
            [](ShapeFix_Shape& self, Standard_Integer value) { self.FixSolidMode() = value; },
            "Mode for fixing solids (-1 default, 0 skip, 1 force). Default True.")
        
        .def_property("fix_free_shell_mode",
            [](const ShapeFix_Shape& self) { return const_cast<ShapeFix_Shape&>(self).FixFreeShellMode(); },
            [](ShapeFix_Shape& self, Standard_Integer value) { self.FixFreeShellMode() = value; },
            "Mode for fixing free shells (-1 default, 0 skip, 1 force). Default True.")
        
        .def_property("fix_free_face_mode",
            [](const ShapeFix_Shape& self) { return const_cast<ShapeFix_Shape&>(self).FixFreeFaceMode(); },
            [](ShapeFix_Shape& self, Standard_Integer value) { self.FixFreeFaceMode() = value; },
            "Mode for fixing free faces (-1 default, 0 skip, 1 force). Default True.")
        
        .def_property("fix_free_wire_mode",
            [](const ShapeFix_Shape& self) { return const_cast<ShapeFix_Shape&>(self).FixFreeWireMode(); },
            [](ShapeFix_Shape& self, Standard_Integer value) { self.FixFreeWireMode() = value; },
            "Mode for fixing free wires (-1 default, 0 skip, 1 force). Default True.")
        
        .def_property("fix_same_parameter_mode",
            [](const ShapeFix_Shape& self) { return const_cast<ShapeFix_Shape&>(self).FixSameParameterMode(); },
            [](ShapeFix_Shape& self, Standard_Integer value) { self.FixSameParameterMode() = value; },
            "Mode for applying SameParameter after all fixes (-1 default, 0 skip, 1 force). Default True.")
        
        .def_property("fix_vertex_position_mode",
            [](const ShapeFix_Shape& self) { return const_cast<ShapeFix_Shape&>(self).FixVertexPositionMode(); },
            [](ShapeFix_Shape& self, Standard_Integer value) { self.FixVertexPositionMode() = value; },
            "Mode for fixing vertex positions before all fixes (-1 default, 0 skip, 1 force). Default False.")
        
        .def_property("fix_vertex_tol_mode",
            [](const ShapeFix_Shape& self) { return const_cast<ShapeFix_Shape&>(self).FixVertexTolMode(); },
            [](ShapeFix_Shape& self, Standard_Integer value) { self.FixVertexTolMode() = value; },
            "Mode for fixing tolerances of vertices on whole shape after all fixes.")
    ;

    // =========================================================================
    // ShapeFix_Wire - Tools for repairing wires
    // =========================================================================
    py::class_<ShapeFix_Wire, opencascade::handle<ShapeFix_Wire>>(m, "Wire",
        R"doc(
        A tool for repairing wires.

        ShapeFix_Wire provides methods to fix various problems in wires:
        - Reordering edges
        - Fixing small edges
        - Fixing connectivity between edges
        - Fixing degenerated edges
        - Fixing self-intersections
        - Fixing gaps between edges
        - And more...

        The fixes can be controlled via mode flags (Fix...Mode properties).
        Use perform() to run all applicable fixes, or call individual Fix...
        methods for specific repairs.

        Example:
            fixer = ShapeFix.Wire(wire, face, precision)
            fixer.perform()  # Run all fixes
            fixed_wire = fixer.wire

        Or with more control:
            fixer = ShapeFix.Wire()
            fixer.init(wire, face, 0.001)
            fixer.fix_reorder()
            fixer.fix_connected()
            fixer.fix_degenerated()
            fixed_wire = fixer.wire
        )doc")
        
        .def(py::init<>(),
            "Creates an empty wire fixer with default flags.")
        
        .def(py::init<const TopoDS_Wire&, const TopoDS_Face&, Standard_Real>(),
            py::arg("wire"), py::arg("face"), py::arg("precision"),
            "Creates a wire fixer and loads data for the wire and face.")
        
        .def("clear_modes", &ShapeFix_Wire::ClearModes,
            "Sets all modes to default values.")
        
        .def("clear_statuses", &ShapeFix_Wire::ClearStatuses,
            "Clears all statuses.")
        
        .def("init", py::overload_cast<const TopoDS_Wire&, const TopoDS_Face&, Standard_Real>(
            &ShapeFix_Wire::Init),
            py::arg("wire"), py::arg("face"), py::arg("precision"),
            "Loads analyzer with all data for the wire and face, and drops all fixing statuses.")
        
        .def("load", py::overload_cast<const TopoDS_Wire&>(&ShapeFix_Wire::Load),
            py::arg("wire"),
            "Loads data for the wire and drops all fixing statuses.")
        
        .def("set_face", &ShapeFix_Wire::SetFace,
            py::arg("face"),
            "Sets the working face for the wire.")
        
        .def("set_precision", &ShapeFix_Wire::SetPrecision,
            py::arg("precision"),
            "Sets working precision.")
        
        .def("set_max_tail_angle", &ShapeFix_Wire::SetMaxTailAngle,
            py::arg("max_tail_angle"),
            "Sets the maximal allowed angle of the tails in radians.")
        
        .def("set_max_tail_width", &ShapeFix_Wire::SetMaxTailWidth,
            py::arg("max_tail_width"),
            "Sets the maximal allowed width of the tails.")
        
        .def_property_readonly("is_loaded", &ShapeFix_Wire::IsLoaded,
            "Returns True if the wire is loaded.")
        
        .def_property_readonly("is_ready", &ShapeFix_Wire::IsReady,
            "Returns True if the wire and face are loaded.")
        
        .def_property_readonly("nb_edges", &ShapeFix_Wire::NbEdges,
            "Returns the number of edges in the working wire.")
        
        .def_property_readonly("wire", &ShapeFix_Wire::Wire,
            "Returns the resulting wire (using basic BRep_Builder).")
        
        .def_property_readonly("wire_api_make", &ShapeFix_Wire::WireAPIMake,
            "Returns the resulting wire (using BRepAPI_MakeWire).")
        
        .def_property_readonly("face", &ShapeFix_Wire::Face,
            py::return_value_policy::reference_internal,
            "Returns the working face.")
        
        // Mode properties (read-write)
        .def_property("modify_topology_mode",
            [](const ShapeFix_Wire& self) { return const_cast<ShapeFix_Wire&>(self).ModifyTopologyMode(); },
            [](ShapeFix_Wire& self, Standard_Boolean value) { self.ModifyTopologyMode() = value; },
            "Flag defining whether topology modification is allowed during fixing.")
        
        .def_property("modify_geometry_mode",
            [](const ShapeFix_Wire& self) { return const_cast<ShapeFix_Wire&>(self).ModifyGeometryMode(); },
            [](ShapeFix_Wire& self, Standard_Boolean value) { self.ModifyGeometryMode() = value; },
            "Flag defining whether geometry modification is allowed during fixing.")
        
        .def_property("closed_wire_mode",
            [](const ShapeFix_Wire& self) { return const_cast<ShapeFix_Wire&>(self).ClosedWireMode(); },
            [](ShapeFix_Wire& self, Standard_Boolean value) { self.ClosedWireMode() = value; },
            "Flag defining whether the wire is to be closed.")
        
        .def_property("fix_reorder_mode",
            [](const ShapeFix_Wire& self) { return const_cast<ShapeFix_Wire&>(self).FixReorderMode(); },
            [](ShapeFix_Wire& self, Standard_Integer value) { self.FixReorderMode() = value; },
            "Mode for FixReorder (-1 default, 0 skip, 1 force).")
        
        .def_property("fix_small_mode",
            [](const ShapeFix_Wire& self) { return const_cast<ShapeFix_Wire&>(self).FixSmallMode(); },
            [](ShapeFix_Wire& self, Standard_Integer value) { self.FixSmallMode() = value; },
            "Mode for FixSmall (-1 default, 0 skip, 1 force).")
        
        .def_property("fix_connected_mode",
            [](const ShapeFix_Wire& self) { return const_cast<ShapeFix_Wire&>(self).FixConnectedMode(); },
            [](ShapeFix_Wire& self, Standard_Integer value) { self.FixConnectedMode() = value; },
            "Mode for FixConnected (-1 default, 0 skip, 1 force).")
        
        .def_property("fix_degenerated_mode",
            [](const ShapeFix_Wire& self) { return const_cast<ShapeFix_Wire&>(self).FixDegeneratedMode(); },
            [](ShapeFix_Wire& self, Standard_Integer value) { self.FixDegeneratedMode() = value; },
            "Mode for FixDegenerated (-1 default, 0 skip, 1 force).")
        
        .def_property("fix_self_intersection_mode",
            [](const ShapeFix_Wire& self) { return const_cast<ShapeFix_Wire&>(self).FixSelfIntersectionMode(); },
            [](ShapeFix_Wire& self, Standard_Integer value) { self.FixSelfIntersectionMode() = value; },
            "Mode for FixSelfIntersection (-1 default, 0 skip, 1 force).")
        
        .def_property("fix_lacking_mode",
            [](const ShapeFix_Wire& self) { return const_cast<ShapeFix_Wire&>(self).FixLackingMode(); },
            [](ShapeFix_Wire& self, Standard_Integer value) { self.FixLackingMode() = value; },
            "Mode for FixLacking (-1 default, 0 skip, 1 force).")
        
        .def_property("fix_gaps_3d_mode",
            [](const ShapeFix_Wire& self) { return const_cast<ShapeFix_Wire&>(self).FixGaps3dMode(); },
            [](ShapeFix_Wire& self, Standard_Integer value) { self.FixGaps3dMode() = value; },
            "Mode for FixGaps3d (-1 default, 0 skip, 1 force).")
        
        .def_property("fix_gaps_2d_mode",
            [](const ShapeFix_Wire& self) { return const_cast<ShapeFix_Wire&>(self).FixGaps2dMode(); },
            [](ShapeFix_Wire& self, Standard_Integer value) { self.FixGaps2dMode() = value; },
            "Mode for FixGaps2d (-1 default, 0 skip, 1 force).")
        
        // Fix methods (API level)
        .def("perform", &ShapeFix_Wire::Perform,
            "Performs all available fixes based on mode flags.\n\n"
            "Returns True if any fix was applied.")
        
        .def("fix_reorder", py::overload_cast<Standard_Boolean>(&ShapeFix_Wire::FixReorder),
            py::arg("mode_both") = false,
            "Reorders edges in the wire using class WireOrder.\n\n"
            "Returns True if reordering was performed.")
        
        .def("fix_small", py::overload_cast<Standard_Boolean, Standard_Real>(&ShapeFix_Wire::FixSmall),
            py::arg("lock_vertex"), py::arg("precision_small") = 0.0,
            "Removes small edges from the wire.\n\n"
            "Parameters:\n"
            "  lock_vertex: If True, vertices are not modified\n"
            "  precision_small: Threshold for small edges (0 = use default)\n\n"
            "Returns the number of removed edges.")
        
        .def("fix_connected", py::overload_cast<Standard_Real>(&ShapeFix_Wire::FixConnected),
            py::arg("precision") = -1.0,
            "Fixes connections between adjacent edges.\n\n"
            "Forces vertices to be shared between adjacent edges.\n"
            "If precision is -1, MaxTolerance is used.\n\n"
            "Returns True if any fix was applied.")
        
        .def("fix_edge_curves", &ShapeFix_Wire::FixEdgeCurves,
            "Fixes issues with 3D and pcurves of edges.\n\n"
            "Returns True if any fix was applied.")
        
        .def("fix_degenerated", py::overload_cast<>(&ShapeFix_Wire::FixDegenerated),
            "Fixes degenerated edges in the wire.\n\n"
            "Returns True if any fix was applied.")
        
        .def("fix_self_intersection", &ShapeFix_Wire::FixSelfIntersection,
            "Fixes self-intersecting edges and intersections between edges.\n\n"
            "Returns True if any fix was applied.")
        
        .def("fix_lacking", py::overload_cast<Standard_Boolean>(&ShapeFix_Wire::FixLacking),
            py::arg("force") = false,
            "Fixes lacking edges (gaps in 2D while connected in 3D).\n\n"
            "Parameters:\n"
            "  force: If True, uses minimal tolerance for gap detection\n\n"
            "Returns True if edge was added or tolerance was increased.")
        
        .def("fix_closed", &ShapeFix_Wire::FixClosed,
            py::arg("precision") = -1.0,
            "Fixes wire to be well closed.\n\n"
            "Performs FixConnected, FixDegenerated, and FixLacking between\n"
            "last and first edges.\n\n"
            "Returns True if any fix was applied.")
        
        .def("fix_gaps_3d", &ShapeFix_Wire::FixGaps3d,
            "Fixes gaps between ends of 3D curves on adjacent edges.\n\n"
            "Returns True if any fix was applied.")
        
        .def("fix_gaps_2d", &ShapeFix_Wire::FixGaps2d,
            "Fixes gaps between ends of pcurves on adjacent edges.\n\n"
            "Returns True if any fix was applied.")
        
        .def("fix_shifted", &ShapeFix_Wire::FixShifted,
            "Fixes edges with pcurves shifted by parameter range on closed surface.\n\n"
            "Returns True if any fix was applied.")
        
        .def("fix_notched_edges", &ShapeFix_Wire::FixNotchedEdges,
            "Fixes notched edges.\n\n"
            "Returns True if any fix was applied.")
        
        .def("fix_tails", &ShapeFix_Wire::FixTails,
            "Fixes tails in the wire.\n\n"
            "Returns True if any fix was applied.")
    ;

}