#include <boost/python.hpp>

#include "core.h"

namespace py = boost::python;

BOOST_PYTHON_MODULE(_core)
{
    py::class_<Subsystems, SubsystemsPtr, boost::noncopyable>("Subsystems")
        .def_readonly("render", &Subsystems::render)
        .def_readonly("physics", &Subsystems::physics)
        .def_readonly("input", &Subsystems::input)
        .def_readonly("script", &Subsystems::script)
        .def_readonly("dispatch", &Subsystems::dispatch);
}
