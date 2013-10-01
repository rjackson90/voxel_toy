#ifndef ERROR_HPP
#define ERROR_HPP

#include <boost/python.hpp>
#include <boost/function.hpp>
#include <string>

std::string parse_python_exception();

// Call the passed functor, transform any Python errors into a std::runtime_error
template<typename return_type>
return_type py_call_ex(boost::function<return_type ()> callable, const std::string &prefix)
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

/* Wrapper for py_safe_call which handles the common case where errors are simply
 * printed to stderr
 */
template<typename return_type>
return_type py_call_nothrow(boost::function<return_type ()> callable, const std::string& prefix)
{
    try
    {
        return py_call_ex(callable, prefix);
    }
    catch(const std::runtime_error &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
}

#endif // ERROR_HPP
