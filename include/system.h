#ifndef SYSTEM_H
#define SYSTEM_H

#include <list>

struct Subsystems;

/* This class provides the basis upon which all subsystems are built. 
 * The important things of note are the tick(float dt) function and the Node struct.
 * The presence of those things are the only assumptions made about the subsystems.
 */
class System
{
    public:
    virtual void tick(const Subsystems &,const double) = 0;

    protected:
    struct Node
    {
        int key;
    };
};

#endif /*SYSTEM_H*/
