import pytest

from mod3d import gp

def test_gp_xyz_operations():
    v1 = gp.XYZ(1.0, 2.0, 3.0)
    v2 = gp.XYZ(4.0, 5.0, 6.0)

    # Test addition
    v3 = v1 + v2
    assert (v3.x, v3.y, v3.z) == (5.0, 7.0, 9.0)

    # Test subtraction
    v4 = v2 - v1
    assert (v4.x, v4.y, v4.z) == (3.0, 3.0, 3.0)

    # Test scalar multiplication
    v5 = v1 * 2.0
    assert (v5.x, v5.y, v5.z) == (2.0, 4.0, 6.0)

    # Test scalar division
    v6 = v2 / 2.0
    assert (v6.x, v6.y, v6.z) == (2.0, 2.5, 3.0)

    # Test cross product
    v7 = v1 ^ v2
    assert (v7.x, v7.y, v7.z) == (-3.0, 6.0, -3.0)

def test_gp_xyz_distance():
    v1 = gp.XYZ(1.0, 2.0, 3.0)
    v2 = gp.XYZ(4.0, 6.0, 8.0)

    dist = v1.distance(v2)
    assert pytest.approx(dist) == 7.0710678118654755  # sqrt(3^2 + 4^2 + 5^2)

def test_gp_xyz_normalize():
    v = gp.XYZ(3.0, 0.0, 4.0)
    v_normalized = v.normalized()
    length = (v_normalized.x**2 + v_normalized.y**2 + v_normalized.z**2) ** 0.5
    assert pytest.approx(length) == 1.0

def test_gp_pnt():
    p = gp.Pnt(1.0, 2.0, 3.0)
    assert (p.x, p.y, p.z) == (1.0, 2.0, 3.0)

def test_gp_pnt_construction():
    # Default constructor
    p1 = gp.Pnt()
    assert (p1.x, p1.y, p1.z) == (0.0, 0.0, 0.0)
    
    # From coordinates
    p2 = gp.Pnt(1.0, 2.0, 3.0)
    assert (p2.x, p2.y, p2.z) == (1.0, 2.0, 3.0)
    
    # From XYZ
    xyz = gp.XYZ(4.0, 5.0, 6.0)
    p3 = gp.Pnt(xyz)
    assert (p3.x, p3.y, p3.z) == (4.0, 5.0, 6.0)

def test_gp_pnt_setters():
    p = gp.Pnt()
    
    p.set_coord(1.0, 2.0, 3.0)
    assert (p.x, p.y, p.z) == (1.0, 2.0, 3.0)
    
    p.set_coord(1, 5.0)  # Set X
    assert p.x == 5.0
    
    p.set_coord(2, 6.0)  # Set Y
    assert p.y == 6.0
    
    p.set_coord(3, 7.0)  # Set Z
    assert p.z == 7.0

def test_gp_pnt_xyz():
    p = gp.Pnt(1.0, 2.0, 3.0)
    xyz = p.xyz
    assert (xyz.x, xyz.y, xyz.z) == (1.0, 2.0, 3.0)
    
    new_xyz = gp.XYZ(4.0, 5.0, 6.0)
    p.xyz = new_xyz
    assert (p.x, p.y, p.z) == (4.0, 5.0, 6.0)

def test_gp_pnt_coord():
    p = gp.Pnt(10.0, 20.0, 30.0)
    
    # Get individual coordinate by index
    assert p.coord(1) == 10.0
    assert p.coord(2) == 20.0
    assert p.coord(3) == 30.0
    
    # Get all coordinates as tuple
    coords = p.coord()
    assert coords == (10.0, 20.0, 30.0)

def test_gp_pnt_distance():
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(3.0, 4.0, 0.0)
    
    # Distance should be 5.0 (3-4-5 triangle)
    dist = p1.distance(p2)
    assert pytest.approx(dist) == 5.0
    
    # Square distance
    sq_dist = p1.square_distance(p2)
    assert pytest.approx(sq_dist) == 25.0

def test_gp_pnt_is_equal():
    p1 = gp.Pnt(1.0, 2.0, 3.0)
    p2 = gp.Pnt(1.0001, 2.0001, 3.0001)
    
    # Should be equal within tolerance
    assert p1.is_equal(p2, 0.001)
    
    # Should not be equal with smaller tolerance
    assert not p1.is_equal(p2, 0.00001)

def test_gp_pnt_bary_center():
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(10.0, 10.0, 10.0)
    
    # Create point at midpoint (alpha=1, beta=1)
    p1.bary_center(1.0, p2, 1.0)
    assert (p1.x, p1.y, p1.z) == (5.0, 5.0, 5.0)

def test_gp_pnt_scale():
    p1 = gp.Pnt(10.0, 10.0, 10.0)
    center = gp.Pnt(0.0, 0.0, 0.0)
    
    # Scale by 2 around origin
    p2 = p1.scaled(center, 2.0)
    assert (p2.x, p2.y, p2.z) == (20.0, 20.0, 20.0)
    
    # Original should be unchanged
    assert (p1.x, p1.y, p1.z) == (10.0, 10.0, 10.0)
    
    # In-place scale
    p1.scale(center, 0.5)
    assert (p1.x, p1.y, p1.z) == (5.0, 5.0, 5.0)

def test_gp_pnt_mirror_point():
    p1 = gp.Pnt(10.0, 0.0, 0.0)
    mirror_center = gp.Pnt(5.0, 0.0, 0.0)
    
    # Mirror around center point
    p2 = p1.mirrored(mirror_center)
    assert (p2.x, p2.y, p2.z) == (0.0, 0.0, 0.0)
    
    # Original should be unchanged
    assert (p1.x, p1.y, p1.z) == (10.0, 0.0, 0.0)

def test_gp_pnt_repr():
    p = gp.Pnt(1.5, 2.5, 3.5)
    repr_str = repr(p)
    assert "gp_Pnt" in repr_str
    assert "1.5" in repr_str or "1.50" in repr_str
    assert "2.5" in repr_str or "2.50" in repr_str
    assert "3.5" in repr_str or "3.50" in repr_str

def test_gp_ax1_construction():
    # Default constructor (Z axis at origin)
    ax1 = gp.Ax1()
    assert (ax1.location.x, ax1.location.y, ax1.location.z) == (0.0, 0.0, 0.0)
    assert (ax1.direction.x, ax1.direction.y, ax1.direction.z) == (0.0, 0.0, 1.0)
    
    # Constructor with point and direction
    p = gp.Pnt(1.0, 2.0, 3.0)
    d = gp.Dir(0.0, 0.0, 1.0)
    ax2 = gp.Ax1(p, d)
    assert (ax2.location.x, ax2.location.y, ax2.location.z) == (1.0, 2.0, 3.0)
    assert (ax2.direction.x, ax2.direction.y, ax2.direction.z) == (0.0, 0.0, 1.0)

def test_gp_ax1_setters():
    ax = gp.Ax1()
    
    # Set location
    new_loc = gp.Pnt(5.0, 6.0, 7.0)
    ax.location = new_loc
    assert (ax.location.x, ax.location.y, ax.location.z) == (5.0, 6.0, 7.0)
    
    # Set direction
    new_dir = gp.Dir(1.0, 0.0, 0.0)
    ax.direction = new_dir
    assert (ax.direction.x, ax.direction.y, ax.direction.z) == (1.0, 0.0, 0.0)

def test_gp_ax1_comparison():
    import math
    
    ax1 = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    ax2 = gp.Ax1(gp.Pnt(0.0, 0.0, 5.0), gp.Dir(0.0, 0.0, 1.0))
    ax3 = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(1.0, 0.0, 0.0))
    
    # Test is_coaxial (same direction and on same line)
    assert ax1.is_coaxial(ax2, 0.01, 0.01)
    
    # Test is_parallel (same direction, may not be on same line)
    assert ax1.is_parallel(ax2, 0.01)
    
    # Test is_normal (perpendicular)
    assert ax1.is_normal(ax3, 0.01)
    
    # Test angle
    angle = ax1.angle(ax3)
    assert pytest.approx(angle) == math.pi / 2.0

def test_gp_ax1_reverse():
    ax1 = gp.Ax1(gp.Pnt(1.0, 2.0, 3.0), gp.Dir(0.0, 0.0, 1.0))
    
    # Test reversed (returns new)
    ax2 = ax1.reversed()
    assert (ax2.location.x, ax2.location.y, ax2.location.z) == (1.0, 2.0, 3.0)
    assert (ax2.direction.x, ax2.direction.y, ax2.direction.z) == (0.0, 0.0, -1.0)
    # Original unchanged
    assert (ax1.direction.x, ax1.direction.y, ax1.direction.z) == (0.0, 0.0, 1.0)
    
    # Test reverse (in-place)
    ax1.reverse()
    assert (ax1.direction.x, ax1.direction.y, ax1.direction.z) == (0.0, 0.0, -1.0)

def test_gp_ax1_mirror():
    ax1 = gp.Ax1(gp.Pnt(10.0, 0.0, 0.0), gp.Dir(1.0, 0.0, 0.0))
    mirror_pt = gp.Pnt(5.0, 0.0, 0.0)
    
    # Test mirrored (returns new)
    ax2 = ax1.mirrored(mirror_pt)
    assert (ax2.location.x, ax2.location.y, ax2.location.z) == (0.0, 0.0, 0.0)
    # Original unchanged
    assert (ax1.location.x, ax1.location.y, ax1.location.z) == (10.0, 0.0, 0.0)

def test_gp_ax1_rotate():
    import math
    
    ax1 = gp.Ax1(gp.Pnt(1.0, 0.0, 0.0), gp.Dir(1.0, 0.0, 0.0))
    z_axis = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    
    # Rotate 90 degrees around Z axis
    ax2 = ax1.rotated(z_axis, math.pi / 2.0)
    
    # Point should be at (0, 1, 0)
    assert pytest.approx(ax2.location.x) == 0.0
    assert pytest.approx(ax2.location.y) == 1.0
    assert pytest.approx(ax2.location.z) == 0.0
    
    # Direction should be (0, 1, 0)
    assert pytest.approx(ax2.direction.x) == 0.0
    assert pytest.approx(ax2.direction.y) == 1.0
    assert pytest.approx(ax2.direction.z) == 0.0

def test_gp_ax1_scale():
    ax1 = gp.Ax1(gp.Pnt(10.0, 0.0, 0.0), gp.Dir(1.0, 0.0, 0.0))
    center = gp.Pnt(0.0, 0.0, 0.0)
    
    # Scale by 2
    ax2 = ax1.scaled(center, 2.0)
    assert (ax2.location.x, ax2.location.y, ax2.location.z) == (20.0, 0.0, 0.0)
    # Direction unchanged
    assert (ax2.direction.x, ax2.direction.y, ax2.direction.z) == (1.0, 0.0, 0.0)
    
    # Scale by -1 (reverses direction)
    ax3 = ax1.scaled(center, -1.0)
    assert (ax3.location.x, ax3.location.y, ax3.location.z) == (-10.0, 0.0, 0.0)
    assert (ax3.direction.x, ax3.direction.y, ax3.direction.z) == (-1.0, 0.0, 0.0)

def test_gp_ax1_translate():
    ax1 = gp.Ax1(gp.Pnt(1.0, 2.0, 3.0), gp.Dir(0.0, 0.0, 1.0))
    
    # Translate by vector
    v = gp.Vec(10.0, 20.0, 30.0)
    ax2 = ax1.translated(v)
    assert (ax2.location.x, ax2.location.y, ax2.location.z) == (11.0, 22.0, 33.0)
    # Direction unchanged
    assert (ax2.direction.x, ax2.direction.y, ax2.direction.z) == (0.0, 0.0, 1.0)
    
    # Translate from point to point
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 5.0, 5.0)
    ax3 = ax1.translated(p1, p2)
    assert (ax3.location.x, ax3.location.y, ax3.location.z) == (6.0, 7.0, 8.0)

def test_gp_ax1_repr():
    ax = gp.Ax1(gp.Pnt(1.0, 2.0, 3.0), gp.Dir(0.0, 0.0, 1.0))
    repr_str = repr(ax)
    assert "gp_Ax1" in repr_str
    assert "loc" in repr_str
    assert "dir" in repr_str

def test_gp_ax2_construction():
    # Default constructor (XY plane at origin)
    ax2 = gp.Ax2()
    assert (ax2.location.x, ax2.location.y, ax2.location.z) == (0.0, 0.0, 0.0)
    assert (ax2.direction.x, ax2.direction.y, ax2.direction.z) == (0.0, 0.0, 1.0)
    assert (ax2.x_direction.x, ax2.x_direction.y, ax2.x_direction.z) == (1.0, 0.0, 0.0)
    assert (ax2.y_direction.x, ax2.y_direction.y, ax2.y_direction.z) == (0.0, 1.0, 0.0)
    
    # Constructor with point and main direction
    p = gp.Pnt(1.0, 2.0, 3.0)
    d = gp.Dir(0.0, 0.0, 1.0)
    ax3 = gp.Ax2(p, d)
    assert (ax3.location.x, ax3.location.y, ax3.location.z) == (1.0, 2.0, 3.0)
    assert (ax3.direction.x, ax3.direction.y, ax3.direction.z) == (0.0, 0.0, 1.0)
    
    # Constructor with point, normal, and X direction
    vx = gp.Dir(1.0, 0.0, 0.0)
    ax4 = gp.Ax2(p, d, vx)
    assert (ax4.location.x, ax4.location.y, ax4.location.z) == (1.0, 2.0, 3.0)
    assert (ax4.direction.x, ax4.direction.y, ax4.direction.z) == (0.0, 0.0, 1.0)
    assert (ax4.x_direction.x, ax4.x_direction.y, ax4.x_direction.z) == (1.0, 0.0, 0.0)

def test_gp_ax2_setters():
    ax = gp.Ax2()
    
    # Set location
    new_loc = gp.Pnt(5.0, 6.0, 7.0)
    ax.location = new_loc
    assert (ax.location.x, ax.location.y, ax.location.z) == (5.0, 6.0, 7.0)
    
    # Set X direction
    new_xdir = gp.Dir(0.0, 1.0, 0.0)
    ax.x_direction = new_xdir
    assert pytest.approx(ax.x_direction.x) == 0.0
    assert pytest.approx(ax.x_direction.y) == 1.0
    assert pytest.approx(ax.x_direction.z) == 0.0
    
    # Set Y direction
    new_ydir = gp.Dir(-1.0, 0.0, 0.0)
    ax.y_direction = new_ydir
    assert pytest.approx(ax.y_direction.x) == -1.0
    assert pytest.approx(ax.y_direction.y) == 0.0
    assert pytest.approx(ax.y_direction.z) == 0.0

def test_gp_ax2_axis():
    ax2 = gp.Ax2(gp.Pnt(1.0, 2.0, 3.0), gp.Dir(0.0, 0.0, 1.0))
    
    # Get the main axis (Ax1)
    ax1 = ax2.axis
    assert (ax1.location.x, ax1.location.y, ax1.location.z) == (1.0, 2.0, 3.0)
    assert (ax1.direction.x, ax1.direction.y, ax1.direction.z) == (0.0, 0.0, 1.0)

def test_gp_ax2_angle():
    import math
    
    ax1 = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    ax2 = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(1.0, 0.0, 0.0))
    
    # Angle between Z and X axes should be 90 degrees
    angle = ax1.angle(ax2)
    assert pytest.approx(angle) == math.pi / 2.0

def test_gp_ax2_is_coplanar():
    ax1 = gp.Ax2(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    ax2 = gp.Ax2(gp.Pnt(1.0, 1.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    ax3 = gp.Ax2(gp.Pnt(0.0, 0.0, 5.0), gp.Dir(0.0, 0.0, 1.0))
    
    # ax1 and ax2 are coplanar (both in XY plane at z=0)
    assert ax1.is_coplanar(ax2, 0.01, 0.01)
    
    # ax1 and ax3 are not coplanar (different z values)
    assert not ax1.is_coplanar(ax3, 0.01, 0.01)

def test_gp_ax2_mirror():
    ax1 = gp.Ax2(gp.Pnt(10.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0), gp.Dir(1.0, 0.0, 0.0))
    mirror_pt = gp.Pnt(5.0, 0.0, 0.0)
    
    # Test mirrored (returns new)
    ax2 = ax1.mirrored(mirror_pt)
    assert (ax2.location.x, ax2.location.y, ax2.location.z) == (0.0, 0.0, 0.0)
    # Original unchanged
    assert (ax1.location.x, ax1.location.y, ax1.location.z) == (10.0, 0.0, 0.0)

def test_gp_ax2_rotate():
    import math
    
    ax1 = gp.Ax2(gp.Pnt(1.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0), gp.Dir(1.0, 0.0, 0.0))
    z_axis = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    
    # Rotate 90 degrees around Z axis
    ax2 = ax1.rotated(z_axis, math.pi / 2.0)
    
    # Location should be at (0, 1, 0)
    assert pytest.approx(ax2.location.x) == 0.0
    assert pytest.approx(ax2.location.y) == 1.0
    assert pytest.approx(ax2.location.z) == 0.0
    
    # X direction should rotate to (0, 1, 0)
    assert pytest.approx(ax2.x_direction.x) == 0.0
    assert pytest.approx(ax2.x_direction.y) == 1.0
    assert pytest.approx(ax2.x_direction.z) == 0.0

def test_gp_ax2_scale():
    ax1 = gp.Ax2(gp.Pnt(10.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0), gp.Dir(1.0, 0.0, 0.0))
    center = gp.Pnt(0.0, 0.0, 0.0)
    
    # Scale by 2
    ax2 = ax1.scaled(center, 2.0)
    assert (ax2.location.x, ax2.location.y, ax2.location.z) == (20.0, 0.0, 0.0)
    # Directions unchanged
    assert (ax2.direction.x, ax2.direction.y, ax2.direction.z) == (0.0, 0.0, 1.0)
    assert (ax2.x_direction.x, ax2.x_direction.y, ax2.x_direction.z) == (1.0, 0.0, 0.0)
    
    # Scale by -1 (reverses X and Y directions)
    ax3 = ax1.scaled(center, -1.0)
    assert (ax3.location.x, ax3.location.y, ax3.location.z) == (-10.0, 0.0, 0.0)
    assert (ax3.x_direction.x, ax3.x_direction.y, ax3.x_direction.z) == (-1.0, 0.0, 0.0)
    assert (ax3.y_direction.x, ax3.y_direction.y, ax3.y_direction.z) == (0.0, -1.0, 0.0)

def test_gp_ax2_translate():
    ax1 = gp.Ax2(gp.Pnt(1.0, 2.0, 3.0), gp.Dir(0.0, 0.0, 1.0))
    
    # Translate by vector
    v = gp.Vec(10.0, 20.0, 30.0)
    ax2 = ax1.translated(v)
    assert (ax2.location.x, ax2.location.y, ax2.location.z) == (11.0, 22.0, 33.0)
    # Directions unchanged
    assert (ax2.direction.x, ax2.direction.y, ax2.direction.z) == (0.0, 0.0, 1.0)
    
    # Translate from point to point
    p1 = gp.Pnt(0.0, 0.0, 0.0)
    p2 = gp.Pnt(5.0, 5.0, 5.0)
    ax3 = ax1.translated(p1, p2)
    assert (ax3.location.x, ax3.location.y, ax3.location.z) == (6.0, 7.0, 8.0)

def test_gp_ax2_repr():
    ax = gp.Ax2(gp.Pnt(1.0, 2.0, 3.0), gp.Dir(0.0, 0.0, 1.0))
    repr_str = repr(ax)
    assert "gp_Ax2" in repr_str
    assert "loc" in repr_str
    assert "dir" in repr_str
    assert "xdir" in repr_str


def test_gp_pln_construction():
    # Default constructor (XY plane at origin)
    pln1 = gp.Pln()
    assert (pln1.location.x, pln1.location.y, pln1.location.z) == (0.0, 0.0, 0.0)
    assert (pln1.axis.direction.x, pln1.axis.direction.y, pln1.axis.direction.z) == (0.0, 0.0, 1.0)
    
    # Constructor from point and normal direction
    p = gp.Pnt(1.0, 2.0, 3.0)
    n = gp.Dir(0.0, 0.0, 1.0)
    pln2 = gp.Pln(p, n)
    assert (pln2.location.x, pln2.location.y, pln2.location.z) == (1.0, 2.0, 3.0)
    assert (pln2.axis.direction.x, pln2.axis.direction.y, pln2.axis.direction.z) == (0.0, 0.0, 1.0)
    
    # Constructor from plane equation coefficients ax + by + cz + d = 0
    # For z = 5, equation is 0x + 0y + 1z - 5 = 0
    pln3 = gp.Pln(0.0, 0.0, 1.0, -5.0)
    assert pytest.approx(pln3.location.z) == 5.0
    assert (pln3.axis.direction.x, pln3.axis.direction.y, pln3.axis.direction.z) == (0.0, 0.0, 1.0)

def test_gp_pln_setters():
    pln = gp.Pln()
    
    # Set location
    new_loc = gp.Pnt(5.0, 6.0, 7.0)
    pln.location = new_loc
    assert (pln.location.x, pln.location.y, pln.location.z) == (5.0, 6.0, 7.0)
    
    # Set axis
    new_axis = gp.Ax1(gp.Pnt(1.0, 2.0, 3.0), gp.Dir(1.0, 0.0, 0.0))
    pln.axis = new_axis
    assert (pln.axis.direction.x, pln.axis.direction.y, pln.axis.direction.z) == (1.0, 0.0, 0.0)

def test_gp_pln_coefficients():
    # XY plane at z=0: 0x + 0y + 1z + 0 = 0
    pln1 = gp.Pln()
    a, b, c, d = pln1.coefficients()
    assert pytest.approx(a) == 0.0
    assert pytest.approx(b) == 0.0
    assert pytest.approx(c) == 1.0
    assert pytest.approx(d) == 0.0
    
    # Plane at z=5: 0x + 0y + 1z - 5 = 0
    pln2 = gp.Pln(0.0, 0.0, 1.0, -5.0)
    a, b, c, d = pln2.coefficients()
    assert pytest.approx(a) == 0.0
    assert pytest.approx(b) == 0.0
    assert pytest.approx(c) == 1.0
    assert pytest.approx(d) == -5.0

def test_gp_pln_distance():
    # XY plane at origin
    pln = gp.Pln()
    
    # Distance from point
    p1 = gp.Pnt(0.0, 0.0, 5.0)
    dist = pln.distance(p1)
    assert pytest.approx(dist) == 5.0
    
    # Square distance
    sq_dist = pln.square_distance(p1)
    assert pytest.approx(sq_dist) == 25.0
    
    # Point on the plane
    p2 = gp.Pnt(10.0, 20.0, 0.0)
    assert pytest.approx(pln.distance(p2)) == 0.0

def test_gp_pln_contains():
    pln = gp.Pln()
    
    # Point on the plane
    p1 = gp.Pnt(5.0, 5.0, 0.0)
    assert pln.contains(p1, 0.001)
    
    # Point slightly off the plane (within tolerance)
    p2 = gp.Pnt(5.0, 5.0, 0.0001)
    assert pln.contains(p2, 0.001)
    
    # Point far from the plane
    p3 = gp.Pnt(5.0, 5.0, 5.0)
    assert not pln.contains(p3, 0.001)

def test_gp_pln_mirror():
    pln1 = gp.Pln(gp.Pnt(0.0, 0.0, 5.0), gp.Dir(0.0, 0.0, 1.0))
    mirror_pt = gp.Pnt(0.0, 0.0, 0.0)
    
    # Test mirrored (returns new)
    pln2 = pln1.mirrored(mirror_pt)
    assert pytest.approx(pln2.location.z) == -5.0
    # Original unchanged
    assert pytest.approx(pln1.location.z) == 5.0

def test_gp_pln_translate():
    pln1 = gp.Pln(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    
    # Translate by vector
    v = gp.Vec(10.0, 20.0, 30.0)
    pln2 = pln1.translated(v)
    assert (pln2.location.x, pln2.location.y, pln2.location.z) == (10.0, 20.0, 30.0)
    # Normal direction unchanged
    assert (pln2.axis.direction.x, pln2.axis.direction.y, pln2.axis.direction.z) == (0.0, 0.0, 1.0)

def test_gp_pln_rotate():
    import math
    
    pln1 = gp.Pln(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))
    x_axis = gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(1.0, 0.0, 0.0))
    
    # Rotate 90 degrees around X axis
    pln2 = pln1.rotated(x_axis, math.pi / 2.0)
    
    # Normal should now point in -Y direction
    assert pytest.approx(pln2.axis.direction.x) == 0.0
    assert pytest.approx(pln2.axis.direction.y) == -1.0
    assert pytest.approx(pln2.axis.direction.z) == 0.0

def test_gp_pln_repr():
    pln = gp.Pln(gp.Pnt(1.0, 2.0, 3.0), gp.Dir(0.0, 0.0, 1.0))
    repr_str = repr(pln)
    assert "gp_Pln" in repr_str
    assert "loc" in repr_str
    assert "normal" in repr_str

