#include "../../include/cad-file-reader/CadFileReader.h"
#include "../../include/cad-file-reader/StepFileReader.h"
#include "../../include/cad-file-reader/ReaderCreationFailedException.h"

#include "../../include/ModelUtils.h"

#include <STEPControl_Reader.hxx>

using namespace Fxt::CadFileReader;

void StepFileReader::makeReader(const std::string& stepFile)
{
    mMyStepReader = std::make_shared<STEPControl_Reader>();

    mMyStepReader->ReadFile(stepFile.c_str());

    std::cout << "File .......... : " << stepFile << '\n';
    IFSelect_PrintCount mode = IFSelect_ListByItem;
    mMyStepReader->PrintCheckLoad(false, mode);

    //Transfer whole file
    Standard_Integer NbRoots = mMyStepReader->NbRootsForTransfer();
    mMyStepReader->ClearShapes();

    for (Standard_Integer n = 1; n<= NbRoots; n++) {
      mMyStepReader->TransferRoot(n);
    }
}

void StepFileReader::extractFaces(Model& model, const std::string& filename)
{
  makeReader(filename);

  if(mMyStepReader == nullptr)
    throw ReaderCreationFailedException("Failed to create STEP file reader.");

  if (mMyStepReader->NbShapes() <= 0)
    throw std::invalid_argument("Unable to read model.");

  FaceID faceID = 0;
  TopoDS_Shape topoDSFaces;
  size_t numShapes = mMyStepReader->NbShapes();

  /* Go through the shape object and extract the faces. */
  for (size_t i = 1; i <= numShapes; i++) {
    topoDSFaces = mMyStepReader->Shape(i);
    TopExp_Explorer myFaceExplorer(topoDSFaces, TopAbs_FACE);

    while (myFaceExplorer.More()){
      TopoDS_Shape aShape = myFaceExplorer.Current();

      ++faceID;

      model.assignFaceAttributes(faceID, aShape);

      myFaceExplorer.Next();
    }
  }
}