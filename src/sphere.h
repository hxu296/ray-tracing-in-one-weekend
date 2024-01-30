#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
  public:
    sphere(point3 _center, double _radius, shared_ptr<material> _material)
      : center(_center), radius(_radius), mat(_material) {}

    bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
        // determine the number of solutions (t) for ray direction intersecting the sphere equation
        // the quadratic equation is:
        // t^2 * dot(B,B) + 2*t*dot(B,A-C) + dot(A-C,A-C) - R^2 = 0
        // where A is the ray origin, B is the ray direction, C is the sphere center, R is the radius
        // the descriminant is:
        // D = b^2 - 4ac
        // where a = dot(B,B), b = 2*dot(B,A-C), c = dot(A-C,A-C) - R^2
        // if D < 0, there is no solution, the ray does not intersect the sphere
        // if D = 0, there is one solution, the ray is tangent to the sphere
        // if D > 0, there are two solutions, the ray goes through the sphere
        vec3 oc = r.origin() - center; // vector from sphere center to ray origin
        auto a = r.direction().length_squared();
        auto half_b = dot(oc, r.direction());
        auto c = oc.length_squared() - radius*radius;

        auto discriminant = half_b*half_b - a*c;
        if (discriminant < 0) return false;
        auto sqrtd = sqrt(discriminant);

        // Find the nearest root that lies in the acceptable range.
        auto root = (-half_b - sqrtd) / a;
        if (!ray_t.surrounds(root)) {
            root = (-half_b + sqrtd) / a; // try the larger root
            if (!ray_t.surrounds(root))
                return false;
        }

        rec.t = root;
        rec.p = r.at(rec.t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);
        rec.mat = mat;

        return true;
    }

  private:
    point3 center;
    double radius;
    shared_ptr<material> mat;
};
