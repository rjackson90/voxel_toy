#include "physicssystem.h"

/* This is where the magic happens. During each subsystem tick the physics
 * simulation is advanced by dt
 */
void PhysicsSystem::tick(__attribute__((unused)) const SubsystemsPtr &systems, const double dt)
{
    // Advance the physics simulation by dt
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        it->second.past = it->second.present;
        integrate(it->second.present, dt);
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

/* This method is critical for interop between subsystems. 
 * The render system calls this method to obtain the position and 
 * orientation of objects in world-space.
 */
glm::mat4 PhysicsSystem::getWorldCoords(int key)
{
    return nodes[key].present.world_coords;
}

/* Certain fields of State objects have to be updated from time to time */
void State::recalculate()
{
    // Linear quantities
    velocity = momentum * inverse_mass;

    // Rotational quantities
    angular_velocity = Vector(inverse_inertia * angular_momentum.toGLMVec());
    orientation.normalize();
    spin = (Quaternion(0.0f, angular_velocity) * orientation) * 0.5f;

    // Coordinates
    world_coords = orientation.toMatrix(position);
}

/* Similar to the State struct, derivatives need to be evaluated from time to time */
Derivative evaluate(const State &state)
{
    Derivative output;
    output.velocity = state.velocity;
    output.spin = state.spin;

    /* NOT IMPLEMENTED: Application of forces from other objects
     * that code would go here.
     */
    
    return output;
}

/* This method is part of the guts of the RK4 integrator.
 * It allows the evaluation of derivatives using prior results
 */
Derivative evaluate(State &state, double dt, const Derivative &d)
{
    state.position         = state.position         + d.velocity * dt;
    state.momentum         = state.momentum         + d.force    * dt;
    state.orientation      = state.orientation      + d.spin     * dt;
    state.angular_momentum = state.angular_momentum + d.torque   * dt;
    state.recalculate();

    Derivative output;
    output.velocity = state.velocity;
    output.spin = state.spin;

    // Apply forces here

    return output;
}

/* Entry point for RK4 integrator. 
 */
void integrate(State &state, double dt)
{
    Derivative a, b, c, d;
    a = evaluate(state);
    b = evaluate(state, dt * 0.5f, a);
    c = evaluate(state, dt * 0.5f, b);
    d = evaluate(state, dt, c);

    float sixth_dt = 1.0f/6.0f * dt;
    state.position = state.position + 
        (a.velocity + (b.velocity + c.velocity) * 2.0f + d.velocity) * sixth_dt;
    state.momentum = state.momentum + 
        (a.force + (b.force + c.force) * 2.0f + d.force) * sixth_dt;
    state.orientation = state.orientation + 
        (a.spin + (b.spin + c.spin) * 2.0f + d.spin) * sixth_dt;
    state.angular_momentum = state.angular_momentum + 
        (a.torque + (b.torque + c.torque) * 2.0f + d.torque) * sixth_dt;

    state.recalculate();
}
