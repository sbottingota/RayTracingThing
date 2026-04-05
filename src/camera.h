#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#include "geometry.h"
#include "ray.h"
#include "color.h"

class camera {
    private:
    int width, height;
    double focal_length;

    int samples_per_pixel = 10;

    double viewport_width, viewport_height;

    point3 camera_center = point3(0, 0, 0);

    vec3 viewport_u, viewport_v;
    vec3 pixel_delta_u, pixel_delta_v;

    point3 viewport_upper_left;
    point3 pixel00_loc;

    std::shared_ptr<screen_object> object; 

    public:
    camera(int width, int height, double focal_length, std::shared_ptr<screen_object> object);
    
    void set_object(std::shared_ptr<screen_object> object);
    color pixel_at(int x, int y) const;
    color sampled_pixel_at(int x, int y) const;

    private:
    color ray_color(const ray& r) const;
};

#endif
