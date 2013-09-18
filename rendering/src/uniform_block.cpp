#include "uniform.h"

using namespace Rendering;

// BlockDefinition implementations

void TransformBlock::getData(const Subsystems &systems, int key)
{
    using glm::mat4;
    using glm::inverse;
    using glm::transpose;

    mat4 m = systems.physics->getWorldCoords(key);
    mat4 v = systems.render->getCameraMatrix();
    mat4 p = systems.render->getPerspectiveMatrix();

    mvp = p * v * m;
    mv = v * m;
    normal_matrix = inverse(transpose(mv));
}

void TransformBlock::updateBuffer() const
{
    using glm::value_ptr;
    using glm::mat4;

    // Get pointers to GLM types
    auto mvp_ptr = value_ptr(mvp);
    auto mv_ptr = value_ptr(mv);
    auto normal_matrix_ptr = value_ptr(normal_matrix);

    // Create buffer to hold data
    float buffer[sizeof(mat4) * 3];

    // Copy values into buffer (following std140 layout rules)
    auto next = std::copy(mvp_ptr, mvp_ptr + 16, buffer);
    next = std::copy(mv_ptr, mv_ptr + 16, next);
    std::copy(normal_matrix_ptr, normal_matrix_ptr + 16, next);

    // Push buffer to GPU
    glBufferData(GL_UNIFORM_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
}

void PointLight::updateBuffer() const
{
    using glm::value_ptr;
    using glm::vec4;

    // Get pointers to GLM types
    auto pos_ptr = value_ptr(position);
    auto in_ptr = value_ptr(intensity);

    // Create a buffer to hold data
    float buffer[sizeof(vec4) * 2];

    // Copy values into buffer (following std140 layout rules)
    auto next = std::copy(pos_ptr, pos_ptr + 3, buffer);
    next += 1;  // Compensate for padding around vec3
    std::copy(in_ptr, in_ptr + 4, next);

    // Push buffer to GPU
    glBufferData(GL_UNIFORM_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
}

void Material::updateBuffer() const
{
    using glm::vec4;
    using glm::value_ptr;

    // Get pointers to GLM types
    auto am_ptr = value_ptr(ambient);
    auto dif_ptr = value_ptr(diffuse);
    auto spec_ptr = value_ptr(specular);
    auto shiny_ptr = &shininess;

    // Create a buffer to hold data
    float buffer[sizeof(vec4) * 3 + sizeof(float)];

    // Copy values into buffer (following std140 layout rules
    auto next = std::copy(am_ptr, am_ptr + 4, buffer);
    next = std::copy(dif_ptr, dif_ptr + 4, next);
    next = std::copy(spec_ptr, spec_ptr + 4, next);
    std::copy(shiny_ptr, shiny_ptr + 1, next);

    // Push buffer to GPU
    glBufferData(GL_UNIFORM_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
}
