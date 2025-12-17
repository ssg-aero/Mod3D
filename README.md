# Mod3D Python Bindings

This workspace packages a lightweight Python extension that exposes a few helper utilities built on top of Open Cascade Technology (OCCT) via `pybind11`. It is designed as a starting point for embedding OCCT modeling capabilities inside a Python workflow or unit tests.

## Prerequisites

- **CMake 3.20+** with a C++17-capable toolchain.
- **Python 3.10+** interpreter and development headers (the extension builds against the interpreter that CMake discovers).
- **Open Cascade Technology** (7.6+ recommended). Make sure the `OpenCascade_DIR` cache variable or environment points to the installation's CMake config directory (for example `C:/Program Files/OpenCASCADE/lib/cmake/OpenCASCADE`).

## Build

```powershell
mkdir -p build
cmake -S . -B build -DOpenCascade_DIR="C:/Program Files/OpenCASCADE/lib/cmake/OpenCASCADE"
cmake --build build --config Release
```

If you are on Linux or macOS, adapt the generator and toolchain flags accordingly (`-G Ninja`, `-DCMAKE_BUILD_TYPE=RelWithDebInfo`, etc.).

## Running

After a successful build the `mod3d` package (with the `pymod3d` extension submodule) is placed into the `build` directory and eventually installs into the interpreter that CMake found. Point the interpreter at the submodule directly:

```powershell
python -c "from mod3d.pymod3d import create_box_summary; print(create_box_summary(25.0, 15.0, 10.0))"
python - <<'PY'
from mod3d.pymod3d import create_box_summary

summary = create_box_summary(25.0, 15.0, 10.0)
print(summary)
PY
```

## Example script

Create `examples/create_box.py` (included in this repo) and run it once the module is built:

```python
from mod3d.pymod3d import create_box_summary

if __name__ == "__main__":
    summary = create_box_summary(40.0, 20.0, 15.0)
    print("Box summary:", summary)
```

## Notes

- `pybind11` is retrieved at configure time via `FetchContent`; no extra dependency is required as long as the machine has an internet connection.
- The build intentionally sets `CMAKE_POSITION_INDEPENDENT_CODE` so the resulting module can be linked into larger Python extension packages or embedded interpreters.
- If you need to customize the OCCT search paths, pass `-DOpenCascade_DIR` or set `OPEN_CASCADE_DIR` before configuring.
