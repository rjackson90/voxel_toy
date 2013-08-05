#ifndef MESHES_H
#define MESHES_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>

#include "math_ext.h"

const int VERTEX_BUFFER_INDEX = 0;
const int ELEMENT_BUFFER_INDEX = 1;

/* Each of the Vectors in a Vertex is a per-vertex attribute, and will be 
 * passed to the vertex shader during rendering.
 */
class Vertex
{
    public:

    Vertex(const Vector&, const Vector&, const Vector&);

    Vector position; 
    Vector normal;
    Vector color;
};

/* A Mesh object tracks all of the OpenGL state information associated with an on-screen object.
 * NOT IMPLEMENTED: Materials.
 */
class Mesh
{
    public:

    Mesh();
    void loadData(Vertex*, int, short*, int);
    void loadProgram(std::string, std::string);    
    static bool loadShaderFile(std::string, GLuint);
    void draw(glm::mat4);

    private:

    int     vertex_count;
    int     index_count;
    GLuint  buffers[2];
    GLuint  vao;
    GLuint  program;

    GLuint locMVP;
};

/* The following functions generate test shapes and store them in a Mesh.
 * Not all of them have implementations. In fact, as of writing, only test_cube
 * has a correct and valid implementation
 */
void test_cube(Mesh &);

void test_latlong_sphere(Mesh &);

void test_ico_sphere(Mesh &);

void test_tetrahedron(Mesh &);

void test_cone(Mesh &);

#endif
