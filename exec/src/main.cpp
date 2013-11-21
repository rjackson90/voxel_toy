#include <GL/glew.h>
#include <iostream>

#include "data_paths.h"
#include "core.h"
#include "rendering.h"
#include "physics.h"

#include "dispatch.h"
#include "rendersystem.h"
#include "scriptsystem.h"
#include "inputsystem.h"
#include "physicssystem.h"

#include "rconsole.h"

int main()
{
    using namespace std;

    // Create container struct and initialize subsystems
    auto systems = SubsystemsPtr(new Subsystems());
    systems->dispatch = DispatchPtr(new Dispatch());
    systems->script = ScriptPtr(new ScriptSystem());
    systems->input = InputPtr(new InputSystem());
    systems->physics = PhysicsPtr(new PhysicsSystem());
    systems->render = RenderPtr(
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

    // Add python sources to import search tree
    systems->script->addPath(Paths::python);
    systems->script->addPath(Paths::python_lib);
    systems->script->importModule("core");
    systems->script->importModule("script");
    systems->script->setSubsystems(systems);

    // Start remote console
    auto console = std::make_shared<Script::RemoteConsole>();
    systems->script->addScript(console);

    // Generate geometry
    auto cube = std::make_shared<Rendering::Geometry>();
    cube->genTestCube();
    cube->setDrawMode(GL_TRIANGLES);

    auto quad = std::make_shared<Rendering::Geometry>();
    quad->genTestQuad();
    quad->setDrawMode(GL_TRIANGLES);

    // Load textures
    auto stonebrick = std::make_shared<Rendering::Texture>(
            GL_TEXTURE_2D, Paths::rendering+"stonebrick.tga");
    auto stonebrickn = std::make_shared<Rendering::Texture>(
            GL_TEXTURE_2D, Paths::rendering+"stonebrickn.tga");
    
    auto woodplank = std::make_shared<Rendering::Texture>(
            GL_TEXTURE_2D, Paths::rendering+"woodplank.tga");
    auto woodplankn = std::make_shared<Rendering::Texture>(
            GL_TEXTURE_2D, Paths::rendering+"woodplankn.tga");
    
    auto obsidian = std::make_shared<Rendering::Texture>(
            GL_TEXTURE_2D, Paths::rendering+"obsidian.tga");
    auto obsidiann = std::make_shared<Rendering::Texture>(
            GL_TEXTURE_2D, Paths::rendering+"obsidiann.tga");

    // Load program
    auto phong_program = std::make_shared<Rendering::Program>();
    if(!phong_program->attachShader(Paths::shaders+"phong.vs", Rendering::VertexShader)){
        return 1;
    }
    if(!phong_program->attachShader(Paths::shaders+"phong.fs", Rendering::FragmentShader)){
        return 1;
    }
    if(!phong_program->link()){
        return 1;
    }

    // Set texture sampling parameters
    Rendering::SamplerParams params;
    params.min_filter = GL_LINEAR;
    
    auto linear_blend = std::make_shared<Rendering::Sampler>(params);

    params.min_filter = GL_NEAREST;
    params.mag_filter = GL_NEAREST;

    auto nearest_sample = std::make_shared<Rendering::Sampler>(params);

    // Bundle texture data into VecTexDataTuplePtr objects
    auto stone_color = std::make_shared<Rendering::TexDataTuple>(
            stonebrick, linear_blend, "texColor", -1);
    auto stone_normal = std::make_shared<Rendering::TexDataTuple>(
            stonebrickn, nearest_sample, "texNormal", -1);
    Rendering::VecTexDataTuplePtr stone_data{stone_color, stone_normal};

    auto wood_color = std::make_shared<Rendering::TexDataTuple>(
            woodplank, linear_blend, "texColor", -1);
    auto wood_normal = std::make_shared<Rendering::TexDataTuple>(
            woodplankn, nearest_sample, "texNormal", -1);
    Rendering::VecTexDataTuplePtr wood_data{wood_color, wood_normal};

    auto obsidian_color = std::make_shared<Rendering::TexDataTuple>(
            obsidian, linear_blend, "texColor", -1);
    auto obsidian_normal = std::make_shared<Rendering::TexDataTuple>(
            obsidiann, nearest_sample, "texNormal", -1);
    Rendering::VecTexDataTuplePtr obsidian_data{obsidian_color, obsidian_normal};

    // Create uniform blocks
    Rendering::TransformBlock transform_block;
    Rendering::PointLight point_light_block;
    Rendering::Material material_block;

    // Set initial values for uniforms
    transform_block.mvp = glm::mat4(1.0f);
    transform_block.mv = glm::mat4(1.0f);
    transform_block.normal_matrix = glm::mat4(1.0f);

    point_light_block.position = glm::vec3(0.0f, 0.0f, 0.0f);
    point_light_block.intensity = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    material_block.ambient = glm::vec4(0.02f, 0.02f, 0.02f, 1.0f);
    material_block.diffuse = glm::vec4(0.6f, 0.65f, 0.6f, 1.0f);
    material_block.specular = glm::vec4(0.95f, 0.9f, 0.99f, 1.0f);
    material_block.shininess = 20.0f;

    // Load the blocks into buffers, then into UniformPairPtrs
    GLuint bindpoint = 0;
    auto transform_buffer = make_shared<Rendering::UniformBuffer>(transform_block, bindpoint++);
    auto point_light_buffer = make_shared<Rendering::UniformBuffer>(point_light_block, bindpoint++);
    auto material_buffer = make_shared<Rendering::UniformBuffer>(material_block, bindpoint++);

    auto transform = std::make_shared<Rendering::UniformPair>(transform_buffer, "TransformBlock");
    auto point_light = std::make_shared<Rendering::UniformPair>(point_light_buffer, "PointLight");
    auto material = std::make_shared<Rendering::UniformPair>(material_buffer, "Material");

    Rendering::VecUniformPairPtr phong_uniforms{transform, point_light, material};

    // Add buffers to Rendersystem's list of updates-per-frame
    systems->render->addFrameUniform(point_light);
    systems->render->addFrameUniform(material);

    // Wrap it all up into Effects
    auto phong_stone = std::make_shared<Rendering::GenericEffect>(
            0, phong_program, stone_data, phong_uniforms);
    auto phong_wood = std::make_shared<Rendering::GenericEffect>(
            2, phong_program, wood_data, phong_uniforms);
    auto phong_obsidian = std::make_shared<Rendering::GenericEffect>(
            4, phong_program, obsidian_data, phong_uniforms);

    // Create new RenderNodes
    systems->render->addNode(1, quad, {{phong_stone}}, {{transform}});
    systems->render->addNode(2, quad, {{phong_stone}}, {{transform}});
    systems->render->addNode(3, quad, {{phong_wood}}, {{transform}});
    systems->render->addNode(4, quad, {{phong_obsidian}}, {{transform}});

    // Add rigid bodies to the physics system
    cout << "Creating rigid bodies" << endl;

    float sixth = 1.0f/6.0f;

    Physics::State start;
    start.orientation = Quaternion( 1.0, Constants::ORIGIN );
    start.position = Vector(-1.25f, 1.25f, -0.0002f);
    start.momentum = Constants::ORIGIN;
    start.angular_momentum = Constants::ORIGIN;
    start.mass = 1.0f;
    start.inverse_mass = 1.0f;
    start.inertia_tensor = glm::mat3(sixth, 0.0f, 0.0f, 0.0f, sixth, 0.0f, 0.0f, 0.0f, sixth);
    start.inverse_inertia_tensor = glm::inverse(start.inertia_tensor);
    systems->physics->addNode(1, start);

    Physics::State cube2 = start;
    cube2.position.x = 1.25f;
    cube2.angular_momentum = Vector(0.0f, 0.05f, 0.0f);
    systems->physics->addNode(2, cube2);

    Physics::State cube3 = start;
    cube3.position.x = -3.75f;
    cube3.position.y = -1.75f;
    cube3.angular_momentum = Vector(0.0f, 0.0f, 0.05f);
    systems->physics->addNode(3, cube3);

    Physics::State cube4 = cube2;
    cube4.position.y = -1.25f;
    cube4.angular_momentum = Vector(0.05f, 0.0f, 0.0f);
    systems->physics->addNode(4, cube4);

    /* GO */
    cout << "Starting main loop" << endl;
    systems->dispatch->run(systems);

    cout << "Stopped." << endl;
    return 0;
}

