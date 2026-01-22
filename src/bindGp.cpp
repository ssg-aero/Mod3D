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
#include <gp_Quaternion.hxx>
#include <gp_EulerSequence.hxx>
#include <gp_Cylinder.hxx>
#include <gp_Cone.hxx>
#include <gp_Sphere.hxx>
#include <gp_Torus.hxx>
#include <gp_Mat.hxx>
#include <gp_Trsf.hxx>

// 2D types
#include <gp_XY.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec2d.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Ax22d.hxx>
#include <gp_Lin2d.hxx>
#include <gp_Circ2d.hxx>
#include <gp_Elips2d.hxx>
#include <gp_Hypr2d.hxx>
#include <gp_Parab2d.hxx>
#include <gp_Trsf2d.hxx>

namespace py = pybind11;

void bind_gp(py::module_ &m)
{
    // gp utility class - provides resolution and standard geometric constants
    py::class_<gp>(m, "gp",
        "Geometric processor utility class.\n\n"
        "Provides implementation of entities for algebraic calculations and\n"
        "basis analytic geometry. This class offers static methods for:\n"
        "- Tolerance criterion (Resolution)\n"
        "- Standard 3D geometric constants (Origin, unit directions, axes)\n"
        "- Standard 2D geometric constants")
        
        // Resolution
        .def_static("resolution", &gp::Resolution,
            "Returns the tolerance criterion for geometric computations.\n\n"
            "This tolerance is used to determine when two numbers can be\n"
            "considered equal. Many class functions use this tolerance,\n"
            "for example, to avoid division by zero")
        
        // 3D Point and Direction constants
        .def_property_readonly_static("origin", 
            [](py::object /* self */) { return gp::Origin(); },
            "Cartesian point with coordinates X = Y = Z = 0")
        
        .def_property_readonly_static("dx",
            [](py::object /* self */) { return gp::DX(); },
            "Unit vector with direction (1, 0, 0)")
        
        .def_property_readonly_static("dy",
            [](py::object /* self */) { return gp::DY(); },
            "Unit vector with direction (0, 1, 0)")
        
        .def_property_readonly_static("dz",
            [](py::object /* self */) { return gp::DZ(); },
            "Unit vector with direction (0, 0, 1)")
        
        // 3D Axis constants
        .def_property_readonly_static("ox",
            [](py::object /* self */) { return gp::OX(); },
            "Axis with origin at Origin and direction (1, 0, 0)")
        
        .def_property_readonly_static("oy",
            [](py::object /* self */) { return gp::OY(); },
            "Axis with origin at Origin and direction (0, 1, 0)")
        
        .def_property_readonly_static("oz",
            [](py::object /* self */) { return gp::OZ(); },
            "Axis with origin at Origin and direction (0, 0, 1)")
        
        // 3D Coordinate system constants
        .def_property_readonly_static("xoy",
            [](py::object /* self */) { return gp::XOY(); },
            "Coordinate system with origin at Origin,\n"
            "main direction Z = (0, 0, 1) and X direction = (1, 0, 0)")
        
        .def_property_readonly_static("zox",
            [](py::object /* self */) { return gp::ZOX(); },
            "Coordinate system with origin at Origin,\n"
            "main direction Y = (0, 1, 0) and X direction = (0, 0, 1)")
        
        .def_property_readonly_static("yoz",
            [](py::object /* self */) { return gp::YOZ(); },
            "Coordinate system with origin at Origin,\n"
            "main direction X = (1, 0, 0) and X direction = (0, 1, 0)")
        
        // 2D Point and Direction constants
        .def_property_readonly_static("origin2d",
            [](py::object /* self */) { return gp::Origin2d(); },
            "Cartesian point with coordinates X = Y = 0")
        
        .def_property_readonly_static("dx2d",
            [](py::object /* self */) { return gp::DX2d(); },
            "Unit vector with direction (1, 0)")
        
        .def_property_readonly_static("dy2d",
            [](py::object /* self */) { return gp::DY2d(); },
            "Unit vector with direction (0, 1)")
        
        // 2D Axis constants
        .def_property_readonly_static("ox2d",
            [](py::object /* self */) { return gp::OX2d(); },
            "Axis with origin at Origin2d and direction (1, 0)")
        
        .def_property_readonly_static("oy2d",
            [](py::object /* self */) { return gp::OY2d(); },
            "Axis with origin at Origin2d and direction (0, 1)")
    ;

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
        .def_property_readonly("magnitude", &gp_Vec::Magnitude)
        .def_property_readonly("square_magnitude", &gp_Vec::SquareMagnitude)
        
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

    py::class_<gp_Ax3>(m, "Ax3")
        .def(py::init<>())
        .def(py::init<const gp_Pnt&, const gp_Dir&>(), py::arg("p"), py::arg("v"))
        .def(py::init<const gp_Pnt&, const gp_Dir&, const gp_Dir&>(), 
             py::arg("p"), py::arg("n"), py::arg("vx"))
        .def(py::init<const gp_Ax2&>(), py::arg("a2"))
        
        // Getters
        .def("angle", &gp_Ax3::Angle, py::arg("other"))
        .def_property("axis", &gp_Ax3::Axis, &gp_Ax3::SetAxis)
        .def_property("direction", &gp_Ax3::Direction, &gp_Ax3::SetDirection)
        .def_property("location", &gp_Ax3::Location, &gp_Ax3::SetLocation)
        .def_property("x_direction", &gp_Ax3::XDirection, &gp_Ax3::SetXDirection)
        .def_property("y_direction", &gp_Ax3::YDirection, &gp_Ax3::SetYDirection)
        .def_property("z_direction", &gp_Ax3::Direction, &gp_Ax3::SetDirection)
        
        // Get as Ax2
        .def("as_ax2", &gp_Ax3::Ax2)
        
        // Comparison
        .def("is_coplanar", py::overload_cast<const gp_Ax3&, const Standard_Real, const Standard_Real>(&gp_Ax3::IsCoplanar, py::const_),
             py::arg("other"), py::arg("linear_tolerance"), py::arg("angular_tolerance"))
        .def("is_coplanar", py::overload_cast<const gp_Ax1&, const Standard_Real, const Standard_Real>(&gp_Ax3::IsCoplanar, py::const_),
             py::arg("a1"), py::arg("linear_tolerance"), py::arg("angular_tolerance"))
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Ax3::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Ax3::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Ax3::Mirror), py::arg("a2"))
        .def("rotate", &gp_Ax3::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Ax3::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Ax3::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Ax3::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Ax3::Translate), 
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Ax3::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Ax3::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Ax3::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Ax3::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Ax3::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Ax3::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Ax3::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Ax3::Translated, py::const_), 
             py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Ax3& self) {
            const gp_Pnt& loc = self.Location();
            const gp_Dir& dir = self.Direction();
            const gp_Dir& xdir = self.XDirection();
            return "gp_Ax3(loc=(" + std::to_string(loc.X()) + ", " + 
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

    py::class_<gp_Cylinder>(m, "Cylinder")
        .def(py::init<>())
        .def(py::init<const gp_Ax2&, Standard_Real>(), 
             py::arg("a2"), py::arg("radius"))
        
        // Properties
        .def_property("axis", &gp_Cylinder::Axis, &gp_Cylinder::SetAxis)
        .def_property("location", &gp_Cylinder::Location, &gp_Cylinder::SetLocation)
        .def_property("position", &gp_Cylinder::Position, &gp_Cylinder::SetPosition)
        .def_property("radius", &gp_Cylinder::Radius, &gp_Cylinder::SetRadius)
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Cylinder::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Cylinder::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Cylinder::Mirror), py::arg("a2"))
        .def("rotate", &gp_Cylinder::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Cylinder::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Cylinder::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Cylinder::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Cylinder::Translate), 
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Cylinder::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Cylinder::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Cylinder::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Cylinder::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Cylinder::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Cylinder::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Cylinder::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Cylinder::Translated, py::const_), 
             py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Cylinder& self) {
            const gp_Pnt& loc = self.Location();
            const gp_Dir& dir = self.Axis().Direction();
            return "gp_Cylinder(loc=(" + std::to_string(loc.X()) + ", " + 
                   std::to_string(loc.Y()) + ", " + std::to_string(loc.Z()) + "), radius=" +
                   std::to_string(self.Radius()) + ")";
        })
    ;

    py::class_<gp_Cone>(m, "Cone")
        .def(py::init<>())
        .def(py::init<const gp_Ax2&, Standard_Real, Standard_Real>(), 
             py::arg("a2"), py::arg("semi_angle"), py::arg("radius"))
        
        // Properties
        .def_property("axis", &gp_Cone::Axis, &gp_Cone::SetAxis)
        .def_property("location", &gp_Cone::Location, &gp_Cone::SetLocation)
        .def_property("position", &gp_Cone::Position, &gp_Cone::SetPosition)
        .def_property("semi_angle", &gp_Cone::SemiAngle, &gp_Cone::SetSemiAngle)
        .def_property("radius", &gp_Cone::RefRadius, &gp_Cone::SetRadius)
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Cone::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Cone::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Cone::Mirror), py::arg("a2"))
        .def("rotate", &gp_Cone::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Cone::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Cone::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Cone::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Cone::Translate), 
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Cone::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Cone::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Cone::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Cone::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Cone::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Cone::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Cone::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Cone::Translated, py::const_), 
             py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Cone& self) {
            return "gp_Cone(semi_angle=" + std::to_string(self.SemiAngle()) + 
                   ", radius=" + std::to_string(self.RefRadius()) + ")";
        })
    ;
    ;

    py::class_<gp_Sphere>(m, "Sphere")
        .def(py::init<>())
        .def(py::init<const gp_Ax2&, Standard_Real>(), 
             py::arg("a2"), py::arg("radius"))
        
        // Properties
        .def_property("location", &gp_Sphere::Location, &gp_Sphere::SetLocation)
        .def_property("position", &gp_Sphere::Position, &gp_Sphere::SetPosition)
        .def_property("radius", &gp_Sphere::Radius, &gp_Sphere::SetRadius)
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Sphere::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Sphere::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Sphere::Mirror), py::arg("a2"))
        .def("rotate", &gp_Sphere::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Sphere::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Sphere::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Sphere::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Sphere::Translate), 
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Sphere::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Sphere::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Sphere::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Sphere::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Sphere::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Sphere::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Sphere::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Sphere::Translated, py::const_), 
             py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Sphere& self) {
            return "gp_Sphere(radius=" + std::to_string(self.Radius()) + ")";
        })
    ;

    py::class_<gp_Torus>(m, "Torus")
        .def(py::init<>())
        .def(py::init<const gp_Ax2&, Standard_Real, Standard_Real>(), 
             py::arg("a2"), py::arg("major_radius"), py::arg("minor_radius"))
        
        // Properties
        .def_property("axis", &gp_Torus::Axis, &gp_Torus::SetAxis)
        .def_property("location", &gp_Torus::Location, &gp_Torus::SetLocation)
        .def_property("position", &gp_Torus::Position, &gp_Torus::SetPosition)
        .def_property("major_radius", &gp_Torus::MajorRadius, &gp_Torus::SetMajorRadius)
        .def_property("minor_radius", &gp_Torus::MinorRadius, &gp_Torus::SetMinorRadius)
        
        // Transformations (in-place)
        .def("mirror", py::overload_cast<const gp_Pnt&>(&gp_Torus::Mirror), py::arg("p"))
        .def("mirror", py::overload_cast<const gp_Ax1&>(&gp_Torus::Mirror), py::arg("a1"))
        .def("mirror", py::overload_cast<const gp_Ax2&>(&gp_Torus::Mirror), py::arg("a2"))
        .def("rotate", &gp_Torus::Rotate, py::arg("a1"), py::arg("ang"))
        .def("scale", &gp_Torus::Scale, py::arg("p"), py::arg("s"))
        .def("transform", &gp_Torus::Transform, py::arg("t"))
        .def("translate", py::overload_cast<const gp_Vec&>(&gp_Torus::Translate), py::arg("v"))
        .def("translate", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Torus::Translate), 
             py::arg("p1"), py::arg("p2"))
        
        // Transformations (returns new)
        .def("mirrored", py::overload_cast<const gp_Pnt&>(&gp_Torus::Mirrored, py::const_), py::arg("p"))
        .def("mirrored", py::overload_cast<const gp_Ax1&>(&gp_Torus::Mirrored, py::const_), py::arg("a1"))
        .def("mirrored", py::overload_cast<const gp_Ax2&>(&gp_Torus::Mirrored, py::const_), py::arg("a2"))
        .def("rotated", &gp_Torus::Rotated, py::arg("a1"), py::arg("ang"))
        .def("scaled", &gp_Torus::Scaled, py::arg("p"), py::arg("s"))
        .def("transformed", &gp_Torus::Transformed, py::arg("t"))
        .def("translated", py::overload_cast<const gp_Vec&>(&gp_Torus::Translated, py::const_), py::arg("v"))
        .def("translated", py::overload_cast<const gp_Pnt&, const gp_Pnt&>(&gp_Torus::Translated, py::const_), 
             py::arg("p1"), py::arg("p2"))
        
        .def("__repr__", [](const gp_Torus& self) {
            const gp_Pnt& loc = self.Location();
            return "gp_Torus(loc=(" + std::to_string(loc.X()) + ", " + 
                   std::to_string(loc.Y()) + ", " + std::to_string(loc.Z()) + "), major_radius=" +
                   std::to_string(self.MajorRadius()) + ", minor_radius=" + std::to_string(self.MinorRadius()) + ")";
        })
    ;

    py::class_<gp_Lin>(m, "Lin", 
        "Representation of an infinite line defined by a point and a direction.")
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

    // =========================================================================
    // gp_Mat - 3x3 Matrix
    // =========================================================================
    py::class_<gp_Mat>(m, "Mat",
        "Describes a three column, three row matrix for various vectorial or matrix computations.")
        .def(py::init<>(), "Creates a matrix with null coefficients")
        .def(py::init<double, double, double,
                      double, double, double,
                      double, double, double>(),
             py::arg("a11"), py::arg("a12"), py::arg("a13"),
             py::arg("a21"), py::arg("a22"), py::arg("a23"),
             py::arg("a31"), py::arg("a32"), py::arg("a33"),
             "Creates a matrix with specified coefficients")
        .def(py::init<const gp_XYZ&, const gp_XYZ&, const gp_XYZ&>(),
             py::arg("col1"), py::arg("col2"), py::arg("col3"),
             "Creates a matrix from three column vectors")

        // Properties
        .def_property_readonly("determinant", &gp_Mat::Determinant,
             "Returns the determinant of the matrix")
        .def_property_readonly("diagonal", &gp_Mat::Diagonal,
             "Returns the main diagonal as gp_XYZ")
        .def_property_readonly("is_singular", &gp_Mat::IsSingular,
             "Returns True if the matrix is singular (determinant ~ 0)")

        // Element access
        .def("value", &gp_Mat::Value, py::arg("row"), py::arg("col"),
             "Returns the coefficient at (row, col), 1-indexed")
        .def("set_value", &gp_Mat::SetValue, py::arg("row"), py::arg("col"), py::arg("value"),
             "Sets the coefficient at (row, col), 1-indexed")
        .def("column", &gp_Mat::Column, py::arg("col"),
             "Returns the column of given index (1-3) as gp_XYZ")
        .def("row", &gp_Mat::Row, py::arg("row"),
             "Returns the row of given index (1-3) as gp_XYZ")
        .def("set_col", &gp_Mat::SetCol, py::arg("col"), py::arg("value"),
             "Assigns gp_XYZ to the column of given index")
        .def("set_cols", &gp_Mat::SetCols, py::arg("col1"), py::arg("col2"), py::arg("col3"),
             "Assigns three gp_XYZ to the columns")
        .def("set_row", &gp_Mat::SetRow, py::arg("row"), py::arg("value"),
             "Assigns gp_XYZ to the row of given index")
        .def("set_rows", &gp_Mat::SetRows, py::arg("row1"), py::arg("row2"), py::arg("row3"),
             "Assigns three gp_XYZ to the rows")
        .def("set_diagonal", &gp_Mat::SetDiagonal, py::arg("x1"), py::arg("x2"), py::arg("x3"),
             "Modifies the main diagonal of the matrix")

        // Special matrices
        .def("set_identity", &gp_Mat::SetIdentity,
             "Modifies this matrix to represent the identity matrix")
        .def("set_rotation", &gp_Mat::SetRotation, py::arg("axis"), py::arg("angle"),
             "Sets this matrix to represent a rotation around axis by angle (radians)")
        .def("set_scale", &gp_Mat::SetScale, py::arg("scale"),
             "Sets this matrix to represent uniform scaling")
        .def("set_cross", &gp_Mat::SetCross, py::arg("ref"),
             "Sets matrix M such that M * v = ref.Cross(v)")
        .def("set_dot", &gp_Mat::SetDot, py::arg("ref"),
             "Sets matrix M such that M * v = ref.Dot(v)")

        // Operations
        .def("invert", &gp_Mat::Invert, "Inverts this matrix in place")
        .def("inverted", &gp_Mat::Inverted, "Returns the inverse matrix")
        .def("transpose", &gp_Mat::Transpose, "Transposes this matrix in place")
        .def("transposed", &gp_Mat::Transposed, "Returns the transposed matrix")
        .def("power", &gp_Mat::Power, py::arg("n"),
             "Computes self^n in place. n=0 gives identity, n<0 gives inverse^|n|")
        .def("powered", &gp_Mat::Powered, py::arg("n"),
             "Returns self^n")

        // Python operators
        .def("__add__", &gp_Mat::Added, py::arg("other"))
        .def("__sub__", &gp_Mat::Subtracted, py::arg("other"))
        .def("__mul__", py::overload_cast<const gp_Mat&>(&gp_Mat::Multiplied, py::const_), py::arg("other"))
        .def("__mul__", py::overload_cast<double>(&gp_Mat::Multiplied, py::const_), py::arg("scalar"))
        .def("__rmul__", py::overload_cast<double>(&gp_Mat::Multiplied, py::const_), py::arg("scalar"))
        .def("__truediv__", &gp_Mat::Divided, py::arg("scalar"))
        .def("__iadd__", [](gp_Mat& self, const gp_Mat& other) { self.Add(other); return self; })
        .def("__isub__", [](gp_Mat& self, const gp_Mat& other) { self.Subtract(other); return self; })
        .def("__imul__", [](gp_Mat& self, const gp_Mat& other) { self.Multiply(other); return self; })
        .def("__imul__", [](gp_Mat& self, double scalar) { self.Multiply(scalar); return self; })
        .def("__itruediv__", [](gp_Mat& self, double scalar) { self.Divide(scalar); return self; })

        .def("__getitem__", [](const gp_Mat& self, std::pair<int, int> idx) {
            return self.Value(idx.first, idx.second);
        }, py::arg("index"), "Access element by (row, col) tuple, 1-indexed")
        .def("__setitem__", [](gp_Mat& self, std::pair<int, int> idx, double value) {
            self.SetValue(idx.first, idx.second, value);
        }, py::arg("index"), py::arg("value"), "Set element by (row, col) tuple, 1-indexed")

        .def("__repr__", [](const gp_Mat& self) {
            return "gp_Mat([" + 
                   std::to_string(self.Value(1,1)) + ", " + std::to_string(self.Value(1,2)) + ", " + std::to_string(self.Value(1,3)) + "; " +
                   std::to_string(self.Value(2,1)) + ", " + std::to_string(self.Value(2,2)) + ", " + std::to_string(self.Value(2,3)) + "; " +
                   std::to_string(self.Value(3,1)) + ", " + std::to_string(self.Value(3,2)) + ", " + std::to_string(self.Value(3,3)) + "])";
        })
    ;

    // =========================================================================
    // gp_EulerSequence - Euler angle sequence enumeration
    // =========================================================================
    py::enum_<gp_EulerSequence>(m, "EulerSequence",
        "Enumerates all 24 possible variants of generalized Euler angles.")
        .value("EulerAngles", gp_EulerAngles, "Classic Euler angles (Intrinsic ZXZ)")
        .value("YawPitchRoll", gp_YawPitchRoll, "Yaw Pitch Roll (Intrinsic ZYX)")
        .value("Extrinsic_XYZ", gp_Extrinsic_XYZ)
        .value("Extrinsic_XZY", gp_Extrinsic_XZY)
        .value("Extrinsic_YZX", gp_Extrinsic_YZX)
        .value("Extrinsic_YXZ", gp_Extrinsic_YXZ)
        .value("Extrinsic_ZXY", gp_Extrinsic_ZXY)
        .value("Extrinsic_ZYX", gp_Extrinsic_ZYX)
        .value("Intrinsic_XYZ", gp_Intrinsic_XYZ)
        .value("Intrinsic_XZY", gp_Intrinsic_XZY)
        .value("Intrinsic_YZX", gp_Intrinsic_YZX)
        .value("Intrinsic_YXZ", gp_Intrinsic_YXZ)
        .value("Intrinsic_ZXY", gp_Intrinsic_ZXY)
        .value("Intrinsic_ZYX", gp_Intrinsic_ZYX)
        .value("Extrinsic_XYX", gp_Extrinsic_XYX)
        .value("Extrinsic_XZX", gp_Extrinsic_XZX)
        .value("Extrinsic_YZY", gp_Extrinsic_YZY)
        .value("Extrinsic_YXY", gp_Extrinsic_YXY)
        .value("Extrinsic_ZYZ", gp_Extrinsic_ZYZ)
        .value("Extrinsic_ZXZ", gp_Extrinsic_ZXZ)
        .value("Intrinsic_XYX", gp_Intrinsic_XYX)
        .value("Intrinsic_XZX", gp_Intrinsic_XZX)
        .value("Intrinsic_YZY", gp_Intrinsic_YZY)
        .value("Intrinsic_YXY", gp_Intrinsic_YXY)
        .value("Intrinsic_ZXZ", gp_Intrinsic_ZXZ)
        .value("Intrinsic_ZYZ", gp_Intrinsic_ZYZ)
        .export_values()
    ;

    // =========================================================================
    // gp_Quaternion - Quaternion for 3D rotations
    // =========================================================================
    py::class_<gp_Quaternion>(m, "Quaternion",
        "Represents rotation in 3D space as a quaternion and provides operations\n"
        "with rotations based on quaternion mathematics. Also provides conversion\n"
        "to/from other rotation representations (matrix, axis+angle, Euler angles).")
        .def(py::init<>(), "Creates an identity quaternion (no rotation)")
        .def(py::init<double, double, double, double>(),
             py::arg("x"), py::arg("y"), py::arg("z"), py::arg("w"),
             "Creates quaternion from components (x, y, z, w)")
        .def(py::init<const gp_Vec&, const gp_Vec&>(),
             py::arg("vec_from"), py::arg("vec_to"),
             "Creates quaternion for shortest-arc rotation from vec_from to vec_to")
        .def(py::init<const gp_Vec&, const gp_Vec&, const gp_Vec&>(),
             py::arg("vec_from"), py::arg("vec_to"), py::arg("help_cross_vec"),
             "Creates quaternion for rotation from vec_from to vec_to with preferred axis")
        .def(py::init<const gp_Vec&, double>(),
             py::arg("axis"), py::arg("angle"),
             "Creates quaternion for rotation around axis by angle (radians)")
        .def(py::init<const gp_Mat&>(),
             py::arg("matrix"),
             "Creates quaternion from 3x3 rotation matrix")

        // Component properties
        .def_property_readonly("x", &gp_Quaternion::X, "X component of quaternion")
        .def_property_readonly("y", &gp_Quaternion::Y, "Y component of quaternion")
        .def_property_readonly("z", &gp_Quaternion::Z, "Z component of quaternion")
        .def_property_readonly("w", &gp_Quaternion::W, "W (scalar) component of quaternion")
        .def_property_readonly("norm", &gp_Quaternion::Norm, "Returns the norm of the quaternion")
        .def_property_readonly("square_norm", &gp_Quaternion::SquareNorm, "Returns the square norm")
        .def_property_readonly("rotation_angle", &gp_Quaternion::GetRotationAngle,
             "Returns the rotation angle from -PI to PI")

        // Setting
        .def("set", py::overload_cast<double, double, double, double>(&gp_Quaternion::Set),
             py::arg("x"), py::arg("y"), py::arg("z"), py::arg("w"),
             "Sets quaternion components")
        .def("set_ident", &gp_Quaternion::SetIdent,
             "Sets this to identity quaternion (no rotation)")
        .def("set_rotation", py::overload_cast<const gp_Vec&, const gp_Vec&>(&gp_Quaternion::SetRotation),
             py::arg("vec_from"), py::arg("vec_to"),
             "Sets quaternion to shortest-arc rotation from vec_from to vec_to")
        .def("set_rotation", py::overload_cast<const gp_Vec&, const gp_Vec&, const gp_Vec&>(&gp_Quaternion::SetRotation),
             py::arg("vec_from"), py::arg("vec_to"), py::arg("help_cross_vec"),
             "Sets quaternion to rotation from vec_from to vec_to with preferred axis")

        // Axis-angle representation
        .def("set_vector_and_angle", &gp_Quaternion::SetVectorAndAngle,
             py::arg("axis"), py::arg("angle"),
             "Sets quaternion from axis and angle (radians)")
        .def("get_vector_and_angle", [](const gp_Quaternion& self) {
            gp_Vec axis;
            double angle;
            self.GetVectorAndAngle(axis, angle);
            return py::make_tuple(axis, angle);
        }, "Returns (axis, angle) tuple representing the rotation")

        // Matrix representation
        .def("set_matrix", &gp_Quaternion::SetMatrix, py::arg("matrix"),
             "Sets quaternion from 3x3 rotation matrix")
        .def_property_readonly("matrix", &gp_Quaternion::GetMatrix,
             "Returns rotation as 3x3 matrix")

        // Euler angles
        .def("set_euler_angles", &gp_Quaternion::SetEulerAngles,
             py::arg("order"), py::arg("alpha"), py::arg("beta"), py::arg("gamma"),
             "Sets quaternion from Euler angles (radians) in specified sequence")
        .def("get_euler_angles", [](const gp_Quaternion& self, gp_EulerSequence order) {
            double alpha, beta, gamma;
            self.GetEulerAngles(order, alpha, beta, gamma);
            return py::make_tuple(alpha, beta, gamma);
        }, py::arg("order"), "Returns (alpha, beta, gamma) Euler angles in specified sequence")

        // Operations
        .def("is_equal", &gp_Quaternion::IsEqual, py::arg("other"),
             "Tests equality (simple comparison without tolerance)")
        .def("reverse", &gp_Quaternion::Reverse,
             "Reverses the rotation direction (conjugates) in place")
        .def("reversed", &gp_Quaternion::Reversed,
             "Returns quaternion with reversed rotation direction")
        .def("invert", &gp_Quaternion::Invert,
             "Inverts this quaternion (both direction and norm) in place")
        .def("inverted", &gp_Quaternion::Inverted,
             "Returns the inverse quaternion q^-1")
        .def("normalize", &gp_Quaternion::Normalize,
             "Scales quaternion so its norm becomes 1")
        .def("normalized", &gp_Quaternion::Normalized,
             "Returns normalized copy of quaternion")
        .def("stabilize_length", &gp_Quaternion::StabilizeLength,
             "Stabilizes quaternion length within 1 - 1/4 (faster than normalize)")
        .def("scale", &gp_Quaternion::Scale, py::arg("scale"),
             "Scales all components by given factor")
        .def("scaled", &gp_Quaternion::Scaled, py::arg("scale"),
             "Returns quaternion with all components scaled")
        .def("dot", &gp_Quaternion::Dot, py::arg("other"),
             "Returns the dot product with another quaternion")
        .def("multiply_vec", py::overload_cast<const gp_Vec&>(&gp_Quaternion::Multiply, py::const_),
             py::arg("vec"), "Rotates vector by this quaternion")

        // Python operators
        .def("__neg__", &gp_Quaternion::Negated)
        .def("__add__", &gp_Quaternion::Added, py::arg("other"))
        .def("__sub__", &gp_Quaternion::Subtracted, py::arg("other"))
        .def("__mul__", py::overload_cast<const gp_Quaternion&>(&gp_Quaternion::Multiplied, py::const_), py::arg("other"))
        .def("__mul__", py::overload_cast<double>(&gp_Quaternion::Scaled, py::const_), py::arg("scalar"))
        .def("__mul__", py::overload_cast<const gp_Vec&>(&gp_Quaternion::Multiply, py::const_), py::arg("vec"))
        .def("__rmul__", py::overload_cast<double>(&gp_Quaternion::Scaled, py::const_), py::arg("scalar"))
        .def("__iadd__", [](gp_Quaternion& self, const gp_Quaternion& other) { self.Add(other); return self; })
        .def("__isub__", [](gp_Quaternion& self, const gp_Quaternion& other) { self.Subtract(other); return self; })
        .def("__imul__", [](gp_Quaternion& self, const gp_Quaternion& other) { self.Multiply(other); return self; })
        .def("__imul__", [](gp_Quaternion& self, double scalar) { self.Scale(scalar); return self; })

        .def("__repr__", [](const gp_Quaternion& self) {
            return "gp_Quaternion(" + 
                   std::to_string(self.X()) + ", " + 
                   std::to_string(self.Y()) + ", " + 
                   std::to_string(self.Z()) + ", " + 
                   std::to_string(self.W()) + ")";
        })
    ;

    // =========================================================================
    // 2D GEOMETRIC TYPES
    // =========================================================================

    // =========================================================================
    // gp_XY - 2D coordinate pair
    // =========================================================================
    py::class_<gp_XY>(m, "XY",
        "Describes a coordinate pair (X, Y) in 2D space.")
        .def(py::init<>(), "Creates XY with zero coordinates")
        .def(py::init<double, double>(), py::arg("x"), py::arg("y"),
             "Creates XY with given coordinates")

        .def_property("x", &gp_XY::X, &gp_XY::SetX,
             "Get or set the X coordinate")
        .def_property("y", &gp_XY::Y, &gp_XY::SetY,
             "Get or set the Y coordinate")
        .def_property("coord",
            [](const gp_XY& self) { return py::make_tuple(self.X(), self.Y()); },
            [](gp_XY& self, py::tuple t) {
                if (t.size() != 2)
                    throw std::runtime_error("Tuple of size 2 expected for coord");
                self.SetCoord(t[0].cast<double>(), t[1].cast<double>());
            },
                "Get or set both coordinates as a tuple (x, y)")
        .def_property("xy",
            [](const gp_XY& self) { return self; },
            [](gp_XY& self, const gp_XY& other) { self.SetX(other.X()); self.SetY(other.Y()); },
            "Get or set both coordinates using another XY")
        .def_property_readonly("modulus", &gp_XY::Modulus, "Returns sqrt(X^2 + Y^2)")
        .def("square_modulus", &gp_XY::SquareModulus, "Returns X^2 + Y^2")
        .def("is_equal", &gp_XY::IsEqual, py::arg("other"), py::arg("tolerance"),
             "Returns True if distance to other is <= tolerance")
        .def("dot", &gp_XY::Dot, py::arg("other"), "Returns the dot product")
        .def("crossed", &gp_XY::Crossed, py::arg("other"),
             "Returns the cross product (X1*Y2 - Y1*X2)")
        .def("normalize", &gp_XY::Normalize, "Normalizes this XY")
        .def("normalized", &gp_XY::Normalized, "Returns a normalized copy")
        .def("reverse", &gp_XY::Reverse, "Reverses both coordinates")
        .def("reversed", &gp_XY::Reversed, "Returns a reversed copy")
        .def("__add__", &gp_XY::Added, py::arg("other"))
        .def("__sub__", &gp_XY::Subtracted, py::arg("other"))
        .def("__mul__", py::overload_cast<double>(&gp_XY::Multiplied, py::const_), py::arg("scalar"))
        .def("__truediv__", &gp_XY::Divided, py::arg("scalar"))
        .def("__neg__", &gp_XY::Reversed)
        .def("__repr__", [](const gp_XY& self) {
            return "gp_XY(" + std::to_string(self.X()) + ", " + 
                   std::to_string(self.Y()) + ")";
        })
    ;

    // =========================================================================
    // gp_Pnt2d - 2D Point
    // =========================================================================
    py::class_<gp_Pnt2d>(m, "Pnt2d",
        "Describes a point in 2D space.")
        .def(py::init<>(), "Creates point at origin (0, 0)")
        .def(py::init<const double, const double>(), py::arg("x"), py::arg("y"),
             "Creates point at (X, Y)")
        .def(py::init<const gp_XY&>(), py::arg("coord"),
             "Creates point from XY coordinates")
        .def_property("x", &gp_Pnt2d::X, &gp_Pnt2d::SetX,
             "Get or set the X coordinate")
        .def_property("y", &gp_Pnt2d::Y, &gp_Pnt2d::SetY,
             "Get or set the Y coordinate")
        .def_property("xy", [](const gp_Pnt2d& self) { return self.XY(); }, &gp_Pnt2d::SetXY,
             "Sets both coordinates from XY")
        .def_property("coord",
            [](const gp_Pnt2d& self) { return self.Coord(); },
            &gp_Pnt2d::SetXY,
            "Get or set the XY coordinates")
        .def("is_equal", &gp_Pnt2d::IsEqual, py::arg("other"), py::arg("tolerance"),
             "Returns True if distance to other is <= tolerance")
        .def("distance", &gp_Pnt2d::Distance, py::arg("other"),
             "Returns the distance to another point")
        .def("square_distance", &gp_Pnt2d::SquareDistance, py::arg("other"),
             "Returns the squared distance to another point")
        .def("mirror", py::overload_cast<const gp_Pnt2d&>(&gp_Pnt2d::Mirror),
             py::arg("p"), "Mirrors this point about point P")
        .def("mirrored", py::overload_cast<const gp_Pnt2d&>(&gp_Pnt2d::Mirrored, py::const_),
             py::arg("p"), "Returns a point mirrored about point P")
        .def("mirror", py::overload_cast<const gp_Ax2d&>(&gp_Pnt2d::Mirror),
             py::arg("a"), "Mirrors this point about axis A")
        .def("mirrored", py::overload_cast<const gp_Ax2d&>(&gp_Pnt2d::Mirrored, py::const_),
             py::arg("a"), "Returns a point mirrored about axis A")
        .def("rotate", &gp_Pnt2d::Rotate, py::arg("p"), py::arg("angle"),
             "Rotates this point about point P by angle (radians)")
        .def("rotated", &gp_Pnt2d::Rotated, py::arg("p"), py::arg("angle"),
             "Returns a rotated copy")
        .def("scale", &gp_Pnt2d::Scale, py::arg("p"), py::arg("s"),
             "Scales this point about point P by factor S")
        .def("scaled", &gp_Pnt2d::Scaled, py::arg("p"), py::arg("s"),
             "Returns a scaled copy")
        .def("transform", &gp_Pnt2d::Transform, py::arg("t"),
             "Transforms this point by transformation T")
        .def("transformed", &gp_Pnt2d::Transformed, py::arg("t"),
             "Returns a transformed copy")
        .def("translate", py::overload_cast<const gp_Vec2d&>(&gp_Pnt2d::Translate),
             py::arg("v"), "Translates this point by vector V")
        .def("translated", py::overload_cast<const gp_Vec2d&>(&gp_Pnt2d::Translated, py::const_),
             py::arg("v"), "Returns a translated copy")
        .def("__repr__", [](const gp_Pnt2d& self) {
            return "gp_Pnt2d(" + std::to_string(self.X()) + ", " + 
                   std::to_string(self.Y()) + ")";
        })
    ;

    // =========================================================================
    // gp_Vec2d - 2D Vector
    // =========================================================================
    py::class_<gp_Vec2d>(m, "Vec2d",
        "Describes a vector in 2D space.")
        .def(py::init<>(), "Creates a null vector")
        .def(py::init<double, double>(), py::arg("x"), py::arg("y"),
             "Creates vector with components (X, Y)")
        .def(py::init<const gp_XY&>(), py::arg("coord"),
             "Creates vector from XY coordinates")
        .def(py::init<const gp_Dir2d&>(), py::arg("v"),
             "Creates a unit vector from a direction")
        .def(py::init<const gp_Pnt2d&, const gp_Pnt2d&>(), py::arg("p1"), py::arg("p2"),
             "Creates vector from P1 to P2")
        .def_property("x", &gp_Vec2d::X, &gp_Vec2d::SetX,
             "Get or set the X component")
        .def_property("y", &gp_Vec2d::Y, &gp_Vec2d::SetY,
             "Get or set the Y component")
        .def_property("xy",
            [](const gp_Vec2d& self) { return self.XY(); }, 
             &gp_Vec2d::SetXY 
        )
        .def_property("coord",
            [](const gp_Vec2d& self) { return self.XY(); }, 
             &gp_Vec2d::SetXY 
        )
        .def_property_readonly("magnitude", &gp_Vec2d::Magnitude, "Returns the magnitude")
        .def_property_readonly("square_magnitude", &gp_Vec2d::SquareMagnitude, "Returns the squared magnitude")
        .def("is_equal", &gp_Vec2d::IsEqual, py::arg("other"), py::arg("linear_tolerance"), py::arg("angular_tolerance"),
             "Tests equality within tolerances")
        .def("is_normal", &gp_Vec2d::IsNormal, py::arg("other"), py::arg("angular_tolerance"),
             "Tests if vectors are normal (perpendicular)")
        .def("is_opposite", &gp_Vec2d::IsOpposite, py::arg("other"), py::arg("angular_tolerance"),
             "Tests if vectors are opposite")
        .def("is_parallel", &gp_Vec2d::IsParallel, py::arg("other"), py::arg("angular_tolerance"),
             "Tests if vectors are parallel")
        .def("angle", &gp_Vec2d::Angle, py::arg("other"),
             "Returns the angle to another vector (-PI to PI)")
        .def("dot", &gp_Vec2d::Dot, py::arg("other"), "Returns the dot product")
        .def("crossed", &gp_Vec2d::Crossed, py::arg("other"),
             "Returns the cross product (X1*Y2 - Y1*X2)")
        .def("cross_magnitude", &gp_Vec2d::CrossMagnitude, py::arg("other"),
             "Returns the magnitude of the cross product")
        .def("cross_square_magnitude", &gp_Vec2d::CrossSquareMagnitude, py::arg("other"),
             "Returns the squared magnitude of the cross product")
        .def("normalize", &gp_Vec2d::Normalize, "Normalizes this vector")
        .def("normalized", &gp_Vec2d::Normalized, "Returns a normalized copy")
        .def("reverse", &gp_Vec2d::Reverse, "Reverses this vector")
        .def("reversed", &gp_Vec2d::Reversed, "Returns a reversed copy")
        .def("mirror", py::overload_cast<const gp_Vec2d&>(&gp_Vec2d::Mirror),
             py::arg("v"), "Mirrors this vector about vector V")
        .def("mirrored", py::overload_cast<const gp_Vec2d&>(&gp_Vec2d::Mirrored, py::const_),
             py::arg("v"), "Returns a mirrored copy")
        .def("mirror", py::overload_cast<const gp_Ax2d&>(&gp_Vec2d::Mirror),
             py::arg("a"), "Mirrors this vector about axis A")
        .def("mirrored", py::overload_cast<const gp_Ax2d&>(&gp_Vec2d::Mirrored, py::const_),
             py::arg("a"), "Returns a mirrored copy")
        .def("rotate", &gp_Vec2d::Rotate, py::arg("angle"),
             "Rotates this vector by angle (radians)")
        .def("rotated", &gp_Vec2d::Rotated, py::arg("angle"),
             "Returns a rotated copy")
        .def("scale", &gp_Vec2d::Scale, py::arg("s"), "Scales this vector")
        .def("scaled", &gp_Vec2d::Scaled, py::arg("s"), "Returns a scaled copy")
        .def("transform", &gp_Vec2d::Transform, py::arg("t"),
             "Transforms this vector")
        .def("transformed", &gp_Vec2d::Transformed, py::arg("t"),
             "Returns a transformed copy")
        .def("__add__", &gp_Vec2d::Added, py::arg("other"))
        .def("__sub__", &gp_Vec2d::Subtracted, py::arg("other"))
        .def("__mul__", &gp_Vec2d::Multiplied, py::arg("scalar"))
        .def("__truediv__", &gp_Vec2d::Divided, py::arg("scalar"))
        .def("__neg__", &gp_Vec2d::Reversed)
        .def("__repr__", [](const gp_Vec2d& self) {
            return "gp_Vec2d(" + std::to_string(self.X()) + ", " + 
                   std::to_string(self.Y()) + ")";
        })
    ;

    // =========================================================================
    // gp_Dir2d - 2D Direction (unit vector)
    // =========================================================================
    py::class_<gp_Dir2d>(m, "Dir2d",
        "Describes a unit vector (direction) in 2D space.\n"
        "Always normalized to have magnitude 1.")
        .def(py::init<>(), "Creates direction (1, 0)")
        .def(py::init<double, double>(), py::arg("x"), py::arg("y"),
             "Creates direction from components (will be normalized)")
        .def(py::init<const gp_XY&>(), py::arg("coord"),
             "Creates direction from XY (will be normalized)")
        .def(py::init<const gp_Vec2d&>(), py::arg("v"),
             "Creates direction from vector (will be normalized)")
        .def_property("x", &gp_Dir2d::X, &gp_Dir2d::SetX,
             "Get or set the X component (will renormalize on set)")
        .def_property("y", &gp_Dir2d::Y, &gp_Dir2d::SetY,
             "Get or set the Y component (will renormalize on set)")
        .def_property("xy",
            [](const gp_Dir2d& self) { return self.XY(); },
            &gp_Dir2d::SetXY,
            "Get or set the XY components (will be normalized on set)")
        .def_property("coord",
            [](const gp_Dir2d& self) { return self.XY(); },
            &gp_Dir2d::SetXY,
            "Get or set the XY components (will be normalized on set)")
        .def("is_equal", &gp_Dir2d::IsEqual, py::arg("other"), py::arg("angular_tolerance"),
             "Tests equality within angular tolerance")
        .def("is_normal", &gp_Dir2d::IsNormal, py::arg("other"), py::arg("angular_tolerance"),
             "Tests if directions are normal (perpendicular)")
        .def("is_opposite", &gp_Dir2d::IsOpposite, py::arg("other"), py::arg("angular_tolerance"),
             "Tests if directions are opposite")
        .def("is_parallel", &gp_Dir2d::IsParallel, py::arg("other"), py::arg("angular_tolerance"),
             "Tests if directions are parallel")
        .def("angle", &gp_Dir2d::Angle, py::arg("other"),
             "Returns the angle to another direction (-PI to PI)")
        .def("dot", &gp_Dir2d::Dot, py::arg("other"), "Returns the dot product")
        .def("crossed", &gp_Dir2d::Crossed, py::arg("other"),
             "Returns the cross product (X1*Y2 - Y1*X2)")
        .def("reverse", &gp_Dir2d::Reverse, "Reverses this direction")
        .def("reversed", &gp_Dir2d::Reversed, "Returns a reversed copy")
        .def("mirror", py::overload_cast<const gp_Dir2d&>(&gp_Dir2d::Mirror),
             py::arg("v"), "Mirrors about direction V")
        .def("mirrored", py::overload_cast<const gp_Dir2d&>(&gp_Dir2d::Mirrored, py::const_),
             py::arg("v"), "Returns a mirrored copy")
        .def("mirror", py::overload_cast<const gp_Ax2d&>(&gp_Dir2d::Mirror),
             py::arg("a"), "Mirrors about axis A")
        .def("mirrored", py::overload_cast<const gp_Ax2d&>(&gp_Dir2d::Mirrored, py::const_),
             py::arg("a"), "Returns a mirrored copy")
        .def("rotate", &gp_Dir2d::Rotate, py::arg("angle"),
             "Rotates by angle (radians)")
        .def("rotated", &gp_Dir2d::Rotated, py::arg("angle"),
             "Returns a rotated copy")
        .def("transform", &gp_Dir2d::Transform, py::arg("t"),
             "Transforms this direction")
        .def("transformed", &gp_Dir2d::Transformed, py::arg("t"),
             "Returns a transformed copy")
        .def("__repr__", [](const gp_Dir2d& self) {
            return "gp_Dir2d(" + std::to_string(self.X()) + ", " + 
                   std::to_string(self.Y()) + ")";
        })
    ;

    // =========================================================================
    // gp_Ax2d - 2D Axis (point + direction)
    // =========================================================================
    py::class_<gp_Ax2d>(m, "Ax2d",
        "Describes an axis in 2D space: a point (location) and a direction.")
        .def(py::init<>(), "Creates an axis at origin with direction (1, 0)")
        .def(py::init<const gp_Pnt2d&, const gp_Dir2d&>(), py::arg("p"), py::arg("v"),
             "Creates an axis at point P with direction V")
        .def_property("location", &gp_Ax2d::Location, &gp_Ax2d::SetLocation,
             "Get or set the location point")
        .def_property("direction", &gp_Ax2d::Direction, &gp_Ax2d::SetDirection,
                "Get or set the direction")
        .def("is_coaxial", &gp_Ax2d::IsCoaxial, py::arg("other"), 
             py::arg("angular_tolerance"), py::arg("linear_tolerance"),
             "Tests if axes are coaxial within tolerances")
        .def("is_normal", &gp_Ax2d::IsNormal, py::arg("other"), py::arg("angular_tolerance"),
             "Tests if axes are normal (perpendicular)")
        .def("is_opposite", &gp_Ax2d::IsOpposite, py::arg("other"), py::arg("angular_tolerance"),
             "Tests if axes are opposite")
        .def("is_parallel", &gp_Ax2d::IsParallel, py::arg("other"), py::arg("angular_tolerance"),
             "Tests if axes are parallel")
        .def("angle", &gp_Ax2d::Angle, py::arg("other"),
             "Returns the angle between directions")
        .def("reverse", &gp_Ax2d::Reverse, "Reverses the direction")
        .def("reversed", &gp_Ax2d::Reversed, "Returns a reversed copy")
        .def("mirror", py::overload_cast<const gp_Pnt2d&>(&gp_Ax2d::Mirror),
             py::arg("p"), "Mirrors about point P")
        .def("mirrored", py::overload_cast<const gp_Pnt2d&>(&gp_Ax2d::Mirrored, py::const_),
             py::arg("p"), "Returns a mirrored copy")
        .def("mirror", py::overload_cast<const gp_Ax2d&>(&gp_Ax2d::Mirror),
             py::arg("a"), "Mirrors about axis A")
        .def("mirrored", py::overload_cast<const gp_Ax2d&>(&gp_Ax2d::Mirrored, py::const_),
             py::arg("a"), "Returns a mirrored copy")
        .def("rotate", &gp_Ax2d::Rotate, py::arg("p"), py::arg("angle"),
             "Rotates about point P by angle (radians)")
        .def("rotated", &gp_Ax2d::Rotated, py::arg("p"), py::arg("angle"),
             "Returns a rotated copy")
        .def("scale", &gp_Ax2d::Scale, py::arg("p"), py::arg("s"),
             "Scales about point P by factor S")
        .def("scaled", &gp_Ax2d::Scaled, py::arg("p"), py::arg("s"),
             "Returns a scaled copy")
        .def("transform", &gp_Ax2d::Transform, py::arg("t"),
             "Transforms this axis")
        .def("transformed", &gp_Ax2d::Transformed, py::arg("t"),
             "Returns a transformed copy")
        .def("translate", py::overload_cast<const gp_Vec2d&>(&gp_Ax2d::Translate),
             py::arg("v"), "Translates by vector V")
        .def("translated", py::overload_cast<const gp_Vec2d&>(&gp_Ax2d::Translated, py::const_),
             py::arg("v"), "Returns a translated copy")
        .def("__repr__", [](const gp_Ax2d& self) {
            return "gp_Ax2d(location=(" + std::to_string(self.Location().X()) + ", " + 
                   std::to_string(self.Location().Y()) + "), direction=(" +
                   std::to_string(self.Direction().X()) + ", " + 
                   std::to_string(self.Direction().Y()) + "))";
        })
    ;

    // =========================================================================
    // gp_Ax22d - 2D Coordinate System (point + two orthogonal directions)
    // =========================================================================
    py::class_<gp_Ax22d>(m, "Ax22d",
        "Describes a coordinate system in 2D: an origin and two perpendicular axes (X and Y).")
        .def(py::init<>(), "Creates a coordinate system at origin with X=(1,0), Y=(0,1)")
        .def(py::init<const gp_Pnt2d&, const gp_Dir2d&, const gp_Dir2d&>(),
             py::arg("p"), py::arg("vx"), py::arg("vy"),
             "Creates a coordinate system at P with X direction Vx and Y direction Vy")
        .def(py::init<const gp_Pnt2d&, const gp_Dir2d&, bool>(),
             py::arg("p"), py::arg("v"), py::arg("sense") = true,
             "Creates a coordinate system at P with X direction V.\n"
             "Y is computed perpendicular (counter-clockwise if sense=True)")
        .def(py::init<const gp_Ax2d&, bool>(), py::arg("a"), py::arg("sense") = true,
             "Creates a coordinate system from axis A")
        .def("location", &gp_Ax22d::Location, "Returns the origin")
        .def("x_axis", &gp_Ax22d::XAxis, "Returns the X axis")
        .def("y_axis", &gp_Ax22d::YAxis, "Returns the Y axis")
        .def("x_direction", &gp_Ax22d::XDirection, "Returns the X direction")
        .def("y_direction", &gp_Ax22d::YDirection, "Returns the Y direction")
        .def("set_location", &gp_Ax22d::SetLocation, py::arg("p"), "Sets the origin")
        .def("set_x_axis", &gp_Ax22d::SetXAxis, py::arg("a"), "Sets the X axis")
        .def("set_y_axis", &gp_Ax22d::SetYAxis, py::arg("a"), "Sets the Y axis")
        .def("set_x_direction", &gp_Ax22d::SetXDirection, py::arg("vx"), "Sets the X direction")
        .def("set_y_direction", &gp_Ax22d::SetYDirection, py::arg("vy"), "Sets the Y direction")
        .def("mirror", py::overload_cast<const gp_Pnt2d&>(&gp_Ax22d::Mirror),
             py::arg("p"), "Mirrors about point P")
        .def("mirrored", py::overload_cast<const gp_Pnt2d&>(&gp_Ax22d::Mirrored, py::const_),
             py::arg("p"), "Returns a mirrored copy")
        .def("mirror", py::overload_cast<const gp_Ax2d&>(&gp_Ax22d::Mirror),
             py::arg("a"), "Mirrors about axis A")
        .def("mirrored", py::overload_cast<const gp_Ax2d&>(&gp_Ax22d::Mirrored, py::const_),
             py::arg("a"), "Returns a mirrored copy")
        .def("rotate", &gp_Ax22d::Rotate, py::arg("p"), py::arg("angle"),
             "Rotates about point P")
        .def("rotated", &gp_Ax22d::Rotated, py::arg("p"), py::arg("angle"),
             "Returns a rotated copy")
        .def("scale", &gp_Ax22d::Scale, py::arg("p"), py::arg("s"), "Scales about point P")
        .def("scaled", &gp_Ax22d::Scaled, py::arg("p"), py::arg("s"), "Returns a scaled copy")
        .def("transform", &gp_Ax22d::Transform, py::arg("t"), "Transforms this coordinate system")
        .def("transformed", &gp_Ax22d::Transformed, py::arg("t"), "Returns a transformed copy")
        .def("translate", py::overload_cast<const gp_Vec2d&>(&gp_Ax22d::Translate),
             py::arg("v"), "Translates by vector V")
        .def("translated", py::overload_cast<const gp_Vec2d&>(&gp_Ax22d::Translated, py::const_),
             py::arg("v"), "Returns a translated copy")
        .def("__repr__", [](const gp_Ax22d& self) {
            return "gp_Ax22d(origin=(" + std::to_string(self.Location().X()) + ", " +
                   std::to_string(self.Location().Y()) + "))";
        })
    ;

    // =========================================================================
    // gp_Lin2d - 2D Line
    // =========================================================================
    py::class_<gp_Lin2d>(m, "Lin2d",
        "Describes an infinite line in 2D space: a point and a direction.")
        .def(py::init<>(), "Creates a line along X axis through origin")
        .def(py::init<const gp_Ax2d&>(), py::arg("a"),
             "Creates a line from an axis")
        .def(py::init<const gp_Pnt2d&, const gp_Dir2d&>(), py::arg("p"), py::arg("v"),
             "Creates a line through point P with direction V")
        .def(py::init<double, double, double>(), py::arg("a"), py::arg("b"), py::arg("c"),
             "Creates a line from equation A*X + B*Y + C = 0")
        .def("location", &gp_Lin2d::Location, "Returns a point on the line")
        .def("direction", &gp_Lin2d::Direction, "Returns the direction")
        .def("position", &gp_Lin2d::Position, "Returns the axis")
        .def("set_location", &gp_Lin2d::SetLocation, py::arg("p"), "Sets the location")
        .def("set_direction", &gp_Lin2d::SetDirection, py::arg("v"), "Sets the direction")
        .def("set_position", &gp_Lin2d::SetPosition, py::arg("a"), "Sets the axis")
        .def("coefficients", [](const gp_Lin2d& self) {
            double a, b, c;
            self.Coefficients(a, b, c);
            return py::make_tuple(a, b, c);
        }, "Returns the coefficients (A, B, C) of A*X + B*Y + C = 0")
        .def("angle", &gp_Lin2d::Angle, py::arg("other"),
             "Returns the angle to another line")
        .def("contains", &gp_Lin2d::Contains, py::arg("p"), py::arg("linear_tolerance"),
             "Tests if point P is on the line within tolerance")
        .def("distance", py::overload_cast<const gp_Pnt2d&>(&gp_Lin2d::Distance, py::const_),
             py::arg("p"), "Returns the distance to point P")
        .def("distance", py::overload_cast<const gp_Lin2d&>(&gp_Lin2d::Distance, py::const_),
             py::arg("other"), "Returns the distance to another line")
        .def("square_distance", py::overload_cast<const gp_Pnt2d&>(&gp_Lin2d::SquareDistance, py::const_),
             py::arg("p"), "Returns the squared distance to point P")
        .def("square_distance", py::overload_cast<const gp_Lin2d&>(&gp_Lin2d::SquareDistance, py::const_),
             py::arg("other"), "Returns the squared distance to another line")
        .def("normal", &gp_Lin2d::Normal, py::arg("p"),
             "Returns a line perpendicular to this line through point P")
        .def("reverse", &gp_Lin2d::Reverse, "Reverses the direction")
        .def("reversed", &gp_Lin2d::Reversed, "Returns a reversed copy")
        .def("mirror", py::overload_cast<const gp_Pnt2d&>(&gp_Lin2d::Mirror),
             py::arg("p"), "Mirrors about point P")
        .def("mirrored", py::overload_cast<const gp_Pnt2d&>(&gp_Lin2d::Mirrored, py::const_),
             py::arg("p"), "Returns a mirrored copy")
        .def("mirror", py::overload_cast<const gp_Ax2d&>(&gp_Lin2d::Mirror),
             py::arg("a"), "Mirrors about axis A")
        .def("mirrored", py::overload_cast<const gp_Ax2d&>(&gp_Lin2d::Mirrored, py::const_),
             py::arg("a"), "Returns a mirrored copy")
        .def("rotate", &gp_Lin2d::Rotate, py::arg("p"), py::arg("angle"),
             "Rotates about point P")
        .def("rotated", &gp_Lin2d::Rotated, py::arg("p"), py::arg("angle"),
             "Returns a rotated copy")
        .def("scale", &gp_Lin2d::Scale, py::arg("p"), py::arg("s"), "Scales about point P")
        .def("scaled", &gp_Lin2d::Scaled, py::arg("p"), py::arg("s"), "Returns a scaled copy")
        .def("transform", &gp_Lin2d::Transform, py::arg("t"), "Transforms this line")
        .def("transformed", &gp_Lin2d::Transformed, py::arg("t"), "Returns a transformed copy")
        .def("translate", py::overload_cast<const gp_Vec2d&>(&gp_Lin2d::Translate),
             py::arg("v"), "Translates by vector V")
        .def("translated", py::overload_cast<const gp_Vec2d&>(&gp_Lin2d::Translated, py::const_),
             py::arg("v"), "Returns a translated copy")
        .def("__repr__", [](const gp_Lin2d& self) {
            return "gp_Lin2d(location=(" + std::to_string(self.Location().X()) + ", " +
                   std::to_string(self.Location().Y()) + "), direction=(" +
                   std::to_string(self.Direction().X()) + ", " +
                   std::to_string(self.Direction().Y()) + "))";
        })
    ;

    // =========================================================================
    // gp_Circ2d - 2D Circle
    // =========================================================================
    py::class_<gp_Circ2d>(m, "Circ2d",
        "Describes a circle in 2D space: a center point and a radius.")
        .def(py::init<>(), "Creates a circle with center at origin and radius 1")
        .def(py::init<const gp_Ax2d&, double, bool>(),
             py::arg("a"), py::arg("radius"), py::arg("sense") = true,
             "Creates a circle with axis A (center at A.Location()) and radius")
        .def(py::init<const gp_Ax22d&, double>(), py::arg("a"), py::arg("radius"),
             "Creates a circle with coordinate system A and radius")
        .def("location", &gp_Circ2d::Location, "Returns the center")
        .def("radius", &gp_Circ2d::Radius, "Returns the radius")
        .def("axis", &gp_Circ2d::Axis, "Returns the main axis")
        .def("position", &gp_Circ2d::Position, "Returns the coordinate system")
        .def("x_axis", &gp_Circ2d::XAxis, "Returns the X axis")
        .def("y_axis", &gp_Circ2d::YAxis, "Returns the Y axis")
        .def("set_location", &gp_Circ2d::SetLocation, py::arg("p"), "Sets the center")
        .def("set_radius", &gp_Circ2d::SetRadius, py::arg("r"), "Sets the radius")
        .def("set_axis", &gp_Circ2d::SetAxis, py::arg("a"), "Sets the main axis")
        .def("set_x_axis", &gp_Circ2d::SetXAxis, py::arg("a"), "Sets the X axis")
        .def("set_y_axis", &gp_Circ2d::SetYAxis, py::arg("a"), "Sets the Y axis")
        .def("area", &gp_Circ2d::Area, "Returns the area")
        .def("length", &gp_Circ2d::Length, "Returns the circumference")
        .def("contains", &gp_Circ2d::Contains, py::arg("p"), py::arg("linear_tolerance"),
             "Tests if point P is on the circle within tolerance")
        .def("distance", &gp_Circ2d::Distance, py::arg("p"),
             "Returns the distance to point P")
        .def("square_distance", &gp_Circ2d::SquareDistance, py::arg("p"),
             "Returns the squared distance to point P")
        .def("reverse", &gp_Circ2d::Reverse, "Reverses the orientation")
        .def("reversed", &gp_Circ2d::Reversed, "Returns a reversed copy")
        .def("is_direct", &gp_Circ2d::IsDirect,
             "Returns True if the coordinate system is direct (counter-clockwise)")
        .def("mirror", py::overload_cast<const gp_Pnt2d&>(&gp_Circ2d::Mirror),
             py::arg("p"), "Mirrors about point P")
        .def("mirrored", py::overload_cast<const gp_Pnt2d&>(&gp_Circ2d::Mirrored, py::const_),
             py::arg("p"), "Returns a mirrored copy")
        .def("mirror", py::overload_cast<const gp_Ax2d&>(&gp_Circ2d::Mirror),
             py::arg("a"), "Mirrors about axis A")
        .def("mirrored", py::overload_cast<const gp_Ax2d&>(&gp_Circ2d::Mirrored, py::const_),
             py::arg("a"), "Returns a mirrored copy")
        .def("rotate", &gp_Circ2d::Rotate, py::arg("p"), py::arg("angle"),
             "Rotates about point P")
        .def("rotated", &gp_Circ2d::Rotated, py::arg("p"), py::arg("angle"),
             "Returns a rotated copy")
        .def("scale", &gp_Circ2d::Scale, py::arg("p"), py::arg("s"), "Scales about point P")
        .def("scaled", &gp_Circ2d::Scaled, py::arg("p"), py::arg("s"), "Returns a scaled copy")
        .def("transform", &gp_Circ2d::Transform, py::arg("t"), "Transforms this circle")
        .def("transformed", &gp_Circ2d::Transformed, py::arg("t"), "Returns a transformed copy")
        .def("translate", py::overload_cast<const gp_Vec2d&>(&gp_Circ2d::Translate),
             py::arg("v"), "Translates by vector V")
        .def("translated", py::overload_cast<const gp_Vec2d&>(&gp_Circ2d::Translated, py::const_),
             py::arg("v"), "Returns a translated copy")
        .def("__repr__", [](const gp_Circ2d& self) {
            return "gp_Circ2d(center=(" + std::to_string(self.Location().X()) + ", " +
                   std::to_string(self.Location().Y()) + "), radius=" +
                   std::to_string(self.Radius()) + ")";
        })
    ;

    // =========================================================================
    // gp_Elips2d - 2D Ellipse
    // =========================================================================
    py::class_<gp_Elips2d>(m, "Elips2d",
        "Describes an ellipse in 2D space.")
        .def(py::init<>(), "Creates an ellipse")
        .def(py::init<const gp_Ax2d&, double, double, bool>(),
             py::arg("a"), py::arg("major_radius"), py::arg("minor_radius"), py::arg("sense") = true,
             "Creates an ellipse with axis A and radii")
        .def(py::init<const gp_Ax22d&, double, double>(),
             py::arg("a"), py::arg("major_radius"), py::arg("minor_radius"),
             "Creates an ellipse with coordinate system A and radii")
        .def("location", &gp_Elips2d::Location, "Returns the center")
        .def("major_radius", &gp_Elips2d::MajorRadius, "Returns the major radius")
        .def("minor_radius", &gp_Elips2d::MinorRadius, "Returns the minor radius")
        .def("axis", &gp_Elips2d::Axis, "Returns the main axis")
        .def("x_axis", &gp_Elips2d::XAxis, "Returns the major axis")
        .def("y_axis", &gp_Elips2d::YAxis, "Returns the minor axis")
        .def("set_location", &gp_Elips2d::SetLocation, py::arg("p"), "Sets the center")
        .def("set_major_radius", &gp_Elips2d::SetMajorRadius, py::arg("r"), "Sets the major radius")
        .def("set_minor_radius", &gp_Elips2d::SetMinorRadius, py::arg("r"), "Sets the minor radius")
        .def("set_axis", &gp_Elips2d::SetAxis, py::arg("a"), "Sets the main axis")
        .def("set_x_axis", &gp_Elips2d::SetXAxis, py::arg("a"), "Sets the major axis")
        .def("set_y_axis", &gp_Elips2d::SetYAxis, py::arg("a"), "Sets the minor axis")
        .def("area", &gp_Elips2d::Area, "Returns the area")
        .def("eccentricity", &gp_Elips2d::Eccentricity, "Returns the eccentricity")
        .def("focal", &gp_Elips2d::Focal, "Returns the focal distance")
        .def("focus1", &gp_Elips2d::Focus1, "Returns the first focus")
        .def("focus2", &gp_Elips2d::Focus2, "Returns the second focus")
        .def("parameter", &gp_Elips2d::Parameter, "Returns the parameter")
        .def("directrix1", &gp_Elips2d::Directrix1, "Returns the first directrix")
        .def("directrix2", &gp_Elips2d::Directrix2, "Returns the second directrix")
        .def("is_direct", &gp_Elips2d::IsDirect, "Returns True if direct orientation")
        .def("reverse", &gp_Elips2d::Reverse, "Reverses the orientation")
        .def("reversed", &gp_Elips2d::Reversed, "Returns a reversed copy")
        .def("__repr__", [](const gp_Elips2d& self) {
            return "gp_Elips2d(center=(" + std::to_string(self.Location().X()) + ", " +
                   std::to_string(self.Location().Y()) + "), major=" +
                   std::to_string(self.MajorRadius()) + ", minor=" +
                   std::to_string(self.MinorRadius()) + ")";
        })
    ;

    // =========================================================================
    // gp_Hypr2d - 2D Hyperbola
    // =========================================================================
    py::class_<gp_Hypr2d>(m, "Hypr2d",
        "Describes a hyperbola in 2D space.")
        .def(py::init<>(), "Creates a hyperbola")
        .def(py::init<const gp_Ax2d&, double, double, bool>(),
             py::arg("a"), py::arg("major_radius"), py::arg("minor_radius"), py::arg("sense") = true,
             "Creates a hyperbola with axis A and radii")
        .def(py::init<const gp_Ax22d&, double, double>(),
             py::arg("a"), py::arg("major_radius"), py::arg("minor_radius"),
             "Creates a hyperbola with coordinate system A and radii")
        .def("location", &gp_Hypr2d::Location, "Returns the center")
        .def("major_radius", &gp_Hypr2d::MajorRadius, "Returns the major radius")
        .def("minor_radius", &gp_Hypr2d::MinorRadius, "Returns the minor radius")
        .def("axis", &gp_Hypr2d::Axis, "Returns the main axis")
        .def("x_axis", &gp_Hypr2d::XAxis, "Returns the transverse axis")
        .def("y_axis", &gp_Hypr2d::YAxis, "Returns the conjugate axis")
        .def("set_location", &gp_Hypr2d::SetLocation, py::arg("p"), "Sets the center")
        .def("set_major_radius", &gp_Hypr2d::SetMajorRadius, py::arg("r"), "Sets the major radius")
        .def("set_minor_radius", &gp_Hypr2d::SetMinorRadius, py::arg("r"), "Sets the minor radius")
        .def("set_axis", &gp_Hypr2d::SetAxis, py::arg("a"), "Sets the main axis")
        .def("set_x_axis", &gp_Hypr2d::SetXAxis, py::arg("a"), "Sets the transverse axis")
        .def("set_y_axis", &gp_Hypr2d::SetYAxis, py::arg("a"), "Sets the conjugate axis")
        .def("eccentricity", &gp_Hypr2d::Eccentricity, "Returns the eccentricity")
        .def("focal", &gp_Hypr2d::Focal, "Returns the focal distance")
        .def("focus1", &gp_Hypr2d::Focus1, "Returns the first focus")
        .def("focus2", &gp_Hypr2d::Focus2, "Returns the second focus")
        .def("parameter", &gp_Hypr2d::Parameter, "Returns the parameter")
        .def("asymptote1", &gp_Hypr2d::Asymptote1, "Returns the first asymptote")
        .def("asymptote2", &gp_Hypr2d::Asymptote2, "Returns the second asymptote")
        .def("directrix1", &gp_Hypr2d::Directrix1, "Returns the first directrix")
        .def("directrix2", &gp_Hypr2d::Directrix2, "Returns the second directrix")
        .def("conjugate_branch1", &gp_Hypr2d::ConjugateBranch1, "Returns the first conjugate branch")
        .def("conjugate_branch2", &gp_Hypr2d::ConjugateBranch2, "Returns the second conjugate branch")
        .def("other_branch", &gp_Hypr2d::OtherBranch, "Returns the other branch")
        .def("is_direct", &gp_Hypr2d::IsDirect, "Returns True if direct orientation")
        .def("reverse", &gp_Hypr2d::Reverse, "Reverses the orientation")
        .def("reversed", &gp_Hypr2d::Reversed, "Returns a reversed copy")
        .def("__repr__", [](const gp_Hypr2d& self) {
            return "gp_Hypr2d(center=(" + std::to_string(self.Location().X()) + ", " +
                   std::to_string(self.Location().Y()) + "), major=" +
                   std::to_string(self.MajorRadius()) + ", minor=" +
                   std::to_string(self.MinorRadius()) + ")";
        })
    ;

    // =========================================================================
    // gp_Parab2d - 2D Parabola
    // =========================================================================
    py::class_<gp_Parab2d>(m, "Parab2d",
        "Describes a parabola in 2D space.")
        .def(py::init<>(), "Creates a parabola")
        .def(py::init<const gp_Ax2d&, double, bool>(),
             py::arg("a"), py::arg("focal"), py::arg("sense") = true,
             "Creates a parabola with axis A and focal length")
        .def(py::init<const gp_Ax22d&, double>(), py::arg("a"), py::arg("focal"),
             "Creates a parabola with coordinate system A and focal length")
        .def("location", &gp_Parab2d::Location, "Returns the vertex")
        .def("focal", &gp_Parab2d::Focal, "Returns the focal length")
        .def("axis", &gp_Parab2d::Axis, "Returns the axis of symmetry")
        .def("mirror_axis", &gp_Parab2d::MirrorAxis, "Returns the axis of symmetry")
        .def("set_location", &gp_Parab2d::SetLocation, py::arg("p"), "Sets the vertex")
        .def("set_focal", &gp_Parab2d::SetFocal, py::arg("f"), "Sets the focal length")
        .def("set_axis", &gp_Parab2d::SetAxis, py::arg("a"), "Sets the axis")
        .def("set_mirror_axis", &gp_Parab2d::SetMirrorAxis, py::arg("a"), "Sets the axis")
        .def("focus", &gp_Parab2d::Focus, "Returns the focus")
        .def("directrix", &gp_Parab2d::Directrix, "Returns the directrix")
        .def("parameter", &gp_Parab2d::Parameter, "Returns the parameter (2 * focal)")
        .def("is_direct", &gp_Parab2d::IsDirect, "Returns True if direct orientation")
        .def("reverse", &gp_Parab2d::Reverse, "Reverses the orientation")
        .def("reversed", &gp_Parab2d::Reversed, "Returns a reversed copy")
        .def("__repr__", [](const gp_Parab2d& self) {
            return "gp_Parab2d(vertex=(" + std::to_string(self.Location().X()) + ", " +
                   std::to_string(self.Location().Y()) + "), focal=" +
                   std::to_string(self.Focal()) + ")";
        })
    ;

    // =========================================================================
    // gp_Trsf2d - 2D Transformation
    // =========================================================================
    py::class_<gp_Trsf2d>(m, "Trsf2d",
        "Describes a 2D transformation: translation, rotation, scaling, or combination.")
        .def(py::init<>(), "Creates an identity transformation")
        .def("set_mirror", py::overload_cast<const gp_Pnt2d&>(&gp_Trsf2d::SetMirror),
             py::arg("p"), "Sets as point mirror about P")
        .def("set_mirror", py::overload_cast<const gp_Ax2d&>(&gp_Trsf2d::SetMirror),
             py::arg("a"), "Sets as axis mirror about A")
        .def("set_rotation", &gp_Trsf2d::SetRotation, py::arg("p"), py::arg("angle"),
             "Sets as rotation about point P")
        .def("set_scale", &gp_Trsf2d::SetScale, py::arg("p"), py::arg("s"),
             "Sets as scaling about point P")
        .def("set_translation", py::overload_cast<const gp_Vec2d&>(&gp_Trsf2d::SetTranslation),
             py::arg("v"), "Sets as translation by vector V")
        .def("set_translation", py::overload_cast<const gp_Pnt2d&, const gp_Pnt2d&>(&gp_Trsf2d::SetTranslation),
             py::arg("p1"), py::arg("p2"), "Sets as translation from P1 to P2")
        .def("set_transformation", py::overload_cast<const gp_Ax2d&, const gp_Ax2d&>(&gp_Trsf2d::SetTransformation),
             py::arg("from_system"), py::arg("to_system"),
             "Sets transformation between coordinate systems")
        .def("set_transformation", py::overload_cast<const gp_Ax2d&>(&gp_Trsf2d::SetTransformation),
             py::arg("to_system"), "Sets transformation from global to local system")
        .def("is_negative", &gp_Trsf2d::IsNegative, "Returns True if determinant is negative")
        .def("form", &gp_Trsf2d::Form, "Returns the transformation form")
        .def("scale_factor", &gp_Trsf2d::ScaleFactor, "Returns the scale factor")
        .def("rotation_part", &gp_Trsf2d::RotationPart, "Returns the rotation angle")
        .def("translation_part", &gp_Trsf2d::TranslationPart, "Returns the translation vector")
        .def("value", &gp_Trsf2d::Value, py::arg("row"), py::arg("col"),
             "Returns the coefficient at row, col (1-indexed)")
        .def("invert", &gp_Trsf2d::Invert, "Inverts this transformation")
        .def("inverted", &gp_Trsf2d::Inverted, "Returns the inverse")
        .def("multiply", &gp_Trsf2d::Multiply, py::arg("t"),
             "Composes with transformation T: self = self * T")
        .def("multiplied", &gp_Trsf2d::Multiplied, py::arg("t"),
             "Returns the composition self * T")
        .def("pre_multiply", &gp_Trsf2d::PreMultiply, py::arg("t"),
             "Composes with transformation T: self = T * self")
        .def("power", &gp_Trsf2d::Power, py::arg("n"),
             "Raises to power n")
        .def("powered", &gp_Trsf2d::Powered, py::arg("n"),
             "Returns raised to power n")
        .def("transforms", [](const gp_Trsf2d& self, double x, double y) {
            Standard_Real tx = x, ty = y;
            self.Transforms(tx, ty);
            return py::make_tuple(tx, ty);
        }, py::arg("x"), py::arg("y"), "Transforms coordinates and returns (x, y)")
        .def("__mul__", &gp_Trsf2d::Multiplied, py::arg("t"))
        .def("__repr__", [](const gp_Trsf2d& self) {
            return "gp_Trsf2d(scale=" + std::to_string(self.ScaleFactor()) + ")";
        })
    ;
    
}