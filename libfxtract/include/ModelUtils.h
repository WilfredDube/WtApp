#ifndef MODEL_UTILS_H_
#define MODEL_UTILS_H_

#include <GeomAbs_CurveType.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopoDS.hxx>
#include <Interface_Static.hxx>

namespace Fxt {

  //! Face identifier
  using FaceID = size_t;

  enum class PlaneType { PLANAR, NON_PLANAR };
  enum class FaceType { NONE, THICKNESS_DEFINING_FACE, BEND_FACE, BEND_SIDE, FACE };
  enum class BendType { UNASSIGNED = -1, INTERNAL, EXTERNAL };

  void printType(GeomAbs_CurveType curvetype);
  void printShapeType(TopAbs_ShapeEnum shapetype);
  void printPrecision();

}

#endif // MODEL_UTILS_H_
