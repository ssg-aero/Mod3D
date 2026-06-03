#pragma once

#include <TopoDS_Shape.hxx>

#include <string>

namespace occt::extended::render {

//! Mesh `shape` (via BRepMesh_IncrementalMesh) and write it as STL.
//! @param deflection linear deflection of the triangulation
//! @param ascii      write ASCII STL instead of binary
//! Raises std::runtime_error if writing fails.
void export_stl(const TopoDS_Shape& shape, const std::string& path,
                double deflection = 0.1, bool ascii = false);

//! Mesh `shape` and write it as Wavefront OBJ (+ .mtl) via RWObj_CafWriter.
//! Raises std::runtime_error if writing fails.
void export_obj(const TopoDS_Shape& shape, const std::string& path,
                double deflection = 0.1);

//! Mesh `shape` and write it as glTF via RWGltf_CafWriter.
//! @param binary write a binary .glb (otherwise a text .gltf + .bin)
//! Raises std::runtime_error if writing fails.
void export_gltf(const TopoDS_Shape& shape, const std::string& path,
                 double deflection = 0.1, bool binary = false);

} // namespace occt::extended::render
