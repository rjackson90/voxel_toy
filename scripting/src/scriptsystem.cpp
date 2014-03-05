#include "scriptsystem.h"

using namespace Script;
namespace py = boost::python;

// PUBLIC

ScriptSystem::ScriptSystem() : scripts(), nodes()
{
    // Initialize Python interpreter
    Py_Initialize();

    // Get a reference to the global namespace
    globals = py::extract<py::dict>(py::import("__main__").attr("__dict__"));
    globals["sys"] = py::import("sys");
}

void ScriptSystem::tick(
        __attribute__((unused)) const SubsystemsPtr &systems,
        __attribute__((unused)) const double dt)
{
    // Tick scripts which run every frame
    for(auto script : scripts)
    {
        try {
            script->tick();
        } catch(const py::error_already_set &ex) {
            std::cout << "A Python error occured: " 
                      << parse_python_exception()
                      << std::endl;
        }
    }

    // Tick scripts associated with nodes
    for(std::pair<int, ScriptNode> script : nodes)
    {
        script.second.script->tick();
    }
}

void ScriptSystem::addPath(const std::string &path)
{
    return py_call_nothrow<void>(boost::bind(&ScriptSystem::py_addPath, this, path),
            "Error adding path to python import search tree: ");
}

void ScriptSystem::importModule(const std::string& module)
{
    return py_call_nothrow<void>(boost::bind(&ScriptSystem::py_importModule, this, module),
                "Error importing module: ");
}

void ScriptSystem::setSubsystems(const SubsystemsPtr &systems)
{
    return py_call_nothrow<void>(boost::bind(&ScriptSystem::py_setSubsystems, this, systems),
            "Error sharing Subsystems struct: ");
}

void ScriptSystem::addScript(const std::shared_ptr<IScript>  &script_ptr)
{
    scripts.push_back(script_ptr);
}

void ScriptSystem::addScriptNode(int id, IScript *script_ptr)
{
    ScriptSystem::ScriptNode node;
    node.key = id;
    node.script = script_ptr;

    nodes.insert({{id},{node}});
}

// PRIVATE

void ScriptSystem::py_addPath(const std::string &path)
{
    globals["import_path"] = path;
    py::exec("sys.path.append(import_path)", globals);
}

void ScriptSystem::py_importModule(const std::string &module)
{
    py::str mod(module.c_str());
    globals[mod] = py::import(mod);
}

void ScriptSystem::py_setSubsystems(const SubsystemsPtr &systems)
{
    py::object script_sys(py::ptr(systems->script.get()));
    globals["script_system"] = script_sys;
}
