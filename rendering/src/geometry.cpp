#include "geometry.h"

using namespace Rendering;

Geometry::Geometry() : draw_mode(GL_POINTS), vertex_count(0), index_count(0), buffers{0,0}, vao(0)
{
    glGenBuffers(2, &buffers[0]);
    glGenVertexArrays(1, &vao);
}

Geometry::~Geometry()
{
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(2, &buffers[0]);
}

void Geometry::draw()
{
    glBindVertexArray(vao);
    glDrawElements(draw_mode, index_count, GL_UNSIGNED_SHORT, (GLvoid*) 0);
    glBindVertexArray(0);
}

void Geometry::setDrawMode(GLenum mode)
{
    draw_mode = mode;
}

void Geometry::loadData(Vertex *vert_buf, int vbuf_size, short* elem_buf, int ebuf_size)
{
    // Store the number of elements
    index_count = ebuf_size;

    // Bind arrays and buffers
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTEX_BUFFER_INDEX]);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[ELEMENT_BUFFER_INDEX]);

    // Fill the buffers
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vbuf_size, (void*) vert_buf, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * ebuf_size, 
            (void*) elem_buf, GL_STATIC_DRAW);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) sizeof(glm::vec3));

    // Enable vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Unbind the buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
 
/* Load unit cube geometry for testing 
 */
void Geometry::genTestCube()
{
    // Cubes have 8 vertices
    Vertex verts[8];
    
    verts[0].position = Vector(-1.0f,  1.0f,  1.0f);
    verts[1].position = Vector( 1.0f,  1.0f,  1.0f);
    verts[2].position = Vector( 1.0f, -1.0f,  1.0f);
    verts[3].position = Vector(-1.0f, -1.0f,  1.0f);
    verts[4].position = Vector(-1.0f,  1.0f, -1.0f);
    verts[5].position = Vector( 1.0f,  1.0f, -1.0f);
    verts[6].position = Vector( 1.0f, -1.0f, -1.0f);
    verts[7].position = Vector(-1.0f, -1.0f, -1.0f);
     
    verts[0].color = Vector(0.0f, 1.0f, 0.0f);
    verts[1].color = Vector(1.0f, 1.0f, 0.0f);
    verts[2].color = Vector(0.0f, 1.0f, 0.0f);
    verts[3].color = Vector(0.0f, 0.0f, 0.0f);
    verts[4].color = Vector(0.0f, 1.0f, 1.0f);
    verts[5].color = Vector(1.0f, 1.0f, 1.0f);
    verts[6].color = Vector(0.0f, 1.0f, 1.0f);
    verts[7].color = Vector(0.0f, 0.0f, 1.0f);
     
    verts[0].uv = Vector(1.0f, 1.0f, 0.0f);
    verts[1].uv = Vector(0.0f, 1.0f, 0.0f);
    verts[2].uv = Vector(0.0f, 0.0f, 0.0f);
    verts[3].uv = Vector(1.0f, 0.0f, 0.0f);
    verts[4].uv = Vector(0.0f, 1.0f, 0.0f);
    verts[5].uv = Vector(1.0f, 1.0f, 0.0f);
    verts[6].uv = Vector(1.0f, 0.0f, 0.0f);
    verts[7].uv = Vector(0.0f, 0.0f, 0.0f);
     
    for(int i = 0; i < 8; i++)
    {
        verts[i].normal = verts[i].position.normalize();
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
     
    loadData(&verts[0], 8, &indices[0], index_length);
}

