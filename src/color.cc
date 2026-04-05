#include "color.h"

void write_color(std::ostream& out, const color& pixel_color) {
    double r = pixel_color[0];
    double g = pixel_color[1];
    double b = pixel_color[2];

    // translate the [0,1] component values to the byte range [0,255]
    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    // write out the pixel color components
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

