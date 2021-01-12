#include "../include/StandardOutputLogger.h"

#include <iostream>

using namespace std;
using namespace Fxt::Logging;

void StandardOutputLogger::writeInfoEntry(std::string_view entry)
{
    std::cout << "INFO: " << entry << '\n';
}

void StandardOutputLogger::writeWarnEntry(std::string_view entry)
{
    std::cout << "WARN: " << entry << '\n';
}

void StandardOutputLogger::writeErrorEntry(std::string_view entry)
{
    std::cout << "ERROR: " << entry << '\n';
}