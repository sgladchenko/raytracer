#pragma once

#include <string>
#include <vector>

extern "C"
{
    #include "rtcore.h"
}

namespace rt
{
    /* Very simple class that wraps things around colors and operations with them */
    class RGB
    {
    public:
        RGB() : r {0}, g {0}, b {0} {} // black, by default
        RGB(int _r, int _g, int _b) : r {_r}, g {_g}, b {_b} {}
        RGB(const std::string& str); // Take a color saved in a string as "#......"

        // Some algebraic operations to be used to mimic how light
        // scatters on the surface of a face
        rt::RGB& operator+=(const rt::RGB& color);
        rt::RGB operator*(const rt::RGB& color);
        rt::RGB operator*(float factor);

    private:
        int r, g, b;
    };

    /* Wrapper around buffer of colored pixels */
    class Pixmap
    {
    public:
        Pixmap(int _Nx, int _Ny) :
            Nx {_Nx}, Ny {_Ny}, buffer(_Nx*_Ny)
        {}

        // Formats supported for saving as an external file
        enum class format { png };
        void saveFile(std::string fname, rt::Pixmap::format fmt) const;

        // Set a pixel
        void setPixel(const rt::RGB& color);

    private:
        // Dimensions of the frame
        int Nx, Ny;
        // Vector that contains the buffer
        std::vector<rt::RGB> buffer;
    };

    /* Main object of the engine used to load mesh & faces and render a picture */
    class Render
    {
    public:
        // Take a mesh from <filename> saved as JSON
        Render(std::string fname, int _Nx, int _Ny);

        // Set the point and the direction of viewing
        void setView(rtVector3D direction,
                     rtVector3D pointofview,
                     float aperture);

        // Returns a pixmap of the rendered picture
        const rt::Pixmap& render();

    private:
        // Dimensions of the frame
        int Nx, Ny;
        // Cache the pixmap rendered
        rt::Pixmap cache;

        // Hidden C-language structures that are used in rtcore.h
        rtMesh* mesh;
        rtFaces* faces;
        rtLights* lights;

        // Function that makes a structure of rtView, taking the
        // coordinates in pixels within a frame
        rtView calc_rtView(int i, int j);

        // Update the cached pixmap if needed
        void do_render();
    };
}