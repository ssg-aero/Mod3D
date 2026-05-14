#include <BRepAdaptor_Surface.hxx>
#include <GeomAbs_SurfaceType.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_BezierSurface.hxx>
#include <gp_Pln.hxx>
#include <gp_Cylinder.hxx>

bool same_surface_geometry(const TopoDS_Face& f1, const TopoDS_Face& f2, Standard_Real tol = 1e-7) {
    BRepAdaptor_Surface surf1(f1);
    BRepAdaptor_Surface surf2(f2);
    
    // Different surface type = not the same surface
    if (surf1.GetType() != surf2.GetType()) return false;
    
    switch (surf1.GetType()) {
        case GeomAbs_Plane: {
            gp_Pln pln1 = surf1.Plane();
            gp_Pln pln2 = surf2.Plane();
            // Same normal (or opposite) and coplanar
            if (!pln1.Axis().IsParallel(pln2.Axis(), tol)) return false;
            return pln1.Distance(pln2.Location()) < tol;
        }
        case GeomAbs_Cylinder: {
            gp_Cylinder cyl1 = surf1.Cylinder();
            gp_Cylinder cyl2 = surf2.Cylinder();
            // Same axis and same radius
            if (std::abs(cyl1.Radius() - cyl2.Radius()) > tol) return false;
            return cyl1.Axis().IsCoaxial(cyl2.Axis(), tol, tol);
        }
        case GeomAbs_Sphere: {
            gp_Sphere sph1 = surf1.Sphere();
            gp_Sphere sph2 = surf2.Sphere();
            if (std::abs(sph1.Radius() - sph2.Radius()) > tol) return false;
            return sph1.Location().Distance(sph2.Location()) < tol;
        }
        case GeomAbs_Cone: {
            gp_Cone cone1 = surf1.Cone();
            gp_Cone cone2 = surf2.Cone();
            if (std::abs(cone1.SemiAngle() - cone2.SemiAngle()) > tol) return false;
            return cone1.Axis().IsCoaxial(cone2.Axis(), tol, tol);
        }
        case GeomAbs_Torus: {
            gp_Torus tor1 = surf1.Torus();
            gp_Torus tor2 = surf2.Torus();
            if (std::abs(tor1.MajorRadius() - tor2.MajorRadius()) > tol) return false;
            if (std::abs(tor1.MinorRadius() - tor2.MinorRadius()) > tol) return false;
            return tor1.Axis().IsCoaxial(tor2.Axis(), tol, tol);
        }
        case GeomAbs_BSplineSurface: {
            Handle(Geom_BSplineSurface) bs1 = surf1.BSpline();
            Handle(Geom_BSplineSurface) bs2 = surf2.BSpline();
            
            // Compare degrees
            if (bs1->UDegree() != bs2->UDegree() || bs1->VDegree() != bs2->VDegree()) return false;
            
            // Compare number of poles
            if (bs1->NbUPoles() != bs2->NbUPoles() || bs1->NbVPoles() != bs2->NbVPoles()) return false;
            
            // Compare number of knots
            if (bs1->NbUKnots() != bs2->NbUKnots() || bs1->NbVKnots() != bs2->NbVKnots()) return false;
            
            // Compare poles
            for (int i = 1; i <= bs1->NbUPoles(); ++i) {
                for (int j = 1; j <= bs1->NbVPoles(); ++j) {
                    if (bs1->Pole(i, j).Distance(bs2->Pole(i, j)) > tol) return false;
                }
            }
            
            // Compare U knots
            for (int i = 1; i <= bs1->NbUKnots(); ++i) {
                if (std::abs(bs1->UKnot(i) - bs2->UKnot(i)) > tol) return false;
                if (bs1->UMultiplicity(i) != bs2->UMultiplicity(i)) return false;
            }
            
            // Compare V knots
            for (int i = 1; i <= bs1->NbVKnots(); ++i) {
                if (std::abs(bs1->VKnot(i) - bs2->VKnot(i)) > tol) return false;
                if (bs1->VMultiplicity(i) != bs2->VMultiplicity(i)) return false;
            }
            
            // Compare weights if rational
            if (bs1->IsURational() != bs2->IsURational()) return false;
            if (bs1->IsVRational() != bs2->IsVRational()) return false;
            if (bs1->IsURational() || bs1->IsVRational()) {
                for (int i = 1; i <= bs1->NbUPoles(); ++i) {
                    for (int j = 1; j <= bs1->NbVPoles(); ++j) {
                        if (std::abs(bs1->Weight(i, j) - bs2->Weight(i, j)) > tol) return false;
                    }
                }
            }
            return true;
        }

        case GeomAbs_BezierSurface: {
            Handle(Geom_BezierSurface) bz1 = surf1.Bezier();
            Handle(Geom_BezierSurface) bz2 = surf2.Bezier();
            
            // Compare degrees
            if (bz1->UDegree() != bz2->UDegree() || bz1->VDegree() != bz2->VDegree()) return false;
            
            // Compare poles
            for (int i = 1; i <= bz1->NbUPoles(); ++i) {
                for (int j = 1; j <= bz1->NbVPoles(); ++j) {
                    if (bz1->Pole(i, j).Distance(bz2->Pole(i, j)) > tol) return false;
                }
            }
            
            // Compare weights if rational
            if (bz1->IsURational() != bz2->IsURational()) return false;
            if (bz1->IsVRational() != bz2->IsVRational()) return false;
            if (bz1->IsURational() || bz1->IsVRational()) {
                for (int i = 1; i <= bz1->NbUPoles(); ++i) {
                    for (int j = 1; j <= bz1->NbVPoles(); ++j) {
                        if (std::abs(bz1->Weight(i, j) - bz2->Weight(i, j)) > tol) return false;
                    }
                }
            }
            return true;
        }
        default:
            // Unsupported surface type - throw error
            // return BRep_Tool::Surface(f1).get() == BRep_Tool::Surface(f2).get();
            throw std::runtime_error("Unsupported surface type for geometry comparison");
    }
}