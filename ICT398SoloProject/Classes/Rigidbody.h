#pragma once
#include <glm/glm.hpp>
#include "Math.h"
namespace solo
{
    struct Mass
    {
        glm::vec3 localCenter = {0, 0, 0};
        glm::vec3 worldCenter = {0, 0, 0};
        float value = 1.0f;
        float inverse = 1.0f;
    };

    struct InertiaTensor
    {
        glm::mat3 WorldInertiaTensor = glm::mat3(1.0f);
        glm::mat3 WorldInverseInertiaTensor = glm::inverse(glm::mat3(1.0f));
        glm::vec3 LocalInertiaTensor = {1,1,1};
        glm::vec3 LocalInverseInertiaTensor = {1,1,1};

        glm::vec3 SetLocalInertiaTensor(const glm::vec3 localTensor)
        {
            LocalInertiaTensor = localTensor;
            LocalInverseInertiaTensor = glm::vec3(localTensor.x != 0.0f ? 1.0f / localTensor.x : 0.0f,
                                                    localTensor.y != 0.0f ? 1.0f / localTensor.y : 0.0f,
                                                    localTensor.z != 0.0f ? 1.0f / localTensor.z : 0.0f);
            return LocalInertiaTensor;
        }

        glm::mat3 SetWorldInertiaTensor(const glm::mat3& worldTensor)
        {
            WorldInertiaTensor = worldTensor;
            WorldInverseInertiaTensor = glm::inverse(worldTensor);
            return WorldInertiaTensor;
        }
    };
    
    struct Rigidbody
    {
        Rigidbody() = default;
        Rigidbody(int i) : id(i)
        {
            rb = nullptr;
        }
        
        //Change this to collision body when actually doing it
        int id;
        reactphysics3d::CollisionBody* rb;
        glm::vec3 scale = {1, 1, 1};

        //Rigidbody components
        Mass mass;
        glm::vec3 angularVelocity = {};
        glm::vec3 linearVelocity = {};
        glm::vec3 torques = {};
        glm::vec3 forces = {};
        InertiaTensor inertiaTensor;

        //collider stuff, might need ??
        /*std::unordered_map<uint32_t, Ref<Collider>> m_Colliders;

        uint32_t m_TotalColliders = 0;*/
    };
}

