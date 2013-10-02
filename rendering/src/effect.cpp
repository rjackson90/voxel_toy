#include "effect.h"

using namespace Rendering;

Effect::Effect(int tex_idx) : tex_unit_start(tex_idx)
{
}

GenericEffect::GenericEffect(int tex_begin, 
        ProgramPtr p_ptr, VecTexDataTuplePtr vtdp, VecUniformPairPtr vup
        ) : Effect(tex_begin), program(p_ptr), textures(vtdp), uniforms(vup)
{
    // Program object ID
    GLuint p = program->getProgramObj();

    // Bind the uniform blocks and buffers to common bind points
    for(const UniformPairPtr& pair : uniforms)
    {
        pair->first->bind(p, pair->second.c_str());
    }

    // Get the location of texture sampler uniforms
    for(const TexDataTuplePtr& data : textures)
    {
        std::get<3>(*data) = glGetUniformLocation(p, std::get<2>(*data).c_str());
    }
}

void GenericEffect::bind()
{
    // Bind program
    program->bind();

    // Bind textures and samplers to texture units
    for(unsigned int i = 0; i < textures.size(); i++)
    {
        TexDataTuple data = *(textures[i]);
        glActiveTexture(GL_TEXTURE0 + tex_unit_start + i);
        std::get<0>(data)->bind();
        std::get<1>(data)->bind(tex_unit_start + i);
        glUniform1i(std::get<3>(data), tex_unit_start + i);
    }
}
