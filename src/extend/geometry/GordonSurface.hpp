#pragma once

#include <Standard_Version.hxx>

#if OCC_VERSION_HEX >= 0x080000

#include <Geom_BSplineSurface.hxx>
#include <Geom_Curve.hxx>
#include <Standard_Handle.hxx>

#include <vector>

namespace occt::extended::geometry {

//! Build a Gordon surface from a network of intersecting profile and guide
//! curves (transfinite interpolation via the Boolean-sum formula).
//!
//! Composes GeomFill_Gordon's Init/Perform/IsDone steps and raises
//! std::runtime_error with a clear message when construction fails, instead
//! of returning a null handle.
//!
//! @param profiles  >= 2 curves running in the V direction
//! @param guides    >= 2 curves running in the U direction
//! @param tolerance geometric tolerance for intersection detection
//! @param parallel  enable OCCT's internal parallel processing
//! @return the resulting B-spline surface (never null)
//!
//! Available only when built against OCCT >= 8.0.
Handle(Geom_BSplineSurface) gordon_surface(
    const std::vector<Handle(Geom_Curve)>& profiles,
    const std::vector<Handle(Geom_Curve)>& guides,
    double tolerance,
    bool parallel = false);

} // namespace occt::extended::geometry

#endif // OCC_VERSION_HEX >= 0x080000
