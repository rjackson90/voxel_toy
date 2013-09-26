#ifndef RCONSOLE_H
#define RCONSOLE_H

#include <boost/python.hpp>
#include "py_error.hpp"

namespace Core
{
    class RemoteConsole
    {
    public:
        RemoteConsole();
        ~RemoteConsole();

        void tick();

    private:
        void py_init();
        void py_destroy();
        void py_tick();

        boost::python::object rconsole;
        boost::python::dict globals;

        boost::python::object listen_socket;
        boost::python::object shell;
    };
}

#endif // RCONSOLE_H
