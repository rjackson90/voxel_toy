#ifndef MESHES_H
#define MESHES_H

#include <math.h>

#define RADIANS(x) ((x) * (PI) / 180.0f)
#define DEGREES(x) ((x) * 180.0f / (PI))

const float PI = 4.0f * atan(1);

class Vector3f
{
    public:

    Vector3f(float, float, float);
    float magnitude();
    Vector3f normalized();

    float x, y, z;
};

class Vertex
{
    public:

    Vertex(const Vector3f&, const Vector3f&, const Vector3f&);

    Vector3f position, normal, color;
};

class Mesh
{
    public:

    Mesh(Vertex*, int, short*, int);
    ~Mesh();

    private:

    Vertex  *vertices;
    int     vertex_count;
    short   *indices;
    int     index_count;
};

Mesh test_cube();

Mesh test_latlong_sphere();

Mesh test_ico_sphere();

Mesh test_tetrahedron();

Mesh test_cone();

#endif
