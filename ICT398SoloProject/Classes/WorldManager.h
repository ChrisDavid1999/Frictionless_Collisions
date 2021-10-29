#pragma once
#include "Physics.h"

class WorldManager
{
public:
    
    void DetlaTimeUpdate(float time);
    void Update();
    void RenderUpdate();
    bool Init();

private:
    float deltaTime = 0.1;
    float oldTime = 0;
};

typedef singleton<WorldManager> Manager;