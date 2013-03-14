#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <list>

#include "math_ext.h"
#include "system.h"

struct State
{
    // primary
    Quaternion orientation;
    Vector position;
    Vector momentum;
    Vector angular_momentum;

    // secondary
    Quaternion spin;
    Vector velocity;
    Vector angular_velocity;
    // glm::mat4 world_coords; // Need a replacement for GLM::mat4

    // constant
    float inertia;
    float inverse_inertia;
    float mass;
    float inverse_mass;

    void recalculate();
};

struct Derivative
{
    Vector velocity;
    Vector force;
    Quaternion spin;
    Vector torque;

};

// These functions make up the RK4 integrator
Derivative evaluate(const State &, float, const Derivative &);
Derivative evaluate(const State &);
void integrate(State &, float);

class PhysicsSystem : public System
{
    public:
    private:
    struct RigidBodyNode : Node
    {
    };

    std::list<RigidBodyNode> nodes;
};
#endif /*PHYSICS_SYSTEM_H*/
