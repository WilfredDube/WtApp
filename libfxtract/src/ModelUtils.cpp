#include "../include/ModelUtils.h"

namespace Fxt {

  void printShapeType(TopAbs_ShapeEnum shapetype)
  {
    switch (shapetype) {
      case TopAbs_COMPOUND : std::cout << "TopAbs_COMPOUND" << '\n';
      break;
      case TopAbs_COMPSOLID : std::cout << "TopAbs_COMPSOLID" << '\n';
      break;
      case TopAbs_SOLID : std::cout << "TopAbs_SOLID" << '\n';
      break;
      default:
      std::cout << "NOTHING" << '\n';
    }
  }

  void printType(GeomAbs_CurveType curvetype)
  {
    switch (curvetype) {
      case GeomAbs_Line:std::cout << "GeomAbs_Line" << '\n';
      break;
      case GeomAbs_Circle:std::cout << "GeomAbs_Circle" << '\n';
      break;
      case GeomAbs_BSplineCurve:std::cout << "GeomAbs_BsplineCurve" << '\n';
      break;
      default:
      std::cout << "NOTHING" << '\n';
    }
  }

  void printPrecision() {
    Standard_Real rp = Interface_Static::RVal("read.precision.val");
    Standard_Integer ic = Interface_Static::IVal("read.precision.mode");
    Standard_Integer mv = Interface_Static::IVal("read.maxprecision.mode");

    std::cout << "Precision value : " << rp << '\n';
    std::cout << "Precision mode : " << ic << '\n';
    std::cout << "Max precision mode : " << mv << '\n';
  }

}
