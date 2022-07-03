#include "render.hpp"

#include <iomanip>
#include <sstream>

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
    red   = std::min(red   + color.red,   255.0f);
    green = std::min(green + color.green, 255.0f);
    blue  = std::min(blue  + color.blue,  255.0f);
    return *this;
}

rt::RGB rt::RGB::operator*(const rt::RGB& color)
{
    // Basically it's just elementwise multiplication, except
    // there's a limit for a channel of 255 (so after mutiplying
    // those numbers we need to normalize them to max 255)
    return rt::RGB {(red   * color.red)   / 255.0f,
                    (green * color.green) / 255.0f,
                    (blue  * color.blue)  / 255.0f};
}

rt::RGB rt::RGB::operator*(float factor)
{
    // Basically it's just elementwise multiplication on a number,
    // except there's a limit for a channel of 255
    return rt::RGB {std::min(red   * factor, 255.0f),
                    std::min(green * factor, 255.0f),
                    std::min(blue  * factor, 255.0f)};
}

std::string rt::RGB::toStr() const
{
    std::stringstream ss;
    ss << "#" << std::setfill('0') << std::setw(2) << std::hex <<( int)red
              << std::setfill('0') << std::setw(2) << std::hex << (int)green
              << std::setfill('0') << std::setw(2) << std::hex << (int)blue;
    return ss.str();
}