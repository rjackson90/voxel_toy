/* dispatch.h -- Grand Central Dispatch dev notes:
 *  This class is responsible for keeping time, routing events, interacting with the system,
 *  and generally keeping everything ticking along. 
 */

#ifndef DISPATCH_H
#define DISPATCH_H

// Forward declarations
class Rendersystem;
struct Subsystems;

// SDL includes
#define SDL_MAIN_HANDLED
#include "SDL.h"

// Other system includes
#include <boost/python.hpp>
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <memory>

// Subsystem includes
class RenderSystem;
#include "rendersystem.h"
#include "physicssystem.h"

// Core includes
#include "timer.h"
#include "data_paths.h"
#include "py_error.hpp"
#include "rconsole.h"

class Dispatch
{
    public:
    Dispatch();
    ~Dispatch();
    void run(const Subsystems&);

    // If the program needs to quit, call this method
    void quit();

    private:
    bool isRunning;
};

bool py_setPath(const std::string&);

/* This struct holds unique pointers to the various subsystems. 
 * Instead of relying on global state, this struct is passed down as needed, making code that needs
 * direct access to subsystems much easier to mock out and test.
 */
struct Subsystems
{
    std::unique_ptr<RenderSystem> render;
    std::unique_ptr<PhysicsSystem> physics;
    std::unique_ptr<Dispatch> dispatch;
};

#endif
