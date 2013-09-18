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
#include "glwindow.h"
#include "effect.h"
#include "geometry.h"

// Forward declarations
namespace Rendering
{
    class Geometry;
    class Effect;
    struct BlockDefinition;
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
    RenderSystem( int, int, string);
    virtual void tick(const Subsystems &, const double)override;
    void addNode(int, const Rendering::Geometry&, 
            std::vector<std::shared_ptr<Rendering::Effect>>,
            std::vector<std::shared_ptr<Rendering::BlockDefinition>>);

    glm::mat4 getCameraMatrix() { return glm::mat4(1.0f); }
    glm::mat4 getPerspectiveMatrix() { return perspective; }

    private:
    struct RenderNode : Node
    {
        Rendering::Geometry mesh;
        std::vector<std::shared_ptr<Rendering::Effect>> effects;
        std::vector<std::shared_ptr<Rendering::BlockDefinition>> uniform_blocks;
    };

    glm::mat4 perspective;
    GLWindow window;
    unordered_map<int, RenderNode> nodes;
};

#endif /* RENDER_SYSTEM_H */
