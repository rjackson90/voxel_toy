#include "effect.h"

using namespace Rendering;

PhongShading::PhongShading(GLuint& tex_unit_begin, 
        const Program& prog, 
        const Texture& col, const Texture& norm,
        const Sampler& color_samp, const Sampler& norm_samp,
        std::shared_ptr<UniformBuffer> trans, 
        std::shared_ptr<UniformBuffer> light, 
        std::shared_ptr<UniformBuffer> mat
        ) : Effect(tex_unit_begin), 
    program(prog), 
    color(col), normal(norm), 
    color_sampler(color_samp), normal_sampler(norm_samp),
    transform(trans), point_light(light), material(mat)
{
    // Id of program object for this effect
    GLuint p = program.getProgramObj();

    // Get the block index for all uniform blocks in the program
    GLuint transform_idx = glGetUniformBlockIndex(p, "TransformBlock");
    GLuint light_idx = glGetUniformBlockIndex(p, "PointLight");
    GLuint material_idx = glGetUniformBlockIndex(p, "Material");

    // Get the uniform location for the textures in the program
    color_samplerID = glGetUniformLocation(p, "texColor");
    normal_samplerID = glGetUniformLocation(p, "texNormal");

    // Bind uniform blocks to binding points
    glUniformBlockBinding(p, transform_idx, transform->getBindPoint());
    glUniformBlockBinding(p, light_idx, point_light->getBindPoint());
    glUniformBlockBinding(p, material_idx, material->getBindPoint());
}

void PhongShading::bind()
{
    // Bind program and get its object ID
    program.bind();

    // Bind textures and samplers to texture units
    glActiveTexture(tex_unit_start + 0);
    color.bind();
    glUniform1i(color_samplerID, 0);
    color_sampler.bind(0);

    glActiveTexture(tex_unit_start + 1);
    normal.bind();
    glUniform1i(normal_samplerID, 1);
    normal_sampler.bind(1);
}

Effect::Effect(GLuint tex_idx) : tex_unit_start(tex_idx)
{
}
