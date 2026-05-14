#include "extend/GbsConverters.hpp"

#if HAS_GBS

#include <TColStd_Array1OfInteger.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array2OfReal.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <gp_Pnt.hxx>

namespace occt::extended::gbs {

opencascade::handle<Geom_BSplineCurve>
to_occt(const ::gbs::BSCurve<double, 3>& curve) {
    const auto poles_ = curve.poles();
    TColgp_Array1OfPnt poles(1, static_cast<Standard_Integer>(poles_.size()));
    for (size_t i = 0; i < poles_.size(); ++i) {
        poles.SetValue(static_cast<Standard_Integer>(i + 1),
                       gp_Pnt(poles_[i][0], poles_[i][1], poles_[i][2]));
    }

    auto [knots, mults] = ::gbs::knots_and_mults(curve.knotsFlats());
    TColStd_Array1OfReal occt_knots(1, static_cast<Standard_Integer>(knots.size()));
    TColStd_Array1OfInteger occt_mults(1, static_cast<Standard_Integer>(mults.size()));
    for (size_t i = 0; i < knots.size(); ++i) {
        occt_knots.SetValue(static_cast<Standard_Integer>(i + 1), knots[i]);
        occt_mults.SetValue(static_cast<Standard_Integer>(i + 1),
                            static_cast<Standard_Integer>(mults[i]));
    }

    return new Geom_BSplineCurve(poles, occt_knots, occt_mults, curve.degree());
}

opencascade::handle<Geom_BSplineCurve>
to_occt(const ::gbs::BSCurveRational<double, 3>& curve) {
    const auto poles_ = curve.polesProjected();
    TColgp_Array1OfPnt poles(1, static_cast<Standard_Integer>(poles_.size()));
    for (size_t i = 0; i < poles_.size(); ++i) {
        poles.SetValue(static_cast<Standard_Integer>(i + 1),
                       gp_Pnt(poles_[i][0], poles_[i][1], poles_[i][2]));
    }

    const auto weights_ = curve.weights();
    TColStd_Array1OfReal weights(1, static_cast<Standard_Integer>(weights_.size()));
    for (size_t i = 0; i < weights_.size(); ++i) {
        weights.SetValue(static_cast<Standard_Integer>(i + 1), weights_[i]);
    }

    auto [knots, mults] = ::gbs::knots_and_mults(curve.knotsFlats());
    TColStd_Array1OfReal occt_knots(1, static_cast<Standard_Integer>(knots.size()));
    TColStd_Array1OfInteger occt_mults(1, static_cast<Standard_Integer>(mults.size()));
    for (size_t i = 0; i < knots.size(); ++i) {
        occt_knots.SetValue(static_cast<Standard_Integer>(i + 1), knots[i]);
        occt_mults.SetValue(static_cast<Standard_Integer>(i + 1),
                            static_cast<Standard_Integer>(mults[i]));
    }

    return new Geom_BSplineCurve(poles, weights, occt_knots, occt_mults, curve.degree());
}

opencascade::handle<Geom_BSplineSurface>
to_occt(const ::gbs::BSSurface<double, 3>& surface) {
    const auto& poles_ = surface.poles();
    const size_t nu = surface.nPolesU();
    const size_t nv = surface.nPolesV();
    const auto u_size = static_cast<Standard_Integer>(nu);
    const auto v_size = static_cast<Standard_Integer>(nv);

    TColgp_Array2OfPnt poles(1, u_size, 1, v_size);
    for (size_t i = 0; i < nu; ++i) {
        for (size_t j = 0; j < nv; ++j) {
            const size_t idx = i + j * nu;
            const auto& pt = poles_[idx];
            poles.SetValue(static_cast<Standard_Integer>(i + 1),
                           static_cast<Standard_Integer>(j + 1),
                           gp_Pnt(pt[0], pt[1], pt[2]));
        }
    }

    auto [knots_u, mults_u] = ::gbs::knots_and_mults(surface.knotsFlatsU());
    auto [knots_v, mults_v] = ::gbs::knots_and_mults(surface.knotsFlatsV());

    TColStd_Array1OfReal occt_knots_u(1, static_cast<Standard_Integer>(knots_u.size()));
    TColStd_Array1OfInteger occt_mults_u(1, static_cast<Standard_Integer>(mults_u.size()));
    for (size_t i = 0; i < knots_u.size(); ++i) {
        occt_knots_u.SetValue(static_cast<Standard_Integer>(i + 1), knots_u[i]);
        occt_mults_u.SetValue(static_cast<Standard_Integer>(i + 1),
                              static_cast<Standard_Integer>(mults_u[i]));
    }

    TColStd_Array1OfReal occt_knots_v(1, static_cast<Standard_Integer>(knots_v.size()));
    TColStd_Array1OfInteger occt_mults_v(1, static_cast<Standard_Integer>(mults_v.size()));
    for (size_t i = 0; i < knots_v.size(); ++i) {
        occt_knots_v.SetValue(static_cast<Standard_Integer>(i + 1), knots_v[i]);
        occt_mults_v.SetValue(static_cast<Standard_Integer>(i + 1),
                              static_cast<Standard_Integer>(mults_v[i]));
    }

    return new Geom_BSplineSurface(poles, occt_knots_u, occt_knots_v,
                                   occt_mults_u, occt_mults_v,
                                   surface.degreeU(), surface.degreeV());
}

opencascade::handle<Geom_BSplineSurface>
to_occt(const ::gbs::BSSurfaceRational<double, 3>& surface) {
    const auto projected_poles = surface.polesProjected();
    const size_t nu = surface.nPolesU();
    const size_t nv = surface.nPolesV();
    const auto u_size = static_cast<Standard_Integer>(nu);
    const auto v_size = static_cast<Standard_Integer>(nv);

    TColgp_Array2OfPnt poles(1, u_size, 1, v_size);
    for (size_t i = 0; i < nu; ++i) {
        for (size_t j = 0; j < nv; ++j) {
            const size_t idx = i + j * nu;
            const auto& pt = projected_poles[idx];
            poles.SetValue(static_cast<Standard_Integer>(i + 1),
                           static_cast<Standard_Integer>(j + 1),
                           gp_Pnt(pt[0], pt[1], pt[2]));
        }
    }

    const auto& weights_ = surface.weights();
    TColStd_Array2OfReal weights(1, u_size, 1, v_size);
    for (size_t i = 0; i < nu; ++i) {
        for (size_t j = 0; j < nv; ++j) {
            const size_t idx = i + j * nu;
            weights.SetValue(static_cast<Standard_Integer>(i + 1),
                             static_cast<Standard_Integer>(j + 1),
                             weights_[idx]);
        }
    }

    auto [knots_u, mults_u] = ::gbs::knots_and_mults(surface.knotsFlatsU());
    auto [knots_v, mults_v] = ::gbs::knots_and_mults(surface.knotsFlatsV());

    TColStd_Array1OfReal occt_knots_u(1, static_cast<Standard_Integer>(knots_u.size()));
    TColStd_Array1OfInteger occt_mults_u(1, static_cast<Standard_Integer>(mults_u.size()));
    for (size_t i = 0; i < knots_u.size(); ++i) {
        occt_knots_u.SetValue(static_cast<Standard_Integer>(i + 1), knots_u[i]);
        occt_mults_u.SetValue(static_cast<Standard_Integer>(i + 1),
                              static_cast<Standard_Integer>(mults_u[i]));
    }

    TColStd_Array1OfReal occt_knots_v(1, static_cast<Standard_Integer>(knots_v.size()));
    TColStd_Array1OfInteger occt_mults_v(1, static_cast<Standard_Integer>(mults_v.size()));
    for (size_t i = 0; i < knots_v.size(); ++i) {
        occt_knots_v.SetValue(static_cast<Standard_Integer>(i + 1), knots_v[i]);
        occt_mults_v.SetValue(static_cast<Standard_Integer>(i + 1),
                              static_cast<Standard_Integer>(mults_v[i]));
    }

    return new Geom_BSplineSurface(poles, weights, occt_knots_u, occt_knots_v,
                                   occt_mults_u, occt_mults_v,
                                   surface.degreeU(), surface.degreeV());
}

} // namespace occt::extended::gbs

#endif // HAS_GBS