#ifndef UNIFORM_BLOCK_H
#define UNIFORM_BLOCK_H

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>
#include <algorithm>

#include "dispatch.h"

namespace Rendering
{
    /* This struct describes the minimum requirements of a uniform block definition,
     * namely that all such definitions have names and optionally a callback function to update
     * the data in the struct
     */
    struct BlockDefinition
    {
        const GLchar* name;

        BlockDefinition(const GLchar* block_name) : name(block_name){}
        virtual void getData(const Subsystems&, __attribute__((unused))int key) {return;}
        virtual void updateBuffer() const = 0;
    };

    /* A TransformBlock holds all of the matrices needed to transform 3D geometry and the
     * associated normal vectors
     */
    struct TransformBlock : BlockDefinition
    {
        TransformBlock() : BlockDefinition("TransformBlock"){}
        virtual void getData(const Subsystems&, int key);
        virtual void updateBuffer() const;

        glm::mat4 mvp;
        glm::mat4 mv;
        glm::mat4 normal_matrix; // Really a mat3, but layout(std140) pads to sizeof(mat4)
    };

    /* A PointLight block is just what it sounds like - all of the information needed to model
     * a point light in 3D space.
     */
    struct PointLight : BlockDefinition
    {
        PointLight() : BlockDefinition("PointLight"){}
        virtual void updateBuffer() const;

        glm::vec3 position;
        glm::vec4 intensity;
    };

    /* Materials are also pretty straightforward. These parameters define how geometry is
     * shaded in response to dynamic lights.
     */
    struct Material : BlockDefinition
    {
        Material() : BlockDefinition("Material"){}
        virtual void updateBuffer() const;

        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;
    };
}

#endif // UNIFORM_BLOCK_H
