// The code in this file was inspired by a series of blog posts by Joseph Turner
// thejosephturner.com/blog/post/embedding-python-in-c-applications-with-boostpython-part-3/

#include "config.h"

namespace py = boost::python;
using namespace std;
using namespace Core;

/// PUBLIC METHODS

ConfigParser::ConfigParser()
{
    py_call_nothrow<void>(boost::bind(&ConfigParser::init, this), 
            "Error setting up ConfigParser: ");
}

bool ConfigParser::parse_file(const string &filename)
{
    return py_call_nothrow<bool>(boost::bind(&ConfigParser::py_parse_file, this, filename),
            "Error parsing file: ");
}

bool ConfigParser::exists(const string &attr, const string &section) const
{
    return py_call_nothrow<bool>(boost::bind(&ConfigParser::py_exists, this, attr, section),
            "Error checking for attribute: ");
}

string ConfigParser::get(const string &attr, const string &section) const
{
    return py_call_nothrow<string>(boost::bind(&ConfigParser::py_get, this, attr, section),
            "Error getting config attribute: ");
}

void ConfigParser::set(const string &attr, const string &value, const string &section) const
{
    py_call_nothrow<void>(boost::bind(&ConfigParser::py_set, this, attr, value, section),
            "Error setting config attribute: ");
}

/// PRIVATE METHODS

void ConfigParser::init()
{
    py::object main = py::import("__main__");
    py::object ns = main.attr("__dict__");
    py::exec("import ConfigParser", ns);
    conf_parser = py::eval("ConfigParser.RawConfigParser()", ns);
}

bool ConfigParser::py_parse_file(const string &filename)
{
    return py::len(conf_parser.attr("read")(filename)) == 1;
}

bool ConfigParser::py_exists(const string &attr, const string &section) const
{
    return py::extract<bool>(conf_parser.attr("has_option")(section, attr));
}

string ConfigParser::py_get(const string &attr, const string &section) const
{
    return py::extract<string>(conf_parser.attr("get")(section, attr));
}

void ConfigParser::py_set(const string &attr, const string &value, const string &section) const
{
    conf_parser.attr("set")(section, attr, value);
}

