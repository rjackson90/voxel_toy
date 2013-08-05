/* dispatch.h -- Grand Central Dispatch dev notes:
 *  This class is responsible for keeping time, routing events, interacting with the system,
 *  and generally keeping everything ticking along. 
 */

#ifndef DISPATCH_H
#define DISPATCH_H

#include <GL/glew.h>
#include <string>
#include <time.h>
#include <iostream>
#include <signal.h>
#include <memory>

class RenderSystem;

#include "rendersystem.h"
#include "physicssystem.h"

// This function wraps the highest-available resolution timer provided by the platform
double hires_time_seconds();

class Dispatch
{
    public:
    Dispatch();
    void run(const Subsystems&);

    // UNIX only. Used to handle Ctrl+C events (possibly others?)
    static void signal_handler(int);

    private:
    static bool isRunning;
};

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
