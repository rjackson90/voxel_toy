#ifndef INTERPRETER_H
#define INTERPRETER_H

// System includes
#include <boost/python.hpp>
#include <unordered_map>

// Subsystem includes
#include "core.h"
#include "system.h"

// Local includes
#include "error.hpp"

namespace Script
{
    class IScript
    {
    public:
        virtual ~IScript(){}
        virtual void tick(const SubsystemsPtr&) = 0;
    };
}

class ScriptSystem : public System
{
public:
    ScriptSystem();

    virtual void tick(const SubsystemsPtr &, const double) override;

    void addPath(const std::string&);
    void importModule(const std::string&);
    void addScript(std::shared_ptr<Script::IScript>);
    void addScriptNode(int, std::shared_ptr<Script::IScript>);

private:
    void py_addPath(const std::string&);
    void py_importModule(const std::string&);

    struct ScriptNode : Node
    {
        std::shared_ptr<Script::IScript> script;
    };

    std::vector<std::shared_ptr<Script::IScript>> scripts;
    std::unordered_map<int, ScriptNode> nodes;
    boost::python::dict globals;
};

#endif // INTERPRETER_H