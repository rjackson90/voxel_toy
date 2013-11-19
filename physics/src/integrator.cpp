#include "integrator.h"

using namespace Physics;


/* Similar to the State struct, derivatives need to be evaluated from time to time */
Derivative Integrator::evaluate(const State &state, const Vector &force)
{
    Derivative output;
    output.velocity = state.velocity;
    output.spin = state.spin;
    output.force = force;
    
    return output;
}

/* This method is part of the guts of the RK4 integrator.
 * It allows the evaluation of derivatives using prior results
 */
Derivative Integrator::evaluate(State &state, double dt, const Derivative &d, const Vector &force)
{
    state.position         += d.velocity    * dt;
    state.momentum         += d.force       * dt;
    state.orientation      += d.spin        * dt;
    state.angular_momentum += d.torque      * dt;
    state.recalculate();

    Derivative output;
    output.velocity = state.velocity;
    output.spin = state.spin;
    output.force = force;
    return output;
}

/* Entry point for RK4 integrator. 
 */
void Integrator::integrate(State &state, const std::vector<Vector> &forces, double dt)
{
    Vector sum_forces;
    for(const Vector &force : forces)
    {
        sum_forces += force;
    }
    
    Derivative a, b, c, d;
    a = evaluate(state, sum_forces);
    b = evaluate(state, dt * 0.5f, a, sum_forces);
    c = evaluate(state, dt * 0.5f, b, sum_forces);
    d = evaluate(state, dt, c, sum_forces);

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
