#include "../include/StandardOutputLogger.h"

#include <iostream>

using namespace std;
using namespace Fxt::Logging;

void StandardOutputLogger::writeInfoEntry(std::string_view username, std::string_view entry, std::string_view other)
{
    std::cout << "INFO: " << entry << '\n';
}

void StandardOutputLogger::writeWarnEntry(std::string_view username, std::string_view entry, std::string_view other)
{
    std::cout << "WARN: " << entry << '\n';
}

void StandardOutputLogger::writeErrorEntry(std::string_view username, std::string_view entry, std::string_view other)
{
    std::cout << "ERROR: " << entry << '\n';
}