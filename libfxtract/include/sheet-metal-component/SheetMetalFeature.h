#pragma once

#include "ModelTypes.h"

#include <TopoDS_Shape.hxx>

#include <map>
#include <memory>

namespace Fxt
{
namespace SheetMetalComponent
{
    namespace Face
    {
        class ModelFace;
    }

    namespace Bend
    {
        class ModelBend;
    }

    using FaceID = Fxt::SheetMetalComponent::ModelTypes::FaceID;

    class SheetMetalFeature
    {
        std::map<FaceID, std::shared_ptr<Face::ModelFace>> mModelFaces;
        std::map<FaceID, std::shared_ptr<Bend::ModelBend>> mModelBends;

        long double mThickness;

        std::shared_ptr<TopoDS_Shape> mTopologicalShape;

    public:
        void addModelFace(std::shared_ptr<Face::ModelFace>& modelFace);

        void addModelBend(std::shared_ptr<Bend::ModelBend>& modelBend);

        std::shared_ptr<gp_Dir> getNormalByFaceID(const FaceID faceID) const;

        FaceID getIdOfBendWithJoiningFaceID(const FaceID currbendID, const FaceID faceID) const;
        
        bool splitModelBends(const FaceID id, std::map<FaceID, std::shared_ptr<Bend::ModelBend>>& innerBends, 
                        std::map<FaceID, std::shared_ptr<Bend::ModelBend>>& outerBends);

        std::map<FaceID, std::shared_ptr<Face::ModelFace>> getFaces() const;
        std::map<FaceID, std::shared_ptr<Bend::ModelBend>> getBends() const;

        double getThickness() const;

        void assignFaceAttributes(FaceID faceID, TopoDS_Shape& aShape);

        void classifyFaces();

        void computeBendAngles();

        bool cleanModel();
    };
}
}