#include "sampler.h"

using namespace Rendering;

GLint SamplerParams::fromString(const std::string &str)
{
    // Min/Mag filters
    if(str.compare("GL_NEAREST") == 0) return GL_NEAREST;
    if(str.compare("GL_LINEAR") == 0) return GL_LINEAR;
    if(str.compare("GL_NEAREST_MIPMAP_NEAREST") == 0) return GL_NEAREST_MIPMAP_NEAREST;
    if(str.compare("GL_LINEAR_MIPMAP_NEAREST") == 0) return GL_LINEAR_MIPMAP_NEAREST;
    if(str.compare("GL_NEAREST_MIPMAP_LINEAR") == 0) return GL_NEAREST_MIPMAP_LINEAR;
    if(str.compare("GL_LINEAR_MIPMAP_LINEAR") == 0) return GL_LINEAR_MIPMAP_LINEAR;
    
    // Edge value sampling
    if(str.compare("GL_REPEAT") == 0) return GL_REPEAT;
    if(str.compare("GL_MIRRORED_REPEAT") == 0) return GL_MIRRORED_REPEAT;
    if(str.compare("GL_CLAMP_TO_EDGE") == 0) return GL_CLAMP_TO_EDGE;
    if(str.compare("GL_CLAMP_TO_BORDER") == 0) return GL_CLAMP_TO_BORDER;
    if(str.compare("GL_MIRROR_CLAMP_TO_EDGE") == 0) return GL_MIRROR_CLAMP_TO_EDGE;

    // Compare mode/function
    if(str.compare("GL_NONE") == 0) return GL_NONE;
    if(str.compare("GL_NEVER") == 0) return GL_NEVER;
    if(str.compare("GL_ALWAYS") == 0) return GL_ALWAYS;
    if(str.compare("GL_LESS") == 0) return GL_LESS;
    if(str.compare("GL_LEQUAL") == 0) return GL_LEQUAL;
    if(str.compare("GL_EQUAL") == 0) return GL_EQUAL;
    if(str.compare("GL_GEQUAL") == 0) return GL_GEQUAL;
    if(str.compare("GL_GREATER") == 0) return GL_GREATER;

    return -1;
}

Sampler::Sampler(const SamplerParams &p) 
{
    sampler_obj = 0;
    glGenSamplers(1, &sampler_obj);

    setParams(p);
}

SamplerPtr Sampler::SamplerFromConfig(const Core::ConfigParser &parser, const std::string &section)
{
    SamplerParams params;
    auto sampler = std::make_shared<Sampler>(params);
    try
    {
        // Get sampler parameters from config, populate new SamplerParams
        params.min_filter = params.fromString(parser.get("min_filter", section));
        params.mag_filter = params.fromString(parser.get("mag_filter", section));
        //std::cout << "Filters: " << params.min_filter << " " 
        //          << params.mag_filter << std::endl;

        params.min_lod = stof(parser.get("min_lod", section));
        params.max_lod = stof(parser.get("max_lod", section));
        //std::cout << "LoD: " << params.min_lod << " " << params.max_lod << std::endl;

        params.wrap_s = params.fromString(parser.get("wrap_s", section));
        params.wrap_t = params.fromString(parser.get("wrap_t", section));
        params.wrap_r = params.fromString(parser.get("wrap_r", section));
        //std::cout << "wrap: " << params.wrap_s << " " << params.wrap_t 
        //          << " " << params.wrap_r << std::endl;

        float data[4];
        std::stringstream stream(parser.get("border_color", section));
        stream >> data[0] >> data[1] >> data[2] >> data[3];
        params.border_color = glm::make_vec4(data);
        //std::cout << "Border color: " << data[0] 
        //          << " " << data[1]
        //          << " " << data[2] 
        //          << " " << data[3]
        //          << std::endl;

        params.compare_mode = params.fromString(parser.get("compare_mode", section));
        params.compare_func = params.fromString(parser.get("compare_func", section));
        //std::cout << "compare: " << params.compare_mode << " " << params.compare_func
        //    << std::endl;

        // Assign updated sampler params
        sampler->setParams(params);
    }
    catch(const std::exception &ex)
    {
        std::cerr << "Error creating sampler: " << ex.what() << std::endl;
    }

    return sampler;
}

void Sampler::bind(GLuint unit) const
{
    glBindSampler(unit, sampler_obj);
}

const SamplerParams& Sampler::getParams() const
{
    return params;
}

void Sampler::setParams(const SamplerParams &p)
{
    params = p;

    glSamplerParameteri(sampler_obj, GL_TEXTURE_MIN_FILTER, params.min_filter);
    glSamplerParameteri(sampler_obj, GL_TEXTURE_MAG_FILTER, params.mag_filter);
    
    glSamplerParameterf(sampler_obj, GL_TEXTURE_MIN_LOD, params.min_lod);
    glSamplerParameterf(sampler_obj, GL_TEXTURE_MAX_LOD, params.max_lod);

    glSamplerParameteri(sampler_obj, GL_TEXTURE_WRAP_S, params.wrap_s);
    glSamplerParameteri(sampler_obj, GL_TEXTURE_WRAP_T, params.wrap_t);
    glSamplerParameteri(sampler_obj, GL_TEXTURE_WRAP_R, params.wrap_r);

    glSamplerParameterfv(sampler_obj, GL_TEXTURE_BORDER_COLOR, glm::value_ptr(params.border_color));

    glSamplerParameteri(sampler_obj, GL_TEXTURE_COMPARE_MODE, params.compare_mode);
    glSamplerParameteri(sampler_obj, GL_TEXTURE_COMPARE_FUNC, params.compare_func);
}

