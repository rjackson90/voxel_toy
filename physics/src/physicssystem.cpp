#include "physicssystem.h"

using namespace Physics;

/* This is where the magic happens. During each subsystem tick the physics
 * simulation is advanced by dt
 */
void PhysicsSystem::tick(__attribute__((unused)) const SubsystemsPtr &systems, const double dt)
{
    // Advance the physics simulation by dt
    for(auto it = nodes.begin(); it != nodes.end(); ++it)
    {
        // Swap State buffers
        it->second.past = it->second.present;

        // Apply impulses before integrating forces
        for(const Vector &impulse : it->second.impulses)
        {
            it->second.present.momentum += impulse;
        }
        it->second.impulses.clear();

        // Integrate forces
        RK4::integrate(it->second.present, it->second.forces, dt);
    }
}

/* This method adds a new node to the physics system. Because it
 * has no past as far as the simulation is concerned, the initial state
 * occupies both state buffers in the RigidBodyNode
 */
void PhysicsSystem::addNode(int key, State &initial)
{
    initial.recalculate();
    
    RigidBodyNode newNode;
    newNode.key = key;
    newNode.past = initial;
    newNode.present = initial;

    nodes.insert({{key, newNode}});
}

// This method parses the config file located at the provided filesystem path
// and creates a new RigidBodyNode from the data
void PhysicsSystem::addNodeFromConfig(const Core::ConfigParser &parser)
{
    Physics::State state;
    int nodeID = -1;
    bool status = false;
    
    // Eat exceptions. Node is created outside critical section
    try
    {
        // Get the NodeID from the Meta section
        nodeID = stoi(parser.get("nodeID", "Meta"));

        // Fill the state with values from the file
        state.orientation = Quaternion::from_string(
                parser.get("orientation", "RigidBodyNode"));
        state.position = Vector::from_string(
                parser.get("position", "RigidBodyNode"));
        state.momentum = Vector::from_string(
                parser.get("momentum", "RigidBodyNode"));
        state.angular_momentum = Vector::from_string(
                parser.get("angular_momentum","RigidBodyNode"));
        
        state.inertia_tensor = mat3_from_string(
                parser.get("inertia_tensor", "RigidBodyNode"));
        state.inverse_inertia_tensor = mat3_from_string(
                parser.get("inverse_inertia_tensor", "RigidBodyNode"));
        
        state.mass = (float) stof(parser.get("mass", "RigidBodyNode"));
        state.inverse_mass = (float) stof(parser.get("inverse_mass", "RigidBodyNode"));

        // Read all data, set status flag to "good"
        status = true;

    }
    catch(const std::exception &ex)
    {
        std::cerr << "Error while parsing config file " << ex.what() 
                  << std::endl;
    }

    // Create a RigidBodyNode from the newly filled state
    if(status) this->addNode(nodeID, state);
}

void PhysicsSystem::addForce(int node, const Vector &force)
{
    nodes[node].forces.push_back(force);
}

void PhysicsSystem::clearForces(int node)
{
    nodes[node].forces.clear();
}

void PhysicsSystem::addDeltaV(int node, const Vector &dV)
{
    nodes[node].impulses.push_back(dV * nodes[node].present.mass);
}

Vector PhysicsSystem::getStopDeltaV(int node)
{
    Vector stop = nodes[node].present.momentum * -1.0f / nodes[node].present.mass;
    return stop;
}

/* This method is critical for interop between subsystems. 
 * The render system calls this method to obtain the position and 
 * orientation of objects in world-space.
 */
glm::mat4 PhysicsSystem::getWorldCoords(int key)
{
    return nodes[key].present.world_coords;
}
