#include "transform.h"

void Transform::tick(__attribute__((unused)) const SubsystemsPtr &systems, 
                     __attribute__((unused)) const double dt)
{

}

void Transform::addNode(int idx, const Vector &pos, const Quaternion &orient)
{
    TransformNode node;
    node.position = pos;
    node.orientation = orient;

    nodes.insert({{idx, node}});
}

void Transform::addNodeDependency(int idx)
{
    TransformNode node;
    node.position = Vector();
    node.orientation = Quaternion();
    
    // We don't want to trample an existing transform, so search before write
    auto has_key = nodes.find(idx);
    if(has_key == nodes.end()) {
        nodes[idx] = node;
    }

    std::cout << "Added new transform node. Node count: " << nodes.size() << std::endl;
}
void Transform::recalculate(int idx)
{
    nodes[idx].world_coordinates = nodes[idx].orientation.toMatrix(nodes[idx].position);
}

// Getters

glm::mat4 Transform::getWorldCoords(int idx)
{
    recalculate(idx);
    return nodes[idx].world_coordinates;
}

Vector Transform::getPosition(int idx)
{
    return nodes[idx].position;
}

Quaternion Transform::getOrientation(int idx)
{
    return nodes[idx].orientation;
}

// Setters

void Transform::setPosition(int idx, Vector pos)
{
    nodes[idx].position = pos;
}

void Transform::setOrientation(int idx, Quaternion direction)
{
    nodes[idx].orientation = direction;
}
