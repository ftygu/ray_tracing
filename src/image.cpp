#include "image.hpp"

#include <iostream>

Image::Image(int width, int height) : width(width), height(height)
{
    pixels.resize(height);
    for (int j = 0; j < height; ++j)
    {
        pixels[j].resize(width);
    }
}

void Image::set_pixel(int i, int j, const Color &color)
{
    pixels[j][i] = color;
}

void Image::write_as_ppm(std::ostream &out) const
{
    out << "P3\n" << width << ' ' << height << "\n255\n";
    for (int j = 0; j < height; ++j)
    {
        for (int i = 0; i < width; ++i)
        {
            pixels[j][i].write_as_ppm(out);
        }
    }
}