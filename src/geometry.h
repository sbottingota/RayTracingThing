#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "ray.h"

class hit_record {
  public:
    point3 p;
    vec3 normal;
    double t;
};

class screen_object {
    public:
    virtual ~screen_object() = default;
    virtual bool hits(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const = 0;
};


class sphere : public screen_object {
    point3 center;
    double radius;

    public:
    sphere(const point3& center, double radius) : center(center), radius(std::max(0.0, radius)) {}
    virtual bool hits(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override;
};

#endif
