#include "effect.h"

using namespace Rendering;

Effect::Effect(int tex_idx) : tex_unit_start(tex_idx)
{
}

EffectPtr GenericEffect::GenericEffectFromConfig(
        const Core::ConfigParser &parser, const std::string &section)
{
    EffectPtr eptr;
    int bindpoint = 0;
    int tex_begin = 0;

    try
    {
        // Program
        auto program = Program::ProgramFromConfig(parser, 
                parser.get("program",section));

        // Color texture
        auto color = Texture::TextureFromConfig(parser,
                parser.get("colorTex", section));
        auto color_sample = Sampler::SamplerFromConfig(parser, 
                parser.get("colorTexSampler", section));
        auto color_pair = std::make_shared<TexDataTuple>(
                color, color_sample, "texColor", -1);

        // Normal texture
        auto normal = Texture::TextureFromConfig(parser,
                parser.get("normalTex", section));
        auto normal_sample = Sampler::SamplerFromConfig(parser,
                parser.get("normalTexSampler", section));
        auto normal_pair = std::make_shared<TexDataTuple>(
                normal, normal_sample, "texNormal", -1);

        // Combined texture data
        VecTexDataTuplePtr textures{color_pair, normal_pair};

        // Transform
        TransformBlock tb = TransformBlock::TransformFromConfig(parser, 
                parser.get("transform",section));
        auto tbuffer = std::make_shared<UniformBuffer>(tb, bindpoint++);
        auto transform = std::make_shared<UniformPair>(tbuffer, "TransformBlock");

        // Point Light
        PointLight pl = PointLight::PointLightFromConfig(parser,
                parser.get("pointLight", section));
        auto plbuffer = std::make_shared<UniformBuffer>(pl, bindpoint++);
        auto pointlight = std::make_shared<UniformPair>(plbuffer, "PointLight");

        // Material
        Material mat = Material::MaterialFromConfig(parser,
                parser.get("material", section));
        auto matbuffer = std::make_shared<UniformBuffer>(mat, bindpoint++);
        auto material = std::make_shared<UniformPair>(matbuffer, "Material");

        // Combined uniform data
        VecUniformPairPtr uniforms{transform, pointlight, material};

        // Effect assembly
        eptr = std::make_shared<GenericEffect>(tex_begin, program, textures, uniforms);
    }
    catch(const std::exception &ex)
    {
        std::cerr << "Error creating generic effect: " << ex.what() << std::endl;
    }

    return eptr;
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
