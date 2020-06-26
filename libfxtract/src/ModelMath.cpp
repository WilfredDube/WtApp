#include "../include/ModelMath.h"

#include <stdexcept>
#include <BRepTools.hxx>
#include <GeomLProp_SLProps.hxx>

namespace Fxt {

  Standard_Real roundd(long double value)
  {
    char str[40];

    sprintf(str, "%.2Lf", value);
    sscanf(str, "%Lf", &value);

    return value;
  }

  gp_Dir computeUnitNormal(TopoDS_Face face)
  {
    Standard_Real umin, umax, vmin, vmax;
    BRepTools::UVBounds(face, umin, umax, vmin, vmax); // create surface
    Handle(Geom_Surface) surf=BRep_Tool::Surface(face); // get surface properties
    GeomLProp_SLProps props(surf, umin, vmin, 1, 0.01); // get surface normal
    gp_Dir norm=props.Normal(); // check orientation

    norm = gp_Dir(roundd(norm.X()), roundd(norm.Y()), roundd(norm.Z()));

    return norm;
  }

  gp_Dir computeNormal(std::vector<ModelEdge> modelEdges)
  {
    ModelEdge aEdge, bEdge;

    aEdge = modelEdges[0];
    for (size_t i = 0; i < modelEdges.size(); i++) {
      if (aEdge.getEdgeNum() == modelEdges[i].getEdgeNum()) {
        continue;
      }

      // Compare edge end points if at least one is equal to one in anEdge
      // then retrieve that edge.
      if (isEqual(aEdge.getEdgeEndPoints()[0], (modelEdges[i]).getEdgeEndPoints()[0]) ||
      isEqual(aEdge.getEdgeEndPoints()[1], (modelEdges[i]).getEdgeEndPoints()[0]) ||
      isEqual(aEdge.getEdgeEndPoints()[1], (modelEdges[i]).getEdgeEndPoints()[1]) ||
      isEqual(aEdge.getEdgeEndPoints()[0], (modelEdges[i]).getEdgeEndPoints()[1])) {
        bEdge = modelEdges[i];
        break;
      }
    }

    gp_Pnt aVertex = computeLineVector(aEdge);
    gp_Pnt bVertex = computeLineVector(bEdge);

    aVertex = computeCrossProduct(&aVertex, &bVertex);

    return gp_Dir(aVertex.X(), aVertex.Y(), aVertex.Z());
  }

  void print(gp_Pnt vertex) {
    std::cout << " X : " << vertex.X() <<
                  " Y : " << vertex.Y() <<
                  " Z : " << vertex.Z() <<
                  '\n';
  }

  gp_Pnt computeLineVector(ModelEdge edge)
  {
    gp_Pnt vertex(
      edge.getEdgeEndPoints()[0].X() - edge.getEdgeEndPoints()[1].X(),
      edge.getEdgeEndPoints()[0].Y() - edge.getEdgeEndPoints()[1].Y(),
      edge.getEdgeEndPoints()[0].Z() - edge.getEdgeEndPoints()[1].Z()
    );

    return vertex;
  }

  gp_Pnt computeLineUnitVector(gp_Pnt lineVector)
  {
    Standard_Real magnitude = computeEuclideanNorm(&lineVector);

    gp_Pnt vt (
      lineVector.X() / magnitude,
      lineVector.Y() / magnitude,
      lineVector.Z() / magnitude
    );

    return vt;
  }

  Standard_Real computePlaneEquation(std::vector<ModelEdge> modelEdges, gp_Pnt faceNormal)
  {
    Standard_Real d1, d2;
    size_t count = modelEdges.size();

    for (size_t i = 0; i < count; i++) {
      ModelEdge edge = modelEdges[i];

      d1 = (faceNormal.X() * edge.getEdgeEndPoints()[0].X()) + (faceNormal.Y() * edge.getEdgeEndPoints()[0].Y()) + (faceNormal.Z() * edge.getEdgeEndPoints()[0].Z());
      d2 = (faceNormal.X() * edge.getEdgeEndPoints()[1].X()) + (faceNormal.Y() * edge.getEdgeEndPoints()[1].Y()) + (faceNormal.Z() * edge.getEdgeEndPoints()[1].Z());

      if (fabsl(d1 - d2) < (Standard_Real)0.00001) {
        return -d1;
      }

    }

    throw std::logic_error("Could not create equation of the plane.");
  }

  bool isEqual(gp_Pnt aVertex, gp_Pnt bVertex)
  {
    bool truth = false;

    if (aVertex.IsEqual(bVertex, Precision::Confusion()))
    {
      truth = true;
    }

    return truth;
  }

  Standard_Real computeEuclideanNorm(gp_Pnt *vertex)
  {
    gp_XYZ v(vertex->X(), vertex->Y(), vertex->Z());

    return v.Modulus();
  }

  gp_Pnt computeCrossProduct(gp_Pnt *aVector, gp_Pnt *bVector)
  {
    Standard_Real X = (aVector->Y() * bVector->Z()) - (aVector->Z() * bVector->Y());
    Standard_Real Y = - ((aVector->X() * bVector->Z()) - (aVector->Z() * bVector->X()));
    Standard_Real Z = (aVector->X() * bVector->Y()) - (aVector->Y() * bVector->X());

    gp_Pnt normal(X, Y, Z);
    return normal;
  }

  Standard_Real computeDotProduct(gp_Pnt *aVertex, gp_Pnt *bVertex)
  {
    Standard_Real A = aVertex->X() * bVertex->X();
    Standard_Real B = aVertex->Y() * bVertex->Y();
    Standard_Real C = aVertex->Z() * bVertex->Z();

    return (A + B + C);
  }

  Standard_Real computeLength(ModelEdge& edge)
  {
    gp_Pnt vertex(
      edge.getEdgeEndPoints()[0].X() - edge.getEdgeEndPoints()[1].X(),
      edge.getEdgeEndPoints()[0].Y() - edge.getEdgeEndPoints()[1].Y(),
      edge.getEdgeEndPoints()[0].Z() - edge.getEdgeEndPoints()[1].Z()
    );

    return roundd(computeEuclideanNorm(&vertex));
  }

  Standard_Real computeThickness(ModelFace face)
  {
    // size_t count = face.getFaceEdges().size();

    // smallest (thickness) represent the length of the smallest side of a
    // THICKNESS_DEFINING_FACE.
    Standard_Real smallest = face.getFaceEdges()[0].getEdgeLength();

    for (auto& edge : face.getFaceEdges()) {
      if (smallest > edge.getEdgeLength()) {
        smallest = edge.getEdgeLength();
      }
    }

    return smallest;
  }

  Standard_Real computeAngle(gp_Pnt aFaceNormal, gp_Pnt bFaceNormal)
  {
    Standard_Real cosine, angle;
    Standard_Real dotProduct = computeDotProduct(&aFaceNormal, &bFaceNormal);

    Standard_Real aEuclideanNorm = computeEuclideanNorm(&aFaceNormal);
    Standard_Real bEuclideanNorm = computeEuclideanNorm(&bFaceNormal);

    if (dotProduct == 0) {
      cosine = 0;
    } else {
      cosine = dotProduct / (aEuclideanNorm * bEuclideanNorm);
    }

    Standard_Real value = 180.0 / M_PI;

    // convert the angle from radian to degrees.
    angle = acos(roundd(cosine)) * value;

    return round(angle);
  }


  gp_Pnt computeMidPoint(gp_Pnt p1, gp_Pnt p2)
  {
    double x, y, z;         

    x = roundd(0.5 * (p1.X() + p2.X()));
    y = roundd(0.5 * (p1.Y() + p2.Y()));
    z = roundd(0.5 * (p1.Z() + p2.Z()));

    return gp_Pnt(x, y, z);
  }

}
