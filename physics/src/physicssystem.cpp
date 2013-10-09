#include "physicssystem.h"

using namespace Physics;

/* This is where the magic happens. During each subsystem tick the physics
 * simulation is advanced by dt
 */
void PhysicsSystem::tick(__attribute__((unused)) const SubsystemsPtr &systems, const double dt)
{
    // Advance the physics simulation by dt
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        it->second.past = it->second.present;
        RK4::integrate(it->second.present, dt);
    }
}

/* This method adds a new node to the physics system. Because it
 * has no past as far as the simulation is concerned, the initial state
 * occupies both state buffers in the RigidBodyNode
 */
void PhysicsSystem::addNode(int key, State &initial)
{
    initial.recalculate();
    
    RigidBodyNode newNode;
    newNode.key = key;
    newNode.past = initial;
    newNode.present = initial;

    nodes.insert({{key, newNode}});
}

void PhysicsSystem::addForce(int node, const Vector &force)
{
    nodes[node].forces.push_back(force);
}

void PhysicsSystem::clearForces(int node)
{
    nodes[node].forces.clear();
}

void PhysicsSystem::addImpulse(int node, const Vector &impulse)
{
    nodes[node].impulses.push_back(impulse);
}

Vector PhysicsSystem::getStopImpulse(int node)
{
    Vector stop = nodes[node].present.momentum * -1.0f;
    return stop;
}

/* This method is critical for interop between subsystems. 
 * The render system calls this method to obtain the position and 
 * orientation of objects in world-space.
 */
glm::mat4 PhysicsSystem::getWorldCoords(int key)
{
    return nodes[key].present.world_coords;
}
