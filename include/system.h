#ifndef SYSTEM_H
#define SYSTEM_H

#include <list>

class System
{
    public:
    virtual void tick(const double) = 0;

    protected:
    struct Node
    {
        int key;
    };
};

#endif /*SYSTEM_H*/
