"""PyVista rendering of a rotationally-periodic blade row vs. an endoscope.

End-to-end demo tying together three Mod3D pieces:

- ``make_periodic_compound`` builds a turbine blade row from a single master
  blade — all N copies share one TShape (one tessellation, N placements);
- ``BRepExtrema.MeshDistance`` reports the minimum clearance between the moving
  endoscope and the fixed blade row;
- ``PyVistaRenderer`` draws the scene with geometry instancing (one GPU upload
  for the N blades) and can emit a desktop window, a Jupyter widget, or a
  self-contained interactive HTML file for sharing.

Run it::

    python examples/vista.py --mode window
    python examples/vista.py --mode html --output blade_row.html
    python examples/vista.py --sectors 36 --mode window

In a notebook, call ``build_scene(...)`` then ``renderer.render(mode="notebook")``.
"""
from __future__ import annotations

import argparse

from mod3d import BRepBuilderAPI, BRepExtrema, gp
from mod3d.periodic import make_periodic_compound
from mod3d.render.pyvista import PyVistaRenderer


def _z_axis():
    return gp.Ax1(gp.Pnt(0.0, 0.0, 0.0), gp.Dir(0.0, 0.0, 1.0))


def make_blade():
    """A single master blade (a box offset from the axis stands in for one)."""
    return BRepBuilderAPI.MakeBox(gp.Pnt(40.0, -4.0, 0.0), 8.0, 8.0, 30.0).shape()


def make_endoscope(z_offset=12.0):
    """A probe approaching the blade row from inside (endoscope stand-in)."""
    return BRepBuilderAPI.MakeBox(gp.Pnt(20.0, -2.0, z_offset), 16.0, 4.0, 4.0).shape()


def build_scene(sectors=24, deflection=0.4):
    """Build the blade row + endoscope, report clearance, return a renderer.

    Returns the configured :class:`PyVistaRenderer` (nothing is drawn yet) and
    the measured minimum distance, so callers can render in any mode.
    """
    blade_row = make_periodic_compound(make_blade(), _z_axis(), sectors)
    endoscope = make_endoscope()

    # Minimum clearance, BVH-accelerated against the shared blade tessellation.
    query = BRepExtrema.MeshDistance(blade_row, deflection=deflection)
    clearance, _p_row, _p_probe = query.distance_to(endoscope)

    renderer = PyVistaRenderer(linear_deflection=deflection)
    renderer.add_shape(blade_row, {"surface_color": "#2194ce"})
    renderer.add_shape(endoscope, {"color": "#d62728", "face_opacity": 0.6})
    return renderer, clearance


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--sectors", type=int, default=24,
                        help="number of blades in the periodic row")
    parser.add_argument("--mode", choices=("window", "notebook", "html"),
                        default="window", help="render target")
    parser.add_argument("--output", default="blade_row.html",
                        help="output file for --mode html")
    parser.add_argument("--deflection", type=float, default=0.4,
                        help="tessellation linear deflection")
    args = parser.parse_args(argv)

    renderer, clearance = build_scene(args.sectors, args.deflection)
    print(f"{args.sectors} blades; endoscope-to-row clearance ~ {clearance:.3f}")

    if args.mode == "html":
        path = renderer.render(mode="html", path=args.output)
        print(f"wrote interactive scene to {path}")
    else:
        renderer.render(mode=args.mode)


if __name__ == "__main__":
    main()
