#include <boost/python.hpp>

#include "scriptsystem.h"

namespace py = boost::python;

class IScriptWrapper : public Script::IScript, public py::wrapper<Script::IScript>
{
    virtual void tick(const SubsystemsPtr& systems) override
    {
        this->get_override("tick")(systems);
    }
};

BOOST_PYTHON_MODULE(_script)
{
    py::class_<ScriptSystem, ScriptPtr>("ScriptSystem", py::no_init)
        .def("add_script", &ScriptSystem::addScript)
        .def("add_script_node", &ScriptSystem::addScriptNode);

    py::class_<IScriptWrapper, boost::noncopyable>("Script", py::no_init)
        .def("tick", py::pure_virtual(&Script::IScript::tick));

    py::register_ptr_to_python<std::shared_ptr<Script::IScript>>();
}
