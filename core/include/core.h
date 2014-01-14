/* This file forward declares classes in the Core module, as well as the
 * subsystems classes. The subsystem classes are defined and "live" in their 
 * modules, but they are forward declared here along with the Subsystems struct
 * in order to centralize header dependencies.
 */
#ifndef CORE_H
#define CORE_H

#include <memory>
#include <vector>

// Forward declare subsystem classes, as well as unique pointer typedefs
class Dispatch;
typedef std::shared_ptr<Dispatch> DispatchPtr;

class RenderSystem;
typedef std::shared_ptr<RenderSystem> RenderPtr;

class PhysicsSystem;
typedef std::shared_ptr<PhysicsSystem> PhysicsPtr;

class InputSystem;
typedef std::shared_ptr<InputSystem> InputPtr;

class ScriptSystem;
typedef std::shared_ptr<ScriptSystem> ScriptPtr;

// Define Subsystems struct

/* This struct holds unique pointers to the various subsystems. 
 * Instead of relying on global state, this struct is passed down as needed, 
 * making code that needs direct access to subsystems much easier to test.
 */
struct Subsystems
{
    RenderPtr render;
    PhysicsPtr physics;
    InputPtr input;
    ScriptPtr script;
    DispatchPtr dispatch;
};
typedef std::shared_ptr<Subsystems> SubsystemsPtr;

// Names in the Core namespace
namespace Core
{
    class Timer;
    typedef std::shared_ptr<Timer> TimerPtr;
}

#endif // CORE_H
