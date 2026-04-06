#ifndef MATERIAL_H
#define MATERIAL_H

#include "screen_object.h"
#include "color.h"

class material {
    public:
    virtual ~material() = default;

    virtual bool scatter(const ray& ray_in, const hit_record& record, color& attenuation, ray& scattered) const {
        return false;
    }
};

class lambertian: public material {
    color albedo;

    public:
    lambertian(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& ray_in, const hit_record& record, color& attenuation, ray& scattered) const override;
};

class metal : public material {
    color albedo;
    double fuzz;

    public:
    metal(const color& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}
    bool scatter(const ray& ray_in, const hit_record& record, color& attenuation, ray& scattered) const override;
};

class dielectric : public material {
    double refractive_index;

    public:
    dielectric(double refractive_index) : refractive_index(refractive_index) {}

    bool scatter(const ray& ray_in, const hit_record& record, color& attenuation, ray& scattered) const override;
};

#endif
