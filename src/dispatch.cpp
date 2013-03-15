#include "dispatch.h"

bool Dispatch::isRunning = false;

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

// The constructor for the Dispatch object performs global initialization and
// triggers init for subsystems
Dispatch::Dispatch(Subsystems *sys) : systems(sys)
{
    std::cout << "Starting up..." << std::endl;

    timespec ts;
    clock_getres(CLOCK_MONOTONIC_RAW, &ts);
    double time_sec = ts.tv_sec + (ts.tv_nsec * 0.000000001);
    std::cout << "Resolution of CLOCK_MONOTONIC_RAW is: " << time_sec << std::endl;
}

void Dispatch::run()
{
    double t = 0.0;
    const double dt = 0.015;

    double lastTime = hires_time_seconds();
    double accumulator = 0.0;

    isRunning = true;
    while(isRunning)
    {
        double now = hires_time_seconds();
        double elapsed = now - lastTime;
        if ( elapsed > 0.250 ) // Limit accumulator to linear growth, skip ahead if behind
            elapsed = 0.250;
        lastTime = now;

        accumulator += elapsed;

        // Poll input devices in sync with screen draws

        while ( accumulator >= dt )
        {
            // physics runs until the simulation is accurate for the present time. 
            systems->physics->tick(dt);

            t += dt;
            accumulator -= dt;
        }
        
        // Interpolation factor, unused at the moment
        // const double alpha = accumulator / dt;
        
        // Interpolate between the current and prior physics state, then render the results
        systems->render->tick(dt);
    }
}

double hires_time_seconds()
{
    static timespec ts;
    clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return (ts.tv_sec * 1.0) + (ts.tv_nsec / 1000000000.0);
}
