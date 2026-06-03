"""PyVista (VTK) rendering backend.

Consumes the backend-neutral scene IR from :mod:`mod3d.render._scene` — the
same decomposition the pythreejs backend uses — so the instancing logic stays
shared between the two. Where pythreejs only renders inside a notebook, PyVista
drives three output targets from one code path:

- ``mode="window"`` — interactive desktop VTK window;
- ``mode="notebook"`` — interactive in Jupyter via the trame backend;
- ``mode="html"`` — a self-contained interactive HTML file (vtk.js) for sharing.

Instancing: shapes sharing a TShape (e.g. the rotated copies of a periodic
blade row) are tessellated once into a single ``pyvista.PolyData``; one VTK
mapper is built per unique geometry and shared across N actors, each placed by
its own ``user_matrix``. VTK uploads the geometry to the GPU a single time, so
memory does not scale with the copy count.
"""
from __future__ import annotations

import numpy as np
import pyvista as pv

from mod3d import Render, TopoDS, Geom

from ._scene import shape_instance_groups


def _faces_polydata(faces_data):
    """Combine OCCT face tessellation tuples into one ``pyvista.PolyData``.

    `faces_data` is the list of ``(triangles, vertices, normals, uvs)`` tuples
    returned by :func:`mod3d.Render.extract_tessellation`, in the group's
    TShape-local frame. Returns ``None`` for an empty face set.
    """
    if not faces_data:
        return None

    all_vertices = []
    all_triangles = []
    all_normals = []
    have_normals = True
    offset = 0
    for triangles, vertices, normals, _uvs in faces_data:
        all_vertices.append(vertices)
        all_triangles.append(triangles + offset)
        if normals is None:
            have_normals = False
        else:
            all_normals.append(normals)
        offset += vertices.shape[0]

    points = np.vstack(all_vertices).astype(np.float64)
    tris = np.vstack(all_triangles).astype(np.int64)
    # VTK face stream: [3, i, j, k, 3, i, j, k, ...] (leading vertex count).
    faces = np.empty((tris.shape[0], 4), dtype=np.int64)
    faces[:, 0] = 3
    faces[:, 1:] = tris
    poly = pv.PolyData(points, faces.ravel())

    if have_normals and all_normals:
        normals = np.vstack(all_normals).astype(np.float64)
        poly.point_data["Normals"] = normals
        poly.point_data.active_normals_name = "Normals"
    return poly


def _edges_polydata(edges_data):
    """Combine OCCT edge tessellation polylines into one ``pyvista.PolyData``.

    `edges_data` is the list of ``(indices, vertices)`` tuples from
    :func:`mod3d.Render.extract_tessellation`. Returns ``None`` if empty.
    """
    if not edges_data:
        return None

    all_vertices = []
    lines = []
    offset = 0
    for _indices, vertices in edges_data:
        n = vertices.shape[0]
        if n < 2:
            continue
        all_vertices.append(vertices)
        # VTK line stream: [n, p0, p1, ..., p(n-1)] for one polyline.
        lines.append(np.concatenate(([n], np.arange(offset, offset + n))))
        offset += n

    if not all_vertices:
        return None

    points = np.vstack(all_vertices).astype(np.float64)
    poly = pv.PolyData(points)
    poly.lines = np.concatenate(lines).astype(np.int64)
    return poly


def _points_polydata(vertices):
    return pv.PolyData(np.asarray(vertices, dtype=np.float64))


def _shared_mapper(poly):
    """One mapper per unique geometry; sharing it across actors makes VTK upload
    the dataset to the GPU only once."""
    mapper = pv.DataSetMapper()
    mapper.dataset = poly
    return mapper


def occt_to_pyvista(shape, deflection=0.01, points_color="blue", points_size=8.0,
                    curve_color="lime", curve_width=2.0, edge_color="black",
                    edge_width=2.0, surface_color="#2194ce", face_opacity=1.0,
                    color=None, **kwargs):
    """Convert an OCCT shape to lists of ``(mesh_actors, edge_actors)``.

    Mirrors :func:`mod3d.render.threejs.occt_to_threejs` but yields ``pyvista``
    actors. Periodic copies sharing a TShape reuse one mapper (and thus one GPU
    upload); each copy is an actor placed by its ``user_matrix``.

    `kwargs` (e.g. ``angle_deflection``) are forwarded to the tessellation.
    """
    if isinstance(shape, (Geom.Curve, TopoDS.Edge, TopoDS.Wire)):
        _indices, vertices = Render.extract_curve_tessellation(shape, deflection)
        poly = _edges_polydata([(_indices, vertices)])
        if poly is None:
            return [], []
        actor = pv.Actor(mapper=_shared_mapper(poly))
        actor.prop.color = curve_color
        actor.prop.line_width = curve_width
        return [], [actor]

    if isinstance(shape, TopoDS.Shape):
        face_color = color if color is not None else surface_color
        return _instanced_actors(
            shape, face_color=face_color, edge_color=edge_color,
            edge_width=edge_width, face_opacity=face_opacity,
            extract_kwargs=kwargs)

    if isinstance(shape, np.ndarray):
        poly = _points_polydata(shape)
        actor = pv.Actor(mapper=_shared_mapper(poly))
        actor.prop.color = points_color
        actor.prop.point_size = points_size
        actor.prop.render_points_as_spheres = True
        return [actor], []

    if isinstance(shape, list):
        mesh_actors = []
        edge_actors = []
        for subshape in shape:
            m, e = occt_to_pyvista(
                subshape, deflection=deflection, points_color=points_color,
                points_size=points_size, curve_color=curve_color,
                curve_width=curve_width, edge_color=edge_color,
                edge_width=edge_width, surface_color=surface_color,
                face_opacity=face_opacity, color=color, **kwargs)
            mesh_actors.extend(m)
            edge_actors.extend(e)
        return mesh_actors, edge_actors

    raise TypeError("Unsupported shape type for rendering")


def _instanced_actors(shape, face_color, edge_color, edge_width, face_opacity,
                      extract_kwargs):
    """Build face/edge actors for `shape`, sharing one mapper per unique TShape.

    The backend-neutral decomposition (which copies share a TShape, and each
    copy's placement matrix) comes from ``_scene.shape_instance_groups``; this
    only turns each group into VTK actors. Every copy reuses the group's mapper
    (one GPU upload) and is positioned by its 4x4 ``user_matrix`` — VTK's matrix
    convention is row-major, matching the IR's layout exactly.
    """
    face_actors = []
    edge_actors = []

    for group in shape_instance_groups(shape, **extract_kwargs):
        face_poly = _faces_polydata(group.faces_data)
        edge_poly = _edges_polydata(group.edges_data)
        face_mapper = _shared_mapper(face_poly) if face_poly is not None else None
        edge_mapper = _shared_mapper(edge_poly) if edge_poly is not None else None

        for matrix in group.matrices:
            if face_mapper is not None:
                actor = pv.Actor(mapper=face_mapper)
                actor.user_matrix = matrix
                actor.prop.color = face_color
                actor.prop.opacity = face_opacity
                actor.prop.interpolation = "pbr" if face_opacity >= 1.0 else "phong"
                face_actors.append(actor)
            if edge_mapper is not None:
                actor = pv.Actor(mapper=edge_mapper)
                actor.user_matrix = matrix
                actor.prop.color = edge_color
                actor.prop.line_width = edge_width
                edge_actors.append(actor)

    return face_actors, edge_actors


class PyVistaRenderer:
    """Accumulate OCCT shapes and display them via PyVista in one scene.

    API mirrors :class:`mod3d.render.threejs.ShapeRenderer` (``add_shape`` /
    ``clear`` / ``render``) so the two backends are interchangeable. ``render``
    targets a desktop window, a Jupyter (trame) widget, or a self-contained
    HTML file depending on ``mode``.
    """

    def __init__(self, linear_deflection=0.1, angle_deflection=15.0,
                 window_size=(1200, 600)):
        self.linear_deflection = linear_deflection
        self.angle_deflection = angle_deflection
        self.window_size = window_size
        self._models = []

        self.point_color = "blue"
        self.point_size = 8.0
        self.curve_color = "lime"
        self.curve_width = 2.0
        self.edge_color = "black"
        self.edge_width = 2.0
        self.surface_color = "#2194ce"

        self.show_grid = True
        self.show_trihedron = True
        self.background = "white"

    def add_shape(self, shape, options=None):
        """Queue an OCCT shape (with optional per-shape overrides) for rendering."""
        self._models.append((shape, options or {}))
        return shape

    def clear(self):
        """Drop all queued shapes."""
        self._models.clear()

    def _build_actors(self):
        mesh_actors = []
        edge_actors = []
        for shape, options in self._models:
            m, e = occt_to_pyvista(
                shape,
                linear_deflection=options.get("linear_deflection", self.linear_deflection),
                angle_deflection=options.get("angle_deflection", self.angle_deflection),
                points_color=options.get("points_color", self.point_color),
                points_size=options.get("points_size", self.point_size),
                curve_color=options.get("curve_color", self.curve_color),
                curve_width=options.get("curve_width", self.curve_width),
                edge_color=options.get("edge_color", self.edge_color),
                edge_width=options.get("edge_width", self.edge_width),
                surface_color=options.get("surface_color", self.surface_color),
                face_opacity=options.get("face_opacity", 1.0),
                color=options.get("color"),
            )
            mesh_actors.extend(m)
            edge_actors.extend(e)
        return mesh_actors, edge_actors

    def render(self, mode="auto", path=None, jupyter_backend=None,
               background=None, off_screen=None):
        """Render every queued shape.

        Parameters
        ----------
        mode : {"auto", "window", "notebook", "html"}
            ``auto`` (default) defers to PyVista's environment / global backend
            (toggled with :func:`pyvista.set_jupyter_backend`): an inline trame
            widget inside Jupyter, an interactive desktop window otherwise.
            ``window`` forces the desktop window, ``notebook`` forces the trame
            widget, and ``html`` writes a self-contained interactive HTML file
            to `path` and returns `path`.
        path : str, optional
            Output file for ``mode="html"`` (required in that mode).
        jupyter_backend : str, optional
            Override the PyVista Jupyter backend for this call. Defaults to
            None (PyVista's global setting); ``notebook`` mode falls back to
            ``"trame"`` when unset.
        background : color, optional
            Overrides the renderer's background for this call.
        off_screen : bool, optional
            Force offscreen rendering (defaults to True for ``html``). Useful
            for headless tests of the window/notebook paths.
        """
        valid_modes = ("auto", "window", "notebook", "html")
        if mode not in valid_modes:
            raise ValueError(f"mode must be one of {valid_modes}, got {mode!r}")
        if not self._models:
            raise RuntimeError("No shapes have been queued for rendering")
        if mode == "html" and path is None:
            raise ValueError('render(mode="html") requires a `path`')

        if off_screen is None:
            off_screen = (mode == "html")

        plotter = pv.Plotter(window_size=self.window_size, off_screen=off_screen)
        plotter.set_background(background if background is not None else self.background)

        mesh_actors, edge_actors = self._build_actors()
        for actor in mesh_actors:
            plotter.add_actor(actor)
        for actor in edge_actors:
            plotter.add_actor(actor)

        if self.show_grid:
            plotter.show_grid()
        if self.show_trihedron:
            plotter.add_axes()

        if mode == "html":
            plotter.export_html(path)
            plotter.close()
            return path
        if mode == "notebook":
            return plotter.show(jupyter_backend=jupyter_backend or "trame",
                                return_viewer=True)
        if mode == "window":
            # Force the desktop window even when called from a Jupyter kernel.
            return plotter.show(jupyter_backend="none")
        # auto: let PyVista's environment / global backend decide (trame widget
        # in Jupyter, desktop window otherwise).
        return plotter.show(jupyter_backend=jupyter_backend)
