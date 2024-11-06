#pragma once

#include "color.hpp"

#include <iostream>
#include <vector>

class Image
{
private:

    int width;
    int height;

    std::vector<std::vector<Color>> pixels;

public:

    Image() = default;
    Image(int width, int height);

    void set_pixel(int i, int j, const Color &color);

    void write_as_ppm(std::ostream &out) const;
};