#include "plots.hpp"
#include "matplot/util/common.h"

void plotRunGRASP(
        const std::string instance,
        const std::vector<int>& zInits,
        const std::vector<int>& zAmels,
        const std::vector<int>& zBests) {
    int i(0), n = zInits.size();
    auto X = matplot::linspace(1, n, n);
    std::string ins(instance);
    for(i = 0; i < (int)ins.size(); i++)
        if(ins[i] == '_')
            ins.replace(i, 1, "＿");
    std::string tit("GRASP-SPP | z_{Init} z_{LS} z_{Best} | " + ins);

    double lb = *std::min_element(std::begin(zInits), std::end(zInits)),
           ub = *std::max_element(std::begin(zBests), std::end(zBests));

    if(lb == ub) {
        lb -= 5;
        ub += 5;
    }

    auto fig = matplot::figure(true);
    fig->name("Examen d'un run");
    fig->size(576, 576);
    fig->title(tit);
    matplot::xlabel("Itérations");
    matplot::ylabel("valeurs de z(x)");
    matplot::xticks({1.0, ceil(n/4.0), ceil(n/2.0), ceil((3*n)/4.0), (double)n});
    matplot::axis({0, n+1.0, lb-(int(lb/100)+1)*2, ub+(int(ub/100)+1)*2});
    matplot::plot(X, zBests)
        ->line_width(2)
        .line_width(2)
        .color("green")
        .display_name("meilleures solutions");
    matplot::hold(true); // Allow multiple plot() calls
    for(i = 1; i <= n; i++) {
        matplot::line(i, zInits[i-1], i, zAmels[i-1])
            ->line_width(0.5)
            .color("blue")
            .display_name(""); // Don't show in legend
    }
    matplot::plot(X, zAmels, "o")
        ->marker_size(4)
        .color("green")
        .marker("^")
        .marker_face(true) // Filled
        .display_name("toutes solutions améliorées");
    matplot::plot(X, zInits, "o")
        ->marker_size(2)
        .color("red")
        .marker(".")
        .display_name("toutes solutions construites");
    matplot::legend()
        ->location(matplot::legend::general_alignment::bottomright);
    fig->draw();
}

void plotAnalyseGRASP(
        const std::string instance,
        const std::vector<double>& divs,
        const std::vector<int> zMin,
        const std::vector<int> zMoy,
        const std::vector<int> zMax) {
    int n = divs.size();
    std::string ins(instance);
    for(int i = 0; i < (int)ins.size(); i++)
        if(ins[i] == '_')
            ins.replace(i, 1, "＿");
    std::string tit("GRASP-SPP | z_{min} z_{moy} z_{max} | " + ins);
    auto err = matplot::transform(matplot::linspace(0, n-1),
            [zMin, zMax](double x) {
                return std::abs(zMin[(int)x]-zMax[(int)x]);
            });

    auto fig = matplot::figure(true);
    fig->name("Bilan tous runs");
    fig->size(576, 576);
    fig->title(tit);
    matplot::xlabel("Itérations");
    matplot::ylabel("valeurs de z(x)");
    if(*std::max_element(err.begin(), err.end()) == 0)
        matplot::axis({divs[0]-1, divs[n-1]+1,
                0, *std::max_element(zMax.begin(), zMax.end())+5.0});
    else
        matplot::axis({divs[0]-1, divs[n-1]+1, matplot::inf, matplot::inf});
    matplot::xticks(divs);
    matplot::errorbar(divs, zMoy, err)
        ->line_width(1)
        .color("black")
        .marker("+")
        .display_name("zMin zMax");
    matplot::hold(true); // Allow multiple plot() calls
    matplot::plot(divs, zMoy, "-")
        ->marker_size(4)
        .color("green")
        .marker("o")
        .marker_face(true)
        .display_name("zMoy");
    matplot::legend()
        ->location(matplot::legend::general_alignment::bottomright);
    fig->draw();
}

void plotCPUt(
        std::vector<std::string> fnames,
        std::vector<float> tMoy) {
    int n;
    for(n = 0; n < (int)fnames.size(); n++)
        for(int i = 0; i < (int)fnames[n].size(); i++)
            if(fnames[n][i] == '_')
                fnames[n].replace(i, 1, "＿");

    auto x = matplot::linspace(1, n, n);

    auto fig = matplot::figure(true);
    fig->name("Bilan CPUt tous runs");
    fig->size(576, 676);
    fig->title("GRASP-SPP | tMoy");
    matplot::ylabel("CPUt moyen (en s)");
    matplot::xticks(x);
    matplot::xticklabels(fnames);
    matplot::xtickangle(60);
    matplot::plot(x, tMoy, "--")
        ->line_width(0.5)
        .marker_size(4)
        .color("blue")
        .marker("o")
        .marker_face(true)
        .display_name("tMoy");
    matplot::legend()
        ->location(matplot::legend::general_alignment::bottomright);
    fig->draw();
}
