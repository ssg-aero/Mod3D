"""Helpers for rotationally periodic geometry (turbomachinery blade rows etc.).

A periodic reference shape (e.g. a row of identical blades around an axis) can
be built from a single master sector via TopoDS_Shape.located(). All N located
copies share the master's underlying TShape, so tessellating the resulting
compound only meshes the master once — the BVH then sees N copies of the
master's triangulation positioned around the axis.

Combined with mod3d.BRepExtrema.MeshDistance this gives near-free periodic
distance queries: one mesh, N positions.
"""
from __future__ import annotations

from math import pi

from mod3d import TopoDS, gp


def make_periodic_compound(master, axis, n_sectors, angular_step=None):
    """Build a Compound of `n_sectors` rotated copies of `master` around `axis`.

    The copies share the master's TShape via TopLoc_Location, so
    BRepMesh_IncrementalMesh on the returned compound computes the master's
    triangulation only once and the N located faces all reference it.

    Any location already carried by `master` is preserved: each copy's
    rotation is *composed* with the master's own placement (via Moved, which
    multiplies the location) rather than replacing it.

    Parameters
    ----------
    master : TopoDS.Shape
        One sector of the periodic pattern.
    axis : gp.Ax1
        Rotation axis (location + direction).
    n_sectors : int
        Total number of sectors (>= 1).
    angular_step : float, optional
        Rotation angle between consecutive sectors in radians. Defaults to
        2*pi / n_sectors (full periodic ring). Pass a smaller value to build
        a partial ring (e.g. a 3-blade window out of a 24-blade ring).

    Returns
    -------
    TopoDS.Compound
        The periodic compound, ready to feed to MeshDistance.
    """
    if n_sectors < 1:
        raise ValueError("n_sectors must be >= 1")

    step = angular_step if angular_step is not None else (2.0 * pi / n_sectors)

    copies = []
    for k in range(n_sectors):
        trsf = gp.Trsf()
        trsf.set_rotation(axis, k * step)
        # Moved multiplies the existing location by `trsf` (R_k * L0), so a
        # master that already carries a placement is rotated about `axis`
        # rather than having its placement discarded by located().
        copies.append(master.moved(trsf))
    return TopoDS.Compound(copies)
