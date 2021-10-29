#pragma once
#include "PhysicsEvent.h"
#include <glm/glm.hpp>
namespace solo
{
    struct Rigidbody
    {
        Rigidbody() = default;
        Rigidbody(std::shared_ptr<int> i) : id(i)
        {
            rb = nullptr;
        }
        
        //Change this to collision body when actually doing it
        std::shared_ptr<int> id;
        reactphysics3d::CollisionBody* rb;
        glm::vec3 scale = {1, 1, 1};

        //Rigidbody components
    };
}

