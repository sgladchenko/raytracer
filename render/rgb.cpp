#include "render.hpp"

rt::RGB::RGB(const std::string& str)
{
    // Assume that color saved as "#......", where '.' is a hex digit
    r = std::stoi(str.substr(1,2),0,16);
    g = std::stoi(str.substr(3,2),0,16);
    b = std::stoi(str.substr(5,2),0,16);
}

rt::RGB& rt::RGB::operator+=(const rt::RGB& color)
{
    // Basically it's just elementwise summation, except
    // there's a limit for a channel of 255
    r = std::min(r + color.r, 255);
    g = std::min(g + color.g, 255);
    b = std::min(b + color.b, 255);
    return *this;
}

rt::RGB rt::RGB::operator*(const rt::RGB& color)
{
    // Basically it's just elementwise multiplication, except
    // there's a limit for a channel of 255 (so after mutiplying
    // those numbers we need to normalize them to max 255)
    return rt::RGB {(r * color.r) / 255,
                    (g * color.g) / 255,
                    (b * color.b) / 255};
}

rt::RGB rt::RGB::operator*(float factor)
{
    // Basically it's just elementwise multiplication on a number,
    // except there's a limit for a channel of 255
    int num = static_cast<int>(factor);
    return rt::RGB {std::min(r * num, 255),
                    std::min(g * num, 255),
                    std::min(b * num, 255)};
}

