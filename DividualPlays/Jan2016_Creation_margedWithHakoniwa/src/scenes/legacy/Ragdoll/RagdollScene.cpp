// 
// RagdollScene.cpp - RAMDanceToolkit
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
 Ragdoll Demo
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

#include "RagdollScene.h"
#include "LinearMath/btIDebugDraw.h"
#include "BulletDynamics/Dynamics/btDynamicsWorld.h"

#include "BulletDynamics/ConstraintSolver/btPoint2PointConstraint.h"//picking
#include "BulletDynamics/ConstraintSolver/btGeneric6DofConstraint.h"//picking

#include "BulletCollision/CollisionShapes/btCollisionShape.h"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "BulletCollision/CollisionShapes/btSphereShape.h"
#include "BulletCollision/CollisionShapes/btCompoundShape.h"
#include "BulletCollision/CollisionShapes/btUniformScalingShape.h"
#include "BulletDynamics/ConstraintSolver/btConstraintSolver.h"
#include "RagdollShapeDrawer.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btSerializer.h"

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btIDebugDraw.h"

#include "RagDoll.h"

#ifdef TARGET_WIN32
#include <glut.h>
#elif TARGET_MAC
#include <GLUT/GLUT.h>
#endif

/// Ragdoll staffs

// Enrico: Shouldn't these three variables be real constants and not defines?

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923
#endif

#ifndef M_PI_4
#define M_PI_4     0.785398163397448309616
#endif

static const float GRAVITY_SCALE = 100.0f;



void RagdollScene::initPhysics()
{
	// Setup the basic world
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
    
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    
	btVector3 worldAabbMin(-10000,-10000,-10000);
	btVector3 worldAabbMax(10000,10000,10000);
	m_broadphase = new btAxisSweep3 (worldAabbMin, worldAabbMax);
    
	m_solver = new btSequentialImpulseConstraintSolver;
    
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_broadphase,m_solver,m_collisionConfiguration);
    //m_dynamicsWorld->setGravity(btVector3(0.0f, -9.8f*GRAVITY_SCALE, 0.0f));
    m_dynamicsWorld->setGravity(btVector3(0.0f, -9.8f*GRAVITY_SCALE*.166666667, 0.0f));
	//m_dynamicsWorld->getDispatchInfo().m_useConvexConservativeDistanceUtil = true;
	//m_dynamicsWorld->getDispatchInfo().m_convexConservativeDistanceThreshold = 0.01f;
    

	// Setup a big ground box
	{
		//btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(1.5),btScalar(0.1),btScalar(1.5)));
        
		btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(300.0f),
                                                                 btScalar(10.0f),
                                                                 btScalar(300.0f)));
        m_collisionShapes.push_back(groundShape);
		btTransform groundTransform;
		groundTransform.setIdentity();
		//groundTransform.setOrigin(btVector3(0,-10,0));
        groundTransform.setOrigin(btVector3(0,-5.0f,0));
        
		btCollisionObject* fixedGround = new btCollisionObject();
		fixedGround->setCollisionShape(groundShape);
		fixedGround->setWorldTransform(groundTransform);
		m_dynamicsWorld->addCollisionObject(fixedGround);   
	}
    
	// Spawn one ragdoll
	btVector3 startOffset(0,50,0);
	spawnRagdoll(startOffset);
	clientResetScene();
}

void RagdollScene::spawnRagdoll(const btVector3& startOffset)
{
	RagDoll* ragDoll = new RagDoll (m_dynamicsWorld, startOffset);
	m_ragdolls.push_back(ragDoll);
}

void RagdollScene::update()
{
	//simple dynamics world doesn't handle fixed-time-stepping
	float ms = getDeltaTimeMicroseconds();
    
	float minFPS = 1000000.f/60.f;
	if (ms > minFPS)
		ms = minFPS;
    
	if (m_dynamicsWorld)
		m_dynamicsWorld->stepSimulation(ms / 1000000.f);
}

void RagdollScene::draw()
{
    GLfloat light_ambient[] = { btScalar(0.2), btScalar(0.2), btScalar(0.2), btScalar(1.0) };
	GLfloat light_diffuse[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0) };
	GLfloat light_specular[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0 )};
	/*	light_position is NOT default value	*/
	GLfloat light_position0[] = { btScalar(1.0), btScalar(10.0), btScalar(1.0), btScalar(0.0 )};
	GLfloat light_position1[] = { btScalar(-1.0), btScalar(-10.0), btScalar(-1.0), btScalar(0.0) };
    
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
    
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
    
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
    
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
    
	if (m_dynamicsWorld) {
        glClear(GL_STENCIL_BUFFER_BIT);
        glEnable(GL_CULL_FACE);
        renderscene(0);
        
        glDisable(GL_LIGHTING);
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glEnable(GL_STENCIL_TEST);
        glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
        glStencilFunc(GL_ALWAYS,1,0xFFFFFFFFL);
        glFrontFace(GL_CCW);
        glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
        renderscene(1);
        glFrontFace(GL_CW);
        glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
        renderscene(1);
        glFrontFace(GL_CCW);
        
        glPolygonMode(GL_FRONT,GL_FILL);
        glPolygonMode(GL_BACK,GL_FILL);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glEnable(GL_LIGHTING);
        glDepthMask(GL_TRUE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);
        glEnable(GL_CULL_FACE);
        glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
        
        glDepthFunc(GL_LEQUAL);
        glStencilFunc( GL_NOTEQUAL, 0, 0xFFFFFFFFL );
        glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
        glDisable(GL_LIGHTING);
        renderscene(2);
        glEnable(GL_LIGHTING);
        glDepthFunc(GL_LESS);
        glDisable(GL_STENCIL_TEST);
        glDisable(GL_CULL_FACE);
        
		glDisable(GL_LIGHTING);
	}
}

void RagdollScene::keyPressed(int key)
{
	switch (key) {
        case 'e': {
            btVector3 startOffset(ofRandom(-295,295),ofRandom(100, 1500),ofRandom(-295,295));
            spawnRagdoll(startOffset);
        }
    }
}

void RagdollScene::exitPhysics()
{
	for (int i=0;i<m_ragdolls.size();i++) {
		RagDoll* doll = m_ragdolls[i];
		delete doll;
	}
    
	//cleanup in the reverse order of creation/initialization
    
	//remove the rigidbodies from the dynamics world and delete them
	
	for (int i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--) {
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState()) {
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}
    
	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++) {
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
    
	//delete dynamics world
	delete m_dynamicsWorld;
    
	//delete solver
	delete m_solver;
    
	//delete broadphase
	delete m_broadphase;
    
	//delete dispatcher
	delete m_dispatcher;
    
	delete m_collisionConfiguration;
}

/// Basic staffs
const int maxNumObjects = 16384;
btTransform startTransforms[maxNumObjects];
btCollisionShape* gShapePtr[maxNumObjects];//1 rigidbody has 1 shape (no re-use of shapes)

RagdollScene::RagdollScene()
//see btIDebugDraw.h for modes
:
m_dynamicsWorld(0),
m_scaleBottom(0.5f),
m_scaleFactor(2.f),
m_stepping(true),
m_singleStep(false),
m_enableshadows(true),
m_sundirection(btVector3(1,-2,1)*1000),
m_defaultContactProcessingThreshold(BT_LARGE_FLOAT)
{
	m_shapeDrawer = new RagdollShapeDrawer();
	m_shapeDrawer->enableTexture(true);
}

RagdollScene::~RagdollScene()
{
    exitPhysics();
    
	if (m_shapeDrawer)
		delete m_shapeDrawer;
}

void RagdollScene::setup(void)
{    
    initPhysics();
}


//#define NUM_SPHERES_ON_DIAGONAL 9

btRigidBody* RagdollScene::localCreateRigidBody(float mass,
                                                   const btTransform& startTransform,
                                                   btCollisionShape* shape)
{
	btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));
    
	//rigidbody is dynamic if and only if mass is non zero, otherwise static
	bool isDynamic = (mass != 0.f);
    
	btVector3 localInertia(0,0,0);
	if (isDynamic)
		shape->calculateLocalInertia(mass,localInertia);
    
	//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
	btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
    
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,shape,localInertia);
    
	btRigidBody* body = new btRigidBody(cInfo);
	body->setContactProcessingThreshold(m_defaultContactProcessingThreshold);

    
	m_dynamicsWorld->addRigidBody(body);
    
	return body;
}

//
void RagdollScene::renderscene(int pass)
{
	btScalar	m[16];
	btMatrix3x3	rot;rot.setIdentity();
	const int	numObjects=m_dynamicsWorld->getNumCollisionObjects();
	btVector3 wireColor(1,0,0);
	for(int i=0;i<numObjects;i++) {
		btCollisionObject*	colObj=m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody*		body=btRigidBody::upcast(colObj);
		if(body&&body->getMotionState()) {
			btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
			myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
			rot=myMotionState->m_graphicsWorldTrans.getBasis();
		}
		else {
			colObj->getWorldTransform().getOpenGLMatrix(m);
			rot=colObj->getWorldTransform().getBasis();
		}
		btVector3 wireColor(1.f,1.0f,0.5f); //wants deactivation
		if(i&1) wireColor=btVector3(0.f,0.0f,1.f);
		///color differently for active, sleeping, wantsdeactivation states
        //active
		if (colObj->getActivationState() == 1) {
			if (i & 1)
				wireColor += btVector3 (1.f,0.f,0.f);
			else
				wireColor += btVector3 (.5f,0.f,0.f);
		}
        //ISLAND_SLEEPING
		if(colObj->getActivationState()==2) {
			if(i&1)
				wireColor += btVector3 (0.f,1.f, 0.f);
			else
				wireColor += btVector3 (0.f,0.5f,0.f);
		}
        
		btVector3 aabbMin,aabbMax;
		m_dynamicsWorld->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);
		
		aabbMin-=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
		aabbMax+=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
        
        switch(pass) {
			case 0:
                m_shapeDrawer->drawOpenGL(m,colObj->getCollisionShape(),wireColor);
                break;
			case 1:
                m_shapeDrawer->drawShadow(m,m_sundirection*rot,colObj->getCollisionShape());
                break;
			case 2:
                m_shapeDrawer->drawOpenGL(m,colObj->getCollisionShape(),wireColor*btScalar(0.3));
                break;
        }
	}
}

#include "BulletCollision/BroadphaseCollision/btAxisSweep3.h"


void RagdollScene::clientResetScene()
{
	int numObjects = 0;
    
	if (m_dynamicsWorld) {
		int numConstraints = m_dynamicsWorld->getNumConstraints();
		for (int i=0;i<numConstraints;i++)
			m_dynamicsWorld->getConstraint(0)->setEnabled(true);

		numObjects = m_dynamicsWorld->getNumCollisionObjects();
        
		///create a copy of the array, not a reference!
		btCollisionObjectArray copyArray = m_dynamicsWorld->getCollisionObjectArray();
        
		for (int i=0;i<numObjects;i++) {
			btCollisionObject* colObj = copyArray[i];
			btRigidBody* body = btRigidBody::upcast(colObj);
			if (body) {
				if (body->getMotionState()) {
					btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
					myMotionState->m_graphicsWorldTrans = myMotionState->m_startWorldTrans;
					body->setCenterOfMassTransform( myMotionState->m_graphicsWorldTrans );
					colObj->setInterpolationWorldTransform( myMotionState->m_startWorldTrans );
					colObj->forceActivationState(ACTIVE_TAG);
					colObj->activate();
					colObj->setDeactivationTime(0);
					//colObj->setActivationState(WANTS_DEACTIVATION);
				}
				//removed cached contact points (this is not necessary if all objects have been removed from the dynamics world)
				if (m_dynamicsWorld->getBroadphase()->getOverlappingPairCache())
					m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(colObj->getBroadphaseHandle(),getDynamicsWorld()->getDispatcher());
                
				btRigidBody* body = btRigidBody::upcast(colObj);
				if (body && !body->isStaticObject()) {
					btRigidBody::upcast(colObj)->setLinearVelocity(btVector3(0,0,0));
					btRigidBody::upcast(colObj)->setAngularVelocity(btVector3(0,0,0));
				}
			}    
		}
        
		///reset some internal cached data in the broadphase
		m_dynamicsWorld->getBroadphase()->resetPool(getDynamicsWorld()->getDispatcher());
		m_dynamicsWorld->getConstraintSolver()->reset();
	}
}


