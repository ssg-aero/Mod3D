#pragma once

#include <BRepExtrema_DistShapeShape.hxx>
#include <BRepExtrema_ExtCC.hxx>
#include <BRepExtrema_ExtCF.hxx>
#include <BRepExtrema_ExtFF.hxx>
#include <BRepExtrema_ExtPC.hxx>
#include <BRepExtrema_ExtPF.hxx>
#include <Standard_Real.hxx>
#include <gp_Pnt.hxx>

#include <vector>

namespace occt::extended::extrema {

struct ShapeShapePointDistance {
    gp_Pnt point_on_shape1;
    gp_Pnt point_on_shape2;
    Standard_Real distance;
};

struct FaceParameters {
    Standard_Real u;
    Standard_Real v;
};

struct ExtPCTrimmedSquareDistances {
    Standard_Real first_distance;
    Standard_Real last_distance;
    gp_Pnt first_point;
    gp_Pnt last_point;
};

struct EdgeParameterPoint {
    Standard_Real parameter;
    gp_Pnt point;
};

struct EdgeEdgeExtremaResult {
    EdgeParameterPoint edge1;
    EdgeParameterPoint edge2;
    Standard_Real square_distance;
};

struct ExtCCTrimmedSquareDistances {
    Standard_Real first_first_distance;
    Standard_Real first_last_distance;
    Standard_Real last_first_distance;
    Standard_Real last_last_distance;
    gp_Pnt first_first_point;
    gp_Pnt first_last_point;
    gp_Pnt last_first_point;
    gp_Pnt last_last_point;
};

struct EdgeFaceExtremaResult {
    EdgeParameterPoint edge;
    FaceParameters face_parameters;
    gp_Pnt face_point;
    Standard_Real square_distance;
};

struct FacePointParameters {
    FaceParameters parameters;
    gp_Pnt point;
};

struct FaceFaceExtremaResult {
    FacePointParameters face1;
    FacePointParameters face2;
    Standard_Real square_distance;
};

std::vector<ShapeShapePointDistance>
points_and_distances(const BRepExtrema_DistShapeShape& extrema);

std::vector<gp_Pnt> points(const BRepExtrema_ExtPC& extrema);

ExtPCTrimmedSquareDistances
trimmed_square_distances(const BRepExtrema_ExtPC& extrema);

std::vector<FaceParameters> parameters(const BRepExtrema_ExtPF& extrema);

std::vector<gp_Pnt> points(const BRepExtrema_ExtPF& extrema);

std::vector<Standard_Real> square_distances(const BRepExtrema_ExtCC& extrema);

std::vector<EdgeEdgeExtremaResult>
parameters_and_points(const BRepExtrema_ExtCC& extrema);

ExtCCTrimmedSquareDistances
trimmed_square_distances(const BRepExtrema_ExtCC& extrema);

std::vector<EdgeFaceExtremaResult>
points_and_parameters(const BRepExtrema_ExtCF& extrema);

std::vector<FaceFaceExtremaResult>
points_and_parameters(const BRepExtrema_ExtFF& extrema);

} // namespace occt::extended::extrema