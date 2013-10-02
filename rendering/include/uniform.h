#ifndef UNIFORM_H
#define UNIFORM_H

// System headers
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <algorithm>

// Forward declarations
namespace Rendering
{
    struct BlockDefinition;
    class UniformBuffer;
}

// Core headers
#include "dispatch.h"

namespace Rendering
{
    /* This class describes a uniform buffer. It holds a reference to a BlockDefinition 
     * (or more likely, a struct that inherits from BlockDefinition) which represents the uniform
     * block inside a shader program. This class manages the buffer backing the uniform block
     */
    class UniformBuffer
    {
    public:
        UniformBuffer(BlockDefinition&, GLuint);
        ~UniformBuffer();

        void bind(GLuint, const GLchar*);
        void updateContents(const Subsystems&, int key);
            
    private:
        GLuint buffer;
        GLuint index;
        BlockDefinition& block;
    };

    /* This struct describes the minimum requirements of a uniform block definition,
     * namely that all such definitions have names and optionally a callback function to update
     * the data in the struct
     */
    struct BlockDefinition
    {
        virtual void getData(const Subsystems&, __attribute__((unused))int key) {return;}
        virtual void updateBuffer() const = 0;
    };

    /* A TransformBlock holds all of the matrices needed to transform 3D geometry and the
     * associated normal vectors
     */
    struct TransformBlock : BlockDefinition
    {
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
        virtual void updateBuffer() const;

        glm::vec3 position;
        glm::vec4 intensity;
    };

    /* Materials are also pretty straightforward. These parameters define how geometry is
     * shaded in response to dynamic lights.
     */
    struct Material : BlockDefinition
    {
        virtual void updateBuffer() const;

        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float shininess;
    };
}

#endif // UNIFORM_H
