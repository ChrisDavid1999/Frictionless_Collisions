#pragma once
#include <deque>
#include <vector>
#include "PhysicsEvent.h"
#include "Singleton.h"

#include <GL/glut.h>
#include <glm/glm.hpp>

class Physics
{
public:
    bool Init();
    void DrawDebugLines();
    void Update(float dt);
    void DrawRigidbodies();
    void AddCollision(CollisionInfo);

    //Main functions
    void ProcessRigidbodies(float dt);
    void UpdateRigidbodies(float dt);
    void Collisions(float dt);
    void IntertiaTensors(float dt);
    
private:
    reactphysics3d::PhysicsCommon common;
    reactphysics3d::PhysicsWorld* world = nullptr;
    std::vector<solo::Rigidbody*> objects;
    PhysicsEvent * events = nullptr;
    std::deque<CollisionInfo> collisions;
    
};
typedef singleton<Physics> PhysicsManager;
