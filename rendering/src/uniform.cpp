#include "uniform.h"

using namespace Rendering;

// Implementation of UniformBuffer

UniformBuffer::UniformBuffer(BlockDefinition &blk, GLuint bind_point) : index(bind_point), block(blk)
{
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    block.updateBuffer();
    glBindBufferBase(GL_UNIFORM_BUFFER, index, buffer);
}

UniformBuffer::~UniformBuffer()
{
    glDeleteBuffers(1, &buffer);
}

void UniformBuffer::bind(GLuint program, const GLchar* text)
{
    GLuint idx = glGetUniformBlockIndex(program, text);
    glUniformBlockBinding(program, idx, index);
}

void UniformBuffer::updateContents(const SubsystemsPtr &systems, int key)
{
    glBindBuffer(GL_UNIFORM_BUFFER, buffer);
    block.getData(systems, key);
    block.updateBuffer();
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

// BlockDefinition implementations

TransformBlock TransformBlock::TransformFromConfig(
        const Core::ConfigParser &parser, const std::string &section)
{
    TransformBlock tb;
    try
    {
        tb.mvp = mat4_from_string(parser.get("mvp", section));
        tb.mv = mat4_from_string(parser.get("mv", section));
        tb.normal_matrix = mat4_from_string(parser.get("normal_matrix", section));
    }
    catch(const std::exception &ex)
    {
        std::cerr << "Error creating transform block: " << ex.what() << std::endl;
    }

    return tb;
}

void TransformBlock::getData( const SubsystemsPtr &systems, int key)
{
    using glm::mat4;
    using glm::inverse;
    using glm::transpose;

    mat4 m = systems->physics->getWorldCoords(key);
    mat4 v = systems->render->getCameraMatrix();
    mat4 p = systems->render->getPerspectiveMatrix();

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
    float buffer[(sizeof(mat4) * 3) / sizeof(float)];

    // Copy values into buffer (following std140 layout rules)
    auto next = std::copy(mvp_ptr, mvp_ptr + 16, buffer);
    next = std::copy(mv_ptr, mv_ptr + 16, next);
    std::copy(normal_matrix_ptr, normal_matrix_ptr + 16, next);

    // Push buffer to GPU
    glBufferData(GL_UNIFORM_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
}

PointLight PointLight::PointLightFromConfig(
        const Core::ConfigParser &parser, const std::string &section)
{
    PointLight pl;
    try
    {
        pl.position = vec3_from_string(parser.get("position", section));
        pl.intensity = vec4_from_string(parser.get("intensity", section));
    }
    catch(const std::exception &ex)
    {
        std::cerr << "Error creating point light: " << ex.what() << std::endl;
    }

    return pl;
}

void PointLight::updateBuffer() const
{
    using glm::value_ptr;
    using glm::vec4;

    // Get pointers to GLM types
    auto pos_ptr = value_ptr(position);
    auto in_ptr = value_ptr(intensity);

    // Create a buffer to hold data
    float buffer[(sizeof(vec4) * 2) / sizeof(float)];

    // Copy values into buffer (following std140 layout rules)
    auto next = std::copy(pos_ptr, pos_ptr + 3, buffer);
    next += 1;  // Compensate for padding around vec3
    std::copy(in_ptr, in_ptr + 4, next);

    // Push buffer to GPU
    glBufferData(GL_UNIFORM_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
}

Material Material::MaterialFromConfig(
        const Core::ConfigParser &parser, const std::string &section)
{
    Material mat;
    try
    {
        mat.ambient = vec4_from_string(parser.get("ambient", section));
        mat.diffuse = vec4_from_string(parser.get("diffuse", section));
        mat.specular = vec4_from_string(parser.get("specular", section));
        mat.shininess = stof(parser.get("shininess", section));
    }
    catch(const std::exception &ex)
    {
        std::cerr << "Error creating material: " << ex.what() << std::endl;
    }

    return mat;
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
    float buffer[(sizeof(vec4) * 3 + sizeof(float)) / sizeof(float)];

    // Copy values into buffer (following std140 layout rules
    auto next = std::copy(am_ptr, am_ptr + 4, buffer);
    next = std::copy(dif_ptr, dif_ptr + 4, next);
    next = std::copy(spec_ptr, spec_ptr + 4, next);
    std::copy(shiny_ptr, shiny_ptr + 1, next);

    // Push buffer to GPU
    glBufferData(GL_UNIFORM_BUFFER, sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
}
