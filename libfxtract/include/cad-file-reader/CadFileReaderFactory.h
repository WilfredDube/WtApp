#pragma once

#include "CadFileReader.h"

namespace Fxt
{
namespace CadFileReader 
{
    enum class CadFileFormat { UNKNOWN_FILE_FORMAT, IGES_FILE_FORMAT, STEP_FILE_FORMAT };

    class CadFileReaderFactory
    {
        inline CadFileFormat checkFileFormat(const std::string& fileExtension) const;
    public:
        std::shared_ptr<Fxt::CadFileReader::CadFileReader> createReader(const std::string& filename);
    };
}
}