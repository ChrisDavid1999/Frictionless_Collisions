#include "PhysicsEvent.h"
#include "Physics.h"
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
            contacts.objectOne.contact = contactPair.getCollider1()->getLocalToWorldTransform()  * contactPoint.getLocalPointOnCollider1();
            contacts.objectTwo.contact = contactPair.getCollider2()->getLocalToWorldTransform()  * contactPoint.getLocalPointOnCollider2();

            //Penetration
            contacts.penetration = contactPoint.getPenetrationDepth();
            
            //Ids
            contacts.objectOne.id = static_cast<solo::Rigidbody*>(contactPair.getBody1()->getUserData())->id;
            contacts.objectTwo.id = static_cast<solo::Rigidbody*>(contactPair.getBody2()->getUserData())->id;

            PhysicsManager::getInstance().AddCollision(contacts);
            std::cout << "TCP" << contactPair.getNbContactPoints() <<  " NCP" << callbackData.getNbContactPairs() << std::endl;
            if(contactPair.getNbContactPoints() == 4)
                break;
        }
    }
}

