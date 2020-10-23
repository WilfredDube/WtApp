#pragma once

#include "CadFileReader.h"

class IGESControl_Reader;

namespace Fxt
{
namespace CadFileReader
{    
    class IgesFileReader : public Fxt::CadFileReader::CadFileReader 
    {
        std::shared_ptr<IGESControl_Reader> mMyIgesReader = nullptr;
    public:
        void makeReader(const std::string& filename);
        void extractFaces(Model& model, const std::string& filename);
    };
}
}