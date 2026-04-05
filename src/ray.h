#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    point3 orig;
    vec3 dir;

    public:
    ray() {}
    ray(const point3& orig, const point3& dir) : orig(orig), dir(dir) {}

    const point3& origin() const;
    const vec3& direction() const;

    point3 at(double t) const;
};

#endif
