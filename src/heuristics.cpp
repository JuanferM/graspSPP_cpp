#include "heuristics.hpp"
#include "librarySPP.hpp"

#include <omp.h>

std::tuple<char*, int, char*> GreedyRandomized(
        int m,
        int n,
        const int* C,
        const char* A,
        const float* U,
        const float alpha) {
    bool valid;
    int i(0), j(0), k(0), s(0), e(0), min_u, max_u;
    float limit(0.0f);
    std::vector<int> RCL;
    char *x = new char[n], *column = new char[m];
    for(i = 0; i < n; i++) x[i] = 0;
    for(j = 0; j < m; j++) column[j] = 0;

    int* u_order = argsort(n, U); // DON'T FORGET TO DELETE

    k = 0;
    while(s != m && k < n) {
        // Indices of max and min utilities
        for(j = 0, max_u = 0; j < n && u_order[max_u] == -1; j++)
            max_u = j;
        for(j = n-1, min_u = 0; j >= 0 && u_order[min_u] == -1; j--)
            min_u = j;
        limit = U[u_order[min_u]] + alpha * (U[u_order[max_u]] - U[u_order[min_u]]);
        for(j = 0; j < n; j++)
            // If variable's index is candidate and the utility
            // greater than limit then we add the index in RCL
            if(u_order[j] != -1 && U[u_order[j]] >= limit)
                RCL.push_back(j);

        // Select an element e from RCL at random
        e = (RCL.size()) ? *select_randomly(RCL.begin(), RCL.end()) : max_u;
        for(j = 0, valid = true; j < m && valid; j++)
            valid = column[j] + A[INDEX(u_order[e], j)] <= 1;
        for(j = 0, s = 0; valid && j < m; s += column[j], j++)
            column[j] += A[INDEX(u_order[e], j)];
        x[u_order[e]] = valid, u_order[e] = -1;
        k += 1; RCL.clear();
    }

    delete[] u_order;
    return std::make_tuple(x, dot(n, x, C), column);
}

void GreedyImprovement(
        int m,
        int n,
        const int* C,
        const char* A,
        char* x,
        int* z,
        bool deep,
        char* column) {
    int i(2);
    bool (*f[3])(int, int, const int*, const char*, char*, int*, bool, char*) = {
            zero_oneExchange,
            one_oneExchange,
            two_oneExchange
        };

    // We modify x and z directly (no copy)
    while(i >= 0){
        if(!f[i](m, n, C, A, x, z, deep, column)) i--;
    }
}

void GRASP(
        int m,
        int n,
        const int* C,
        const char* A,
        const float* U,
        std::vector<int>& zInits,
        std::vector<int>& zAmels,
        std::vector<int>& zBests,
        const float alpha,
        int nbIter,
        bool deep,
        bool parallel) {
    int iter(0), zBest(-1);

    #pragma omp parallel for if(parallel)
    for(iter = 0; iter < nbIter; iter++) {
        char *x(nullptr), *column(nullptr);
        std::tie(x, zInits[iter], column) = GreedyRandomized(m, n, C, A, U, alpha);
        zAmels[iter] = zInits[iter];
        GreedyImprovement(m, n, C, A,
                x, &zAmels[iter], deep, column);

        /* MOST IMPORTANT SECTION */
        if(x) delete[] x, x = nullptr;
        if(column) delete[] column, column = nullptr;
    }

    // Compute zBests using zAmels
    for(iter = 0; iter < nbIter; iter++) {
        zBest = std::max(zBest, zAmels[iter]);
        zBests[iter] = zBest;
    }
}
