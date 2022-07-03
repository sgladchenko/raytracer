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
        typedef float color_t;

        // Default color is black
        RGB()
            : red {0.0f}, green {0.0f}, blue {0.0f}
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

        std::string toStr() const;

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

        rt::RGB& operator()(int i, int j) { return buffer[j*Nx + i]; }
        rt::RGB& pixel(int i, int j) { return buffer[j*Nx + i]; }

        const rt::RGB& operator()(int i, int j) const { return buffer[j*Nx + i]; }
        const rt::RGB& pixel(int i, int j) const { return buffer[j*Nx + i]; }

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
        // Take information about the schene saved as JSON in <filename>
        Render(std::string fname, int _Nx, int _Ny);
        virtual ~Render();

        // Set point and direction of viewing
        void setPOV(rtVector3D _directionOfView, rtVector3D _pointOfView, float _aperture);

        // Returns a pixmap of the rendered picture
        const rt::Pixmap& pixmap();

        // Color of the background (when ray doesn't hit any faces)
        const rt::RGB blankColor {"#FFFFFF"};

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

        // Base colors of faces, colors of lights and the color of ambient light
        rt::RGB* facecolors;
        rt::RGB* lightcolors;
        rt::RGB ambientcolor;

        // Point of view
        rtVector3D directionOfView;
        rtVector3D pointOfView;
        float aperture;

        // Base vectors of the physical frame
        rtVector3D frameE_x;
        rtVector3D frameE_y;
        // Scale factor between physical coordinates of the frame and
        // coordinates in pixels
        float frameScale;

        // Function that makes a structure of rtView, taking
        // coordinates (in pixels) of a point in the frame
        void makertView(int i, int j, rtView* view);

        // Update cached pixmap if needed
        void doRender();
    };
}

#endif // __RENDER