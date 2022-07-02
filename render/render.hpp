#ifndef __RENDER
#define __RENDER

#include <string>
#include <vector>

#include "rtcore.h"

namespace rt
{
    // Forward declarations of all the classes in this hpp
    class RGB;
    class Pixmap;
    class Render;
}

namespace rt
{
    /* Very simple class that wraps things around colors and operations with them */
    struct RGB
    {
        typedef int color_t;        
        // Default color is black
        RGB()
            : red {0}, green {0}, blue {0}
        {}

        RGB(color_t r, color_t g, color_t b)
            : red {r}, green {g}, blue {b}
        {}

        // Take a color saved in a string as "#......"
        RGB(const std::string& str);

        // Some algebraic operations to be used to mimic how light
        // scatters on the surface of a face
        rt::RGB& operator+=(const rt::RGB& color);
        rt::RGB operator*(const rt::RGB& color);
        rt::RGB operator*(float factor);

        color_t red, green, blue;
    };

    /* Wrapper around buffer of colored pixels */
    class Pixmap
    {
        // May be helpful as in multithreaded version the buffer will need to be
        // split into chunks evaluated by different threads.
        friend rt::Render;

    public:
        Pixmap(int _Nx, int _Ny) :
            Nx {_Nx}, Ny {_Ny}, buffer(_Nx*_Ny)
        {}

        // Formats supported for saving as an external file
        enum class format { png };
        void saveFile(std::string fname, rt::Pixmap::format fmt) const;

        rt::RGB& operator()(int i, int j) { return buffer[i*Nx + j]; }
        rt::RGB& pixel(int i, int j) { return buffer[i*Nx + j]; }

        const rt::RGB& operator()(int i, int j) const { return buffer[i*Nx + j]; }
        const rt::RGB& pixel(int i, int j) const { return buffer[i*Nx + j]; }

    private:
        // Dimensions of the frame
        int Nx, Ny;
        // Vector that contains the buffer
        std::vector<rt::RGB> buffer;

        void savepng(std::string fname) const;
    };

    /* Main object of the engine used to load mesh & faces and render a picture */
    class Render
    {
    public:
        // Take a mesh from <filename> saved as JSON
        Render(std::string fname, int _Nx, int _Ny);
        virtual ~Render();

        // Set the point and the direction of viewing
        void setView(rtVector3D direction,
                     rtVector3D pointofview,
                     float aperture);

        // Returns a pixmap of the rendered picture
        const rt::Pixmap& pixmap();

    private:
        // Dimensions of the frame
        int Nx, Ny;
        // Cache the rendered pixmap
        bool pixmapReady = false;
        rt::Pixmap pixmapCache;

        // Hidden C-language structures that are used in rtcore.h
        rtMesh* mesh;
        rtFaces* faces;
        rtLights* lights;

        // Base colors of faces, colors of lights and color of the ambient light
        rt::RGB* facecolors;
        rt::RGB* lightcolors;
        rt::RGB ambientcolor;

        // Function that makes a structure of rtView, taking the
        // coordinates in pixels within a frame
        rtView makeView(int i, int j);

        // Update the cached pixmap if needed
        void doRender();
    };
}

#endif // __RENDER