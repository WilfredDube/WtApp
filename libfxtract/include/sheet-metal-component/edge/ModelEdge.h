#pragma once

#include "../ModelTypes.h"

#include <BRep_Tool.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <BRepLProp_SLProps.hxx>
#include <GeomConvert.hxx>
#include <memory>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include <vector>

namespace Fxt
{
namespace SheetMetalComponent
{
namespace Edge
{
    using EdgeID = Fxt::SheetMetalComponent::ModelTypes::EdgeID;
    using FaceID = Fxt::SheetMetalComponent::ModelTypes::FaceID;
    using EdgeType = Fxt::SheetMetalComponent::ModelTypes::EdgeType;
    using EdgePosition = Fxt::SheetMetalComponent::ModelTypes::EdgePosition;

    class ModelEdge
    {
        EdgeID mEdgeID;
        EdgeType mEdgeType;
        EdgePosition mEdgePosition;

        std::shared_ptr<gp_Dir> mEdgeLineVector;

        long double mEdgeLength;

        FaceID mJoinedToFaceID;

        //! Whether its a straight line (rational = 0) or arc (rational = 1)
        bool mIsrational;

        //! Edge ID of an edge in the Model shared by another face.
        EdgeID mEdgeSameAsEdgeID;

        std::shared_ptr<TopoDS_Edge> mModelEdge;
    public:
        ModelEdge() = default;
        ModelEdge(std::shared_ptr<TopoDS_Edge> edge);

        std::shared_ptr<TopoDS_Edge> getTModelEdge() const;

        std::vector<std::shared_ptr<gp_Pnt>> getEdgeEndPoints() const;

        void setIsRational(bool isRational);
        bool IsRational() const;

        void setEdgeNum(const EdgeID edgeID);
        EdgeID getEdgeNum() const;

        void setEdgeType(const GeomAbs_CurveType type);
        EdgeType getEdgeType() const;

        void setEdgePosition(const EdgePosition edgePosition);
        EdgePosition getEdgePosition() const;

        void setEdgeLength(const long double edgeLength);
        long double getEdgeLength() const;

        void setEdgeLineVector();
        std::shared_ptr<gp_Dir> getEdgeLineVector();

        void printEdgePosition();

        void setJoiningFaceID(const FaceID id);
        FaceID getJoiningFaceID();

        bool operator==(const std::shared_ptr<ModelEdge>& otherEdge) const;
    };
}
}
}