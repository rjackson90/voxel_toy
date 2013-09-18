#ifndef EFFECT_H
#define EFFECT_H

#include <GL/glew.h>

#include "texture.h"
#include "sampler.h"
#include "program.h"
#include "uniform.h"

namespace Rendering
{
    class UniformBuffer;
}

namespace Rendering
{
    /* The effect class is not meant to be instantiated directly. It only provides the ability to
     * manage texture units and uniform buffer binding points. All "real" functionality is delegated
     * to classes which inherit Effect. Such child classes contain private members to the data they
     * require, and are responsible for ensuring such data is properly bound
     */
    class Effect
    {
    public:
        Effect(GLuint&, GLuint, GLuint&, GLuint);

    protected:
        GLuint tex_unit_start;
        GLuint uniform_binding_start;
    };
    
    /* This class is an implementation of an Effect. It implements simple Phong shading, with
     * support for a single point light
     */
    class PhongShading : public Effect
    {
    public:
        PhongShading(GLuint&, GLuint&, 
                const Program&, 
                const Texture&, const Texture&, 
                const Sampler&, const Sampler&,
                const UniformBuffer&, const UniformBuffer&, const UniformBuffer&);

    private:
        const Program &program;
        
        const Texture &color;
        const Texture &normal;

        const Sampler &linear_blend;
        const Sampler &nearest;

        const UniformBuffer &transform;
        const UniformBuffer &point_light;
        const UniformBuffer &material;
    };
}
#endif // EFFECT_H
