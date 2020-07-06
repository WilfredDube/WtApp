#include "BendSeqGen.h"

#include <iostream>
#include <cmath>
#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

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

Sequence generateBendingSeq(Fxt::Model& model, std::vector<int> initialSeq)
{   
    // current generation 
    int generation = 0, factorial {1};   
    std::vector<Sequence> population; 
    bool found = false;
    size_t POPULATION_SIZE, arrSize = initialSeq.size();

    factorial = computeFactorial(arrSize);

    POPULATION_SIZE = factorial;  

    if (initialSeq.size() == 1 || initialSeq.size() == 2)
    {
        return Sequence(initialSeq, model, initialSeq.size());
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
        auto newPerm = Sequence(genomes[i], model, initialSeq.size());
        population.push_back(newPerm); 
    }

    std::cout << "Num in pop = " << population.size() << std::endl;
  
    std::vector<Sequence> new_generation; 
    
    while (generation < POPULATION_SIZE)
    {   
        new_generation.push_back(population[generation]);

        // population = new_generation; 
        std::cout<< "Generation: " << generation << "\t"; 
        std::cout<< "String: " ;
        for(auto& c : new_generation[generation].chromosome)
        	std::cout<< c << " ";
        std::cout <<"\t"; 
        std::cout << "Fitness: "<< population[generation].fitness << "  "<< "Flips: "<< new_generation[generation].nFlips;
        std::cout << " Tools: " << population[generation].nTools;
        std::cout << " Rotations: " << population[generation].nRotations;
        std::cout << " Distance: " << population[generation].distance << "\n"; 
  
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
    std::cout<< "Distance : "<< new_generation[0].distance << "\n";
    std::cout << "======================================================================";
    std::cout << "==============================" << std::endl;
    std::cout << "======================================================================";
    std::cout << "==============================" << std::endl;

    int n = 0;   
    while (n < POPULATION_SIZE)
    {   
        // new_generation.push_back(population[n]);

        // population = new_generation; 
        std::cout<< "Generation: " << n << "\t"; 
        std::cout<< "String: " ;
        for(auto& c : new_generation[n].chromosome)
        	std::cout<< c << " ";
        std::cout <<"\t"; 
        std::cout << "Fitness: "<< new_generation[n].fitness << "  "<< "Flips: "<< new_generation[n].nFlips;
        std::cout << " Tools: " << new_generation[n].nTools;
        std::cout << " Rotations: " << new_generation[n].nRotations;
        std::cout << " Distance: " << new_generation[n].distance << "\n"; 
  
        ++n; 
     }

    return new_generation[0];
} 