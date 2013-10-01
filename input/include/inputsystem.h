#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H
#include "dispatch.h"
#include <SDL2/SDL.h>
#include <vector>
#include <stdarg.h>
#include <memory>
class InputSystem : public System{
    public:
        void addEvent(const SDL_Event&);
        virtual void tick(const Subsystems&,double);
       // bool evaluate(int);
        void addNode(std::vector<SDL_Keycode>, void (*)());
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

