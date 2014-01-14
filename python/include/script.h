#ifndef SCRIPT_H
#define SCRIPT_H

// Subsystems includes
#include "core.h"

namespace Script
{
    class IScript
    {
        public:
            virtual ~IScript() {}
            virtual void tick(const SubsystemsPtr &systems) = 0;
    };
}
#endif // SCRIPT_H
