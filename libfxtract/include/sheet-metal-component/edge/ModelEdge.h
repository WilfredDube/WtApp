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

    class ModelEdge
    {
    public:
        enum class EdgePosition 
        { 
            DEFAULT = -1, 
            JOINING_EDGE, 
            SIDE_EDGE, 
            DISJOINT_EDGE 
        };

        enum class EdgeType { 
            DEFAULT = -1, 
            LINE, 
            ARC 
        };

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

        bool operator==(const ModelEdge& otherEdge) const;
    };
}
}
}