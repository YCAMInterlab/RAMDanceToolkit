// 
// btPicker.cpp - RAMDanceToolkit
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
//  btPicker.cpp
//  DanceWirdtk::thRagdall2
//
//  Created by Onishi Yoshito on 1/24/13.
//
//

#include "btPicker.h"

#include "btBulletDynamicsCommon.h"

btPicker::btPicker()
:
m_world(NULL),
m_pickConstraint(NULL),
m_pickedBody(NULL)
{
}

btPicker::~btPicker()
{
    clear();
}

void btPicker::clear()
{
    if (m_pickConstraint && m_world)
    {
        m_world->removeConstraint(m_pickConstraint);
        delete m_pickConstraint;
        m_pickConstraint = NULL;
        //printf("removed constraint %i",gPickingConstraintId);
        m_pickedBody->forceActivationState(ACTIVE_TAG);
        m_pickedBody->setDeactivationTime( 0.f );
        m_pickedBody = NULL;
    }
}

void btPicker::setWorld(class btDynamicsWorld *world)
{
    m_world = world;
}

void btPicker::updatePosition(const btVector3 &position)
{
    if (m_pickConstraint) {
        if (m_pickConstraint->getConstraintType() == D6_CONSTRAINT_TYPE) {
            btGeneric6DofConstraint* pickCon = static_cast<btGeneric6DofConstraint*>(m_pickConstraint);
            if (pickCon) {
                pickCon->getFrameOffsetA().setOrigin(position);
                m_position = position;
            }
        }
    }
}

btVector3 btPicker::getPosition() const
{
    return m_position;
}

void btPicker::attatchRigidBody(class btRigidBody *body)
{
    clear();
    
    assert(body);
    assert(m_world);
    
    if (!m_world || !body) return;
    
    m_pickedBody = body;
    
    if (!(m_pickedBody->isStaticObject() || m_pickedBody->isKinematicObject())) {
        m_pickedBody->setActivationState(DISABLE_DEACTIVATION);
        
        //btVector3 pickPos = rayCallback.m_hitPointWorld;
        btVector3 pickPos = m_pickedBody->getCenterOfMassTransform().getOrigin();
        
        //printf("pickPos=%f,%f,%f\n",pickPos.getX(),pickPos.getY(),pickPos.getZ());
        
        btVector3 localPivot = m_pickedBody->getCenterOfMassTransform().inverse() * pickPos;
        
        btTransform tr;
        tr.setIdentity();
        tr.setOrigin(localPivot);
        btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*m_pickedBody, tr,false);
        dof6->setLinearLowerLimit(btVector3(0,0,0));
        dof6->setLinearUpperLimit(btVector3(0,0,0));
        dof6->setAngularLowerLimit(btVector3(0,0,0));
        dof6->setAngularUpperLimit(btVector3(0,0,0));
        
        m_world->addConstraint(dof6,true);
        m_pickConstraint = dof6;
        
        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,0);
        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,1);
        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,2);
        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,3);
        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,4);
        dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,5);
        
        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,0);
        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,1);
        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,2);
        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,3);
        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,4);
        dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,5);
        
        //PickingPos = rayTo;
        //gHitPos = pickPos;
        //
        //gOldPickingDist  = (pickPos-rayFrom).length();
    }
}
