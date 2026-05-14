#pragma once

#if HAS_GBS

#include <Standard_Handle.hxx>

#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>

#include <gbs/bscurve.h>
#include <gbs/bssurf.h>

namespace occt::extended::gbs {

opencascade::handle<Geom_BSplineCurve>
to_occt(const ::gbs::BSCurve<double, 3>& curve);

opencascade::handle<Geom_BSplineCurve>
to_occt(const ::gbs::BSCurveRational<double, 3>& curve);

opencascade::handle<Geom_BSplineSurface>
to_occt(const ::gbs::BSSurface<double, 3>& surface);

opencascade::handle<Geom_BSplineSurface>
to_occt(const ::gbs::BSSurfaceRational<double, 3>& surface);

} // namespace occt::extended::gbs

#endif // HAS_GBS