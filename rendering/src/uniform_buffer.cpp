#include "uniform_buffer.h"

using namespace Rendering;

// Implementation of UniformBuffer

UniformBuffer::UniformBuffer()
{
    glGenBuffers(1, &buffer);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &buffer);
}

void UniformBuffer::bind(GLuint bind_point) const
{
    glBindBufferBase(GL_UNIFORM_BUFFER, bind_point, buffer);
}

void UniformBuffer::setBlock(const BlockDefinition &block)
{
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    block.updateBuffer();
}


