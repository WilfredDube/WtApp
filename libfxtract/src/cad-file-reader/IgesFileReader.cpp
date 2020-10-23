#include "../../include/cad-file-reader/CadFileReader.h"
#include "../../include/cad-file-reader/IgesFileReader.h"
#include "../../include/cad-file-reader/ReaderCreationFailedException.h"

#include "../../include/ModelUtils.h"

#include <IGESControl_Reader.hxx>

using namespace Fxt::CadFileReader;

void IgesFileReader::makeReader(const std::string& igesFile)
{
    mMyIgesReader = std::make_shared<IGESControl_Reader>();

    std::cout << "File .......... : " << igesFile << '\n';
    mMyIgesReader->ReadFile (igesFile.c_str());

    IFSelect_PrintCount mode = IFSelect_ListByItem;
    mMyIgesReader->PrintCheckLoad(false, mode);

    //selects all IGES faces in the file and puts them into a list  called //MyList,
    Handle(TColStd_HSequenceOfTransient) myList =  mMyIgesReader->GiveList("iges-faces");
    mMyIgesReader->TransferRoots();

    // //translates MyList,
    myList->Length();
    mMyIgesReader->TransferList(myList);
}

void IgesFileReader::extractFaces(Model& model, const std::string& filename)
{
  makeReader(filename);

  if(mMyIgesReader == nullptr)
    throw ReaderCreationFailedException("Failed to create IGES file reader.");

  if (mMyIgesReader->NbShapes() <= 0)
    throw std::invalid_argument("Unable to read model.");

  FaceID faceID = 0;
  size_t numShapes = mMyIgesReader->NbShapes();

  /* Go through the shape object and extract the faces. */
  for (size_t i = 1; i <= numShapes; i++) {
    TopoDS_Shape aShape = mMyIgesReader->Shape(i);

    ++faceID;

    model.assignFaceAttributes(faceID, aShape);
  }
}

