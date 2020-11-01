#include "../../../include/sheet-metal-component/bend/ModelBend.h"
#include "../../../include/sheet-metal-component/bend/BendLine.h"
#include "../../../include/sheet-metal-component/bend/BendFeature.h"
#include "../../../include/Computation.h"
#include "../../../include/sheet-metal-component/ModelTypes.h"

using namespace Fxt::SheetMetalComponent::Bend;
using namespace Fxt::SheetMetalComponent::Edge;

ModelBend::ModelBend(Fxt::SheetMetalComponent::FaceID faceID, std::shared_ptr<TopoDS_Face> topoDSFace)
: MFaceAbstract(faceID, topoDSFace)
{
    if(topoDSFace != nullptr)
    {
      setPlaneType(PlaneType::NON_PLANAR);
      setFaceType(FaceType::BEND_FACE);

      mBendFeature->setJoiningFaceID1(0);
      mBendFeature->setJoiningFaceID2(0);
      mBendFeature->setBendAngle(0);

      init();
    }
}

void ModelBend::init()
{
    extractEdges();

    Standard_Real curvature = computeCurvature<TopoDS_Face>(mModelFace);
    mBendFeature->setCurvature(curvature);

    computeFaceNormal();
    setUnitNormal(computeUnitNormal(mModelFace));

    setBendLength();


    mBendLine->computeBendLine(mArcEdges);
}

void ModelBend::arcEdgeExtraction()
{
    for(const auto& edge : mFaceEdges){
        if(edge->getEdgeType() == Fxt::SheetMetalComponent::ModelTypes::EdgeType::ARC){
            mArcEdges.push_back(edge);
        }
    }
}

void ModelBend::setBendLength()
{
    for(const auto& edge : mFaceEdges){
        if(edge->getEdgeType() == Fxt::SheetMetalComponent::ModelTypes::EdgeType::LINE){
            mBendFeature->setBendLength(edge->getEdgeLength());
            break;
        }
    }
}
       }       
}

std::shared_ptr<Fxt::SheetMetalComponent::Bend::BendLine> ModelBend::getBendLine() const 
{
    return mBendLine;
}

double ModelBend::computeBendDistance(const std::shared_ptr<Fxt::SheetMetalComponent::Bend::ModelBend>& otherBend) const
{
    return mBendLine->distance(otherBend->getBendLine());
}

bool ModelBend::isParallel(const std::shared_ptr<ModelBend>& otherBend) const
{
    auto angle = mBendLine->angle(otherBend->getBendLine());

    return (angle == 0.0 || angle == 180);
}