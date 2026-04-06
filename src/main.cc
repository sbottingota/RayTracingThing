#include <iostream>
#include <memory>

#include "camera.h"
#include "material.h"
#include "parsing.h"

int main() {
    auto objects = std::make_shared<object_group>();

    auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = std::make_shared<dielectric>(1.50);
    auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.1);

    /*
    objects->add(std::make_shared<sphere>(point3(0.0, -100.5, -1.0), 100.0, material_ground));
    objects->add(std::make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    objects->add(std::make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));
    objects->add(std::make_shared<sphere>(point3(0.0, 0.0, -1.2), 0.5, material_center));
    */

    /*
    params.set_focus(point3(-2, 2, 1), point3(0, 0, -1), vec3(0, 1, 0));
    params.vfov = 20;
    */

    camera_params params;
    parse_file("shapes.txt", params, *objects);

    camera cam(params, objects);

    int width = params.get_width();
    int height = params.get_height();

    std::cout << "P3\n" << width << ' ' << height << "\n255\n";

    for (int j = 0; j < height; ++j) {
        std::clog << "\rScanlines: " << j << '/' << height - 1 << std::flush;

        for (int i = 0; i < width; i++) {
            write_color(std::cout, cam.sampled_pixel_at(i, j));
        }
    }

    std::clog << "\rRendering complete.                 \n";
}

