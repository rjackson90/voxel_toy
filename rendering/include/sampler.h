#ifndef SAMPLER_H
#define SAMPLER_H

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "config.h"
#include "rendering.h"

namespace Rendering
{
    struct SamplerParams
    {
        GLint min_filter = GL_NEAREST_MIPMAP_LINEAR;
        GLint mag_filter = GL_LINEAR;

        GLfloat min_lod = -1000.0f;
        GLfloat max_lod =  1000.0f;

        GLint wrap_s = GL_REPEAT;
        GLint wrap_t = GL_REPEAT;
        GLint wrap_r = GL_REPEAT;

        glm::vec4 border_color = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);

        GLint compare_mode = GL_NONE;
        GLint compare_func = GL_LEQUAL;

        GLint fromString(const std::string &);
    };

    class Sampler
    {
        public:
        Sampler(const SamplerParams&);

        static SamplerPtr SamplerFromConfig(const Core::ConfigParser &, const std::string &);

        void bind(GLuint) const;
        const SamplerParams& getParams() const;
        void setParams(const SamplerParams&);

        private:
        GLuint sampler_obj;
        SamplerParams params;

    };

}

#endif // SAMPLER_H
