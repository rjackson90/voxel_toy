
#include "meshes.h"

Vector3f::Vector3f(float X=0.0f, float Y=0.0f, float Z=0.0f)
    : x(X), y(Y), z(Z)
{}

float Vector3f::magnitude()
{
    return sqrt((x*x) + (y*y) + (z*z));
}

Vector3f Vector3f::normalized()
{
    Vector3f result = *this;
    float mag = result.magnitude();
    if(mag == 0.0f)
    {
        result.x = 0.0f;
        result.y = 0.0f;
        result.z = 0.0f;
        return result;
    }

    result.x /= mag;
    result.y /= mag;
    result.z /= mag;

    return result;
}

Vertex::Vertex(const Vector3f &pos = Vector3f(), const Vector3f &rgb = Vector3f(), 
               const Vector3f &norm = Vector3f())
    : position(pos), normal(norm), color(rgb)
{}

Mesh::Mesh(Vertex* vertices = nullptr, int vertex_count = 0, short *indices = nullptr,
           int index_count = 0)
    : vertices(vertices), vertex_count(vertex_count), indices(indices), index_count(index_count)
{}

Mesh::~Mesh()
{
    if(vertices != nullptr)
        delete[] vertices;
    if(indices != nullptr)
        delete[] indices;
}

Mesh test_cube()
{
    // Cubes have 8 vertices
    Vertex *verts = new Vertex[8];

    verts[0].position = Vector3f(-1.0f,  1.0f,  1.0f);
    verts[1].position = Vector3f( 1.0f,  1.0f,  1.0f);
    verts[2].position = Vector3f( 1.0f, -1.0f,  1.0f);
    verts[3].position = Vector3f(-1.0f, -1.0f,  1.0f);
    verts[4].position = Vector3f(-1.0f,  1.0f, -1.0f);
    verts[5].position = Vector3f( 1.0f,  1.0f, -1.0f);
    verts[6].position = Vector3f( 1.0f, -1.0f, -1.0f);
    verts[7].position = Vector3f(-1.0f, -1.0f, -1.0f);

    verts[0].color = Vector3f(0.0f, 1.0f, 0.0f);
    verts[1].color = Vector3f(1.0f, 1.0f, 0.0f);
    verts[2].color = Vector3f(0.0f, 1.0f, 0.0f);
    verts[3].color = Vector3f(0.0f, 0.0f, 0.0f);
    verts[4].color = Vector3f(0.0f, 1.0f, 1.0f);
    verts[5].color = Vector3f(1.0f, 1.0f, 1.0f);
    verts[6].color = Vector3f(0.0f, 1.0f, 1.0f);
    verts[7].color = Vector3f(0.0f, 0.0f, 1.0f);

    for(int i = 0; i < 8; i++)
    {
        verts[i].normal = verts[i].position.normalized();
    }

    short *indices = new short[10];

    return Mesh(verts, 8, indices, 10);
}

Mesh test_latlong_sphere()
{
    const int lat_div = 36;
    const int long_div = 76;

    int vert_count = lat_div * long_div;

    Vertex *verts = new Vertex[vert_count];

    for(int i =0; i < vert_count; i++)
    {
        float phi = RADIANS(360.0f / long_div * (i % long_div));
        float theta = RADIANS(180.0f / lat_div * (i / long_div));
        
        verts[i].position = Vector3f(cos(theta) * cos(phi), cos(theta) * sin(phi), sin(theta));
        verts[i].position.x = verts[i].position.x * 2.0f - 1.0f;
        verts[i].position.y = verts[i].position.y * 2.0f - 1.0f;
        verts[i].position.z = verts[i].position.z * 2.0f - 1.0f;

        verts[i].color = Vector3f(cos(theta) * cos(phi), cos(theta) * sin(phi), sin(theta));
        verts[i].normal = verts[i].position.normalized();
    }

    // This is just a placeholder, clearly incorrect
    short *indices = new short[10];

    return Mesh(verts, vert_count, indices, 10);
}
