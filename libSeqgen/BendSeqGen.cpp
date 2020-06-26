#include "BendSeqGen.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

#include "GaIndividual.h"
#include "GA.h"
 
 // run ./hello --approot ../approot --docroot ../docroot --http-listen localhost:8090

 size_t computeFactorial(size_t nBends)
 {
    int factorial {1};
    for (unsigned long long n {1}, sum {}; n <= nBends; ++n)
    {
        factorial *= n;                    // Calculate n! for current n
    }

    return factorial;
 }

std::vector<int> generateBendingSeq(Fxt::Model& model, std::vector<int> initialSeq)
{   
    // current generation 
    int generation = 0, factorial {1};   
    std::vector<Individual> population; 
    bool found = false;
    size_t POPULATION_SIZE, arrSize = initialSeq.size();

    factorial = computeFactorial(arrSize);

    POPULATION_SIZE = factorial;  

    if (initialSeq.size() == 1 || initialSeq.size() == 2)
    {
        return initialSeq;
    }
      
    // create initial population 
    int arr[arrSize];

    for (size_t i = 0; i < arrSize; i++)
    {
        arr[i] = initialSeq[i];
    }
    
    std::vector<std::vector<int>> genomes;
    findPermutations(arr, arrSize, genomes);

    for(int i = 0; i < genomes.size() ; i++) 
    { 
        auto newPerm = Individual(genomes[i], model, initialSeq.size());
        population.push_back(newPerm); 
    }

    std::cout << "Num in pop = " << population.size() << std::endl;
  
    std::vector<Individual> new_generation; 
    
    while (generation < POPULATION_SIZE)
    {   
        new_generation.push_back(population[generation]);

        // population = new_generation; 
        std::cout<< "Generation: " << generation << "\t"; 
        std::cout<< "String: " ;
        for(auto& c : new_generation[generation].chromosome)
        	std::cout<< c << " ";
        std::cout <<"\t"; 
        std::cout<< "Fitness: "<< population[generation].fitness << "  "<< "No of flips : "<< new_generation[generation].nFlips << "\n"; 
  
        ++generation; 
     }
     

    // sort the population in increasing order of fitness score 
    std::sort(new_generation.begin(), new_generation.end()); 

    std::cout << "======================================================================";
    std::cout << "==============================" << std::endl;
    std::cout << "======================================================================";
    std::cout << "==============================" << std::endl;
    std::cout<< "Best Sequence: " << "\t"; 
    for(auto& c : new_generation[0].chromosome) {
        std::cout<< c << " ";
        std::cout <<"\t"; 
    }
    std::cout<< "Fitness : "<< new_generation[0].fitness << "\n";
    std::cout<< "No of tools : "<< new_generation[0].nTools << "\n";
    std::cout<< "No of rotations : "<< new_generation[0].nRotations << "\n";
    std::cout<< "No of flips : "<< new_generation[0].nFlips << "\n";
    std::cout << "======================================================================";
    std::cout << "==============================" << std::endl;
    std::cout << "======================================================================";
    std::cout << "==============================" << std::endl;

    return new_generation[0].chromosome;
} 