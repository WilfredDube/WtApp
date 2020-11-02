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

