// 
// ChainObject.cpp - RAMDanceToolkit
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
//  ChainObject.cpp
//  ConstrainTest
//
//  Created by Onishi Yoshito on 1/26/13.
//
//

#include "ChainObject.h"

#include "btBulletDynamicsCommon.h"

ChainObject::ChainObject(btDynamicsWorld *m_ownerWorld,
             const btVector3 &origin,
             int edges,
             float edgeLength,
             float edgeThickness)
:
BaseConstrains(m_ownerWorld),
m_thickness(4.0f)
{
    // Setup all the rigid bodies
    m_edgeLength = edgeLength;
    m_edgeThickness = edgeThickness;
    
    btTransform offset; offset.setIdentity();
    offset.setOrigin(origin);
    
    for (int i=0; i<edges; i++) {
        btTransform transform = createTransform(i);
        m_bodies.push_back(localCreateRigidBody(btScalar(1.0f),
                                                offset*transform,
                                                m_shapes.at(i)));
    }

    for (int i=0; i<edges-1; i++) {
        createConeTwistJoint(m_bodies.at(i),
                             m_bodies.at(i+1),
                             btVector3(0.0f, 0.0f, -m_edgeLength),
                             btVector3(0.0f, 0.0f,  m_edgeLength),
                             0, 0, 0,
                             0, 0, 0);
    }
}

ChainObject::~ChainObject()
{
    
}

btTransform ChainObject::createTransform(int i)
{
    const float R = m_thickness/2.0f;
    const float L = m_edgeLength/2.0f;
    
    btTransform transform;
    transform.setIdentity();
    
    m_shapes.push_back(new btBoxShape(btVector3(R, R, L)));
    transform.setOrigin(btVector3(0.0f, 0.0f, (L*2.0f)*i));
    
    return transform;
}
