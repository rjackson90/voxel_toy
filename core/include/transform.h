#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "core.h"
#include "system.h"
#include "math_ext.h"
#include "config.h"

#include <unordered_map>

class Transform : public System
{
    public:
    virtual void tick(const SubsystemsPtr &, const double) override;
    void addNode(int, const Vector &, const Quaternion &);
    void addNodeFromConfig(const Core::ConfigParser &, const std::string &);

    glm::mat4 getWorldCoords(int);
    Vector getPosition(int);
    Quaternion getOrientation(int);

    void setPosition(int, Vector);
    void setOrientation(int, Quaternion);


    private:
    struct TransformNode : Node
    {
        glm::mat4 world_coordinates;
        Vector position;
        Quaternion orientation;
    };

    std::unordered_map<int, TransformNode> nodes;

    void recalculate(int);
};

#endif /*TRANSFORM_H*/
