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
        double distance{0}, fitness{0.0};

        Sequence(std::vector<int> chromosome) : chromosome { chromosome }, targetSize { chromosome.size() } {}

        double cal_fitness(SheetMetalFeaturePtr& sheetMetalFeature);
        
        Sequence mate(Sequence parentSequence);
        bool operator<(const Sequence &individualSequence);
        bool operator==(const Sequence& individualSequence);
    };

    std::shared_ptr<Sequence> sequenceImpl_;

    SheetMetalPtr& sheetMetalFeature;

public:
    BendSequenceGenerator() = delete;
    explicit BendSequenceGenerator(std::vector<int> chromosome, SheetMetalFeaturePtr& model);

    void generateBendingSequence();
    void print();
};