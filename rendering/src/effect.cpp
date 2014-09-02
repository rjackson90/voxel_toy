#include "effect.h"

using namespace Rendering;


Effect::Effect(int tex_idx) : tex_unit_start(tex_idx)
{
}

EffectPtr TextureEffect::TextureEffectFromConfig(
        const Core::ConfigParser &parser, const std::string &section)
{
    EffectPtr eptr;

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

        // Transform
        auto tb = TransformBlock::TransformFromConfig(parser, 
                parser.get("transform",section));
        auto transform = std::make_shared<UniformBuffer>(tb, 0);

        // Effect assembly
        eptr = std::make_shared<TextureEffect>(program, color_pair, transform);
    }
    catch(const std::exception &ex)
    {
        std::cerr << "Error creating TextureEffect: " << ex.what() << std::endl;
    }

    return eptr;
}

TextureEffect::TextureEffect(ProgramPtr pgm, TexDataTuplePtr tex, UniformPtr txm) :
    Effect(0), program(pgm), color_tex(tex), transform(txm)
{
    GLuint p = program->getProgramObj();

    tex_loc = glGetUniformLocation(p, "texColor");

    transform->bind(p, "TransformBlock");
}

void TextureEffect::bind(const SubsystemsPtr &systems, int key)
{
    // Bind program
    program->bind();

    // Bind color texture
    glActiveTexture(GL_TEXTURE0);
    std::get<0>(*color_tex)->bind();
    std::get<1>(*color_tex)->bind(0);
    glUniform1i(tex_loc, 0);

    // Update uniform buffers, then bind them
    transform->updateContents(systems, key);

}


