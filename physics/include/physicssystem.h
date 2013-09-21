#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "math_ext.h"
#include "system.h"

/* This struct represents all of the state information required to physically
 * simulate a single rigid body. Constants need to be set prior to runtime, primary
 * values should be set before simulation begins
 */
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

/* This struct is not user facing, as its members are all
 * calculated from the State struct defined above.
 */
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

/* Like other subsystems, the Physics subsystem is surprisingly simple.
 * Put simply, it consists of a data structure which holds RigidBodyNodes.
 * The id associated with each RigidBodyNode identifies the in-game object
 * associated with the node. 
 */
class PhysicsSystem : public System
{
    public:
        virtual void tick(const Subsystems&, const double) override;
        void addNode(int, State&);
        glm::mat4 getWorldCoords(int);
    private:
    struct RigidBodyNode : Node
    {
        /* RigidBodyNodes know about both their present and past states.
         * This knowledge makes interpolating between them for a smooth 
         * result super easy.
         */
        State past;
        State present;
    };

    std::unordered_map<int, RigidBodyNode> nodes;
};
#endif /*PHYSICS_SYSTEM_H*/
