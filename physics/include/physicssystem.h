#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core.h"
#include "math_ext.h"
#include "system.h"

#include "physics.h"
#include "integrator.h"

/* Like other subsystems, the Physics subsystem is surprisingly simple.
 * Put simply, it consists of a data structure which holds RigidBodyNodes.
 * The id associated with each RigidBodyNode identifies the in-game object
 * associated with the node. 
 */
class PhysicsSystem : public System
{
    public:
        virtual void tick(const SubsystemsPtr &, const double) override;
        
        void addNode(int, Physics::State&);
        
        void addForce(int, const Vector&);
        void clearForces(int);

        void addImpulse(int, const Vector&);
        Vector getStopImpulse(int);

        glm::mat4 getWorldCoords(int);
    private:
    struct RigidBodyNode : Node
    {
        /* RigidBodyNodes know about both their present and past states.
         * This knowledge makes interpolating between them for a smooth 
         * result super easy.
         */
        Physics::State past;
        Physics::State present;

        std::vector<Vector> forces;
        std::vector<Vector> impulses;
    };

    std::unordered_map<int, RigidBodyNode> nodes;
};
#endif /*PHYSICS_SYSTEM_H*/
