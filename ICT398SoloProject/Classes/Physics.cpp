#include "Physics.h"

#include <iostream>
#include <ostream>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/quaternion.hpp>
# define M_PI           3.14159265358979323846
bool Physics::Init()
{
    events = new PhysicsEvent();
    world = common.createPhysicsWorld();
    world->setIsDebugRenderingEnabled(true);
    world->setEventListener(events);
    world->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
    world->setIsGravityEnabled(false);

    {
        solo::Rigidbody* object = new solo::Rigidbody(objects.size());
        reactphysics3d::Transform test;
        test.setPosition(rp3d::Vector3(1, 2, -3));
        object->scale = {0.25, 2, 2};
        object->rb = world->createRigidBody(test);
        object->rb->setUserData(object);
        reactphysics3d::CollisionShape* box = common.createBoxShape(rp3d::Vector3(object->scale.x/2, object->scale.y/2, object->scale.z/2));
        object->rb->addCollider(box, reactphysics3d::Transform().identity());
        objects.push_back(object);
    }

    {
        solo::Rigidbody* object = new solo::Rigidbody(objects.size());
        reactphysics3d::Transform test2;
        test2.setPosition(rp3d::Vector3(-1, 1.25, -3));
        object->rb = world->createRigidBody(test2);
        object->scale = {0.25, 2, 2};
        object->rb->setUserData(object);
        reactphysics3d::CollisionShape* box2 = common.createBoxShape(rp3d::Vector3(object->scale.x/2, object->scale.y/2, object->scale.z/2));
        object->rb->addCollider(box2, reactphysics3d::Transform().identity());
        objects.push_back(object);
    }

    {
        solo::Rigidbody* object = new solo::Rigidbody(objects.size());
        reactphysics3d::Transform test3;
        test3.setPosition(rp3d::Vector3(-3, 2, -3));
        object->rb = world->createRigidBody(test3);
        object->scale = {0.25, 0.25, 0.25};
        object->rb->setUserData(object);
        reactphysics3d::CollisionShape* sphere = common.createSphereShape(object->scale.x);
        object->rb->addCollider(sphere, reactphysics3d::Transform().identity());
        objects.push_back(object);
    }
    
    std::cout << "Physics go brrrrrrr" << std::endl;
    return world; 
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

    for(int i = 0; i < linesCount; i++)
    {
        rp3d::Vector3 point1 = lines[i].point1;
        rp3d::Vector3 point2 = lines[i].point2;
        glBegin(GL_LINE_STRIP);
            glPushMatrix();
            glColor3f(124/255, 252/255, 0/255);
                glVertex3f(point1.x, point1.y, point1.z);
                glVertex3f(point2.x, point2.y, point2.z);
            glPopMatrix();
        glEnd();
    }
}

void Physics::Update(float dt)
{
    //Update inertias

    //Process collisions (need to test where this best fits)

    //Process Rbs

    //Update Rbs

    //Might need to reset?
    
    world->update(dt);
}
//This helped with the quat rotations http://goldsequence.blogspot.com/2016/04/quaternion-based-rotation-in-opengl.html
void Physics::DrawRigidbodies()
{
    for(int i = 0; i < 3; i++)
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
        if(i != 2)
            glutSolidCube(1);
        else
        {
            glutSolidSphere(1, 10, 10);
            rp3d::Transform t = objects[i]->rb->getTransform();
            t.setPosition(rp3d::Vector3(t.getPosition().x + 0.1, t.getPosition().y, t.getPosition().z));
            objects[i]->rb->setTransform(t);
        }
        
        glPopMatrix();
    }
}

void Physics::AddCollision(CollisionInfo info)
{
    collisions.push_back(info);
}

void Physics::Collisions(float dt)
{
    for(CollisionInfo col : collisions)
    {
        int one = col.objectOne.id;
        int two = col.objectTwo.id;

        //rp3d::Transform transformOne = objects[one]->rb->getTransform();

        //Get Initial values
        glm::vec3 linearVelocityOne = objects[one]->linearVelocity;
        glm::vec3 linearVelocityTwo = objects[two]->linearVelocity;
        glm::vec3 angularVelocityOne = objects[one]->angularVelocity;
        glm::vec3 angularVelocityTwo = objects[two]->angularVelocity;

        glm::vec3 contactPositionOne = Math::ToVec3(col.objectOne.contact);
        glm::vec3 contactPositionTwo = Math::ToVec3(col.objectTwo.contact);

        glm::vec3 contactNormal = Math::ToVec3(col.normal);

        glm::vec3 rigidbodyOne = contactPositionOne - Math::ToVec3(objects[one]->rb->getTransform().getPosition());
        glm::vec3 rigidbodyTwo = contactPositionTwo - Math::ToVec3(objects[two]->rb->getTransform().getPosition());

        //Math time
    }
}

void Physics::IntertiaTensors(float dt)
{
    
}

void Physics::ProcessRigidbodies(float dt)
{
    
}

void Physics::UpdateRigidbodies(float dt)
{
    
}




