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
        // Swap State buffers
        it->second.past = it->second.present;

        // Apply impulses before integrating forces
        for(const Vector &impulse : it->second.impulses)
        {
            it->second.present.momentum += impulse;
        }
        it->second.impulses.clear();

        // Integrate forces
        RK4::integrate(it->second.present, it->second.forces, dt);
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

void PhysicsSystem::addDeltaV(int node, const Vector &dV)
{
    nodes[node].impulses.push_back(dV * nodes[node].present.mass);
}

Vector PhysicsSystem::getStopDeltaV(int node)
{
    Vector stop = nodes[node].present.momentum * -1.0f / nodes[node].present.mass;
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
