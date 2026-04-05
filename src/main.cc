#include <iostream>
#include <memory>

#include "camera.h"

#define WIDTH 640
#define HEIGHT 360

#define FOCAL_LENGTH 1.0

int main() {
    auto objects = std::make_shared<object_group>();
    objects->add(std::make_shared<sphere>(point3(0, 0, -1), 0.3));
    objects->add(std::make_shared<sphere>(point3(-20, 0, -20), 10));

    camera cam(WIDTH, HEIGHT, FOCAL_LENGTH, objects);

    std::cout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

    for (int j = 0; j < HEIGHT; ++j) {
        std::clog << "\rScanlines: " << j << '/' << WIDTH - 1 << std::flush;

        for (int i = 0; i < WIDTH; i++) {
            write_color(std::cout, cam.sampled_pixel_at(i, j));
        }
    }

    std::clog << "\rRendering complete.                 \n";
}

