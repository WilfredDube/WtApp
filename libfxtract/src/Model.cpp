#include "../include/Model.h"
#include "../include/ModelMath.h"
#include "../include/ModelReader.h"


#include <TopoDS_Shape.hxx>
#include <TopoDS_Face.hxx>
#include <TopTools_ListOfShape.hxx>
#include <TopTools_ShapeMapHasher.hxx>
#include <NCollection_IndexedDataMap.hxx>
#include <TopOpeBRepBuild_Tools.hxx>
#include <TopExp.hxx>

#include <gp_Lin.hxx>

using std::make_unique;

namespace Fxt {

  std::ostream & operator<<(std::ostream &os, const Model &m)
  {
    for(auto it = m.mInnerBendsMap_.begin(); it != m.mInnerBendsMap_.end(); it++){
        os << it->first << it->second;
    }
  
    return os;
  }

  Model::Model(const char* modelFile)
  {
    mModelFile = modelFile;
  }

  void Model::init()
  {
    auto mReader = make_unique<ModelReader>(*this);
    std::cerr << "File name : " << mModelFile << '\n';
    mReader->processModel(mModelFile);
  }

  void Model::addBend(ModelBend modelbend)
  {
    mModelBends.push_back(modelbend);
  }

  std::vector<ModelBend> Model::getBends()
  {
    return mInnerBends_;//mModelBends;
  }

  std::vector<ModelFace> Model::getFaces()
  {
    return mInnerFaces_;//mModelFaces;
  }

  void Model::addFace(ModelFace face)
  {
    mModelFaces.push_back(face);
  }

  void Model::assignFaceAttributes(const FaceID faceID, TopoDS_Shape& aShape)
  {
    mModelShape = aShape;

    TopoDS_Face pTopoDSFace = TopoDS::Face(mModelShape);

    Standard_Real curvature = computeCurvature(pTopoDSFace);

    if (curvature == 0.0){

      ModelFace pModelFace(faceID, pTopoDSFace);

      addFace(pModelFace);

    } else {

      ModelBend pModelFace(faceID, pTopoDSFace);

      addBend(pModelFace);
    }
  }

  bool Model::findFace(FaceID query, std::vector<ModelBend> &BL, std::vector<ModelBend> &innerBends)
  {
    bool found = false;

    for (auto iter = BL.begin(); iter != BL.end();) {
      if (query == iter->getJoiningFaceID1()) {
        innerBends.push_back(*iter);
        found = true;
        iter = BL.erase(iter);
      } else if (query == iter->getJoiningFaceID2()) {
        innerBends.push_back(*iter);
        found = true;
        iter = BL.erase(iter);
      } else {
        ++iter;
      }
    }

    return found;
  }

  bool Model::cleanModel()
  {
    bool isClean = false;
    FaceID searchID;
    std::vector<ModelBend> mBendList;
    std::vector<ModelBend> mOuterBends;

    mBendList = mModelBends; 

    mOuterBends.push_back(mBendList[0]);

    mBendList.erase(mBendList.begin());

    searchID = mOuterBends[0].getJoiningFaceID1();

    size_t i = 0;
    while(mOuterBends.size() != mBendList.size())
    {
      if(findFace(searchID, mBendList, mOuterBends)) {        
        if (searchID == mOuterBends[i].getJoiningFaceID1()) {
          searchID = mOuterBends[i].getJoiningFaceID2();
        } else {
          ++i;
          searchID = mOuterBends[i].getJoiningFaceID1();
        }
      } else {
        if(mOuterBends.size() != mBendList.size())
        {
          if (searchID != mOuterBends[i].getJoiningFaceID2()) {
            searchID = mOuterBends[i].getJoiningFaceID2();
          } else {
            ++i;
            searchID = mOuterBends[i].getJoiningFaceID1();
          }
        }        
      }
    }

    mInnerBends_ = mBendList;

    for(auto& face : mModelFaces){
      for(auto& bend : mInnerBends_) {
        if(bend.getJoiningFaceID1() == face.getFaceId() || bend.getJoiningFaceID2() == face.getFaceId()){
          mInnerFaces_.push_back(face);
          break;
        }
      }
    }

    size_t fid = 0, bbid = 0;
    std::cout << "Num Inner Bends : " << mInnerBends_.size() << std::endl;
    for(auto& c: mInnerBends_){
      ++bbid;
      c.setFaceId(bbid);
      c.setJoiningFaceID1(0);
      c.setJoiningFaceID2(0);
    }

    std::cout << "Size : " << mInnerFacesMap_.size() << std::endl;
    std::cout << "Num Inner Faces : " << mInnerFaces_.size() << std::endl;
    for(auto& c: mInnerFaces_){
      ++fid;
      c.setFaceId(fid);
    }
    
    for (auto& bend: mInnerBends_){
      for (auto& edge: bend.getFaceEdges()){
        if (edge.getEdgeType() == EdgeType::LINE) {
          for (auto& face: mInnerFaces_) {
            if (face.getFaceType() == FaceType::FACE) {
              for (auto& faceEdge : face.getFaceEdges()){
                if (abs(faceEdge.getEdgeLength() - edge.getEdgeLength()) < 0.01) {
                  if(faceEdge == edge) { 
                    faceEdge.setEdgePosition(EdgePosition::JOINING_EDGE);           
                    if ((bend.getJoiningFaceID1() == 0) && (bend.getJoiningFaceID2() == 0)) {
                      bend.setJoiningFaceID1(face.getFaceId());
                      edge.setJoiningFaceID(face.getFaceId());
                    } else if (bend.getJoiningFaceID2() == 0){
                      bend.setJoiningFaceID2(face.getFaceId());
                      edge.setJoiningFaceID(face.getFaceId());
                    } else {
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    for(auto& c: mInnerBends_){
      mInnerBendsMap_.insert({c.getFaceId(), c});
    }

    for(auto& c: mInnerFaces_){
      mInnerFacesMap_.insert({c.getFaceId(), c});
    }

    for (auto& bend: mInnerBends_) {
      mInnerFaceAdjacencyMap_[bend.getJoiningFaceID1()].push_back(std::make_pair(bend.getFaceId(), 0));
      mInnerFaceAdjacencyMap_[bend.getJoiningFaceID2()].push_back(std::make_pair(bend.getFaceId(), 0));
    }

    std::cout << "\n=============================================" << '\n';
    printAdjacencyGraph(mInnerFaceAdjacencyMap_);
    computeBendDistance(mInnerFaceAdjacencyMap_);

    if(mOuterBends.size() == mBendList.size())
      isClean = true;
    
    return isClean;
  }

  void Model::classifyFaces()
  {
    std::vector<ModelFace> modelFaces_0;
    
    // Extract all the useful faces
    for (auto& bend: mModelBends){
      for (auto& edge: bend.getFaceEdges()){
        if (edge.getEdgeType() == EdgeType::LINE) {
          for (auto& face: mModelFaces)
          {
            if (face.getFaceType() == FaceType::NONE) {
              for (auto& faceEdge : face.getFaceEdges()){
                if (abs(faceEdge.getEdgeLength() - edge.getEdgeLength()) < 0.01) {
                  if(faceEdge == edge) {
                    faceEdge.setEdgePosition(EdgePosition::JOINING_EDGE);
                    face.setFaceType(FaceType::FACE);
                  
                    modelFaces_0.push_back(face);
                  }
                }
              }
            }
          }
        }
      }
    }

    for (auto& a: mModelFaces) {
      if (a.getFaceType() == FaceType::NONE) {
        a.setFaceType(FaceType::THICKNESS_DEFINING_FACE);        
      }
    }

    for (auto& a: mModelFaces) {
      if (a.getFaceType() == FaceType::THICKNESS_DEFINING_FACE){
        a.setFaceId(0);
      }
    }

    for (auto& a: mModelFaces) {
      if (a.getFaceType() == FaceType::THICKNESS_DEFINING_FACE){
        a.setFaceId(0);
        mThickness = computeThickness(a);
        break;
      }
    }

    std::cout << "After extracting faces : " << modelFaces_0.size() << '\n';
    size_t fid = 0;
    for(auto& c: modelFaces_0){
      ++fid;
      c.setFaceId(fid);
    }

    std::cout << "Bend faces : " << mModelBends.size() << '\n';
    size_t bbid = 0;
    for(auto& c: mModelBends){
      ++bbid;
      c.setFaceId(bbid);
    }

    mModelFaces = modelFaces_0;

    for (auto& bend: mModelBends){
      for (auto& edge: bend.getFaceEdges()){
        if (edge.getEdgeType() == EdgeType::LINE) {
          for (auto& face: mModelFaces)
          {
            if (face.getFaceType() == FaceType::FACE) {
              for (auto& faceEdge : face.getFaceEdges()){
                if (abs(faceEdge.getEdgeLength() - edge.getEdgeLength()) < 0.01) {
                  if(faceEdge == edge) {       
                    faceEdge.setEdgePosition(EdgePosition::JOINING_EDGE);           
                    if ((bend.getJoiningFaceID1() == 0) && (bend.getJoiningFaceID2() == 0)) {
                      bend.setJoiningFaceID1(face.getFaceId());
                    } else if (bend.getJoiningFaceID2() == 0){
                      bend.setJoiningFaceID2(face.getFaceId());
                    } else {
                    }
                  }
                }
              }
            }
          }
        }
      }
    }

    for(auto& c: mModelFaces)
      c.setFaceEdgePosition();
  }

  void Model::computeBendAngles()
  {
    for (auto& bend : mModelBends){
      if ((bend.getJoiningFaceID1() != 0) && (bend.getJoiningFaceID2() != 0)) {
        gp_Dir fn1 = getNormal(bend.getJoiningFaceID1());
        gp_Dir fn2 = getNormal(bend.getJoiningFaceID2());

        double angle = round(fn1.Angle(fn2) * (180.0 / M_PI));
        
        bend.setBendAngle(roundd(angle));
      }
    }
  }

  void Model::printBendInfo()
  {
    std::cout << "\n=============================================" << '\n';
    std::cout << "Thickness : " << mThickness << " mm" << '\n';
    for (auto& a: mInnerBends_) {
      std::cout << "F"<< a.getJoiningFaceID1() <<
      "---B" << a.getFaceId() <<
      "---F" << a.getJoiningFaceID2()<<
      " Angle : " << a.getBendAngle() <<
      " Radius : " << a.getBendRadius() <<
      " Length : " << a.getBendLength() << " mm\n";
    }

    std::cout << "\n=============================================" << '\n';    
  }

  gp_Dir Model::getNormal(FaceID faceID)
  {
    gp_Dir vt;
    for (auto& face : mModelFaces) {
      if (face.getFaceId() == faceID) {
        vt = face.getFaceNormal();
      }
    }
    
    return vt;
  }

  FaceID Model::getBendWithJoiningFID(FaceID id, FaceID currbendID)
  {
    FaceID bid = 10000;

    for(auto& bend : mInnerBends_) {
      if ((id == bend.getJoiningFaceID1() || id == bend.getJoiningFaceID2()) && (bend.getFaceId() != currbendID)) {
        bid = bend.getFaceId();
        break;
      }
    }

    return bid;
  }

  void Model::printAdjacencyGraph(std::map<FaceID, std::vector<std::pair<FaceID, double>>> mInnerFaceAdjacencyMap)
  {
    for (const auto& face : mInnerFaceAdjacencyMap) {
      std::cout << face.first << "---->";
      for (size_t i = 0; i < face.second.size(); i++)
      {
        std::cout << face.second[i].first << "(" << face.second[i].second << ") " ;
      }
      std::cout << std::endl;
    }

    std::cout << std::endl;
  }

  double Model::computeDistance(ModelFace& mf)
  {
    double length;

    for(const auto& edge : mf.getFaceEdges()) {
      if (edge.getEdgePosition() == EdgePosition::SIDE_EDGE)
      {
        length = edge.getEdgeLength();
        break;
      }      
    }

    return length;
  }

  double Model::computeDistance(FaceID faceId, ModelBend& b1)
  {
    double length = 0.0;
    ModelEdge b1_edge;

    for(auto& edge : b1.getFaceEdges()) {
      if (edge.getJoiningFaceID() == faceId)
      {
        b1_edge = edge;
        break;
      }      
    }
    auto face = mInnerFacesMap_.find(faceId);

    if (face != mInnerFacesMap_.end())
    {
      for (auto& edge : face->second.getFaceEdges()) {
        auto angle = roundd(b1_edge.getEdgeLineVector().Angle(edge.getEdgeLineVector()) * (180.0 / M_PI));

        if ((angle == 0.0 || angle == 180) && ((b1_edge.getEdgeLength() >= edge.getEdgeLength()) || (edge.getEdgeLength() >= b1_edge.getEdgeLength()))) {
          gp_Pnt aVertex = computeLineVector(b1_edge);
          gp_Lin lineSide (b1_edge.getEdgeEndPoints()[0], gp_Dir(aVertex.X(), aVertex.Y(), aVertex.Z()));

          gp_Pnt bVertex = computeLineVector(edge);
          gp_Lin lineB (edge.getEdgeEndPoints()[0], gp_Dir(bVertex.X(), bVertex.Y(), bVertex.Z()));

          length = lineSide.Distance(lineB);
        }
      }
    }

    return length;
  }

  double Model::computeDistance(FaceID faceId, ModelBend& b1, ModelBend& b2)
  {
    double length = 0.0;

    auto parallelism = b1.isParallel(b2);

    if ((parallelism) && ((b1.getBendLength() >= b2.getBendLength()) || (b2.getBendLength() >= b1.getBendLength()))) {
      length = b1.computeBendDistance(b2);
    }      

    return length;
  }

  void Model::computeBendDistance(std::map<FaceID, std::vector<std::pair<FaceID, double>>>& mInnerFaceAdjacencyMap)
  {
    double dist = 0.0;

    for (auto& face : mInnerFaceAdjacencyMap) {

      if (face.second.size() == 1)
      {
        // Distance between bend and parallel disjoint face edge
        auto f = mInnerFacesMap_.find(face.first);
        if (f != mInnerFacesMap_.end()) {

          auto b1 = mInnerBendsMap_.find(face.second[0].first);
          for(auto& edge : b1->second.getFaceEdges()) {
            if (edge.getJoiningFaceID() == f->first) {
              dist = computeDistance(f->second);              
              break;
            }            
          }

          std::cout << "One   : " << f->first << "----| " << dist << " |----" << b1->first << std::endl;
        }        
      } else if (face.second.size() == 2)
      {
        // Distance between the 2 bends or distance between one disjoint face edge and bend (if the two bends are perpendicular)
        double length1 = 0, length2 = 0;

        auto f = mInnerFacesMap_.find(face.first);

        if (f != mInnerFacesMap_.end())
        {
          auto b1 = mInnerBendsMap_.find(face.second[0].first);
          auto b2 = mInnerBendsMap_.find(face.second[1].first);

          auto length = computeDistance(f->first, b1->second , b2->second);

          if (length == 0.0)
          {
            length1 = computeDistance(f->first, b1->second);
            length2 = computeDistance(f->first, b2->second);
            
            std::cout << "Two   : " << f->first << "----| " << length1 << " |" << b1->first;
            std::cout << "----| " << length2 << " |----" << b2->first << std::endl;

          } else
          {
            std::cout << "Two   : " << f->first << "----" << b1->first;
            std::cout << "----| " << length << " |----" << b2->first << std::endl;
          }   
        }  
      } else if (face.second.size() == 3)
      {
        // Distance between the 2 bends and distance between one disjoint face edge and bend
        double length1 = 0, length2 = 0;
        auto f = mInnerFacesMap_.find(face.first);

        if (f != mInnerFacesMap_.end())
        {
          auto b1 = mInnerBendsMap_.find(face.second[0].first);
          auto b2 = mInnerBendsMap_.find(face.second[1].first);          
          auto b3 = mInnerBendsMap_.find(face.second[2].first);

          if (b1->second.isParallel(b2->second))
          {
            length1 = computeDistance(f->first, b1->second, b2->second);
            length2 = computeDistance(f->first, b3->second);

            std::cout << "1Three : "  << f->first << "----| " << length1 << " |----" << b1->first;          
            std::cout << "----| " << length1 << " |----" << b2->first;          
            std::cout << "----| " << length2 << " |----" << b3->first << std::endl;
          } else {
            if (b3->second.isParallel(b2->second))
            {
              length1 = computeDistance(f->first, b1->second);
              length2 = computeDistance(f->first, b3->second, b2->second);

              std::cout << "2Three : "  << f->first << "----| " << length1 << " |----" << b1->first;;          
              std::cout << "----| " << length2 << " |----" << b2->first;          
              std::cout << "----| " << length2 << " |----" << b3->first << std::endl;
            } else {
              if (b1->second.isParallel(b3->second))
              {
                length1 = computeDistance(f->first, b1->second, b3->second);
                length2 = computeDistance(f->first, b2->second);

                std::cout << "3Three : "  << f->first << "----| " << length1 << " |----" << b1->first;;          
                std::cout << "----| " << length2 << " |----" << b2->first;          
                std::cout << "----| " << length1 << " |----" << b3->first << std::endl;
              }
            }           
            
          }       
          
        }        
      } else if (face.second.size() == 4)
      {
        // Distance between parallel pairs of bends
        double length1 = 0, length2 = 0;
        auto f = mInnerFacesMap_.find(face.first);

        if (f != mInnerFacesMap_.end())
        {
          auto b1 = mInnerBendsMap_.find(face.second[0].first);
          auto b2 = mInnerBendsMap_.find(face.second[1].first);          
          auto b3 = mInnerBendsMap_.find(face.second[2].first);
          auto b4 = mInnerBendsMap_.find(face.second[3].first);

          if (b1->second.isParallel(b2->second) || b3->second.isParallel(b4->second))
          {
            length1 = computeDistance(f->first, b1->second, b2->second);
            length2 = computeDistance(f->first, b3->second, b4->second);

            std::cout << "1Four : "  << f->first << "----" << b1->first << "----| " << length1 << " |----" << b2->first;          
            std::cout << "----" << b3->first << "----| " << length2 << " |----" << b4->first << std::endl;
          } else {
            if (b3->second.isParallel(b2->second) || b1->second.isParallel(b4->second))
            {
              length1 = computeDistance(f->first, b1->second, b4->second);
              length2 = computeDistance(f->first, b3->second, b2->second);

              std::cout << "2Four : "  << f->first << "----" << b1->first << "----| " << length1 << " |----" << b4->first;          
              std::cout << "----" << b2->first << "----| " << length2 << " |----" << b3->first << std::endl;
            } else {
              if (b1->second.isParallel(b3->second) || b2->second.isParallel(b4->second))
              {
                length1 = computeDistance(f->first, b1->second, b3->second);
                length2 = computeDistance(f->first, b2->second, b4->second);

                std::cout << "3Four : "  << f->first << "----" << b1->first << "----| " << length1 << " |----" << b3->first;          
                std::cout << "----" << b2->first << "----| " << length2 << " |----" << b4->first << std::endl;
              }              
            }           
            
          }
        }
      }      
    }

    std::cout << std::endl;
  }

  bool Model::isParallel(size_t bend1, size_t bend2)
  {
    bool parallel = false;
    auto b1 = mInnerBendsMap_.find(bend1);
    auto b2 = mInnerBendsMap_.find(bend2);

    if (b1 != mInnerBendsMap_.end() && b2 != mInnerBendsMap_.end()) {
      parallel = b1->second.isParallel(b2->second);
    }

    return parallel;
  }

  bool Model::isSameAngle(size_t bend1, size_t bend2)
  {
    bool parallel = false;
    auto b1 = mInnerBendsMap_.find(bend1);
    auto b2 = mInnerBendsMap_.find(bend2);

    if (b1 != mInnerBendsMap_.end() && b2 != mInnerBendsMap_.end()) {
      parallel = (b1->second.getBendAngle() == b2->second.getBendAngle()); //(b1->second.getBendAngle() - b2->second.getBendAngle()) < 0.01;
    }

    return parallel;
  }

  double Model::distance(size_t bend1, size_t bend2)
  {
    double distance = 0;
    auto b1 = mInnerBendsMap_.find(bend1);
    auto b2 = mInnerBendsMap_.find(bend2);

    if (b1 != mInnerBendsMap_.end() && b2 != mInnerBendsMap_.end()) {
      if (b1->second.isParallel(b2->second))
      {
        distance = b1->second.computeBendDistance(b2->second);
      }
    }

    return distance;
  }

  bool Model::isSameDirection(size_t bend1, size_t bend2)
  { 
    auto b1 = mInnerBendsMap_.find(bend1);
    auto b2 = mInnerBendsMap_.find(bend2);

    return (b1->second.getBendDirection() == b2->second.getBendDirection());
  }
  
  bool Model::hasCollision(size_t bend1, size_t bend2){ return true;}

  void Model::assignBendDirection()
  {
    auto max = std::numeric_limits<long double>::min();
    auto min = std::numeric_limits<long double>::max();

    for(auto& bend : mInnerBends_){
      if(max < bend.getBendRadius()){
        max = bend.getBendRadius();
        std::cerr << "max -> " << max << '\n';
        bend.setBendDirection(2);
      }
      
      if(min > bend.getBendRadius()){
        min = bend.getBendRadius();
        std::cerr << "min -> " << min << '\n';
        bend.setBendDirection(1);
      }
    }

    for(auto& bend : mInnerBends_){
      if(max == bend.getBendRadius()){
        bend.setBendDirection(2);
      }
      
      if(min == bend.getBendRadius()){
        bend.setBendDirection(1);
      }

      bend.setBendRadius(min);
    }

    // auto max = std::max(mModelBends.begin(), mModelBends.end(), [](auto& first, auto& sec)
    // {
    //   return first->getBendRadius() > sec->getBendRadius();
    // });

    // auto min = std::min(mModelBends.begin(), mModelBends.end(), [](auto& first, auto& sec)
    // {
    //   return first->getBendRadius() < sec->getBendRadius();
    // });

    std::cerr << "Max radius: " << max << '\n';
    std::cerr << "Min radius: " << min << '\n';
    std::cerr << "Actual radius: " << (min + max) / 2 << '\n';
    
  }
}