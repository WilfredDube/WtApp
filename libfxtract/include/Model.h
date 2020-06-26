#ifndef MODEL_H_
#define MODEL_H_

#include <vector>
#include <memory>
#include <map>
#include <TopoDS_Face.hxx>
#include "MFace.h"
#include "ModelFace.h"
#include "ModelBend.h"

namespace Fxt {

  class Model
  {
    friend class boost::serialization::access;
    friend std::ostream & operator<<(std::ostream &os, const Model &m);

    // std::map<FaceID, ModelBend> bends;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & mModelFile;
        ar & mInnerBendsMap_;
    }

  public:
    Model() = default;
    Model(const char* modelFile);

    void init();

    std::string getModelFile() { return mModelFile; }

    std::vector<ModelFace> getFaces();
    std::vector<ModelBend> getBends();

    std::map<FaceID, ModelBend>& getBendMap(){ return mInnerBendsMap_;}

    Standard_Real getThickness() { return mThickness; }

    ModelFace getFacesAt(int index);

    void assignFaceAttributes(FaceID faceID, TopoDS_Shape& aShape);

    void classifyFaces();

    void computeBendAngles();

    bool cleanModel();
    
    void printBendInfo();

    bool isParallel(size_t bend1, size_t bend2);
    bool isSameAngle(size_t bend1, size_t bend2);
    bool isSameDirection(size_t bend1, size_t bend2);
    double distance(size_t bend1, size_t bend2);
    bool hasCollision(size_t bend1, size_t bend2);

  private:
    std::string mModelFile;

    //!< thickness of the model sheetmetal.
    Standard_Real mThickness;
    
    std::vector<ModelFace> mModelFaces;
    std::vector<ModelBend> mModelBends;

    std::vector<ModelBend> mInnerBends_;
    std::vector<ModelBend> mOutterBends_;
    
    std::map<FaceID, ModelFace> mInnerFacesMap_;
    std::map<FaceID, ModelBend> mInnerBendsMap_;
    std::map<FaceID, std::vector<std::pair<FaceID, double>>> mInnerFaceAdjacencyMap_;

    std::vector<ModelFace> mInnerFaces_;
    std::vector<ModelFace> mOutterFaces_;

    std::vector<FaceID> mFaceAdjacencyGraph;
    TopoDS_Shape mModelShape;

    gp_Dir getNormal(FaceID faceID);

    void addFace(ModelFace modelFace);
    void addBend(ModelBend modelFace);

    FaceID getBendWithJoiningFID(FaceID id, FaceID currbendID);

    bool findFace(FaceID query, std::vector<ModelBend>& BL, std::vector<ModelBend>& innerBends);

    void printAdjacencyGraph(std::map<FaceID, std::vector<std::pair<FaceID, double>>> mInnerFaceAdjacencyMap);

    void computeBendDistance(std::map<FaceID, std::vector<std::pair<FaceID, double>>>& mInnerFaceAdjacencyMap);

    double computeDistance(ModelFace& mf);
    double computeDistance(FaceID faceId, ModelBend& b);
    double computeDistance(FaceID faceId, ModelBend& b1, ModelBend& b2);

  };

}

#endif
