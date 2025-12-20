#include <Geom_Curve.hxx>
#include <Standard_Handle.hxx>
#include <gp_Pnt.hxx>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h> // for py::self * py::self etc.

#include <TopLoc_Location.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRep_Tool.hxx>
#include <BRep_Builder.hxx>
#include <pybind11/pytypes.h>

namespace py = pybind11;

// Declare opencascade::handle as a holder type for pybind11
// Required to process opencascade::handle<T> as arguments/return types
PYBIND11_DECLARE_HOLDER_TYPE(T, opencascade::handle<T>);

void bind_shapes(py::module_ &m) {

    
    py::class_<TopLoc_Location, std::shared_ptr<TopLoc_Location>>(m, "TopLoc_Location",
        "A composite local coordinate system.\n\n"
        "Location represents a coordinate transformation comprised of a series\n"
        "of elementary reference coordinates (TopLoc_Datum3D objects) and the\n"
        "powers to which these objects are raised. It is used to define the\n"
        "placement of shapes in 3D space.\n\n"
        "Example:\n"
        "  loc = TopLoc.Location(gp.Trsf())  # Identity location\n"
        "  loc2 = loc * other_loc  # Compose transformations")
        
        .def(py::init<>(),
            "Constructs an empty local coordinate system (identity transformation).\n\n"
            "Note: A Location constructed from a default datum is said to be 'empty'")
        
        .def(py::init<const gp_Trsf&>(),
            py::arg("transformation"),
            "Constructs a location from a transformation.\n\n"
            "Parameters:\n"
            "  transformation: The gp_Trsf defining the coordinate system\n\n"
            "The transformation T invokes in turn a TopLoc_Datum3D object.\n\n"
            "Raises:\n"
            "  Standard_ConstructionError if T does not represent a 3D coordinate system")
        
        // Identity checking and reset
        .def("is_identity", &TopLoc_Location::IsIdentity,
            "Returns True if this location equals the identity transformation")
        
        .def("identity", &TopLoc_Location::Identity,
            "Resets this location to the identity transformation")
        
        // Transformation property (read-only)
        .def_property_readonly("transformation",
            [](const TopLoc_Location &self) -> gp_Trsf {
                return self.Transformation();
            },
            "The transformation associated with this coordinate system.\n\n"
            "Returns the gp_Trsf representation of this location")
        
        // Advanced datum access (for internal structure inspection)
        .def_property_readonly("first_datum",
            [](const TopLoc_Location &self) -> Handle(TopLoc_Datum3D) {
                return self.FirstDatum();
            },
            "Returns the first elementary datum.\n\n"
            "Use next_location recursively to access other data.\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if this location is empty")
        
        .def_property_readonly("first_power",
            &TopLoc_Location::FirstPower,
            "Returns the power elevation of the first elementary datum.\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if this location is empty")
        
        .def_property_readonly("next_location",
            [](const TopLoc_Location &self) -> TopLoc_Location {
                return self.NextLocation();
            },
            "Returns this location without the first datum.\n\n"
            "Relation: self = next_location * first_datum ^ first_power\n\n"
            "Raises:\n"
            "  Standard_NoSuchObject if this location is empty")
        
        // Operations
        .def("inverted", &TopLoc_Location::Inverted,
            "Returns the inverse of this location.\n\n"
            "The result satisfies: self * self.inverted() == Identity")
        
        .def("multiplied", &TopLoc_Location::Multiplied,
            py::arg("other"),
            "Returns self * other.\n\n"
            "The elementary datums are concatenated.\n\n"
            "Parameters:\n"
            "  other: The location to multiply with")
        
        .def("divided", &TopLoc_Location::Divided,
            py::arg("other"),
            "Returns self / other.\n\n"
            "Equivalent to self * other.inverted()")
        
        .def("predivided", &TopLoc_Location::Predivided,
            py::arg("other"),
            "Returns other.inverted() * self.\n\n"
            "Pre-division operation")
        
        .def("powered", &TopLoc_Location::Powered,
            py::arg("power"),
            "Returns this location raised to the given power.\n\n"
            "Parameters:\n"
            "  power: The exponent (can be negative or zero)\n\n"
            "If power is zero, returns Identity.\n"
            "Negative powers return the inverse raised to |power|")
        
        // Python operators
        .def(py::self * py::self,
            "Multiply two locations (composition of transformations)")
        
        .def(py::self / py::self,
            "Divide two locations (equivalent to self * other.inverted())")
        
        .def("__pow__", &TopLoc_Location::Powered,
            py::arg("power"),
            "Raise location to a power (supports negative exponents)")
        
        // Comparison operators
        .def("is_equal", &TopLoc_Location::IsEqual,
            py::arg("other"),
            "Returns True if locations have the same elementary data.\n\n"
            "Checks if both contain the same series of TopLoc_Datum3D\n"
            "and respective powers")
        
        .def("is_different", &TopLoc_Location::IsDifferent,
            py::arg("other"),
            "Returns True if locations have different elementary data")
        
        .def("__eq__", &TopLoc_Location::IsEqual,
            py::arg("other"),
            "Equality operator (same as is_equal)")
        
        .def("__ne__", &TopLoc_Location::IsDifferent,
            py::arg("other"),
            "Inequality operator (same as is_different)")
        
        .def("__hash__", [](const TopLoc_Location& self) {
            return self.HashCode();
        },
            "Returns a hash value for this location.\n\n"
            "Used with map tables to store and retrieve the object")
        
        // Utility methods
        .def("clear", &TopLoc_Location::Clear,
            "Clears the internal items, resetting to empty state")
        
        .def("dump_json", &TopLoc_Location::DumpJson,
            py::arg("stream"), py::arg("depth") = -1,
            "Dumps the content of this location to a stream in JSON format")
        
        .def("shallow_dump", &TopLoc_Location::ShallowDump,
            py::arg("stream"),
            "Prints the contents of this location on the stream")
    ;

    py::class_<TopoDS_Shape, std::shared_ptr<TopoDS_Shape>>(m, "Shape",
        "Base class for all topological shapes.\n\n"
        "A shape references an underlying shape with the potential to be given\n"
        "a location and an orientation. It consists of:\n"
        "- An underlying TShape with geometry\n"
        "- A Location for placement in local coordinate system\n"
        "- An Orientation in terms of geometry\n\n"
        "Note: A Shape is empty if it references an underlying shape which\n"
        "has an empty list of shapes")
        
        .def(py::init<>(),
            "Creates a null shape referring to nothing")
        
        .def(py::init([](const TopoDS_Shape& other) {
            return TopoDS_Shape(other);
        }),
            py::arg("other"),
            "Creates a copy of another shape.\n\n"
            "The copy shares the same TShape, location, and orientation")
        
        // Null checking
        .def("is_null", &TopoDS_Shape::IsNull,
            "Returns True if this shape is null.\n\n"
            "A null shape references no underlying shape")
        
        .def("nullify", &TopoDS_Shape::Nullify,
            "Destroys the reference to the underlying shape.\n\n"
            "As a result, this shape becomes null")
        
        // Location property
        .def_property("location",
            [](const TopoDS_Shape &self) -> TopLoc_Location {
                return self.Location();
            },
            [](TopoDS_Shape &self, const TopLoc_Location &theLoc) {
                self.Location(theLoc, false);
            },
            "The shape's local coordinate system.\n\n"
            "Get/set the transformation applied to this shape")
        
        .def("set_location", 
            py::overload_cast<const TopLoc_Location &, Standard_Boolean>(&TopoDS_Shape::Location),
            py::arg("location"), py::arg("raise_exc") = false,
            "Sets the shape local coordinate system.\n\n"
            "Parameters:\n"
            "  location: The new local coordinate system\n"
            "  raise_exc: If True, raises exception for transformations with scale\n\n"
            "Raises:\n"
            "  Standard_DomainError if transformation contains scaling and raise_exc=True")
        
        .def("located", &TopoDS_Shape::Located,
            py::arg("location"), py::arg("raise_exc") = false,
            "Returns a copy of this shape with a new location.\n\n"
            "Parameters:\n"
            "  location: The new local coordinate system\n"
            "  raise_exc: If True, raises exception for transformations with scale\n\n"
            "Returns:\n"
            "  New shape with the specified location")
        
        // Orientation property
        .def_property("orientation",
            [](const TopoDS_Shape &self) -> TopAbs_Orientation {
                return self.Orientation();
            },
            [](TopoDS_Shape &self, TopAbs_Orientation theOrient) {
                self.Orientation(theOrient);
            },
            "The shape's orientation.\n\n"
            "Get/set the orientation (FORWARD, REVERSED, INTERNAL, EXTERNAL)")
        
        .def("oriented", &TopoDS_Shape::Oriented,
            py::arg("orientation"),
            "Returns a copy of this shape with a new orientation.\n\n"
            "Returns:\n"
            "  New shape with the specified orientation")
        
        // Type
        .def_property_readonly("shape_type", &TopoDS_Shape::ShapeType,
            "Returns the type of this shape.\n\n"
            "Possible values: COMPOUND, COMPSOLID, SOLID, SHELL, FACE,\n"
            "WIRE, EDGE, VERTEX, SHAPE")
        
        // Boolean flags as properties
        .def_property("free",
            [](const TopoDS_Shape &self) { return self.Free(); },
            [](TopoDS_Shape &self, Standard_Boolean val) { self.Free(val); },
            "The free flag")
        
        .def_property("locked",
            [](const TopoDS_Shape &self) { return self.Locked(); },
            [](TopoDS_Shape &self, Standard_Boolean val) { self.Locked(val); },
            "The locked flag")
        
        .def_property("modified",
            [](const TopoDS_Shape &self) { return self.Modified(); },
            [](TopoDS_Shape &self, Standard_Boolean val) { self.Modified(val); },
            "The modification flag")
        
        .def_property("checked",
            [](const TopoDS_Shape &self) { return self.Checked(); },
            [](TopoDS_Shape &self, Standard_Boolean val) { self.Checked(val); },
            "The checked flag")
        
        .def_property("orientable",
            [](const TopoDS_Shape &self) { return self.Orientable(); },
            [](TopoDS_Shape &self, Standard_Boolean val) { self.Orientable(val); },
            "The orientability flag")
        
        .def_property("closed",
            [](const TopoDS_Shape &self) { return self.Closed(); },
            [](TopoDS_Shape &self, Standard_Boolean val) { self.Closed(val); },
            "The closedness flag")
        
        .def_property("infinite",
            [](const TopoDS_Shape &self) { return self.Infinite(); },
            [](TopoDS_Shape &self, Standard_Boolean val) { self.Infinite(val); },
            "The infinity flag")
        
        .def_property("convex",
            [](const TopoDS_Shape &self) { return self.Convex(); },
            [](TopoDS_Shape &self, Standard_Boolean val) { self.Convex(val); },
            "The convexness flag")
        
        // Transformation methods
        .def("move",
            py::overload_cast<const TopLoc_Location&, const Standard_Boolean>(&TopoDS_Shape::Move),
            py::arg("position"), py::arg("raise_exc") = false,
            "Multiplies the shape location by position.\n\n"
            "Modifies this shape in place")
        
        .def("move",
            [](TopoDS_Shape &self, const gp_Trsf &theTrsf, Standard_Boolean theRaiseExc) {
                self.Move(TopLoc_Location(theTrsf), theRaiseExc);
            },
            py::arg("transformation"), py::arg("raise_exc") = false,
            "Multiplies the shape location by transformation.\n\n"
            "Modifies this shape in place")
        
        .def("moved",
            py::overload_cast<const TopLoc_Location&, const Standard_Boolean>(&TopoDS_Shape::Moved, py::const_),
            py::arg("position"), py::arg("raise_exc") = false,
            "Returns a copy with location multiplied by position")
        
        .def("moved",
            [](const TopoDS_Shape &self, const gp_Trsf &theTrsf, Standard_Boolean theRaiseExc) {
                return self.Moved(TopLoc_Location(theTrsf), theRaiseExc);
            },
            py::arg("transformation"), py::arg("raise_exc") = false,
            "Returns a copy with location multiplied by transformation")
        
        // Orientation manipulation
        .def("reverse", &TopoDS_Shape::Reverse,
            "Reverses the orientation.\n\n"
            "Uses TopAbs::Reverse. Modifies this shape in place")
        
        .def("reversed", &TopoDS_Shape::Reversed,
            "Returns a copy with reversed orientation")
        
        .def("complement", &TopoDS_Shape::Complement,
            "Complements the orientation.\n\n"
            "Uses TopAbs::Complement. Modifies this shape in place")
        
        .def("complemented", &TopoDS_Shape::Complemented,
            "Returns a copy with complemented orientation")
        
        .def("compose", &TopoDS_Shape::Compose,
            py::arg("orientation"),
            "Updates orientation by composition.\n\n"
            "Uses TopAbs::Compose. Modifies this shape in place")
        
        .def("composed", &TopoDS_Shape::Composed,
            py::arg("orientation"),
            "Returns a copy with orientation composed with the given orientation")
        
        // Sub-shapes
        .def_property_readonly("nb_children", &TopoDS_Shape::NbChildren,
            "Returns the number of direct sub-shapes (children).\n\n"
            "Use TopoDS.Iterator to access the sub-shapes")
        
        // Comparison methods
        .def("is_partner", &TopoDS_Shape::IsPartner,
            py::arg("other"),
            "Returns True if two shapes are partners.\n\n"
            "Partners share the same TShape but may have different\n"
            "locations and orientations")
        
        .def("is_same", &TopoDS_Shape::IsSame,
            py::arg("other"),
            "Returns True if two shapes are the same.\n\n"
            "Same shapes share the same TShape and location but\n"
            "may have different orientations")
        
        .def("is_equal", &TopoDS_Shape::IsEqual,
            py::arg("other"),
            "Returns True if two shapes are equal.\n\n"
            "Equal shapes share the same TShape, location, and orientation")
        
        .def("is_not_equal", &TopoDS_Shape::IsNotEqual,
            py::arg("other"),
            "Returns True if two shapes are not equal")
        
        // Python operators
        .def("__eq__", &TopoDS_Shape::IsEqual,
            py::arg("other"),
            "Equality operator (same as is_equal)")
        
        .def("__ne__", &TopoDS_Shape::IsNotEqual,
            py::arg("other"),
            "Inequality operator (same as is_not_equal)")
        
        .def("__hash__", [](const TopoDS_Shape& self) {
            return std::hash<TopoDS_Shape>{}(self);
        },
            "Returns hash value for this shape")
        
        // Empty copy
        .def("empty_copy", &TopoDS_Shape::EmptyCopy,
            "Replaces this shape with a new shape.\n\n"
            "The new shape has the same orientation and location,\n"
            "and a new TShape with the same geometry but no sub-shapes")
        
        .def("empty_copied", &TopoDS_Shape::EmptyCopied,
            "Returns a new shape with no sub-shapes.\n\n"
            "The new shape has the same orientation and location,\n"
            "and a new TShape with the same geometry but no sub-shapes")
        
        // TShape access
        .def_property_readonly("tshape",
            [](const TopoDS_Shape &self) -> Handle(TopoDS_TShape) {
                return self.TShape();
            },
            "Returns a handle to the actual shape implementation")
        
        // Debug
        .def("dump_json", &TopoDS_Shape::DumpJson,
            py::arg("stream"), py::arg("depth") = -1,
            "Dumps the content of this shape to a stream in JSON format")
    ;

    py::class_<TopoDS_Compound, std::shared_ptr<TopoDS_Compound>, TopoDS_Shape>(m, "Compound",
        "A compound is a collection of shapes")
        .def(py::init<>(),
            "Creates an empty compound")
        .def(py::init([](const TopoDS_Shape& shape) {
            return TopoDS::Compound(shape);
        }),
            py::arg("shape"),
            "Creates a compound from a shape (casts to compound type)")
    ;

    py::class_<TopoDS_CompSolid, std::shared_ptr<TopoDS_CompSolid>, TopoDS_Shape>(m, "CompSolid",
        "A composite solid")
        .def(py::init<>(),
            "Creates an empty composite solid")
        .def(py::init([](const TopoDS_Shape& shape) {
            return TopoDS::CompSolid(shape);
        }),
            py::arg("shape"),
            "Creates a composite solid from a shape (casts to compsolid type)")
    ;
    
    py::class_<TopoDS_Solid, std::shared_ptr<TopoDS_Solid>, TopoDS_Shape>(m, "Solid",
        "A solid shape")
        .def(py::init<>(),
            "Creates an empty solid")
        .def(py::init([](const TopoDS_Shape& shape) {
            return TopoDS::Solid(shape);
        }),
            py::arg("shape"),
            "Creates a solid from a shape (casts to solid type)")
    ;
    
    py::class_<TopoDS_Shell, std::shared_ptr<TopoDS_Shell>, TopoDS_Shape>(m, "Shell",
        "A shell (collection of connected faces)")
        .def(py::init<>(),
            "Creates an empty shell")
        .def(py::init([](const TopoDS_Shape& shape) {
            return TopoDS::Shell(shape);
        }),
            py::arg("shape"),
            "Creates a shell from a shape (casts to shell type)")
    ;

    py::class_<TopoDS_Face, std::shared_ptr<TopoDS_Face>, TopoDS_Shape>(m, "Face",
        "A face (surface bounded by wires)")
        .def(py::init<>(),
            "Creates an empty face")
        .def(py::init([](const TopoDS_Shape& shape) {
            return TopoDS::Face(shape);
        }),
            py::arg("shape"),
            "Creates a face from a shape (casts to face type)")
    ;

    py::class_<TopoDS_Wire, std::shared_ptr<TopoDS_Wire>, TopoDS_Shape>(m, "Wire",
        "A wire (sequence of connected edges)")
        .def(py::init<>(),
            "Creates an empty wire")
        .def(py::init([](const TopoDS_Shape& shape) {
            return TopoDS::Wire(shape);
        }),
            py::arg("shape"),
            "Creates a wire from a shape (casts to wire type)")
    ;

    py::class_<TopoDS_Edge, std::shared_ptr<TopoDS_Edge>, TopoDS_Shape>(m, "Edge",
        "An edge (curve bounded by vertices)")
        .def(py::init<>(),
            "Creates an empty edge")
        .def(py::init([](const TopoDS_Shape& shape) {
            return TopoDS::Edge(shape);
        }),
            py::arg("shape"),
            "Creates an edge from a shape (casts to edge type)")
        .def("curve", [](const TopoDS_Edge& self) -> py::object {
            Standard_Real first, last;
            opencascade::handle<Geom_Curve> curve = BRep_Tool::Curve(self, first, last);
            if (curve.IsNull()) {
                return py::none();
            }
            return py::make_tuple(curve, first, last);
        })
    ;

    py::class_<TopoDS_Vertex, std::shared_ptr<TopoDS_Vertex>, TopoDS_Shape>(m, "Vertex",
        "A vertex (topological point)")
        .def(py::init<>(),
            "Creates an empty vertex")
        .def(py::init([](const TopoDS_Shape& shape) {
            return TopoDS::Vertex(shape);
        }),
            py::arg("shape"),
            "Creates a vertex from a shape (casts to vertex type)")
        .def(py::init([](const gp_Pnt& thePnt, Standard_Real tolerance) {
            TopoDS_Vertex vertex;
            BRep_Builder builder;
            builder.MakeVertex(vertex, thePnt, tolerance);
            return vertex;
        }), py::arg("point"), py::arg("tolerance") = 1e-7)
        .def(py::init([](double x, double y, double z, Standard_Real tolerance) {
            gp_Pnt pnt(x, y, z);
            TopoDS_Vertex vertex;
            BRep_Builder builder;
            builder.MakeVertex(vertex, pnt, tolerance);
            return vertex;
        }), py::arg("x"), py::arg("y"), py::arg("z"), py::arg("tolerance") = 1e-7)
        .def_property("point", 
            [](const TopoDS_Vertex &self) -> gp_Pnt {
                return BRep_Tool::Pnt(self);
            }, 
            [](TopoDS_Vertex &self, const gp_Pnt &thePnt) {
                BRep_Builder builder;
                Standard_Real tolerance = BRep_Tool::Tolerance(self);
                builder.UpdateVertex(self, thePnt, tolerance);
        })
        .def("x", [](const TopoDS_Vertex &self) -> Standard_Real {
            gp_Pnt pnt = BRep_Tool::Pnt(self);
            return pnt.X();
        })
        .def("y", [](const TopoDS_Vertex &self) -> Standard_Real {
            gp_Pnt pnt = BRep_Tool::Pnt(self);
            return pnt.Y();
        })
        .def("z", [](const TopoDS_Vertex &self) -> Standard_Real {
            gp_Pnt pnt = BRep_Tool::Pnt(self);
            return pnt.Z();
        })
    ;
}