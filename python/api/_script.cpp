#include <boost/python.hpp>

#include <iostream>

#include "scriptsystem.h"
#include "script.h"
#include "core.h"

namespace py = boost::python;

class Default : public Script::IScript {
    public:
    void tick () const override {
        std::cout << "Default Script implementation" << std::endl;
    }
};

class ScriptWrapper : public Script::IScript, public py::wrapper<Script::IScript>
{
    public:
    void tick()const override
    {
        // Check for Python override
        if(py::override f = this->get_override("tick")) {
            std::cout << "Found Python override for 'void tick(const SubsystemsPtr&)'"
                      << std::endl;
            f();
            return;
        }

        // If there is no override, call the default implementation
        std::cout << "No Python override found, calling C++ implementation." << std::endl;
        this->Script::IScript::tick();
        return;
    }

    void default_tick() const {
        return this->Script::IScript::tick();
    }
};

typedef std::shared_ptr<ScriptWrapper> ScriptWrapperPtr;


void test_sc(const Script::IScript &sc) {
    sc.tick();
}


void test_sc_ptr(const Script::IScript * const sc) {
    sc->tick();
}

void test_sc_shptr(const std::shared_ptr<Script::IScript> &sc) {
    sc->tick();
}

void test_sc_vec(const std::shared_ptr<Script::IScript> &sc) {
    ScriptSystem sys;
    sys.addScript(sc);
    sc->tick();
}

BOOST_PYTHON_MODULE(_script)
{
    py::class_<ScriptSystem, ScriptPtr, boost::noncopyable>("ScriptSystem")
        .def("add_script", &ScriptSystem::addScript)
        .def("add_script_node", &ScriptSystem::addScriptNode);

    py::class_<ScriptWrapper, std::shared_ptr<ScriptWrapper>, boost::noncopyable>("Script")
        .def("tick", &Script::IScript::tick, &ScriptWrapper::default_tick);

    py::class_<Default>("Default")
        .def("tick", &Default::tick);

    py::def("test_sc", test_sc);
    py::def("test_sc_ptr", test_sc_ptr);
    py::def("test_sc_shptr", test_sc_shptr);
    py::def("test_sc_vec", test_sc_vec);

    py::implicitly_convertible< ScriptWrapperPtr, std::shared_ptr<Script::IScript> >();
    py::implicitly_convertible< ScriptWrapper*, Script::IScript* >();
}
