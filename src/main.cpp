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
    /* Create container struct and initialize subsystems */
    Subsystems systems;
    systems.render = std::unique_ptr<RenderSystem>(new RenderSystem(1024, 768, "Really Fun Game!"));
    systems.physics = std::unique_ptr<PhysicsSystem>(new PhysicsSystem());
    systems.dispatch = std::unique_ptr<Dispatch>(new Dispatch());

    /* Register signal handler with the system */
    cout << "Setting system signal handler." << endl;
    signal(SIGINT, Dispatch::signal_handler);

    /* Add meshes to the render system */
    cout << "Creating meshes" << endl;

    Mesh cube;
    test_cube(cube);
    cube.loadProgram("res/phong.vs", "res/phong.fs");
    Mesh box1/*, box2, box3, box4*/ = cube;

    box1.loadTextures("res/stonebrick.tga", "res/stonebrickn.tga");
    //box2.loadTextureFile("res/stonebrickn.tga");

    systems.render->addNode(1, &box1);    
    systems.render->addNode(2, &box1);
    systems.render->addNode(3, &box1);
    systems.render->addNode(4, &box1);

    /*
    Mesh light_cube;
    test_cube(light_cube);
    light_cube.loadProgram("res/texture.vs", "res/texture.fs");
    cube.loadTextureFile("res/stonebrickn.tga");
    systems.render->addNode(2, &light_cube);
    */

    /* Add rigid bodies to the physics system */
    cout << "Creating rigid bodies" << endl;

    float sixth = 1.0f/6.0f;

    State start;
    start.orientation = Quaternion( 1.0, Constants::ORIGIN );
    start.position = Vector(-1.0f, 1.0f, -1.5f);
    start.momentum = Constants::ORIGIN;
    start.angular_momentum = Constants::ORIGIN;
    start.mass = 1.0f;
    start.inverse_mass = 1.0f;
    start.inertia = glm::mat3(sixth, 0.0f, 0.0f, 0.0f, sixth, 0.0f, 0.0f, 0.0f, sixth);
    start.inverse_inertia = glm::inverse(start.inertia);
    systems.physics->addNode(1, start);

    State cube2 = start;
    cube2.position.x = 1.0f;
    cube2.angular_momentum = Vector(0.0f, 0.0025f, 0.0f);
    systems.physics->addNode(2, cube2);

    State cube3 = start;
    cube3.position.y = -1.0f;
    cube3.angular_momentum = Vector(0.0f, 0.0f, 0.0025f);
    systems.physics->addNode(3, cube3);

    State cube4 = cube2;
    cube4.position.y = -1.0f;
    cube4.angular_momentum = Vector(0.0025f, 0.0f, 0.0f);
    systems.physics->addNode(4, cube4);

    /* GO */
    cout << "Starting main loop" << endl;
    systems.dispatch->run(systems);

    cout << "Stopped." << endl;
    return 0;
}

