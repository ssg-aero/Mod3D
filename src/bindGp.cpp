#include <Bnd_Box.hxx>
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
// #include <gp_Ax3.hxx>
// #include <gp_Trsf.hxx>

namespace py = pybind11;

PYBIND11_MODULE(gp, m)
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
}