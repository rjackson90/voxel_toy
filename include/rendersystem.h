#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <unordered_map>

#include "dispatch.h"
#include "system.h"
#include "glwindow.h"
#include "meshes.h"

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
    void addNode(int, Mesh*);

    private:
    struct RenderNode : Node
    {
        Mesh* mesh = nullptr;
    };

    glm::mat4 perspective;
    GLWindow window;
    unordered_map<int, RenderNode> nodes;
};

#endif /* RENDER_SYSTEM_H */
