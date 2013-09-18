#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <GL/glew.h>

#include "uniform_block.h"

namespace Rendering
{
    /* This class describes a uniform buffer. It holds a reference to a BlockDefinition 
     * (or more likely, a struct that inherits from BlockDefinition) which represents the uniform
     * block inside a shader program. This class manages the buffer backing the uniform block
     */
    class UniformBuffer
    {
    public:
        UniformBuffer();
        ~UniformBuffer();
        void bind(GLuint) const;
        void setBlock(const BlockDefinition&);
    
    private:
        GLuint buffer;
    };
}
#endif // UNIFORM_BUFFER_H
