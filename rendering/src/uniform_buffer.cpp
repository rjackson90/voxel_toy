#include "uniform.h"

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
    glQuitOnError();
}

void UniformBuffer::setBlock(std::shared_ptr<BlockDefinition> block)
{
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    block->updateBuffer();
    glQuitOnError();
}


