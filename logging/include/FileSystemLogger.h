#pragma once

#include "LoggingFacility.h"

namespace Fxt 
{
namespace Logging
{
    class FileSystemLogger : public ILoggingFacility
    {
    public:
        FileSystemLogger(std::string_view username) : ILoggingFacility(username){ }

    };
}
}