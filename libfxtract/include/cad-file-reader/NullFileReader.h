#pragma once

#include "CadFileReader.h"

namespace Fxt
{
namespace CadFileReader
{    
    class NullFileReader : public CadFileReader 
    {
    public:
        void makeReader(const std::string& filename) override {}
        void extractFaces(SheetMetalFeaturePtr& model, const std::string& filename) override {}
        bool isUsable() const noexcept override { return false; }
    };
}
}