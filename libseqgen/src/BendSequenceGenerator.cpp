#include "../include/BendSequenceGenerator.h"
#include "../include/GA.h"

#include "../../libfxtract/include/Computation.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>

BendSequenceGenerator::BendSequenceGenerator(std::vector<int> chromosome, SheetMetalPtr& model) 
: sheetMetalFeature { model}
{
    sequenceImpl_ = std::make_shared<BendSequenceGenerator::Sequence>(chromosome);
};

double BendSequenceGenerator::Sequence::cal_fitness(SheetMetalPtr& sheetMetalFeature)
{
    int len = targetSize; 
    double parallel = 1, equality = 1, direction = 1, fitness = 0, seqDistance = 0.0;

    for(int i = 0; i < len; i++) 
    { 
        if ((i + 1) < len)
        {
            // Add 15 if the current and next bend have the same bend angle
            if(sheetMetalFeature->isSameAngle(chromosome[i], chromosome[i + 1])) {
                equality = 10;
            } else {
                ++nTools;
            }
        
            // Compute the distance between the current and next bend
            double dist = sheetMetalFeature->distance(chromosome[i], chromosome[i + 1]);

            if(dist != 0){
                seqDistance = sheetMetalFeature->distance(chromosome[i], chromosome[i + 1]);
            }             
        
            if(sheetMetalFeature->isSameDirection(chromosome[i], chromosome[i + 1])) {
                direction = 30;
            } else {
                ++nFlips;
            }            
       
            // Add 20 to the fitness if the current and next bend are parallel
            if(sheetMetalFeature->isParallel(chromosome[i], chromosome[i + 1])){
                parallel = 20;
            } else {
                ++nRotations;
            }
        } 

        fitness += parallel * equality * direction / (seqDistance * nFlips * nRotations + 1);
  
        distance += seqDistance;

        parallel = 1; equality = 1; direction = 1; seqDistance = 0.0;
    }

    return fitness;
}

void BendSequenceGenerator::generateBendingSequence()
{   
    std::vector<int> initialSequence = sequenceImpl_->chromosome;

    if (initialSequence.size() == 1 || initialSequence.size() == 2)
    {
        return ;
    }

    // current generation 
    int generation = 0;   
    std::vector<Sequence> population; 

    size_t POPULATION_SIZE = Fxt::Computation::computeFactorial(initialSequence.size());

    // create initial population 
    int arr[initialSequence.size()];

    for (size_t i = 0; i < initialSequence.size(); i++)
    {
        arr[i] = initialSequence[i];
    }
    
    std::vector<std::vector<int>> genomes;
    findPermutations(arr, initialSequence.size(), genomes);

    for(int i = 0; i < genomes.size() ; i++) 
    { 
        population.push_back({ Sequence(genomes[i]) });
        population[i].fitness = population[i].cal_fitness(sheetMetalFeature);
    }    

    // // sort the population in increasing order of fitness score 
    std::sort(population.begin(), population.end());

    sequenceImpl_->chromosome = population[0].chromosome;
    sequenceImpl_->fitness = population[0].fitness;
}

size_t BendSequenceGenerator::getSequenceSize() { return sequenceImpl_->chromosome.size(); }
    
void BendSequenceGenerator::print()
{
    std::cout << "======================================================================";
    std::cout << "==============================" << std::endl;
    std::cout << "======================================================================";
    std::cout << "==============================" << std::endl;
    std::cout<< "Best Sequence: " << "\t"; 
    for(auto& c : sequenceImpl_->chromosome) {
        std::cout<< c << " ";
        std::cout <<"\t"; 
    }
    std::cout<< "Fitness : "<< sequenceImpl_->fitness << "\n";
    std::cout<< "No of tools : "<< sequenceImpl_->nTools << "\n";
    std::cout<< "No of rotations : "<< sequenceImpl_->nRotations << "\n";
    std::cout<< "No of flips : "<< sequenceImpl_->nFlips << "\n";
    std::cout<< "Distance : "<< sequenceImpl_->distance << "\n";
    std::cout<< "Target size : "<< sequenceImpl_->targetSize << "\n";
    std::cout << "======================================================================";
    std::cout << "==============================" << std::endl;
    std::cout << "======================================================================";
    std::cout << "==============================" << std::endl;
}

bool BendSequenceGenerator::Sequence::operator<(const BendSequenceGenerator::Sequence& individualSequence)
{
    return (fitness > individualSequence.fitness);
}