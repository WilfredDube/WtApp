#include "../../../include/sheet-metal-component/bend/ModelBend.h"
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

    setBendLength(mFaceEdges);

    computeBendLine();
}

void ModelBend::setBendLength(const std::vector<std::shared_ptr<Fxt::SheetMetalComponent::Edge::ModelEdge>> mFaceEdges)
{
    for(const auto& edge : mFaceEdges){
        if(edge->getEdgeType() == Fxt::SheetMetalComponent::ModelTypes::EdgeType::LINE){
            mBendFeature->setBendLength(edge->getEdgeLength());
            break;
        }
    }
}

void ModelBend::computeBendLine()
{
    std::vector<gp_Pnt> endPoints;

    for(auto& edge :  getFaceEdges()) {
       if (edge->getEdgeType() == EdgeType::ARC)
       {  
         endPoints.push_back(computeMidPoint(edge->getEdgeEndPoints()[0], edge->getEdgeEndPoints()[1]));
       }       
    }

    gp_Pnt dirVertex(
      endPoints[0].X() - endPoints[1].X(),
      endPoints[0].Y() - endPoints[1].Y(),
      endPoints[0].Z() - endPoints[1].Z()
    );


    mBendLine = std::make_shared<gp_Lin>(endPoints[0], gp_Dir(dirVertex.X(), dirVertex.Y(), dirVertex.Z()));

    mBendLinePnt->X = endPoints[0].X();//bendLine_.Location().X();
    mBendLinePnt->Y = endPoints[0].Y();//bendLine_.Location().Y();
    mBendLinePnt->Z = endPoints[0].Z();//bendLine_.Location().Z();

    mBendLineDir->X = dirVertex.X();
    mBendLineDir->Y = dirVertex.Y();
    mBendLineDir->Z = dirVertex.Z();
}

void ModelBend::makeBendLine()
{
    mBendLine = std::make_shared<gp_Lin>(
        gp_Pnt( mBendLinePnt->X, mBendLinePnt->Y, mBendLinePnt->Z), gp_Dir(mBendLineDir->X, mBendLineDir->Y, mBendLineDir->Z)
        );
}

std::shared_ptr<gp_Lin> ModelBend::getBendLine() const
{
    return mBendLine;
}

double ModelBend::computeBendDistance(const std::shared_ptr<Fxt::SheetMetalComponent::Bend::ModelBend>& otherBend) const
{
    return mBendLine->Distance(*(otherBend->getBendLine()));
}

bool ModelBend::isParallel(const std::shared_ptr<ModelBend>& otherBend) const
{
    auto angle = roundd(mBendLine->Angle(*(otherBend->getBendLine())) * (180 / M_PI));

    return (angle == 0.0 || angle == 180);
}