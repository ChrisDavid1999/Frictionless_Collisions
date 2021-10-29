#pragma once
#include "Singleton.h"
#include "Rigidbody.h"
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
    solo::Rigidbody testSubject;
    solo::Rigidbody testSubject2;
    solo::Rigidbody testSubject3;
};
typedef singleton<Physics> PhysicsManager;
