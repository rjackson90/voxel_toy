#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "core.h"

class Transform : public System
{
    public:
    virtual void tick(const SubsystemsPtr &, const double) override;

    glm::mat4 getWorldCoords(int);
    Vector getPosition(int);
    Quaternion getOrientation(int);

    void setWorldCoords(int, glm::mat4);
    void setPosition(int, Vector);
    void setOrientation(int, Quaternion);


    private:
    struct TransformNode : Node
    {
        glm::mat4 world_coordinates;
        Vector position;
        Quaternion orientation;
    }

    std::unordered_map<int, TransformNode> nodes;
}

#endif /*TRANSFORM_H*/
