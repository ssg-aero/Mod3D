#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>

#include <gp_Trsf.hxx>
#include <gp_Ax1.hxx>

namespace occt{
    namespace extended {
        namespace shapes {

            class Shape : public TopoDS_Shape
            {
            public:
                Shape() : TopoDS_Shape() {}
                Shape(const TopoDS_Shape& shape) : TopoDS_Shape(shape) {}
                Shape(const TopoDS_Face& face) : TopoDS_Shape(face) {}
                Shape(const TopoDS_Wire& wire) : TopoDS_Shape(wire) {}
                Shape(const TopoDS_Edge& edge) : TopoDS_Shape(edge) {}
                Shape(const TopoDS_Vertex& vertex) : TopoDS_Shape(vertex) {}

                void rotate(const gp_Ax1 &theAxis, double theAngle) {
                    gp_Trsf trsf;
                    trsf.SetRotation(theAxis, theAngle);
                    this->Move(TopLoc_Location(trsf));
                }
            };

        }
    
    }
};