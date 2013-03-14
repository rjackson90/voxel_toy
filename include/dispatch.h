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

// Subsystem headers
#include "rendersystem.h"
#include "physicssystem.h"


static const int WINDOW_WIDTH = 1024;
static const int WINDOW_HEIGHT = 768;
static const string WINDOW_TITLE = "A Really Fun Game!!!";

double hires_time_seconds();

class Dispatch
{
    public:
    Dispatch();
    void run();
    static void signal_handler(int);

    // Subsystems
    RenderSystem render_sys;
    PhysicsSystem physics_sys;

    private:
    static bool isRunning;
};

#endif
