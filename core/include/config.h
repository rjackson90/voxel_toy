#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <boost/python.hpp>
#include "error.hpp"

namespace Core
{
    class ConfigParser
    {
    public:
        ConfigParser();

        bool parse_file(const std::string &);
        std::string get(const std::string &, const std::string &);
        void set(const std::string &, const std::string &, const std::string &);

    private:
        void init();

        bool py_parse_file(const std::string &);
        std::string py_get(const std::string &, const std::string &);
        void py_set(const std::string &, const std::string &, const std::string &);

        boost::python::object conf_parser;
    };
}
#endif // CONFIGPARSER_H
