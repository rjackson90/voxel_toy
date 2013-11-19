#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "math_ext.h"
#include "physics.h"

namespace Physics
{
    /* This struct is used internally in the RK4 integrator. It's fields are
     * calculated from State objects.
     */
    struct Derivative
    {
        Vector velocity;
        Vector force;
        Quaternion spin;
        Vector torque;
    };
    
    class Integrator
    {
    public:
        // This function is the interface to the RK4 integrator implementation
        static void integrate(State &, const std::vector<Vector> &, double);

    private:
        // This class cannot be instantiated
        Integrator() {}

        // These functions actually perform the integration
        static Derivative evaluate( State &, double, const Derivative &, const Vector &);
        static Derivative evaluate(const State &, const Vector &);
    };
}

typedef Physics::Integrator RK4;

#endif //INTEGRATOR_H
