#include <boost/python.hpp>
#include "interpreter.h"

namespace py = boost::python;

class IScriptWrapper : public Python::IScript, public py::wrapper<Python::IScript>
{
    virtual void tick(const Subsystems& systems) override
    {
        this->get_override("tick")(systems);
    }
};

BOOST_PYTHON_MODULE(interpreter)
{
    py::class_<Interpreter>("Interpreter")
        .def("add_script", &Interpreter::addScript)
        .def("add_script_node", &Interpreter::addScriptNode);

    py::class_<IScriptWrapper, boost::noncopyable>("Script")
        .def("tick", py::pure_virtual(&Python::IScript::tick));
}
