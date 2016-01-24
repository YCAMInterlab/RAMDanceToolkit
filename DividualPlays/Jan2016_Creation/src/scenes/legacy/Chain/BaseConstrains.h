// 
// BaseConstrains.h - RAMDanceToolkit
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
//  BaseConstrains.h
//  BaseConstrains
//
//  Created by Onishi Yoshito on 1/23/13.
//
//

#ifndef __BaseConstrains__BaseConstrains__
#define __BaseConstrains__BaseConstrains__

#include "LinearMath/btScalar.h"
#include "LinearMath/btAlignedObjectArray.h"

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
