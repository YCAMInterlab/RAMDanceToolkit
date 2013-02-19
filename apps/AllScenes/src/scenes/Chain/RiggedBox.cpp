//
//  RiggedBox.cpp
//  ConstrainTest
//
//  Created by Onishi Yoshito on 1/26/13.
//
//

#include "RiggedBox.h"

#include "btBulletDynamicsCommon.h"

static const float R_LENGTH = 140.0f;
//static const float R_LENGTH = 300.0f;
static const int N_EDGES = 2;

btTransform RiggedBox::createTransform(int i)
{
    //const float R = 0.7f;
    const float R = 50.0f;
    //const float R = 5.0f;
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

RiggedBox::~RiggedBox()
{
    
}

RiggedBox::RiggedBox(btDynamicsWorld* ownerWorld, const btVector3& positionOffset)
: BaseConstrains(ownerWorld)
{
    const int L = R_LENGTH/2.0f;

    // Setup all the rigid bodies
    btTransform offset; offset.setIdentity();
    offset.setOrigin(positionOffset);
    
    for (int i=0; i<N_EDGES; i++) {
        btTransform transform = createTransform(i);
        m_bodies.push_back(localCreateRigidBody(btScalar(1.0f), offset*transform, m_shapes.at(i)));
    }
    
    // Setup some damping on the m_bodies
    for (int i = 0; i < m_bodies.size(); ++i) {
        //        m_bodies.at(i)->setDamping(0.05, 0.85);
        //        m_bodies.at(i)->setDeactivationTime(0.8);
        //        m_bodies.at(i)->setSleepingThresholds(1.6, 2.5);
    }
    
    createConeTwistJoint(m_bodies.at(0), m_bodies.at(1), btVector3( 0, 0,-L), btVector3(-L, 0, 0), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at(1), m_bodies.at(2), btVector3( L, 0, 0), btVector3( 0, 0,-L), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at(2), m_bodies.at(3), btVector3( 0, 0, L), btVector3( L, 0, 0), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at(3), m_bodies.at(0), btVector3(-L, 0, 0), btVector3( 0, 0, L), 0, 0, 0, 0, 0, 0);

    createConeTwistJoint(m_bodies.at(4), m_bodies.at(0), btVector3( 0,-L, 0), btVector3( 0, 0,-L), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at(5), m_bodies.at(1), btVector3( 0,-L, 0), btVector3( L, 0, 0), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at(6), m_bodies.at(2), btVector3( 0,-L, 0), btVector3( 0, 0, L), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at(7), m_bodies.at(3), btVector3( 0,-L, 0), btVector3(-L, 0, 0), 0, 0, 0, 0, 0, 0);

    createConeTwistJoint(m_bodies.at(4), m_bodies.at( 8), btVector3( 0, L, 0), btVector3( 0, 0,-L), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at(5), m_bodies.at( 9), btVector3( 0, L, 0), btVector3( L, 0, 0), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at(6), m_bodies.at(10), btVector3( 0, L, 0), btVector3( 0, 0, L), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at(7), m_bodies.at(11), btVector3( 0, L, 0), btVector3(-L, 0, 0), 0, 0, 0, 0, 0, 0);

    createConeTwistJoint(m_bodies.at( 8), m_bodies.at( 9), btVector3( 0, 0,-L), btVector3(-L, 0, 0), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at( 9), m_bodies.at(10), btVector3( L, 0, 0), btVector3( 0, 0,-L), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at(10), m_bodies.at(11), btVector3( 0, 0, L), btVector3( L, 0, 0), 0, 0, 0, 0, 0, 0);
    createConeTwistJoint(m_bodies.at(11), m_bodies.at( 8), btVector3(-L, 0, 0), btVector3( 0, 0, L), 0, 0, 0, 0, 0, 0);
}
