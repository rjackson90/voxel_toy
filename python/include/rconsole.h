#ifndef RCONSOLE_H
#define RCONSOLE_H

#include "interpreter.h"

namespace Python
{
    class RemoteConsole : public IScript
    {
    public:
        RemoteConsole();
        ~RemoteConsole();

        virtual void tick(const Subsystems &) override;

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
