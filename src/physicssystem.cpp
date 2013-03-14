#include "physicssystem.h"

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

Derivative evaluate(State &state, float dt, const Derivative &d)
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

void integrate(State &state, float dt)
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
