//
//  BaseConstrains.h
//  BaseConstrains
//
//  Created by Onishi Yoshito on 1/23/13.
//
//

#ifndef __BaseConstrains__BaseConstrains__
#define __BaseConstrains__BaseConstrains__

#include "LinearMath/btScalar.h"
#include "btAlignedObjectArray.h"

class btDynamicsWorld;
class btCollisionShape;
class btRigidBody;
class btTypedConstraint;
class btVector3;
class btTransform;
class btGeneric6DofConstraint;
class btConeTwistConstraint;

class BaseConstrains
{
public:
    //override this
	BaseConstrains(btDynamicsWorld* ownerWorld) : m_ownerWorld(ownerWorld) {}
    
	virtual	~BaseConstrains();
    
    virtual void draw() {}
    
	btDynamicsWorld* m_ownerWorld;
	btAlignedObjectArray<btCollisionShape *>    m_shapes;
	btAlignedObjectArray<btRigidBody *>         m_bodies;
	btAlignedObjectArray<btTypedConstraint *>   m_joints;
    
protected:
    //
    btConeTwistConstraint *createConeTwistJoint(btRigidBody *bodyA,
                                                btRigidBody *bodyB,
                                                const btVector3 &originA,
                                                const btVector3 &originB,
                                                btScalar eulerXA,btScalar eulerYA,btScalar eulerZA,
                                                btScalar eulerXB,btScalar eulerYB,btScalar eulerZB);
    
    btGeneric6DofConstraint *create6DofJoint(btRigidBody *body,
                                             const btVector3 &angularLowerLimit,
                                             const btVector3 &angularUpperLimit,
                                             const btVector3 &linearLowerLimit,
                                             const btVector3 &linearUpperLimit);
    
	btRigidBody* localCreateRigidBody(btScalar mass,
                                      const btTransform& startTransform,
                                      btCollisionShape* shape);
    
};

#endif /* defined(__        __BaseConstrains__) */
