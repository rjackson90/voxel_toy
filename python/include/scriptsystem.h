#ifndef SCRIPTSYSTEM_H
#define SCRIPTSYSTEM_H

// System includes
#include <boost/python.hpp>
#include <unordered_map>

// Subsystem includes
#include "core.h"
#include "system.h"

// Local includes
#include "error.hpp"
#include "script.h"

class ScriptSystem : public System
{
public:
    ScriptSystem();

    virtual void tick(const SubsystemsPtr &, const double) override;

    void addPath(const std::string&);
    void importModule(const std::string&);
    void setSubsystems(const SubsystemsPtr &);

    void addScript(const std::shared_ptr<Script::IScript> &);
    void addScriptNode(int, Script::IScript *);

private:
    void py_addPath(const std::string&);
    void py_importModule(const std::string&);
    void py_setSubsystems(const SubsystemsPtr &);

    struct ScriptNode : Node
    {
        Script::IScript * script;
    };

    std::vector<std::shared_ptr<Script::IScript>> scripts;
    std::unordered_map<int, ScriptNode> nodes;
    boost::python::dict globals;
};

#endif // SCRIPTSYSTEM_H
