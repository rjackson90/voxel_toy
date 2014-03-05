#ifndef ERROR_HPP
#define ERROR_HPP

#include <boost/python.hpp>
#include <boost/function.hpp>
#include <string>
#include <type_traits>

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
    std::string error_string(prefix);

    try
    {
        return py_call_ex(callable, prefix);
    }
    catch(const std::runtime_error &ex)
    {
        error_string = error_string + parse_python_exception();
        std::cerr << ex.what() << std::endl << error_string << std::endl;
    }

    // The function cannot return unless return_type is void. Unfortunately,
    // an exception MUST be thrown. Probably for the best, I guess...
    throw std::runtime_error("Previously reported Python error");
}

#endif // ERROR_HPP
