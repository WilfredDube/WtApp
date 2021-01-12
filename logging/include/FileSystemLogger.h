#pragma once

#include "LoggingFacility.h"

namespace Fxt 
{
namespace Logging
{
    class FileSystemLogger : public ILoggingFacility
    {
    public:
        void writeInfoEntry(std::string_view& entry) override;
        void writeWarnEntry(std::string_view& entry) override;
        void writeErrorEntry(std::string_view& entry) override;
};
}
}