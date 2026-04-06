#include <iostream>
#include <memory>

#include "camera.h"
#include "material.h"

#define WIDTH 640
#define HEIGHT 360

int main() {
    auto objects = std::make_shared<object_group>();

    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<dielectric>(1.50);
    auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.1);

    objects->add(std::make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    objects->add(std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    objects->add(std::make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
    objects->add(std::make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));

    camera_params params(WIDTH, HEIGHT);
    // params.set_size(WIDTH, HEIGHT);
    camera cam(params, objects);

    std::cout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

    for (int j = 0; j < HEIGHT; ++j) {
        std::clog << "\rScanlines: " << j << '/' << WIDTH - 1 << std::flush;

        for (int i = 0; i < WIDTH; i++) {
            write_color(std::cout, cam.sampled_pixel_at(i, j));
        }
    }

    std::clog << "\rRendering complete.                 \n";
}

