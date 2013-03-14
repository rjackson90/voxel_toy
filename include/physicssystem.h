#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <vector>

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
Derivative evaluate(const State &, double, const Derivative &);
Derivative evaluate(const State &);
void integrate(State &, double);

// Physics Subsystem
class PhysicsSystem : public System
{
    public:
        PhysicsSystem(int=10);
        virtual void tick(double);
        void addNode(int, Quaternion, Vector, Vector, Vector, float, float, float, float);
    private:
    struct RigidBodyNode : Node
    {
        State past;
        State present;
    };

    std::vector<RigidBodyNode> nodes;
};
#endif /*PHYSICS_SYSTEM_H*/
