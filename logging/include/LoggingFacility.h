#pragma once

#include <memory>
#include <string_view>

namespace Fxt 
{
namespace Logging
{
    class ILoggingFacility
    {
    public:
        virtual ~ILoggingFacility() = default;

        virtual void writeInfoEntry(std::string_view entry) = 0;
        virtual void writeWarnEntry(std::string_view entry) = 0;
        virtual void writeErrorEntry(std::string_view entry) = 0;
    };
}
}

using Logger = std::shared_ptr<Fxt::Logging::ILoggingFacility>;