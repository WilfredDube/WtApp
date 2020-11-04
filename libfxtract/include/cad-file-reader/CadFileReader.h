#pragma once

#include "../../include/sheet-metal-component/SheetMetalFeature.h"
#include "UnknownFileFormatException.h"

#include <memory>
#include <string>
#include <XSControl_Reader.hxx>

class XSControl_Reader;

namespace Fxt 
{
namespace CadFileReader
{
    class CadFileReader 
    {
    protected:
        virtual void makeReader(const std::string& filename) = 0;
    public:
        virtual void extractFaces(std::shared_ptr<Fxt::SheetMetalComponent::SheetMetalFeature>& model, const std::string& filename) = 0;
    };
}
}
