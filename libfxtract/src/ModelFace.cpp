#include "../include/ModelFace.h"
#include "../include/ModelMath.h"

namespace Fxt {
  ModelFace::ModelFace(FaceID faceID, TopoDS_Face& topoDSFace)
  {
    setFaceId(faceID);
    setTModelFace(topoDSFace);
    setPlaneType(PlaneType::PLANAR);
    setFaceType(FaceType::NONE);

    init();
  }

  void ModelFace::init()
  {
    extractEdges();

    computeFaceNormal();
    setUnitNormal(computeUnitNormal(getTModelFace()));
  }

}
