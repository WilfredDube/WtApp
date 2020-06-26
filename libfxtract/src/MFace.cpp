#include <TopoDS_Edge.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Adaptor3d_Curve.hxx>
#include <GeomAdaptor_Curve.hxx>
#include <BRepAdaptor_Surface.hxx>
#include <gp_Lin.hxx>
 
#include "../include/MFace.h"
#include "../include/ModelMath.h"

namespace Fxt {

  std::ostream & operator<<(std::ostream &os, const MFace &mf)
  {
      return os << ' ' << (size_t) mf.mFaceID << ' ';
  }

  void MFace::setTModelFace(TopoDS_Face tface)
  {
    mModelFace = tface;
  }

  TopoDS_Face MFace::getTModelFace()
  {
    return mModelFace;
  }

  void MFace::setFaceId(FaceID faceID)
  {
    mFaceID = faceID;
  }

  FaceID MFace::getFaceId() const
  {
    return mFaceID;
  }

  void MFace::setUnitNormal(gp_Dir unit_normal)
  {
    mFaceUnitNormal = unit_normal;
  }

  gp_Dir MFace::getUnitNormal()
  {
    return mFaceUnitNormal;
  }

  gp_Dir MFace::getFaceNormal() const
  {
    return mFaceNormal;
  }

  void MFace::setFaceType(FaceType ftype)
  {
    mFaceType = ftype;
  }

  FaceType MFace::getFaceType(){ return mFaceType; }

  void MFace::setPlaneType(PlaneType ptype)
  {
    mPlaneType = ptype;
  }

  PlaneType MFace::getPlaneType()
  {
    return mPlaneType;
  }

  /**
  * computes and sets the normal vector of the face.
  */
  void MFace::computeFaceNormal()
  {
    mFaceNormal = computeNormal(mFaceEdges);
  }

  void MFace::addEdge(const ModelEdge& edge)
  {
    mFaceEdges.push_back(edge);
  }

  void MFace::extractEdges()
  {
    EdgeID edgeid = 0;

    for (TopExp_Explorer edgeEx((mModelFace), TopAbs_EDGE); edgeEx.More(); edgeEx.Next())
    {
      TopoDS_Edge edge = TopoDS::Edge(edgeEx.Current());
      ++edgeid;

      ModelEdge edgex(edge);
      edgex.setEdgeNum(edgeid);

      if (getPlaneType() == PlaneType::PLANAR) {
        if (edgex.getEdgeType() == EdgeType::ARC) {
          edgex.setEdgePosition(EdgePosition::SIDE_EDGE);
          setFaceType(FaceType::BEND_SIDE);
          setFaceId(0);
        }
      }

      if (getPlaneType() == PlaneType::NON_PLANAR) {
        if (edgex.getEdgeType() == EdgeType::LINE) {
          edgex.setEdgePosition(EdgePosition::JOINING_EDGE);
        } else
        {
          edgex.setEdgePosition(EdgePosition::SIDE_EDGE);
        }
        
      }

      addEdge(edgex);
    }
  }

  void MFace::setFaceEdgePosition()
  {
     // set edge positions
    for (auto& a : mFaceEdges)
    {
      if(a.getEdgePosition() == EdgePosition::JOINING_EDGE) {
        for (auto& b : mFaceEdges) {
          if(b.getEdgePosition() != EdgePosition::JOINING_EDGE) {
            auto angle = round(a.getEdgeLineVector().Angle(b.getEdgeLineVector()) * (180.0 / M_PI));
            // std::cout << "Angle : " << angle << std::endl;

            if (angle == 0.0 || angle == 180){
              b.setEdgePosition(EdgePosition::DISJOINT_EDGE);
            } else if(angle == 90.0) {
              b.setEdgePosition(EdgePosition::SIDE_EDGE);
            } else {
              b.setEdgePosition(EdgePosition::SIDE_EDGE);
            }
          }

        }
      }
    } 
  }

  std::vector<ModelEdge>& MFace::getFaceEdges()
  {
    return mFaceEdges;
  }

  ModelEdge MFace::getFaceEdgesAt(int index) const
  {
    return mFaceEdges.at(index);
  }

  void MFace::getFaceEdgePosition() const
  {
    std::cout << "=========================================================" << std::endl;
    std::cout << "Face ID : " << mFaceID << std::endl;
    std::cout << "Face Type : " << (mFaceType == FaceType::FACE ? "Face" : "other") << std::endl;
    for (auto a : mFaceEdges)
    {
      a.printEdgePosition();
    }    
  }

}
