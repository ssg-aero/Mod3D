"""Tests for BRepGProp bindings (global geometric properties of shapes)."""

import pytest
import math

from mod3d import gp, BRepBuilderAPI, BRepGProp


class TestGProps:
    """Tests for GProp_GProps class."""

    def test_default_constructor(self):
        """Test creating GProps with default reference point (origin)."""
        props = BRepGProp.GProps()
        assert props is not None

    def test_constructor_with_location(self):
        """Test creating GProps with custom reference point."""
        location = gp.Pnt(1.0, 2.0, 3.0)
        props = BRepGProp.GProps(location)
        assert props is not None

    def test_mass_empty(self):
        """Test mass of empty GProps is zero."""
        props = BRepGProp.GProps()
        assert props.mass == 0.0


class TestBRepGPropLinearProperties:
    """Tests for BRepGProp.linear_properties (edge-based computation)."""

    @pytest.mark.skip(reason="occt count edges 2 times")
    def test_linear_properties_box(self):
        """Test computing linear properties of a box's edges."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.linear_properties(box)
        
        # A box has 12 edges, each 10 units long = total length 120
        assert props.mass == pytest.approx(120.0, rel=1e-6)

    def test_linear_properties_cube_center_of_mass(self):
        """Test center of mass from linear properties."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.linear_properties(box)
        
        # Center of mass of edges should be at (5, 5, 5)
        com = props.centre_of_mass
        assert com.x == pytest.approx(5.0, rel=1e-6)
        assert com.y == pytest.approx(5.0, rel=1e-6)
        assert com.z == pytest.approx(5.0, rel=1e-6)


class TestBRepGPropSurfaceProperties:
    """Tests for BRepGProp.surface_properties (face-based computation)."""

    def test_surface_properties_box(self):
        """Test computing surface area of a box."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.surface_properties(box)
        
        # A 10x10x10 box has surface area = 6 * 100 = 600
        assert props.mass == pytest.approx(600.0, rel=1e-6)

    def test_surface_properties_box_center_of_mass(self):
        """Test center of mass from surface properties."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.surface_properties(box)
        
        # Center of mass should be at (5, 5, 5)
        com = props.centre_of_mass
        assert com.x == pytest.approx(5.0, rel=1e-6)
        assert com.y == pytest.approx(5.0, rel=1e-6)
        assert com.z == pytest.approx(5.0, rel=1e-6)

    def test_surface_properties_sphere(self):
        """Test computing surface area of a sphere."""
        sphere = BRepBuilderAPI.MakeSphere(5.0).shape()
        props = BRepGProp.BRepGProp.surface_properties(sphere)
        
        # Surface area of sphere = 4 * pi * r^2 = 4 * pi * 25 ≈ 314.159
        expected_area = 4 * math.pi * 25.0
        assert props.mass == pytest.approx(expected_area, rel=1e-4)

    def test_surface_properties_with_precision(self):
        """Test surface properties with specified precision."""
        sphere = BRepBuilderAPI.MakeSphere(5.0).shape()
        props, error = BRepGProp.BRepGProp.surface_properties_eps(sphere, 0.001)
        
        expected_area = 4 * math.pi * 25.0
        assert props.mass == pytest.approx(expected_area, rel=1e-3)
        assert error >= 0  # Error should be non-negative


class TestBRepGPropVolumeProperties:
    """Tests for BRepGProp.volume_properties (solid-based computation)."""

    def test_volume_properties_box(self):
        """Test computing volume of a box."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.volume_properties(box)
        
        # Volume = 10 * 10 * 10 = 1000
        assert props.mass == pytest.approx(1000.0, rel=1e-6)

    def test_volume_properties_box_center_of_mass(self):
        """Test center of mass from volume properties."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.volume_properties(box)
        
        # Center of mass should be at (5, 5, 5)
        com = props.centre_of_mass
        assert com.x == pytest.approx(5.0, rel=1e-6)
        assert com.y == pytest.approx(5.0, rel=1e-6)
        assert com.z == pytest.approx(5.0, rel=1e-6)

    def test_volume_properties_sphere(self):
        """Test computing volume of a sphere."""
        radius = 5.0
        sphere = BRepBuilderAPI.MakeSphere(radius).shape()
        props = BRepGProp.BRepGProp.volume_properties(sphere)
        
        # Volume of sphere = (4/3) * pi * r^3
        expected_volume = (4.0 / 3.0) * math.pi * (radius ** 3)
        assert props.mass == pytest.approx(expected_volume, rel=1e-3)

    def test_volume_properties_cylinder(self):
        """Test computing volume of a cylinder."""
        radius = 2.0
        height = 10.0
        cylinder = BRepBuilderAPI.MakeCylinder(radius, height).shape()
        props = BRepGProp.BRepGProp.volume_properties(cylinder)
        
        # Volume = pi * r^2 * h
        expected_volume = math.pi * (radius ** 2) * height
        assert props.mass == pytest.approx(expected_volume, rel=1e-4)

    def test_volume_properties_with_precision(self):
        """Test volume properties with specified precision."""
        sphere = BRepBuilderAPI.MakeSphere(5.0).shape()
        props, error = BRepGProp.BRepGProp.volume_properties_eps(sphere, 0.001)
        
        expected_volume = (4.0 / 3.0) * math.pi * 125.0
        assert props.mass == pytest.approx(expected_volume, rel=1e-3)
        assert error >= 0  # Error should be non-negative

    def test_volume_properties_gk(self):
        """Test volume properties using Gauss-Kronrod integration."""
        sphere = BRepBuilderAPI.MakeSphere(5.0).shape()
        props, error = BRepGProp.BRepGProp.volume_properties_gk(sphere)
        
        expected_volume = (4.0 / 3.0) * math.pi * 125.0
        assert props.mass == pytest.approx(expected_volume, rel=1e-3)
        assert error >= 0


class TestGPropsInertia:
    """Tests for inertia-related properties."""

    def test_matrix_of_inertia_box(self):
        """Test matrix of inertia for a box."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.volume_properties(box)
        
        matrix = props.matrix_of_inertia
        assert matrix is not None
        
        # For a unit density cube centered at origin, the diagonal elements should be equal
        # Ixx = Iyy = Izz for a cube
        # Note: The matrix is computed about the center of mass

    def test_static_moments(self):
        """Test static moments computation."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.volume_properties(box)
        
        ix, iy, iz = props.static_moments()
        # Static moments should all be equal for a symmetric cube
        assert ix > 0
        assert iy > 0
        assert iz > 0

    def test_moment_of_inertia_about_axis(self):
        """Test moment of inertia about a specific axis."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.volume_properties(box)
        
        # Axis through center of mass, along Z
        axis = gp.Ax1(gp.Pnt(5.0, 5.0, 5.0), gp.Dir(0.0, 0.0, 1.0))
        moment = props.moment_of_inertia(axis)
        assert moment > 0

    def test_radius_of_gyration(self):
        """Test radius of gyration computation."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.volume_properties(box)
        
        axis = gp.Ax1(gp.Pnt(5.0, 5.0, 5.0), gp.Dir(0.0, 0.0, 1.0))
        radius = props.radius_of_gyration(axis)
        assert radius > 0


class TestPrincipalProperties:
    """Tests for principal properties of inertia."""

    def test_principal_properties_cube(self):
        """Test principal properties for a cube (symmetric)."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.volume_properties(box)
        
        principal = props.principal_properties
        assert principal is not None

    def test_principal_moments_cube(self):
        """Test principal moments for a cube."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.volume_properties(box)
        
        principal = props.principal_properties
        ixx, iyy, izz = principal.moments()
        
        # For a cube, all principal moments should be equal
        assert ixx == pytest.approx(iyy, rel=1e-6)
        assert iyy == pytest.approx(izz, rel=1e-6)

    def test_principal_radii_of_gyration(self):
        """Test principal radii of gyration."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.volume_properties(box)
        
        principal = props.principal_properties
        rxx, ryy, rzz = principal.radii_of_gyration()
        
        # For a cube, all radii should be equal
        assert rxx == pytest.approx(ryy, rel=1e-6)
        assert ryy == pytest.approx(rzz, rel=1e-6)

    def test_principal_axes_of_inertia(self):
        """Test principal axes of inertia."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        props = BRepGProp.BRepGProp.volume_properties(box)
        
        principal = props.principal_properties
        
        axis1 = principal.first_axis_of_inertia()
        axis2 = principal.second_axis_of_inertia()
        axis3 = principal.third_axis_of_inertia()
        
        assert axis1 is not None
        assert axis2 is not None
        assert axis3 is not None


class TestGPropsAdd:
    """Tests for combining global properties."""

    def test_add_two_boxes(self):
        """Test adding properties of two boxes."""
        box1 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        box2 = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        
        props1 = BRepGProp.BRepGProp.volume_properties(box1)
        props2 = BRepGProp.BRepGProp.volume_properties(box2)
        
        # Create combined props
        combined = BRepGProp.GProps()
        combined.add(props1)
        combined.add(props2)
        
        # Total volume should be 2000
        assert combined.mass == pytest.approx(2000.0, rel=1e-6)

    def test_add_with_density(self):
        """Test adding properties with different densities."""
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        
        props = BRepGProp.BRepGProp.volume_properties(box)
        
        # Create combined props with density 2.0
        combined = BRepGProp.GProps()
        combined.add(props, 2.0)
        
        # Mass should be volume * density = 1000 * 2 = 2000
        assert combined.mass == pytest.approx(2000.0, rel=1e-6)
