#include "plots.hpp"

void plotRunGRASP(
        std::string instance,
        std::vector<int>& zInits,
        std::vector<int>& zAmels,
        std::vector<int>& zBests) {
    int i(0), n = zInits.size();
    std::vector<int> X(n);
    std::iota(std::begin(X), std::end(X), 0);
    std::string tit("GRASP-SPP | z_{Init} z_{LS} z_{Best} | " + instance);

    auto fig = matplot::figure(true);
    fig->name("Examen d'un run");
    fig->size(576, 576);
    fig->title(tit.c_str());
    matplot::xlabel("Itérations");
    matplot::ylabel("valeurs de z(x)");
    matplot::ylim({
            *std::min_element(std::begin(zInits), std::end(zInits))-10.0,
            *std::max_element(std::begin(zBests), std::end(zBests))+5.0
            });
    matplot::xticks({1.0, ceil(n/4.0), ceil(n/2.0), ceil(n/4.0*3), (double)n});
    matplot::plot(X, zBests)
        ->line_width(2)
        .color("green")
        .display_name("meilleures solutions");
    matplot::plot(X, zAmels)
        ->line_style("")
        .marker_size(2)
        .color("green")
        .marker("^")
        .display_name("toutes solutions améliorées");
    matplot::plot(X, zInits)
        ->line_style("")
        .marker_size(2)
        .color("red")
        .marker(".")
        .display_name("toutes solutions construites");
    for(i = 0; i < n; i++)
        matplot::line(i, zInits[i], i, zAmels[i])
            ->line_width(0.5);
    // matplot::legend()
    //     ->location(matplot::legend::general_alignment::bottomright);
    matplot::show(fig);
}
