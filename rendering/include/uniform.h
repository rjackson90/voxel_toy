#ifndef UNIFORM_H
#define UNIFORM_H

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "dispatch.h"

namespace Rendering
{
    struct BlockDefinition
    {
        const GLchar* name;
        void getData(const Subsystems&);
    };

    class Uniform
    {
    public:
        Uniform(const BlockDefinition&, GLuint, GLuint);
        ~Uniform();
        void update(const Subsystems&);
    
    private:
        BlockDefinition block;
        GLuint index;
        GLuint binding;
        GLuint buffer;
    };
}
#endif // UNIFORM_H
