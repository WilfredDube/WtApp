#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include "../libfxtract/include/Model.h"

#include <vector>

// Class representing individual bend in population 
class Sequence 
{ 
    static double min_dist;
public:
    std::vector<int> chromosome;
    unsigned nTools, nRotations, nFlips;
    double distance;

    size_t targetSize; 
    double fitness;
    Fxt::Model model;
    
    Sequence(std::vector<int> chromosome, Fxt::Model& model, size_t target_size); 
    Sequence mate(Sequence parent2); 
    double cal_fitness();
    
    bool operator<(const Sequence &ind2) 
    { 
        return fitness > ind2.fitness; 
    }

    bool operator==(const Sequence& ind2);
};

#endif