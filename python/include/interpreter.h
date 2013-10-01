#ifndef INTERPRETER_H
#define INTERPRETER_H

// System includes
#include <boost/python.hpp>

// Local includes
#include "dispatch.h"
#include "error.hpp"

namespace Python
{
    class IScript
    {
    public:
        virtual ~IScript(){}
        virtual void tick(const Subsystems&) = 0;
    };
}

class Interpreter : public System
{
public:
    Interpreter();

    virtual void tick(const Subsystems&, const double) override;

    void addPath(const std::string&);
    void importModule(const std::string&);
    void addScript(std::shared_ptr<Python::IScript>);
    void addScriptNode(int, std::shared_ptr<Python::IScript>);

private:
    void py_addPath(const std::string&);
    void py_importModule(const std::string&);

    struct ScriptNode : Node
    {
        std::shared_ptr<Python::IScript> script;
    };

    std::vector<std::shared_ptr<Python::IScript>> scripts;
    std::unordered_map<int, ScriptNode> nodes;
    boost::python::dict globals;
};

#endif // INTERPRETER_H
