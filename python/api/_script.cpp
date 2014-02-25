#include <boost/python.hpp>

#include <iostream>

#include "scriptsystem.h"
#include "script.h"
#include "core.h"

namespace py = boost::python;

class ScriptWrapper : public Script::IScript, public py::wrapper<Script::IScript>
{
    public:
    void tick()const override
    {
        // Check for Python override
        if(py::override f = this->get_override("tick")) {
            f();
            return;
        }

        // If there is no override, call the default implementation
        this->Script::IScript::tick();
        return;
    }

    void default_tick() const {
        return this->Script::IScript::tick();
    }
};

typedef std::shared_ptr<ScriptWrapper> ScriptWrapperPtr;
typedef std::shared_ptr<Script::IScript> IScriptPtr;


BOOST_PYTHON_MODULE(_script)
{
    py::class_<ScriptSystem, boost::noncopyable>("ScriptSystem")
        .def("add_script", &ScriptSystem::addScript)
        .def("add_script_node", &ScriptSystem::addScriptNode);

    py::class_<ScriptWrapper, ScriptWrapperPtr, boost::noncopyable>("Script")
        .def("tick", &Script::IScript::tick, &ScriptWrapper::default_tick);

    py::implicitly_convertible<ScriptWrapperPtr, IScriptPtr>();
}
