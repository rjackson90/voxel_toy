#include "rendersystem.h"

using namespace Rendering;
using std::pair;
using std::vector;

/* The constuctor initializes GLEW, a critical library which makes loading
 * function pointers for OpenGL functionality REALLY EASY
 */
RenderSystem::RenderSystem(const GLWindowPtr &win) : window(win), frame_uniforms()
{
    // Orthographic projection
    std::pair<int, int> dimensions = window->getDimensions();
    float aspect = (float) dimensions.first / (float) dimensions.second;

    float left = -5.0f, right = 5.0f;
    perspective = glm::ortho(left, right, left / aspect, right / aspect, -100.0f, 100.0f);
    
    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Print info about the GL context
    std::cout << "GL_VENDOR:\t"   << glGetString(GL_VENDOR) << std::endl <<
                 "GL_RENDERER:\t" << glGetString(GL_RENDERER) << std::endl <<
                 "GL_VERSION:\t"  << glGetString(GL_VERSION) << std::endl <<
                 "GL_SHADING_LANGUAGE_VERSION:\t" << glGetString(GL_SHADING_LANGUAGE_VERSION) << 
                 std::endl;
}

/* This function adds another node to the batch
 */
void RenderSystem::addNode(int key, 
        GeometryPtr geo, 
        VecEffectPtr effect_list, 
        VecUniformPairPtr uniform_list)
{
    auto newNode = std::make_shared<RenderNode>(geo, effect_list, uniform_list);
    newNode->key = key;
    nodes.insert({{key, newNode}});
}

/* This function creates a new Node based on information in the provided config file.
 */
void addNodeFromConfig(const Core::ConfigParser &parser)
{   
    // Swallow exceptions
    try
    {
        // The Node's ID is in the Meta section
        int nodeID = stoi(parser.get("nodeID", "Meta"));
        nodeID = nodeID + 1 - 1; // Prevent "unused variable" warning

        // Geometry
        std::string geom_section = parser.get("geom_section", "RenderNode");
        GeometryPtr geo = Geometry::GeometryFromConfig(parser, geom_section);
    }
    catch(const std::exception &ex)
    {
        std::cerr << "Error parsing config file " << ex.what()
                  << std::endl;
    }
}

/* RenderSystem's tick simply draws the whole scene, calling upon the PhysicsSystem 
 * to provide correct world coordinates.
 */
void RenderSystem::tick(const SubsystemsPtr &systems, 
        __attribute__((unused)) const double dt)
{
    // Clear the frame buffer, depth buffer, and stencil buffer
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    /*
    // Update uniforms which are at frame scope
    for(const auto& uniform : frame_uniforms)
    {
        // Pass invalid key to cause errors for buffers which aren't actually frame-scope
        uniform->first->updateContents(systems, -1);
    }
    */

    // Iterate over the RenderNodes
    for(auto i = nodes.begin(); i != nodes.end(); ++i)
    {
        /*
        // Update node-scope uniforms
        for(const auto& uniform : i->second->object_uniforms)
        {
            uniform->first->updateContents(systems, i->first);
        }
        */

        // For every effect in the list: bind it then draw the geometry
        for(const auto &effect : i->second->effects)
        {
            effect->bind(systems, i->first);
            i->second->mesh->draw();
        }
    }

    // Swap front and rear buffers.
    window->swap();
}

RenderSystem::RenderNode::RenderNode(
        GeometryPtr geo, 
        VecEffectPtr effect_stack, 
        VecUniformPairPtr buffers) :
    mesh(geo), effects(effect_stack), object_uniforms(buffers)
{
}
