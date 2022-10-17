#include "heuristics.hpp"
#include "librarySPP.hpp"
#include <algorithm>

std::tuple<char*, int, char*> GreedyRandomized(
        int m,
        int n,
        const int* C,
        const char* A,
        const float* U,
        const float alpha) {
    bool valid, *candidate = new bool[n];
    int i(0), j(0), s(0), e(0);
    float limit(0.0f);
    std::set<int> RCL;
    char *x = new char[n], *column = new char[m];
    for(i = 0; i < n; i++) x[i] = 0, candidate[i] = true;
    for(j = 0; j < m; j++) column[j] = 0;

    size_t min_u, max_u, *u_order = argsort(n, U); // DON'T FORGET TO DELETE

    i = 0;
    while(s != m && i < n) {
        max_u = -1, min_u = -1;
        // Indices of max and min utilities
        for(j = 0; j < n && !candidate[j]; j++);
        max_u = u_order[j];
        for(j = n-1; j >= 0 && !candidate[j]; j--);
        min_u = u_order[j];
        limit = U[min_u] + alpha * (U[max_u] - U[min_u]);
        for(j = 0; j < n; j++)
            // If variable's index is candidate and the utility
            // greater than limit then we add the index in RCL
            if(candidate[j] && U[j] >= limit)
                RCL.insert(j);

        // Select an element e from RCL at random
        e = *select_randomly(RCL.begin(), RCL.end());
        for(j = 0, valid = true; j < m && valid; j++)
            valid = column[j] + A[INDEX(e, j)] <= 1;
        for(j = 0, s = 0; j < m && valid; s += column[j], j++)
            column[j] += A[INDEX(e, j)];
        x[e] = valid;
        i += 1; RCL = std::set<int>();
    }

    delete[] u_order; delete[] candidate;
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
        bool deep) {
    int iter(0);
    int zBest(-1);
    char *x(nullptr), *column(nullptr);

    for(iter = 0; iter < nbIter; iter++) {
        std::tie(x, zInits[iter], column) = GreedyRandomized(m, n, C, A, U, alpha);
        zAmels[iter] = zInits[iter];
        GreedyImprovement(m, n, C, A,
                x, &zAmels[iter], deep, column);

        zBest = std::max(zBest, zAmels[iter]);
        zBests[iter] = zBest;

        /* MOST IMPORTANT SECTION */
        if(x) delete[] x, x = nullptr;
        if(column) delete[] column, column = nullptr;
    }
}
