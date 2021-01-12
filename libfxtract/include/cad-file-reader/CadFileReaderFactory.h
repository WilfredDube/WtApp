#pragma once

#include "CadFileReader.h"

namespace Fxt
{
namespace CadFileReader 
{
    class CadFileReaderFactory
    {
        enum class CadFileFormat { 
            UNKNOWN_FILE_FORMAT, 
            IGES_FILE_FORMAT, 
            STEP_FILE_FORMAT
        };

        inline CadFileFormat checkFileFormat(const std::string& fileExtension) const;
    public:
        std::shared_ptr<Fxt::CadFileReader::CadFileReader> createReader(const std::string& filename);
    };
}
}