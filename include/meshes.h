#ifndef MESHES_H
#define MESHES_H

#include <math.h>
#include <algorithm>
#include <GL/glew.h>

#define RADIANS(x) ((x) * (PI) / 180.0f)
#define DEGREES(x) ((x) * 180.0f / (PI))

const float PI = 4.0f * atan(1);
const int VERTEX_BUFFER_INDEX = 0;
const int ELEMENT_BUFFER_INDEX = 1;

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

    Mesh();
    void loadData(Vertex*, int, short*, int);
    void draw();

    private:

    int     vertex_count;
    int     index_count;
    GLuint buffers[2];
    GLuint vao;
    GLuint program;
    GLchar *vertex_source, *fragment_source;
    GLuint vertex_shader, fragment_shader;
};

void test_cube(Mesh &);

void test_latlong_sphere(Mesh &);

void test_ico_sphere(Mesh &);

void test_tetrahedron(Mesh &);

void test_cone(Mesh &);

#endif
