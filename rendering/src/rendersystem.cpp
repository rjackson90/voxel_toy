#include "rendersystem.h"

/* The constuctor initializes GLEW, a critical library which makes loading
 * function pointers for OpenGL functionality REALLY EASY
 */
RenderSystem::RenderSystem(int width, int height, string title) 
    : frame_uniforms(), window(GLWindow(width, height, title))
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
    
    // Orthographic projection
    float aspect = (float) width / (float) height;
    perspective = glm::ortho(-2.5f, 2.5f, -2.5f / aspect, 2.5f / aspect, -100.0f, 100.0f);
    
    glEnable(GL_DEPTH_TEST);
}

/* This function adds another node to the batch
 */
void RenderSystem::addNode(int key, 
        const Rendering::Geometry& geo, 
        std::vector<std::shared_ptr<Rendering::Effect>> effect_list,
        std::vector<std::shared_ptr<Rendering::BlockDefinition>> block_list)
{
    RenderNode newNode;
    newNode.mesh = geo;
    newNode.effects = effect_list;
    newNode.object_uniforms = block_list;

    nodes.insert({{key, newNode}});
}

/* RenderSystem's tick simply draws the whole scene, calling upon the PhysicsSystem 
 * to provide correct world coordinates.
 */
void RenderSystem::tick(__attribute__((unused)) const Subsystems &systems, 
        __attribute__((unused)) const double dt)
{
    // Clear the frame buffer, depth buffer, and stencil buffer
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Update uniforms which are at frame scope
    for(const auto &uniform : frame_uniforms)
    {
        // Pass invalid key to cause errors for buffers which aren't actually frame-scope
        uniform->getData(systems, -1);
        uniform->updateBuffer();
    }

    // Iterate over the RenderNodes
    for(auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        // Update node-scope uniforms
        for(const auto &uniform : i->second.object_uniforms)
        {
            uniform->getData(systems, i->first);
            uniform->updateBuffer();
        }

        // For every effect in the list: bind it then draw the geometry
        for(const auto &effect : i->second.effects)
        {
            effect->bind();
            i->second.mesh.draw();
        }
    }

    // Swap front and rear buffers. In other words, display the just-rendered scene
    window.swap();
}
