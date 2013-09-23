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
        Effect(GLuint);
        virtual void bind() = 0;

    protected:
        GLuint tex_unit_start;
    };
    
    /* This class is an implementation of an Effect. It implements simple Phong shading, with
     * support for a single point light
     */
    class PhongShading : public Effect
    {
    public:
        PhongShading(GLuint&,
                const Program&, 
                const Texture&, const Texture&, 
                const Sampler&, const Sampler&,
                std::shared_ptr<UniformBuffer>, 
                std::shared_ptr<UniformBuffer>, 
                std::shared_ptr<UniformBuffer>
                );
        virtual void bind();

    private:
        const Program &program;
        
        const Texture &color;
        const Texture &normal;

        const Sampler &color_sampler;
        const Sampler &normal_sampler;

        std::shared_ptr<UniformBuffer> transform;
        std::shared_ptr<UniformBuffer> point_light;
        std::shared_ptr<UniformBuffer> material;

        GLuint color_samplerID, normal_samplerID;
    };
}
#endif // EFFECT_H
