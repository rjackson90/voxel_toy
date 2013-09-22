#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

// System headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <unordered_map>
#include <vector>

// Core headers
#include "dispatch.h"
#include "system.h"

// Rendering headers
#include "effect.h"
#include "geometry.h"
#include "error.h"
#include "glwindow.h"

// Forward declarations
namespace Rendering
{
    class Geometry;
    class Effect;
    struct BlockDefinition;
    class GLWindow;
}
struct Subsystems;

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
    RenderSystem(int, int, const std::string&, const std::vector<std::pair<SDL_GLattr, int>>&);
    virtual void tick(const Subsystems &, const double)override;
    void addNode(int, const Rendering::Geometry&, 
            std::vector<std::shared_ptr<Rendering::Effect>>,
            std::vector<std::shared_ptr<Rendering::BlockDefinition>>);
    glm::mat4 getCameraMatrix() { return glm::mat4(1.0f); }
    glm::mat4 getPerspectiveMatrix() { return perspective; }

    std::vector<std::shared_ptr<Rendering::BlockDefinition>> frame_uniforms;
    private:
    struct RenderNode : Node
    {
        RenderNode(const Rendering::Geometry&, 
                std::vector<std::shared_ptr<Rendering::Effect>> &,
                std::vector<std::shared_ptr<Rendering::BlockDefinition>>&);
        const Rendering::Geometry& mesh;
        std::vector<std::shared_ptr<Rendering::Effect>> effects;
        std::vector<std::shared_ptr<Rendering::BlockDefinition>> object_uniforms;
    };

    Rendering::GLWindow window;
    glm::mat4 perspective;
    std::unordered_map<int, RenderNode> nodes;
};

#endif /* RENDER_SYSTEM_H */
