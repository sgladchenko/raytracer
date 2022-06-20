#ifndef __RTCORE
#define __RTCORE

///////////////////////////////////////////
/* Core structures needed for raytracing */
///////////////////////////////////////////

/* Just some easier way to handle buffers of arrays containing components of vectors.
In order to apply SIMD these structs will be usually allocated contigiously.
So technically floats in them will be allocated next to each other, so we
will still be able to apply loads from memory to vector registers. */
struct rtVector3D
{
    float x;
    float y;
    float z;
};

/* This structure defines points present in the mesh and faces spanned on them */
struct rtMesh
{
    // Number of points
    int Npoints;
    // Number of faces
    int NFaces;
    // Points themselves
    struct rtVector3D *points;
    // Points of faces; e.g. p0[i] defines a base point of ith face
    int *p0, *p1, *p2;
};

/* Values describing faces of polygons */
struct rtFaces
{
    // Total number of faces (with SIMD, must be a multiple of RT_VNUM)
    int N;
    // Base points
    struct rtVector3D *p;
    // Side edges
    struct rtVector3D *e1, *e2;
    /* Normals */
    struct rtVector3D *n;
    /* Unit normals */
    struct rtVector3D *un;
};

/* Info about direction of viewing and the point of view */
struct rtView
{
    // Direction of view
    struct rtVector3D r;
    // Point of view
    struct rtVector3D p;
};

/* Info about lights */
struct rtLights
{
    // Total number of point-like lights
    int N;
    // Their points
    struct rtVector3D *p;
};

//////////////////////////////////////////
/* Core functions needed for raytracing */
//////////////////////////////////////////

struct rtFaces *rtMakeFaces(struct rtMesh *mesh);
void rtFreeFaces(struct rtFaces *faces);

int rtIntersect(struct rtFaces *faces, struct rtView *view, struct rtVector3D *intersection);
int rtIsExposed(struct rtFaces *faces,
                struct rtView *view,
                struct rtVector3D *lightpoint,
                int iface, struct rtVector3D *intersection,
                float* cosine);

#endif // __RTCORE