#ifndef MODEL_MATH_H_
#define MODEL_MATH_H_

#include <cmath>
#include <vector>
#include <Precision.hxx>
#include "ModelFace.h"
#include "ModelEdge.h"

namespace Fxt {

  /*
  *   Prints a vertex
  */
  void print(gp_Pnt vertex);

  /**
  *   Rounds of the value to 2 decimal places to avoid error in floating point values
  *   such as getting 89.999999999999999999999999 instead of 90.
  */
  Standard_Real roundd(long double value);

  /**
  *   Computes the unit normal vector of a Face.
  */
  gp_Dir computeUnitNormal(TopoDS_Face face);
  gp_Dir computeNormal(std::vector<ModelEdge> modelEdges);
  gp_Pnt computeLineUnitVector(gp_Pnt lineVector);

  /**
  *   Computes the curvature of a ModelFace and ModelBend.
  */
  template<typename T>
  Standard_Real computeCurvature(T face)
  {
    BRepAdaptor_Surface surface = BRepAdaptor_Surface(face);
    double u = (surface.FirstUParameter() + surface.LastUParameter()) / 2.0;
    double v = (surface.FirstVParameter() + surface.LastVParameter()) / 2.0;

    BRepLProp_SLProps surfaceProps(surface, u, v, 2, gp::Resolution());

    return surfaceProps.MeanCurvature();
  }

  /**
  *   Computes the equation of a plane.
  *   @param modelEdges are the edges of a plane (ModelFace) and its normal.
  */
  Standard_Real computePlaneEquation(std::vector<ModelEdge> modelEdges, gp_Pnt faceNormal);

  /**
  *   Tests whether two vertices v1 and v2 are equal.
  */
  bool isEqual(gp_Pnt aVertex, gp_Pnt bVertex);

  Standard_Real computeEuclideanNorm(gp_Pnt *vertex);
  gp_Pnt computeCrossProduct(gp_Pnt *aVector, gp_Pnt *bVector);
  Standard_Real computeDotProduct(gp_Pnt *aVertex, gp_Pnt *bVertex);

  /*
  * Computes the directional vector of an edge.
  */
  gp_Pnt computeLineVector(ModelEdge edge);

  /*
  *   Computes the length of an edge.
  */
  Standard_Real computeLength(ModelEdge& edge);

  /*
  *   Computes the thickness of the entire model.
  *   @param face is a face with FaceType::THICKNESS_DEFINING_FACE attribute.
  */
  Standard_Real computeThickness(ModelFace face);

  /*
  *   Computes the angle between two faces.
  *   @param n1 and n2 are normals of the two faces.
  */
  Standard_Real computeAngle(gp_Pnt aFaceNormal, gp_Pnt bFaceNormal);

  /*
  *   Computer the midpoint of 2 points
  * */
  gp_Pnt computeMidPoint(gp_Pnt p1, gp_Pnt p2);

}

#endif // MODEL_MATH_H_
