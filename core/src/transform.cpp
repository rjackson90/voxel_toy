#include "transform.h"

void Transform::tick(__attribute__((unused)) const SubsystemsPtr &systems, const double dt)
{

}

void Transform::recalculate(int idx)
{
    nodes[idx].world_coordinates = nodes[idx].orientation.toMatrix(nodes[idx].position);
}

// Getters

glm::mat4 Transform::getWorldCoords(int idx)
{
    return nodes[idx].world_coordinates;
}

Vector Transform::getPosition(int idx)
{
    return nodes[idx].position;
}

Quaternion Transform::getOrientation(int idx)
{
    return nodex[idx].orientation;
}

// Setters

void Transform::setPosition(int idx, Vector pos)
{
    nodes[idx].position = pos;
    recalculate(idx);
}

void Transform::setOrientation(int idx, Quaternion direction)
{
    nodes[idx].orientation = direction;
    recalculate(idx);
}
