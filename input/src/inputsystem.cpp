#include "inputsystem.h"

void InputSystem::tick(__attribute__((unused)) const Subsystems& systems, __attribute__((unused))double dt)
{
    bool activate;
    for(InputNode & node : nodes)
    {
       activate = true;
       for(const SDL_Keycode & key : node.keys)
       {
            for(const std::shared_ptr<SDL_Event> & event : events)
            {
        
                if(event->key.keysym.sym==key)
                {
                    activate = true;
                    break;
                }
                else
                {
                    activate = false;
                }

            }
            if(!activate)
            {
                node.active = false;
                break;
            }

       }
       if(activate && !(node.active))
       {
         node.func();
         node.active = true;
       }

    }
    events.clear();
    return;
}

void InputSystem::addNode(int key, std::vector<SDL_Keycode> keyst, void (*funct)())
{
    InputNode node;
    node.key = key;
    node.keys = keyst;
    node.func = funct;
    node.active = false;
    nodes.insert(nodes.begin() + key, node);
}

void InputSystem::addEvent(const SDL_Event& event)
{
    events.push_back(std::make_shared<SDL_Event>(event));
    return;
}
