#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include "glwindow.h"
#include "meshes.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stack>

class GLRenderer
{
    public:

    GLRenderer(int, int, const char*);
    GLRenderer();
    ~GLRenderer();

    private:

    GLWindow window;
    stack<glm::mat4> space;

};
#endif
