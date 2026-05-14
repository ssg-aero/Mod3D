

#include <BRepBuilderAPI_MakeFace.hxx>
#include <Geom_Surface.hxx>
#include <Precision.hxx>
#include <Standard_Handle.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <BRepTools.hxx>
// #include <BRepBuilderAPI_MakeShell.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <ShapeAnalysis_FreeBounds.hxx>

#include <TopoDS_Solid.hxx>
#include <list>
#include <tuple>

// Enum shape mapping to OCCT types
template <TopAbs_ShapeEnum E>
struct ShapeTypeMap {};
template <>
struct ShapeTypeMap<TopAbs_FACE> { using type = TopoDS_Face; };
template <>
struct ShapeTypeMap<TopAbs_EDGE> { using type = TopoDS_Edge; };
template <>
struct ShapeTypeMap<TopAbs_VERTEX> { using type = TopoDS_Vertex; };
template <>
struct ShapeTypeMap<TopAbs_SOLID> { using type = TopoDS_Solid; };

template <TopAbs_ShapeEnum E>
auto get_shapes_of_type(const TopoDS_Shape& shape) {
    std::list<typename ShapeTypeMap<E>::type> shapes;
    for (TopExp_Explorer exp(shape, E); exp.More(); exp.Next())
    {
        if constexpr (E == TopAbs_FACE) {
            TopoDS_Face s = TopoDS::Face(exp.Current());
            shapes.push_back(s);
        } else if constexpr (E == TopAbs_EDGE) {
            TopoDS_Edge s = TopoDS::Edge(exp.Current());
            shapes.push_back(s);
        } else if constexpr (E == TopAbs_VERTEX) {
            TopoDS_Vertex s = TopoDS::Vertex(exp.Current());
            shapes.push_back(s);
        } else if constexpr (E == TopAbs_SOLID) {
            TopoDS_Solid s = TopoDS::Solid(exp.Current());
            shapes.push_back(s);
        }
    }
    return shapes;
}


TopoDS_Shape union_faces(const TopoDS_Shape& shape)
{

    std::list<TopoDS_Face> faces = get_shapes_of_type<TopAbs_FACE>(shape);
    
    // Build edge-to-face mapping using OCCT's indexed map
    TopTools_IndexedDataMapOfShapeListOfShape edge_to_faces;
    TopExp::MapShapesAndAncestors(shape, TopAbs_EDGE, TopAbs_FACE, edge_to_faces);
    
    // Find adjacent faces with same geometry.
    // Remove them from the list of faces to process. 
    // Store them in a separate list for merging.
    std::list<std::tuple<TopoDS_Face, TopoDS_Face, TopoDS_Edge, Handle(Geom_Surface)>> adjacent_faces;
    TopTools_IndexedMapOfShape faces_to_remove;  // Track faces to remove after loop
    
    for (Standard_Integer i = 1; i <= edge_to_faces.Extent(); ++i) {
        const TopoDS_Edge& edge = TopoDS::Edge(edge_to_faces.FindKey(i));
        const TopTools_ListOfShape& face_list = edge_to_faces(i);
        
        // Convert to vector for easier iteration
        std::vector<TopoDS_Face> face_vec;
        for (TopTools_ListIteratorOfListOfShape it(face_list); it.More(); it.Next()) {
            face_vec.push_back(TopoDS::Face(it.Value()));
        }
        
        for (size_t j = 0; j < face_vec.size(); ++j) {
            for (size_t k = j + 1; k < face_vec.size(); ++k) {
                const TopoDS_Face& face1 = face_vec[j];
                const TopoDS_Face& face2 = face_vec[k];
                
                Handle(Geom_Surface) surf1 = BRep_Tool::Surface(face1);
                Handle(Geom_Surface) surf2 = BRep_Tool::Surface(face2);
                
                // Check if face1 and face2 have the same underlying surface object
                // AND they are not the same face
                if (!face1.IsSame(face2) && surf1.get() == surf2.get()) {
                    // Check if this pair was already added (faces might share multiple edges)
                    bool already_added = false;
                    for (const auto& [f1, f2, e, s] : adjacent_faces) {
                        if ((f1.IsSame(face1) && f2.IsSame(face2)) || 
                            (f1.IsSame(face2) && f2.IsSame(face1))) {
                            already_added = true;
                            break;
                        }
                    }
                    
                    if (!already_added) {
                        adjacent_faces.emplace_back(face1, face2, edge, surf1);
                        faces_to_remove.Add(face1);
                        faces_to_remove.Add(face2);
                    }
                }
            }
        }
    }
    
    // Remove faces that will be merged
    for (Standard_Integer i = 1; i <= faces_to_remove.Extent(); ++i) {
        faces.remove(TopoDS::Face(faces_to_remove(i)));
    }
    // Replace adjacent faces with a single face and build the resulting shell
    for(auto& [face1, face2, edge, surface] : adjacent_faces) {
        // get face1 and face2's outer wires
        TopoDS_Wire outer_wire1 = BRepTools::OuterWire(face1);
        TopoDS_Wire outer_wire2 = BRepTools::OuterWire(face2);
        // Build new outer wire by merging the two outer wires and removing the common edge
        Handle(TopTools_HSequenceOfShape) edges = new TopTools_HSequenceOfShape();
        Handle(TopTools_HSequenceOfShape) wires = new TopTools_HSequenceOfShape();
        for (TopExp_Explorer exp(outer_wire1, TopAbs_EDGE); exp.More(); exp.Next()) {
            const TopoDS_Edge& e = TopoDS::Edge(exp.Current());
            if (!e.IsSame(edge)) {
                edges->Append(e);
            }
        }
        for (TopExp_Explorer exp(outer_wire2, TopAbs_EDGE); exp.More(); exp.Next()) {
            const TopoDS_Edge& e = TopoDS::Edge(exp.Current());
            if (!e.IsSame(edge)) {
                edges->Append(e);
            }
        }
        ShapeAnalysis_FreeBounds::ConnectEdgesToWires(edges, Precision::Approximation(), true, wires);
        if(wires->Length() != 1) {
            // throw std::runtime_error("Failed to merge wires for union operation. Expected 1 wire, got " + std::to_string(wires->Length()));
        }
        TopoDS_Wire new_outer_wire = TopoDS::Wire(wires->Value(1));
        BRepBuilderAPI_MakeFace face_builder(surface, new_outer_wire);
        
        // get all inner wires if any for each face
        // std::vector<TopoDS_Wire> wires1, wires2;
        for (TopExp_Explorer exp(face1, TopAbs_WIRE); exp.More(); exp.Next()) {
            auto wire1 = TopoDS::Wire(exp.Current());
            if(!wire1.IsSame(outer_wire1))
                // wires1.push_back(wire1);
                face_builder.Add(wire1);
        }
        for (TopExp_Explorer exp(face2, TopAbs_WIRE); exp.More(); exp.Next()) {
            auto wire2 = TopoDS::Wire(exp.Current());
            if(!wire2.IsSame(outer_wire2))
                // wires2.push_back(wire2);
                face_builder.Add(wire2);
        }
        if (!face_builder.IsDone()) {
            throw std::runtime_error("Failed to build merged face for union operation");
        }
        TopoDS_Face merged_face = face_builder.Face();
        faces.push_back(merged_face);
    }

    BRepBuilderAPI_Sewing shell_maker;
    for(const auto& face : faces) {
        shell_maker.Add(face);
    }
    shell_maker.Perform();
    TopoDS_Shape result = shell_maker.SewedShape();
        
    return result;
    // return shape; // Placeholder until we implement the actual union logic
}

