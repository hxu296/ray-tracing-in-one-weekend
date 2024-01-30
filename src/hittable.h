#pragma once

#include "interval.h"
#include "ray.h"

class material;

struct hit_record {
    point3 p;
    vec3 normal;
    double t;
    shared_ptr<material> mat;
    bool front_face;

    void set_face_normal(const ray& r, const vec3& outward_normal) {
        // Sets the hit record normal vector.
        // NOTE: the parameter `outward_normal` is assumed to have unit length.

        front_face = dot(r.direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

// hittable is a pure virtual class (interface)
class hittable {
  public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval ray_t, hit_record& rec) const = 0;
};