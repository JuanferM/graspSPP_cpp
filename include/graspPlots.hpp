#ifndef GRASPPLOTS_H
#define GRASPPLOTS_H

#include "librarySPP.hpp"

#include <cmath>
#include <matplot/matplot.h>

// Plot l'examen d'un run de GRASP
void plotRunGRASP(
        std::string instance,
        std::vector<int>& zInits,
        std::vector<int>& zAmels,
        std::vector<int>& zBests);

#endif /* end of include guard: GRASPPLOTS_H */