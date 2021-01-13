#pragma once

#include "LoggingFacility.h"

namespace Fxt 
{
namespace Logging
{
    class StandardOutputLogger : public ILoggingFacility
    {
    public:
        StandardOutputLogger(std::string_view username) : ILoggingFacility(username){ }

    };
}
}