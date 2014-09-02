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
    systems->transform = TransformPtr(new Transform());
    
    // Initialize hardware-accelerated window.
    int width = 1280;
    int height = 800;
    const std::string title = "Really Fun Game!";
    Rendering::GLWindowPtr window = make_shared<Rendering::GLWindow>();

    // Quit if the window requirements cannot be met
    if(!window->init(width, height, title)) {
        cout << "Failed to initialize window. Please ensure your system "
             << "meets minimum requirements for graphics hardware and that "
             << "your hardware is properly configured." << endl;
        return 1;
    }
    systems->render = RenderPtr(new RenderSystem(window));

    // Add python sources to import search tree
    systems->script->addPath(Paths::python);
    systems->script->addPath(Paths::python_lib);
    systems->script->importModule("core");
    systems->script->importModule("script");
    systems->script->importModule("testpkg");
    systems->script->setSubsystems(systems);

    // Start remote console
    auto console = std::make_shared<Script::RemoteConsole>();
    systems->script->addScript(console);

    // Create ConfigParser
    Core::ConfigParser parser;
    parser.parse_file(Paths::config_root+"static_stone.cfg");

    // Create Transform
    systems->transform->addNode(1, Vector(), Quaternion());

    // Create RenderNodes
    auto geometry = Rendering::Geometry::GeometryFromConfig(parser, "Geometry");
    auto effect = Rendering::TextureEffect::TextureEffectFromConfig(parser, "Effect");
    //auto effect = Rendering::GenericEffect::GenericEffectFromConfig(parser, "Effect");
    systems->render->addNode(1, geometry, {{effect}}, {});

/*
    // Generate geometry
    Core::ConfigParser gparse;

    gparse.parse_file(Paths::config_root+"quad_geom.cfg");
    auto quad = Rendering::Geometry::GeometryFromConfig(gparse, "QuadGeometry");

    // Load textures

    gparse.parse_file(Paths::config_root+"textures.cfg");
    auto stonebrick = Rendering::Texture::TextureFromConfig(gparse, "StoneBrick");
    auto stonebrickn = Rendering::Texture::TextureFromConfig(gparse, "StoneBrickNormal");
    
    auto woodplank = Rendering::Texture::TextureFromConfig(gparse, "WoodPlank");
    auto woodplankn = Rendering::Texture::TextureFromConfig(gparse, "WoodPlankNormal");
    
    auto obsidian = Rendering::Texture::TextureFromConfig(gparse, "Obsidian");
    auto obsidiann = Rendering::Texture::TextureFromConfig(gparse, "ObsidianNormal");
    
    // Load program

    gparse.parse_file(Paths::config_root+"phong_shading.cfg");
    auto phong_program = Rendering::Program::ProgramFromConfig(gparse, "PhongShading");

    // Set texture sampling parameters

    gparse.parse_file(Paths::config_root+"sampler.cfg");
    auto linear_blend = Rendering::Sampler::SamplerFromConfig(gparse, "LinearBlend");
    auto nearest_sample = Rendering::Sampler::SamplerFromConfig(gparse, "NearestSample");
    
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
    gparse.parse_file(Paths::config_root+"uniforms.cfg");

    Rendering::TransformBlock transform_block = 
        Rendering::TransformBlock::TransformFromConfig(gparse, "Transform");
    Rendering::PointLight point_light_block = 
        Rendering::PointLight::PointLightFromConfig(gparse, "PointLight");
    Rendering::Material material_block = 
        Rendering::Material::MaterialFromConfig(gparse, "Material");

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
*/
    // Add rigid bodies to the physics system
    cout << "Creating rigid bodies" << endl;
    
    parser.parse_file(Paths::config_root+"static_stone.cfg");
    systems->physics->addNodeFromConfig(parser);

    parser.parse_file(Paths::config_root+"spinny_stone.cfg");
    systems->physics->addNodeFromConfig(parser);

    parser.parse_file(Paths::config_root+"turning_wood.cfg");
    systems->physics->addNodeFromConfig(parser);

    parser.parse_file(Paths::config_root+"obsidian.cfg");
    systems->physics->addNodeFromConfig(parser);

    /* GO */
    cout << "Starting main loop" << endl;
    systems->dispatch->run(systems);

    cout << "Stopped." << endl;
    return 0;
}

