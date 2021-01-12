#pragma once

#include "../MFaceAbstract.h"
#include "../../../include/sheet-metal-component/bend/BendFeature.h"

namespace Fxt
{
namespace SheetMetalComponent
{
namespace Bend
{            
    class BendLine;
    class BendFeature;

    class ModelBend : public Fxt::SheetMetalComponent::MFaceAbstract
    {
        std::shared_ptr< Fxt::SheetMetalComponent::Bend::BendFeature> mBendFeature;
    
        unsigned long mBendSameAsId;

        // The arc type edges in a bend.
        // useful for computing the bend line because if prevents multiple calls to
        // the arcEdgeExtraction function.
        std::vector<std::shared_ptr<Edge::ModelEdge>> mArcEdges;

        std::vector<std::shared_ptr<Edge::ModelEdge>> mStraightEdges;

        std::shared_ptr<Fxt::SheetMetalComponent::Bend::BendLine> mBendLine;

    public:
        ModelBend(const FaceID faceID = 0, std::shared_ptr<TopoDS_Face> topoDSFace = nullptr);

        void init();

        void arcEdgeExtraction();

        void setBendLength();
        
        std::shared_ptr<Fxt::SheetMetalComponent::Bend::BendLine> getBendLine() const;

        double computeBendDistance(const std::shared_ptr<Fxt::SheetMetalComponent::Bend::ModelBend>& otherBend) const;

        bool isParallel(const std::shared_ptr<Fxt::SheetMetalComponent::Bend::ModelBend>& otherBend) const;

        std::shared_ptr< Fxt::SheetMetalComponent::Bend::BendFeature> getBendFeature() const;

        std::vector<std::shared_ptr<Edge::ModelEdge>> getStraightEdges() const;

        friend std::ostream& operator<<(std::ostream& os, const std::shared_ptr<ModelBend>& modelBend)
        {
            auto bendFeature = modelBend->getBendFeature();

            os << "F" << bendFeature->getJoiningFaceID1() << "---" << 
                "B" << modelBend->getFaceId() << "---" <<
                "F" << bendFeature->getJoiningFaceID2() << " ";
            
            os << "Angle: " << bendFeature->getBendAngle() << ", ";
            os << "Bend length: " << bendFeature->getBendLength() << "mm, ";
            os << "Radius: " << bendFeature->getBendRadius() << "mm";
            
            return os;
        }
    };
}
} 
}