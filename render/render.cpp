#include "render.hpp"

#include <iostream>
#include <fstream>

#include "json.hpp"

// Keywords that are used to separate sections in a JSON file

const std::string points_k = "points";
const std::string faces_k = "faces";

const std::string lights_k = "lights";
const std::string pointlike_k = "pointlike";
const std::string ambient_k = "ambient";

// Smaller function to read the whole text file into std::string
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



rt::Render::~Render()
{
    rtFreeLights(lights);
    rtFreeFaces(faces);
    rtFreeMesh(mesh);

    delete [] facecolors;
    delete [] lightcolors;
}