#include <pybind11/detail/common.h>
#include <pybind11/pybind11.h>
#include <pybind11/operators.h> // for py::self * py::self etc.

#include <gp.hxx>
#include <gp_XYZ.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_Dir.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax3.hxx>
#include <gp_Lin.hxx>
#include <gp_Pln.hxx>
#include <gp_Circ.hxx>
#include <gp_Elips.hxx>
#include <gp_Hypr.hxx>
#include <gp_Parab.hxx>
// #include <gp_Ax3.hxx>
#include <gp_Quaternion.hxx>

#include <gp_Trsf.hxx>

namespace py = pybind11;

void bind_gp(py::module_ &m)
{
    py::class_<gp_XYZ>(m, "XYZ")
        .def(py::init<>())
        .def(py::init<Standard_Real, Standard_Real, Standard_Real>(), py::arg("x"), py::arg("y"), py::arg("z"))
        
        // Coordinate accessors
        .def_property("x", &gp_XYZ::X, &gp_XYZ::SetX)
        .def_property("y", &gp_XYZ::Y, &gp_XYZ::SetY)
        .def_property("z", &gp_XYZ::Z, &gp_XYZ::SetZ)
        .def("coord", py::overload_cast<const Standard_Integer>(&gp_XYZ::Coord, py::const_), py::arg("index"))
        .def("coord", [](const gp_XYZ& self) {
            Standard_Real x, y, z;
            self.Coord(x, y, z);
            return py::make_tuple(x, y, z);
        }, "Returns tuple of (x, y, z)")
        
        // Coordinate setters
        .def("set_coord", py::overload_cast<const Standard_Real, const Standard_Real, const Standard_Real>(&gp_XYZ::SetCoord), 
             py::arg("x"), py::arg("y"), py::arg("z"))
        .def("set_coord", py::overload_cast<const Standard_Integer, const Standard_Real>(&gp_XYZ::SetCoord), 
             py::arg("index"), py::arg("value"))
        
        // Magnitude
        .def("modulus", &gp_XYZ::Modulus)
        .def("square_modulus", &gp_XYZ::SquareModulus)
        .def("distance", [](const gp_XYZ& self, const gp_XYZ& other) {
            return gp_Pnt(self).Distance(gp_Pnt(other));
        }, py::arg("other"))
        
        // Comparison
        .def("is_equal", &gp_XYZ::IsEqual, py::arg("other"), py::arg("tolerance"))
        
        // Vector operations
        .def("add", &gp_XYZ::Add, py::arg("other"))
        .def("added", &gp_XYZ::Added, py::arg("other"))
        .def("subtract", &gp_XYZ::Subtract, py::arg("other"))
        .def("subtracted", &gp_XYZ::Subtracted, py::arg("other"))
        .def("multiply", py::overload_cast<const Standard_Real>(&gp_XYZ::Multiply), py::arg("scalar"))
        .def("multiply", py::overload_cast<const gp_XYZ&>(&gp_XYZ::Multiply), py::arg("other"))
        .def("multiplied", py::overload_cast<const Standard_Real>(&gp_XYZ::Multiplied, py::const_), py::arg("scalar"))
        .def("multiplied", py::overload_cast<const gp_XYZ&>(&gp_XYZ::Multiplied, py::const_), py::arg("other"))
        .def("divide", &gp_XYZ::Divide, py::arg("scalar"))
        .def("divided", &gp_XYZ::Divided, py::arg("scalar"))
        
        // Cross and dot products
        .def("cross", &gp_XYZ::Cross, py::arg("other"))
        .def("crossed", &gp_XYZ::Crossed, py::arg("other"))
        .def("dot", &gp_XYZ::Dot, py::arg("other"))
        .def("cross_magnitude", &gp_XYZ::CrossMagnitude, py::arg("right"))
        .def("cross_square_magnitude", &gp_XYZ::CrossSquareMagnitude, py::arg("right"))
        .def("cross_cross", &gp_XYZ::CrossCross, py::arg("coord1"), py::arg("coord2"))
        .def("cross_crossed", &gp_XYZ::CrossCrossed, py::arg("coord1"), py::arg("coord2"))
        .def("dot_cross", &gp_XYZ::DotCross, py::arg("coord1"), py::arg("coord2"))
        
        // Normalization
        .def("normalize", &gp_XYZ::Normalize)
        .def("normalized", &gp_XYZ::Normalized)
        
        // Reverse
        .def("reverse", &gp_XYZ::Reverse)
        .def("reversed", &gp_XYZ::Reversed)
        
        // Operators
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self * Standard_Real())
        .def(Standard_Real() * py::self)
        .def(py::self * py::self)  // dot product
        .def(py::self / Standard_Real())
        .def(py::self *= Standard_Real())
        .def(py::self /= Standard_Real())
        .def(py::self ^ py::self)  // cross product
        .def(py::self ^= py::self)
        // .def(py::self == py::self)
        // Custom comparator with lambda
        .def("__eq__", [](const gp_XYZ& self, const gp_XYZ& other) {
            // Custom logic - e.g., with tolerance
            return self.IsEqual(other, gp::Resolution());
        })
        // .def(-py::self)
        
        // String representation
        .def("__repr__", [](const gp_XYZ& self) {
            return "xyz(" + std::to_string(self.X()) + ", " + 
                   std::to_string(self.Y()) + ", " + std::to_string(self.Z()) + ")";
        })
    ;

    py::class_<gp_Pnt>(m, "Pnt")
        .def(py::init<>())
        .def(py::init<const gp_XYZ&>(), py::arg("coord"))
        .def(py::init<Standard_Real, Standard_Real, Standard_Real>(), 
             py::arg("x"), py::arg("y"), py::arg("z"))
        
        // Coordinate accessors
        .def_property("x", &gp_Pnt::X, &gp_Pnt::SetX)
        .def_property("y", &gp_Pnt::Y, &gp_Pnt::SetY)
        .def_property("z", &gp_Pnt::Z, &gp_Pnt::SetZ)
        .def_property("xyz", &gp_Pnt::XYZ, &gp_Pnt::SetXYZ)
        .def("coord", py::overload_cast<const Standard_Integer>(&gp_Pnt::Coord, py::const_), py::arg("index"))
        .def("coord", [](const gp_Pnt& self) {
            Standard_Real x, y, z;
            self.Coord(x, y, z);
            return py::make_tuple(x, y, z);
        }, "Returns tuple of (x, y, z)")
        
        // Coordinate setters
        .def("set_coord", py::overload_cast<const Standard_Real, const Standard_Real, const Standard_Real>(&gp_Pnt::SetCoord), 
             py::arg("x"), py::arg("y"), py::arg("z"))
        .def("set_coord", py::overload_cast<const Standard_Integer, const Standard_Real>(&gp_Pnt::SetCoord), 
             py::arg("index"), py::arg("value"))
        
        // Distance operations
        .def("distance", &gp_Pnt::Distance, py::arg("other"))
        .def("square_distance", &gp_Pnt::SquareDistance, py::arg("other"))
        .def("is_equal", &gp_Pnt::IsEqual, py::arg("other"), py::arg("linear_tolerance"))
        
        // Barycenter
        .def("bary_center", &gp_Pnt::BaryCenter, py::arg("alpha"), py::arg("p"), py::arg("beta"))
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Pnt::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Pnt::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Pnt::Mirror), py::arg("a2"))
        .def("rotate", &gp_Pnt::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Pnt::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Pnt::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Pnt::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Pnt::Translate), 
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Pnt::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Pnt::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Pnt::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Pnt::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Pnt::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Pnt::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Pnt::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Pnt::Translated, py::const_), 
             py::arg("p1"), py::arg("p2"))
        
        // String representation
        .def("__repr__", [](const gp_Pnt& self) {
            return "gp_Pnt(" + std::to_string(self.X()) + ", " + 
                   std::to_string(self.Y()) + ", " + std::to_string(self.Z()) + ")";
        })

    ;

    py::class_<gp_Vec>(m, "Vec")
        .def(py::init<>())
        .def(py::init<const gp_Dir&>(), py::arg("dir"))
        .def(py::init<const gp_XYZ&>(), py::arg("coord"))
        .def(py::init<Standard_Real, Standard_Real, Standard_Real>(), 
             py::arg("x"), py::arg("y"), py::arg("z"))
        .def(py::init<const gp_Pnt&, const gp_Pnt&>(), py::arg("p1"), py::arg("p2"))
        
        // Coordinate accessors
        .def_property("x", &gp_Vec::X, &gp_Vec::SetX)
        .def_property("y", &gp_Vec::Y, &gp_Vec::SetY)
        .def_property("z", &gp_Vec::Z, &gp_Vec::SetZ)
        .def_property("xyz", &gp_Vec::XYZ, &gp_Vec::SetXYZ)
        
        // Magnitude
        .def("magnitude", &gp_Vec::Magnitude)
        .def("square_magnitude", &gp_Vec::SquareMagnitude)
        
        // Comparison
        .def("is_equal", &gp_Vec::IsEqual, py::arg("other"), py::arg("linear_tolerance"), py::arg("angular_tolerance"))
        .def("is_normal", &gp_Vec::IsNormal, py::arg("other"), py::arg("angular_tolerance"))
        .def("is_opposite", &gp_Vec::IsOpposite, py::arg("other"), py::arg("angular_tolerance"))
        .def("is_parallel", &gp_Vec::IsParallel, py::arg("other"), py::arg("angular_tolerance"))
        
        // Angles
        .def("angle", &gp_Vec::Angle, py::arg("other"))
        .def("angle_with_ref", &gp_Vec::AngleWithRef, py::arg("other"), py::arg("vref"))
        
        // Operations
        .def("add", &gp_Vec::Add, py::arg("other"))
        .def("added", &gp_Vec::Added, py::arg("other"))
        .def("subtract", &gp_Vec::Subtract, py::arg("other"))
        .def("subtracted", &gp_Vec::Subtracted, py::arg("other"))
        .def("multiply", &gp_Vec::Multiply, py::arg("scalar"))
        .def("multiplied", &gp_Vec::Multiplied, py::arg("scalar"))
        .def("divide", &gp_Vec::Divide, py::arg("scalar"))
        .def("divided", &gp_Vec::Divided, py::arg("scalar"))
        
        // Cross and dot products
        .def("cross", &gp_Vec::Cross, py::arg("right"))
        .def("crossed", &gp_Vec::Crossed, py::arg("right"))
        .def("cross_magnitude", &gp_Vec::CrossMagnitude, py::arg("right"))
        .def("cross_square_magnitude", &gp_Vec::CrossSquareMagnitude, py::arg("right"))
        .def("cross_cross", &gp_Vec::CrossCross, py::arg("v1"), py::arg("v2"))
        .def("cross_crossed", &gp_Vec::CrossCrossed, py::arg("v1"), py::arg("v2"))
        .def("dot", &gp_Vec::Dot, py::arg("other"))
        .def("dot_cross", &gp_Vec::DotCross, py::arg("v1"), py::arg("v2"))
        
        // Normalization
        .def("normalize", &gp_Vec::Normalize)
        .def("normalized", &gp_Vec::Normalized)
        
        // Reverse
        .def("reverse", &gp_Vec::Reverse)
        .def("reversed", &gp_Vec::Reversed)
        
        // Transformations
        .def("mirror", py::overload_cast<const gp_Vec&>(&gp_Vec::Mirror), py::arg("v"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Vec::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Vec::Mirror), py::arg("a2"))
        .def("mirrored", py::overload_cast<const gp_Vec&>(&gp_Vec::Mirrored, py::const_), py::arg("v"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Vec::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Vec::Mirrored, py::const_), py::arg("a2"))
        .def("rotate", &gp_Vec::Rotate, py::arg("a1"), py::arg("ang"))
        .def("rotated", &gp_Vec::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Vec::Scale, py::arg("s"))
        .def("scaled", &gp_Vec::Scaled, py::arg("s"))
        .def("transform", &gp_Vec::Transform, py::arg("t"))
        .def("transformed", &gp_Vec::Transformed, py::arg("t"))
        
        // Operators
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self += py::self)
        .def(py::self -= py::self)
        .def(py::self * Standard_Real())
        .def(Standard_Real() * py::self)
        .def(py::self * py::self)  // dot product
        .def(py::self / Standard_Real())
        .def(py::self *= Standard_Real())
        .def(py::self /= Standard_Real())
        .def(py::self ^ py::self)  // cross product
        .def(py::self ^= py::self)
        .def(-py::self)
        
        .def("__repr__", [](const gp_Vec& self) {
            return "gp_Vec(" + std::to_string(self.X()) + ", " + 
                   std::to_string(self.Y()) + ", " + std::to_string(self.Z()) + ")";
        })
    ;

    py::class_<gp_Dir>(m, "Dir")
        .def(py::init<>())
        .def(py::init<const gp_Vec&>(), py::arg("v"))
        .def(py::init<const gp_XYZ&>(), py::arg("coord"))
        .def(py::init<Standard_Real, Standard_Real, Standard_Real>(), 
             py::arg("xv"), py::arg("yv"), py::arg("zv"))
        
        // Coordinate 
        .def_property("x", &gp_Dir::X, &gp_Dir::SetX)
        .def_property("y", &gp_Dir::Y, &gp_Dir::SetY)
        .def_property("z", &gp_Dir::Z, &gp_Dir::SetZ)
        .def_property("xyz", &gp_Dir::XYZ, &gp_Dir::SetXYZ)
        .def("coord", py::overload_cast<const Standard_Integer>(&gp_Dir::Coord, py::const_), py::arg("index"))
        .def("coord", [](const gp_Dir& self) {
            Standard_Real x, y, z;
            self.Coord(x, y, z);
            return py::make_tuple(x, y, z);
        })

        
        // Comparison
        .def("is_equal", &gp_Dir::IsEqual, py::arg("other"), py::arg("angular_tolerance"))
        .def("is_normal", &gp_Dir::IsNormal, py::arg("other"), py::arg("angular_tolerance"))
        .def("is_opposite", &gp_Dir::IsOpposite, py::arg("other"), py::arg("angular_tolerance"))
        .def("is_parallel", &gp_Dir::IsParallel, py::arg("other"), py::arg("angular_tolerance"))
        
        // Angles
        .def("angle", &gp_Dir::Angle, py::arg("other"))
        .def("angle_with_ref", &gp_Dir::AngleWithRef, py::arg("other"), py::arg("vref"))
        
        // Cross products
        .def("cross", &gp_Dir::Cross, py::arg("right"))
        .def("crossed", &gp_Dir::Crossed, py::arg("right"))
        .def("cross_cross", &gp_Dir::CrossCross, py::arg("v1"), py::arg("v2"))
        .def("cross_crossed", &gp_Dir::CrossCrossed, py::arg("v1"), py::arg("v2"))
        
        // Dot products
        .def("dot", &gp_Dir::Dot, py::arg("other"))
        .def("dot_cross", &gp_Dir::DotCross, py::arg("v1"), py::arg("v2"))
        
        // Reverse
        .def("reverse", &gp_Dir::Reverse)
        .def("reversed", &gp_Dir::Reversed)
        
        // Transformations
        .def("mirror", py::overload_cast<const gp_Dir&>(&gp_Dir::Mirror), py::arg("v"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Dir::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Dir::Mirror), py::arg("a2"))
        .def("mirrored", py::overload_cast<const gp_Dir&>(&gp_Dir::Mirrored, py::const_), py::arg("v"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Dir::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Dir::Mirrored, py::const_), py::arg("a2"))
        .def("rotate", &gp_Dir::Rotate, py::arg("a1"), py::arg("ang"))
        .def("rotated", &gp_Dir::Rotated, py::arg("a1"), py::arg("ang"))
        .def("transform", &gp_Dir::Transform, py::arg("t"))
        .def("transformed", &gp_Dir::Transformed, py::arg("t"))
        
        // Operators
        .def(py::self * py::self)  // dot product
        .def(py::self ^ py::self)  // cross product
        .def(-py::self)
        
        .def("__repr__", [](const gp_Dir& self) {
            return "gp_Dir(" + std::to_string(self.X()) + ", " + 
                   std::to_string(self.Y()) + ", " + std::to_string(self.Z()) + ")";
        })
    ;

    py::class_<gp_Ax1>(m, "Ax1")
        .def(py::init<>())
        .def(py::init<const gp_Pnt&, const gp_Dir&>(), py::arg("p"), py::arg("v"))
        
        // // Setters
        // .def("set_direction", &gp_Ax1::SetDirection, py::arg("v"))
        // .def("set_location", &gp_Ax1::SetLocation, py::arg("p"))
        
        // Getters
        .def_property("direction", &gp_Ax1::Direction, &gp_Ax1::SetDirection)
        .def_property("location", &gp_Ax1::Location, &gp_Ax1::SetLocation)
        
        // Comparison
        .def("is_coaxial", &gp_Ax1::IsCoaxial, py::arg("other"), py::arg("angular_tolerance"), py::arg("linear_tolerance"))
        .def("is_normal", &gp_Ax1::IsNormal, py::arg("other"), py::arg("angular_tolerance"))
        .def("is_opposite", &gp_Ax1::IsOpposite, py::arg("other"), py::arg("angular_tolerance"))
        .def("is_parallel", &gp_Ax1::IsParallel, py::arg("other"), py::arg("angular_tolerance"))
        .def("angle", &gp_Ax1::Angle, py::arg("other"))
        
        // Reverse
        .def("reverse", &gp_Ax1::Reverse)
        .def("reversed", &gp_Ax1::Reversed)
        
        // Transformations
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Ax1::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Ax1::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Ax1::Mirror), py::arg("a2"))
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Ax1::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Ax1::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Ax1::Mirrored, py::const_), py::arg("a2"))
        .def("rotate", &gp_Ax1::Rotate, py::arg("a1"), py::arg("ang"))
        .def("rotated", &gp_Ax1::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Ax1::Scale, py::arg("p"), py::arg("s"))
        .def("scaled", &gp_Ax1::Scaled, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Ax1::Transform, py::arg("t"))
        .def("transformed", &gp_Ax1::Transformed, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Ax1::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Ax1::Translate), py::arg("p1"), py::arg("p2"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Ax1::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Ax1::Translated, py::const_), py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Ax1& self) {
            const gp_Pnt& loc = self.Location();
            const gp_Dir& dir = self.Direction();
            return "gp_Ax1(loc=(" + std::to_string(loc.X()) + ", " + 
                   std::to_string(loc.Y()) + ", " + std::to_string(loc.Z()) + "), dir=(" +
                   std::to_string(dir.X()) + ", " + std::to_string(dir.Y()) + ", " + 
                   std::to_string(dir.Z()) + "))";
        })
    ;

    py::class_<gp_Ax2>(m, "Ax2")
        .def(py::init<>())
        .def(py::init<const gp_Pnt&, const gp_Dir&>(), py::arg("p"), py::arg("v"))
        .def(py::init<const gp_Pnt&, const gp_Dir&, const gp_Dir&>(), 
             py::arg("p"), py::arg("n"), py::arg("vx"))
        
        // Getters
        .def("angle", &gp_Ax2::Angle, py::arg("other"))
        .def_property("axis", &gp_Ax2::Axis, &gp_Ax2::SetAxis)
        .def_property("direction", &gp_Ax2::Direction, &gp_Ax2::SetDirection)
        .def_property("location", &gp_Ax2::Location, &gp_Ax2::SetLocation)
        .def_property("x_direction", &gp_Ax2::XDirection, &gp_Ax2::SetXDirection)
        .def_property("x_direction", &gp_Ax2::XDirection, &gp_Ax2::SetXDirection)
        .def_property("y_direction", &gp_Ax2::YDirection, &gp_Ax2::SetYDirection)
        
        // Comparison
        .def("is_coplanar", py::overload_cast<const gp_Ax2&, const Standard_Real, const Standard_Real>(&gp_Ax2::IsCoplanar, py::const_),
             py::arg("other"), py::arg("linear_tolerance"), py::arg("angular_tolerance"))
        .def("is_coplanar", py::overload_cast<const gp_Ax1&, const Standard_Real, const Standard_Real>(&gp_Ax2::IsCoplanar, py::const_),
             py::arg("a1"), py::arg("linear_tolerance"), py::arg("angular_tolerance"))
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Ax2::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Ax2::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Ax2::Mirror), py::arg("a2"))
        .def("rotate", &gp_Ax2::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Ax2::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Ax2::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Ax2::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Ax2::Translate), 
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Ax2::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Ax2::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Ax2::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Ax2::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Ax2::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Ax2::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Ax2::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Ax2::Translated, py::const_), 
             py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Ax2& self) {
            const gp_Pnt& loc = self.Location();
            const gp_Dir& dir = self.Direction();
            const gp_Dir& xdir = self.XDirection();
            return "gp_Ax2(loc=(" + std::to_string(loc.X()) + ", " + 
                   std::to_string(loc.Y()) + ", " + std::to_string(loc.Z()) + "), dir=(" +
                   std::to_string(dir.X()) + ", " + std::to_string(dir.Y()) + ", " + 
                   std::to_string(dir.Z()) + "), xdir=(" +
                   std::to_string(xdir.X()) + ", " + std::to_string(xdir.Y()) + ", " + 
                   std::to_string(xdir.Z()) + "))";
        })
    ;

    py::class_<gp_Pln>(m, "Pln")
        .def(py::init<>())
        .def(py::init<const gp_Ax3&>(), py::arg("a3"))
        .def(py::init<const gp_Pnt&, const gp_Dir&>(), py::arg("p"), py::arg("v"))
        .def(py::init<const Standard_Real, const Standard_Real, const Standard_Real, const Standard_Real>(),
             py::arg("a"), py::arg("b"), py::arg("c"), py::arg("d"))
        
        // Properties
        .def_property("axis", &gp_Pln::Axis, &gp_Pln::SetAxis)
        .def_property("location", &gp_Pln::Location, &gp_Pln::SetLocation)
        .def_property("position", &gp_Pln::Position, &gp_Pln::SetPosition)
        
        // Getters
        .def("x_axis", &gp_Pln::XAxis)
        .def("y_axis", &gp_Pln::YAxis)
        .def("direct", &gp_Pln::Direct)
        
        // Coefficients
        .def("coefficients", [](const gp_Pln& self) {
            double a, b, c, d;
            self.Coefficients(a, b, c, d);
            return py::make_tuple(a, b, c, d);
        })
        
        // Distance
        .def("distance", py::overload_cast<const gp_Pnt&>(&gp_Pln::Distance, py::const_), py::arg("p"))
        .def("distance", py::overload_cast<const gp_Lin&>(&gp_Pln::Distance, py::const_), py::arg("l"))
        .def("distance", py::overload_cast<const gp_Pln&>(&gp_Pln::Distance, py::const_), py::arg("other"))
        .def("square_distance", py::overload_cast<const gp_Pnt&>(&gp_Pln::SquareDistance, py::const_), py::arg("p"))
        .def("square_distance", py::overload_cast<const gp_Lin&>(&gp_Pln::SquareDistance, py::const_), py::arg("l"))
        .def("square_distance", py::overload_cast<const gp_Pln&>(&gp_Pln::SquareDistance, py::const_), py::arg("other"))
        
        // Tests
        .def("contains", py::overload_cast<const gp_Pnt&, double>(&gp_Pln::Contains, py::const_), py::arg("p"), py::arg("linear_tolerance"))
        .def("contains", py::overload_cast<const gp_Lin&, double, double>(&gp_Pln::Contains, py::const_), py::arg("p"), py::arg("linear_tolerance"), py::arg("angular_tolerance"))
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Pln::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Pln::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Pln::Mirror), py::arg("a2"))
        .def("rotate", &gp_Pln::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Pln::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Pln::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Pln::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Pln::Translate),
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Pln::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Pln::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Pln::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Pln::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Pln::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Pln::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Pln::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Pln::Translated, py::const_),
             py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Pln& self) {
            const gp_Pnt& loc = self.Location();
            const gp_Dir& dir = self.Axis().Direction();
            return "gp_Pln(loc=(" + std::to_string(loc.X()) + ", " + 
                   std::to_string(loc.Y()) + ", " + std::to_string(loc.Z()) + "), normal=(" +
                   std::to_string(dir.X()) + ", " + std::to_string(dir.Y()) + ", " + 
                   std::to_string(dir.Z()) + "))";
        })
    ;

    py::class_<gp_Lin>(m, "Lin")
        // Constructors
        .def(py::init<>())
        .def(py::init<const gp_Ax1&>(), py::arg("a1"))
        .def(py::init<const gp_Pnt&, const gp_Dir&>(), py::arg("p"), py::arg("v"))
        
        // Properties
        .def_property("direction", &gp_Lin::Direction, &gp_Lin::SetDirection)
        .def_property("location", &gp_Lin::Location, &gp_Lin::SetLocation)
        .def_property("position", &gp_Lin::Position, &gp_Lin::SetPosition)
        
        // Reverse
        .def("reverse", &gp_Lin::Reverse)
        .def("reversed", &gp_Lin::Reversed)
        
        // Queries
        .def("angle", &gp_Lin::Angle, py::arg("other"))
        .def("contains", &gp_Lin::Contains, py::arg("p"), py::arg("linear_tolerance"))
        
        // Distance
        .def("distance", py::overload_cast<const gp_Pnt&>(&gp_Lin::Distance, py::const_), py::arg("p"))
        .def("distance", py::overload_cast<const gp_Lin&>(&gp_Lin::Distance, py::const_), py::arg("other"))
        .def("square_distance", py::overload_cast<const gp_Pnt&>(&gp_Lin::SquareDistance, py::const_), py::arg("p"))
        .def("square_distance", py::overload_cast<const gp_Lin&>(&gp_Lin::SquareDistance, py::const_), py::arg("other"))
        
        // Normal
        .def("normal", &gp_Lin::Normal, py::arg("p"))
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Lin::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Lin::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Lin::Mirror), py::arg("a2"))
        .def("rotate", &gp_Lin::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Lin::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Lin::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Lin::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Lin::Translate),
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Lin::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Lin::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Lin::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Lin::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Lin::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Lin::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Lin::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Lin::Translated, py::const_),
             py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Lin& self) {
            const gp_Pnt& loc = self.Location();
            const gp_Dir& dir = self.Direction();
            return "gp_Lin(loc=(" + std::to_string(loc.X()) + ", " + 
                   std::to_string(loc.Y()) + ", " + std::to_string(loc.Z()) + "), dir=(" +
                   std::to_string(dir.X()) + ", " + std::to_string(dir.Y()) + ", " + 
                   std::to_string(dir.Z()) + "))";
        })
    ;

    py::class_<gp_Circ>(m, "Circ")
        // Constructors
        .def(py::init<>(), "Creates an indefinite circle")
        .def(py::init<const gp_Ax2&, const Standard_Real>(), 
             py::arg("a2"), py::arg("radius"),
             "Constructs a circle with a2 locating the circle and giving its orientation:\n"
             "- center is the origin of a2\n"
             "- origin, X Direction and Y Direction of a2 define the plane\n"
             "Note: Radius can be 0.0. Raises exception if radius < 0")
        
        // Properties
        .def_property("axis", &gp_Circ::Axis, &gp_Circ::SetAxis,
                      "Get or set the main axis of the circle (perpendicular to the plane, passing through center)")
        .def_property("location", &gp_Circ::Location, &gp_Circ::SetLocation,
                      "Get or set the center of the circle (Location point of the local coordinate system)")
        .def_property("position", &gp_Circ::Position, &gp_Circ::SetPosition,
                      "Get or set the position (local coordinate system) of the circle")
        .def_property("radius", &gp_Circ::Radius, &gp_Circ::SetRadius,
                      "Get or set the radius of this circle")
        
        // Read-only properties
        .def_property_readonly("x_axis", &gp_Circ::XAxis,
                               "Returns the XAxis of the circle (perpendicular to main axis, defines parametrization origin)")
        .def_property_readonly("y_axis", &gp_Circ::YAxis,
                               "Returns the YAxis of the circle (perpendicular to XAxis, completes the plane)")
        .def("area", &gp_Circ::Area,
             "Computes the area of the circle (π * r²)")
        .def("length", &gp_Circ::Length,
             "Computes the circumference of the circle (2π * r)")
        
        // Distance and containment
        .def("distance", &gp_Circ::Distance, py::arg("p"),
             "Computes the minimum distance between point p and any point on the circumference")
        .def("square_distance", &gp_Circ::SquareDistance, py::arg("p"),
             "Computes the square distance between the circle and point p")
        .def("contains", &gp_Circ::Contains, py::arg("p"), py::arg("linear_tolerance"),
             "Returns True if point p is on the circumference within the given tolerance")
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Circ::Mirror), py::arg("p"),
             "Performs symmetrical transformation with respect to point p")
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Circ::Mirror), py::arg("a1"),
             "Performs symmetrical transformation with respect to axis a1")
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Circ::Mirror), py::arg("a2"),
             "Performs symmetrical transformation with respect to plane defined by a2")
        .def("rotate", &gp_Circ::Rotate, py::arg("a1"), py::arg("ang"),
             "Rotates the circle around axis a1 by angle ang (in radians)")
        .def("scale", &gp_Circ::Scale, py::arg("p"), py::arg("s"),
             "Scales the circle with center p and scale factor s.\n"
             "If s is negative, radius stays positive but XAxis and YAxis are reversed")
        .def("transform", &gp_Circ::Transform, py::arg("t"),
             "Transforms the circle with the transformation t")
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Circ::Translate), py::arg("v"),
             "Translates the circle in the direction of vector v")
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Circ::Translate),
             py::arg("p1"), py::arg("p2"),
             "Translates the circle from point p1 to point p2")
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Circ::Mirrored, py::const_), py::arg("p"),
             "Returns a mirrored copy with respect to point p")
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Circ::Mirrored, py::const_), py::arg("a1"),
             "Returns a mirrored copy with respect to axis a1")
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Circ::Mirrored, py::const_), py::arg("a2"),
             "Returns a mirrored copy with respect to plane defined by a2")
        .def("rotated", &gp_Circ::Rotated, py::arg("a1"), py::arg("ang"),
             "Returns a rotated copy around axis a1 by angle ang (in radians)")
        .def("scaled", &gp_Circ::Scaled, py::arg("p"), py::arg("s"),
             "Returns a scaled copy with center p and scale factor s")
        .def("transformed", &gp_Circ::Transformed, py::arg("t"),
             "Returns a transformed copy with transformation t")
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Circ::Translated, py::const_), py::arg("v"),
             "Returns a translated copy in the direction of vector v")
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Circ::Translated, py::const_),
             py::arg("p1"), py::arg("p2"),
             "Returns a translated copy from point p1 to point p2")
        
        .def("__repr__", [](const gp_Circ& self) {
            const gp_Pnt& loc = self.Location();
            const gp_Dir& dir = self.Axis().Direction();
            return "gp_Circ(center=(" + std::to_string(loc.X()) + ", " + 
                   std::to_string(loc.Y()) + ", " + std::to_string(loc.Z()) + 
                   "), normal=(" + std::to_string(dir.X()) + ", " + 
                   std::to_string(dir.Y()) + ", " + std::to_string(dir.Z()) + 
                   "), radius=" + std::to_string(self.Radius()) + ")";
        })
    ;

    py::class_<gp_Elips>(m, "Elips",
        "Describes an ellipse in 3D space.\n"
        "An ellipse is defined by its major and minor radii and positioned with a\n"
        "right-handed coordinate system where:\n"
        "- origin is the center of the ellipse\n"
        "- X Direction defines the major axis\n"
        "- Y Direction defines the minor axis\n"
        "Parameterization: P(U) = O + MajorRad*Cos(U)*XDir + MinorRad*Sin(U)*YDir\n"
        "Parameter range: [0, 2*Pi), period = 2*Pi")
        
        // Constructors
        .def(py::init<>(), "Creates an indefinite ellipse")
        .def(py::init<const gp_Ax2&, const Standard_Real, const Standard_Real>(),
             py::arg("a2"), py::arg("major_radius"), py::arg("minor_radius"),
             "Constructs an ellipse with given major and minor radii, where a2 locates the ellipse:\n"
             "- center is the origin of a2\n"
             "- X Direction defines the major axis\n"
             "- Y Direction defines the minor axis\n"
             "Raises exception if major_radius < minor_radius or minor_radius < 0")
        
        // Properties
        .def_property("axis", &gp_Elips::Axis, &gp_Elips::SetAxis,
                      "Get or set the main axis of the ellipse (perpendicular to the plane, passing through center)")
        .def_property("location", &gp_Elips::Location, &gp_Elips::SetLocation,
                      "Get or set the center of the ellipse")
        .def_property("position", &gp_Elips::Position, &gp_Elips::SetPosition,
                      "Get or set the position (local coordinate system) of the ellipse")
        .def_property("major_radius", &gp_Elips::MajorRadius, &gp_Elips::SetMajorRadius,
                      "Get or set the major radius of the ellipse")
        .def_property("minor_radius", &gp_Elips::MinorRadius, &gp_Elips::SetMinorRadius,
                      "Get or set the minor radius of the ellipse")
        
        // Read-only properties
        .def_property_readonly("x_axis", &gp_Elips::XAxis,
                               "Returns the major axis of the ellipse")
        .def_property_readonly("y_axis", &gp_Elips::YAxis,
                               "Returns the minor axis of the ellipse")
        
        // Geometric properties
        .def("area", &gp_Elips::Area,
             "Computes the area of the ellipse")
        .def("focal", &gp_Elips::Focal,
             "Computes the focal distance (distance between the two foci)")
        .def("focus1", &gp_Elips::Focus1,
             "Returns the first focus (on positive side of major axis)")
        .def("focus2", &gp_Elips::Focus2,
             "Returns the second focus (on negative side of major axis)")
        .def("directrix1", &gp_Elips::Directrix1,
             "Returns the first directrix of the ellipse")
        .def("directrix2", &gp_Elips::Directrix2,
             "Returns the second directrix of the ellipse")
        .def("eccentricity", &gp_Elips::Eccentricity,
             "Returns the eccentricity of the ellipse (0 < e < 1, or 0 if circle)")
        .def("parameter", &gp_Elips::Parameter,
             "Returns p = (1 - e²) * MajorRadius where e is the eccentricity")

        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Elips::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Elips::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Elips::Mirror), py::arg("a2"))
        .def("rotate", &gp_Elips::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Elips::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Elips::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Elips::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Elips::Translate),
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Elips::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Elips::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Elips::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Elips::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Elips::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Elips::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Elips::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Elips::Translated, py::const_),
             py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Elips& self) {
            const gp_Pnt& loc = self.Location();
            return "gp_Elips(center=(" + std::to_string(loc.X()) + ", " +
                   std::to_string(loc.Y()) + ", " + std::to_string(loc.Z()) +
                   "), major_radius=" + std::to_string(self.MajorRadius()) +
                   ", minor_radius=" + std::to_string(self.MinorRadius()) + ")";
        })
    ;

    py::class_<gp_Parab>(m, "Parab",
        "Describes a parabola in 3D space.\n"
        "A parabola is defined by its focal length and positioned with a\n"
        "right-handed coordinate system where:\n"
        "- origin is the apex of the parabola\n"
        "- X Axis defines the axis of symmetry (parabola on positive side)\n"
        "- Y Direction is parallel to the directrix\n"
        "Parameterization: P(U) = O + U²/(4*F)*XDir + U*YDir\n"
        "where F is the focal length. Parameter range: ]-infinite, +infinite[")
        
        // Constructors
        .def(py::init<>(), "Creates an indefinite parabola")
        .def(py::init<const gp_Ax2&, const Standard_Real>(),
             py::arg("a2"), py::arg("focal"),
             "Constructs a parabola with its local coordinate system a2 and focal length.\n"
             "The XDirection defines the axis of symmetry.\n"
             "The YDirection is parallel to the directrix.\n"
             "The Location point is the vertex (apex) of the parabola.\n"
             "Raises exception if focal < 0")
        .def(py::init<const gp_Ax1&, const gp_Pnt&>(),
             py::arg("d"), py::arg("f"),
             "Constructs a parabola from directrix d and focus point f")
        
        // Properties
        .def_property("axis", &gp_Parab::Axis, &gp_Parab::SetAxis,
                      "Get or set the symmetry axis of the parabola (perpendicular to directrix, passing through apex)")
        .def_property("location", &gp_Parab::Location, &gp_Parab::SetLocation,
                      "Get or set the apex (vertex) of the parabola")
        .def_property("position", &gp_Parab::Position, &gp_Parab::SetPosition,
                      "Get or set the position (local coordinate system) of the parabola")
        .def_property("focal", &gp_Parab::Focal, &gp_Parab::SetFocal,
                      "Get or set the focal length of the parabola")
        
        // Read-only properties
        .def_property_readonly("x_axis", &gp_Parab::XAxis,
                               "Returns the symmetry axis of the parabola")
        .def_property_readonly("y_axis", &gp_Parab::YAxis,
                               "Returns the YAxis of the parabola (parallel to directrix)")
        
        // Geometric properties
        .def("directrix", &gp_Parab::Directrix,
             "Returns the directrix of the parabola (on negative side of symmetry axis)")
        .def("focus", &gp_Parab::Focus,
             "Returns the focus of the parabola (on positive side of symmetry axis)")
        .def("parameter", &gp_Parab::Parameter,
             "Returns the parameter p (distance between focus and directrix = 2*focal)")
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Parab::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Parab::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Parab::Mirror), py::arg("a2"))
        .def("rotate", &gp_Parab::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Parab::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Parab::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Parab::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Parab::Translate),
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Parab::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Parab::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Parab::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Parab::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Parab::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Parab::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Parab::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Parab::Translated, py::const_),
             py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Parab& self) {
            const gp_Pnt& loc = self.Location();
            return "gp_Parab(apex=(" + std::to_string(loc.X()) + ", " +
                   std::to_string(loc.Y()) + ", " + std::to_string(loc.Z()) +
                   "), focal=" + std::to_string(self.Focal()) + ")";
        })
    ;

    py::class_<gp_Hypr>(m, "Hypr",
        "Describes a branch of a hyperbola in 3D space.\n"
        "A hyperbola is defined by its major and minor radii and positioned with a\n"
        "right-handed coordinate system where:\n"
        "- origin is the center of the hyperbola\n"
        "- X Direction defines the major axis\n"
        "- Y Direction defines the minor axis\n"
        "The branch described is on the positive side of the major axis.\n"
        "Parameterization: P(U) = O + MajRad*Cosh(U)*XDir + MinRad*Sinh(U)*YDir\n"
        "Parameter range: ]-infinite, +infinite[")
        
        // Constructors
        .def(py::init<>(), "Creates an indefinite hyperbola")
        .def(py::init<const gp_Ax2&, const Standard_Real, const Standard_Real>(),
             py::arg("a2"), py::arg("major_radius"), py::arg("minor_radius"),
             "Constructs a hyperbola with given major and minor radii, where a2 locates the hyperbola:\n"
             "- center is the origin of a2\n"
             "- X Direction defines the major axis\n"
             "- Y Direction defines the minor axis\n"
             "Raises exception if major_radius < 0 or minor_radius < 0")
        
        // Properties
        .def_property("axis", &gp_Hypr::Axis, &gp_Hypr::SetAxis,
                      "Get or set the main axis of the hyperbola (perpendicular to the plane, passing through center)")
        .def_property("location", &gp_Hypr::Location, &gp_Hypr::SetLocation,
                      "Get or set the center of the hyperbola")
        .def_property("position", &gp_Hypr::Position, &gp_Hypr::SetPosition,
                      "Get or set the position (local coordinate system) of the hyperbola")
        .def_property("major_radius", &gp_Hypr::MajorRadius, &gp_Hypr::SetMajorRadius,
                      "Get or set the major radius of the hyperbola")
        .def_property("minor_radius", &gp_Hypr::MinorRadius, &gp_Hypr::SetMinorRadius,
                      "Get or set the minor radius of the hyperbola")
        
        // Read-only properties
        .def_property_readonly("x_axis", &gp_Hypr::XAxis,
                               "Returns the major axis of the hyperbola")
        .def_property_readonly("y_axis", &gp_Hypr::YAxis,
                               "Returns the minor axis of the hyperbola")
        
        // Geometric properties
        .def("asymptote1", &gp_Hypr::Asymptote1,
             "Returns the first asymptote of the hyperbola")
        .def("asymptote2", &gp_Hypr::Asymptote2,
             "Returns the second asymptote of the hyperbola")
        .def("conjugate_branch1", &gp_Hypr::ConjugateBranch1,
             "Returns the conjugate branch on the positive side of the YAxis")
        .def("conjugate_branch2", &gp_Hypr::ConjugateBranch2,
             "Returns the conjugate branch on the negative side of the YAxis")
        .def("directrix1", &gp_Hypr::Directrix1,
             "Returns the first directrix of the hyperbola")
        .def("directrix2", &gp_Hypr::Directrix2,
             "Returns the second directrix of the hyperbola")
        .def("eccentricity", &gp_Hypr::Eccentricity,
             "Returns the eccentricity of the hyperbola (e > 1)")
        .def("focal", &gp_Hypr::Focal,
             "Computes the focal distance (distance between the two foci)")
        .def("focus1", &gp_Hypr::Focus1,
             "Returns the first focus (on positive side of major axis)")
        .def("focus2", &gp_Hypr::Focus2,
             "Returns the second focus (on negative side of major axis)")
        .def("other_branch", &gp_Hypr::OtherBranch,
             "Returns the other branch of this hyperbola (symmetrical w.r.t. center)")
        .def("parameter", &gp_Hypr::Parameter,
             "Returns p = (e² - 1) * MajorRadius where e is the eccentricity")
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Hypr::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Hypr::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Hypr::Mirror), py::arg("a2"))
        .def("rotate", &gp_Hypr::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Hypr::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Hypr::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Hypr::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Hypr::Translate),
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Hypr::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Hypr::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Hypr::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Hypr::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Hypr::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Hypr::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Hypr::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Hypr::Translated, py::const_),
             py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Hypr& self) {
            const gp_Pnt& loc = self.Location();
            return "gp_Hypr(center=(" + std::to_string(loc.X()) + ", " +
                   std::to_string(loc.Y()) + ", " + std::to_string(loc.Z()) +
                   "), major_radius=" + std::to_string(self.MajorRadius()) +
                   ", minor_radius=" + std::to_string(self.MinorRadius()) + ")";
        })
    ;

    py::enum_<gp_TrsfForm>(m, "TrsfForm",
        "Identifies the type of transformation")
        .value("Identity", gp_Identity, "No transformation")
        .value("Rotation", gp_Rotation, "Rotation transformation")
        .value("Translation", gp_Translation, "Translation transformation")
        .value("PntMirror", gp_PntMirror, "Symmetry with respect to a point")
        .value("Ax1Mirror", gp_Ax1Mirror, "Symmetry with respect to an axis")
        .value("Ax2Mirror", gp_Ax2Mirror, "Symmetry with respect to a plane")
        .value("Scale", gp_Scale, "Scale transformation")
        .value("CompoundTrsf", gp_CompoundTrsf, "Combination of transformations")
        .value("Other", gp_Other, "Other transformation")
        .export_values();

    py::class_<gp_Trsf>(m, "Trsf",
        "Defines a non-persistent transformation in 3D space.\n\n"
        "Implements transformations:\n"
        "- Translation, Rotation, Scale\n"
        "- Symmetry with respect to a point, a line, a plane\n\n"
        "Complex transformations can be obtained by combining elementary transformations.\n"
        "The transformation is represented as a 4x4 matrix where the vectorial part {V1, V2, V3}\n"
        "defines the rotation/scale and T defines the translation.")
        
        // Constructors
        .def(py::init<>(),
            "Creates an identity transformation")
        
        .def(py::init<const gp_Trsf2d&>(),
            py::arg("t"),
            "Creates a 3D transformation from a 2D transformation")
        
        // Mirror transformations
        .def("set_mirror", py::overload_cast<const gp_Pnt&>(&gp_Trsf::SetMirror),
            py::arg("p"),
            "Makes the transformation into a symmetrical transformation with respect to point p")
        
        .def("set_mirror", py::overload_cast<const gp_Ax1&>(&gp_Trsf::SetMirror),
            py::arg("a1"),
            "Makes the transformation into an axial symmetry with axis a1")
        
        .def("set_mirror", py::overload_cast<const gp_Ax2&>(&gp_Trsf::SetMirror),
            py::arg("a2"),
            "Makes the transformation into a planar symmetry with plane defined by a2")
        
        // Rotation transformations
        .def("set_rotation", py::overload_cast<const gp_Ax1&, const Standard_Real>(&gp_Trsf::SetRotation),
            py::arg("a1"), py::arg("ang"),
            "Changes the transformation into a rotation around axis a1 by angle ang (radians)")
        
        .def("set_rotation", py::overload_cast<const gp_Quaternion&>(&gp_Trsf::SetRotation),
            py::arg("r"),
            "Changes the transformation into a rotation defined by quaternion (around origin)")
        
        .def("set_rotation_part", &gp_Trsf::SetRotationPart,
            py::arg("r"),
            "Replaces the rotation part with specified quaternion")

        // Scale transformation
        .def("set_scale", &gp_Trsf::SetScale,
            py::arg("p"), py::arg("s"),
            "Changes the transformation into a scale with center p and scaling value s")
        
        // Coordinate system transformations
        .def("set_displacement", &gp_Trsf::SetDisplacement,
            py::arg("from_system"), py::arg("to_system"),
            "Modifies to transform coordinate system from_system into to_system.\n"
            "Use SetTransformation for point coordinate conversion")
        
        .def("set_transformation", py::overload_cast<const gp_Ax3&, const gp_Ax3&>(&gp_Trsf::SetTransformation),
            py::arg("from_system"), py::arg("to_system"),
            "Sets transformation to convert coordinates from from_system to to_system")
        
        .def("set_transformation", py::overload_cast<const gp_Ax3&>(&gp_Trsf::SetTransformation),
            py::arg("to_system"),
            "Sets transformation from absolute coordinate system to to_system")
        
        .def("set_transformation", py::overload_cast<const gp_Quaternion&, const gp_Vec&>(&gp_Trsf::SetTransformation),
            py::arg("r"), py::arg("t"),
            "Sets transformation by directly specified rotation and translation")
        
        // Translation
        .def("set_translation", py::overload_cast<const gp_Vec&>(&gp_Trsf::SetTranslation),
            py::arg("v"),
            "Changes the transformation into a translation by vector v")
        
        .def("set_translation", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Trsf::SetTranslation),
            py::arg("p1"), py::arg("p2"),
            "Changes the transformation into a translation from point p1 to point p2")
        
        .def("set_translation_part", &gp_Trsf::SetTranslationPart,
            py::arg("v"),
            "Replaces the translation vector with vector v")
        
        // Direct matrix setting
        .def("set_values", &gp_Trsf::SetValues,
            py::arg("a11"), py::arg("a12"), py::arg("a13"), py::arg("a14"),
            py::arg("a21"), py::arg("a22"), py::arg("a23"), py::arg("a24"),
            py::arg("a31"), py::arg("a32"), py::arg("a33"), py::arg("a34"),
            "Sets the transformation coefficients directly.\n"
            "The matrix is orthogonalized before future use.\n"
            "Raises ConstructionError if determinant is null")
        
        // Properties
        .def_property("scale_factor",
            &gp_Trsf::ScaleFactor,
            &gp_Trsf::SetScaleFactor,
            "Get or set the scale factor")
        
        .def_property("form",
            &gp_Trsf::Form,
            &gp_Trsf::SetForm,
            "Get or set the nature of the transformation")
        
        .def_property_readonly("is_negative",
            &gp_Trsf::IsNegative,
            "True if the determinant of the vectorial part is negative")

        .def_property_readonly("translation_part",
            &gp_Trsf::TranslationPart,
            "Returns the translation part of the transformation")

        .def_property_readonly("vectorial_part",
            &gp_Trsf::VectorialPart,
            "Returns the vectorial part (3x3 matrix including scale factor)")
        
        .def_property_readonly("h_vectorial_part",
            &gp_Trsf::HVectorialPart,
            "Returns the homogeneous vectorial part (3x3 matrix without scale factor)")

        // Query methods
        .def("get_rotation", [](const gp_Trsf& self) {
            gp_XYZ axis;
            Standard_Real angle;
            Standard_Boolean has_rotation = self.GetRotation(axis, angle);
            if (has_rotation) {
                return py::make_tuple(true, axis, angle);
            }
            return py::make_tuple(false, gp_XYZ(), 0.0);
        }, "Returns (has_rotation, axis, angle) where angle is in range (0, PI]")

     //    .def("get_rotation_quaternion", static_cast<gp_Quaternion (gp_Trsf::*)() const>(&gp_Trsf::GetRotation),
     .def("get_rotation_quaternion", py::overload_cast<>(&gp_Trsf::GetRotation, py::const_),
            "Returns quaternion representing the rotational part")

        .def("get_rotation",  py::overload_cast<gp_XYZ&, Standard_Real&>(&gp_Trsf::GetRotation, py::const_),
            py::arg("axis"), py::arg("angle"),
            "Outputs the axis and angle (in range (0, PI]) of the rotational part.\n"
            "Returns True if there is a rotational part, False otherwise")
          
        .def("value", &gp_Trsf::Value,
            py::arg("row"), py::arg("col"),
            "Returns coefficient at position (row, col) including scale factor.\n"
            "row: 1-3, col: 1-4")
        
        // Operations
        .def("invert", &gp_Trsf::Invert,
            "Inverts the transformation in place")
        
        .def("inverted", &gp_Trsf::Inverted,
            "Returns the inverse transformation")
        
        .def("multiply", &gp_Trsf::Multiply,
            py::arg("t"),
            "Composes this transformation with t: self = self * t")
        
        .def("multiplied", &gp_Trsf::Multiplied,
            py::arg("t"),
            "Returns the composition self * t")
        
        .def("pre_multiply", &gp_Trsf::PreMultiply,
            py::arg("t"),
            "Composes this transformation with t: self = t * self")
        
        .def("power", &gp_Trsf::Power,
            py::arg("n"),
            "Raises the transformation to power n: self = self^n")
        
        .def("powered", &gp_Trsf::Powered,
            py::arg("n"),
            "Returns the transformation raised to power n")
        
        // Transform application
        .def("transforms", py::overload_cast<Standard_Real&, Standard_Real&, Standard_Real&>(&gp_Trsf::Transforms, py::const_),
            py::arg("x"), py::arg("y"), py::arg("z"),
            "Transforms coordinates (x, y, z) in place")
        
        .def("transforms", py::overload_cast<gp_XYZ&>(&gp_Trsf::Transforms, py::const_),
            py::arg("coord"),
            "Transforms XYZ coordinates in place")

        // Operators
        .def("__mul__", &gp_Trsf::Multiplied,
            py::arg("t"),
            "Returns the composition self * t")
        
        .def("__imul__", [](gp_Trsf& self, const gp_Trsf& t) -> gp_Trsf& {
            self.Multiply(t);
            return self;
        }, py::arg("t"),
            "Composes this transformation with t: self *= t")
        
        .def("__repr__", [](const gp_Trsf& self) {
            std::string form_str;
            switch(self.Form()) {
                case gp_Identity: form_str = "Identity"; break;
                case gp_Rotation: form_str = "Rotation"; break;
                case gp_Translation: form_str = "Translation"; break;
                case gp_PntMirror: form_str = "PntMirror"; break;
                case gp_Ax1Mirror: form_str = "Ax1Mirror"; break;
                case gp_Ax2Mirror: form_str = "Ax2Mirror"; break;
                case gp_Scale: form_str = "Scale"; break;
                case gp_CompoundTrsf: form_str = "CompoundTrsf"; break;
                default: form_str = "Other";
            }
            return "gp_Trsf(form=" + form_str + 
                   ", scale=" + std::to_string(self.ScaleFactor()) + ")";
        })
    ;
    
}