#include "extend/geometry/GordonSurface.hpp"

#if OCC_VERSION_HEX >= 0x080000

#include <GeomFill_Gordon.hxx>
#include <NCollection_Array1.hxx>

#include <stdexcept>

namespace occt::extended::geometry {

namespace {

NCollection_Array1<Handle(Geom_Curve)>
to_array(const std::vector<Handle(Geom_Curve)>& curves, const char* name)
{
    if (curves.size() < 2) {
        throw std::invalid_argument(std::string(name) +
                                    " must contain at least 2 curves");
    }
    NCollection_Array1<Handle(Geom_Curve)> array(1, static_cast<int>(curves.size()));
    for (std::size_t i = 0; i < curves.size(); ++i) {
        if (curves[i].IsNull()) {
            throw std::invalid_argument(std::string(name) +
                                        " contains a null curve");
        }
        array.SetValue(static_cast<int>(i) + 1, curves[i]);
    }
    return array;
}

} // namespace

Handle(Geom_BSplineSurface) gordon_surface(
    const std::vector<Handle(Geom_Curve)>& profiles,
    const std::vector<Handle(Geom_Curve)>& guides,
    double tolerance,
    bool parallel)
{
    GeomFill_Gordon gordon;
    gordon.SetParallelMode(parallel);
    gordon.Init(to_array(profiles, "profiles"), to_array(guides, "guides"),
                tolerance);
    gordon.Perform();
    if (!gordon.IsDone()) {
        throw std::runtime_error(
            "Gordon surface construction failed: the curve network is not "
            "compatible (check that every profile intersects every guide "
            "within the given tolerance, and that curves are non-rational)");
    }
    return gordon.Surface();
}

} // namespace occt::extended::geometry

#endif // OCC_VERSION_HEX >= 0x080000
