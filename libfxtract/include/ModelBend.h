#ifndef MODEL_BEND_H_
#define MODEL_BEND_H_

#include <TopoDS_Face.hxx>
#include <gp_Lin.hxx>
#include <vector>
#include "ModelEdge.h"
#include "ModelUtils.h"
#include "MFace.h"
#include "ModelMath.h"

namespace Fxt {

  struct BendDistance {
    FaceID connectedFace;
    Standard_Real distance;
    FaceID bendID;
  };

  class Point {
    friend class boost::serialization::access;
    friend std::ostream & operator<<(std::ostream &os, const Point &p);

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
      // save/load base class information
      ar & X & Y & Z;
    }

  public:
    long double X, Y, Z;
  };

  class ModelBend : public MFace
  {
    friend class boost::serialization::access;
    friend std::ostream & operator<<(std::ostream &os, const ModelBend &br);

    Standard_Real mBendLength;
    BendDistance mBendDistance_Left, mBendDistance_Right;
    Standard_Real mCurvature; //!< face curvature value : zero for planar type faces and non-zero for non-planar type faces.

    //!< the face ID of the face connected to the bend face via its Straight edge.
    FaceID mJoinedFaceId1;

    //!< the face ID of the second face connected to the bend face via its Straight edge.
    FaceID mJoinedFaceId2;

    Standard_Real mBendingForce; //!< force required to create the bend.
    Standard_Real mBendAngle; //!< bend angle.
    unsigned long mBendSameAsId;
    BendType mBendType;
    Standard_Real mBendRadius; //!< bend radius : zero if face is planar and non-zero if face is a bend.
    size_t mBendDirection;

    gp_Lin bendLine_;

    Point bendLinePnt, bendLineDir;

    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        // save/load base class information
        ar & boost::serialization::base_object<MFace>(*this);
        ar & mBendAngle & mBendDirection;
        ar & bendLinePnt & bendLineDir;

        if (Archive::is_loading::value) makeBendLine();
    }
    
  public:

    ModelBend(const FaceID& faceID = 0, TopoDS_Face topoDSFace = {});
    // ~ModelBend() { std::cout << "ModelBend Deleted...." << std::endl; }

    void init();

    void makeBendLine();
    void setJoiningFaceID1(FaceID mFaceID);
    FaceID getJoiningFaceID1();

    void setJoiningFaceID2(FaceID mFaceID);
    FaceID getJoiningFaceID2();

    void setBendAngle(long double angle);
    long double getBendAngle();

    void setBendDirection(size_t bend_direction);
    size_t getBendDirection();

    Standard_Real getBendLength();
    void setBendLength(std::vector<ModelEdge> mFaceEdges);

    void setCurvature(Standard_Real cv);
    void setBendRadius(Standard_Real bendRadius);
    Standard_Real getBendRadius();

    void computeBendLine();
    gp_Lin getBendLine();

    double computeBendDistance(ModelBend& otherBend);

    bool isParallel(ModelBend& otherBend);
    
    // TODO : calculate Bending force, direction, orientation & distance
  };
}

#endif // MODEL_FACE_H_
