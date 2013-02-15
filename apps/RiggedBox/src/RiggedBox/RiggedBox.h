//
//  RiggedBox.h
//  RiggedBox
//
//  Created by Onishi Yoshito on 1/23/13.
//
//

#ifndef __RiggedBox__RiggedBox__
#define __RiggedBox__RiggedBox__

#include "LinearMath/btScalar.h"
#include "btAlignedObjectArray.h"

class btDynamicsWorld;
class btCollisionShape;
class btRigidBody;
class btTypedConstraint;
class btVector3;
class btTransform;

class RiggedBox
{
public:
	RiggedBox (btDynamicsWorld* ownerWorld, const btVector3& positionOffset);
	virtual	~RiggedBox();
    
    static const int N_EDGES = 12;
    
	btDynamicsWorld* m_ownerWorld;
	btAlignedObjectArray<btCollisionShape *>    m_shapes;
	btAlignedObjectArray<btRigidBody *>         m_bodies;
	btAlignedObjectArray<btTypedConstraint *>   m_joints;
    
private:
    btTransform createBoxTransform(int i);
    void createJoint(btRigidBody *bodyA,
                     btRigidBody *bodyB,
                     const btVector3 &originA,
                     const btVector3 &originB,
                     btScalar eulerXA,btScalar eulerYA,btScalar eulerZA,
                     btScalar eulerXB,btScalar eulerYB,btScalar eulerZB);
    
	btRigidBody* localCreateRigidBody(btScalar mass,
                                      const btTransform& startTransform,
                                      btCollisionShape* shape);
    static const float RAG_DALL_SCALE;
    
};

#endif /* defined(__        __RiggedBox__) */
