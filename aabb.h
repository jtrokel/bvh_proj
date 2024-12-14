#ifndef AABB_H
#define AABB_H

#include "interval.h"
#include "hittable.h"
#include <vector>

class aabb : hittable {
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
    std::vector<int> max_axis() {
        auto x_size = bounds[0].size();
        auto y_size = bounds[1].size();
        auto z_size = bounds[2].size();

        if (x_size > y_size && x_size > z_size) {
            if (y_size > z_size) {
                return {0, 1, 2};
            } else {
                return {0, 2, 1};
            }
        } else if (y_size > z_size) {
            if (x_size > z_size) {
                return {1, 0, 2};
            } else {
                return {1, 2, 0};
            }
        } else {
            if (x_size > y_size) {
                return {2, 0, 1};
            } else {
                return {2, 1, 0};
            }
        }
    }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // Overlap x, y, z
        interval overlapIntervals[3];
        for (int dim = 0; dim < 3; dim++) {
            auto h1 = (bounds[dim].min - r.origin()[dim]) / r.direction()[dim];
            auto h2 = (bounds[dim].max - r.origin()[dim]) / r.direction()[dim];
            overlapIntervals[dim].min = std::min(h1, h2);
            overlapIntervals[dim].max = std::max(h1, h2);
        }
        if (overlapIntervals[0].overlaps(overlapIntervals[1]) 
            && overlapIntervals[0].overlaps(overlapIntervals[2])
            && overlapIntervals[1].overlaps(overlapIntervals[2])) {
                return true;
        }
        return false;
    }


};

#endif
