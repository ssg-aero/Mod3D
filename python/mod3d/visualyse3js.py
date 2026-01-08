import numpy as np
from mod3d import Render
from pythreejs import (
    BufferGeometry, BufferAttribute, Mesh, Scene, 
    PerspectiveCamera, Renderer, AmbientLight, DirectionalLight,
    MeshBasicMaterial, OrbitControls,  LineSegments,
    LineBasicMaterial, MeshPhongMaterial, LineMaterial, Geometry, Line, MeshPhysicalMaterial
)
from ipywidgets import Box, Layout

def faces_mesh(faces_data):
    """
    Convert OCCT face tessellation data to a pythreejs mesh.
    
    Parameters:
    -----------
    faces_data : list
        List of tuples (triangles, vertices, normals, uvs) for each face
        
    Returns:
    --------
    mesh : pythreejs.Mesh or None
        The combined mesh for all faces with polygon offset to prevent z-fighting with edges
    """
    if not faces_data:
        return None
    
    # Combine all faces into single mesh
    all_vertices = []
    all_indices = []
    all_normals = []
    vertex_offset = 0
    
    for triangles, vertices, normals, uvs in faces_data:
        # Add vertices
        all_vertices.append(vertices)
        
        # Add normals if available
        if normals is not None:
            all_normals.append(normals)
        
        # Add indices with offset
        all_indices.append(triangles + vertex_offset)
        vertex_offset += vertices.shape[0]
    
    # Concatenate all data
    vertices_combined = np.vstack(all_vertices).astype(np.float32)
    indices_combined = np.vstack(all_indices).astype(np.uint32).ravel()
    
    # Create BufferGeometry
    geometry = BufferGeometry(
        attributes={
            'position': BufferAttribute(vertices_combined, normalized=False),
            'index': BufferAttribute(indices_combined, normalized=False)
        }
    )
    
    # Compute vertex normals for smooth shading
    geometry.exec_three_obj_method('computeVertexNormals')
    
    # Create material with polygon offset to prevent z-fighting with edges
    # polygonOffset pushes the faces back slightly in depth buffer
    material = MeshPhongMaterial(
        color='#2194ce',
        shininess=100,
        side='DoubleSide',
        polygonOffset=True,        # Enable polygon offset
        polygonOffsetFactor=1,     # Offset factor
        polygonOffsetUnits=1       # Offset units
    )

    material = MeshBasicMaterial(
        color='#2194ce',
        side='DoubleSide',
        polygonOffset=True,        # Enable polygon offset
        polygonOffsetFactor=1,     # Offset factor
        polygonOffsetUnits=1       # Offset units
    )

    material = MeshPhysicalMaterial(
        color='#049ef4',              # Bright blue color
        metalness=0.1,                # Low metalness for non-metallic appearance (0-1)
        roughness=1.0,                # Some roughness for realistic surface (0=mirror, 1=diffuse)
        clearcoat=0.5,                # Clear coating layer on top (0-1)
        clearcoatRoughness=0.1,       # Smooth clear coat
        reflectivity=0.5,             # How reflective the surface is (0-1)
        ior=1.5,                      # Index of refraction (glass=1.5, diamond=2.4)
        side='DoubleSide',            # Render both sides of faces
        polygonOffset=True,           # Enable polygon offset to prevent z-fighting with edges
        polygonOffsetFactor=1,        # Offset factor
        polygonOffsetUnits=1          # Offset units
    )
    
    mesh = Mesh(geometry=geometry, material=material)
    mesh.renderOrder = 0  # Render faces first
    return mesh

def edges_mesh(edges_data):
    """
    Convert OCCT edge tessellation data to pythreejs lines.
    
    Parameters:
    -----------
    edges_data : list
        List of tuples (indices, vertices) for each edge
        
    Returns:
    --------
    edge_lines : list or None
        List of pythreejs.Line objects, one per edge, with renderOrder set to appear on top
    """
    if not edges_data:
        return None
    
    # Create separate Line object for each edge
    edge_lines = []
    material = LineBasicMaterial(linewidth=5, color='#000000')
    
    for _, vertices in edges_data:
        # The vertices are already in the correct order for the polyline
        # Just use them directly to create continuous lines
        vertices_array = vertices.astype(np.float32)
        
        # Create geometry with vertices for continuous line
        geometry = Geometry(vertices=vertices_array.tolist())
        
        # Create continuous line for this edge
        line = Line(geometry=geometry, material=material)
        line.renderOrder = 1  # Render edges after faces to ensure visibility
        edge_lines.append(line)
    
    # Return list of edge lines
    return edge_lines

def occt_to_threejs(shape, linear_deflection=0.1, **kwargs):
    """
    Convert an OCCT shape to pythreejs mesh and edge lines.
    
    Parameters:
    -----------
    shape : TopoDS_Shape
        The OCCT shape to convert
    linear_deflection : float, optional
        The maximum distance between the triangulation and the surface (default: 0.1)
        Smaller values create finer meshes
    **kwargs : dict
        Additional arguments passed to extract_tessellation:
        - angle_deflection: Angular deflection in degrees (default: 20.0)
        - relative: Use relative deflection (default: False)
    
    Returns:
    --------
    mesh_face : pythreejs.Mesh
        The rendered mesh for faces
    mesh_edges : list of pythreejs.Line
        List of line objects for edges, rendered on top of faces
        
    Notes:
    ------
    - Faces use polygon offset to prevent z-fighting with edges
    - Edges are rendered as separate continuous polylines per edge
    - RenderOrder ensures edges appear on top of faces
    """
    # Extract tessellation data
    faces_data, edges_data = Render.extract_tessellation(shape, linear_deflection, **kwargs)
    
    mesh_face = faces_mesh(faces_data)
    mesh_edges = edges_mesh(edges_data)

    return mesh_face, mesh_edges

class ShapeRenderer:
    """Simple renderer that can accumulate OCCT shapes and display them in one scene."""

    def __init__(self, linear_deflection=0.1, angle_deflection=15.0, width=1200, height=600):
        self.linear_deflection = linear_deflection
        self.angle_deflection = angle_deflection
        self.width = width
        self.height = height
        self._models = []

    def add_shape(self, shape):
        """Queue an OCCT shape for rendering."""
        self._models.append(shape)
        return shape

    def clear(self):
        """Drop all queued shapes."""
        self._models.clear()

    def _prepare_meshes(self):
        meshes = []
        for shape in self._models:
            mesh_face, mesh_edges = occt_to_threejs(
                shape,
                linear_deflection=self.linear_deflection,
                angle_deflection=self.angle_deflection,
            )
            meshes.append((mesh_face, mesh_edges))
        return meshes

    def render(self, background=None, lights=None):
        """Return a renderer showing everything that has been queued via `add_shape`."""
        if not self._models:
            raise RuntimeError("No shapes have been queued for rendering")

        camera = PerspectiveCamera(position=[0, 20, 40], aspect=self.width / self.height)
        default_lights = [
            AmbientLight(intensity=0.5),
            DirectionalLight(position=[10, 10, 10], intensity=0.5),
        ]
        lights = lights or default_lights

        scene_children = [camera, *lights]
        for mesh_face, mesh_edges in self._prepare_meshes():
            scene_children.append(mesh_face)
            if mesh_edges:
                scene_children.extend(mesh_edges)

        scene_kwargs = {"children": scene_children}
        if background is not None:
            scene_kwargs["background"] = background
        scene = Scene(**scene_kwargs)
        controls = OrbitControls(controlling=camera)
        # renderer = ShapeRenderer().render(shape)
        renderer = Renderer(
            camera=camera,
            scene=scene,
            controls=[controls],
            width=self.width,
            height=self.height,
            antialias=True,
        )
        bordered = Box([renderer], layout=Layout(
            border="2px solid #444",
            padding="4px",
            width="100%",
            height="100%"
        ))
        return bordered
