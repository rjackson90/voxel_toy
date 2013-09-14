#include "rendersystem.h"

/* The constuctor initializes GLEW, a critical library which makes loading
 * function pointers for OpenGL functionality REALLY EASY
 */
RenderSystem::RenderSystem(int width, int height, string title) 
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
    
    /*
    // Perspective projection
    perspective = glm::perspective(40.0,                            // FOV
                                   (double)width / (double)height,  // Viewport
                                   1.0,                             // z-min
                                   1000.0);                         // z-max
    */

    // Orthographic projection
    float aspect = (float) width / (float) height;
    perspective = glm::ortho(-2.5f, 2.5f, -2.5f / aspect, 2.5f / aspect, -100.0f, 100.0f);
    
    glEnable(GL_DEPTH_TEST);
}

/* This function adds another node to the "Scene Graph"
 */
void RenderSystem::addNode(int key)
{
    RenderNode newNode;
    newNode.key = key;

    nodes.insert({{key, newNode}});
}

/* RenderSystem's tick simply draws the whole scene, calling upon the PhysicsSystem 
 * to provide correct world coordinates.
 */
void RenderSystem::tick(const Subsystems &systems, __attribute__((unused)) const double dt)
{
    // Clear the frame buffer, depth buffer, and stencil buffer
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    for(auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        /* In reality the coordinate matrix would be pulled from the physics simulation
         * and the camera matrix would be pulled from somewhere else... input system?
         */
        __attribute__((unused)) glm::mat4 coords = systems.physics->getWorldCoords(i->second.key);
        __attribute__((unused)) glm::mat4 camera = glm::translate( glm::mat4(1.0), 
                glm::vec3(0.0, 0.0, 0.0));
        // i->second.mesh->draw(perspective, camera, coords);
    }

    // Swap front and rear buffers. In other words, display the just-rendered scene
    window.swap();
}
