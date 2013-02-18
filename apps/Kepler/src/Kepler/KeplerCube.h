//
//  KeplerCube.h
//  KeplerCube
//
//  Created by Onishi Yoshito on 1/23/13.
//
//

#ifndef __KeplerCube__KeplerCube__
#define __KeplerCube__KeplerCube__

#include "LinearMath/btScalar.h"
#include "LinearMath/btAlignedObjectArray.h"

class btDynamicsWorld;
class btCollisionShape;
class btRigidBody;
class btTypedConstraint;
class btVector3;
class btTransform;

class KeplerCube
{
public:
    enum EdgeType {
        EDGE_0  = 0,
        EDGE_1,
        EDGE_2,
        EDGE_3,
        
        EDGE_4,
        EDGE_5,
        EDGE_6,
        EDGE_7,
        
        EDGE_8,
        EDGE_9,
        EDGE_10,
        EDGE_11,
        
        N_EDGES,
    };
    
	KeplerCube (btDynamicsWorld* ownerWorld, const btVector3& positionOffset);
	virtual	~KeplerCube();
        
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

#endif /* defined(__        __KeplerCube__) */
