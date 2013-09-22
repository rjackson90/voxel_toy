#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

namespace Core
{
    class Timer
    {
    public:
        Timer();
        void start();
        double time_since_start();
    private:
        unsigned long start_tick;
        unsigned long tick_per_second;
    };
}

#endif // TIMER_H
