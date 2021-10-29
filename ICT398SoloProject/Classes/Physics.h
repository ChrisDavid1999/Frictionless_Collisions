#pragma once
#include "Singleton.h"
#include <reactphysics3d/reactphysics3d.h>
#include <GL/glut.h>
#include <glm/glm.hpp>

class Physics
{
public:
    bool Init();
    void DrawDebugLines();
    void Update(float dt);
    void DrawRigidbodies();
private:
    reactphysics3d::PhysicsCommon common;
    reactphysics3d::PhysicsWorld* world = nullptr;
    reactphysics3d::RigidBody* testSubject = nullptr;
    reactphysics3d::RigidBody* testSubject2 = nullptr;
};

typedef singleton<Physics> PhysicsManager;
