#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
namespace solo
{
    struct Rigidbody
    {
        //Change this to collision body when actually doing it
        reactphysics3d::RigidBody* rb;
        glm::vec3 scale = {1, 1, 1};

        //Rigidbody components
    };
}

