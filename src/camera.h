#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#include "screen_object.h"
#include "ray.h"
#include "color.h"

class camera_params {
    int width, height;
    double viewport_width, viewport_height;

    double focal_length;
    point3 center;

    vec3 u, v, w; // camera frame basis vectors

    public:

    int samples_per_pixel;
    int max_depth;

    double vfov; // vertical view angle


    camera_params(int width, int height);
    void set_focus(point3 lookfrom, point3 lookat, vec3 vup);
    void set_size(int width, int height);

    private:
    void set_viewport_size(); // set viewport size, ensuring that width:height = viewport width:viewport height

    friend class camera;
};

class camera {
    camera_params params;

    vec3 viewport_u, viewport_v;
    vec3 pixel_delta_u, pixel_delta_v;

    point3 viewport_upper_left;
    point3 pixel00_loc;

    std::shared_ptr<screen_object> object; 

    public:
    camera(camera_params params, std::shared_ptr<screen_object> object);
    
    void set_object(std::shared_ptr<screen_object> object);
    color pixel_at(int x, int y) const;
    color sampled_pixel_at(int x, int y) const;

    private:
    color ray_color(const ray& r, int depth) const;
};

#endif
