#pragma once
#include <reactphysics3d/reactphysics3d.h>
class PhysicsEvent : public rp3d::EventListener
{
public:
    void onTrigger(const reactphysics3d::OverlapCallback::CallbackData&) override;
    void onContact(const CallbackData& callbackData) override;
};

struct CollisionObject
{
    rp3d::Vector3 contact;
    int id;
};

struct CollisionInfo
{
    CollisionObject objectOne;
    CollisionObject objectTwo;
    rp3d::Vector3 normal;
    float penetration;
};
