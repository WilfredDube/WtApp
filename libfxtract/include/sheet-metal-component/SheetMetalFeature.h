#pragma once

#include "ModelTypes.h"
#include "../../include/sheet-metal-component/bend/ModelBend.h"
#include "../../include/sheet-metal-component/face/ModelFace.h"

#include <TopoDS_Shape.hxx>

#include <ostream>
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

        long double mThickness {0.0};

        std::shared_ptr<TopoDS_Shape> mTopologicalShape;

        void removeOuterFaces();

        void reAssignFaceId(bool isBend = false);

        void setThicknessDefiningFaceAttributes();

        void setFaceIdWrapper(std::map<FaceID, std::shared_ptr<Face::ModelFace>>& mFaces);
        void setBendIdWrapper(std::map<FaceID, std::shared_ptr<Bend::ModelBend>>& mBends);

        /**
         * After reassigning the bend and face ID's the is a disconnection between faces and bends
         * because the JoiningFaceId to which the bends were pointing no long exit.
         * 
         * Method : searches the face list (mModelFaces) to find the faces to which each bend is 
         *           connected too.
         */
        void connectBendsToNewFaceId();

    public:
        void addModelFace(std::shared_ptr<Face::ModelFace>& modelFace);

        void addModelBend(std::shared_ptr<Bend::ModelBend>& modelBend);

        std::shared_ptr<gp_Dir> getNormalByFaceID(const FaceID faceID) const;

        FaceID getIdOfBendWithJoiningFaceID(const FaceID currbendID, const FaceID faceID) const;
        
        /**
         * Seperates bends into inner and outer bend faces.
         */
        bool splitModelBends(const FaceID id, std::map<FaceID, 
                        std::shared_ptr<Bend::ModelBend>>& innerBends, 
                        std::map<FaceID, std::shared_ptr<Bend::ModelBend>>& outerBends);

        std::map<FaceID, std::shared_ptr<Face::ModelFace>> getFaces() const;
        std::map<FaceID, std::shared_ptr<Bend::ModelBend>> getBends() const;

        double getThickness() const;

        void assignFaceAttributes(const FaceID faceID, std::shared_ptr<TopoDS_Shape>& aShape);

        void classifyFaces();

        /**
         *  Method removes outer faces leaving inner faces for easy computation and representation 
         *  of bend features.
         *  Fixes the problem of processing a bends twice because of it has an inner and outer face.
         */
        bool removeOuterBendFaces();

        bool reduceModelSize();

        void computeBendAngles();

        friend std::ostream& operator<<(std::ostream& os, const std::shared_ptr<SheetMetalFeature>& sheetMetal)
        {
            using namespace Fxt::SheetMetalComponent::Bend;

            os << "Thickness : " << sheetMetal->getThickness() << '\n';

            for(const auto& [bendId, bend] : sheetMetal->getBends()){
                os << bend << '\n';
            }
            
            return os;
        }
    };
}
}