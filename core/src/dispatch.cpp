#include "dispatch.h"

/* 'Magic Bool' which keeps track of program state.
 */
bool Dispatch::isRunning = false;

/* UNIX ONLY
 * This function handles signals from the platform, such as SIGINT, SIGTERM, etc.
 * The most obvious benefit is that Ctrl+C causes a clean shutdown instead of an abort
 */
void Dispatch::signal_handler(int signum)
{
    switch(signum)
    {
        case SIGINT:
            std::cout << "Recieved SIGINT: Shutting down." << std::endl;
            Dispatch::isRunning = false;
            break;
        default:
            std::cout << "Recieved signal for which no action is defined: " << signum << std::endl;
    }
        
}

/* The constructor for the Dispatch object simply initializes the platform's
 * high resolution timer.
 */
Dispatch::Dispatch()
{
    std::cout << "Starting up..." << std::endl;

    timespec ts;
    clock_getres(CLOCK_MONOTONIC_RAW, &ts);
    double time_sec = ts.tv_sec + (ts.tv_nsec * 0.000000001);
    std::cout << "Resolution of CLOCK_MONOTONIC_RAW is: " << time_sec << std::endl;
}

/* This member function acts as the "main loop" for the game. The while loop in this function
 * determines the start and stop times for each tick, and is responsible for triggering each
 * subsystem in the proper order. Interpolation of game state also happens here to account for 
 * sync errors.
 */
void Dispatch::run(const Subsystems &systems)
{
    /* Times are in seconds */
    double t = 0.0;
    const double dt = 0.015;

    double lastTime = hires_time_seconds();
    double accumulator = 0.0;

    isRunning = true;
    while(isRunning)
    {
        double now = hires_time_seconds();
        double elapsed = now - lastTime;
        
        /* Accumulate time between frames. Think of this as a 'Time Supplier'.
         * Accumulator growth capped at 250ms, at which point frames will be skipped. This is 
         * done to prevent a death spiral if the simulation gets behind for several frames
         */
        if ( elapsed > 0.250 )
            elapsed = 0.250;
        lastTime = now;
        accumulator += elapsed;

        /* NOT IMPLEMENTED
         * Poll input devices in sync with screen draws
         */

        /* This loop consumes time from the accumulator in dt sized chunks. Most of the time,
         * the physics simulation will be run once per drawn frame, but occaisonally we need to 
         * run the physical simulation multiple times to catch up to the present time. This happens
         * if the simulation had a couple of slow frames,
         */
        while ( accumulator >= dt )
        {
            // physics runs until the simulation is accurate for the present time. 
            systems.physics->tick(systems, dt);

            t += dt;
            accumulator -= dt;
        }
        
        /* NOT IMPLEMENTED
         * const double alpha = accumulator / dt;
         * where alpha is the state interpolation factor.
         * In the future, the physics state will be interpolated such that precisely matches
         * the time at which the graphics frame is rendered, +/- a few ms.
         */
        
        /* Render the scene
         */
        systems.render->tick(systems, dt);
    }
}

/* This function provides a wrapper around the platform-dependent high resolution timer
 */
double hires_time_seconds()
{
    static timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (ts.tv_sec * 1.0) + (ts.tv_nsec / 1000000000.0);
}
