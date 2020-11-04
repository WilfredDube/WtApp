#pragma once

#include "../MFaceAbstract.h"

namespace Fxt
{
namespace SheetMetalComponent
{
namespace Bend
{            
    using FaceID = Fxt::SheetMetalComponent::ModelTypes::FaceID;
    using BendType = Fxt::SheetMetalComponent::ModelTypes::BendType;

    class BendLine;
    class BendFeature;

    class ModelBend : public Fxt::SheetMetalComponent::MFaceAbstract
    {
        BendType mBendType;

        std::shared_ptr< Fxt::SheetMetalComponent::Bend::BendFeature> mBendFeature;
    
        unsigned long mBendSameAsId;

        // The arc type edges in a bend.
        // useful for computing the bend line because if prevents multiple calls to
        // the arcEdgeExtraction function.
        std::vector<std::shared_ptr<Edge::ModelEdge>> mArcEdges;

        std::vector<std::shared_ptr<Edge::ModelEdge>> mStraightEdges;

        std::shared_ptr<Fxt::SheetMetalComponent::Bend::BendLine> mBendLine;

    public:
        ModelBend(const Fxt::SheetMetalComponent::FaceID faceID = 0, std::shared_ptr<TopoDS_Face> topoDSFace = nullptr);

        void init();

        void arcEdgeExtraction();

        void setBendLength();
        
        std::shared_ptr<Fxt::SheetMetalComponent::Bend::BendLine> getBendLine() const;

        double computeBendDistance(const std::shared_ptr<Fxt::SheetMetalComponent::Bend::ModelBend>& otherBend) const;

        bool isParallel(const std::shared_ptr<Fxt::SheetMetalComponent::Bend::ModelBend>& otherBend) const;

        std::shared_ptr< Fxt::SheetMetalComponent::Bend::BendFeature> getBendFeature() const;

        std::vector<std::shared_ptr<Edge::ModelEdge>> getStraightEdges() const;

    };
}
} 
}