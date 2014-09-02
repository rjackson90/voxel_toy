#ifndef EFFECT_H
#define EFFECT_H

#include <GL/glew.h>
#include <tuple>
#include <memory>
#include <vector>

#include "rendering.h"
#include "program.h"
#include "uniform.h"
#include "texture.h"
#include "sampler.h"

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
        Effect(int);
        virtual void bind(const SubsystemsPtr &, int) = 0;

    protected:
        int tex_unit_start;
    };


    /* This class covers the common case for an Effect 
     */
    class GenericEffect : public Effect
    {
    public:
        static EffectPtr GenericEffectFromConfig(
                const Core::ConfigParser &, const std::string &);

        GenericEffect(int, ProgramPtr, VecTexDataTuplePtr, VecUniformPairPtr);
        virtual void bind(const SubsystemsPtr &, int)   override;

    private:
        ProgramPtr program;
        VecTexDataTuplePtr textures;
        VecUniformPairPtr uniforms;
    };

    /* This class describes an Effect which simply textures a single piece of geometry
     */
    class TextureEffect : public Effect
    {
    public:
        static EffectPtr TextureEffectFromConfig(
                const Core::ConfigParser &, const std::string &);

        TextureEffect(ProgramPtr, TexDataTuplePtr, UniformPtr);
        virtual void bind(const SubsystemsPtr &, int)   override;

    private:
        ProgramPtr program;
        TexDataTuplePtr color_tex;
        UniformPtr transform;

        GLint tex_loc;
    };

}
#endif // EFFECT_H
