#include "plots.hpp"
#include "heuristics.hpp"

#define USE_GLPK        false
#define VERBOSE_GLPK    false

#define ALPHA           0.5
#define NUM_ITER        100
#define DEEPSEARCH      true

int main() {
    std::string pwd(std::filesystem::current_path());
    std::string path(pwd + "/instances/");
    std::cout.precision(6);

    m_print(std::cout, _CLRd, "Etudiants : MERCIER et PICHON\n", _CLR);
    #if !USE_GLPK
        INIT_TIMER();
        float *U(nullptr), *times(nullptr);
        char *A(nullptr);
        int i(0), m(-1), n(-1), *C(nullptr);
        std::vector<int> zInits(NUM_ITER, 0),
                         zAmels(NUM_ITER, 0),
                         zBests(NUM_ITER, 0);

        m_print(std::cout, _CLP, "\nalpha\t\t\t= ", ALPHA);
        m_print(std::cout, "\nnombre d'itérations\t= ", NUM_ITER);
        m_print(std::cout, "\ndescente profonde\t= ", (DEEPSEARCH ? "oui" : "non"), _CLR, "\n\n");
    #endif

    std::set<std::string> fnames = getfname(path);
    for(auto instance : fnames) {
        #if USE_GLPK
            modelSPP(instance, path, &tt, VERBOSE_GLPK);
        #else
            if(times == nullptr) times = new float[fnames.size()];
            // Load one numerical instance
            std::tie(m, n, C, A, U) = loadSPP(path + instance);
            m_print(std::cout, _CLB, "\nInstance : ", instance, "\n", _CLR);
            TIMED(times[i],
                GRASP(m, n, C, A, U, zInits, zAmels, zBests, ALPHA, NUM_ITER, DEEPSEARCH);
            ); i++;

            // Plots
            m_print(std::cout, "Plot du run...\n");
            plotRunGRASP(instance, zInits, zAmels, zBests);

            /* MOST IMPORTANT SECTIONS */
            freeSPP(C, A, U);
        #endif
    }

    glp_free_env();
    if(times) delete[] times, times = nullptr;
    return 0;
}
