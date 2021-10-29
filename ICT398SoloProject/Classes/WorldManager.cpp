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
    glColor3f(0.9f, 0.9f, 0.9f);
    
    glBegin(GL_QUADS);
    glVertex3f(-100.0f, 0.0f, -100.0f);
    glVertex3f(-100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, 100.0f);
    glVertex3f(100.0f, 0.0f, -100.0f);
    glEnd();

    /*glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = -3; i < 3; i++)
        for (int j = -3; j < 3; j++) {
            glPushMatrix();
            glTranslatef(i * 10.0, 1, j * 10.0);
            glutSolidCube(1);
            glPopMatrix();
        }*/
    
    PhysicsManager::getInstance().DrawRigidbodies();
}

void WorldManager::DetlaTimeUpdate(float time)
{
    deltaTime = (time - oldTime) / 1000;
    oldTime = time;
}
