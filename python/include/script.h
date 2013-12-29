#ifndef SCRIPT_H
#define SCRIPT_H

// Subsystems includes
#include "core.h"

namespace Script
{
    class IScript
    {
        public:
            virtual void tick(const SubsystemsPtr &systems)
            {
                // Default implementation is a no-op
                (void)&systems;
            }
    };
}
#endif // SCRIPT_H
