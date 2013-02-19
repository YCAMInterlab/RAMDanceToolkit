//
//  Rail.cpp
//  ConstrainTest
//
//  Created by Onishi Yoshito on 1/26/13.
//
//

#include "Rail.h"

#include "btBulletDynamicsCommon.h"

static const int N_EDGES = 23;

Rail::Rail(btDynamicsWorld *m_ownerWorld, const btVector3 &origin)
:
BaseConstrains(m_ownerWorld),
m_length(20.0f),
m_thickness(20.0f)
{
    btTransform offset; offset.setIdentity();
    offset.setOrigin(origin);
    
    for (int i=0; i<N_EDGES; i++) {
        btTransform transform = createTransform(i);
        m_initialTransforms.push_back(offset*transform);
        m_bodies.push_back(localCreateRigidBody(btScalar(1.0f),
                                                offset*transform,
                                                m_shapes.at(i)));
    }
    
    for (int i = 0; i < m_bodies.size(); ++i) {
        btGeneric6DofConstraint *dof6 =
        create6DofJoint(m_bodies.at(i),
                        btVector3(0.0f, 0.0f, 0.0f),
                        btVector3(0.0f, 0.0f, 0.0f),
                        btVector3(-500.0f, 0.0f, 0.0f),
                        btVector3( 500.0f, 0.0f, 0.0f));
        
        dof6->getTranslationalLimitMotor()->m_enableMotor[0] = true;
        dof6->getTranslationalLimitMotor()->m_targetVelocity[0] = -5.0f;
		dof6->getTranslationalLimitMotor()->m_maxMotorForce[0] = 0.1f;

    }
}

Rail::~Rail()
{    
}

void Rail::draw()
{
    for (int i=0; i<m_joints.size(); i++) {
        btGeneric6DofConstraint *dof6 = static_cast<btGeneric6DofConstraint *>(m_joints.at(i));
        
        btVector3 lower, upper;
        dof6->getLinearLowerLimit(lower);
        dof6->getLinearUpperLimit(upper);

        btVector3 pos = m_initialTransforms.at(i).getOrigin();
        ofPushMatrix();
        ofTranslate(pos.x(), pos.y(), pos.z());
        ofLine(lower.x(), lower.y(), lower.z(), upper.x(), upper.y(), upper.z());
        ofPopMatrix();
    }
}

btTransform Rail::createTransform(int i)
{
    const float R = m_thickness/2.0f;
    const float L = m_length/2.0f;
    
    btTransform transform;
    transform.setIdentity();
    
    m_shapes.push_back(new btBoxShape(btVector3(R, R, L)));
    transform.setOrigin(btVector3(0.0f, (L*2.0f)*i, 0.0f));
    
    return transform;
}
