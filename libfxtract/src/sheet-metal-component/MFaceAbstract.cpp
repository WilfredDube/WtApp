#include  "../../include/sheet-metal-component/MFaceAbstract.h"

#include "../../include/Computation.h"
#include "../../include/sheet-metal-component/edge/ModelEdge.h"


using namespace Fxt::SheetMetalComponent;

MFaceAbstract::MFaceAbstract(const Fxt::SheetMetalComponent::FaceID faceID, std::shared_ptr<TopoDS_Face> topoDSFace)
{
    setFaceId(faceID);
    setTModelFace(topoDSFace);
}

void MFaceAbstract::setTModelFace(std::shared_ptr<TopoDS_Face>& tface) 
{ 
    mModelFace = tface;
}

std::shared_ptr<TopoDS_Face> MFaceAbstract::getTModelFace() const 
{ 
    return mModelFace; 
}

void MFaceAbstract::setFaceId(const FaceID faceID)
{ 
    mFaceID = faceID; 
}

FaceID MFaceAbstract::getFaceId() const 
{ 
    return mFaceID; 
}

void MFaceAbstract::setUnitNormal(std::shared_ptr<gp_Dir> unit_normal)
{ 
    mFaceUnitNormal = unit_normal; 
}

std::shared_ptr<gp_Dir> MFaceAbstract::getUnitNormal() const 
{ 
    return mFaceUnitNormal; 
}

void MFaceAbstract::computeFaceNormal()
{
    mFaceNormal = computeNormal(mFaceEdges);
}

std::shared_ptr<gp_Dir> MFaceAbstract::getFaceNormal() const 
{ 
    return mFaceNormal; 
}

void MFaceAbstract::setFaceType(const FaceType ftype) 
{ 
    mFaceType = ftype; 
}

FaceType MFaceAbstract::getFaceType() const 
{ 
    return mFaceType; 
}

void MFaceAbstract::setPlaneType(PlaneType ptype) 
{ 
    mPlaneType = ptype; 
}

PlaneType MFaceAbstract::getPlaneType() const 
{ 
    return mPlaneType; 
}

void MFaceAbstract::extractEdges()
{
    EdgeID edgeid = 0;

    for (TopExp_Explorer edgeEx((*mModelFace), TopAbs_EDGE); edgeEx.More(); edgeEx.Next())
    {
      std::shared_ptr<TopoDS_Edge> edge { std::make_shared<TopoDS_Edge>(TopoDS::Edge(edgeEx.Current())) };
      ++edgeid;

      std::shared_ptr<Edge::ModelEdge> edgex { std::make_shared<Edge::ModelEdge>(edge) };
      edgex->setEdgeNum(edgeid);

      if (getPlaneType() == PlaneType::PLANAR) {
        if (edgex->getEdgeType() == EdgeType::ARC) {
          edgex->setEdgePosition(EdgePosition::SIDE_EDGE);
          setFaceType(FaceType::BEND_SIDE);
          setFaceId(0);
        }
      }

      if (getPlaneType() == PlaneType::NON_PLANAR) {
        if (edgex->getEdgeType() == EdgeType::LINE) {
          edgex->setEdgePosition(EdgePosition::JOINING_EDGE);
        } else {
          edgex->setEdgePosition(EdgePosition::SIDE_EDGE);
        }
      }

      addEdge(edgex);
    }
}

void MFaceAbstract::setFaceEdgePosition()
{
    // set edge positions
    for (auto& a : mFaceEdges)
    {
      if(a->getEdgePosition() == EdgePosition::JOINING_EDGE) {
        for (auto& b : mFaceEdges) {
          if(b->getEdgePosition() != EdgePosition::JOINING_EDGE) {
            auto angle = round(a->getEdgeLineVector()->Angle(*(b->getEdgeLineVector())) * (180.0 / M_PI));
            // std::cout << "Angle : " << angle << std::endl;

            if (angle == 0.0 || angle == 180){
              b->setEdgePosition(EdgePosition::DISJOINT_EDGE);
            } else if(angle == 90.0) {
              b->setEdgePosition(EdgePosition::SIDE_EDGE);
            } else {
              b->setEdgePosition(EdgePosition::SIDE_EDGE);
            }
          }

        }
      }
    } 
}

void MFaceAbstract::addEdge(const std::shared_ptr<Edge::ModelEdge>& edge)
{
    mFaceEdges.push_back(edge);
}

std::vector<std::shared_ptr<Edge::ModelEdge>>& MFaceAbstract::getFaceEdges()
{
    return mFaceEdges;
}

std::shared_ptr<Edge::ModelEdge> MFaceAbstract::getFaceEdgesAt(const int index) const
{
    return mFaceEdges.at(index);
}