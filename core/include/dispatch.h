/* dispatch.h -- Grand Central Dispatch dev notes:
 *  This class is responsible for keeping time, routing events, interacting with the system,
 *  and generally keeping everything ticking along. 
 */

#ifndef DISPATCH_H
#define DISPATCH_H

// SDL includes
#define SDL_MAIN_HANDLED
#include "SDL.h"

// Other system includes
#include <GL/glew.h>
#include <string>
#include <iostream>
#include <memory>

// Forward declarations, typedefs
#include "core.h"

// Subsystem includes
#include "rendersystem.h"
#include "physicssystem.h"
#include "scriptsystem.h"
#include "inputsystem.h"

// Core includes
#include "timer.h"
#include "data_paths.h"

class Dispatch
{
    public:
    Dispatch();
    ~Dispatch();
    void run(const SubsystemsPtr&);

    // If the program needs to quit, call this method
    void quit();

    private:
    bool isRunning;
};

#endif
