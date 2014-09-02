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

    // Create ConfigParser, parse nodes
    Core::ConfigParser parser;

    static const string config_files[] = {
        "static_stone.cfg",
        "spinny_stone.cfg",
        "turning_wood.cfg",
        "obsidian.cfg"
    };

    for(int i = 0; i < 4; i++) {
        parser.parse_file(Paths::config_root+config_files[i]);
        systems->transform->addNodeDependency(stoi(parser.get("nodeID", "Meta")));
        systems->render->addNodeFromConfig(parser);
        systems->physics->addNodeFromConfig(parser);
    }

    /* GO */
    cout << "Starting main loop" << endl;
    systems->dispatch->run(systems);

    cout << "Stopped." << endl;
    return 0;
}

