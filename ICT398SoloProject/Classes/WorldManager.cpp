#include "WorldManager.h"

#include <iostream>
#include <ostream>

#include "GL/glut.h"

bool WorldManager::Init()
{
    std::cout << "World go brrrr" << std::endl;
    return PhysicsManager::getInstance().Init();
}


void WorldManager::Update()
{
    if(deltaTime > 0)
        PhysicsManager::getInstance().Update(deltaTime);
}

void WorldManager::RenderUpdate()
{
    PhysicsManager::getInstance().DrawDebugLines();
    PhysicsManager::getInstance().DrawRigidbodies();
}

void WorldManager::DetlaTimeUpdate(float time)
{
    deltaTime = (time - oldTime) / 1000;
    oldTime = time;
}
