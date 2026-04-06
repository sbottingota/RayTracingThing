#ifndef SCREEN_OBJECT_H
#define SCREEN_OBJECT_H

#include "ray.h"

#include <vector>
#include <memory>

#include "util.h"

class material;

class hit_record {
  public:
    point3 p;
    vec3 normal;
    std::shared_ptr<material> mat;
    double t;
    bool front_face;

    // sets the hit record normal vector
    // note that the parameter outward_normal is assumed to have unit length
    void set_face_normal(const ray& r, const vec3& outward_normal) {
        front_face = r.direction().dot(outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class screen_object {
    public:
    virtual ~screen_object() = default;
    virtual bool hits(const ray& r, interval ray_t, hit_record& rec) const = 0;
};


class object_group : public screen_object {
    std::vector<std::shared_ptr<screen_object>> objects;

    public:
    object_group() {}
    void add(std::shared_ptr<screen_object> object);
    virtual bool hits(const ray& r, interval ray_t, hit_record& record) const override;
};

class sphere : public screen_object {
    point3 center;
    double radius;
    std::shared_ptr<material> mat;

    public:
    sphere(const point3& center, double radius, std::shared_ptr<material> mat) : center(center), radius(std::max(0.0, radius)), mat(mat) {}
    virtual bool hits(const ray& r, interval ray_t, hit_record& record) const override;
};

#endif
