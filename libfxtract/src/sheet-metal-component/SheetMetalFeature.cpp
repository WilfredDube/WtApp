#include "../../include/sheet-metal-component/SheetMetalFeature.h"
#include "../../include/sheet-metal-component/face/ModelFace.h"
#include "../../include/sheet-metal-component/bend/ModelBend.h"
#include "../../include/sheet-metal-component/bend/BendFeature.h"

using namespace Fxt::SheetMetalComponent;

void SheetMetalFeature::addModelFace(std::shared_ptr<Face::ModelFace>& modelFace)
{
    mModelFaces[modelFace->getFaceId()] = modelFace;
}

void SheetMetalFeature::addModelBend(std::shared_ptr<Bend::ModelBend>& modelBend)
{
    mModelBends[modelBend->getFaceId()] = modelBend;
}

std::shared_ptr<gp_Dir> SheetMetalFeature::getNormalByFaceID(const FaceID faceID) const
{
    auto modelFace = mModelFaces.find(faceID)->second;
    return modelFace->getFaceNormal();
}

FaceID SheetMetalFeature::getIdOfBendWithJoiningFaceID(const FaceID currbendID, const FaceID faceID) const
{
    auto modelBend = mModelBends.find(currbendID);

    if(modelBend != mModelBends.end()){
        auto bend = modelBend->second;

        if(faceID == bend->getBendFeature()->getJoiningFaceID1() ||
            faceID == bend->getBendFeature()->getJoiningFaceID2())
            return bend->getFaceId();
    }

    return -1;
}

bool SheetMetalFeature::splitModelBends(const FaceID id, std::map<FaceID, std::shared_ptr<Bend::ModelBend>>& innerBends, 
                std::map<FaceID, std::shared_ptr<Bend::ModelBend>>& outerBends)
{
    bool found = false;
    // TODO: Find appropriate STL <algorithm> functions
    for (auto bendIter = innerBends.begin(); bendIter != innerBends.end();) {
        auto& bend = bendIter->second;
        if(id == bend->getBendFeature()->getJoiningFaceID1()){
            outerBends[bend->getFaceId()] = bend;
            found = true;
            bendIter = innerBends.erase(bendIter);
        }
        else if (id == bend->getBendFeature()->getJoiningFaceID2()) {
            outerBends[bend->getFaceId()] = bend;
            found = true;
            bendIter = innerBends.erase(bendIter);
        } else {
            ++bendIter;
        }
    }

    return found;
}

std::map<FaceID, std::shared_ptr<Face::ModelFace>> SheetMetalFeature::getFaces() const
{
    return mModelFaces;
}

std::map<FaceID, std::shared_ptr<Bend::ModelBend>> SheetMetalFeature::getBends() const
{
    return mModelBends;
}

double SheetMetalFeature::getThickness() const
{
    return mThickness;
}

void SheetMetalFeature::assignFaceAttributes(const FaceID faceID, std::shared_ptr<TopoDS_Shape>& aShape)
{
    mTopologicalShape = aShape;

    auto pTopoDSFace { std::make_shared<TopoDS_Face>(TopoDS::Face(*mTopologicalShape)) };

    Standard_Real curvature = computeCurvature(pTopoDSFace);

    if (curvature == 0.0){

      auto pModelFace { std::make_shared<Face::ModelFace>(faceID, pTopoDSFace) };

      addModelFace(pModelFace);

    } else {

      auto pModelFace { std::make_shared<Bend::ModelBend>(faceID, pTopoDSFace) };

      pModelFace->getBendFeature()->setCurvature(curvature);

      addModelBend(pModelFace);
    }
}

// void SheetMetalFeature::classifyFaces();

// void SheetMetalFeature::computeBendAngles();

// bool SheetMetalFeature::cleanModel();