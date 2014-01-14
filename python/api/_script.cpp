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
        this->get_override("tick")(systems);
        return;
    }
};

typedef std::shared_ptr<ScriptWrapper> ScriptWrapperPtr;

BOOST_PYTHON_MODULE(_script)
{
    py::class_<ScriptSystem, boost::noncopyable>("ScriptSystem", py::no_init)
        .def("add_script", &ScriptSystem::addScript)
        .def("add_script_node", &ScriptSystem::addScriptNode);

    py::class_<ScriptWrapper, ScriptWrapperPtr, boost::noncopyable>("Script")
        .def("tick", py::pure_virtual(&Script::IScript::tick));

    py::implicitly_convertible< ScriptWrapperPtr, std::shared_ptr<Script::IScript> >();
}
