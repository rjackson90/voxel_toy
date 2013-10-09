#include "integrator.h"

using namespace Physics;


/* Similar to the State struct, derivatives need to be evaluated from time to time */
Derivative Integrator::evaluate(const State &state)
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
Derivative Integrator::evaluate(State &state, double dt, const Derivative &d)
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
void Integrator::integrate(State &state, double dt)
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
