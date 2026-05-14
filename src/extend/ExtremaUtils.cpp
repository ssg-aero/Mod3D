#include "extend/ExtremaUtils.hpp"

namespace occt::extended::extrema {

std::vector<ShapeShapePointDistance>
points_and_distances(const BRepExtrema_DistShapeShape& extrema) {
    std::vector<ShapeShapePointDistance> results;
    const Standard_Integer count = extrema.NbSolution();
    results.reserve(static_cast<size_t>(count));
    for (Standard_Integer i = 1; i <= count; ++i) {
        const gp_Pnt point_on_shape1 = extrema.PointOnShape1(i);
        const gp_Pnt point_on_shape2 = extrema.PointOnShape2(i);
        results.push_back({point_on_shape1, point_on_shape2, point_on_shape1.Distance(point_on_shape2)});
    }
    return results;
}

std::vector<gp_Pnt> points(const BRepExtrema_ExtPC& extrema) {
    std::vector<gp_Pnt> results;
    const Standard_Integer count = extrema.NbExt();
    results.reserve(static_cast<size_t>(count));
    for (Standard_Integer i = 1; i <= count; ++i) {
        results.push_back(extrema.Point(i));
    }
    return results;
}

ExtPCTrimmedSquareDistances
trimmed_square_distances(const BRepExtrema_ExtPC& extrema) {
    ExtPCTrimmedSquareDistances result;
    extrema.TrimmedSquareDistances(result.first_distance,
                                   result.last_distance,
                                   result.first_point,
                                   result.last_point);
    return result;
}

std::vector<FaceParameters> parameters(const BRepExtrema_ExtPF& extrema) {
    std::vector<FaceParameters> results;
    const Standard_Integer count = extrema.NbExt();
    results.reserve(static_cast<size_t>(count));
    for (Standard_Integer i = 1; i <= count; ++i) {
        FaceParameters params;
        extrema.Parameter(i, params.u, params.v);
        results.push_back(params);
    }
    return results;
}

std::vector<gp_Pnt> points(const BRepExtrema_ExtPF& extrema) {
    std::vector<gp_Pnt> results;
    const Standard_Integer count = extrema.NbExt();
    results.reserve(static_cast<size_t>(count));
    for (Standard_Integer i = 1; i <= count; ++i) {
        results.push_back(extrema.Point(i));
    }
    return results;
}

std::vector<Standard_Real> square_distances(const BRepExtrema_ExtCC& extrema) {
    std::vector<Standard_Real> results;
    const Standard_Integer count = extrema.NbExt();
    results.reserve(static_cast<size_t>(count));
    for (Standard_Integer i = 1; i <= count; ++i) {
        results.push_back(extrema.SquareDistance(i));
    }
    return results;
}

std::vector<EdgeEdgeExtremaResult>
parameters_and_points(const BRepExtrema_ExtCC& extrema) {
    std::vector<EdgeEdgeExtremaResult> results;
    const Standard_Integer count = extrema.NbExt();
    results.reserve(static_cast<size_t>(count));
    for (Standard_Integer i = 1; i <= count; ++i) {
        results.push_back({
            {extrema.ParameterOnE1(i), extrema.PointOnE1(i)},
            {extrema.ParameterOnE2(i), extrema.PointOnE2(i)},
            extrema.SquareDistance(i),
        });
    }
    return results;
}

ExtCCTrimmedSquareDistances
trimmed_square_distances(const BRepExtrema_ExtCC& extrema) {
    ExtCCTrimmedSquareDistances result;
    extrema.TrimmedSquareDistances(result.first_first_distance,
                                   result.first_last_distance,
                                   result.last_first_distance,
                                   result.last_last_distance,
                                   result.first_first_point,
                                   result.first_last_point,
                                   result.last_first_point,
                                   result.last_last_point);
    return result;
}

std::vector<EdgeFaceExtremaResult>
points_and_parameters(const BRepExtrema_ExtCF& extrema) {
    std::vector<EdgeFaceExtremaResult> results;
    const Standard_Integer count = extrema.NbExt();
    results.reserve(static_cast<size_t>(count));
    for (Standard_Integer i = 1; i <= count; ++i) {
        FaceParameters face_parameters;
        extrema.ParameterOnFace(i, face_parameters.u, face_parameters.v);
        results.push_back({
            {extrema.ParameterOnEdge(i), extrema.PointOnEdge(i)},
            face_parameters,
            extrema.PointOnFace(i),
            extrema.SquareDistance(i),
        });
    }
    return results;
}

std::vector<FaceFaceExtremaResult>
points_and_parameters(const BRepExtrema_ExtFF& extrema) {
    std::vector<FaceFaceExtremaResult> results;
    const Standard_Integer count = extrema.NbExt();
    results.reserve(static_cast<size_t>(count));
    for (Standard_Integer i = 1; i <= count; ++i) {
        FacePointParameters face1;
        FacePointParameters face2;
        extrema.ParameterOnFace1(i, face1.parameters.u, face1.parameters.v);
        extrema.ParameterOnFace2(i, face2.parameters.u, face2.parameters.v);
        face1.point = extrema.PointOnFace1(i);
        face2.point = extrema.PointOnFace2(i);
        results.push_back({face1, face2, extrema.SquareDistance(i)});
    }
    return results;
}

} // namespace occt::extended::extrema