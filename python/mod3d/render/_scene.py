"""Backend-neutral scene decomposition for the renderers.

This module turns an OCCT shape into a small intermediate representation (IR)
that any renderer (pythreejs, PyVista, ...) can consume. It depends only on the
mod3d core bindings — never on a rendering library — so the instancing logic
(tessellate once per shared TShape, place every copy by its location) lives in
one place and both backends stay in sync.

The key trick: shapes that share an underlying TShape (e.g. the rotated copies
of a turbine blade row built by :func:`mod3d.periodic.make_periodic_compound`)
are tessellated a single time, in the TShape-local frame, and each copy's
placement is carried as a 4x4 matrix. The backend then uploads one geometry and
draws it N times under N matrices instead of re-uploading N identical meshes.
"""
from __future__ import annotations

from dataclasses import dataclass
from typing import Any

import numpy as np

from mod3d import Render, TopoDS, TopExp, TopAbs


def shape_occurrences(shape):
    """Renderable sub-shapes of `shape`, located in their parent frame.

    For a Compound (e.g. a periodic blade row) this returns the coarsest
    non-empty level of sub-shapes (solids, else shells, else faces); periodic
    copies share a TShape and differ only by location. Any non-compound shape
    is its own single occurrence.
    """
    if shape.shape_type != TopAbs.COMPOUND:
        return [shape]
    for sub_type in (TopAbs.SOLID, TopAbs.SHELL, TopAbs.FACE):
        occurrences = list(TopExp.Explorer(shape, sub_type))
        if occurrences:
            return occurrences
    return [shape]


def group_partners(occurrences):
    """Group occurrences sharing the same underlying TShape.

    Returns a list of (representative, members). Partners differ only by
    location/orientation, so each group can be tessellated once (from the
    representative) and reused for every member. N is small (sector count),
    so the O(N*groups) partner scan is negligible.
    """
    groups = []
    for occ in occurrences:
        for rep, members in groups:
            if occ.is_partner(rep):
                members.append(occ)
                break
        else:
            groups.append((occ, [occ]))
    return groups


def trsf_to_matrix4(trsf):
    """Convert a ``gp.Trsf`` to a 4x4 row-major numpy matrix.

    ``gp.Trsf`` is a 3x4 affine (rows 1-3, cols 1-4, with an implicit
    ``[0,0,0,1]`` bottom row). Carrying the full matrix (rather than
    decomposing to position/quaternion) keeps any rotation, translation, scale
    or mirror exactly as OCCT stored it. The result uses the conventional
    mathematical layout ``M[row, col]``; backends flatten/transpose as their
    API requires (three.js wants column-major length-16, VTK wants this 4x4).
    """
    v = trsf.value
    return np.array(
        [
            [v(1, 1), v(1, 2), v(1, 3), v(1, 4)],
            [v(2, 1), v(2, 2), v(2, 3), v(2, 4)],
            [v(3, 1), v(3, 2), v(3, 3), v(3, 4)],
            [0.0, 0.0, 0.0, 1.0],
        ],
        dtype=np.float64,
    )


@dataclass
class InstanceGroup:
    """One shared tessellation plus every placement that reuses it.

    ``faces_data`` and ``edges_data`` are exactly the tuples returned by
    :func:`mod3d.Render.extract_tessellation` for the group's representative,
    tessellated in its TShape-local frame. ``matrices`` holds one 4x4 row-major
    matrix per copy; applying it to the local geometry yields world coordinates.
    A non-periodic shape is simply a group with a single identity-ish matrix.
    """

    faces_data: list[Any]
    edges_data: list[Any]
    matrices: list[np.ndarray]


def shape_instance_groups(shape, **extract_kwargs):
    """Decompose `shape` into :class:`InstanceGroup` records.

    Each group corresponds to one unique TShape: its representative is
    tessellated once (location stripped) at the given extraction parameters,
    and every occurrence sharing that TShape contributes its placement matrix.

    `extract_kwargs` are forwarded verbatim to
    :func:`mod3d.Render.extract_tessellation` (e.g. ``linear_deflection``,
    ``angle_deflection``).
    """
    identity = TopoDS.TopLoc_Location()
    groups = []
    for rep, members in group_partners(shape_occurrences(shape)):
        canonical = rep.located(identity)
        faces_data, edges_data = Render.extract_tessellation(canonical, **extract_kwargs)
        matrices = [trsf_to_matrix4(occ.location.transformation) for occ in members]
        groups.append(InstanceGroup(faces_data, edges_data, matrices))
    return groups
