// 
// BaseConstrains.cpp - RAMDanceToolkit
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
//  BaseConstrains.cpp
//  BaseConstrains
//
//  Created by Onishi Yoshito on 1/23/13.
//
//

#include "BaseConstrains.h"

#include "btBulletDynamicsCommon.h"

BaseConstrains::~BaseConstrains()
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

btConeTwistConstraint *BaseConstrains::createConeTwistJoint(btRigidBody *bodyA,
                                                            btRigidBody *bodyB,
                                                            const btVector3 &originA,
                                                            const btVector3 &originB,
                                                            btScalar eulerXA,btScalar eulerYA,btScalar eulerZA,
                                                            btScalar eulerXB,btScalar eulerYB,btScalar eulerZB)
{
    btTransform localA, localB;
    
    localA.setIdentity(); localB.setIdentity();
    localA.getBasis().setEulerZYX(eulerXA, eulerYA, eulerZA);
    localA.setOrigin(originA);
    localB.getBasis().setEulerZYX(eulerXB, eulerYB, eulerZB);
    localB.setOrigin(originB);
    btConeTwistConstraint *conC = new btConeTwistConstraint(*bodyA, *bodyB, localA, localB);
    //conC->setLimit(M_PI_4, M_PI_4, M_PI_2);
    //conC->setDbgDrawSize(CONSTRAINT_DEBUG_SIZE);
    
    conC->setParam(BT_CONSTRAINT_CFM, 0.9f, 0);
    conC->setParam(BT_CONSTRAINT_CFM, 0.9f, 1);
    conC->setParam(BT_CONSTRAINT_CFM, 0.9f, 2);
    conC->setParam(BT_CONSTRAINT_CFM, 0.9f, 3);
    conC->setParam(BT_CONSTRAINT_CFM, 0.9f, 4);
    conC->setParam(BT_CONSTRAINT_CFM, 0.9f, 5);
    conC->setParam(BT_CONSTRAINT_ERP, 0.1f, 0);
    conC->setParam(BT_CONSTRAINT_ERP, 0.1f, 1);
    conC->setParam(BT_CONSTRAINT_ERP, 0.1f, 2);
    conC->setParam(BT_CONSTRAINT_ERP, 0.1f, 3);
    conC->setParam(BT_CONSTRAINT_ERP, 0.1f, 4);
    conC->setParam(BT_CONSTRAINT_ERP, 0.1f, 5);
    m_ownerWorld->addConstraint(conC, true);
    
    m_joints.push_back(conC);
    
    return conC;
}

btGeneric6DofConstraint *BaseConstrains::create6DofJoint(btRigidBody *body,
                                                         const btVector3 &angularLowerLimit,
                                                         const btVector3 &angularUpperLimit,
                                                         const btVector3 &linearLowerLimit,
                                                         const btVector3 &linearUpperLimit)
{
    // 6DOF connected to the world, with motor
    body->setActivationState(DISABLE_DEACTIVATION);
    btTransform frameB;
    frameB.setIdentity();
    btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*body, frameB, false);
    m_ownerWorld->addConstraint(dof6);
    
    dof6->setAngularLowerLimit(angularLowerLimit);
    dof6->setAngularUpperLimit(angularUpperLimit);
    dof6->setLinearLowerLimit(linearLowerLimit);
    dof6->setLinearUpperLimit(linearUpperLimit);
    
    m_joints.push_back(dof6);
    
    return dof6;
    
}

btRigidBody* BaseConstrains::localCreateRigidBody(btScalar mass,
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
