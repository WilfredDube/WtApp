#pragma once

#include "../MFaceAbstract.h"
#include "BendFeature.h"
// #include "BendComputation.h"

namespace Fxt
{
namespace SheetMetalComponent
{
namespace Bend
{            
    using FaceID = Fxt::SheetMetalComponent::ModelTypes::FaceID;
    using BendType = Fxt::SheetMetalComponent::ModelTypes::BendType;

    class ModelBend : Fxt::SheetMetalComponent::MFaceAbstract
    {
        BendType mBendType;

        std::shared_ptr< Fxt::SheetMetalComponent::Bend::BendFeature> mBendFeature;
        // std::shared_ptr< Fxt::SheetMetalComponent::Bend::BendComputation> mComputation;
    
        unsigned long mBendSameAsId;

        std::shared_ptr<gp_Lin> mBendLine;
        
        std::shared_ptr<Point> mBendLinePnt, mBendLineDir;    
    public:
        ModelBend(const Fxt::SheetMetalComponent::FaceID faceID = 0, std::shared_ptr<TopoDS_Face> topoDSFace = nullptr);

        void init();

        void setBendLength(const std::vector<std::shared_ptr<Fxt::SheetMetalComponent::Edge::ModelEdge>> mFaceEdges);

        void computeBendLine();

        void makeBendLine();
        
        std::shared_ptr<gp_Lin> getBendLine() const;

        double computeBendDistance(const std::shared_ptr<Fxt::SheetMetalComponent::Bend::ModelBend>& otherBend) const;

        bool isParallel(const std::shared_ptr<Fxt::SheetMetalComponent::Bend::ModelBend>& otherBend) const;
    };
}
} 
}