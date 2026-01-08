#include <TopoDS_Edge.hxx>
#include <pybind11/pybind11.h>
#include <pybind11/pytypes.h>
#include <pybind11/stl.h>
#include <BRepAlgoAPI_Algo.hxx>
#include <BRepAlgoAPI_BuilderAlgo.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Section.hxx>
#include <BOPAlgo_Options.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS.hxx>
#include <TopTools_ListOfShape.hxx>

namespace py = pybind11;

void bind_brep_boolean_op(py::module_ &m)
{
    /*
    // BOPAlgo_Options - Base class for Boolean operation options
    py::class_<BOPAlgo_Options>(m, "BOPAlgoOptions",
        R"(Base class for Boolean operation options.
        
        Provides common options for algorithms in Boolean Component:
        - Memory allocation tool
        - Error and warning reporting
        - Parallel processing mode
        - Fuzzy tolerance for detecting touching/coinciding cases
        - Oriented Bounding Boxes usage)")
        
        .def(py::init<>(), "Default constructor")
        
        // Fuzzy tolerance
        .def_property("fuzzy_value",
            &BOPAlgo_Options::FuzzyValue,
            &BOPAlgo_Options::SetFuzzyValue,
            "Additional tolerance for operations (read/write)")
        
        // Parallel processing

        .def_property("run_parallel",
            &BOPAlgo_Options::RunParallel,
            &BOPAlgo_Options::SetRunParallel,
            "Enable/disable parallel processing (read/write)")
        
        .def_property("parallel_mode",
            &BOPAlgo_Options::GetParallelMode,
            &BOPAlgo_Options::SetParallelMode,
            "Get/Set parallel mode (read/write)")
        
        // Oriented Bounding Boxes
        .def_property("use_obb",
            &BOPAlgo_Options::UseOBB,
            &BOPAlgo_Options::SetUseOBB,
            "Enable/disable usage of Oriented Bounding Boxes (read/write)")
        
        // Error and warning handling
        .def("has_errors", &BOPAlgo_Options::HasErrors,
            "Returns True if algorithm has failed with errors")
        
        .def("has_warnings", &BOPAlgo_Options::HasWarnings,
            "Returns True if algorithm has generated warnings")
        
        .def("dump_errors", &BOPAlgo_Options::DumpErrors,
            py::arg("stream"),
            R"(Dumps error status to the output stream.
            
            Parameters:
            -----------
            stream : ostream
                Output stream for error messages)")
        
        .def("dump_warnings", &BOPAlgo_Options::DumpWarnings,
            py::arg("stream"),
            R"(Dumps warning status to the output stream.
            
            Parameters:
            -----------
            stream : ostream
                Output stream for warning messages)")
        
        .def("clear", &BOPAlgo_Options::Clear,
            "Clears all warnings and errors, and any cached data")
        
        .def("clear_warnings", &BOPAlgo_Options::ClearWarnings,
            "Clears all warnings")
    ;
    */
    // BRepAlgoAPI_Algo - Base class for algorithms
    // Note: BOPAlgo_Options is inherited as protected, so we must use lambdas to access those methods
    // Note: Destructor is protected, so we use py::nodelete to prevent pybind11 from deleting it
    py::class_<BRepAlgoAPI_Algo, BRepBuilderAPI_MakeShape, std::unique_ptr<BRepAlgoAPI_Algo, py::nodelete>>(m, "Algo",
        R"(Root interface for Boolean operation algorithms.
        
        This is the base class providing common functionality for all Boolean
        and general fuse algorithms in OCCT. Note: This class has a protected
        constructor and cannot be instantiated directly - use derived classes.)")
        
        .def("shape", &BRepAlgoAPI_Algo::Shape,
            "Returns the result shape built by the algorithm")
        
        // Fuzzy tolerance (using lambdas because BOPAlgo_Options is protected base)
        .def_property("fuzzy_value",
            [](const BRepAlgoAPI_Algo& self) { return self.FuzzyValue(); },
            [](BRepAlgoAPI_Algo& self, double val) { self.SetFuzzyValue(val); },
            "Additional tolerance for operations (read/write)")
        
        // Parallel processing
        .def_property("run_parallel",
            [](const BRepAlgoAPI_Algo& self) { return self.RunParallel(); },
            [](BRepAlgoAPI_Algo& self, bool flag) { self.SetRunParallel(flag); },
            "Enable/disable parallel processing (read/write)")
        
        .def_static("get_parallel_mode", &BOPAlgo_Options::GetParallelMode,
            "Gets the global parallel mode setting")
        
        .def_static("set_parallel_mode", &BOPAlgo_Options::SetParallelMode,
            py::arg("new_mode"),
            "Sets the global parallel mode setting")
        
        // Oriented Bounding Boxes
        .def("set_use_obb", [](BRepAlgoAPI_Algo& self, bool flag) { self.SetUseOBB(flag); },
            py::arg("use_obb"),
            "Enables/disables the usage of Oriented Bounding Boxes")
        
        // Error and warning handling
        .def("has_errors", [](const BRepAlgoAPI_Algo& self) { return self.HasErrors(); },
            "Returns True if algorithm has failed with errors")
        
        .def("has_warnings", [](const BRepAlgoAPI_Algo& self) { return self.HasWarnings(); },
            "Returns True if algorithm has generated warnings")
        
        .def("clear", [](BRepAlgoAPI_Algo& self) { self.Clear(); },
            "Clears all warnings and errors, and any cached data")
        
        .def("clear_warnings", [](BRepAlgoAPI_Algo& self) { self.ClearWarnings(); },
            "Clears all warnings")
    ;
    
    // BRepAlgoAPI_BuilderAlgo - General fuse algorithm
    py::class_<BRepAlgoAPI_BuilderAlgo, BRepAlgoAPI_Algo>(m, "BuilderAlgo",
        R"(General Fuse algorithm for Boolean operations.
        
        This class provides advanced Boolean operations with support for
        multiple arguments, non-manifold topology, and sophisticated error handling.
        
        Key features:
        - Safe processing mode (non-destructive)
        - Gluing options for performance optimization
        - Check for inverted solids
        - History collection of shape modifications)")
        
        .def(py::init<>(), "Default constructor")
        
        .def("shape", &BRepAlgoAPI_Algo::Shape,
            "Returns the result shape built by the algorithm")

        // Arguments management
        .def("set_arguments", [](BRepAlgoAPI_BuilderAlgo& self, const py::list& shapes) {
            TopTools_ListOfShape arguments;
            for (const auto& item : shapes) {
            arguments.Append(item.cast<TopoDS_Shape>());
            }
            self.SetArguments(arguments);
        },
            py::arg("arguments"),
            R"(Sets the list of arguments (shapes) for the operation.
            
            Parameters:
            -----------
            arguments : list[TopoDS_Shape]
            List of input shapes)")
        
        .def("arguments", [](const BRepAlgoAPI_BuilderAlgo& self)
        {
            py::list args;
            for(const auto& arg: self.Arguments())
            {
                args.append(arg);
            }
            return args;
        },
            "Returns the list of arguments")
        
        // Non-destructive mode
        .def("set_non_destructive", &BRepAlgoAPI_BuilderAlgo::SetNonDestructive,
            py::arg("flag"),
            R"(Sets the flag for non-destructive mode.
            
            In non-destructive mode, input shapes are not modified. Instead,
            a copy of a sub-shape is created in the result if needed.
            
            Parameters:
            -----------
            flag : bool
                True to preserve input shapes)")
        
        .def("non_destructive", &BRepAlgoAPI_BuilderAlgo::NonDestructive,
            "Returns True if non-destructive mode is enabled")
        
        .def_property("non_destructive",
            &BRepAlgoAPI_BuilderAlgo::NonDestructive,
            &BRepAlgoAPI_BuilderAlgo::SetNonDestructive,
            "Enable/disable non-destructive mode (read/write)")
        
        // Glue option
        .def("set_glue", &BRepAlgoAPI_BuilderAlgo::SetGlue,
            py::arg("glue"),
            R"(Sets the glue option for the algorithm.
            
            Allows increasing performance of intersection when some sub-shapes
            are coinciding.
            
            Parameters:
            -----------
            glue : BOPAlgo_GlueEnum
                Gluing mode setting)")
        
        .def("glue", &BRepAlgoAPI_BuilderAlgo::Glue,
            "Returns the glue option of the algorithm")
        
        .def_property("glue",
            &BRepAlgoAPI_BuilderAlgo::Glue,
            &BRepAlgoAPI_BuilderAlgo::SetGlue,
            "Gluing mode for performance optimization (read/write)")
        
        // Check inverted solids
        .def_property("check_inverted",
            &BRepAlgoAPI_BuilderAlgo::CheckInverted,
            &BRepAlgoAPI_BuilderAlgo::SetCheckInverted,
            "Enable/disable check for inverted solids (read/write)")
        
        // History collection
        .def_property("fill_history",
            &BRepAlgoAPI_BuilderAlgo::HasHistory,
            &BRepAlgoAPI_BuilderAlgo::SetToFillHistory,
            "Enable/disable history collection (read/write)")
        
        // Building
        .def("build", &BRepAlgoAPI_BuilderAlgo::Build,
            "Performs the algorithm and builds the result shape")
        
        // Result simplification
        .def("simplify_result", &BRepAlgoAPI_BuilderAlgo::SimplifyResult,
            py::arg("unify_edges") = true,
            py::arg("unify_faces") = true,
            py::arg("angular_tol") = 1e-4,
            R"(Simplifies the result by unifying tangential edges and faces.
            
            Uses ShapeUpgrade_UnifySameDomain algorithm to simplify the result
            without breaking shape connections. Works on the whole result shape,
            including unmodified connected tangent edges/faces from input.
            
            Parameters:
            -----------
            unify_edges : bool
                Controls edge unification (default: True)
            unify_faces : bool
                Controls face unification (default: True)
            angular_tol : float
                Angular criteria for tangency (default: 1e-4))")
        
        // History queries
        .def("modified", [](BRepAlgoAPI_BuilderAlgo& self, const TopoDS_Shape& shape){
                py::list modified;
                for(const auto& modified_sub_shape : self.Modified(shape))
                {
                    modified.append(modified_sub_shape);
                }
                return modified;
            },
            py::arg("shape"),
            R"(Returns shapes modified from the given shape.
            
            Parameters:
            -----------
            shape : TopoDS_Shape
                Original shape
                
            Returns:
            --------
            TopTools_ListOfShape
                List of modified shapes (splits) contained in the result)")
        
        .def("generated", [](BRepAlgoAPI_BuilderAlgo& self, const TopoDS_Shape& shape){
                py::list generated;
                for(const auto& generated_item : self.Generated(shape))
                {
                    generated.append(generated_item);
                }
                return generated;
            },
            py::arg("shape"),
            R"(Returns shapes generated from the given shape.
            
            Only Edges and Faces can have generated elements:
            - Edges can generate new vertices
            - Faces can generate new edges and vertices
            
            Parameters:
            -----------
            shape : TopoDS_Shape
                Original shape
                
            Returns:
            --------
            TopTools_ListOfShape
                List of generated shapes)"
)
        .def("is_deleted", &BRepAlgoAPI_BuilderAlgo::IsDeleted,
            py::arg("shape"),
            R"(Checks if the shape has been completely removed from the result.
            
            Parameters:
            -----------
            shape : TopoDS_Shape
                Shape to check
                
            Returns:
            --------
            bool
                True if the shape has been deleted)")
        
        .def("has_modified", &BRepAlgoAPI_BuilderAlgo::HasModified,
            "Returns True if any input shape has been modified during operation")
        
        .def("has_generated", &BRepAlgoAPI_BuilderAlgo::HasGenerated,
            "Returns True if any input shape has generated shapes during operation")
        
        .def("has_deleted", &BRepAlgoAPI_BuilderAlgo::HasDeleted,
            "Returns True if any input shape has been deleted during operation")
        
        // Section edges
        .def("section_edges", [](BRepAlgoAPI_BuilderAlgo& self){
                py::list sections;
                for( auto& shape : self.SectionEdges())
                {
                    sections.append(TopoDS::Edge(shape));
                }
                return sections;
            },
            R"(Returns a list of section edges.
            
            The edges represent the result of intersection between arguments
            of the operation.
            
            Returns:
            --------
            TopTools_ListOfShape
                List of section edges)"
        )
    ;
    
    // BRepAlgoAPI_Common - Boolean intersection
    py::class_<BRepAlgoAPI_Common, BRepAlgoAPI_BuilderAlgo>(m, "Common",
        R"(Boolean INTERSECTION operation.
        
        Computes the common (intersection) part between two or more shapes.
        The result contains only the volume/area that exists in ALL input shapes.)")
        
        .def(py::init<>(), "Default constructor")
        
        .def(py::init<const TopoDS_Shape&, const TopoDS_Shape&>(),
            py::arg("shape1"),
            py::arg("shape2"),
            R"(Constructor with two shapes.
            
            Parameters:
            -----------
            shape1 : TopoDS_Shape
                First shape
            shape2 : TopoDS_Shape
                Second shape)")
    ;
    
    // BRepAlgoAPI_Cut - Boolean subtraction
    py::class_<BRepAlgoAPI_Cut, BRepAlgoAPI_BuilderAlgo>(m, "Cut",
        R"(Boolean SUBTRACTION (difference) operation.
        
        Subtracts the tool shape(s) from the object shape.
        The result is the part of the object that does NOT overlap with the tools.)")
        
        .def(py::init<>(), "Default constructor")
        
        .def(py::init<const TopoDS_Shape&, const TopoDS_Shape&>(),
            py::arg("shape"),
            py::arg("tool"),
            R"(Constructor with object and tool shapes.
            
            Parameters:
            -----------
            shape : TopoDS_Shape
                Object shape to cut from
            tool : TopoDS_Shape
                Tool shape to subtract)")
    ;
    
    // BRepAlgoAPI_Fuse - Boolean union
    py::class_<BRepAlgoAPI_Fuse, BRepAlgoAPI_BuilderAlgo>(m, "Fuse",
        R"(Boolean UNION operation.
        
        Fuses (merges) two or more shapes into a single shape.
        The result contains the combined volume/area of all input shapes.)")
        
        .def(py::init<>(), "Default constructor")
        
        .def(py::init<const TopoDS_Shape&, const TopoDS_Shape&>(),
            py::arg("shape1"),
            py::arg("shape2"),
            R"(Constructor with two shapes.
            
            Parameters:
            -----------
            shape1 : TopoDS_Shape
                First shape
            shape2 : TopoDS_Shape
                Second shape)")
    ;
    
    // BRepAlgoAPI_Section - Boolean section (intersection curve)
    py::class_<BRepAlgoAPI_Section, BRepAlgoAPI_BuilderAlgo>(m, "Section",
        R"(Computes the intersection curve/edge between shapes.
        
        Unlike Common which gives the volume/area, Section computes only the
        curve/edge where the shapes intersect.)")
        
        .def(py::init<>(), "Default constructor")
        
        .def(py::init<const TopoDS_Shape&, const TopoDS_Shape&>(),
            py::arg("shape1"),
            py::arg("shape2"),
            R"(Constructor with two shapes.
            
            Parameters:
            -----------
            shape1 : TopoDS_Shape
                First shape
            shape2 : TopoDS_Shape
                Second shape)")
        
        .def(py::init<const TopoDS_Shape&, const TopoDS_Shape&, const Standard_Boolean>(),
            py::arg("shape1"),
            py::arg("shape2"),
            py::arg("perform_now"),
            R"(Constructor with option to compute immediately.
            
            Parameters:
            -----------
            shape1 : TopoDS_Shape
                First shape
            shape2 : TopoDS_Shape
                Second shape
            perform_now : bool
                If True, compute the section immediately)")
        
        .def("approximation", &BRepAlgoAPI_Section::Approximation,
            py::arg("flag"),
            R"(Sets whether to approximate intersection curves.
            
            Parameters:
            -----------
            flag : bool
                True to enable approximation)")
        
        .def("compute_pcurve_on1", &BRepAlgoAPI_Section::ComputePCurveOn1,
            py::arg("flag"),
            R"(Sets whether to compute parametric curves on the first shape.
            
            Parameters:
            -----------
            flag : bool
                True to compute parametric curves)")
        
        .def("compute_pcurve_on2", &BRepAlgoAPI_Section::ComputePCurveOn2,
            py::arg("flag"),
            R"(Sets whether to compute parametric curves on the second shape.
            
            Parameters:
            -----------
            flag : bool
                True to compute parametric curves)")
    ;
}