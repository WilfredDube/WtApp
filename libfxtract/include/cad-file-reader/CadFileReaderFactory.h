#pragma once

#include "CadFileReader.h"

#include "../../../logging/include/LoggingFacility.h"

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

        Logger logger;
    public:
        explicit CadFileReaderFactory(const Logger& loggingService);

        CadFileReaderPtr createReader(const std::string& filename);
    };
}
}