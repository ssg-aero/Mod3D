"""
BRep library module
"""
from __future__ import annotations
import mod3d.BRepTools
import mod3d.Geom
import mod3d.GeomAbs
import mod3d.TopoDS
import typing
__all__: list[str] = ['build_curve_3d', 'build_curves_3d', 'build_curves_3d_with_defaults', 'build_pcurve_for_edge_on_plane', 'check_same_range', 'continuity_of_faces', 'encode_regularity', 'encode_regularity_edge', 'ensure_normal_consistency', 'extend_face', 'find_valid_range', 'orient_closed_solid', 'plane', 'precision', 'reverse_sort_faces', 'same_parameter', 'same_parameter_reshape', 'same_parameter_shape', 'same_range', 'set_plane', 'set_precision', 'sort_faces', 'update_deflection', 'update_edge_tol', 'update_edge_tolerance', 'update_inner_tolerances', 'update_tolerances', 'update_tolerances_reshape']
def build_curve_3d(edge: mod3d.TopoDS.Edge, tolerance: typing.SupportsFloat = 1e-05, continuity: mod3d.GeomAbs.Shape = ..., max_degree: typing.SupportsInt = 14, max_segment: typing.SupportsInt = 0) -> bool:
    """
    Computes the 3d curve for the edge if it does not exist.
    """
def build_curves_3d(shape: mod3d.TopoDS.Shape, tolerance: typing.SupportsFloat, continuity: mod3d.GeomAbs.Shape = ..., max_degree: typing.SupportsInt = 14, max_segment: typing.SupportsInt = 0) -> bool:
    """
    Computes 3d curves for all edges of the shape with tolerance.
    """
def build_curves_3d_with_defaults(shape: mod3d.TopoDS.Shape) -> bool:
    """
    Computes 3d curves for all edges of the shape with default parameters.
    """
def build_pcurve_for_edge_on_plane(edge: mod3d.TopoDS.Edge, face: mod3d.TopoDS.Face) -> None:
    """
    Builds pcurve of edge on face if the surface is plane, and updates the edge.
    """
def check_same_range(edge: mod3d.TopoDS.Edge, confusion: typing.SupportsFloat = 1e-12) -> bool:
    """
    Checks if the Edge is same range ignoring the same range flag.
    """
def continuity_of_faces(edge: mod3d.TopoDS.Edge, face1: mod3d.TopoDS.Face, face2: mod3d.TopoDS.Face, angle_tol: typing.SupportsFloat) -> mod3d.GeomAbs.Shape:
    """
    Returns the order of continuity between two faces connected by an edge.
    """
def encode_regularity(shape: mod3d.TopoDS.Shape, tol_ang: typing.SupportsFloat = 1e-10) -> None:
    """
    Encodes the Regularity of edges on a Shape.
    """
def encode_regularity_edge(edge: mod3d.TopoDS.Edge, face1: mod3d.TopoDS.Face, face2: mod3d.TopoDS.Face, tol_ang: typing.SupportsFloat = 1e-10) -> None:
    """
    Encodes the Regularity between face1 and face2 by edge.
    """
def ensure_normal_consistency(shape: mod3d.TopoDS.Shape, ang_tol: typing.SupportsFloat = 0.001, force_compute_normals: bool = False) -> bool:
    """
    Corrects normals in Poly_Triangulation of faces along smooth edges.
    """
def extend_face(face: mod3d.TopoDS.Face, ext_val: typing.SupportsFloat, ext_u_min: bool, ext_u_max: bool, ext_v_min: bool, ext_v_max: bool) -> mod3d.TopoDS.Face:
    """
    Enlarges the face on the given value in specified directions.
    """
def find_valid_range(edge: mod3d.TopoDS.Edge, first: typing.SupportsFloat, last: typing.SupportsFloat) -> bool:
    """
    Finds a range of 3d curve of the edge not covered by vertices tolerances.
    """
def orient_closed_solid(solid: mod3d.TopoDS.Solid) -> bool:
    """
    Orients the solid forward and shell to have matter in the solid.
    """
def plane() -> mod3d.Geom.Plane:
    """
    Returns the current plane.
    """
def precision() -> float:
    """
    Returns the default precision.
    """
def reverse_sort_faces(shape: mod3d.TopoDS.Shape) -> list[mod3d.TopoDS.Face]:
    """
    Sorts faces by reverse surface complexity.
    """
def same_parameter(edge: mod3d.TopoDS.Edge, tolerance: typing.SupportsFloat = 1e-05) -> None:
    """
    Computes new 2d curve(s) for the edge to have the same parameter as the 3d curve.
    """
def same_parameter_reshape(shape: mod3d.TopoDS.Shape, reshaper: mod3d.BRepTools.ReShape, tolerance: typing.SupportsFloat = 1e-05, forced: bool = False) -> None:
    """
    Computes new 2d curve(s) for all edges, recording modifications in reshaper.
    """
def same_parameter_shape(shape: mod3d.TopoDS.Shape, tolerance: typing.SupportsFloat = 1e-05, forced: bool = False) -> None:
    """
    Computes new 2d curve(s) for all edges of the shape.
    """
def same_range(edge: mod3d.TopoDS.Edge, tolerance: typing.SupportsFloat = 1e-05) -> None:
    """
    Makes all curve representations have the same range domain.
    """
def set_plane(p: mod3d.Geom.Plane) -> None:
    """
    Sets the current plane.
    """
def set_precision(p: typing.SupportsFloat) -> None:
    """
    Sets the default precision.
    """
def sort_faces(shape: mod3d.TopoDS.Shape) -> list[mod3d.TopoDS.Face]:
    """
    Sorts faces by surface complexity (Plane, Cylinder, Cone, Sphere, Torus, other).
    """
def update_deflection(shape: mod3d.TopoDS.Shape) -> None:
    """
    Updates value of deflection in Poly_Triangulation by the maximum deviation.
    """
def update_edge_tol(edge: mod3d.TopoDS.Edge, min_tolerance_request: typing.SupportsFloat, max_tolerance_to_check: typing.SupportsFloat) -> bool:
    """
    Updates edge tolerance if needed. Returns True if updated.
    """
def update_edge_tolerance(shape: mod3d.TopoDS.Shape, min_tolerance_request: typing.SupportsFloat, max_tolerance_to_check: typing.SupportsFloat) -> bool:
    """
    Updates tolerances for all edges of the shape.
    """
def update_inner_tolerances(shape: mod3d.TopoDS.Shape) -> None:
    """
    Checks tolerances of edges and vertices and updates them for SameParameter condition.
    """
def update_tolerances(shape: mod3d.TopoDS.Shape, verify_face_tolerance: bool = False) -> None:
    """
    Replaces tolerance of FACE EDGE VERTEX by the tolerance Max of their connected shapes.
    """
def update_tolerances_reshape(shape: mod3d.TopoDS.Shape, reshaper: mod3d.BRepTools.ReShape, verify_face_tolerance: bool = False) -> None:
    """
    Replaces tolerances, recording modifications in reshaper.
    """
