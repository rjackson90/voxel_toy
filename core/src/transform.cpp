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

void Transform::addNodeFromConfig(const Core::ConfigParser &parser, const std::string &section)
{
    Vector position;
    Quaternion orientation;
    int idx = -1;
    bool status = false;

    // Swallow exceptions. If something bad happens, don't add the node to the map
    try
    {
        // The nodeID is in the Meta section
        idx = stoi(parser.get("nodeID", "Meta"));

        // Everything else is in the passed section name
        position = Vector::from_string(parser.get("position", section));
        orientation = Quaternion::from_string(parser.get("orientation", section));

        // If execution gets here, everything is fine and we can add the node to the map
        status = true;
    }
    catch(const std::exception &ex)
    {
        std::cerr << "Error while parsing config file " << ex.what()
                  << std::endl;
    }

    if(status) this->addNode(idx, position, orientation);
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
