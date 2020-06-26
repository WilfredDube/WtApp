#include <memory>

#include "../include/ModelReader.h"
#include "../include/IgesProcessor.h"
#include "../include/StepProcessor.h"

using namespace std;

namespace Fxt {

  FileFormat ModelReader::checkFileFormat(const string fileName){
    std::cout << "/* message */" << '\n';
    if (fileName.find("igs") != std::string::npos || fileName.find("iges") != std::string::npos) {
      return FileFormat::IGES_FORMAT;
    } else if (fileName.find("IGS") != std::string::npos || fileName.find("IGES") != std::string::npos) {
      return FileFormat::STEP_FORMAT;
    } else if (fileName.find("step") != std::string::npos || fileName.find("stp") != std::string::npos) {
      return FileFormat::STEP_FORMAT;
    } else if (fileName.find("STEP") != std::string::npos || fileName.find("STP") != std::string::npos) {
      return FileFormat::STEP_FORMAT;
    } else {
      throw invalid_argument("Unknown file format : Fxtract only accepts iges and step file formats.");
    }

  }

  void ModelReader::processModel(const string fileName)
  {
    FileFormat fileFormat = checkFileFormat(fileName);

    processModelFile(fileFormat, fileName.c_str());
  }

  void ModelReader::processModelFile(FileFormat fileFormat, const char* fileName)
  {
    auto igesProcessor = make_unique<IgesProcessor>();
    auto stepProcessor = make_unique<StepProcessor>();
    IGESControl_Reader myIgesReader;
    STEPControl_Reader myStepReader;

    switch (fileFormat) {

      case FileFormat::IGES_FORMAT:
          myIgesReader = processIgesFile(fileName);
          igesProcessor->extractFaces(model_, myIgesReader);
          break;
      case FileFormat::STEP_FORMAT:
          myStepReader = processStepFile(fileName);
          stepProcessor->extractFaces(model_, myStepReader);
          break;
      default:
      throw invalid_argument("Unknown file format : Fxtract only accepts iges and step file formats.");
      break;

    }

  }

  IGESControl_Reader ModelReader::processIgesFile(const char* igesFile){
    IGESControl_Reader myIgesReader;

    std::cout << "File .......... : " << igesFile << '\n';
    myIgesReader.ReadFile (igesFile);

    IFSelect_PrintCount mode = IFSelect_ListByItem;
    myIgesReader.PrintCheckLoad(false, mode);

    //selects all IGES faces in the file and puts them into a list  called //MyList,
    Handle(TColStd_HSequenceOfTransient) myList =  myIgesReader.GiveList("iges-faces");
    myIgesReader.TransferRoots();

    // //translates MyList,
    myList->Length();
    myIgesReader.TransferList(myList);

    return myIgesReader;
  }

  STEPControl_Reader ModelReader::processStepFile(const char* stepFile){
    STEPControl_Reader myStepReader;

    myStepReader.ReadFile(stepFile);

    std::cout << "File .......... : " << stepFile << '\n';
    IFSelect_PrintCount mode = IFSelect_ListByItem;
    myStepReader.PrintCheckLoad(false, mode);

    //Transfer whole file
    Standard_Integer NbRoots = myStepReader.NbRootsForTransfer();
    myStepReader.ClearShapes();

    for (Standard_Integer n = 1; n<= NbRoots; n++) {
      myStepReader.TransferRoot(n);
    }

    return myStepReader;
  }

}
