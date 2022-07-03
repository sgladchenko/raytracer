#include "render.hpp"

#include <iostream>
#include <fstream>
#include <cmath>
#include <sstream>

#include "json.hpp"

// Keywords that are used to separate sections in a JSON file

const std::string points_k = "points";
const std::string faces_k = "faces";

const std::string lights_k = "lights";
const std::string pointlike_k = "pointlike";
const std::string ambient_k = "ambient";

// Conversion from deg to rad
inline float degToRad(float x)
{
    return M_PIf * (x / 180.0f);
}

// Some useful function for forming the vector of viewing

inline rtVector3D normalize(rtVector3D vec)
{
    float norm = std::sqrt(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
    return rtVector3D {vec.x / norm,
                       vec.y / norm,
                       vec.z / norm};
}

inline rtVector3D cross(rtVector3D v1, rtVector3D v2)
{
    return rtVector3D {v1.y*v2.z - v1.z*v2.y,
                       v1.z*v2.x - v1.x*v2.z,
                       v1.x*v2.y - v1.y*v2.x};
}

inline rtVector3D operator+ (rtVector3D v1, rtVector3D v2)
{
    return rtVector3D {v1.x + v2.x,
                       v1.y + v2.y,
                       v1.z + v2.z};
}

inline rtVector3D operator* (rtVector3D v, float factor)
{
    return rtVector3D {v.x * factor,
                       v.y * factor,
                       v.z * factor};
}

// E_3 vector of the main coordinate frame
const rtVector3D E_3 {0.0f, 0.0f, 1.0f};


// Read the whole text file into std::string
std::string readFile(std::string fname)
{
    std::ifstream ifs(fname);
    std::stringstream ss; 
    ss << ifs.rdbuf();
    return ss.str();
}

rt::Render::Render(std::string fname, int Nx, int Ny) :
    pixmapCache {Nx, Ny},
    Nx {Nx}, Ny {Ny}
{
    // Text of the whole JSON file containing information about
    // mesh, faces and lights
    std::string jsontxt = readFile(fname);
    nlohmann::json j = nlohmann::json::parse(jsontxt);

    ////////////////////////////////////////////////////////
    // Obtain rtMesh; make rtFaces out of it, save colors //

    mesh = rtMakeMesh(j[points_k].size(), j[faces_k].size());
    facecolors = new rt::RGB[mesh->Nfaces];

    for (int i = 0; i < mesh->Npoints; i++)
    {
        mesh->points[i].x = j[points_k][i][0].get<float>();
        mesh->points[i].y = j[points_k][i][1].get<float>();
        mesh->points[i].z = j[points_k][i][2].get<float>();
    }

    for (int i = 0; i < mesh->Nfaces; i++)
    {
        mesh->p0[i] = j[faces_k][i][0][0].get<int>();
        mesh->p1[i] = j[faces_k][i][0][1].get<int>();
        mesh->p2[i] = j[faces_k][i][0][2].get<int>();
        // For each face save its base color
        facecolors[i] = rt::RGB(j[faces_k][i][1].get<std::string>());
    }

    faces = rtMakeFaces(mesh);

    //////////////////////////////////////////////////////
    // Save positions of light sources and their colors //

    lights = rtMakeLights(j[lights_k][pointlike_k].size());
    lightcolors = new rt::RGB[lights->N];

    for (int i = 0; i < lights->N; ++i)
    {
        lights->p[i].x = j[lights_k][pointlike_k][i][0][0].get<float>();
        lights->p[i].y = j[lights_k][pointlike_k][i][0][1].get<float>();
        lights->p[i].z = j[lights_k][pointlike_k][i][0][2].get<float>();
        // Save colors of the light sources
        lightcolors[i] = rt::RGB(j[lights_k][pointlike_k][i][1].get<std::string>());
    }

    // Save a color of ambient light
    ambientcolor = rt::RGB(j[lights_k][ambient_k].get<std::string>());
}

void rt::Render::setPOV(rtVector3D _directionOfView, rtVector3D _pointOfView, float _aperture)
{
    // Always normalize this vector to 1
    directionOfView = normalize(_directionOfView);
    pointOfView = _pointOfView;
    // Save aperture in radians
    aperture = degToRad(_aperture);
    // After changing POV we will need to re-render the pixmap
    pixmapReady = false;

    // Form base vectors defining orientation of the frame
    frameE_x = normalize(cross(E_3, directionOfView));
    frameE_y = cross(directionOfView, frameE_x);

    // Scaling factor between physical coordinates and coordinates in pixels
    frameScale = (2.0f * std::tan(aperture / 2.0f)) / Nx;
}

void rt::Render::makertView(int i, int j, rtView* view)
{
    // Relative (physical) coordinates of a point in the frame
    float dx = frameScale * (i - Nx/2.0f);
    float dy = frameScale * (j - Ny/2.0f);

    view->r = directionOfView + (frameE_x*dx + frameE_y*dy)*(-1.0f);
    view->p = pointOfView;
}

void rt::Render::doRender()
{
    for (int i = 0; i < Nx; ++i)
    {
        for (int j = 0; j < Ny; ++j)
        {
            /////////////////////////////////////////
            // Evaluate (i,j) pixel in the picture //

            // Currect direction of viewing (ray)
            rtView view;
            makertView(i, j, &view);

            // Find which face this ray intersects
            rtVector3D intersection;
            int iface = rtIntersect(faces, &view, &intersection);

            if (iface == -1)
            {
                // We haven't hit any face; that means that the pixel
                // must be black
                pixmapCache(i,j) = blankColor;
                continue;
            }
            else
            {
                // We hit a face, now we need to find out the color of the pixel
                // The first contribution comes from ambient light
                rt::RGB pixelColor = ambientcolor*facecolors[iface];

                // Then for each light source understand if it exposes
                // the point that we are seeing, if it does, than add
                // its contribution
                for (int l = 0; l < lights->N; ++l)
                {
                    float cosine;
                    if (rtIsExposed(faces, &view, &(lights->p[l]), iface, &intersection, &cosine))
                    {
                        pixelColor += (lightcolors[l]*cosine)*facecolors[iface];
                    }
                }

                // Set the color in the pixmap
                pixmapCache(i,j) = pixelColor;
            }
        }
    }
}

const rt::Pixmap& rt::Render::pixmap()
{
    if (!pixmapReady) { doRender(); }
    return pixmapCache;
}

rt::Render::~Render()
{
    rtFreeLights(lights);
    rtFreeFaces(faces);
    rtFreeMesh(mesh);

    delete [] facecolors;
    delete [] lightcolors;
}