#include <Geom_Curve.hxx>
#include <Standard_Handle.hxx>
#include <gp_Pnt.hxx>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h> // for py::self * py::self etc.

#include <TopLoc_Location.hxx>

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

    
/*
Public Member Functions
 	TopLoc_Location ()
 	Constructs an empty local coordinate system object. Note: A Location constructed from a default datum is said to be "empty".
 
 	TopLoc_Location (const gp_Trsf &T)
 	Constructs the local coordinate system object defined by the transformation T. T invokes in turn, a TopLoc_Datum3D object.
 
 	TopLoc_Location (const Handle< TopLoc_Datum3D > &D)
 	Constructs the local coordinate system object defined by the 3D datum D. Exceptions Standard_ConstructionError if the transformation T does not represent a 3D coordinate system.
 
Standard_Boolean 	IsIdentity () const
 	Returns true if this location is equal to the Identity transformation.
 
void 	Identity ()
 	Resets this location to the Identity transformation.
 
const Handle< TopLoc_Datum3D > & 	FirstDatum () const
 	Returns the first elementary datum of the Location. Use the NextLocation function recursively to access the other data comprising this location. Exceptions Standard_NoSuchObject if this location is empty.
 
Standard_Integer 	FirstPower () const
 	Returns the power elevation of the first elementary datum. Exceptions Standard_NoSuchObject if this location is empty.
 
const TopLoc_Location & 	NextLocation () const
 	Returns a Location representing <me> without the first datum. We have the relation :
 
const gp_Trsf & 	Transformation () const
 	Returns the transformation associated to the coordinate system.
 
 	operator gp_Trsf () const
 
TopLoc_Location 	Inverted () const
 	Returns the inverse of <me>.
 
TopLoc_Location 	Multiplied (const TopLoc_Location &Other) const
 	Returns <me> * <Other>, the elementary datums are concatenated.
 
TopLoc_Location 	operator* (const TopLoc_Location &Other) const
 
TopLoc_Location 	Divided (const TopLoc_Location &Other) const
 	Returns <me> / <Other>.
 
TopLoc_Location 	operator/ (const TopLoc_Location &Other) const
 
TopLoc_Location 	Predivided (const TopLoc_Location &Other) const
 	Returns <Other>.Inverted() * <me>.
 
TopLoc_Location 	Powered (const Standard_Integer pwr) const
 	Returns me at the power <pwr>. If <pwr> is zero returns Identity. <pwr> can be lower than zero (usual meaning for powers).
 
size_t 	HashCode () const
 	Returns a hashed value for this local coordinate system. This value is used, with map tables, to store and retrieve the object easily.
 
Standard_Boolean 	IsEqual (const TopLoc_Location &Other) const
 	Returns true if this location and the location Other have the same elementary data, i.e. contain the same series of TopLoc_Datum3D and respective powers. This method is an alias for operator ==.
 
Standard_Boolean 	operator== (const TopLoc_Location &Other) const
 
Standard_Boolean 	IsDifferent (const TopLoc_Location &Other) const
 	Returns true if this location and the location Other do not have the same elementary data, i.e. do not contain the same series of TopLoc_Datum3D and respective powers. This method is an alias for operator !=.
 
Standard_Boolean 	operator!= (const TopLoc_Location &Other) const
 
void 	DumpJson (Standard_OStream &theOStream, Standard_Integer theDepth=-1) const
 	Dumps the content of me into the stream.
 
void 	ShallowDump (Standard_OStream &S) const
 	Prints the contents of <me> on the stream .
 
void 	Clear ()
 	Clear myItems.
    */
    py::class_<TopLoc_Location, std::shared_ptr<TopLoc_Location>>(m, "TopLoc_Location")
        .def(py::init<>())
        .def("is_identity", &TopLoc_Location::IsIdentity)
        .def("identity", &TopLoc_Location::Identity)
        .def("transformation", &TopLoc_Location::Transformation, py::return_value_policy::copy)
        .def("inverted", &TopLoc_Location::Inverted, py::return_value_policy::copy, "Returns the inverse of self.")
        .def("multiplied", &TopLoc_Location::Multiplied, py::arg("lhs"), "Returns self * lhs, the elementary datums are concatenated.")
        .def("divided", &TopLoc_Location::Divided, py::arg("lhs"), "Returns self / lhs.")
        .def("predivided", &TopLoc_Location::Predivided, py::arg("lhs"), "Returns lhs.Inverted() * self.")
        .def("powered", &TopLoc_Location::Powered, py::arg("lhs"), "Returns self at the power lhs. If lhs is zero returns Identity. <pwr> can be lower than zero (usual meaning for powers).")
        .def(py::self * py::self)
        .def(py::self / py::self)
        .def("__pow__",     &TopLoc_Location::Powered, py::arg("pwr"))

    ;

    py::class_<TopoDS_Shape, std::shared_ptr<TopoDS_Shape>>(m, "Shape")
        .def(py::init<>())
        .def("is_null", &TopoDS_Shape::IsNull)
        .def("nullify", &TopoDS_Shape::Nullify)
        .def("location", [](const TopoDS_Shape &self) -> TopLoc_Location {
            return self.Location();
        }, py::return_value_policy::copy)
        .def("set_location", py::overload_cast<const TopLoc_Location &, Standard_Boolean>(&TopoDS_Shape::Location), py::arg("theLoc"), py::arg("theRaiseExc") = false)
        .def("orientation", [](const TopoDS_Shape &self) -> TopAbs_Orientation {
            return self.Orientation();
        })
        .def("set_orientation", py::overload_cast<TopAbs_Orientation>(&TopoDS_Shape::Orientation), py::arg("theOrient"))
        .def("located", &TopoDS_Shape::Located, py::arg("theLoc"), py::arg("theRaiseExc") = false)
        .def("oriented", &TopoDS_Shape::Oriented, py::arg("theOrient"))
        .def("shape_type", &TopoDS_Shape::ShapeType) 
        .def("infinite", [](const TopoDS_Shape &self) -> Standard_Boolean {
            return self.Infinite();
        })
        .def("convex", [](const TopoDS_Shape &self) -> Standard_Boolean {
            return self.Convex();
        })
        .def("closed", [](const TopoDS_Shape &self) -> Standard_Boolean {
            return self.Closed();
        })
        .def("orientable", [](const TopoDS_Shape &self) -> Standard_Boolean {
            return self.Orientable();
        })
        .def("checked", [](const TopoDS_Shape &self) -> Standard_Boolean {
            return self.Checked();
        })
        .def("locked", [](const TopoDS_Shape &self) -> Standard_Boolean {
            return self.Locked();
        })
        .def("modified", [](const TopoDS_Shape &self) -> Standard_Boolean {
            return self.Modified();
        })
        .def("is_free", [](const TopoDS_Shape &self) -> Standard_Boolean {
            return self.Free();
        })
        .def("set_free", [](TopoDS_Shape &self, Standard_Boolean theIsFree) {
            self.Free(theIsFree);
        }, py::arg("theIsFree"))
        .def("move", &TopoDS_Shape::Move, py::arg("thePosition"), py::arg("theRaiseExc") = false)
        .def("moved", &TopoDS_Shape::Moved, py::arg("thePosition"), py::arg("theRaiseExc") = false)
        .def("reverse", &TopoDS_Shape::Reverse)
        .def("reversed", &TopoDS_Shape::Reversed)
        .def("complement", &TopoDS_Shape::Complement)
        .def("complemented", &TopoDS_Shape::Complemented)
        .def("compose", &TopoDS_Shape::Compose, py::arg("theOrient"))
        .def("composed", &TopoDS_Shape::Composed, py::arg("theOrient"))
        .def("nb_children", &TopoDS_Shape::NbChildren)
        .def("is_partner", &TopoDS_Shape::IsPartner, py::arg("theOther"))
        .def("is_same", &TopoDS_Shape::IsSame, py::arg("theOther"))
        .def("is_equal", &TopoDS_Shape::IsEqual, py::arg("theOther"))
        .def("is_not_equal", &TopoDS_Shape::IsNotEqual, py::arg("theOther"))
        .def("empty_copy", &TopoDS_Shape::EmptyCopy)
        .def("empty_copied", &TopoDS_Shape::EmptyCopied)
        .def("tshape", [](const TopoDS_Shape &self) -> Handle(TopoDS_TShape) {
            return self.TShape();
        }, py::return_value_policy::copy)
        .def("dump_json", &TopoDS_Shape::DumpJson, py::arg("theOStream"), py::arg("theDepth") = -1)
    ;

    py::class_<TopoDS_Compound, std::shared_ptr<TopoDS_Compound>, TopoDS_Shape>(m, "Compound")
        .def(py::init<>())
    ;

    py::class_<TopoDS_CompSolid, std::shared_ptr<TopoDS_CompSolid>, TopoDS_Shape>(m, "CompSolid")
        .def(py::init<>())
    ;
    
    py::class_<TopoDS_Solid, std::shared_ptr<TopoDS_Solid>, TopoDS_Shape>(m, "Solid")
    .def(py::init<>())
    ;
    
    py::class_<TopoDS_Shell, std::shared_ptr<TopoDS_Shell>, TopoDS_Shape>(m, "Shell")
    .def(py::init<>())
    ;

    py::class_<TopoDS_Face, std::shared_ptr<TopoDS_Face>, TopoDS_Shape>(m, "Face")
        .def(py::init<>())
    ;

    py::class_<TopoDS_Wire, std::shared_ptr<TopoDS_Wire>, TopoDS_Shape>(m, "Wire")
        .def(py::init<>())
    ;

    py::class_<TopoDS_Edge, std::shared_ptr<TopoDS_Edge>, TopoDS_Shape>(m, "Edge")
        .def(py::init<>())
        .def("curve", [](const TopoDS_Edge& self) -> py::object {
            Standard_Real first, last;
            opencascade::handle<Geom_Curve> curve = BRep_Tool::Curve(self, first, last);
            if (curve.IsNull()) {
                return py::none();
            }
            return py::make_tuple(curve, first, last);
        })
    ;

    py::class_<TopoDS_Vertex, std::shared_ptr<TopoDS_Vertex>, TopoDS_Shape>(m, "Vertex")
        .def(py::init<>())
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