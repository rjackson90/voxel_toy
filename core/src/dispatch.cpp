#include "dispatch.h"

/* 'Magic Bool' which keeps track of program state.
 */
bool Dispatch::isRunning = true;

/* The constructor for the Dispatch object simply initializes the platform's
 * high resolution timer.
 */
Dispatch::Dispatch()
{
    std::cout << "Starting up..." << std::endl;

    // Initialize SDL
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    timespec ts;
    clock_getres(CLOCK_MONOTONIC_RAW, &ts);
    double time_sec = ts.tv_sec + (ts.tv_nsec * 0.000000001);
    std::cout << "Resolution of CLOCK_MONOTONIC_RAW is: " << time_sec << std::endl;
}

Dispatch::~Dispatch()
{
    // Clean up SDL
    SDL_Quit();
}

/* This method pushes an SQL_QUIT event onto the event queue, signalling the program to 
 * gracefully terminate ASAP
 */
void Dispatch::quit()
{
    SDL_Event *quit = new SDL_Event();
    quit->type = SDL_QUIT;
    SDL_PushEvent(quit);
    delete quit;
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

    SDL_Event ev_buffer;
    
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

        // Poll SDL Events, dispatch or handle as appropriate
        while(SDL_PollEvent(&ev_buffer))
        {
            switch(ev_buffer.type)
            {
                case SDL_QUIT:
                    isRunning=false;
                    return;
            }
        }

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
