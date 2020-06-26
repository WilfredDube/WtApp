#include "../include/StepProcessor.h"
#include "../include/ModelUtils.h"

namespace Fxt {

  void StepProcessor::extractFaces(Model& model_, const XSControl_Reader reader)
  {
    if (reader.NbShapes() <= 0)
      throw std::invalid_argument("Unable to read model.");

    FaceID faceID = 0;
    TopoDS_Shape topoDSFaces;
    size_t numShapes = reader.NbShapes();

    /* Go through the shape object and extract the faces. */
    for (size_t i = 1; i <= numShapes; i++) {
      topoDSFaces = reader.Shape(i);
      TopExp_Explorer myFaceExplorer(topoDSFaces, TopAbs_FACE);

      while (myFaceExplorer.More()){
        TopoDS_Shape aShape = myFaceExplorer.Current();

        ++faceID;

        model_.assignFaceAttributes(faceID, aShape);

        myFaceExplorer.Next();
      }
    }
  }

}
