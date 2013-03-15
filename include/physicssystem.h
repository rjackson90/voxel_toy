#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    glm::mat4 world_coords;

    // constant
    glm::mat3 inertia;
    glm::mat3 inverse_inertia;
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

struct Subsystems;

class PhysicsSystem : public System
{
    public:
        PhysicsSystem(Subsystems *);
        virtual void tick(const double) override;
        void addNode(int, State);
        glm::mat4 getWorldCoords(int);
    private:
    struct RigidBodyNode : Node
    {
        State past;
        State present;
    };

    std::unordered_map<int, RigidBodyNode> nodes;
    Subsystems *systems;
};
#endif /*PHYSICS_SYSTEM_H*/
