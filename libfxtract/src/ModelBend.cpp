#include "../include/ModelBend.h"

namespace Fxt {

  std::ostream & operator<<(std::ostream &os, const ModelBend &mb)
  {
    os << std::setprecision(3);
    // os << mb.mBendAngle << " " << mb.mBendDirection << " " << (ceilf(mb.bendLine_.Location().X() * 100) / 100) 
    //    << " " << (ceilf(mb.bendLine_.Location().Y() * 100) / 100) << " " << (ceilf(mb.bendLine_.Location().Z() * 100) / 100);

    // os << (ceilf(mb.bendLine_.Direction().X() * 100) / 100) << " " << (ceilf(mb.bendLine_.Direction().Y() * 100) / 100) << " " 
    //     << (ceilf(mb.bendLine_.Direction().Z() * 100) / 100);
    // os << '\n';
    os << mb.mBendAngle << " " << mb.mBendDirection << " " << mb.bendLine_.Location().X() << " " << mb.bendLine_.Location().Y() << " " << mb.bendLine_.Location().Z();
    os << mb.bendLine_.Direction().X() << " " << mb.bendLine_.Direction().Y() << " " << mb.bendLine_.Direction().Z();
    os << '\n';
     
    return os;
  }

  // ModelBend::ModelBend(const FaceID& faceID, TopoDS_Face& topoDSFace)
  ModelBend::ModelBend(const FaceID& faceID, TopoDS_Face topoDSFace)
  {
    if(topoDSFace != (TopoDS_Face){})
    {
      setTModelFace(topoDSFace);
      setPlaneType(PlaneType::NON_PLANAR);
      setFaceType(FaceType::BEND_FACE);
      setJoiningFaceID1(0);
      setJoiningFaceID2(0);
      setBendAngle(0);

      init();
    }
    
    setFaceId(faceID);
  }

  void ModelBend::init()
  {
    extractEdges();

    Standard_Real curvature = computeCurvature<TopoDS_Face>(getTModelFace());
    setCurvature(curvature);

    computeFaceNormal();
    setUnitNormal(computeUnitNormal(getTModelFace()));

    setBendLength(getFaceEdges());

    computeBendLine();
  }

  void ModelBend::setJoiningFaceID1(FaceID mFaceID)
  {
    mJoinedFaceId1 = mFaceID;
  }

  FaceID ModelBend::getJoiningFaceID1() { return mJoinedFaceId1; }

  void ModelBend::setJoiningFaceID2(FaceID mFaceID)
  {
    mJoinedFaceId2 = mFaceID;
  }

  FaceID ModelBend::getJoiningFaceID2()
  {
    return mJoinedFaceId2;
  }

  void ModelBend::setBendAngle(long double angle)
  {
    mBendAngle = angle;
  }

  long double ModelBend::getBendAngle()
  {
    return mBendAngle;
  }

  void ModelBend::setBendDirection(size_t bend_direction)
  {
    mBendDirection = bend_direction;
  }

  size_t ModelBend::getBendDirection()
  {
    return mBendDirection;
  }

  Standard_Real ModelBend::getBendLength()
  {
    return mBendLength;
  }

  void ModelBend::computeBendLine()
  {
    std::vector<gp_Pnt> endPoints;

    for(auto& edge :  getFaceEdges()) {
       if (edge.getEdgeType() == EdgeType::ARC)
       {  
         endPoints.push_back(computeMidPoint(edge.getEdgeEndPoints()[0], edge.getEdgeEndPoints()[1]));
       }       
    }

    gp_Pnt dirVertex(
      endPoints[0].X() - endPoints[1].X(),
      endPoints[0].Y() - endPoints[1].Y(),
      endPoints[0].Z() - endPoints[1].Z()
    );


    bendLine_ = gp_Lin(endPoints[0], gp_Dir(dirVertex.X(), dirVertex.Y(), dirVertex.Z()));

    bendLinePnt.X = endPoints[0].X();//bendLine_.Location().X();
    bendLinePnt.Y = endPoints[0].Y();//bendLine_.Location().Y();
    bendLinePnt.Z = endPoints[0].Z();//bendLine_.Location().Z();

    bendLineDir.X = dirVertex.X();
    bendLineDir.Y = dirVertex.Y();
    bendLineDir.Z = dirVertex.Z();
  }

  gp_Lin ModelBend::getBendLine() { return bendLine_; }

  void ModelBend::makeBendLine(){
    bendLine_ = gp_Lin(gp_Pnt( bendLinePnt.X, bendLinePnt.Y, bendLinePnt.Z), gp_Dir(bendLineDir.X, bendLineDir.Y, bendLineDir.Z));
  }

  void ModelBend::setBendLength(std::vector<ModelEdge> mFaceEdges)
  {
    size_t i;
    for (i = 0; i < mFaceEdges.size(); i++) {
      if (mFaceEdges[i].getEdgeType() == EdgeType::LINE) {
        mBendLength = mFaceEdges[i].getEdgeLength();
        break;
      }
    }
  }

  /**
  * a normal member taking one argument to set the curvature attribute of the face. If the curvature != 0 then
  * the face is a bend face and so its radius is also set. *
  * @param cv a real/floating point.
  */
  void ModelBend::setCurvature(Standard_Real cv)
  {
    mCurvature = cv;

    if (mCurvature != 0.0) {
      // Divide by 2 to get radius else you get the diameter.
      mBendRadius = roundd(((mCurvature < 0.0) ? -(1 / mCurvature) : (1 / mCurvature)) / 2);
    }
  }

  void ModelBend::setBendRadius(Standard_Real bendRadius)
  {
    mBendRadius = bendRadius;
  }

  Standard_Real ModelBend::getBendRadius(){ return mBendRadius; }

  double ModelBend::computeBendDistance(ModelBend& otherBend)
  {
    return bendLine_.Distance(otherBend.getBendLine());
  }

  bool ModelBend::isParallel(ModelBend& otherBend)
  {
    auto angle = roundd(getBendLine().Angle(otherBend.getBendLine()) * (180 / M_PI));

    return (angle == 0.0 || angle == 180);
  }
    
}