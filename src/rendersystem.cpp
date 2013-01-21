#include "rendersystem.h"

RenderSystem::RenderSystem( int width, int height, string title) 
    : window(GLWindow(width, height, title))
{
    // Run GLEW init here, after context creation and before just about anything else
    cout << "Getting OpenGL bindings...";
    window.makeCurrent();
    GLenum err = glewInit();
    if ( err == GLEW_OK )
    {
        cout << " OK!" << endl;
    } else {
        cout << " FAIL: " << glewGetErrorString(err) << endl;
    }

    perspective = glm::perspective(90.0,                            // FOV
                                   (double)width / (double)height,  // Viewport
                                   0.1,                             // z-min
                                   100.0);                          // z-max
    glEnable(GL_DEPTH_TEST);
}

void RenderSystem::addNode(int key, Mesh data)
{
    RenderNode newNode;
    newNode.key = key;
    newNode.mesh = data;

    nodes.push_back(newNode);
}

void RenderSystem::tick(__attribute__((unused)) float dt)
{
    // Clear the frame buffer, depth buffer, and stencil buffer
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for(std::list<RenderNode>::iterator i=nodes.begin(); i != nodes.end(); ++i)
    {
        /* In reality the coordinate matrix would be pulled from the physics simulation
         * and the camera matrix would be pulled from somewhere else... input system?
         */
        glm::mat4 coords = glm::mat4(1.0);
        glm::mat4 camera = glm::translate( glm::mat4(1.0), glm::vec3(0.0, 0.0, -5.0));
        i->mesh.draw(perspective * camera * coords);
    }

    // Swap front and rear buffers. In other words, display the just-rendered scene
    window.swap();
}
