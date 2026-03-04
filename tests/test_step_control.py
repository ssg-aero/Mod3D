"""
Test suite for STEP I/O bindings (STEPControl_Reader / STEPControl_Writer).

Tests cover:
- Reader: reading a STEP file, transferring roots, retrieving shapes
- Reader: unit query, shape count, one_shape
- Writer: creating a box and writing to STEP, then re-reading it
- Enum values for StepModelType and ReturnStatus
"""

import os
import tempfile
import pytest
from mod3d import StepControl, BRepBuilderAPI


# Path to sample STEP files shipped with the repo
STEP_DIR = os.path.join(os.path.dirname(__file__), "..", "studies")
SIMPLE_STEP = os.path.join(STEP_DIR, "filet2d.step")
ASSEMBLY_STEP = os.path.join(STEP_DIR, "assembly.step")


# ---------------------------------------------------------------------------
# Enum smoke-tests
# ---------------------------------------------------------------------------

class TestEnums:
    def test_return_status_values(self):
        assert StepControl.ReturnStatus.RetDone  is not None
        assert StepControl.ReturnStatus.RetError is not None
        assert StepControl.ReturnStatus.RetFail  is not None
        assert StepControl.ReturnStatus.RetVoid  is not None
        assert StepControl.ReturnStatus.RetStop  is not None

    def test_step_model_type_values(self):
        assert StepControl.StepModelType.AsIs                      is not None
        assert StepControl.StepModelType.ManifoldSolidBrep         is not None
        assert StepControl.StepModelType.FacetedBrep               is not None
        assert StepControl.StepModelType.ShellBasedSurfaceModel    is not None
        assert StepControl.StepModelType.GeometricCurveSet         is not None


# ---------------------------------------------------------------------------
# Reader tests
# ---------------------------------------------------------------------------

class TestReader:
    def test_constructor(self):
        reader = StepControl.Reader()
        assert reader is not None

    @pytest.mark.skipif(not os.path.exists(SIMPLE_STEP), reason="STEP file not found")
    def test_read_file_returns_done(self):
        reader = StepControl.Reader()
        status = reader.read_file(SIMPLE_STEP)
        assert status == StepControl.ReturnStatus.RetDone

    @pytest.mark.skipif(not os.path.exists(SIMPLE_STEP), reason="STEP file not found")
    def test_nb_roots_for_transfer(self):
        reader = StepControl.Reader()
        reader.read_file(SIMPLE_STEP)
        n = reader.nb_roots_for_transfer
        assert n >= 1

    @pytest.mark.skipif(not os.path.exists(SIMPLE_STEP), reason="STEP file not found")
    def test_transfer_roots(self):
        reader = StepControl.Reader()
        reader.read_file(SIMPLE_STEP)
        n_transferred = reader.transfer_roots()
        assert n_transferred >= 1
        assert reader.nb_shapes >= 1

    @pytest.mark.skipif(not os.path.exists(SIMPLE_STEP), reason="STEP file not found")
    def test_one_shape_not_null(self):
        reader = StepControl.Reader()
        reader.read_file(SIMPLE_STEP)
        reader.transfer_roots()
        shape = reader.one_shape
        assert not shape.is_null()

    @pytest.mark.skipif(not os.path.exists(SIMPLE_STEP), reason="STEP file not found")
    def test_shape_by_rank(self):
        reader = StepControl.Reader()
        reader.read_file(SIMPLE_STEP)
        reader.transfer_roots()
        n = reader.nb_shapes
        if n >= 1:
            s = reader.shape(1)
            assert not s.is_null()

    @pytest.mark.skipif(not os.path.exists(SIMPLE_STEP), reason="STEP file not found")
    def test_clear_shapes(self):
        reader = StepControl.Reader()
        reader.read_file(SIMPLE_STEP)
        reader.transfer_roots()
        assert reader.nb_shapes >= 1
        reader.clear_shapes()
        assert reader.nb_shapes == 0

    @pytest.mark.skipif(not os.path.exists(SIMPLE_STEP), reason="STEP file not found")
    def test_file_units_returns_tuple_of_lists(self):
        reader = StepControl.Reader()
        reader.read_file(SIMPLE_STEP)
        result = reader.file_units
        assert isinstance(result, tuple)
        assert len(result) == 3
        length_units, angle_units, solid_units = result
        assert isinstance(length_units, list)
        assert isinstance(angle_units, list)
        assert isinstance(solid_units, list)
        # STEP files typically declare at least one length unit
        assert len(length_units) >= 1

    @pytest.mark.skipif(not os.path.exists(SIMPLE_STEP), reason="STEP file not found")
    def test_system_length_unit(self):
        reader = StepControl.Reader()
        reader.read_file(SIMPLE_STEP)
        unit = reader.system_length_unit()
        assert isinstance(unit, float)

    @pytest.mark.skipif(not os.path.exists(SIMPLE_STEP), reason="STEP file not found")
    def test_set_system_length_unit(self):
        reader = StepControl.Reader()
        reader.read_file(SIMPLE_STEP)
        reader.set_system_length_unit(0.001)
        unit = reader.system_length_unit()
        assert abs(unit - 0.001) < 1e-10

    @pytest.mark.skipif(not os.path.exists(SIMPLE_STEP), reason="STEP file not found")
    def test_step_model_not_none_after_read(self):
        reader = StepControl.Reader()
        reader.read_file(SIMPLE_STEP)
        model = reader.step_model
        assert model is not None

    def test_read_nonexistent_file_fails(self):
        reader = StepControl.Reader()
        status = reader.read_file("/nonexistent/path/to/file.step")
        assert status != StepControl.ReturnStatus.RetDone


# ---------------------------------------------------------------------------
# Writer tests
# ---------------------------------------------------------------------------

class TestWriter:
    def test_constructor(self):
        writer = StepControl.Writer()
        assert writer is not None

    def test_transfer_box_asis(self):
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        writer = StepControl.Writer()
        status = writer.transfer(box, StepControl.StepModelType.AsIs)
        assert status == StepControl.ReturnStatus.RetDone

    def test_write_to_temp_file(self):
        box = BRepBuilderAPI.MakeBox(10.0, 10.0, 10.0).shape()
        writer = StepControl.Writer()
        writer.transfer(box, StepControl.StepModelType.AsIs)

        with tempfile.NamedTemporaryFile(suffix=".step", delete=False) as f:
            path = f.name

        try:
            status = writer.write(path)
            assert status == StepControl.ReturnStatus.RetDone
            assert os.path.getsize(path) > 0
        finally:
            os.unlink(path)

    def test_roundtrip_box(self):
        """Write a box to STEP and read it back; result should be non-null."""
        box = BRepBuilderAPI.MakeBox(20.0, 15.0, 10.0).shape()

        with tempfile.NamedTemporaryFile(suffix=".step", delete=False) as f:
            path = f.name

        try:
            writer = StepControl.Writer()
            writer.transfer(box, StepControl.StepModelType.AsIs)
            assert writer.write(path) == StepControl.ReturnStatus.RetDone

            reader = StepControl.Reader()
            assert reader.read_file(path) == StepControl.ReturnStatus.RetDone
            reader.transfer_roots()
            result = reader.one_shape
            assert not result.is_null()
        finally:
            os.unlink(path)

    def test_model_access(self):
        writer = StepControl.Writer()
        model = writer.model
        assert model is not None

    def test_set_and_unset_tolerance(self):
        """Tolerance methods should not raise."""
        writer = StepControl.Writer()
        writer.set_tolerance(0.01)
        writer.unset_tolerance()
