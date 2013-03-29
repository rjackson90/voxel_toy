#include <GL/glew.h>
#include <iostream>
#include <signal.h>

#include "meshes.h"
#include "dispatch.h"

using namespace std;

struct Subsystems;
class Dispatch;
class RenderSystem;
class PhysicsSystem;


int main()
{
    // Create container struct and initialize subsystems
    Subsystems systems;
    RenderSystem render = RenderSystem(&systems, 1024, 768, "Really Fun Game!");
    PhysicsSystem physics = PhysicsSystem(&systems);
    Dispatch dispatch = Dispatch(&systems);

    // Update references to point to newly created objects
    systems.render = &render;
    systems.physics = &physics;
    systems.dispatch = &dispatch;

    // Use our own system signal handler
    cout << "Setting system signal handler." << endl;
    signal(SIGINT, Dispatch::signal_handler);

    // Add meshes to the render system
    cout << "Creating meshes" << endl;
    Mesh cube;
    test_cube(cube);
    cube.loadProgram("res/flat.vs", "res/flat.fs");
    systems.render->addNode(1, &cube);

    // Add rigid bodies to the physics system
    cout << "Creating rigid bodies" << endl;

    float sixth = 1.0f/6.0f;

    State start;
    start.orientation = Quaternion( 1.0, Constants::ORIGIN );
    start.position = Vector(0.0f, 0.0f, -10.0f);
    start.momentum = Constants::ORIGIN;
    start.angular_momentum = Constants::ORIGIN;
    start.mass = 1.0f;
    start.inverse_mass = 1.0f;
    start.inertia = glm::mat3(sixth, 0.0f, 0.0f, 0.0f, sixth, 0.0f, 0.0f, 0.0f, sixth);
    start.inverse_inertia = glm::inverse(start.inertia);

    systems.physics->addNode(1, start);

    // GO
    cout << "Starting main loop" << endl;
    dispatch.run();

    cout << "Stopped." << endl;
    return 0;
}

