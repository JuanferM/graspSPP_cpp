#include "plots.hpp"

void plotRunGRASP(
        std::string instance,
        std::vector<int>& zInits,
        std::vector<int>& zAmels,
        std::vector<int>& zBests) {
    int i(0), n = zInits.size();
    auto X = matplot::linspace(1, n, n);
    for(i = 0; i < (int)instance.size(); i++)
        if(instance[i] == '_')
            instance.replace(i, 1, "＿");
    std::string tit("GRASP-SPP | z_{Init} z_{LS} z_{Best} | " + instance);

    auto fig = matplot::figure(true);
    fig->name("Examen d'un run");
    fig->size(576, 576);
    fig->title(tit);
    matplot::xlabel("Itérations");
    matplot::ylabel("valeurs de z(x)");
    matplot::ylim({
            *std::min_element(std::begin(zInits), std::end(zInits))-10.0,
            *std::max_element(std::begin(zBests), std::end(zBests))+5.0
            });
    matplot::xticks({1.0, ceil(n/4.0), ceil(n/2.0), ceil((3*n)/4.0), (double)n});
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
    matplot::show(fig);
}
