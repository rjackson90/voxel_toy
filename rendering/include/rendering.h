/* This file forward declares classes in the project, as well as various 
 * combinations of pointers-to-type and containers-of-pointer-to-type
 */
#ifndef RENDERING_H
#define RENDERING_H

namespace Rendering
{
    // Effects
    class Effect;
    typedef std::shared_ptr<Effect> EffectPtr;
    typedef std::vector<EffectPtr> VecEffectPtr;

    // Shader programs
    class Program;
    typedef std::shared_ptr<Program> ProgramPtr;

    // Textures
    class Texture;
    typedef std::shared_ptr<Texture> TexturePtr;

    // Samplers
    class Sampler;
    typedef std::shared_ptr<Sampler> SamplerPtr;

    // Uniform buffers
    class UniformBuffer;
    typedef std::shared_ptr<UniformBuffer> UniformPtr;
    typedef std::vector<UniformPtr> VecUniformPtr;

    // Geometry
    class Geometry;
    typedef std::shared_ptr<Geometry> GeometryPtr;

    // GLWindow
    class GLWindow;
    typedef std::shared_ptr<GLWindow> GLWindowPtr;
    typedef std::unique_ptr<GLWindow> GLWindowPtrUnique;

    // Texturing data tuple. This tuple contains a texture, its sampler, as
    // well as the string identifier of the shader's sampler object
    typedef std::tuple<TexturePtr, SamplerPtr, std::string, int> TexDataTuple;
    typedef std::shared_ptr<TexDataTuple> TexDataTuplePtr;
    typedef std::vector<TexDataTuplePtr> VecTexDataTuplePtr;

    // Uniform data pair. This pair contains a uniform buffer as well as the
    // associated shader identifier
    typedef std::pair<UniformPtr, std::string> UniformPair;
    typedef std::shared_ptr<UniformPair> UniformPairPtr;
    typedef std::vector<UniformPairPtr> VecUniformPairPtr;
}


#endif // RENDERING_H
