#pragma once
#include <queue>
#include <vector>
#include "PhysicsEvent.h"
#include "Singleton.h"

#include <GL/glut.h>
#include <glm/glm.hpp>

class Physics
{
public:
    bool Init();
    void LoadFile(std::string file);
    void DrawDebugLines();
    void Update(float dt);
    void DrawRigidbodies();
    void AddCollision(CollisionInfo);

    //Main functions
    void ProcessRigidbodies(float dt);
    void UpdateRigidbodies(float dt);
    void Collisions(float dt);
    void IntertiaTensors(float dt);

    void ImGuiRender();
private:
    reactphysics3d::PhysicsCommon common;
    reactphysics3d::PhysicsWorld* world = nullptr;
    std::vector<solo::Rigidbody*> objects;
    PhysicsEvent * events = nullptr;
    std::queue<CollisionInfo> collisions;
    
};
typedef singleton<Physics> PhysicsManager;
