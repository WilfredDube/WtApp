#pragma once

#include "CadFileReader.h"

class STEPControl_Reader;

namespace Fxt 
{
namespace CadFileReader
{
    class StepFileReader : public Fxt::CadFileReader::CadFileReader 
    {
        std::shared_ptr<STEPControl_Reader> mMyStepReader = nullptr;
    public:
        void makeReader(const std::string& filename);
        void extractFaces(std::shared_ptr<Fxt::SheetMetalComponent::SheetMetalFeature>& model, const std::string& filename);
    };
}
}