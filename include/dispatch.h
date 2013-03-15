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

class RenderSystem;

#include "rendersystem.h"
#include "physicssystem.h"

double hires_time_seconds();

class Dispatch
{
    public:
    Dispatch(Subsystems *);
    void run();
    static void signal_handler(int);

    private:
    static bool isRunning;
    Subsystems *systems;
};

struct Subsystems
{
    RenderSystem* render = nullptr;
    PhysicsSystem* physics = nullptr;
    Dispatch* dispatch = nullptr;
};

#endif
