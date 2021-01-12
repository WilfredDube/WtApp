#pragma once

namespace Fxt
{
namespace SheetMetalComponent
{
namespace ModelTypes
{
    using FaceID = unsigned int;
    using EdgeID = unsigned int;

    enum class FaceType { NONE, THICKNESS_DEFINING_FACE, BEND_FACE, BEND_SIDE, FACE };

    enum class BendOrientation { PARALLEL,  PERPENDICULAR };
}
}
}

using FaceID = Fxt::SheetMetalComponent::ModelTypes::FaceID;
using EdgeID = Fxt::SheetMetalComponent::ModelTypes::EdgeID;
using FaceType = Fxt::SheetMetalComponent::ModelTypes::FaceType;
