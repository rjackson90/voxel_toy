#ifndef REDNER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <list>

#include "system.h"
#include "glwindow.h"
#include "meshes.h"


class RenderSystem : public System
{
    public:
    RenderSystem(int, int, string);
    virtual void tick(float);
    void addNode(int, Mesh);

    private:
    struct RenderNode : Node
    {
        Mesh mesh;
    };

    glm::mat4 perspective;
    GLWindow window;
    list<RenderNode> nodes;
};

#endif /* RENDER_SYSTEM_H */
