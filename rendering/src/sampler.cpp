#include "sampler.h"

using namespace Rendering;

Sampler::Sampler(const SamplerParams &p) 
{
    sampler_obj = 0;
    glGenSamplers(1, &sampler_obj);

    setParams(p);
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

