#ifndef MODEL_UTILS_H_
#define MODEL_UTILS_H_

#include <GeomAbs_CurveType.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopoDS.hxx>
#include <Interface_Static.hxx>

namespace Fxt {

  //! Face identifier
  using FaceID = size_t;

  enum class FileFormat{ UNKNOWN_FORMAT, IGES_FORMAT, STEP_FORMAT };

  enum class BendOrientation { PARALLEL,  PERPENDICULAR };

  enum class EdgeType { DEFAULT = -1, LINE, ARC };

  enum class EdgePosition { DEFAULT = -1, JOINING_EDGE, SIDE_EDGE, DISJOINT_EDGE };

  enum class PlaneType { PLANAR, NON_PLANAR };

  enum class FaceType { NONE, THICKNESS_DEFINING_FACE, BEND_FACE, BEND_SIDE, FACE };
  
  enum class BendType { UNASSIGNED = -1, INTERNAL, EXTERNAL };

  void printType(GeomAbs_CurveType curvetype);
  void printShapeType(TopAbs_ShapeEnum shapetype);
  void printPrecision();

}

#endif // MODEL_UTILS_H_
