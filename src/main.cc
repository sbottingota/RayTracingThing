#include <iostream>
#include <memory>

#include "renderer.h"

#define WIDTH 1280
#define HEIGHT 720

#define FOCAL_LENGTH 1.0

int main() {
    auto objects = std::make_shared<object_group>();
    objects->add(std::make_shared<sphere>(point3(0, 0, -1), 0.3));
    objects->add(std::make_shared<sphere>(point3(-20, 0, -20), 10));

    rendering r(WIDTH, HEIGHT, FOCAL_LENGTH, objects);

    std::cout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

    for (int j = 0; j < HEIGHT; ++j) {
        std::clog << "\rScanlines: " << j << '/' << WIDTH - 1 << std::flush;

        for (int i = 0; i < WIDTH; i++) {
            write_color(std::cout, r.pixel_at(i, j));
        }
    }

    std::clog << "\rRendering complete.                 \n";
}

