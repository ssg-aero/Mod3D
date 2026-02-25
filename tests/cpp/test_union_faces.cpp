#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <STEPControl_Reader.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <TopExp_Explorer.hxx>
#include <BRepTools.hxx>

#include "heal.hxx"

// Helper to count faces in a shape
int count_faces(const TopoDS_Shape& shape) {
    int count = 0;
    for (TopExp_Explorer exp(shape, TopAbs_FACE); exp.More(); exp.Next()) {
        count++;
    }
    return count;
}

// Helper to load a solid from STEP file
TopoDS_Solid load_solid_from_step(const char* filename) {
    STEPControl_Reader reader;
    IFSelect_ReturnStatus status = reader.ReadFile(filename);
    REQUIRE(status == IFSelect_RetDone);
    
    reader.TransferRoots();
    TopoDS_Shape shape = reader.OneShape();
    
    for (TopExp_Explorer exp(shape, TopAbs_SOLID); exp.More(); exp.Next()) {
        return TopoDS::Solid(exp.Current());
    }
    return TopoDS_Solid();
}

TEST_CASE("union_faces on assembly.step") {
    const char* step_file = "C:\\Users\\sebas\\workspace\\lib_maintenance\\studies\\out.step";
    
    SUBCASE("loads file and finds solid") {
        TopoDS_Solid solid = load_solid_from_step(step_file);
        CHECK_FALSE(solid.IsNull());
    }
    
    SUBCASE("union_faces executes without crash") {
        TopoDS_Solid solid = load_solid_from_step(step_file);
        REQUIRE_FALSE(solid.IsNull());
        
        int faces_before = count_faces(solid);
        MESSAGE("Faces before: ", faces_before);
        
        TopoDS_Shape result = union_faces(solid);
        
        int faces_after = count_faces(result);
        MESSAGE("Faces after: ", faces_after);
        
        CHECK_FALSE(result.IsNull());
        // Optionally write result for inspection
        BRepTools::Write(result, "result.brep");
    }
}
