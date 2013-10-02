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
        virtual void bind() = 0;

    protected:
        int tex_unit_start;
    };

    /* This class covers the common case for an Effect 
     */
    class GenericEffect : public Effect
    {
    public:
        GenericEffect(int, ProgramPtr, VecTexDataTuplePtr, VecUniformPairPtr);
        virtual void bind();

    private:
        ProgramPtr program;
        VecTexDataTuplePtr textures;
        VecUniformPairPtr uniforms;
    };

}
#endif // EFFECT_H
