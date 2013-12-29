#include <boost/python.hpp>

#include <iostream>

#include "scriptsystem.h"
#include "script.h"
#include "core.h"

namespace py = boost::python;

class ScriptWrapper : public Script::IScript, public py::wrapper<Script::IScript>
{
    public:
    void tick(const SubsystemsPtr& systems)
    {
        if(py::override tick = this->get_override("tick"))
        {
            tick(systems);
            return;
        }
        return Script::IScript::tick(systems);
    }

    void default_tick(const SubsystemsPtr& systems) 
    { 
        return this->Script::IScript::tick(systems); 
    }
};

class Implementation : public Script::IScript
{
    public:
        virtual void tick(const SubsystemsPtr& systems) override
        {
            (void)&systems;
            std::cout << "tick" << std::endl;
        }
};


BOOST_PYTHON_MODULE(_script)
{

    py::class_<Script::IScript>("Script");

    py::class_<ScriptWrapper, std::shared_ptr<ScriptWrapper>, boost::noncopyable >(
            "Script", py::no_init)
        .def("tick", &Script::IScript::tick, &ScriptWrapper::default_tick);

    py::class_<Implementation, std::shared_ptr<Implementation>, py::bases<Script::IScript>>(
            "Implementation", py::init<>())
        .def("tick", &Implementation::tick);

    py::class_<ScriptSystem, ScriptPtr>("ScriptSystem", py::no_init)
        .def("add_script", &ScriptSystem::addScript)
        .def("add_script_node", &ScriptSystem::addScriptNode);
    
    py::implicitly_convertible<std::shared_ptr<Implementation>, std::shared_ptr<Script::IScript>>();
    py::implicitly_convertible<std::shared_ptr<ScriptWrapper>, std::shared_ptr<Script::IScript>>();
}
