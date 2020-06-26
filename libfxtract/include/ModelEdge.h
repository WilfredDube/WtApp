#ifndef MODEL_EDGE_H_
#define MODEL_EDGE_H_

#include <BRep_Tool.hxx>
#include <BRepAdaptor_Curve.hxx>
#include <GeomConvert.hxx>
#include <BRepLProp_SLProps.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopExp_Explorer.hxx>
#include <vector>

namespace Fxt {

  using EdgeID = size_t;
  enum class EdgeType { DEFAULT = -1, LINE, ARC };
  enum class EdgePosition { DEFAULT = -1, JOINING_EDGE, SIDE_EDGE, DISJOINT_EDGE };

  class ModelEdge
  {
    EdgeID mEdgeNumber;
    EdgeType mEdgeType;
    EdgePosition mEdgePosition;
    gp_Dir mEdgeLineVector;
    Standard_Real mEdgeLength;
    size_t joinedToFaceID;

    //! Whether its a straight line (rational = 0) or arc (rational = 1)
    bool isrational;

    //! Edge ID of an edge in the Model shared by another face.
    Standard_Integer mEdgeSameAsEdgeID;

    TopoDS_Edge mModelEdge;
  public:
    ModelEdge();
    ModelEdge(TopoDS_Edge edge);

    TopoDS_Edge getTModelEdge() const;

    std::vector<gp_Pnt> getEdgeEndPoints() const;

    void setIsRational(bool is_it);
    bool IsRational() const;

    void setEdgeNum(EdgeID enumb);
    EdgeID getEdgeNum() const;

    void setEdgeType(GeomAbs_CurveType type);
    EdgeType getEdgeType() const;

    void setEdgePosition(EdgePosition epos);
    EdgePosition getEdgePosition() const;

    void setEdgeLength(Standard_Real len);
    Standard_Real getEdgeLength() const;

    void setEdgeLineVector();
    gp_Dir getEdgeLineVector();

    void printEdgePosition();

    void setJoiningFaceID(size_t id);
    size_t getJoiningFaceID();

    bool operator==(ModelEdge const& edge) const;
  };

}

#endif // MODEL_EDGE_H_
