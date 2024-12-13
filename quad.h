#ifndef QUAD_H
#define QUAD_H

#include "hittable.h"
#include "rtcentral.h"

class quad : public hittable {
  public:
    quad(const point3& Q, const vec3& u, const vec3& v, shared_ptr<material> mat)
        : Q(Q), u(u), v(v), mat(mat) {
            auto n = cross(u, v);
            normal = unit_vector(n);
            D = dot(normal, Q);
            w = n / dot(n,n);
        }

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        auto denom = dot(normal, r.direction());

        // No hit if the ray is parallel to the plane.
        if (std::fabs(denom) < 1e-8)
            return false;

        // Return false if the hit point parameter t is outside the ray interval.
        auto t = (D - dot(normal, r.origin())) / denom;
        if (!ray_t.contains(t))
            return false;

        // Determine if the hit point lies within the planar shape using its plane coordinates.
        auto intersection = r.at(t);
        vec3 planar_hitpt_vector = intersection - Q;
        auto alpha = dot(w, cross(planar_hitpt_vector, v));
        auto beta = dot(w, cross(u, planar_hitpt_vector));

        if (!is_interior(alpha, beta, rec))
            return false;

        // Ray hits the 2D shape; set the rest of the hit record and return true.
        rec.t = t;
        rec.p = intersection;
        rec.mat = mat;
        rec.set_face_normal(r, normal);

        return true;
    }

    virtual bool is_interior(double a, double b, hit_record& rec) const {
        interval unit_interval = interval(0, 1);
        // Given the hit point in plane coordinates, return false if it is outside the
        // primitive, otherwise set the hit record UV coordinates and return true.

        if (!unit_interval.contains(a) || !unit_interval.contains(b))
            return false;

        //rec.u = a;
        //rec.v = b;
        return true;
    }

    point3 mins() const override {
        point3 result = point3();

        result[0] = std::min(Q[0], std::min(Q[0] + u[0], std::min(Q[0] + v[0], Q[0] + u[0] + v[0])));
        result[1] = std::min(Q[1], std::min(Q[1] + u[1], std::min(Q[1] + v[1], Q[1] + u[1] + v[1])));
        result[2] = std::min(Q[2], std::min(Q[2] + u[2], std::min(Q[2] + v[2], Q[2] + u[2] + v[2])));

        return result;
    }

    point3 maxs() const override {
        point3 result = point3();

        result[0] = std::max(Q[0], std::max(Q[0] + u[0], std::max(Q[0] + v[0], Q[0] + u[0] + v[0])));
        result[1] = std::max(Q[1], std::max(Q[1] + u[1], std::max(Q[1] + v[1], Q[1] + u[1] + v[1])));
        result[2] = std::max(Q[2], std::max(Q[2] + u[2], std::max(Q[2] + v[2], Q[2] + u[2] + v[2])));

        return result;
    }

    point3 cent() const override {
        point3 result = point3();

        result[0] = (Q[0] + Q[0] + u[0] + v[0]) / 2;
        result[1] = (Q[1] + Q[1] + u[1] + v[1]) / 2;
        result[2] = (Q[2] + Q[2] + u[2] + v[2]) / 2;

        return result;
    }

  private:
    point3 Q;
    vec3 u, v;
    vec3 w;
    shared_ptr<material> mat;
    vec3 normal;
    double D;
};

#endif