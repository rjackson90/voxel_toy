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
    struct UniformBuffer;
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
            const std::vector<std::shared_ptr<Rendering::Effect>>&,
            const std::vector<std::shared_ptr<Rendering::UniformBuffer>>&);
    glm::mat4 getCameraMatrix() { 
        return glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -5.0f)); 
    }
    glm::mat4 getPerspectiveMatrix() { return perspective; }

    std::vector<std::shared_ptr<Rendering::UniformBuffer>> frame_uniforms;
    private:
    struct RenderNode : Node
    {
        RenderNode(const Rendering::Geometry&, 
                const std::vector<std::shared_ptr<Rendering::Effect>>&,
                const std::vector<std::shared_ptr<Rendering::UniformBuffer>>&);
        const Rendering::Geometry& mesh;
        const std::vector<std::shared_ptr<Rendering::Effect>> effects;
        const std::vector<std::shared_ptr<Rendering::UniformBuffer>> object_uniforms;
    };

    Rendering::GLWindow window;
    glm::mat4 perspective;
    std::unordered_map<int, RenderNode> nodes;
};

#endif /* RENDER_SYSTEM_H */
