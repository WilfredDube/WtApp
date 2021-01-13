#include "../../include/cad-file-reader/CadFileReaderFactory.h"
#include "../../include/cad-file-reader/IgesFileReader.h"
#include "../../include/cad-file-reader/StepFileReader.h"
#include "../../include/cad-file-reader/NullFileReader.h"

using namespace Fxt::CadFileReader;

CadFileReaderFactory::CadFileReaderFactory(const Logger& loggingService) 
: logger { loggingService } 
{}

inline CadFileReaderFactory::CadFileFormat CadFileReaderFactory::checkFileFormat(const std::string& fileName) const
{
    CadFileReaderFactory::CadFileFormat cadFileFormat;

    if (fileName.find("igs") != std::string::npos || fileName.find("iges") != std::string::npos) {
      cadFileFormat = CadFileFormat::IGES_FILE_FORMAT;
    } else if (fileName.find("IGS") != std::string::npos || fileName.find("IGES") != std::string::npos) {
      cadFileFormat = CadFileFormat::IGES_FILE_FORMAT;
    } else if (fileName.find("step") != std::string::npos || fileName.find("stp") != std::string::npos) {
      cadFileFormat = CadFileFormat::STEP_FILE_FORMAT;
    } else if (fileName.find("STEP") != std::string::npos || fileName.find("STP") != std::string::npos) {
      cadFileFormat = CadFileFormat::STEP_FILE_FORMAT;
    } else {
      cadFileFormat = CadFileFormat::UNKNOWN_FILE_FORMAT;
    }

    return cadFileFormat;
}

std::shared_ptr<CadFileReader> CadFileReaderFactory::createReader(const std::string& filename)
{
  CadFileFormat format = checkFileFormat(filename);      

  if(format == CadFileFormat::STEP_FILE_FORMAT)
    return std::make_shared<StepFileReader>();
  else if(format == CadFileFormat::IGES_FILE_FORMAT)
    return std::make_shared<IgesFileReader>();

  std::string_view warningMessage { filename + "-> Unknown file format : Fxtract only accepts iges and step file formats." };
  logger->writeErrorEntry(warningMessage);

  return { std::make_shared<NullFileReader>() };
}
