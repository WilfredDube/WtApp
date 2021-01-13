#pragma once

#include "LoggingFacility.h"

namespace Fxt 
{
namespace Logging
{
    class FileSystemLogger : public ILoggingFacility
    {
    public:
        void writeInfoEntry(std::string_view username, std::string_view entry, std::string_view other = "") override;
        void writeWarnEntry(std::string_view username, std::string_view entry, std::string_view other = "") override;
        void writeErrorEntry(std::string_view username, std::string_view entry, std::string_view other = "") override;
    };
}
}