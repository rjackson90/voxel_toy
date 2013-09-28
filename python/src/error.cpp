#include "error.hpp"

namespace py = boost::python;

// This function parses a Python exception into a nice, human-readable string
std::string parse_python_exception()
{
    PyObject* type = NULL;
    PyObject* value = NULL;
    PyObject* trace = NULL;
    std::string result("Unfetchable Python Error");

    PyErr_Fetch(&type, &value, &trace);

    // Extract the type of the exception
    if(type != NULL)
    {
        py::handle<> type_handle(type);
        py::str type_str(type_handle);
        py::extract<std::string> err_type_str(type_str);
        if(err_type_str.check())
            result = err_type_str();
        else
            result = "Unknown Type";
    }

    // Extract the value of the exception
    if(value != NULL)
    {
        py::handle<> value_handle(value);
        py::str value_str(value_handle);
        py::extract<std::string> err_value_str(value_str);
        if(err_value_str.check())
            result += ": " + err_value_str.check();
        else
            result += std::string(": Unparseable Value");
    }

    // Extract the traceback data
    if(trace != NULL)
    {
        py::handle<> tb_handle(trace);
        py::object tb(py::import("traceback"));
        py::object tb_fmt(tb.attr("format_tb"));
        py::object tb_list(tb_fmt(tb_handle));
        py::object tb_str(py::str("\n").join(tb_list));
        py::extract<std::string> err_tb_str(tb_str);
        if(err_tb_str.check())
            result += ": " + err_tb_str();
        else
            result += std::string(": Unparseable traceback");
    }

    return result;
}
