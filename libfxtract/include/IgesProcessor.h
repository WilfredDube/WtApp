#ifndef IGES_PROCESSOR_H_
#define IGES_PROCESSOR_H_

#include <XSControl_Reader.hxx>
#include "Model.h"

namespace Fxt {

  class IgesProcessor
  {
  public:
    /**
    *   Extracts all the TopoDS_Face objects from a step file model
    *   @param model_ the current model from which to extract TopoDS_Faces
    *   @param reader a IGESControl_Reader object
    */
    void extractFaces(Model& model_, const XSControl_Reader reader) const;

    ~IgesProcessor() { std::cout << "IgesProcessor Deleted...." << std::endl; }
  };
}
#endif // IGES_PROCESSOR_H_
