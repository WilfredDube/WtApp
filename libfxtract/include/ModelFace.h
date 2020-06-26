#ifndef MODEL_FACE_H_
#define MODEL_FACE_H_

#include "MFace.h"
#include <TopoDS_Face.hxx>

namespace Fxt {

  class ModelFace:  public MFace
  {
  public:
    ModelFace(FaceID faceID, TopoDS_Face& topoDSFace);
    // ~ModelFace() { std::cout << "ModelFace Deleted...." << std::endl; }

    void init();
  };
}

#endif // MODEL_FACE_H_
