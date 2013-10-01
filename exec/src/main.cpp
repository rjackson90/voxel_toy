#include <GL/glew.h>
#include <iostream>

#include "dispatch.h"

// Special header generated by build system, defines variables holding absolute 
// paths to module data directories
#include "data_paths.h"

struct Subsystems;
class Dispatch;
class RenderSystem;
class PhysicsSystem;
class InputSystem;

int main()
{
    using namespace std;

    // Create container struct and initialize subsystems
    Subsystems systems;
    systems.dispatch = std::unique_ptr<Dispatch>(new Dispatch());
    systems.input = std::unique_ptr<InputSystem>(new InputSystem());
    systems.physics = std::unique_ptr<PhysicsSystem>(new PhysicsSystem());
    systems.render = std::unique_ptr<RenderSystem>(
            new RenderSystem(
                1024, 
                768, 
                "Really Fun Game!",
                {
                {SDL_GL_RED_SIZE, 8},
                {SDL_GL_GREEN_SIZE, 8},
                {SDL_GL_BLUE_SIZE, 8},
                {SDL_GL_ALPHA_SIZE, 8},
                {SDL_GL_ACCELERATED_VISUAL, 1},
//                {SDL_GL_CONTEXT_MAJOR_VERSION, 3},
//                {SDL_GL_CONTEXT_MINOR_VERSION, 3},
                {SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG},
                {SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE}
                })
            );

    // Generate geometry
    Rendering::Geometry cube;
    cube.genTestCube();
    cube.setDrawMode(GL_TRIANGLES);

    // Load textures
    Rendering::Texture stonebrick(GL_TEXTURE_2D, Paths::rendering+"stonebrick.tga");
    Rendering::Texture stonebrickn(GL_TEXTURE_2D, Paths::rendering+"stonebrickn.tga");
    
    Rendering::Texture woodplank(GL_TEXTURE_2D, Paths::rendering+"woodplank.tga");
    Rendering::Texture woodplankn(GL_TEXTURE_2D, Paths::rendering+"woodplankn.tga");
    
    Rendering::Texture obsidian(GL_TEXTURE_2D, Paths::rendering+"obsidian.tga");
    Rendering::Texture obsidiann(GL_TEXTURE_2D, Paths::rendering+"obsidiann.tga");

    // Load program
    Rendering::Program phong_program(Paths::shaders+"phong.vs", Paths::shaders+"phong.fs");
    if(!phong_program.isValid())
    {
        return 1;
    }

    // Set texture sampling parameters
    Rendering::SamplerParams params;
    params.min_filter = GL_LINEAR;
    
    Rendering::Sampler linear_blend(params);

    params.min_filter = GL_NEAREST;
    params.mag_filter = GL_NEAREST;

    Rendering::Sampler nearest_sample(params);

    // Create uniform blocks
    Rendering::TransformBlock transform;
    Rendering::PointLight point_light;
    Rendering::Material material;

    // Set initial values for uniforms
    transform.mvp = glm::mat4(1.0f);
    transform.mv = glm::mat4(1.0f);
    transform.normal_matrix = glm::mat4(1.0f);

    point_light.position = glm::vec3(0.0f, 0.0f, 0.0f);
    point_light.intensity = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    material.ambient = glm::vec4(0.02f, 0.02f, 0.02f, 1.0f);
    material.diffuse = glm::vec4(0.6f, 0.65f, 0.6f, 1.0f);
    material.specular = glm::vec4(0.95f, 0.9f, 0.99f, 1.0f);
    material.shininess = 20.0f;

    // Load the blocks into buffers
    GLuint bindpoint = 0;
    auto transform_buffer = make_shared<Rendering::UniformBuffer>(transform, bindpoint++);
    auto point_light_buffer = make_shared<Rendering::UniformBuffer>(point_light, bindpoint++);
    auto material_buffer = make_shared<Rendering::UniformBuffer>(material, bindpoint++);

    // Add buffers to Rendersystem's list of updates-per-frame
    systems.render->frame_uniforms.push_back(point_light_buffer);
    systems.render->frame_uniforms.push_back(material_buffer);

    // Wrap it all up into Effects
    auto phong_stone = std::make_shared<Rendering::PhongShading>(
            0, 
            phong_program,
            stonebrick, stonebrickn,
            linear_blend, nearest_sample,
            transform_buffer, point_light_buffer, material_buffer
            );
    auto phong_wood = std::make_shared<Rendering::PhongShading>(
            2,
            phong_program,
            woodplank, woodplankn,
            linear_blend, nearest_sample,
            transform_buffer, point_light_buffer, material_buffer
            );
    auto phong_obsidian = std::make_shared<Rendering::PhongShading>(
            4,
            phong_program,
            obsidian, obsidiann,
            linear_blend, nearest_sample,
            transform_buffer, point_light_buffer, material_buffer
            );

    // Create new RenderNodes
    systems.render->addNode(1, cube, {{phong_stone}}, {{transform_buffer}});
    systems.render->addNode(2, cube, {{phong_stone}}, {{transform_buffer}});
    systems.render->addNode(3, cube, {{phong_wood}}, {{transform_buffer}});
    systems.render->addNode(4, cube, {{phong_obsidian}}, {{transform_buffer}});

    // Add rigid bodies to the physics system
    cout << "Creating rigid bodies" << endl;

    float sixth = 1.0f/6.0f;

    State start;
    start.orientation = Quaternion( 1.0, Constants::ORIGIN );
    start.position = Vector(-1.25f, 1.25f, -0.0002f);
    start.momentum = Constants::ORIGIN;
    start.angular_momentum = Constants::ORIGIN;
    start.mass = 1.0f;
    start.inverse_mass = 1.0f;
    start.inertia = glm::mat3(sixth, 0.0f, 0.0f, 0.0f, sixth, 0.0f, 0.0f, 0.0f, sixth);
    start.inverse_inertia = glm::inverse(start.inertia);
    systems.physics->addNode(1, start);

    State cube2 = start;
    cube2.position.x = 1.25f;
    cube2.angular_momentum = Vector(0.0f, 0.05f, 0.0f);
    systems.physics->addNode(2, cube2);

    State cube3 = start;
    cube3.position.x = -3.75f;
    cube3.position.y = -1.75f;
    cube3.angular_momentum = Vector(0.0f, 0.0f, 0.05f);
    systems.physics->addNode(3, cube3);

    State cube4 = cube2;
    cube4.position.y = -1.25f;
    cube4.angular_momentum = Vector(0.05f, 0.0f, 0.0f);
    systems.physics->addNode(4, cube4);

    /* GO */
    cout << "Starting main loop" << endl;
    systems.dispatch->run(systems);

    cout << "Stopped." << endl;
    return 0;
}

