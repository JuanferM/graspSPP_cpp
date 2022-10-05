#include "heuristics.hpp"

std::tuple<char*, int, char*> GreedyRandomized(
        std::ostream* io[],
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

std::tuple<char*, int> GreedyImprovement(
        std::ostream* io[],
        int m,
        int n,
        const int* C,
        const char* A,
        const char* x,
        int zInit,
        bool deep,
        char* column) {
    int i(2), t(1), nz(zInit);
    char *nx = new char[n];
    char names[3][4] = { "0-1", "1-1", "2-1" };
    bool printed[3] = { false },
        (*f[3])(int, int, const int*, const char*, char*, int*, bool, char*) = {
            zero_oneExchange,
            one_oneExchange,
            two_oneExchange
        };

    // nx is a deep copy of x at first. With each k-p exchange we search
    // the neighborhood of nx. If a neighbor with a better value for z is
    // found then we replace nx by that neighbor and we keep applying the
    // same k-p exchange on nx. If the k-p exchange fail to find a neighbor that
    // improves z then we select another k-p exchange and we repeat the process.
    memcpy(nx, x, sizeof(char) * n);
    while(i >= 0) {
        t = f[i](m, n, C, A, nx, &nz, deep, column);
        if(!printed[i])
            m_print(*io[0], "\n > ", names[i], " : "), printed[i] = true;
        if(t) m_print(*io[0], "x"); else i--;
    }

    return std::make_tuple(nx, nz);
}
