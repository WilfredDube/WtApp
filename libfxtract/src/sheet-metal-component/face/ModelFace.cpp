#include "../../../include/sheet-metal-component/face/ModelFace.h"
#include "../../../include/Computation.h"

using namespace Fxt::SheetMetalComponent::Face;

ModelFace::ModelFace(Fxt::SheetMetalComponent::FaceID faceID, std::shared_ptr<TopoDS_Face> topoDSFace)
: MFaceAbstract(faceID, topoDSFace) 
{
    setPlaneType(PlaneType::PLANAR);
    setFaceType(FaceType::NONE);

    init();
}

void ModelFace::init()
{
    extractEdges();

    computeFaceNormal();
    setUnitNormal(computeUnitNormal(getTModelFace()));
}
   