#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

// System headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>

// Core headers
#include "core.h"
#include "system.h"

// Rendering headers
#include "rendering.h"
#include "effect.h"
#include "geometry.h"
#include "uniform.h"
#include "glwindow.h"

/* The render system is rather simple.
 * Most of the heavy lifting is done in the Mesh class. 
 * This subsystem pretty much just has to keep track of all the RenderNodes,
 * and render them at the appropriate time.
 * If this engine had a "scene graph", it would be a datastructure used to hold
 * renderNodes.
 */
class RenderSystem : public System
{
    public:
    RenderSystem(int, int, const std::string&, std::vector<std::pair<SDL_GLattr, int>>);
    virtual void tick(const SubsystemsPtr &, const double)override;
    void addNode(int, Rendering::GeometryPtr, Rendering::VecEffectPtr, Rendering::VecUniformPairPtr);
    void addFrameUniform(Rendering::UniformPairPtr ptr) { frame_uniforms.push_back(ptr); }
    
    glm::mat4 getCameraMatrix() { 
        return glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)); 
    }
    glm::mat4 getPerspectiveMatrix() { return perspective; }

    private:
    struct RenderNode : Node
    {
        RenderNode(Rendering::GeometryPtr, Rendering::VecEffectPtr, Rendering::VecUniformPairPtr);

        Rendering::GeometryPtr mesh;
        Rendering::VecEffectPtr effects;
        Rendering::VecUniformPairPtr object_uniforms;
    };

    Rendering::GLWindowPtr window;
    glm::mat4 perspective;
    std::unordered_map<int, std::shared_ptr<RenderNode>> nodes;
    Rendering::VecUniformPairPtr frame_uniforms;
};

#endif /* RENDER_SYSTEM_H */
