#pragma once

namespace Fxt
{
namespace SheetMetalComponent
{
namespace ModelTypes
{
    using FaceID = unsigned int;
    using EdgeID = unsigned int;

    enum class EdgeType { DEFAULT = -1, LINE, ARC };
    enum class EdgePosition { DEFAULT = -1, JOINING_EDGE, SIDE_EDGE, DISJOINT_EDGE };

    enum class PlaneType { PLANAR, NON_PLANAR };
    enum class FaceType { NONE, THICKNESS_DEFINING_FACE, BEND_FACE, BEND_SIDE, FACE };

    enum class BendType { UNASSIGNED = -1, INTERNAL, EXTERNAL };
    enum class BendOrientation { PARALLEL,  PERPENDICULAR };
}
}
}