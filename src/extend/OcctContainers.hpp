#pragma once

#include <TColStd_SequenceOfAsciiString.hxx>
#include <TopoDS_Shape.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopTools_ListOfShape.hxx>

#include <string>
#include <vector>

namespace occt::extended::containers {

inline std::vector<TopoDS_Shape> to_vector(const TopTools_ListOfShape& shapes) {
    std::vector<TopoDS_Shape> result;
    result.reserve(static_cast<size_t>(shapes.Size()));
    for (TopTools_ListIteratorOfListOfShape it(shapes); it.More(); it.Next()) {
        result.push_back(it.Value());
    }
    return result;
}

inline std::vector<std::string> to_vector(const TColStd_SequenceOfAsciiString& sequence) {
    std::vector<std::string> result;
    result.reserve(static_cast<size_t>(sequence.Size()));
    for (Standard_Integer i = 1; i <= sequence.Size(); ++i) {
        result.push_back(sequence.Value(i).ToCString());
    }
    return result;
}

} // namespace occt::extended::containers