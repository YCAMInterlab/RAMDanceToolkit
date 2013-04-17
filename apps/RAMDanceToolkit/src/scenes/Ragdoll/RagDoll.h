// 
// RagDoll.h - RAMDanceToolkit
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
//  Ragdoll.h
//  Ragdoll
//
//  Created by Onishi Yoshito on 1/23/13.
//
//

#ifndef __Ragdoll__Ragdoll__
#define __Ragdoll__Ragdoll__

#include "LinearMath/btScalar.h"

class btDynamicsWorld;
class btCollisionShape;
class btRigidBody;
class btTypedConstraint;
class btVector3;
class btTransform;

class RagDoll
{
public:
	RagDoll (btDynamicsWorld* ownerWorld, const btVector3& positionOffset);
	virtual	~RagDoll();
        
	enum
	{
		BODYPART_PELVIS = 0,
		BODYPART_SPINE,
		BODYPART_HEAD,
        
		BODYPART_LEFT_UPPER_LEG,
		BODYPART_LEFT_LOWER_LEG,
        
		BODYPART_RIGHT_UPPER_LEG,
		BODYPART_RIGHT_LOWER_LEG,
        
		BODYPART_LEFT_UPPER_ARM,
		BODYPART_LEFT_LOWER_ARM,
        
		BODYPART_RIGHT_UPPER_ARM,
		BODYPART_RIGHT_LOWER_ARM,
        
		BODYPART_COUNT
	};
    
	enum
	{
		JOINT_PELVIS_SPINE = 0,
		JOINT_SPINE_HEAD,
        
		JOINT_LEFT_HIP,
		JOINT_LEFT_KNEE,
        
		JOINT_RIGHT_HIP,
		JOINT_RIGHT_KNEE,
        
		JOINT_LEFT_SHOULDER,
		JOINT_LEFT_ELBOW,
        
		JOINT_RIGHT_SHOULDER,
		JOINT_RIGHT_ELBOW,
        
		JOINT_COUNT
	};
    

	btDynamicsWorld* m_ownerWorld;
	btCollisionShape* m_shapes[BODYPART_COUNT];
	btRigidBody* m_bodies[BODYPART_COUNT];
	btTypedConstraint* m_joints[JOINT_COUNT];

private:
	btRigidBody* localCreateRigidBody(btScalar mass,
                                      const btTransform& startTransform,
                                      btCollisionShape* shape);
    static const float RAG_DALL_SCALE;
    
};

#endif /* defined(__        __Ragdoll__) */
