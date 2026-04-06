#ifndef UTIL_H
#define UTIL_H

// returns a random double in the range[0, 1)
double random_double();

class interval {
    public:
    double min, max;

    interval(double min, double max) : min(min), max(max) {}

    double size() const;   
    bool contains(double x) const;
    bool surrounds(double x) const;
    double clamp(double x) const;

    static const interval empty;
    static const interval universe;
};

#endif
