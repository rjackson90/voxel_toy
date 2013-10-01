#include "dispatch.h"

/* The constructor for the Dispatch object simply initializes the platform's
 * high resolution timer.
 */
Dispatch::Dispatch() : isRunning(true)
{
    // Initialize SDL
    std::cout << "Starting up...";
    SDL_SetMainReady();
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << " FAIL: " << SDL_GetError() << std::endl;
        isRunning = false;
        return;
    }
    std::cout << " OK!" << std::endl;
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

    Core::Timer frame_timer;
    frame_timer.start();

    double accumulator = 0.0;

    SDL_Event ev_buffer;
    
    while(isRunning)
    {
        double elapsed = frame_timer.time_since_start();
        frame_timer.start();
        
        /* Accumulate time between frames. Think of this as a 'Time Supplier'.
         * Accumulator growth capped at 250ms, at which point frames will be skipped. This is 
         * done to prevent a death spiral if the simulation gets behind for several frames
         */
        if ( elapsed > 0.250 )
            elapsed = 0.250;
        accumulator += elapsed;
        // Poll SDL Events, dispatch or handle as appropriate
        while(SDL_PollEvent(&ev_buffer))
        {
            switch(ev_buffer.type)
            {
                case SDL_KEYDOWN:
                    systems.input->addEvent(ev_buffer);
                    break;

                case SDL_KEYUP:
                    systems.input->addEvent(ev_buffer);
                    break;

                case SDL_QUIT:
                    isRunning=false;
                    return;
            }
        }
        systems.input->tick(systems, dt);
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
