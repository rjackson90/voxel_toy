#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "math_ext.h"

namespace Rendering
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec2 texture;
    };

    enum DrawMethod { ARRAYS, ELEMENTS };

    class Geometry
    {
    public:
        Geometry();
        ~Geometry();

        void setDrawMethod(DrawMethod dm) { method = dm; }

        void genTestCube();
        void genTestQuad();
        void genHexGrid(int, double);

        void draw() const;
        void setDrawMode(GLenum);
        void loadVertexArray(Vertex*, int, GLenum);
        void loadElements(short*, int, GLenum);

    private:
        const static unsigned char VERTEX_BUFFER_INDEX = 0;
        const static unsigned char ELEMENT_BUFFER_INDEX = 1;

        DrawMethod method;
        GLenum draw_mode;
        GLint index_count;
        GLuint buffers[2];
        GLuint vao;
    };
}

#endif // GEOMETRY_H
