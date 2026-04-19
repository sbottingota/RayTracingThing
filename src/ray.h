#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
    point3 orig;
    vec3 dir;
    double tm;

    public:
    ray() {}
    ray(const point3& orig, const point3& dir, double tm) : orig(orig), dir(dir), tm(tm) {}
    ray(const point3& orig, const point3& dir) : ray(orig, dir, 0) {}

    const point3& origin() const;
    const vec3& direction() const;
    double time() const;

    point3 at(double t) const;
};

#endif
