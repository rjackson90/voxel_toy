#include "interpreter.h"

using namespace Python;
namespace py = boost::python;

// PUBLIC

Interpreter::Interpreter() : scripts(), nodes()
{
    // Initialize Python interpreter
    Py_Initialize();

    // Get a reference to the global namespace
    globals = py::extract<py::dict>(py::import("__main__").attr("__dict__"));
    globals["sys"] = py::import("sys");
}

void Interpreter::tick(const Subsystems &systems, __attribute__((unused))const double dt)
{
    // Tick scripts which run every frame
    for(const std::shared_ptr<IScript>& script : scripts)
    {
        script->tick(systems);
    }

    // Tick scripts associated with nodes
    for(std::pair<int, ScriptNode> script : nodes)
    {
        script.second.script->tick(systems);
    }
}

void Interpreter::addPath(const std::string &path)
{
    return py_call_nothrow<void>(boost::bind(&Interpreter::py_addPath, this, path),
            "Error adding path to python import search tree: ");
}

void Interpreter::importModule(const std::string& module)
{
    return py_call_nothrow<void>(boost::bind(&Interpreter::py_importModule, this, module),
                "Error importing module: ");
}

void Interpreter::addScript(std::shared_ptr<Python::IScript> script_ptr)
{
    scripts.push_back(script_ptr);
}

void Interpreter::addScriptNode(int id, std::shared_ptr<Python::IScript> script_ptr)
{
    Interpreter::ScriptNode node;
    node.key = id;
    node.script = script_ptr;

    nodes.insert({{id},{node}});
}

// PRIVATE

void Interpreter::py_addPath(const std::string &path)
{
    globals["import_path"] = path;
    py::exec("sys.path.append(import_path)", globals);
}

void Interpreter::py_importModule(const std::string &module)
{
    py::str mod(module.c_str());
    globals[mod] = py::import(mod);
}
