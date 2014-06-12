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
        bool exists(const std::string &, const std::string &) const;
        std::string get(const std::string &, const std::string &) const;
        void set(const std::string &, const std::string &, const std::string &) const;

    private:
        void init();

        bool py_parse_file(const std::string &);
        bool py_exists(const std::string &, const std::string &) const;
        std::string py_get(const std::string &, const std::string &) const;
        void py_set(const std::string &, const std::string &, const std::string &) const;

        boost::python::object conf_parser;
    };
}
#endif // CONFIGPARSER_H
