#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <memory>

#include "geometry.h"
#include "ray.h"
#include "color.h"

class rendering {
    private:
    int width, height;
    double focal_length;

    int viewport_width, viewport_height;

    point3 camera_center = point3(0, 0, 0);

    vec3 viewport_u, viewport_v;
    vec3 pixel_delta_u, pixel_delta_v;

    point3 viewport_upper_left;
    point3 pixel00_loc;

    std::vector<std::unique_ptr<screen_object>> objects; 

    public:
    rendering(int width, int height, double focal_length);

    void add_object(std::unique_ptr<screen_object> object);
    color pixel_at(int x, int y) const;

    private:
    color ray_color(const ray& r) const;
};

#endif
