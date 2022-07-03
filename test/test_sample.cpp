#include "render.hpp"

#include "fstream"

int main()
{
    rt::Render render("test/sample.json", 1920, 1080);

    render.setPOV(rtVector3D {-15.0f, -15.0f, -15.0f},
                  rtVector3D { 15.0f,  15.0f,  15.0f},
                  100.0);
    
    render.pixmap().saveFile("test_sample.png", rt::Pixmap::format::png);

    return 0;
}