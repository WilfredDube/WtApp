#pragma once

#include "../../libfxtract/include/sheet-metal-component/SheetMetalFeature.h"

#include <memory>
#include <vector>

class BendSequenceGenerator
{
    struct Sequence
    {
        std::vector<int> chromosome;
        size_t nTools{1}, nRotations{0}, nFlips{0}, targetSize;
        double distance{0}, fitness;

        Sequence(std::vector<int> chromosome) : chromosome { chromosome }, targetSize { chromosome.size() } {}

        double cal_fitness(Fxt::SheetMetalComponent::SheetMetalFeature& model);
        
        Sequence mate(Sequence parentSequence);
        bool operator<(const Sequence &individualSequence);
        bool operator==(const Sequence& individualSequence);
    };

    std::shared_ptr<Sequence> sequenceImpl_;

    void print(const std::vector<Sequence>& new_generation);

public:
    BendSequenceGenerator() = default;
    BendSequenceGenerator(std::vector<int> chromosome, Fxt::SheetMetalComponent::SheetMetalFeature& model);

    Sequence generateBendingSequence();
};