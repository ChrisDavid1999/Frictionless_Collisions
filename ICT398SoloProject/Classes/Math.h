#pragma once
#include <reactphysics3d/reactphysics3d.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
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

    static glm::quat ToQuat(const rp3d::Quaternion& values)
    {
        return glm::quat(values.w, values.x, values.y, values.z);
    }

    static rp3d::Quaternion ToQuaternion(const glm::quat& values)
    {
        return rp3d::Quaternion(values.x, values.y, values.z, values.w);
    }
}

