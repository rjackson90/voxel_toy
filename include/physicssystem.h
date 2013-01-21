#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <list>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "system.h"

struct State
{
    // primary
    glm::quat orientation;
    glm::vec3 position;
    glm::vec3 momentum;
    glm::vec3 angular_momentum;

    // secondary
    glm::quat spin;
    glm::vec3 velocity;
    glm::vec3 angular_velocity;
    glm::mat4 world_coords;

    // constant
    float inertia;
    float inverse_inertia;
    float mass;
    float inverse_mass;

    void recalculate();
};

struct Derivative
{
    glm::vec3 velocity;
    glm::vec3 force;
    glm::quat spin;
    glm::vec3 torque;
};

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
