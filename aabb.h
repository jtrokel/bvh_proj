#ifndef AABB_H
#define AABB_H
#include "interval.h"

class aabb {
  public:
    // intervals for the x, y, and z axes
    interval bounds[3];

    aabb() : bounds() {} // Default constructor
    aabb(const interval& x, const interval& y, const interval& z) {
        bounds[0] = x;
        bounds[1] = y;
        bounds[2] = z;
    }

    // Returns the axis with the largest interval size.
    int max_axis() {
        auto x_size = bounds[0].size();
        auto y_size = bounds[1].size();
        auto z_size = bounds[2].size();

        if (x_size > y_size && x_size > z_size) {
            return 0;
        } else if (y_size > z_size) {
            return 1;
        } else {
            return 2;
        }
    }

};

#endif
