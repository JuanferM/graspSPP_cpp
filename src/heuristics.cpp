#include "heuristics.hpp"
#include <algorithm>

std::tuple<char*, int, char*> GreedyRandomized(
        int m,
        int n,
        const int* C,
        const char* A,
        const float* U,
        float alpha) {
    bool valid;
    int i(0), j(0), e(0), min_u, max_u;
    float limit(0.0f);
    std::unordered_set<int> RCL;
    std::unordered_map<int, char> prohib;
    char *x = new char[n], *column = new char[m];
    for(i = 0; i < n; i++) x[i] = 0;
    for(j = 0; j < m; j++) column[j] = 0;

    while((int)prohib.size() < n) {
        min_u = INT_MAX, max_u = INT_MIN;
        // Indices of utilities (get min and max)
        for(i = 0; i < n; i++) {
            if(!(*(&prohib[i])) && U[i] > max_u)
                max_u = U[i];
            if(!(*(&prohib[i])) && U[i] < min_u)
                min_u = U[i];
        }
        // Compute the limit and get RCL
        RCL.clear(); // Empty RCL
        limit = min_u + alpha * (max_u - min_u);
        for(i = 0; i < n; i++)
            // If variable's index not in prohib then we
            // add the index in the RCL
            if(!(*(&prohib[i])) && U[i] >= limit)
                RCL.insert(i);

        // Select an element e from the RCL at random
        e = *select_randomly(RCL.begin(), RCL.end());
        // Incorporate variable e into x
        x[e] = 1;

        // Update the prohibited candidate list
        prohib[e] = 1;
        // Selecting variable e means that we must select the
        // corresponding column in the matrix A
        for(j = 0; j < m; j++) column[j] += A[INDEX(e, j)];

        // Check which candidates have a conflict with e and add
        // them to the prohibited candidate list
        for(i = 0; i < n; i++) {
            if(!(*(&prohib[i]))) {
                for(j = 0, valid = true; j < m && valid; j++)
                    valid = column[j] + A[INDEX(i, j)] <= 1;
                if(!valid) prohib[i] = 1;
            }
        }
    }

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
        float alpha,
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
