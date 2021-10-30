#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
namespace Math
{
    static rp3d::Vector3 ToVector3(const glm::vec3& values)
    {
        return rp3d::Vector3(values.x, values.y, values.z);
    }

    static glm::vec3 ToVec3(const rp3d::Vector3& values)
    {
        return glm::vec3(values.x, values.y, values.z);
    }
}

