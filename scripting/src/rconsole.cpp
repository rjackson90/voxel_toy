#include "rconsole.h"

namespace py = boost::python;
using namespace Script;

RemoteConsole::RemoteConsole() : rconsole(), globals(), listen_socket(), shell()
{
    py_call_nothrow<void>(boost::bind(&RemoteConsole::py_init, this), 
            "Error starting remote shell server: ");
}

RemoteConsole::~RemoteConsole()
{
    py_call_nothrow<void>(boost::bind(&RemoteConsole::py_destroy, this),
            "Error stopping remote shell server: ");
}

void RemoteConsole::tick() const
{
    py_call_nothrow<void>(boost::bind(&RemoteConsole::py_tick, this),
            "Error in the remote shell interactive loop: ");
}

void RemoteConsole::py_tick() const
{
    // If we don't have a shell yet, call rconsole.accept()
    if(shell == NULL || shell.is_none())
    {
        shell = rconsole.attr("accept")(listen_socket, globals);
        if (!shell.is_none()){
            std::cout << "Remote console connected to client" << std::endl;
        }
        return;
    }

    bool result = py::extract<bool>(shell.attr("tick")());
    if(!result){
        shell = py::object();
        std::cout << "Remote console disconnected from client" << std::endl;
    }
}

void RemoteConsole::py_init()
{
    rconsole = py::import("core.rconsole");
    listen_socket = rconsole.attr("listen")();

    globals = py::extract<py::dict>(py::import("__main__").attr("__dict__"));

    std::cout << "Initialized remote console" << std::endl;
}

void RemoteConsole::py_destroy()
{
    std::cout << "Cleaning up Remote Console" << std::endl;
    if(!shell.is_none())
        shell.attr("cleanup")();
    if(!listen_socket.is_none())
        listen_socket.attr("close")();

    std::cout << "Cleaned up remote console" << std::endl;
}
