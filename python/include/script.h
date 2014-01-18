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
            virtual void tick() const {
                std::cout << "Default implementation" << std::endl;
            }
    };
}
#endif // SCRIPT_H
