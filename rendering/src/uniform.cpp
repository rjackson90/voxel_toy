#include "uniform.h"

using namespace Rendering;

Uniform::Uniform(const BlockDefinition &blk, GLuint program, GLuint bind_loc) : 
    block(blk), binding(bind_loc)
{
    index = glGetUniformBlockIndex(program, block.name);
    glUniformBlockBinding(program, index, binding);

    glGenBuffers(1, &buffer);

    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, binding, buffer);
}

Uniform::~Uniform()
{
    glDeleteBuffers(1, &buffer);
}

void Uniform::update(const Subsystems& systems)
{
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    block.getData(systems);
}




