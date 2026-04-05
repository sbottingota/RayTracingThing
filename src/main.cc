#include <iostream>

#include "renderer.h"

#define WIDTH 1280
#define HEIGHT 720

#define FOCAL_LENGTH 1.0

int main() {
    rendering r(WIDTH, HEIGHT, FOCAL_LENGTH);

    std::cout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

    for (int j = 0; j < HEIGHT; ++j) {
        std::clog << "\rScanlines: " << j << '/' << WIDTH - 1 << std::flush;

        for (int i = 0; i < WIDTH; i++) {
            write_color(std::cout, r.pixel_at(i, j));
        }
    }

    std::clog << "\rRendering complete.                 \n";
}

