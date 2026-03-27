"""
Shape upgrading module
"""
from __future__ import annotations
import mod3d.TopoDS
import typing
__all__: list[str] = ['UnifySameDomain', 'union_faces']
class UnifySameDomain:
    """
    
            Unifies faces and edges of a shape that lie on the same geometry.
    
            Faces/edges are considered 'same-domain' if a group of neighbouring
            faces/edges lie on coincident surfaces/curves. In this case these
            faces/edges can be unified into one face/edge.
    
            Parameters (constructor or initialize):
                shape: The input shape to process
                unify_edges: Try to unify all possible edges (default: True)
                unify_faces: Try to unify all possible faces (default: True)  
                concat_bsplines: Merge neighbouring edges on BSpline/Bezier curves
                    with C1 continuity (default: False)
    
            Example:
                unifier = ShapeUpgrade.UnifySameDomain(shape)
                unifier.build()
                result = unifier.shape
            
    """
    @typing.overload
    def __init__(self) -> None:
        """
        Creates an empty unifier.
        """
    @typing.overload
    def __init__(self, shape: mod3d.TopoDS.Shape, unify_edges: bool = True, unify_faces: bool = True, concat_bsplines: bool = False) -> None:
        """
        Creates a unifier with the given shape and options.
        """
    def allow_internal_edges(self, allow: bool) -> None:
        """
        Sets whether internal edges can be created inside merged faces
        in non-manifold topology. Default is False.
        """
    def build(self) -> None:
        """
        Performs unification and builds the resulting shape.
        """
    def initialize(self, shape: mod3d.TopoDS.Shape, unify_edges: bool = True, unify_faces: bool = True, concat_bsplines: bool = False) -> None:
        """
        Initializes with a shape and options. Does not perform unification.
        """
    def keep_shape(self, shape: mod3d.TopoDS.Shape) -> None:
        """
        Marks a shape (vertex or edge) to avoid merging.
        If vertex: forbids merging of connected edges.
        If edge: forbids merging of connected faces.
        """
    def set_angular_tolerance(self, tolerance: typing.SupportsFloat) -> None:
        """
        Sets the angular tolerance. Shapes with connection angle greater
        than this value will not be merged. Default is Precision::Angular().
        """
    def set_linear_tolerance(self, tolerance: typing.SupportsFloat) -> None:
        """
        Sets the linear tolerance (chord error for merging decisions).
        Default is Precision::Confusion().
        """
    def set_safe_input_mode(self, safe: bool) -> None:
        """
        Sets whether the original shape can be modified. Default is True (safe).
        """
    @property
    def shape(self) -> mod3d.TopoDS.Shape:
        """
        Returns the resulting unified shape.
        """
def union_faces(shape: mod3d.TopoDS.Shape) -> mod3d.TopoDS.Shape:
    """
    Unifies faces of a shape that lie on the same geometry.
    
    Faces are considered 'same-domain' if a group of neighbouring
    faces lie on coincident surfaces. In this case these faces can be
    unified into one face.
    
    This is a healing operation that can be used to clean up imported
    models or to prepare shapes for downstream operations.
    """
