# import pytest
from mod3d import (
    gp,
    Geom,
)


def test_geom_line_creation():
    # Create a Geom_Line using a point and direction
    p = gp.Pnt(0.0, 0.0, 0.0)
    d = gp.Dir(1.0, 0.0, 0.0)
    line = Geom.Line(p, d)

    # Verify that the line was created correctly
    lin = line.line
    assert lin.location.xyz == p.xyz
    assert lin.direction.xyz == d.xyz
