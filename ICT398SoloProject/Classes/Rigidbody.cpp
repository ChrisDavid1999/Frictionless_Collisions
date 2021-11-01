#include "Rigidbody.h"

#include <glm/gtx/norm.inl>

namespace solo
{
    void Rigidbody::AddCollisionShape(Collider::type shape, reactphysics3d::CollisionShape* col)
    {
        rb->addCollider(col, reactphysics3d::Transform().identity());
        colliders.push_back(Collider(shape, col, rb->getTransform()));
    }

    void Collider::Box(reactphysics3d::CollisionShape* shape)
    {
        rp3d::BoxShape* box = static_cast<rp3d::BoxShape*>(shape);
        collider = shape;
        const glm::vec3 fullExtents = Math::ToVec3(box->getHalfExtents() * 2.0f);

        //Volume of a cube/box: V = length * width * height
        volume =  fullExtents.x * fullExtents.y * fullExtents.z;
        mass.SetMass(density * volume);

        //Generate a inertia tensor for the shape
        float factor = 1.0f / 3.0f * mass.value;
        //Todo Check if we really should be using half or full extents
        auto x = box->getHalfExtents().x * box->getHalfExtents().x;
        auto y = box->getHalfExtents().y * box->getHalfExtents().y;
        auto z = box->getHalfExtents().z * box->getHalfExtents().z;
        auto tensor = glm::vec3(factor * (y + z), factor * (x + z), factor * (x + y));
        inertiaTensor.LocalInertiaTensor = tensor;
    }
    
    void Collider::Sphere(reactphysics3d::CollisionShape* shape)
    {
        rp3d::SphereShape* sphere = static_cast<rp3d::SphereShape*>(shape);

        //Volume of a Sphere: V = (4/3)πr3
        volume = (4/3) * glm::pi<float>() * glm::pow(sphere->getRadius(), 3);
        mass.SetMass(density * volume);

        //Generate a inertia tensor for the shape
        float diag = 0.4f * mass.value * glm::pow(sphere->getRadius(),2);
        auto tensor = glm::vec3(diag, diag, diag);
        inertiaTensor.LocalInertiaTensor = tensor;
    }

    void Rigidbody::Update()
    {
        const auto oldWorldCOM = mass.worldCenter;
        //const auto localCOM = CalculateCentreOfMass();
        const glm::vec3 worldCOM = (Math::ToQuat(rb->getTransform().getOrientation()) * localCOM) + Math::ToVec3(rb->getTransform().getPosition());
        mass.worldCenter = worldCOM;

        //auto localCOM = mass.localCenter;
        glm::mat3 tempLocalInertiaTensor = glm::mat3(1.0f);

        // calc inertia tensor using colliders
        for(Collider col : colliders)
        {
            glm::vec3 shapeInertiaTensor = col.inertiaTensor.LocalInertiaTensor;
            auto rot = glm::mat3_cast(Math::ToQuat(col.transform.getOrientation()));
            auto rotTranspose = glm::transpose(rot);
            rotTranspose[0] *= shapeInertiaTensor.x;
            rotTranspose[1] *= shapeInertiaTensor.y;
            rotTranspose[2] *= shapeInertiaTensor.z;
            glm::mat3 inertiaTensor = rot * rotTranspose;

            // shift to the rigidbody COM with parallel axis theorem
            glm::vec3 offset = Math::ToVec3(col.transform.getPosition()) - localCOM;
            float offsetSqr = glm::length2(offset);
            glm::mat3 offsetMatrix = glm::mat3(0.0f);
            offsetMatrix[0][0] = offsetSqr;
            offsetMatrix[1][1] = offsetSqr;
            offsetMatrix[2][2] = offsetSqr;

            offsetMatrix[0] += offset * (-offset.x);
            offsetMatrix[1] += offset * (-offset.y);
            offsetMatrix[2] += offset * (-offset.z);
            offsetMatrix *= col.mass.value;

            tempLocalInertiaTensor += inertiaTensor + offsetMatrix;
        }

        float x = tempLocalInertiaTensor[0][0];
        float y = tempLocalInertiaTensor[1][1];
        float z = tempLocalInertiaTensor[2][2];
        inertiaTensor.SetLocalInertiaTensor({x,y,z});
    }


}
