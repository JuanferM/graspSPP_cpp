#ifndef HEURISTICS_H
#define HEURISTICS_H

#include "movements.hpp"

#include <string>
#include <cstring>

// Greedy construction of a feasible solution
std::tuple<char*, int, char*> GreedyRandomized(
        int m,
        int n,
        const int* C,
        const char* A,
        const float* U,
        float alpha);

// Greedy improvement of a feasible solution through (deep) local search
void GreedyImprovement(
        int m,
        int n,
        const int* C,
        const char* A,
        char* x,
        int z,
        bool deep = true,
        char* column = nullptr);

// GRASP for the Set Packing Problem
// returns the scores of greedy randomized and
// greedy improvement for each iteration.
// Also returns the elapsed time of each iteration.
std::tuple<int*, int*, int*, float*> GRASP(
        int m,
        int n,
        const int* C,
        const char* A,
        const float* U,
        float alpha = 0.7,
        int nbIter = 100,
        bool deep = true);

#endif /* end of include guard: HEURISTICS_H */
