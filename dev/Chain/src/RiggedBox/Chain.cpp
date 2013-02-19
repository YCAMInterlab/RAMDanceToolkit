//
//  Chain.cpp
//  ConstrainTest
//
//  Created by Onishi Yoshito on 1/26/13.
//
//

#include "Chain.h"

#include "btBulletDynamicsCommon.h"

static const int N_EDGES = 60;

Chain::Chain(btDynamicsWorld *m_ownerWorld, const btVector3 &origin)
:
BaseConstrains(m_ownerWorld),
m_length(50.0f),
m_thickness(4.0f)
{
    const int L = m_length/2.0f;
    
    // Setup all the rigid bodies
    btTransform offset; offset.setIdentity();
    offset.setOrigin(origin);
    
    for (int i=0; i<N_EDGES; i++) {
        btTransform transform = createTransform(i);
        m_bodies.push_back(localCreateRigidBody(btScalar(1.0f),
                                                offset*transform,
                                                m_shapes.at(i)));
    }

    for (int i=0; i<N_EDGES-1; i++) {
        
        createConeTwistJoint(m_bodies.at(i),
                             m_bodies.at(i+1),
                             btVector3(0.0f, 0.0f, -L),
                             btVector3(0.0f, 0.0f,  L),
                             0, 0, 0,
                             0, 0, 0);
    }
}

Chain::~Chain()
{
    
}

btTransform Chain::createTransform(int i)
{
    const float R = m_thickness/2.0f;
    const float L = m_length/2.0f;
    
    btTransform transform;
    transform.setIdentity();
    
    m_shapes.push_back(new btBoxShape(btVector3(R, R, L)));
    transform.setOrigin(btVector3(0.0f, 0.0f, (L*2.0f)*i));
    
    return transform;
}
