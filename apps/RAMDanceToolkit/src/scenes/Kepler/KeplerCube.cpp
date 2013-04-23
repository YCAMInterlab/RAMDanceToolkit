// 
// KeplerCube.cpp - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

//
//  KeplerCube.cpp
//  KeplerCube
//
//  Created by Onishi Yoshito on 1/23/13.
//
//

#include "KeplerCube.h"

#include "btBulletDynamicsCommon.h"

#define CONSTRAINT_DEBUG_SIZE 0.2f

#define R_LENGTH (140)

const float KeplerCube::RAG_DALL_SCALE = 0.85f;

btTransform KeplerCube::createBoxTransform(int i)
{
    const float R = 0.7f;
    const float L = R_LENGTH/2.0f;
    
    btTransform transform;
    transform.setIdentity();
    
    switch (i) {
        case 0:
            m_shapes.push_back(new btBoxShape(btVector3(R,R,L)));
            transform.setOrigin(btVector3(-L, -L,  0));
            break;
        case 1:
            m_shapes.push_back(new btBoxShape(btVector3(L,R,R)));
            transform.setOrigin(btVector3( 0, -L, -L));
            break;
        case 2:
            m_shapes.push_back(new btBoxShape(btVector3(R,R,L)));
            transform.setOrigin(btVector3( L, -L,  0));
            break;
        case 3:
            m_shapes.push_back(new btBoxShape(btVector3(L,R,R)));
            transform.setOrigin(btVector3( 0, -L,  L));
            break;
        case 4:
            m_shapes.push_back(new btBoxShape(btVector3(R,L,R)));
            transform.setOrigin(btVector3(-L, 0, -L));
            break;
        case 5:
            m_shapes.push_back(new btBoxShape(btVector3(R,L,R)));
            transform.setOrigin(btVector3( L, 0, -L));
            break;
        case 6:
            m_shapes.push_back(new btBoxShape(btVector3(R,L,R)));
            transform.setOrigin(btVector3( L, 0,  L));
            break;
        case 7:
            m_shapes.push_back(new btBoxShape(btVector3(R,L,R)));
            transform.setOrigin(btVector3(-L, 0,  L));
            break;
        case 8:
            m_shapes.push_back(new btBoxShape(btVector3(R,R,L)));
            transform.setOrigin(btVector3(-L,  L,  0));
            break;
        case 9:
            m_shapes.push_back(new btBoxShape(btVector3(L,R,R)));
            transform.setOrigin(btVector3( 0,  L, -L));
            break;
        case 10:
            m_shapes.push_back(new btBoxShape(btVector3(R,R,L)));
            transform.setOrigin(btVector3( L,  L,  0));
            break;
        case 11:
            m_shapes.push_back(new btBoxShape(btVector3(L,R,R)));
            transform.setOrigin(btVector3( 0,  L,  L));
            break;
    }
    
    return transform;
}

void KeplerCube::createJoint(btRigidBody *bodyA,
                            btRigidBody *bodyB,
                            const btVector3 &originA,
                            const btVector3 &originB,
                            btScalar eulerXA,btScalar eulerYA,btScalar eulerZA,
                            btScalar eulerXB,btScalar eulerYB,btScalar eulerZB)
{
    btConeTwistConstraint *coneC;
    btTransform localA, localB;
    
    localA.setIdentity(); localB.setIdentity();
    localA.getBasis().setEulerZYX(eulerXA, eulerYA, eulerZA);
    localA.setOrigin(originA);
    localB.getBasis().setEulerZYX(eulerXB, eulerYB, eulerZB);
    localB.setOrigin(originB);
    coneC = new btConeTwistConstraint(*bodyA, *bodyB, localA, localB);
    //coneC->setLimit(M_PI_4, M_PI_4, M_PI_2);
    coneC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);
    
    m_ownerWorld->addConstraint(coneC, true);
    
    m_joints.push_back(coneC);
}


KeplerCube::KeplerCube(btDynamicsWorld* ownerWorld, const btVector3& positionOffset)
: m_ownerWorld (ownerWorld)
{
    //    const int R = 3.0f;
    const int L = R_LENGTH/2.0f;
    //
    //    const float d = L/2.0f;
    
    //    for (int i=0; i<N_EDGES; i++)
    //        m_shapes.push_back(new btBoxShape(btVector3());
    
    // Setup all the rigid bodies
    btTransform offset; offset.setIdentity();
    offset.setOrigin(positionOffset);
    
    for (int i=0; i<N_EDGES; i++) {
        btTransform transform = createBoxTransform(i);
        m_bodies.push_back(localCreateRigidBody(btScalar(1.0f), offset*transform, m_shapes.at(i)));
    }
    
    // Setup some damping on the m_bodies
    for (int i = 0; i < m_bodies.size(); ++i) {
        //        m_bodies.at(i)->setDamping(0.05, 0.85);
        //        m_bodies.at(i)->setDeactivationTime(0.8);
        //        m_bodies.at(i)->setSleepingThresholds(1.6, 2.5);
        
        //        m_bodies.at(i)->setActivationState(DISABLE_DEACTIVATION);
        //        m_bodies.at(i)->setCollisionFlags(m_bodies.at(i)->getCollisionFlags() |
        //                                          btCollisionObject::CF_KINEMATIC_OBJECT);
    }
    
    createJoint(m_bodies.at(0), m_bodies.at(1), btVector3( 0, 0,-L), btVector3(-L, 0, 0), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at(1), m_bodies.at(2), btVector3( L, 0, 0), btVector3( 0, 0,-L), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at(2), m_bodies.at(3), btVector3( 0, 0, L), btVector3( L, 0, 0), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at(3), m_bodies.at(0), btVector3(-L, 0, 0), btVector3( 0, 0, L), 0, 0, 0, 0, 0, 0);
    
    createJoint(m_bodies.at(4), m_bodies.at(0), btVector3( 0,-L, 0), btVector3( 0, 0,-L), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at(5), m_bodies.at(1), btVector3( 0,-L, 0), btVector3( L, 0, 0), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at(6), m_bodies.at(2), btVector3( 0,-L, 0), btVector3( 0, 0, L), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at(7), m_bodies.at(3), btVector3( 0,-L, 0), btVector3(-L, 0, 0), 0, 0, 0, 0, 0, 0);
    
    createJoint(m_bodies.at(4), m_bodies.at( 8), btVector3( 0, L, 0), btVector3( 0, 0,-L), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at(5), m_bodies.at( 9), btVector3( 0, L, 0), btVector3( L, 0, 0), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at(6), m_bodies.at(10), btVector3( 0, L, 0), btVector3( 0, 0, L), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at(7), m_bodies.at(11), btVector3( 0, L, 0), btVector3(-L, 0, 0), 0, 0, 0, 0, 0, 0);
    
    createJoint(m_bodies.at( 8), m_bodies.at( 9), btVector3( 0, 0,-L), btVector3(-L, 0, 0), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at( 9), m_bodies.at(10), btVector3( L, 0, 0), btVector3( 0, 0,-L), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at(10), m_bodies.at(11), btVector3( 0, 0, L), btVector3( L, 0, 0), 0, 0, 0, 0, 0, 0);
    createJoint(m_bodies.at(11), m_bodies.at( 8), btVector3(-L, 0, 0), btVector3( 0, 0, L), 0, 0, 0, 0, 0, 0);
}

KeplerCube::~KeplerCube()
{
    // Remove all constraints
    for (int i = 0; i <m_joints.size(); ++i) {
        m_ownerWorld->removeConstraint(m_joints.at(i));
        delete m_joints[i]; m_joints[i] = NULL;
    }
    
    // Remove all bodies and shapes
    for (int i = 0; i < m_bodies.size(); ++i) {
        m_ownerWorld->removeRigidBody(m_bodies.at(i));
        
        delete m_bodies[i]->getMotionState();
        
        delete m_bodies[i]; m_bodies[i] = NULL;
        delete m_shapes[i]; m_shapes[i] = NULL;
    }
}

btRigidBody* KeplerCube::localCreateRigidBody(btScalar mass,
                                             const btTransform& startTransform,
                                             btCollisionShape* shape)
{
    const bool isDynamic = (mass != 0.f);
    
    btVector3 localInertia(0,0,0);
    if (isDynamic)
        shape->calculateLocalInertia(mass,localInertia);
    
    btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    
    btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,shape,localInertia);
    btRigidBody* body = new btRigidBody(rbInfo);
    
    m_ownerWorld->addRigidBody(body);
    
    return body;
}
