#pragma once
#include <vector>

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
    std::vector<solo::Rigidbody> objects;
    PhysicsEvent * events = nullptr;
    
};
typedef singleton<Physics> PhysicsManager;
