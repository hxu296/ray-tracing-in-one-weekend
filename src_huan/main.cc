#include "color.h"
#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "rtweekend.h"
#include "camera.h"

#include <iostream>


int main() {
    hittable_list world;

    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

    camera cam;

    cam.aspect_ratio = 16.0 / 9.0;
    cam.image_width  = 400;

    cam.render(world);
}