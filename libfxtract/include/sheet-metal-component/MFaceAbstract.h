#pragma once

#include "ModelTypes.h"
#include "./edge/ModelEdge.h"

#include <GeomAbs_CurveType.hxx>
#include <Interface_Static.hxx>
#include <TopAbs_ShapeEnum.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Face.hxx>

#include <memory>
#include <vector>

namespace Fxt
{
namespace SheetMetalComponent
{
    using FaceID = Fxt::SheetMetalComponent::ModelTypes::FaceID;
    using EdgeID = Fxt::SheetMetalComponent::ModelTypes::EdgeID;
    using FaceType = Fxt::SheetMetalComponent::ModelTypes::FaceType;
    using PlaneType = Fxt::SheetMetalComponent::ModelTypes::PlaneType;

    class MFaceAbstract
    {
    protected:
        FaceID mFaceID; /*!< an integer value  represents the Face Identification number */
        PlaneType mPlaneType;
        FaceType mFaceType;

        std::shared_ptr<gp_Dir> mFaceNormal; //!< the normal vector to the face.
        std::shared_ptr<gp_Dir> mFaceUnitNormal; //!< the unit normal vector to the face.

        std::vector<std::shared_ptr<Edge::ModelEdge>> mFaceEdges; //!< edges of the face.

        std::shared_ptr<TopoDS_Face> mModelFace;
    public:
        MFaceAbstract() = default;

        MFaceAbstract(const FaceID faceID = 0, std::shared_ptr<TopoDS_Face> topoDSFace = nullptr);

        virtual void init() = 0;

        void setTModelFace(std::shared_ptr<TopoDS_Face>& tface);
        std::shared_ptr<TopoDS_Face> getTModelFace() const;

        void setFaceId(FaceID faceID);
        FaceID getFaceId() const;

        void setUnitNormal(std::shared_ptr<gp_Dir> unit_normal);
        std::shared_ptr<gp_Dir> getUnitNormal() const;

        std::shared_ptr<gp_Dir> getFaceNormal() const;
        void computeFaceNormal();

        void setFaceType(FaceType ftype);
        FaceType getFaceType() const;

        void setPlaneType(PlaneType ptype);
        PlaneType getPlaneType() const;

        void extractEdges();

        void setFaceEdgePosition();

        void addEdge(const std::shared_ptr<Edge::ModelEdge>& edge);

        std::vector<std::shared_ptr<Edge::ModelEdge>>& getFaceEdges();

        std::shared_ptr<Edge::ModelEdge> getFaceEdgesAt(const int index) const;

        // void getFaceEdgePosition() const; 
    };
}
}