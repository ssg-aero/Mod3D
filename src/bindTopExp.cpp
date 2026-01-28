#include <pybind11/pybind11.h>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Shape.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <pybind11/pytypes.h>

#include <explorer_utils.hpp>

namespace py = pybind11;

void bind_top_exp(py::module_ &m)
{
    py::class_<TopExp_Explorer>(m, "Explorer",
        "Tool to visit and explore a topological data structure.\n\n"
        "An Explorer visits all shapes in a structure to find shapes of a\n"
        "requested type that are not contained in a type to avoid.\n\n"
        "Example to find all faces in shape S:\n"
        "  explorer = TopExp.Explorer(S, TopAbs.ShapeEnum.FACE)\n"
        "  while explorer.more():\n"
        "      face = explorer.current()\n"
        "      process_face(face)\n"
        "      explorer.next()\n\n"
        "To find all vertices not in an edge:\n"
        "  explorer = TopExp.Explorer(S, TopAbs.ShapeEnum.VERTEX, TopAbs.ShapeEnum.EDGE)\n\n"
        "Note: If the type to avoid is the same or less complex than the type\n"
        "to find, it has no effect on the exploration")
        
        // Constructors
        .def(py::init<>(),
            "Creates an empty explorer.\n\n"
            "Becomes useful after calling init() to set the shape and search parameters")
        
        .def(py::init<const TopoDS_Shape&, const TopAbs_ShapeEnum, const TopAbs_ShapeEnum>(),
            py::arg("shape"), py::arg("to_find"), py::arg("to_avoid") = TopAbs_SHAPE,
            "Creates an Explorer on a shape.\n\n"
            "Parameters:\n"
            "  shape: The shape to explore\n"
            "  to_find: The type of shapes to search for\n"
            "           (e.g., TopAbs.ShapeEnum.VERTEX, TopAbs.ShapeEnum.EDGE)\n"
            "  to_avoid: The type of shapes to skip in the exploration (default: SHAPE)\n"
            "            If to_avoid is equal to or less complex than to_find,\n"
            "            or if to_avoid is SHAPE, it has no effect\n\n"
            "Example:\n"
            "  # Find all edges in a shape\n"
            "  explorer = TopExp.Explorer(shape, TopAbs.ShapeEnum.EDGE)\n"
            "  # Find all vertices not in edges\n"
            "  explorer = TopExp.Explorer(shape, TopAbs.ShapeEnum.VERTEX, TopAbs.ShapeEnum.EDGE)")
        
        // Initialization
        .def("init", &TopExp_Explorer::Init,
            py::arg("shape"), py::arg("to_find"), py::arg("to_avoid") = TopAbs_SHAPE,
            "Resets this explorer on a shape.\n\n"
            "Initializes the explorer to search the shape for shapes of type to_find\n"
            "that are not part of a shape to_avoid.\n\n"
            "Parameters:\n"
            "  shape: The shape to explore\n"
            "  to_find: The type of shapes to search for\n"
            "  to_avoid: The type of shapes to skip (default: SHAPE = no restriction)\n\n"
            "If to_avoid is equal to TopAbs.ShapeEnum.SHAPE, or if it is the same as\n"
            "or less complex than to_find, it has no effect on the search")
        
        .def("reinit", &TopExp_Explorer::ReInit,
            "Reinitializes the exploration with the original arguments.\n\n"
            "Restarts the exploration from the beginning with the same shape\n"
            "and search parameters that were used in the constructor or last init() call")
        
        // Navigation
        .def("more", &TopExp_Explorer::More,
            "Returns True if there are more shapes in the exploration.\n\n"
            "Use this in a loop condition to iterate through all matching shapes")
        
        .def("next", &TopExp_Explorer::Next,
            "Moves to the next shape in the exploration.\n\n"
            "Raises:\n"
            "  Standard_NoMoreObject if there are no more shapes to explore")
        
        .def("current", &TopExp_Explorer::Current,
            py::return_value_policy::reference_internal,
            "Returns the current shape in the exploration.\n\n"
            "Returns:\n"
            "  The current shape being visited\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if there are no more shapes to explore")
        
        .def("value", &TopExp_Explorer::Value,
            py::return_value_policy::reference_internal,
            "Returns the current shape in the exploration.\n\n"
            "This is an alias for current().\n\n"
            "Returns:\n"
            "  The current shape being visited\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if there are no more shapes to explore")
        
        // Query methods
        .def("clear", &TopExp_Explorer::Clear,
            "Clears the content of the explorer.\n\n"
            "After calling this, more() will return False")
        
        .def_property_readonly("explored_shape", &TopExp_Explorer::ExploredShape,
            py::return_value_policy::reference_internal,
            "Returns the shape being explored")
        
        .def_property_readonly("depth", &TopExp_Explorer::Depth,
            "Returns the current depth of the exploration.\n\n"
            "0 is the shape to explore itself, 1 is its direct sub-shapes, etc.")
        
        // Python iteration protocol
        .def("__iter__", [](TopExp_Explorer& self) -> TopExp_Explorer& {
            self.ReInit();
            return self;
        }, py::return_value_policy::reference_internal,
            "Returns the iterator itself (enables Python 'for' loops)")
        
        .def("__next__", [](TopExp_Explorer& self) -> TopoDS_Shape {
            if (!self.More()) {
                throw py::stop_iteration();
            }
            TopoDS_Shape current = self.Current();
            self.Next();
            return current;
        },
            "Returns the next shape in iteration (enables Python 'for' loops).\n\n"
            "Example:\n"
            "  for face in TopExp.Explorer(shape, TopAbs.ShapeEnum.FACE):\n"
            "      process_face(face)")
        .def("get_shapes", [](const TopExp_Explorer& self, bool unique=false) {
            py::list shapes;
            for(TopExp_Explorer exp = self; exp.More(); exp.Next()) {
                TopoDS_Shape current = exp.Current();
                
                if(unique) {
                    // Check if shape already exists using IsSame
                    bool found = false;
                    for(py::size_t i = 0; i < py::len(shapes); ++i) {
                        TopoDS_Shape existing = shapes[i].cast<TopoDS_Shape>();
                        if(existing.IsSame(current)) {
                            found = true;
                            break;
                        }
                    }
                    if(found) continue;
                }
                
                shapes.append(current);
            }
            return shapes;
        }, py::arg("unique") = false,
            "Returns a list of all shapes found by the explorer.\n\n"
            "Parameters:\n"
            "  unique: If True, only unique shapes are included (default: False)\n\n"
            "Returns:\n"
            "  A list of shapes found during exploration"
    )
    ;

    
    m.def("get_solids",
        [](const TopoDS_Shape& shape) {
            return get_shapes_of_type<TopAbs_SOLID>(shape);
        },
        py::arg("shape"),
        "Returns a list of all solids in the given shape.\n\n"
        "Parameters:\n"
        "  shape: The shape to explore\n\n"
        "Returns:\n"
        "  A list of TopoDS_Solid objects contained in the shape"
    );

    m.def("get_faces",
        [](const TopoDS_Shape& shape) {
            return get_shapes_of_type<TopAbs_FACE>(shape);
        },
        py::arg("shape"),
        "Returns a list of all faces in the given shape.\n\n"
        "Parameters:\n"
        "  shape: The shape to explore\n\n"
        "Returns:\n"
        "  A list of TopoDS_Face objects contained in the shape"
    );

    m.def("get_edges",
        [](const TopoDS_Shape& shape) {
            return get_shapes_of_type<TopAbs_EDGE>(shape);
        },
        py::arg("shape"),
        "Returns a list of all edges in the given shape.\n\n"
        "Parameters:\n"
        "  shape: The shape to explore\n\n"
        "Returns:\n"
        "  A list of TopoDS_Edge objects contained in the shape"
    );  

    m.def("get_vertices",
        [](const TopoDS_Shape& shape) {
            return get_shapes_of_type<TopAbs_VERTEX>(shape);
        },
        py::arg("shape"),
        "Returns a list of all vertices in the given shape.\n\n"
        "Parameters:\n"
        "  shape: The shape to explore\n\n"
        "Returns:\n"
        "  A list of TopoDS_Vertex objects contained in the shape"
    );
}