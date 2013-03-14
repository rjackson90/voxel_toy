#ifndef SYSTEM_H
#define SYSTEM_H

#include <list>

class Dispatch;

class System
{
    public:
    virtual void tick(double dt=0.0) = 0;
    void attachDispatch(Dispatch*);

    protected:
    struct Node
    {
        int key;
    };
    Dispatch* dispatch;
};

#endif /*SYSTEM_H*/
