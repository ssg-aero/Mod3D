import numpy as np
from mod3d import Render, TopoDS, Geom
from pythreejs import (
    BufferGeometry, BufferAttribute, Mesh, Scene, Points,
    PerspectiveCamera, Renderer, AmbientLight, DirectionalLight,
    MeshBasicMaterial, OrbitControls, LineSegments,
    LineBasicMaterial, MeshPhongMaterial, LineMaterial, Geometry, Line, MeshPhysicalMaterial, PointsMaterial, ShaderMaterial,
    CylinderGeometry, ConeGeometry, SphereGeometry, GridHelper, Fog, Group,
)

try:
    from pythreejs import Line2, LineGeometry
    _HAS_LINE2 = True
except ImportError:  # Fallback for environments without fat-line support
    _HAS_LINE2 = False
from ipywidgets import Box, Layout


def axis_angle_quaternion(axis, angle):
    """Convert axis-angle to quaternion (x, y, z, w)."""
    axis = np.array(axis, dtype=np.float64)
    axis = axis / np.linalg.norm(axis) if np.linalg.norm(axis) > 0 else axis
    half_angle = angle / 2.0
    sin_half = np.sin(half_angle)
    cos_half = np.cos(half_angle)
    return (axis[0] * sin_half, axis[1] * sin_half, axis[2] * sin_half, cos_half)


def create_trihedron(size=1):
    """Create XYZ axes (trihedron) for orientation reference."""
    axes = []
    colors = ['red', 'green', 'blue']  # X=red, Y=green, Z=blue
    directions = [
        [[0, 0, 0], [size, 0, 0]],  # X
        [[0, 0, 0], [0, size, 0]],  # Y
        [[0, 0, 0], [0, 0, size]],  # Z
    ]

    for color, pts in zip(colors, directions):
        vertices = np.array(pts, dtype=np.float32)
        geometry = BufferGeometry(attributes={
            'position': BufferAttribute(vertices, normalized=False)
        })
        material = LineBasicMaterial(color=color, linewidth=2)
        line = Line(geometry=geometry, material=material)
        axes.append(line)

    return axes


def make_axis_decoration(color, axis, angle, cylinder_pos, cone_pos, depth_test, opacity):
    """Create cylinder shaft and cone arrowhead for an axis."""
    quaternion = axis_angle_quaternion(axis, angle)
    cylinder = Mesh(
        geometry=CylinderGeometry(radiusTop=0.05, radiusBottom=0.05, height=0.9, radialSegments=16),
        material=MeshBasicMaterial(color=color, depthTest=depth_test, opacity=opacity, transparent=True),
        quaternion=quaternion,
        position=cylinder_pos,
    )
    cone = Mesh(
        geometry=ConeGeometry(radius=0.1, height=0.3, radialSegments=16),
        material=MeshBasicMaterial(color=color, depthTest=depth_test, opacity=opacity, transparent=True),
        quaternion=quaternion,
        position=cone_pos,
    )
    return cylinder, cone


def create_trihedron_helper(depth_test, opacity):
    """Create a complete trihedron helper with optional decorations and origin sphere."""
    # trihedron_axes = create_trihedron(size=1)
    # trihedron_lines = Group(children=trihedron_axes)

    # children = [trihedron_lines]

    children = []

    # if show_decorations:
    axis_configs = [
        ('x', '#ff3333', [0, 0, 1], -np.pi / 2, [0.45, 0, 0], [0.95, 0, 0]),
        ('y', '#33ff33', [0, 0, 1], 0, [0, 0.45, 0], [0, 0.95, 0]),
        ('z', '#3333ff', [1, 0, 0], np.pi / 2, [0, 0, 0.45], [0, 0, 0.95]),
    ]
    decorations = []
    for _, color, axis, angle, cyl_pos, cone_pos in axis_configs:
        cylinder, cone = make_axis_decoration(color, axis, angle, cyl_pos, cone_pos, depth_test, opacity)
        decorations.extend([cylinder, cone])
    trihedron_decorations = Group(children=decorations)
    children.append(trihedron_decorations)

    origin_sphere = Mesh(
        geometry=SphereGeometry(radius=0.08, widthSegments=16, heightSegments=16),
        material=MeshBasicMaterial(color='gray', transparent=True, opacity=opacity, depthTest=depth_test),
        position=[0, 0, 0],
    )
    children.append(origin_sphere)

    return Group(children=children)


# Custom shader to draw circles instead of squares
vertex_shader = """
uniform float size;
void main() {
    vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);
    gl_PointSize = size;
    gl_Position = projectionMatrix * mvPosition;
}
"""

fragment_shader = """
uniform vec3 color;
void main() {
    // Calculate distance from center of point (0.5, 0.5 is center in gl_PointCoord)
    vec2 center = gl_PointCoord - vec2(0.5);
    float dist = length(center);
    
    // Discard pixels outside the circle
    if (dist > 0.5) discard;
    
    gl_FragColor = vec4(color, 1.0);
}
"""

def faces_mesh(faces_data, color='#2194ce', material = None):
    """
    Convert OCCT face tessellation data to a pythreejs mesh.
    
    Parameters:
    -----------
    faces_data : list
        List of tuples (triangles, vertices, normals, uvs) for each face
    color : str
        Color for the mesh material
    material : pythreejs.Material or None
        If provided, use this material instead of creating a new one.
        
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
    # material = MeshPhongMaterial(
    #     color=color,
    #     shininess=100,
    #     side='DoubleSide',
    #     polygonOffset=True,        # Enable polygon offset
    #     polygonOffsetFactor=1,     # Offset factor
    #     polygonOffsetUnits=1       # Offset units
    # )

    # material = MeshBasicMaterial(
    #     color=color,
    #     side='DoubleSide',
    #     polygonOffset=True,        # Enable polygon offset
    #     polygonOffsetFactor=1,     # Offset factor
    #     polygonOffsetUnits=1       # Offset units
    # )

    if material is None:

        material = MeshPhysicalMaterial(
            color=color,
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

def edges_mesh(edges_data, color='#000000', linewidth=2, resolution=None):
    """
    Convert OCCT edge tessellation data to pythreejs lines.
    
    Parameters:
    -----------
    edges_data : list
        List of tuples (indices, vertices) for each edge
    color : str
    Returns:
    --------
    edge_lines : list or None
        List of pythreejs.Line objects, one per edge, with renderOrder set to appear on top
    """
    if not edges_data:
        return None
    
    # Create separate Line object for each edge
    edge_lines = []

    if _HAS_LINE2:
        # LineMaterial respects linewidth when used with Line2/LineGeometry; resolution keeps the size stable in px.
        material = LineMaterial(
            linewidth=linewidth,
            color=color,
            resolution=resolution if resolution is not None else (1200, 800),
            dashed=False,
        )
        for _, vertices in edges_data:
            vertices_array = vertices.astype(np.float32)
            geometry = LineGeometry(positions=vertices_array)
            line = Line2(geometry=geometry, material=material)
            line.renderOrder = 1  # Render edges after faces to ensure visibility
            edge_lines.append(line)
    else:
        material = LineBasicMaterial(linewidth=linewidth, color=color)
        for _, vertices in edges_data:
            vertices_array = vertices.astype(np.float32)
            geometry = Geometry(vertices=vertices_array.tolist())
            line = Line(geometry=geometry, material=material)
            line.renderOrder = 1
            edge_lines.append(line)
    
    # Return list of edge lines
    return edge_lines

def mesh_vertices(vertices, color='#2194ce', size=0.1, rounded=True):
    """
    Create a point cloud mesh from vertices.
    
    Parameters:
    -----------
    vertices : np.ndarray
        Nx3 array of vertex positions
    color : str
        Color for the points
    size : float
        Size of each point
        
    Returns:
    --------
    points_mesh : pythreejs.Points
        The point cloud mesh
    """
    geometry = BufferGeometry(
        attributes={
            'position': BufferAttribute(vertices.astype(np.float32), normalized=False)
        }
    )
    if rounded:
        material = ShaderMaterial(
            vertexShader=vertex_shader,
            fragmentShader=fragment_shader,
            uniforms={
                'size': {'value': size * 2},
                'color': {'value': color},
            },
            transparent=True,
        )
    else:
        material = PointsMaterial(color=color, size=size, sizeAttenuation=False)
    points_mesh = Points(geometry=geometry, material=material)
    return points_mesh

def occt_to_threejs(shape, linear_deflection=0.1, points_color='blue', points_size=5.0, points_rounded=True, curve_color='lime', edge_color='black', curve_width=2, edge_width=0.5, line_resolution=None, surface_color='#2194ce', color=None, **kwargs):
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
    if(isinstance(shape, Geom.Curve) or isinstance(shape, TopoDS.Edge) or isinstance(shape, TopoDS.Wire)):
        edges_data = Render.extract_curve_tessellation(shape, linear_deflection)

        mesh_edges = edges_mesh([edges_data], color=curve_color, linewidth=curve_width, resolution=line_resolution)

        return None, mesh_edges
    elif(isinstance(shape, TopoDS.Shape)):
        faces_data, edges_data = Render.extract_tessellation(shape, linear_deflection, **kwargs)
        
        face_color = color if color is not None else surface_color
        mesh_face = faces_mesh(faces_data, color=face_color)
        mesh_edges = edges_mesh(edges_data, color=edge_color, linewidth=edge_width, resolution=line_resolution)

        return mesh_face, mesh_edges

    elif(isinstance(shape, np.ndarray)):
        points_mesh = mesh_vertices(shape, color=points_color, size=points_size, rounded=points_rounded)
        return points_mesh, None
    
    elif(isinstance(shape, list)):
        mesh_face = []
        mesh_edges = []
        for subshape in shape:
            mf, me = occt_to_threejs(subshape, linear_deflection, line_resolution=line_resolution, **kwargs)
            if mf is not None:
                mesh_face.append(mf)
            if me is not None:
                mesh_edges.extend(me)

        return mesh_face, mesh_edges

    else:
        raise TypeError("Unsupported shape type for rendering")

class ShapeRenderer:
    """Simple renderer that can accumulate OCCT shapes and display them in one scene."""

    def __init__(self, linear_deflection=0.1, angle_deflection=15.0, width=1200, height=600):
        self.linear_deflection = linear_deflection
        self.angle_deflection = angle_deflection
        self.width = width
        self.height = height
        self._models = []

        self.point_color = 'blue'
        self.point_size = 5.
        self.point_rounded = True
        self.curve_color = 'lime'
        self.curve_width = 2
        self.edge_color = 'black'
        self.edge_width = 0.5
        self.surface_color = '#2194ce'

        # Trihedron and grid settings
        self.show_trihedron = True
        self.trihedron_scale = 2.5
        self.trihedron_opacity = 0.8
        self.trihedron_depth_test = False
        self.show_grid = True
        self.grid_size = 800
        self.grid_divisions = 80
        self.grid_color = '#bbbbbb'
        self.grid_center_color = '#777777'
        self.show_fog = True
        self.fog_color = 'lightgray'
        self.fog_near = 30
        self.fog_far = None  # Auto-calculated from grid_size if None

    def add_shape(self, shape, color=None):
        """Queue an OCCT shape for rendering."""
        self._models.append((shape, color))
        return shape

    def clear(self):
        """Drop all queued shapes."""
        self._models.clear()

    def _prepare_meshes(self):
        meshes = []
        for shape, color in self._models:
            if color is not None:
                mesh_face, mesh_edges = occt_to_threejs(
                    shape,
                    linear_deflection=self.linear_deflection,
                    angle_deflection=self.angle_deflection,
                    color=color,
                    line_resolution=(self.width, self.height),
                )
            else:
                mesh_face, mesh_edges = occt_to_threejs(
                    shape,
                    linear_deflection=self.linear_deflection,
                    angle_deflection=self.angle_deflection,
                    points_color=self.point_color,
                    points_size=self.point_size,
                    points_rounded=self.point_rounded,
                    curve_color=self.curve_color,
                    curve_width=self.curve_width,
                    edge_color=self.edge_color,
                    edge_width=self.edge_width,
                    line_resolution=(self.width, self.height),
                    surface_color=self.surface_color,
                )
            meshes.append((mesh_face, mesh_edges))
        return meshes

    def render(self, background=None, lights=None, camera_position=None):
        """Return a renderer showing everything that has been queued via `add_shape`."""
        if not self._models:
            raise RuntimeError("No shapes have been queued for rendering")

        if camera_position is None:
            camera_position = [0, 20, 40]
        camera = PerspectiveCamera(position=camera_position, aspect=self.width / self.height)
        default_lights = [
            AmbientLight(intensity=0.5),
            DirectionalLight(position=[10, 10, 10], intensity=0.5),
        ]
        lights = lights or default_lights

        scene_children = [camera, *lights]
        for mesh_face, mesh_edges in self._prepare_meshes():
            if mesh_face is not None:
                if isinstance(mesh_face, list):
                    scene_children.extend(mesh_face)
                else:
                    scene_children.append(mesh_face)
            if mesh_edges:
                scene_children.extend(mesh_edges)

        # Add grid helper
        if self.show_grid:
            grid_helper = GridHelper(
                size=self.grid_size,
                divisions=self.grid_divisions,
                colorCenterLine=self.grid_center_color,
                colorGrid=self.grid_color,
            )
            scene_children.append(grid_helper)

        # Add trihedron helper
        helper_root = None
        if self.show_trihedron:
            helper_root = create_trihedron_helper(
                depth_test=self.trihedron_depth_test,
                opacity=self.trihedron_opacity,
            )
            initial_distance = np.linalg.norm(np.array(camera_position))
            helper_root.scale = (self.trihedron_scale, self.trihedron_scale, self.trihedron_scale)
            scene_children.append(helper_root)

        scene_kwargs = {"children": scene_children}
        if background is not None:
            scene_kwargs["background"] = background

        # Add fog
        if self.show_fog:
            fog_far = self.fog_far if self.fog_far is not None else self.grid_size / 2
            scene_fog = Fog(color=self.fog_color, near=self.fog_near, far=fog_far)
            scene_kwargs["fog"] = scene_fog

        scene = Scene(**scene_kwargs)
        controls = OrbitControls(controlling=camera)
        renderer = Renderer(
            camera=camera,
            scene=scene,
            controls=[controls],
            width=self.width,
            height=self.height,
            antialias=True,
        )

        # Setup dynamic trihedron scaling and fog adjustment
        if self.show_trihedron or self.show_fog:
            initial_distance = np.linalg.norm(np.array(camera_position))
            fog_near_base = self.fog_near
            fog_far_base = self.fog_far if self.fog_far is not None else self.grid_size / 2
            trihedron_scale = self.trihedron_scale
            scene_fog_ref = scene_kwargs.get("fog")

            def update_camera_dependent(change):
                pos = np.array(camera.position)
                distance = np.linalg.norm(pos)
                if helper_root is not None:
                    scale = distance * trihedron_scale / initial_distance
                    helper_root.scale = (scale, scale, scale)
                if scene_fog_ref is not None:
                    scene_fog_ref.near = max(fog_near_base, distance * 0.15)
                    scene_fog_ref.far = max(fog_far_base, fog_far_base + distance)

            camera.observe(update_camera_dependent, names=['position'])
            update_camera_dependent(None)

        bordered = Box([renderer], layout=Layout(
            border="2px solid #444",
            padding="4px",
            width="100%",
            height="100%"
        ))
        return bordered
