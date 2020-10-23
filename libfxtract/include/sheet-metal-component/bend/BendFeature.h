#pragma once

#include "../BoostSerializer.h"
#include "../edge/ModelEdge.h"
#include "../MFaceAbstract.h"
#include "../Point.h"
#include "../../Computation.h"

#include <gp_Lin.hxx>
#include <TopoDS_Face.hxx>
#include <vector>

namespace Fxt
{
namespace SheetMetalComponent
{
namespace Bend
{
    class BendFeature
    {
        using FaceID = Fxt::SheetMetalComponent::ModelTypes::FaceID;

        long double mBendLength;
        long double mCurvature; //!< face curvature value : zero for planar type faces and non-zero for non-planar type faces.

        //!< the face ID of the face connected to the bend face via its Straight edge.
        FaceID mJoinedFaceId1;

        //!< the face ID of the second face connected to the bend face via its Straight edge.
        FaceID mJoinedFaceId2;

        long double mBendingForce; //!< force required to create the bend.
        long double mBendAngle; //!< bend angle.
        long double mBendRadius; //!< bend radius : zero if face is planar and non-zero if face is a bend.
        size_t mBendDirection;

        // std::shared_ptr<BendComputation> mBendComputation;
    public:

        void setJoiningFaceID1(const FaceID mFaceID);
        FaceID getJoiningFaceID1() const;

        void setJoiningFaceID2(const FaceID mFaceID);
        FaceID getJoiningFaceID2() const;

        void setBendAngle(const long double angle);
        long double getBendAngle() const;

        void setBendDirection(const size_t bend_direction);
        size_t getBendDirection() const;

        void setBendLength(const long double bendLength);
        long double getBendLength() const;

        void setCurvature(const long double cv);
        long double getBendRadius() const;
        
        // TODO : calculate Bending force, direction, orientation & distance
    };
}
}
}