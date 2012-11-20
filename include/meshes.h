#ifndef MESHES_H
#define MESHES_H

#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <fstream>

const float PI = 4.0f * atan(1);
inline float RADIANS(float x){return (x * PI)/180.0f;}
inline float DEGREES(float x){return (x * 180.0f)/PI;}

const int VERTEX_BUFFER_INDEX = 0;
const int ELEMENT_BUFFER_INDEX = 1;

class Vertex
{
    public:

    Vertex(const glm::vec3&, const glm::vec3&, const glm::vec3&);

    glm::vec3 position; 
    glm::vec3 normal;
    glm::vec3 color;
};

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

void test_cube(Mesh &);

void test_latlong_sphere(Mesh &);

void test_ico_sphere(Mesh &);

void test_tetrahedron(Mesh &);

void test_cone(Mesh &);

#endif
