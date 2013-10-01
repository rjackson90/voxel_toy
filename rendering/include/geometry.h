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

    class Geometry
    {
    public:
        Geometry();
        ~Geometry();

        void genTestCube();
        void genTestQuad();

        void draw() const;
        void setDrawMode(GLenum);
        void loadData(Vertex*, int, short*, int);

    private:
        const static unsigned char VERTEX_BUFFER_INDEX = 0;
        const static unsigned char ELEMENT_BUFFER_INDEX = 1;

        GLenum draw_mode;
        GLint index_count;
        GLuint buffers[2];
        GLuint vao;
    };
}

#endif // GEOMETRY_H
