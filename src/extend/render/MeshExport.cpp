#include "extend/render/MeshExport.hpp"

#include <BRepMesh_IncrementalMesh.hxx>
#include <StlAPI_Writer.hxx>
#include <RWObj_CafWriter.hxx>
#include <RWGltf_CafWriter.hxx>

#include <TDocStd_Document.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <TCollection_AsciiString.hxx>
#include <NCollection_IndexedDataMap.hxx>
#include <Message_ProgressRange.hxx>

#include <stdexcept>

namespace occt::extended::render {

namespace {

using FileInfo =
    NCollection_IndexedDataMap<TCollection_AsciiString, TCollection_AsciiString>;

void ensure_meshed(const TopoDS_Shape& shape, double deflection) {
    if (deflection <= 0.0) {
        throw std::invalid_argument("deflection must be > 0");
    }
    // Triangulates the shape in place (required by the OBJ/glTF/STL writers).
    BRepMesh_IncrementalMesh mesher(shape, deflection);
    mesher.Perform();
}

// A minimal XCAF document holding the (already meshed) shape, as required by
// the CAF-based OBJ / glTF writers.
Handle(TDocStd_Document) make_caf_document(const TopoDS_Shape& shape) {
    Handle(TDocStd_Document) doc = new TDocStd_Document("BinXCAF");
    Handle(XCAFDoc_ShapeTool) tool =
        XCAFDoc_DocumentTool::ShapeTool(doc->Main());
    tool->AddShape(shape);
    return doc;
}

} // namespace

void export_stl(const TopoDS_Shape& shape, const std::string& path,
                double deflection, bool ascii) {
    ensure_meshed(shape, deflection);
    StlAPI_Writer writer;
    writer.ASCIIMode() = ascii;
    if (!writer.Write(shape, path.c_str())) {
        throw std::runtime_error("Failed to write STL file: " + path);
    }
}

void export_obj(const TopoDS_Shape& shape, const std::string& path,
                double deflection) {
    ensure_meshed(shape, deflection);
    Handle(TDocStd_Document) doc = make_caf_document(shape);
    RWObj_CafWriter writer(path.c_str());
    FileInfo file_info;
    if (!writer.Perform(doc, file_info, Message_ProgressRange())) {
        throw std::runtime_error("Failed to write OBJ file: " + path);
    }
}

void export_gltf(const TopoDS_Shape& shape, const std::string& path,
                 double deflection, bool binary) {
    ensure_meshed(shape, deflection);
    Handle(TDocStd_Document) doc = make_caf_document(shape);
    RWGltf_CafWriter writer(path.c_str(), binary);
    FileInfo file_info;
    if (!writer.Perform(doc, file_info, Message_ProgressRange())) {
        throw std::runtime_error("Failed to write glTF file: " + path);
    }
}

} // namespace occt::extended::render
