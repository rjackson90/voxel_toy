#ifndef PHYSICS_SYSTEM_H
#define PHYSICS_SYSTEM_H

#include <unordered_map>
#include <cstdlib>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "core.h"
#include "math_ext.h"
#include "system.h"
#include "config.h"

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
        // Required to implement System interface
        virtual void tick(const SubsystemsPtr &, const double) override;
        void addNode(int, Physics::State&);
        void addNodeFromConfig(const std::string &);

        // Provide a 4x4 matrix describing the precise position and 
        // orientation of each node to other subsystems
        glm::mat4 getWorldCoords(int);
        
        // Methods to manage forces
        void addForce(int, const Vector&);
        void clearForces(int);

        // Methods to manage changes in velocity (delta-V or dV)
        void addDeltaV(int, const Vector&);
        Vector getStopDeltaV(int);

    private:
    struct RigidBodyNode : Node
    {
        /* RigidBodyNodes know about both their present and past states.
         * This fact makes smooth interpolation between states possible
         */
        Physics::State past;
        Physics::State present;

        std::vector<Vector> forces;
        std::vector<Vector> impulses;
    };

    std::unordered_map<int, RigidBodyNode> nodes;
};
#endif /*PHYSICS_SYSTEM_H*/
