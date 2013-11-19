#include "geometry.h"

using namespace Rendering;

Geometry::Geometry() : method(ELEMENTS), draw_mode(GL_POINTS), index_count(0), buffers{0,0}, vao(0)
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

void Geometry::draw() const
{
    glBindVertexArray(vao);
    glDrawElements(draw_mode, index_count, GL_UNSIGNED_SHORT, (GLvoid*) 0);
    glBindVertexArray(0);
}

void Geometry::setDrawMode(GLenum mode)
{
    draw_mode = mode;
}

void Geometry::loadVertexArray(Vertex *vert_buf, int vbuf_size, GLenum usage = GL_STATIC_DRAW)
{
    // Bind array and buffer
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, buffers[VERTEX_BUFFER_INDEX]);

    // Fill the buffers
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vbuf_size, (void*) vert_buf, usage);

    // Set vertex attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) 0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) sizeof(glm::vec3));

    // Enable vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // Unbind the buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Geometry::loadElements(short* buf, int size, GLenum usage = GL_STATIC_DRAW)
{
    index_count = size;

    // Bind the buffer
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[ELEMENT_BUFFER_INDEX]);

    // Fill the buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * size, (void*) buf, usage);

    // Unbind the buffer
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
 
/* Load unit cube geometry for testing 
 */
void Geometry::genTestCube()
{
    using glm::vec3;
    using glm::vec2;
    // Cubes have 8 vertices
    Vertex verts[8];
    
    verts[0].position = vec3(-1.0f,  1.0f,  1.0f);
    verts[1].position = vec3( 1.0f,  1.0f,  1.0f);
    verts[2].position = vec3( 1.0f, -1.0f,  1.0f);
    verts[3].position = vec3(-1.0f, -1.0f,  1.0f);
    verts[4].position = vec3(-1.0f,  1.0f, -1.0f);
    verts[5].position = vec3( 1.0f,  1.0f, -1.0f);
    verts[6].position = vec3( 1.0f, -1.0f, -1.0f);
    verts[7].position = vec3(-1.0f, -1.0f, -1.0f);
 
    const float third = 1.0f / 3.0f;
    const float two_thirds = 2.0f * third;

    verts[0].texture = vec2(third, 0.0f);
    verts[1].texture = vec2(two_thirds, 0.0f);
    verts[2].texture = vec2(two_thirds, third);
    verts[3].texture = vec2(third, third);
    verts[4].texture = vec2(0.0f, two_thirds);
    verts[5].texture = vec2(0.0f, 1.0f);
    verts[6].texture = vec2(two_thirds, two_thirds);
    verts[7].texture = vec2(third, two_thirds);
     
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
     
    loadVertexArray(&verts[0], 8);
    loadElements(&indices[0], index_length);
}

/* Load unit quad geometry for testing
 */
void Geometry::genTestQuad()
{
    using glm::vec3;
    using glm::vec2;

    // Only 4 verts in a quad
    Vertex verts[4];

    verts[0].position = vec3(-1.0f, -1.0f, 0.0f);
    verts[1].position = vec3( 1.0f, -1.0f, 0.0f);
    verts[2].position = vec3( 1.0f,  1.0f, 0.0f);
    verts[3].position = vec3(-1.0f,  1.0f, 0.0f);

    // Texture coordinates happen to be the first two components 
    // of the position
    for(int i = 0; i < 4; i++)
    {
        verts[i].texture = vec2(verts[i].position);
    }

    // Configure indicies for GL_TRIANGLES
    int index_length = 2 * 3;
    short indices[] =
    {
        0, 1, 2,
        3, 0, 2
    };

    loadVertexArray(&verts[0], 4);
    loadElements(&indices[0], index_length);
}

/* This function generates a series of vertices corresponding to the centers
 * of hexagonal tiles. This function is appropriate to use with a geometry
 * shader to create a tiled plane
 */
void Geometry::genHexGrid(int dimension, double scale)
{
    using glm::vec3;
    using glm::vec2;

    // Create an array of hexagonal tiles
    int length = dimension * dimension;
    Vertex* verts = new Vertex[length];

    // Calculate tile parameters
    double a = scale;
    double h = 2.0 * sqrt(3.0/4.0 *a);

    // Iterate over the tiles
    int row, col, x, y; 
    double z = a;
    for(int i = 0; i < length; i++)
    {
        row = i / dimension;
        col = i % dimension;

        // Find the coordinates of the center point
        x = ((1 + row % 2) + col * 2) * h;
        y = (1 + row * 1) * a;

        // The z-component is a vertical offset value. Default creates a
        // flat surface out of the tops of tiles
        z = z;

        // Write tile position to buffer, texture coords used to pass tile
        // parameters
        verts[i].position = vec3((float)x, (float)y, (float)z);
        verts[i].texture = vec2((float)a, (float)h);
    }

    // Load data then free buffer
    loadVertexArray(&verts[0], length);
    index_count = length;

    delete[] verts;
}

