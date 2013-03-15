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

class RenderSystem : public System
{
    public:
    RenderSystem(Subsystems *, int, int, string);
    virtual void tick(const double)override;
    void addNode(int, Mesh*);

    private:
    struct RenderNode : Node
    {
        Mesh* mesh = nullptr;
    };

    glm::mat4 perspective;
    GLWindow window;
    unordered_map<int, RenderNode> nodes;
    Subsystems *systems;
};

#endif /* RENDER_SYSTEM_H */
