#include "effect.h"

using namespace Rendering;

PhongShading::PhongShading(GLuint& tex_unit_begin, GLuint& uniform_binding_begin, 
        const Program& prog, 
        const Texture& col, const Texture& norm,
        const Sampler& linear, const Sampler& near,
        const UniformBuffer& trans, const UniformBuffer& light, const UniformBuffer& mat) :
    Effect(tex_unit_begin, 2, uniform_binding_begin, 3),
    program(prog), 
    color(col), normal(norm), 
    linear_blend(linear), nearest(near),
    transform(trans), point_light(light), material(mat)
{
    // Bind textures and samplers to texture units
    glActiveTexture(tex_unit_start);
    color.bind();
    linear_blend.bind(tex_unit_start);

    glActiveTexture(tex_unit_start + 1);
    normal.bind();
    nearest.bind(tex_unit_start + 1);

    // Get the block index for all uniform blocks in the program
    GLuint p = program.getProgramObj();
    GLuint transform_idx = glGetUniformBlockIndex(p, "TransformBlock");
    GLuint light_idx = glGetUniformBlockIndex(p, "PointLight");
    GLuint material_idx = glGetUniformBlockIndex(p, "Material");

    // Bind the block indices to the binding points set aside for this object
    glUniformBlockBinding(p, transform_idx, uniform_binding_start + 0);
    glUniformBlockBinding(p, light_idx, uniform_binding_start + 1);
    glUniformBlockBinding(p, material_idx, uniform_binding_start + 2);

    // Bind the uniform buffers to the appropriate binding point
    transform.bind(transform_idx);
    point_light.bind(light_idx);
    material.bind(material_idx);
}

Effect::Effect(GLuint &tex_idx, GLuint tex_size, GLuint &uniform_idx, GLuint uniform_size)
{
    tex_unit_start = tex_idx;
    tex_idx += tex_size;

    uniform_binding_start = uniform_idx;
    uniform_idx += uniform_size;
}
