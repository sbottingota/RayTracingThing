#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>

class interval {
    public:
    double min, max;

    interval(double min, double max) : min(min), max(max) {}

    double size() const;   
    bool contains(double x) const;
    bool surrounds(double x) const;

    static const interval empty;
    static const interval universe;
};

#endif
