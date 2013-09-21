#include "effect.h"

using namespace Rendering;

PhongShading::PhongShading(GLuint& tex_unit_begin, GLuint& uniform_binding_begin, 
        const Program& prog, 
        const Texture& col, const Texture& norm,
        const Sampler& color_samp, const Sampler& norm_samp,
        const UniformBuffer& trans, const UniformBuffer& light, const UniformBuffer& mat) :
    Effect(tex_unit_begin, 2, uniform_binding_begin, 3),
    program(prog), 
    color(col), normal(norm), 
    color_sampler(color_samp), normal_sampler(norm_samp),
    transform(trans), point_light(light), material(mat)
{
    // Store the ID of the associated program object
    p = program.getProgramObj();

    // Get the block index for all uniform blocks in the program
    transform_idx = glGetUniformBlockIndex(p, "TransformBlock");
    light_idx = glGetUniformBlockIndex(p, "PointLight");
    material_idx = glGetUniformBlockIndex(p, "Material");
}

void PhongShading::bind()
{
    // Bind program and get its object ID
    program.bind();

    // Bind textures and samplers to texture units
    glActiveTexture(tex_unit_start + 0);
    color.bind();
    color_sampler.bind(0);

    glActiveTexture(tex_unit_start + 1);
    normal.bind();
    normal_sampler.bind(1);

    // Bind program uniform blocks and buffers to binding points
    glUniformBlockBinding(p, transform_idx, uniform_binding_start);
    transform.bind(uniform_binding_start);

    glUniformBlockBinding(p, light_idx, uniform_binding_start + 1);
    point_light.bind(uniform_binding_start + 1);

    glUniformBlockBinding(p, material_idx, uniform_binding_start + 2);
    material.bind(uniform_binding_start + 2);
}

Effect::Effect(GLuint &tex_idx, GLuint tex_size, GLuint &uniform_idx, GLuint uniform_size)
{
    tex_unit_start = tex_idx;
    tex_idx += tex_size;

    uniform_binding_start = uniform_idx;
    uniform_idx += uniform_size;
}
