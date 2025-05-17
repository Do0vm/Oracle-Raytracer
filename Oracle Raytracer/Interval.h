#ifndef INTERVAL_H
#define INTERVAL_H

#include <limits>    // for numeric_limits<double>::infinity()

class interval {
public:
    double min, max;

    // default constructor: empty interval
    interval()
        : min(std::numeric_limits<double>::infinity()),
        max(-std::numeric_limits<double>::infinity())
    {}

    interval(double _min, double _max)
        : min(_min), max(_max)
    {}

    bool contains(double x) const {
        return min <= x && x <= max;
    }

    bool surrounds(double x) const {
        return min < x && x < max;
    }

    double clamp(double x) const {
        if (x < min) return min;
        if (x > max) return max;
        return x;
    }

    // declarations of two special intervals:
    static const interval empty;
    static const interval universe;
};



#endif // INTERVAL_H
