#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "Math.h"
namespace solo
{
    struct Mass
    {
        glm::vec3 localCenter = {0, 0, 0};
        glm::vec3 worldCenter = {0, 0, 0};

        void SetMass(float mass = 1.0f)
        {
            value = mass;
            inverse = 1.0f / (value > 0.0f ? value : 1.0f); // cant invert mass if its 0
        }
        
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
        
    struct Collider
    {
        enum type { box, sphere };
        

        Collider(type t, reactphysics3d::CollisionShape* shape, rp3d::Transform trans)
        {
            collider = nullptr;
            transform = trans;
        }
        
        rp3d::CollisionShape * collider;

        void Box(reactphysics3d::CollisionShape* shape);
        void Sphere(reactphysics3d::CollisionShape* shape);
        Mass mass;
        float density;
        float volume;
        InertiaTensor inertiaTensor;
        rp3d::Transform transform;
    };
    
    struct Rigidbody
    {
        Rigidbody() = default;
        Rigidbody(int i) : id(i)
        {
            rb = nullptr;
        }

        void AddCollisionShape(Collider::type shape, reactphysics3d::CollisionShape*);
        void Update();
        
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
        std::vector<Collider> colliders;
        //collider stuff, might need ??
        /*std::unordered_map<uint32_t, Ref<Collider>> m_Colliders;

        uint32_t m_TotalColliders = 0;*/
    };
}

