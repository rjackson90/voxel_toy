
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

Mesh::Mesh()
    : vertex_count(0), index_count(0), buffers({0,0}), vao(0), program(0), 
      vertex_source(nullptr), fragment_source(nullptr), vertex_shader(0), fragment_shader(0)
{
    glGenBuffers(2, &buffers[0]);
    glGenVertexArrays(1, &vao);
}

void Mesh::loadData(Vertex *verts, int vert_count, short *index_array, int index_count)
{
    // Store the number of vertices and the number of indices in the mesh object
    vertex_count = vert_count;
    this->index_count = index_count;

    // Bind vertex array, vertex buffer, element buffer
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTEX_BUFFER_INDEX]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[ELEMENT_BUFFER_INDEX]);

    // Initialize the buffers by loading in data
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vert_count, (void *) verts, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * index_count, (void *)index_array, 
                 GL_STATIC_DRAW);

    // Tell OpenGL about the layout of the data in the vertex buffer, map attributes to shader 
    // indexes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) sizeof(Vector3f));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) (2*sizeof(Vector3f)));
    
    // Enable vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    // unbind the buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
}

void test_cube(Mesh &mesh)
{
    // Cubes have 8 vertices
    Vertex verts[8];

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

    int index_length = 6*2*3;
    short indices[] = 
    {
        0, 3, 1,
        1, 3, 2,
        4, 1, 0,
        1, 4, 5,
        7, 5, 4,
        5, 6, 7,
        3, 0, 4,
        4, 7, 3,
        3, 7, 6,
        6, 2, 3,
        5, 1, 2,
        2, 6, 5
    };

    mesh.loadData(&verts[0], 8, &indices[0], index_length);
}

void test_latlong_sphere(Mesh &mesh)
{
    const int lat_div = 36;
    const int long_div = 76;

    const int vert_count = lat_div * long_div;

    Vertex verts[vert_count];

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
    short indices[10];

    mesh.loadData(&verts[0], vert_count, &indices[0], 10);
}
