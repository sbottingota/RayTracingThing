#include "camera.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>
#include <iostream>

#include "util.h"
#include "material.h"

constexpr double inf = std::numeric_limits<double>::infinity();

camera_params::camera_params(int width, int height) {
    samples_per_pixel = 10;
    max_depth = 10;
    vfov = 90;

    set_focus(point3(0, 0, 0), point3(0, 0, -1), vec3(0, 1, 0));
    set_size(width, height);
}

void camera_params::set_focus(point3 lookfrom, point3 lookat, vec3 vup) {
    center = lookfrom;
    focal_length = (lookfrom - lookat).length();

    w = (lookfrom - lookat).unit_vector();
    u = vup.cross(w).unit_vector();
    v = w.cross(u);
}

void camera_params::set_size(int width, int height) {
    this->width = width;
    this->height = height;
}

// set viewport aspect ratio to width:height
void camera_params::set_viewport_size() {
    double theta = vfov * (M_PI / 180);
    double h = std::tan(theta / 2);

    viewport_height = 2 * h * focal_length;
    viewport_width = width * viewport_height / height;
}

camera::camera(camera_params params, std::shared_ptr<screen_object> object) : params(params), object(object) {
    params.set_viewport_size();

    viewport_u = params.viewport_width * params.u;
    viewport_v = params.viewport_height * -params.v;

    pixel_delta_u = viewport_u / params.width;
    pixel_delta_v = viewport_v / params.height;

    viewport_upper_left = params.center - params.focal_length * params.w - viewport_u / 2 - viewport_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
}

void camera::set_object(std::shared_ptr<screen_object> object) {
    this->object = object;
}

color camera::ray_color(const ray& r, int depth) const {
    if (depth <= 0) {
        return color(0, 0, 0);
    }

    hit_record record;
    if (object->hits(r, interval(0.01, inf), record)) {
        ray scattered;
        color attenuation;

        if (record.mat->scatter(r, record, attenuation, scattered)) {
            return attenuation * ray_color(scattered, depth - 1);
        } else {
            return color(0, 0, 0);
        }
    }

    // create a gradient
    vec3 unit_direction = r.direction().unit_vector();
    double a =  0.5 * (unit_direction[1] + 1.0);
    return (1.0-a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}

color camera::pixel_at(int x, int y) const {
    point3 pixel_center = pixel00_loc + x*pixel_delta_u + y*pixel_delta_v;
    vec3 ray_direction = pixel_center - params.center;
    ray r(params.center, ray_direction);

    return ray_color(r, params.max_depth);
}

color camera::sampled_pixel_at(int x, int y) const {
    color pixel_color(0, 0, 0);

    for (int i = 0; i < params.samples_per_pixel; ++i) {
        pixel_color += pixel_at(x + random_double()-0.5, y + random_double()-0.5);
    }

    pixel_color /= params.samples_per_pixel;
    return pixel_color;
}

