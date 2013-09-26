#ifndef PY_ERROR_HPP
#define PY_ERROR_HPP

#include <boost/python.hpp>
#include <boost/function.hpp>
#include <string>

std::string parse_python_exception();

// Call the passed functor, transform any Python errors into a std::runtime_error
template<typename return_type>
return_type py_safe_call(boost::function<return_type ()> callable, const std::string &prefix)
{
    std::string error_string(prefix);

    try
    {
        return callable();
    }
    catch(boost::python::error_already_set const &)
    {
        error_string = error_string + parse_python_exception();
        throw std::runtime_error(error_string);
    }
}

#endif // PY_ERROR_HPP
