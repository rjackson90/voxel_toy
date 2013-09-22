#include "timer.h"

using namespace Core;

Timer::Timer() : tick_per_second(SDL_GetPerformanceFrequency())
{
}

void Timer::start()
{
    start_tick = SDL_GetPerformanceCounter();
}

double Timer::time_since_start()
{
    unsigned long end_tick = SDL_GetPerformanceCounter();
    return(double)(end_tick - start_tick) / (double)tick_per_second;
}
