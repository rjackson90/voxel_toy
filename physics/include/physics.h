/* This file forward declares types in the Physics module. It also provides
 * convenient typedefs for pointers-to-type and containers-of-pointers-to-type
 * where appropriate. This header also defines common structures.
 */
#ifndef PHYSICS_H
#define PHYSICS_H

#include "math_ext.h"
#include <glm/glm.hpp>

namespace Physics
{
    /* This struct represents all of the state information required to physically
     * simulate a single rigid body. Constants need to be set prior to runtime, primary
     * values should be set before simulation begins
     */
    struct State
    {
        // Primary fields
        Quaternion orientation;
        Vector position;
        Vector momentum;
        Vector angular_momentum;

        // Secondary fields
        Quaternion spin;
        Vector velocity;
        Vector angular_velocity;
        glm::mat4 world_coords;

        // Constant fields
        glm::mat3 inertia_tensor;
        glm::mat3 inverse_inertia_tensor;
        float mass;
        float inverse_mass;

        /* Certain fields of State objects have to be updated from time to time */
        void recalculate()
        {
            // Linear quantities
            velocity = momentum * inverse_mass;

            // Rotational quantities
            angular_velocity = Vector(inverse_inertia_tensor * angular_momentum.toGLMVec());
            orientation.normalize();
            spin = (Quaternion(0.0f, angular_velocity) * orientation) * 0.5f;

            // Coordinates
            world_coords = orientation.toMatrix(position);
        }
    };
}

#endif
