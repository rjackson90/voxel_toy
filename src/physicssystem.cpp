#include "physicssystem.h"

void State::recalculate()
{
    // Linear quantities
    velocity = momentum * inverse_mass;

    // Rotational quantities
    angular_velocity = angular_momentum * inverse_inertia;
    glm::normalize(orientation);
    spin = 0.5f * glm::dot(glm::quat(0.0, angular_velocity), orientation);

    // Coordinates
    world_coords = glm::translate(glm::mat4(1.0), position) * glm::mat4_cast(orientation);
}
