#ifndef GA_INDIVIDUAL_H_
#define GA_INDIVIDUAL_H_

#include "../libfxtract/include/Model.h"

#include <vector>

// Class representing individual bend in population 
class Individual 
{ 
    static double min_dist;
public:
    std::vector<int> chromosome;
    unsigned nTools, nRotations, nFlips;
    double distance;

    size_t targetSize; 
    double fitness;
    Fxt::Model model;
    
    Individual(std::vector<int> chromosome, Fxt::Model& model, size_t target_size); 
    Individual mate(Individual parent2); 
    double cal_fitness();
    
    // Overloading < operator 
    bool operator<(const Individual &ind2) 
    { 
        return fitness > ind2.fitness; 
    }

    bool operator==(const Individual& ind2);
};

#endif