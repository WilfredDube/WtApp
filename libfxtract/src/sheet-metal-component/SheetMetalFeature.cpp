#include "../../include/sheet-metal-component/SheetMetalFeature.h"
#include "../../include/sheet-metal-component/face/ModelFace.h"
#include "../../include/sheet-metal-component/bend/ModelBend.h"

using namespace Fxt::SheetMetalComponent;

void SheetMetalFeature::addModelFace(std::shared_ptr<Face::ModelFace>& modelFace)
{
    mModelFaces[modelFace->getFaceId()] = modelFace;
}

void SheetMetalFeature::addModelBend(std::shared_ptr<Bend::ModelBend>& modelBend)
{
    mModelBends[modelBend->getFaceId()] = modelBend;
}

