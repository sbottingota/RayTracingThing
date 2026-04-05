#include <iostream>

#include "color.h"

#define WIDTH 256
#define HEIGHT 256

int main() {
    std::cout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

    for (int r = 0; r < HEIGHT; ++r) {
        std::clog << "\rScanlines: " << r << '/' << WIDTH - 1 << std::flush;

        for (int g = 0; g < WIDTH; g++) {
            color pixel_color = color(double(r) / WIDTH, double(g) / HEIGHT, 0);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rRendering complete.                 \n";
}

