// 
// KeplerBtDynamics.h - RAMDanceToolkit
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

/*
 Bullet Continuous Collision Detection and Physics Library
 Copyright (c) 2003-2006 Erwin Coumans  http://continuousphysics.com/Bullet/
 
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the use of this software.
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 */

/*
 Bullet Continuous Collision Detection and Physics Library
 Kepler Demo
 Copyright (c) 2007 Starbreeze Studios
 
 This software is provided 'as-is', without any express or implied warranty.
 In no event will the authors be held liable for any damages arising from the use of this software.
 Permission is granted to anyone to use this software for any purpose,
 including commercial applications, and to alter it and redistribute it freely,
 subject to the following restrictions:
 
 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
 3. This notice may not be removed or altered from any source distribution.
 
 Written by: Marten Svanfeldt
 */

#pragma once

#include "ofMain.h"

//#include "GlutStuff.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


#include "LinearMath/btVector3.h"
#include "LinearMath/btMatrix3x3.h"
#include "LinearMath/btTransform.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btAlignedObjectArray.h"

class	btCollisionShape;
class	btDynamicsWorld;
class	btRigidBody;
class	btTypedConstraint;
class   btBroadphaseInterface;
class   btCollisionShape;
class   btOverlappingPairCache;
class   btCollisionDispatcher;
class   btConstraintSolver;
struct  btCollisionAlgorithmCreateFunc;
class   btDefaultCollisionConfiguration;

class   KeplerBtShapeDrawer;


class KeplerBtDynamics
{
public:    
	void spawnKepler(const btVector3& startOffset);
    
    virtual void setup();
    virtual void update();
	virtual void draw();
    virtual void keyPressed(int key);
    
public:
	KeplerBtDynamics();
	virtual ~KeplerBtDynamics();
    
	inline btDynamicsWorld *getDynamicsWorld();
    
	inline btScalar getDeltaTimeMicroseconds();
    
    inline btAlignedObjectArray<class KeplerCube *> &getKepleres();
    
protected:
    virtual void clientResetScene();
    
	btRigidBody* localCreateRigidBody(float mass, const btTransform& startTransform,btCollisionShape* shape);

    void initPhysics();
	void exitPhysics();
    
    void renderscene(int pass);
    
    btClock             m_clock;
    
	///this is the most important class
	btDynamicsWorld     *m_dynamicsWorld;

	float               m_scaleBottom;
	float               m_scaleFactor;
	
	bool                m_stepping;
	bool                m_singleStep;
   
	KeplerBtShapeDrawer  *m_shapeDrawer;
	bool                m_enableshadows;
	btVector3           m_sundirection;
	btScalar            m_defaultContactProcessingThreshold;
    
    
    //Kepler scene
    btAlignedObjectArray<class KeplerCube *>       m_boxes;
    
	//keep the collision shapes, for deletion/cleanup
	btAlignedObjectArray<btCollisionShape *>    m_collisionShapes;
    
	btBroadphaseInterface   *m_broadphase;
    
	btCollisionDispatcher   *m_dispatcher;
    
	btConstraintSolver      *m_solver;
    
	btDefaultCollisionConfiguration *m_collisionConfiguration;
    
    struct MyInfo
    {
        bool isGround;
    } m_groundInfo;
};

inline btAlignedObjectArray<class KeplerCube *> &KeplerBtDynamics::getKepleres()
{
    return m_boxes;
}

inline btDynamicsWorld *KeplerBtDynamics::getDynamicsWorld()
{
    return m_dynamicsWorld;
}

inline btScalar KeplerBtDynamics::getDeltaTimeMicroseconds()
{
    btScalar dt = (btScalar)m_clock.getTimeMicroseconds();
    m_clock.reset();
    return dt;
}

