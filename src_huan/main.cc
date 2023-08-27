#include "color.h"
#include "vec3.h"
#include "ray.h"

#include <iostream>

double hit_sphere(const point3& center, double radius, const ray& r) {
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
   auto a = dot(r.direction(), r.direction());
   auto b = 2.0 * dot(r.direction(), r.origin() - center);
   auto c = dot(r.origin() - center, r.origin() - center) - radius*radius;
   auto discriminant = b*b - 4*a*c;
    if (discriminant < 0) {
         return -1.0; // because t >= 0, -1.0 means no solution
    } else {
         // return the smaller t, it is the front intersection point
         return (-b - sqrt(discriminant)) / (2.0*a);
    }
}

color ray_color(const ray& r) {
    auto t = hit_sphere(point3(0,0,-1), 0.5, r);
    if (t > 0.0) {
        // calculate the intersection point
        auto intersection_point = r.at(t);
        // calculate the normal vector at the intersection point
        auto normal = unit_vector(intersection_point - point3(0,0,-1));
        // transform the normal vector to a color, normal in [-1, 1], color in [0, 1]
        return 0.5 * color(normal.x()+1, normal.y()+1, normal.z()+1);
    }
    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0); // y in [-1, 1], a in [0, 1]
    // blend value = (1-a)*start_value + a*end_value
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

int main() {
    // Image
    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    // Calculate the image height, and ensure that it's at least 1.
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera, centered at (0,0,0), looking at the negative z-axis
    auto focal_length = 1.0; // distance from the camera center to viewport
    auto viewport_height = 2.0;
    // use the actual aspect ratio of the image to calculate the viewport width
    auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);
    auto camera_center = point3(0, 0, 0);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    // pixel width and height in the viewport coordinate (u, v)
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    // viewport_upper_left is at (-viewport_width/2, +viewport_height/2, -focal_length)
    // this is also the location of the upper left corner of the pixel (0,0)
    auto viewport_upper_left = camera_center
                             - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    // pixel00_loc is the center of the pixel (0,0), in the viewport coordinate (u, v)
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; ++j) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            // for each pixel, we create a ray from the camera center to the pixel center
            // and then calculate the color of the pixel based on the ray
            
            // pixel center is the center of the pixel (i,j), in the viewport coordinate (u, v)
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }

    std::clog << "\rDone.                 \n";
}