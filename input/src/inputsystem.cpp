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
        
                if(event->keysym->sym==key)
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
                &node->active = false;
                break;
            }

       }
       if(activate&&!(&node->active))
       {
         *node.func();
         node.active = true;
       }

    }
    events.clear();
    return;
}
/*bool InputSystem::evaluate(int num)
{
    InputNode *node = &nodes.at(num);
    for(auto it = node->keys.begin(); it !=node->keys.end(); ++it)
    {
        auto event = events.begin(); 
        while(event->keysym->sym!=*it&&event!=events->end())
        {
            event++;
            if(event==events->end()&&event.keysym->sym!=*it)
            {
                node->active = false;
                return false;    
            }
        }
    }
    return true;
}
*/
void InputSystem::addNode(std::vector<SDL_Keycode> keyst, void (*funct)());
{
    InputNode node;
    node.keys = keyst;
    node.func = funct;
    node->active = false;
    nodes.insert({{key, node}});
}
void InputSystem::addEvent(const SDL_Event& event)
{
    events.push_back(&event);
    return;
}
