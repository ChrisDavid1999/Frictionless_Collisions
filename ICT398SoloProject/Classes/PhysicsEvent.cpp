#include "PhysicsEvent.h"

void PhysicsEvent::onTrigger(const reactphysics3d::OverlapCallback::CallbackData&)
{

}

void PhysicsEvent::onContact(const CallbackData& callbackData)
{
    for(int i = 0; i < callbackData.getNbContactPairs(); i++)
    {
        ContactPair contactPair = callbackData.getContactPair(i);
        for(int x = 0; x < contactPair.getNbContactPoints(); x++)
        {
            CollisionInfo contacts;
            ContactPoint contactPoint = contactPair.getContactPoint(x);

            //Normal
            contacts.normal = contactPoint.getWorldNormal();
            
            //Objects contact points
            contacts.objectOne.contact = contactPair.getCollider1()->getLocalToBodyTransform() * contactPoint.getLocalPointOnCollider1();
            contacts.objectTwo.contact = contactPair.getCollider2()->getLocalToBodyTransform() * contactPoint.getLocalPointOnCollider2();

            //Penetration
            contacts.penetration = contactPoint.getPenetrationDepth();

            std::cout << reinterpret_cast<int*>(contactPair.getBody1()->getUserData()) << " " << reinterpret_cast<int*>(contactPair.getBody2()->getUserData()) << std::endl;
        }
    }
}

