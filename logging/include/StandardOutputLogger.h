#pragma once

#include "LoggingFacility.h"

class StandardOutputLogger : public ILoggingFacility
{
public:
    void writeInfoEntry(std::string_view& entry) override;
    void writeWarnEntry(std::string_view& entry) override;
    void writeErrorEntry(std::string_view& entry) override;
};