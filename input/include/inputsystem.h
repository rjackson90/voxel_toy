#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include <SDL2/SDL.h>

#include "core.h"
#include "system.h"


class InputSystem : public System{
    public:
        void addEvent(const SDL_Event&);
        virtual void tick(const SubsystemsPtr &,double);
        void addNode(int, std::vector<SDL_Keycode>, void (*)());
    private:
        struct InputNode : Node
        {
            std::vector<SDL_Keycode> keys;
            void (*func)();
            bool active;
        };
        std::vector<InputNode> nodes;
        std::vector<std::shared_ptr<SDL_Event>> events;
};
#endif // INPUT_SYSTEMS_H

