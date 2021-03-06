#include "Physics.h"

#include <iostream>
#include <ostream>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

#include "../imgui/imgui.h"
# define M_PI           3.14159265358979323846

bool Physics::Init()
{
    events = new PhysicsEvent();
    world = common.createPhysicsWorld();
    world->setIsDebugRenderingEnabled(true);
    world->setEventListener(events);
    world->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
    world->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_POINT, true);
    world->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::CONTACT_NORMAL, true);
    world->setIsGravityEnabled(false);
    
    std::cout << "Physics go brrrrrrr" << std::endl;
    return world; 
}

void Physics::LoadFile()
{
    {
        solo::Rigidbody* object = new solo::Rigidbody(objects.size());
        reactphysics3d::Transform test;
        test.setPosition(rp3d::Vector3(1, 1, -3));
        object->scale = {0.1, 2, 1};
        object->rb = world->createRigidBody(test);
        object->rb->setUserData(object);
        reactphysics3d::CollisionShape* box = common.createBoxShape(rp3d::Vector3(object->scale.x/2, object->scale.y/2, object->scale.z/2));
        object->AddCollisionShape(solo::Collider::type::box, box, 1);
        objects.push_back(object);
    }

    {
        solo::Rigidbody* object = new solo::Rigidbody(objects.size());
        reactphysics3d::Transform test2;
        test2.setPosition(rp3d::Vector3(0.1, 2.5, -3));
        object->rb = world->createRigidBody(test2);
        object->scale = {0.1, 2, 2};
        object->rb->setUserData(object);
        reactphysics3d::CollisionShape* box2 = common.createBoxShape(rp3d::Vector3(object->scale.x/2, object->scale.y/2, object->scale.z/2));
        object->AddCollisionShape(solo::Collider::type::box, box2, 10);
        objects.push_back(object);
    }

    {
        solo::Rigidbody* object = new solo::Rigidbody(objects.size());
        reactphysics3d::Transform test3;
        test3.setPosition(rp3d::Vector3(5, 0, -3.2));
        object->rb = world->createRigidBody(test3);
        object->scale = {0.25, 0.25, 0.25};
        object->rb->setUserData(object);
        reactphysics3d::CollisionShape* box3 = common.createBoxShape(rp3d::Vector3(object->scale.x/2, object->scale.y/2, object->scale.z/2));
        object->AddCollisionShape(solo::Collider::type::box, box3, 10);
        object->linearVelocity = {-5, 0, 0};
        objects.push_back(object);
    }
}

void Physics::DrawDebugLines()
{
    auto triangles = world->getDebugRenderer().getTriangles();
    auto triCount = world->getDebugRenderer().getNbTriangles();
    auto lines = world->getDebugRenderer().getLines();
    auto linesCount = world->getDebugRenderer().getNbLines();
    glLineWidth(2.0f);
    for(int i = 0; i < triCount; i++)
    {
        rp3d::Vector3 point1 = triangles[i].point1;
        rp3d::Vector3 point2 = triangles[i].point2;
        rp3d::Vector3 point3 = triangles[i].point3;
        
        glBegin(GL_LINE_STRIP);
            glPushMatrix();
                glColor3f(255/255, 0/255, 255/255);
                glVertex3f(point1.x, point1.y, point1.z);
                glVertex3f(point2.x, point2.y, point2.z);
                glVertex3f(point3.x, point3.y, point3.z);
            glPopMatrix();
        glEnd();
    }
    glLineWidth(2.0f);
    for(int i = 0; i < linesCount; i++)
    {
        rp3d::Vector3 point1 = lines[i].point1;
        rp3d::Vector3 point2 = lines[i].point2;
        glBegin(GL_LINE_STRIP);
            glPushMatrix();
                glColor3f(255/255, 100/255, 155/255);
                glVertex3f(point1.x, point1.y, point1.z);
                glVertex3f(point2.x, point2.y, point2.z);
            glPopMatrix();
        glEnd();
    }
}

void Physics::Update(float dt)
{
    //Update inertias
    IntertiaTensors(dt);
    //Process Rbs
    ProcessRigidbodies(dt);
    //Update Rbs
    UpdateRigidbodies(dt);
    //Process collisions (need to test where this best fits)
    Collisions(dt);
    //Might need to reset?
    world->update(dt);
}

//This helped with the quat rotations http://goldsequence.blogspot.com/2016/04/quaternion-based-rotation-in-opengl.html
void Physics::DrawRigidbodies()
{
    for(int i = 0; i < objects.size(); i++)
    {
        glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(objects[i]->rb->getTransform().getPosition().x, objects[i]->rb->getTransform().getPosition().y, objects[i]->rb->getTransform().getPosition().z);
        auto q = objects[i]->rb->getTransform().getOrientation();
    
        float norm = glm::sqrt(q.w*q.w + q.x*q.x + q.y*q.y + q.z*q.z);

        float q0 = q.w / norm;
        float q1 = q.x / norm;
        float q2 = q.y / norm;
        float q3 = q.z / norm;
        float theta = glm::acos(q0) * 2;
        float aNorm = glm::sqrt(q.x*q.x + q.y*q.y + q.z*q.z);
    
        if (aNorm != 0)
        {
            glRotatef(theta*180.0f/M_PI, q1/aNorm, q2/aNorm, q3/aNorm);
        }
        else
        {
            glRotatef(theta*180.0f/M_PI, q1, q2, q3);
        }
        glScalef(objects[i]->scale.x, objects[i]->scale.y, objects[i]->scale.z);
        
        glutSolidCube(1);
        /*else
        {
            glutSolidSphere(1, 10, 10);
        }*/
        
        glPopMatrix();
    }
}

void Physics::AddCollision(CollisionInfo info)
{
    collisions.push(info);
}

void Physics::Collisions(float dt)
{
    int i = 0;
    while(!collisions.empty())
    {
        CollisionInfo col = collisions.front();
        int one = col.objectOne.id;
        int two = col.objectTwo.id;
        glm::vec3 linearVelocityOne = objects[one]->linearVelocity;
        glm::vec3 angularVelocityOne = objects[one]->angularVelocity;
        glm::vec3 contactPositionOne = Math::ToVec3(col.objectTwo.contact);
        
        glm::vec3 angularVelocityTwo = objects[two]->angularVelocity;
        glm::vec3 linearVelocityTwo = objects[two]->linearVelocity;
        glm::vec3 contactPositionTwo = Math::ToVec3(col.objectTwo.contact);

        glm::vec3 contactNormal = Math::ToVec3(col.normal);
        
        /*
        objects[one]->rb->setTransform(transformOne);
        objects[two]->rb->setTransform(transformTwo);
        */

        glm::vec3 rigidbodyOne = contactPositionOne - Math::ToVec3(objects[one]->rb->getTransform().getPosition());
        glm::vec3 rigidbodyTwo = contactPositionTwo - Math::ToVec3(objects[two]->rb->getTransform().getPosition());
        
        //Math time
        float restitution = -(1 + 0.4);//Need to check this out

        glm::vec3 velocityDifference = linearVelocityOne - linearVelocityTwo;

        glm::vec3 rigidbodyCrossNormalOne = glm::cross(rigidbodyOne, contactNormal);
        glm::vec3 rigidbodyCrossNormalTwo = glm::cross(rigidbodyTwo, contactNormal);
        
        //Numerator (top part of big boy equation)
        float dotVelocity = glm::dot(contactNormal, velocityDifference);
        float dotAngular = glm::dot(angularVelocityOne, rigidbodyCrossNormalOne) - glm::dot(angularVelocityTwo, rigidbodyCrossNormalTwo);
        float dotVelocityAngular = dotVelocity + dotAngular;
        float numerator = restitution * dotVelocityAngular;

        //Denominator
        float totalMass = objects[one]->mass.inverse + objects[two]->mass.inverse;
        /*glm::vec3 inertia = (rigidbodyOne * objects[one]->inertiaTensor.WorldInverseInertiaTensor * rigidbodyOne) +
            (rigidbodyTwo * objects[two]->inertiaTensor.WorldInverseInertiaTensor * rigidbodyTwo);
        glm::vec3 denominatorVec = totalMass + inertia;*/
        
        float inertiaFinal = glm::dot((rigidbodyOne * objects[one]->inertiaTensor.WorldInverseInertiaTensor * rigidbodyOne),
            (rigidbodyTwo * objects[two]->inertiaTensor.WorldInverseInertiaTensor * rigidbodyTwo));
        float denominator = totalMass + inertiaFinal;

        //Lambda + impulse
        float lambda = numerator/denominator;
        glm::vec3 impulse = lambda * contactNormal;
        //Set values one
        if(lambda < 0)
        {
            linearVelocityOne += impulse * objects[one]->mass.inverse;
            angularVelocityOne += (lambda * objects[one]->inertiaTensor.WorldInverseInertiaTensor * rigidbodyCrossNormalOne);
            //Set values two
            linearVelocityTwo -= impulse * objects[two]->mass.inverse;
            angularVelocityTwo -= (lambda * objects[two]->inertiaTensor.WorldInverseInertiaTensor * rigidbodyCrossNormalTwo);
        }
        objects[one]->linearVelocity = linearVelocityOne;
        objects[one]->angularVelocity = angularVelocityOne;
            objects[two]->linearVelocity = linearVelocityTwo;
            objects[two]->angularVelocity = angularVelocityTwo;   
        
            collisions.pop();
    }
}

void Physics::IntertiaTensors(float dt)
{
    for(int i = 0; i < objects.size(); i++)
    {
        glm::mat3 rot = glm::mat3_cast(Math::ToQuat(objects[i]->rb->getTransform().getOrientation()));
        objects[i]->inertiaTensor.WorldInverseInertiaTensor[0][0] = rot[0][0] * objects[i]->inertiaTensor.LocalInverseInertiaTensor.x;
        objects[i]->inertiaTensor.WorldInverseInertiaTensor[0][1] = rot[1][0] * objects[i]->inertiaTensor.LocalInverseInertiaTensor.x;
        objects[i]->inertiaTensor.WorldInverseInertiaTensor[0][2] = rot[2][0] * objects[i]->inertiaTensor.LocalInverseInertiaTensor.x;
	
        objects[i]->inertiaTensor.WorldInverseInertiaTensor[1][0] = rot[0][1] * objects[i]->inertiaTensor.LocalInverseInertiaTensor.y;
        objects[i]->inertiaTensor.WorldInverseInertiaTensor[1][1] = rot[1][1] * objects[i]->inertiaTensor.LocalInverseInertiaTensor.y;
        objects[i]->inertiaTensor.WorldInverseInertiaTensor[1][2] = rot[2][1] * objects[i]->inertiaTensor.LocalInverseInertiaTensor.y;
	
        objects[i]->inertiaTensor.WorldInverseInertiaTensor[2][0] = rot[0][2] * objects[i]->inertiaTensor.LocalInverseInertiaTensor.z;
        objects[i]->inertiaTensor.WorldInverseInertiaTensor[2][1] = rot[1][2] * objects[i]->inertiaTensor.LocalInverseInertiaTensor.z;
        objects[i]->inertiaTensor.WorldInverseInertiaTensor[2][2] = rot[2][2] * objects[i]->inertiaTensor.LocalInverseInertiaTensor.z;

        objects[i]->inertiaTensor.WorldInverseInertiaTensor = rot * objects[i]->inertiaTensor.WorldInverseInertiaTensor;
    }
}

void Physics::ProcessRigidbodies(float dt)
{
    for(int i = 0; i < objects.size(); i++)
    {
        objects[i]->linearVelocity += dt * objects[i]->mass.inverse * objects[i]->forces;
        objects[i]->angularVelocity += dt * objects[i]->inertiaTensor.WorldInertiaTensor * objects[i]->torques;
        //objects[i]->angularVelocity += dt * objects[i]->inertiaTensor.WorldInertiaTensor * objects[i]->torques;
        //std::cout << "AV: " << objects[i]->angularVelocity.x << " " << objects[i]->angularVelocity.y << " " << objects[i]->angularVelocity.z << std::endl;
        
        //Get important values
        rp3d::Transform objectTransform = objects[i]->rb->getTransform();
        rp3d::Vector3 position = objectTransform.getPosition();
        rp3d::Quaternion orientation = objectTransform.getOrientation();
        
        //Update them
        position += Math::ToVector3(objects[i]->linearVelocity * dt);
        glm::quat o = Math::ToQuat(orientation) * glm::quat(objects[i]->angularVelocity * dt);
        orientation = Math::ToQuaternion(o);
        //orientation = Math::ToQuaternion(glm::normalize(o));
        
        //Pass them back in
        objectTransform.setPosition(position);
        objectTransform.setOrientation(orientation);
        objects[i]->rb->setTransform(objectTransform);
    }
}

void Physics::UpdateRigidbodies(float dt)
{
    for(int i = 0; i < objects.size(); i++)
    {
        objects[i]->Update();
    }
}

void Physics::ImGuiRender()
{
    for(int i = 0; i < objects.size(); i++)
    {

    }
}






