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
    std::cout << "Physics go brrrrrrr" << std::endl;
    
    world = common.createPhysicsWorld();
    world->setIsDebugRenderingEnabled(true);
    world->getDebugRenderer().setIsDebugItemDisplayed(reactphysics3d::DebugRenderer::DebugItem::COLLISION_SHAPE, true);
    world->setIsGravityEnabled(false);
    
    reactphysics3d::Transform test;
    test.setPosition(rp3d::Vector3(1, 2, -3));
    testSubject.scale = {0.25, 2, 2};
    testSubject.rb = world->createRigidBody(test);
    reactphysics3d::CollisionShape* box = common.createBoxShape(rp3d::Vector3(testSubject.scale.x/2, testSubject.scale.y/2, testSubject.scale.z/2));
    testSubject.rb->addCollider(box, reactphysics3d::Transform().identity());

    reactphysics3d::Transform test2;
    test2.setPosition(rp3d::Vector3(-1, 1.25, -3));
    testSubject2.rb = world->createRigidBody(test2);
    testSubject2.scale = {0.25, 2, 2};
    reactphysics3d::CollisionShape* box2 = common.createBoxShape(rp3d::Vector3(testSubject2.scale.x/2, testSubject2.scale.y/2, testSubject2.scale.z/2));
    testSubject2.rb->addCollider(box2, reactphysics3d::Transform().identity());

    reactphysics3d::Transform test3;
    test3.setPosition(rp3d::Vector3(-3, 2, -3));
    testSubject3.rb = world->createRigidBody(test3);
    testSubject3.scale = {0.25, 0.25, 0.25};
    reactphysics3d::CollisionShape* sphere = common.createSphereShape(testSubject3.scale.x);
    testSubject3.rb->addCollider(sphere, reactphysics3d::Transform().identity());
    testSubject3.rb->applyLocalForceAtLocalPosition(rp3d::Vector3(10, 0, 0), testSubject3.rb->getTransform().getPosition());
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
    world->update(dt);
}
//This helped with the quat rotations http://goldsequence.blogspot.com/2016/04/quaternion-based-rotation-in-opengl.html
void Physics::DrawRigidbodies()
{
    {
        glPushMatrix();
        glColor3f(1, 1, 1);
        glTranslatef(testSubject.rb->getTransform().getPosition().x, testSubject.rb->getTransform().getPosition().y, testSubject.rb->getTransform().getPosition().z);
        auto q = testSubject.rb->getTransform().getOrientation();
    
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
        glScalef(testSubject.scale.x, testSubject.scale.y, testSubject.scale.z);
        glutSolidCube(1);
        glPopMatrix();
    }
    
    {
        glPushMatrix();
        glColor3f(0.5, 0.75, 1);
        glTranslatef(testSubject2.rb->getTransform().getPosition().x, testSubject2.rb->getTransform().getPosition().y, testSubject2.rb->getTransform().getPosition().z);
        auto q = testSubject2.rb->getTransform().getOrientation();
        
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
        glScalef(testSubject2.scale.x, testSubject2.scale.y, testSubject2.scale.z);
        glutSolidCube(1);
        glPopMatrix();
    }

    {
        glPushMatrix();
        glColor3f(0.25, 1, 0.5);
        glTranslatef(testSubject3.rb->getTransform().getPosition().x, testSubject3.rb->getTransform().getPosition().y, testSubject3.rb->getTransform().getPosition().z);
        auto q = testSubject3.rb->getTransform().getOrientation();
        
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
        glScalef(testSubject3.scale.x, testSubject3.scale.y, testSubject3.scale.z);
        glutSolidSphere(1, 10, 10);
        glPopMatrix();
    }
}
