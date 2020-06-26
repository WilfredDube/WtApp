#include "GaIndividual.h"

#include "GA.h"
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

double Individual::min_dist = 0.0;

// Overloading < operator 
bool operator<(const Individual &ind1, const Individual &ind2) 
{ 
    return ind1.fitness > ind2.fitness; 
} 

bool Individual::operator==(const Individual& ind2) 
{ 
    return std::equal(cbegin(this->chromosome), cend(this->chromosome), cbegin(ind2.chromosome)); 
}

Individual::Individual(std::vector<int> chromosome, Fxt::Model& model, size_t target_size) 
: chromosome (chromosome), targetSize(target_size), model(model), nTools(1), nRotations(0), nFlips(0)
{ 
    fitness = cal_fitness();  
}; 

// Perform mating and produce new offspring  
Individual Individual::mate(Individual par2) 
{ 
    // chromosome for offspring 
    std::vector<int> child_chromosome = par2.chromosome;

    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

    std::shuffle (child_chromosome.begin(), child_chromosome.end(), std::default_random_engine(seed));

    // create new Individual(offspring) using generated chromosome for offspring 
    return Individual(child_chromosome, model, targetSize); 
};

// Calculate fittness score
double Individual::cal_fitness() 
{ 
    int len = targetSize; 
    double parallel = 1, equality = 1, direction = 1, fitness = 0, seqDistance = 0.0;

    for(int i = 0; i < len; i++) 
    { 
        if ((i + 1) < len){
            // Add 15 if the current and next bend have the same bend angle
            if(model.isSameAngle(chromosome[i], chromosome[i + 1])) {
                equality = 10;
            } else
            {
                ++nTools;
            }
        }

        if ((i + 1) < len){
            // Compute the distance between the current and next bend
            double dist = model.distance(chromosome[i], chromosome[i + 1]);

            if(dist != 0){
                seqDistance = model.distance(chromosome[i], chromosome[i + 1]);
            }             
            std::cout << chromosome[i] << " - " << chromosome[i + 1] << " " << "Distance : " << seqDistance << std::endl;
        }

        if((i + 1) < len){
            if(model.isSameDirection(chromosome[i], chromosome[i + 1])) {
                direction = 30;
            } else
            {
                ++nFlips;
            }            
        }

        if ((i + 1) < len){
            // Add 8 to the fitness if the current and next bend are parallel
            // if(model.isParallel(chromosome[i], chromosome[i + 1])){
            //     parallel = 20;
            // } else
            // {
            //     ++nRotations;
            // }

            if (seqDistance != 0)
            {
                parallel = 20;
            } else
            {
                ++nRotations;
            }  
        } 

        if (seqDistance == 0.0)
        {
            fitness += parallel * equality * direction * seqDistance;
        } else {
            fitness += parallel * equality * direction / seqDistance;
        }        
        
        parallel = 1; equality = 1; direction = 1; seqDistance = 0.0;
    }

    std::cout << "===============================================" << seqDistance << std::endl;

    return fitness;
}; 