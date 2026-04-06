#include "color.h"

#include "util.h"

#include <cmath>

double linear_to_gamma(double linear_component) {
    if (linear_component > 0) {
        return std::sqrt(linear_component);
    } else {
        return 0;
    }
}

void write_color(std::ostream& out, const color& pixel_color) {
    double r = linear_to_gamma(pixel_color[0]);
    double g = linear_to_gamma(pixel_color[1]);
    double b = linear_to_gamma(pixel_color[2]);

    // translate the [0,1] component values to the byte range [0,255]
    static const interval intensity(0.0, 0.999);
    int rbyte = int(256 * intensity.clamp(r));
    int gbyte = int(256 * intensity.clamp(g));
    int bbyte = int(256 * intensity.clamp(b));

    // write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

