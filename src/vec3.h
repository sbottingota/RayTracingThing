#ifndef VEC3_H
#define VEC3_H

#include <iostream>

class vec3 {
    double e[3];

    public:
    vec3() : e{0, 0, 0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

    vec3 operator-() const;
    double operator[](int i) const;
    double& operator[](int i);

    vec3& operator+=(const vec3& v);
    vec3& operator*=(const vec3& v);
    vec3& operator*=(double t);
    vec3& operator/=(double t);

    vec3 operator+(const vec3& v) const;
    vec3 operator-(const vec3& v) const;
    vec3 operator*(const vec3& v) const;
    vec3 operator/(double t) const;

    double dot(const vec3& v) const;
    vec3 cross(const vec3& v) const;

    vec3 unit_vector() const;

    double length() const;
    double length_squared() const;

    bool near_zero() const;

    vec3 reflected(const vec3& normal) const;
    vec3 refracted(const vec3& normal, double eta_i_over_eta_t) const;

    static vec3 random();
    static vec3 random(double min, double max);
    static vec3 random_unit();
};

using point3 = vec3; // alias for geometric clarity

// utility functions
std::ostream& operator<<(std::ostream& out, const vec3& v);
vec3 operator*(double t, const vec3& v);

#endif
