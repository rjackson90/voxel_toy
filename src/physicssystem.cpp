#include "physicssystem.h"

const Vector UNIT_X = Vector(1.0f, 0.0f, 0.0f);
const Vector UNIT_Y = Vector(0.0f, 1.0f, 0.0f);
const Vector UNIT_Z = Vector(0.0f, 0.0f, 1.0f);

PhysicsSystem::PhysicsSystem(int node_count) : nodes(node_count)
{
}

void PhysicsSystem::tick(double dt)
{
    // Advance the physics simulation by dt
    for(std::vector<RigidBodyNode>::iterator it = nodes.begin(); it != nodes.end(); ++it)
    {
        it->past = it->present;
        integrate(it->present, dt);
    }
}

void PhysicsSystem::addNode(int key, Quaternion orient, Vector pos, Vector mo, Vector a_mo,
                            float inert, float inv_inert, float mass, float inv_mass)
{
    State initial;
    initial.orientation = orient;
    initial.position = pos;
    initial.momentum = mo;
    initial.angular_momentum = a_mo;
    initial.inertia = inert;
    initial.inverse_inertia = inv_inert;
    initial.mass = mass;
    initial.inverse_mass = inv_mass;
    initial.recalculate();

    State initial_prev = initial;

    RigidBodyNode newNode;
    newNode.key = key;
    newNode.past = initial_prev;
    newNode.present = initial;

    nodes.push_back(newNode);
}

void State::recalculate()
{
    // Linear quantities
    velocity = momentum * inverse_mass;

    // Rotational quantities
    angular_velocity = angular_momentum * inverse_inertia;
    orientation.normalize();
    spin = (Quaternion(0.0f, angular_velocity) * orientation) * 0.5f;

    // Coordinates -- there isn't currently a method to convert a Quaternion to a matrix
    // world_coords = glm::translate(glm::mat4(1.0), position) * glm::mat4_cast(orientation);
}

Derivative evaluate(const State &state)
{
    Derivative output;
    output.velocity = state.velocity;
    output.spin = state.spin;

    // Apply forces here
    
    return output;
}

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
