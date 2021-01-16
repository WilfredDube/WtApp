#include "../include/BendSequenceGenerator.h"
#include "../include/GA.h"

#include "../../libfxtract/include/Computation.h"

#include <algorithm>
#include <iostream>
#include <chrono>
#include <random>

BendSequenceGenerator::BendSequenceGenerator(std::vector<int> chromosome, SheetMetalFeaturePtr& model) 
{
    sequenceImpl_ = std::make_shared<BendSequenceGenerator::Sequence>(chromosome);
    sequenceImpl_->fitness = sequenceImpl_->cal_fitness(model);  
};

double BendSequenceGenerator::Sequence::cal_fitness(SheetMetalFeaturePtr& sheetMetalFeature)
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
    }

    std::cout << "Num in pop = " << population.size() << std::endl;
  
    std::vector<Sequence> new_generation; 
    
    while (generation < POPULATION_SIZE)
    {   
        new_generation.push_back(population[generation]);
        ++generation; 
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

    // int n = 0;   
    // while (n < new_generation.size())
    // {   
    //     std::cout<< "Generation: " << n << "\t"; 
    //     std::cout<< "String: " ;
    //     for(auto& c : new_generation[n].chromosome)
    //     	std::cout<< c << " ";
    //     std::cout <<"\t"; 
    //     std::cout << "Fitness: "<< new_generation[n].fitness << "  "<< "Flips: "<< new_generation[n].nFlips;
    //     std::cout << " Tools: " << new_generation[n].nTools;
    //     std::cout << " Rotations: " << new_generation[n].nRotations;
    //     std::cout << " Distance: " << new_generation[n].distance << "\n"; 
  
    //     ++n; 
    //  }
}

bool BendSequenceGenerator::Sequence::operator<(const BendSequenceGenerator::Sequence& individualSequence)
{
    return (fitness < individualSequence.fitness);
}




// BendSequenceGenerator::Sequence BendSequenceGenerator::Sequence::mate(BendSequenceGenerator::Sequence parentSequence)
// {
//     // chromosome for offspring 
//     std::vector<int> child_chromosome = parentSequence.chromosome;

//     unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

//     std::shuffle (child_chromosome.begin(), child_chromosome.end(), std::default_random_engine(seed));

//     // create new Individual(offspring) using generated chromosome for offspring 
//     return Sequence(child_chromosome); 
// }

// bool BendSequenceGenerator::Sequence::operator<(const BendSequenceGenerator::Sequence& individualSequence)
// {
//     return (fitness < individualSequence.fitness);
// }

// bool BendSequenceGenerator::Sequence::operator==(const BendSequenceGenerator::Sequence& individualSequence)
// {
//     return std::equal(cbegin(chromosome), cend(chromosome), cbegin(individualSequence.chromosome));
// }
