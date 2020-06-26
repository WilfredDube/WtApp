#include "../include/ModelEdge.h"
#include "../include/ModelUtils.h"
#include "../include/ModelMath.h"

namespace Fxt {
  ModelEdge::ModelEdge(){}
  ModelEdge::ModelEdge(TopoDS_Edge edge)
  {
    mModelEdge = edge;

    setEdgePosition(EdgePosition::DEFAULT);
    setIsRational(BRepAdaptor_Curve(edge).IsRational());

    BRepAdaptor_Curve curve = BRepAdaptor_Curve(edge);
    setEdgeType(BRepAdaptor_Curve(edge).GetType());

    setEdgeLength(computeLength(*this));
    setEdgeLineVector();
    setJoiningFaceID(0);
  }

  TopoDS_Edge ModelEdge::getTModelEdge() const
  {
    return mModelEdge;
  }

  std::vector<gp_Pnt> ModelEdge::getEdgeEndPoints() const
  {
    std::vector<gp_Pnt> endPoints;

    for (TopExp_Explorer vertexEx(mModelEdge, TopAbs_VERTEX); vertexEx.More(); vertexEx.Next())
    {
      TopoDS_Vertex vertex = TopoDS::Vertex(vertexEx.Current());
      gp_Pnt currentVertex = BRep_Tool::Pnt(vertex);
      endPoints.push_back(currentVertex);
    }

    return endPoints;
  }

  void ModelEdge::setIsRational(bool is_it) { isrational = is_it; }
  bool ModelEdge::IsRational() const { return isrational; }

  void ModelEdge::setEdgeNum(EdgeID enumb){  mEdgeNumber = enumb; }
  EdgeID ModelEdge::getEdgeNum() const { return mEdgeNumber; }

  void ModelEdge::printEdgePosition(){
    switch (mEdgePosition) {
      case EdgePosition::DEFAULT:
        std::cout << "DEFAULT" << std::endl;
      break;
      case EdgePosition::JOINING_EDGE:
        std::cout << "JOINING_EDGE" << std::endl;
      break;
      case EdgePosition::SIDE_EDGE:
        std::cout << "SIDE_EDGE" << std::endl;
      break;
      case EdgePosition::DISJOINT_EDGE:
        std::cout << "DISJOINT_EDGE" << std::endl;
      break;
      default:
        ;
    }
  }

  void ModelEdge::setEdgeType(GeomAbs_CurveType type){
    switch (type) {
      case GeomAbs_Line:
      mEdgeType = EdgeType::LINE;
      break;
      case GeomAbs_Circle:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_Ellipse:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_Hyperbola:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_Parabola:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_BezierCurve:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_BSplineCurve:
      mEdgeType = EdgeType::ARC;
      if (IsRational() != true)
      {
        mEdgeType = EdgeType::LINE;
      }
      break;
      case GeomAbs_OffsetCurve:
      mEdgeType = EdgeType::ARC;
      break;
      case GeomAbs_OtherCurve:
      default:;
      mEdgeType = EdgeType::ARC;
    }
  }

  EdgeType ModelEdge::getEdgeType() const { return mEdgeType; }

  void ModelEdge::setEdgePosition(EdgePosition epos){ mEdgePosition = epos; }
  EdgePosition ModelEdge::getEdgePosition() const { return mEdgePosition; }

  void ModelEdge::setEdgeLength(Standard_Real len) { mEdgeLength = len; }
  Standard_Real ModelEdge::getEdgeLength() const { return  mEdgeLength; }

  void ModelEdge::setEdgeLineVector() 
  { 
    auto point = computeLineVector(*this);
    mEdgeLineVector = gp_Dir(point.X(), point.Y(), point.Z()); 
  }
  gp_Dir ModelEdge::getEdgeLineVector() { return mEdgeLineVector; };

  void ModelEdge::setJoiningFaceID(size_t id) { joinedToFaceID = id; }
  size_t ModelEdge::getJoiningFaceID() { return joinedToFaceID; }

  bool ModelEdge::operator==(ModelEdge const& otherEdge) const
  {
    bool equal = false;
    const gp_Pnt wThisEdge = getEdgeEndPoints()[0];
    const gp_Pnt xThisEdge = getEdgeEndPoints()[1];
    const gp_Pnt yOtherEdge = otherEdge.getEdgeEndPoints()[0];
    const gp_Pnt zOtherEdge = otherEdge.getEdgeEndPoints()[1];    
    
    if ((wThisEdge.IsEqual(yOtherEdge, Precision::Approximation())) && (xThisEdge.IsEqual(zOtherEdge, Precision::Approximation())))
    {
      equal = true;
    } 
    else if ((wThisEdge.IsEqual(zOtherEdge, Precision::Approximation())) && (xThisEdge.IsEqual(yOtherEdge, Precision::Approximation())))
    {
      equal = true;
    }
    
    // if (equal)
    // {
    //   // printEdgePosition(this->getEdgePosition());
    //   std::cout << "( " << wThisEdge.X() << ", " << wThisEdge.Y() << ", " << wThisEdge.Z() << " )" << std::endl;
    //   std::cout << "( " << xThisEdge.X() << ", " << xThisEdge.Y() << ", " << xThisEdge.Z() << " )" << std::endl;
    //   std::cout << "---------" << Precision::Approximation() << std::endl;
    //   // printEdgePosition(otherEdge.getEdgePosition());
    //   std::cout << "( " << yOtherEdge.X() << ", " << yOtherEdge.Y() << ", " << yOtherEdge.Z() << " )" << std::endl;
    //   std::cout << "( " << zOtherEdge.X() << ", " << zOtherEdge.Y() << ", " << zOtherEdge.Z() << " )" << std::endl;
    //   std::cout << "Equal : " << (equal ? "TRUE" : "FALSE") << std::endl;
    //   std::cout << "===============================================================" << std::endl;
    // }

    return equal;
  }

}