#include "render.hpp"

rt::RGB::RGB(const std::string& str)
{
    // Assume that color saved as "#......", where '.' is a hex digit
    red   = std::stoi(str.substr(1,2),0,16);
    green = std::stoi(str.substr(3,2),0,16);
    blue  = std::stoi(str.substr(5,2),0,16);
}

rt::RGB& rt::RGB::operator+=(const rt::RGB& color)
{
    // Basically it's just elementwise summation, except
    // there's a limit for a channel of 255
    red   = std::min(red   + color.red,   255);
    green = std::min(green + color.green, 255);
    blue  = std::min(blue  + color.blue,  255);
    return *this;
}

rt::RGB rt::RGB::operator*(const rt::RGB& color)
{
    // Basically it's just elementwise multiplication, except
    // there's a limit for a channel of 255 (so after mutiplying
    // those numbers we need to normalize them to max 255)
    return rt::RGB {(red   * color.red)   / 255,
                    (green * color.green) / 255,
                    (blue  * color.blue)  / 255};
}

rt::RGB rt::RGB::operator*(float factor)
{
    // Basically it's just elementwise multiplication on a number,
    // except there's a limit for a channel of 255
    color_t num = static_cast<color_t>(factor);
    return rt::RGB {std::min(red   * num, 255),
                    std::min(green * num, 255),
                    std::min(blue  * num, 255)};
}
