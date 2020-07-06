#ifndef MODEL_READER_H_
#define MODEL_READER_H_

#include <XSControl_Reader.hxx>
#include <IGESControl_Reader.hxx>
#include <STEPControl_Reader.hxx>
#include "ModelUtils.h"
#include "Model.h"

namespace Fxt {

  class ModelReader
  {
  private:
    /**
    *   Parses an IGES/STEP model file and returns a reader containing all the
    *   topological objects.
    */
    IGESControl_Reader processIgesFile(const char* igesFile);
    STEPControl_Reader processStepFile(const char* stepFile);

    /**
    *   Checks and returns the file format on a file.
    */
    FileFormat checkFileFormat(const std::string fileName);

    /*
    *   Extracts all the features in a model based on the uploaded file format.
    */
    void processModelFile(FileFormat fileFormat, const char* fileName);

  public:
    ModelReader(Model& model) : model_(model){}
    ~ModelReader() { std::cout << "ModelReader Deleted...." << std::endl; }

    // Interface for processModelFile()
    void processModel(const std::string fileName);

  private:
    //! The current model being processed.
    Model& model_;
  };

}
#endif // MODEL_READER_H_
