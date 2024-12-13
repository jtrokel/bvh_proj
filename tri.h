#ifndef TRI_H
#define TRI_H

#include "hittable.h"
#include "rtcentral.h"

class tri : public hittable {
  public:
    tri(const point3& a, const point3& b, const point3& c, shared_ptr<material> mat)
        : a(a), b(b), c(c), mat(mat) {
            const float cx = (a.x() + b.x() + c.x()) / 3;
            const float cy = (a.y() + b.y() + c.y()) / 3;
            const float cz = (a.z() + b.z() + c.z()) / 3;
            centroid = point3(cx, cy, cz);
        }

    point3 cent() const { return centroid; }

    // Moller-Trumbore intersection algorithm
    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        const vec3 e1 = b - a;
        const vec3 e2 = c - a;
        const vec3 h = cross(r.direction(), e2);
        const float a_dot_h = dot(e1, h);
        // Ray parallel to triangle
        if (a_dot_h > -1e-8 && a_dot_h < 1e-8)
            return false;

        const float det_inv = 1 / a_dot_h;
        const vec3 s = r.origin() - a;
        const float u = det_inv * dot(s, h);
        if (u < 0 || u > 1)
            return false;
        
        const vec3 q = cross(s, e1);
        const float v = det_inv * dot(r.direction(), q);
        if (v < 0 || u + v > 1)
            return false;

        const float t = det_inv * dot(e2, q);
        if (!ray_t.contains(t))
            return false;

        rec.t = t;
        rec.p = r.at(t);
        rec.mat = mat;
        rec.set_face_normal(r, unit_vector(cross(e1, e2)));

        return true;
    }

    point3 mins() const override {
        point3 result = point3();

        result[0] = std::min(a[0], std::min(b[0], c[0]));
        result[1] = std::min(a[1], std::min(b[1], c[1]));
        result[2] = std::min(a[2], std::min(b[2], c[2]));

        return result;
    }

    point3 maxs() const override {
        point3 result = point3();

        result[0] = std::max(a[0], std::max(b[0], c[0]));
        result[1] = std::max(a[1], std::max(b[1], c[1]));
        result[2] = std::max(a[2], std::max(b[2], c[2]));

        return result;
    }

  private:
    point3 a, b, c, centroid;
    shared_ptr<material> mat;
};

#endif