#include "heuristics.hpp"

#define USE_GLPK        false
#define VERBOSE_GLPK    false

#define ALPHA           0.7
#define NUM_ITER        100
#define DEEPSEARCH      true

int main() {
    std::string pwd(std::filesystem::current_path());
    std::string path(pwd + "/instances/");
    std::cout.precision(6);

    #if !USE_GLPK
        float *U(nullptr), *times(nullptr);
        char *A(nullptr);
        int m(-1), n(-1), *C(nullptr);
        int *zInits(nullptr), *zAmels(nullptr), *zBests(nullptr);
    #endif

    m_print(std::cout, _CLRd, "Etudiants : MERCIER et PICHON\n", _CLR);
    std::set<std::string> fnames = getfname(path);
    for(auto instance : fnames) {
        #if USE_GLPK
            modelSPP(instance, path, &tt, VERBOSE_GLPK);
        #else
            // Load one numerical instance
            std::tie(m, n, C, A, U) = loadSPP(path + instance);
            m_print(std::cout, _CLB, "\n\nInstance : ", instance, "\n\n", _CLR);
            std::tie(zInits, zAmels, zBests, times) = GRASP(m, n, C, A, U, ALPHA, NUM_ITER, DEEPSEARCH);

            // Plots


            /* MOST IMPORTANT SECTIONS */
            freeSPP(C, A, U);
            if(zInits) delete[] zInits, zInits = nullptr;
            if(zAmels) delete[] zAmels, zAmels = nullptr;
            if(zBests) delete[] zBests, zBests = nullptr;
            if(times) delete[] times, times = nullptr;
        #endif
    }

    glp_free_env();
    return 0;
}
