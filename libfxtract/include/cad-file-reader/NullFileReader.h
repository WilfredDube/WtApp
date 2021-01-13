#pragma once

#include "CadFileReader.h"

namespace Fxt
{
namespace CadFileReader
{    
    class NullFileReader : public CadFileReader 
    {
    public:
        void makeReader(const std::string& filename) {}
        void extractFaces(SheetMetalFeaturePtr& model, const std::string& filename){}
        bool isUsable() const noexcept override { return false; }
    };
}
}