#include "GA.h"

#include <algorithm>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock  

std::vector<int> bends;

// Function to generate random numbers in given range  
int random_num(int start, int end) 
{ 
   // int range = (end-start)+1; 
   // int random_int = start+(rand()%range);
   // return random_int; 
   // std::default_random_engine generator;
   // std::uniform_int_distribution<int> distribution(start, end);
   // int random_int = distribution(generator);  

   std::mt19937 prng(std::random_device{}());
	std::uniform_int_distribution<int> idist(start, end);
   return idist(prng); 
} 
  
// Create random genes for mutation 
char mutated_genes() 
{  
   int len = bends.size();
   int r = random_num(0, len-1);
   return bends[r];
} 
  
// create chromosome or string of genes 
// std::vector<int> create_gnome(const std::vector<int>& foo) 
// { 
//    int len = foo.size();
//    std::vector<int> fooSet;
   
//    bends = foo;
//    // srand((unsigned)(time(0)));

//    for(int i = 0; i < len; i++) {
//       int x = mutated_genes();
      
//       auto endIt = cend(fooSet);
//       auto it = find(cbegin(fooSet), endIt, x);
   
//       if(it != endIt) {
//    	    --i;
//       } else
//        	fooSet.push_back(x);
//      }
     
//      return fooSet;
// }

//function to swap the variables
// void swap(int *a, int *b)
// {
//     int temp;
//     temp = *a;
//     *a = *b;
//     *b = temp;
// }

//permutation function
void permutation(std::vector<int>& arr, int start, int end, std::vector<std::vector<int>>& perms)
{
   // static size_t nPerms = 0;
   bends = arr;

   // if (nPerms == 1000)
   // {
   //    return;
   // }

   if(start==end)
   {
      perms.push_back(arr);
      return;
   }

   int i;
   for(i = start; i <= end; i++)
   {
   //swapping numbers
   //   swap((arr+i), (arr+start));
   std::iter_swap(arr.begin() + i, arr.begin() + start);

   //fixing one first digit
   //and calling permutation on
   //the rest of the digits
   // ++nPerms;
   permutation(arr, start + 1, end, perms);

   // swap((arr+i), (arr+start));
   std::iter_swap(arr.begin() + i, arr.begin() + start);   
   }
}

// Function to find the permutations 
void findPermutations(int a[], int n, std::vector<std::vector<int>>& perms) 
{ 
   // static size_t nPerms = 0;
    // Sort the given array 
   //  std::sort(a, a + n); 
  
    // Find all possible permutations 
   //  std::cout << "Possible permutations are:\n"; 
    do { 
      std::vector<int> res;

      for (size_t i = 0; i < n; i++)
      {
         res.push_back(a[i]);
      }

      perms.push_back(res);
      // ++nPerms;
       
    } while (std::next_permutation(a, a + n));// && nPerms < 10000); 
} 

