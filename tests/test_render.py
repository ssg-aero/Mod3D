"""Tests for rendering/tessellation functions."""

import numpy as np
from mod3d import (
    BRepBuilderAPI,
    Render,
)


def test_extract_tessellation_box():
    """Test extract_tessellation with a simple box."""
    # Create a box
    box = BRepBuilderAPI.MakeBox(10.0, 20.0, 30.0).shape()
    
    # Extract tessellation
    result_faces, result_edges = Render.extract_tessellation(box, linear_deflection=0.1)
    
    # A box has 6 faces
    assert len(result_faces) == 6, "Box should have 6 faces"
    
    # Check each face
    for i, (triangles, vertices, normals, uvs) in enumerate(result_faces):
        # Check types
        msg = f"Face {i}: triangles should be numpy array"
        assert isinstance(triangles, np.ndarray), msg
        msg = f"Face {i}: vertices should be numpy array"
        assert isinstance(vertices, np.ndarray), msg
        msg = f"Face {i}: normals should be numpy array"
        assert isinstance(normals, np.ndarray), msg
        msg = f"Face {i}: uvs should be numpy array"
        assert isinstance(uvs, np.ndarray), msg
        
        # Check dtypes
        msg = f"Face {i}: triangles should be int32"
        assert triangles.dtype == np.int32, msg
        msg = f"Face {i}: vertices should be float64"
        assert vertices.dtype == np.float64, msg
        msg = f"Face {i}: normals should be float64"
        assert normals.dtype == np.float64, msg
        msg = f"Face {i}: uvs should be float64"
        assert uvs.dtype == np.float64, msg
        
        # Check shapes
        msg = f"Face {i}: triangles should be 2D"
        assert triangles.ndim == 2, msg
        msg = f"Face {i}: vertices should be 2D"
        assert vertices.ndim == 2, msg
        msg = f"Face {i}: normals should be 2D"
        assert normals.ndim == 2, msg
        msg = f"Face {i}: uvs should be 2D"
        assert uvs.ndim == 2, msg
        
        msg = f"Face {i}: triangles should have 3 columns"
        assert triangles.shape[1] == 3, msg
        msg = f"Face {i}: vertices should have 3 columns"
        assert vertices.shape[1] == 3, msg
        msg = f"Face {i}: normals should have 3 columns"
        assert normals.shape[1] == 3, msg
        msg = f"Face {i}: uvs should have 2 columns"
        assert uvs.shape[1] == 2, msg
        
        # Check matching dimensions
        n_vertices = vertices.shape[0]
        msg = f"Face {i}: normals count should match vertices"
        assert normals.shape[0] == n_vertices, msg
        msg = f"Face {i}: uvs count should match vertices"
        assert uvs.shape[0] == n_vertices, msg
        
        # Check triangle indices are valid
        msg = f"Face {i}: triangle indices should be non-negative"
        assert triangles.min() >= 0, msg
        msg = f"Face {i}: triangle indices within vertex range"
        assert triangles.max() < n_vertices, msg
        
        # Check we have some data
        msg = f"Face {i}: should have triangles"
        assert triangles.shape[0] > 0, msg
        msg = f"Face {i}: should have vertices"
        assert vertices.shape[0] > 0, msg


# def test_extract_tessellation_cylinder():
#     """Test extract_tessellation with a cylinder."""
#     # Create a cylinder
#     cylinder = BRepBuilderAPI.MakeCylinder(5.0, 10.0).shape()
    
#     # Extract tessellation with finer deflection
#     result = Render.extract_tessellation(
#         cylinder, linear_deflection=0.05, angle_deflection=0.3
#     )
    
#     # A cylinder has 3 faces (top, bottom, side)
#     assert len(result) == 3, "Cylinder should have 3 faces"
    
#     # Verify all faces have data
#     for triangles, vertices, normals, uvs in result:
#         assert triangles.size > 0, "Face should have triangles"
#         assert vertices.size > 0, "Face should have vertices"
#         assert normals.size > 0, "Face should have normals"
#         assert uvs.size > 0, "Face should have UVs"


# def test_extract_tessellation_sphere():
#     """Test extract_tessellation with a sphere."""
#     # Create a sphere
#     sphere = BRepBuilderAPI.MakeSphere(5.0).shape()
    
#     # Extract tessellation
#     result = Render.extract_tessellation(sphere, linear_deflection=0.1)
    
#     # A sphere has 1 face
#     assert len(result) == 1, "Sphere should have 1 face"
    
#     triangles, vertices, normals, uvs = result[0]
    
#     # Verify data integrity
#     assert triangles.size > 0, "Sphere should have triangles"
#     assert vertices.size > 0, "Sphere should have vertices"
    
#     # Check that all vertices are approximately on the sphere surface
#     distances = np.linalg.norm(vertices, axis=1)
#     msg = "All vertices should be on sphere surface (radius 5.0)"
#     assert np.allclose(distances, 5.0, atol=0.2), msg
    
#     # Check that normals are unit vectors
#     normal_lengths = np.linalg.norm(normals, axis=1)
#     msg = "All normals should be unit vectors"
#     assert np.allclose(normal_lengths, 1.0, atol=0.01), msg


# def test_extract_tessellation_relative_deflection():
#     """Test extract_tessellation with relative deflection."""
#     # Create a box
#     box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    
#     # Extract with relative deflection
#     result = extract_tessellation(
#         box, linear_deflection=0.01, is_relative=True
#     )
    
#     assert len(result) == 6, "Box should have 6 faces"
    
#     # Verify all faces have data
#     for triangles, vertices, normals, uvs in result:
#         assert triangles.size > 0, "Face should have data"


# def test_extract_tessellation_parallel_vs_sequential():
#     """Test extract_tessellation parallel vs sequential mode."""
#     # Create a box
#     box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
    
#     # Extract with parallel
#     result_parallel = Render.extract_tessellation(
#         box, linear_deflection=0.1, parallel=True
#     )
    
#     # Extract without parallel
#     result_sequential = Render.extract_tessellation(
#         box, linear_deflection=0.1, parallel=False
#     )
    
#     # Both should produce the same number of faces
#     msg = "Parallel and sequential should produce same number of faces"
#     assert len(result_parallel) == len(result_sequential), msg
    
#     # Check similar vertex counts (not identical due to meshing variations)
#     for i in range(len(result_parallel)):
#         v_par = result_parallel[i][1].shape[0]
#         v_seq = result_sequential[i][1].shape[0]
#         # Allow some variation in vertex count
#         msg = f"Face {i}: vertex counts should be similar"
#         assert abs(v_par - v_seq) / max(v_par, v_seq) < 0.5, msg


# def test_extract_tessellation_empty_face():
#     """Test that extract_tessellation handles shapes properly."""
#     # Create a simple edge (no faces)
#     from mod3d import gp, BRepBuilderAPI
    
#     p1 = gp.Pnt(0.0, 0.0, 0.0)
#     p2 = gp.Pnt(10.0, 0.0, 0.0)
#     edge = BRepBuilderAPI.MakeEdge(p1, p2).edge()
    
#     # Extract tessellation from edge (should return empty list)
#     result = Render.extract_tessellation(edge, linear_deflection=0.1)
    
#     assert len(result) == 0, "Edge should have no faces"


# def test_extract_tessellation_angle_deflection():
#     """Test extract_tessellation with different angle deflections."""
#     # Create a cylinder (curved surface)
#     cylinder = BRepBuilderAPI.MakeCylinder(5.0, 10.0).shape()
    
#     # Coarse meshing (larger angle)
#     result_coarse = Render.extract_tessellation(
#         cylinder, linear_deflection=0.1, angle_deflection=1.0
#     )
    
#     # Fine meshing (smaller angle)
#     result_fine = Render.extract_tessellation(
#         cylinder, linear_deflection=0.1, angle_deflection=0.1
#     )
    
#     # Fine meshing should generally produce more vertices on curves
#     # Compare the side face (typically index 2)
#     if len(result_coarse) >= 3 and len(result_fine) >= 3:
#         vertices_coarse = result_coarse[2][1].shape[0]
#         vertices_fine = result_fine[2][1].shape[0]
        
#         msg = "Finer angle deflection should produce more vertices"
#         assert vertices_fine >= vertices_coarse, msg
