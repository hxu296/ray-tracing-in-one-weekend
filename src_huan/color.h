#pragma once

#include "vec3.h"

#include <iostream>

using color = vec3;

void write_color(std::ostream &out, const color& pixel_color) {
    // Write the translated [0,255] value of each color component.
    // assume pixel_color x y z are in [0,1]
    out << static_cast<int>(255.999 * pixel_color.x()) << ' '
        << static_cast<int>(255.999 * pixel_color.y()) << ' '
        << static_cast<int>(255.999 * pixel_color.z()) << '\n';
}