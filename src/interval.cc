#include "interval.h"

double interval::size() const {
    return max - min;
}

bool interval::contains(double x) const {
    return min <= x && x <= max;
}

bool interval::surrounds(double x) const {
    return min < x && x < max; 
}

double interval::clamp(double x) const {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}


constexpr double inf = std::numeric_limits<double>::infinity();

const interval interval::empty = interval(inf, -inf);
const interval interval::universe = interval(-inf, inf);


