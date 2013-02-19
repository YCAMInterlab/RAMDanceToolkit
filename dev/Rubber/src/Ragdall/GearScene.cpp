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

#include "GearScene.h"
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
#include "GearShapeDrawer.h"
#include "LinearMath/btQuickprof.h"
#include "LinearMath/btDefaultMotionState.h"
#include "LinearMath/btSerializer.h"

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btIDebugDraw.h"

#include <GLUT/GLUT.h>

/// Gear staffs

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

const int numObjects = 3;

#define ENABLE_ALL_DEMOS 1

#define CUBE_HALF_EXTENTS 10.f

#define SIMD_PI_2 ((SIMD_PI)*0.5f)
#define SIMD_PI_4 ((SIMD_PI)*0.25f)




btTransform sliderTransform;
btVector3 lowerSliderLimit = btVector3(-10,0,0);
btVector3 hiSliderLimit = btVector3(10,0,0);

btRigidBody* d6body0 =0;

btHingeConstraint* spDoorHinge = NULL;
btHingeConstraint* spHingeDynAB = NULL;
btGeneric6DofConstraint* spSlider6Dof = NULL;

static bool s_bTestConeTwistMotor = false;

void drawLimit()
{
    btVector3 from = sliderTransform*lowerSliderLimit;
    btVector3 to = sliderTransform*hiSliderLimit;
    btVector3 color(255,0,0);
    glBegin(GL_LINES);
    glColor3f(color.getX(), color.getY(), color.getZ());
    glVertex3d(from.getX(), from.getY(), from.getZ());
    glVertex3d(to.getX(), to.getY(), to.getZ());
    if (d6body0)
    {
        from = d6body0->getWorldTransform().getOrigin();
        to = from + d6body0->getWorldTransform().getBasis() * btVector3(0,0,10);
        glVertex3d(from.getX(), from.getY(), from.getZ());
        glVertex3d(to.getX(), to.getY(), to.getZ());
    }
    glEnd();
}

void GearScene::setupEmptyDynamicsWorld()
{
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_overlappingPairCache = new btDbvtBroadphase();
	m_constraintSolver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher,m_overlappingPairCache,m_constraintSolver,m_collisionConfiguration);
    
}

void GearScene::addP2P(const btVector3 &boxSizeA, const btVector3 &boxSizeB, const btVector3 &posA, const btVector3 &posB)
{
    btCollisionShape* shapeA = new btBoxShape(boxSizeA);
    btCollisionShape* shapeB = new btBoxShape(boxSizeB);
    
    btTransform trans;
    float mass;
    trans.setIdentity();
    trans.setOrigin(posA);
    mass = 1.0f;
    btRigidBody *body0 = localCreateRigidBody(mass,trans,shapeA);
    trans.setOrigin(posB);
    mass = 1.0f;
    btRigidBody *body1 = localCreateRigidBody(mass,trans,shapeB);
    btVector3 pivotInA = posA;
    btVector3 pivotInB = posB;
    // btTypedConstraint* p2p = new btPoint2PointConstraint(*body0,pivotInA);
    btTypedConstraint *p2p = new btPoint2PointConstraint(*body0, *body1, pivotInA, pivotInB);
    m_dynamicsWorld->addConstraint(p2p);
    // p2p ->setBreakingImpulseThreshold(100.0);
    //        p2p->setDbgDrawSize(btScalar(5.f));
    
    P2P _p2p = { shapeA, shapeB, body0, body1, p2p };
    m_p2ps.push_back(_p2p);
}

void GearScene::initPhysics()
{
    //	setTexturing(true);
    //	setShadows(true);
    //
    //	setCameraDistance(26.f);
	m_Time = 0;
    
	setupEmptyDynamicsWorld();
    
    //	m_dynamicsWorld->setDebugDrawer(&gDebugDrawer);
    
    
	btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(3000.),btScalar(1000.),btScalar(3000.)));
	//btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),40);
    
	m_collisionShapes.push_back(groundShape);
	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin(btVector3(0,-1000.,0));
	m_groundBody= localCreateRigidBody(0, groundTransform, groundShape);
    
	btCollisionShape* shape = new btBoxShape(btVector3(CUBE_HALF_EXTENTS,CUBE_HALF_EXTENTS,CUBE_HALF_EXTENTS));
	m_collisionShapes.push_back(shape);
    
    //#define THETA SIMD_PI/4.f
    //#define L_1 (2 - tan(THETA))
    //#define L_2 (1 / cos(THETA))
    //#define RATIO L_2 / L_1
    
    addP2P(btVector3(5.0f, 5.0f, 5.0f),
           btVector3(3.0f, 3.0f, 3.0f),
           btVector3(-20.0f, 30.0f, 0.0f),
           btVector3(-20.0f, 60.0f, 0.0f));
    
    addP2P(btVector3(5.0f, 5.0f, 5.0f),
           btVector3(3.0f, 3.0f, 3.0f),
           btVector3(20.0f, 30.0f, 0.0f),
           btVector3(20.0f, 60.0f, 0.0f));
    
}

void GearScene::update()
{
    float dt = float(getDeltaTimeMicroseconds()) * 0.000001f;
	//printf("dt = %f: ",dt);
    
	// drive cone-twist motor
	m_Time += 0.03f;
	if (s_bTestConeTwistMotor)
	{  // this works only for obsolete constraint solver for now
		// build cone target
		btScalar t = 1.25f*m_Time;
		btVector3 axis(0,sin(t),cos(t));
		axis.normalize();
		btQuaternion q1(axis, 0.75f*SIMD_PI);
        
		// build twist target
		//btQuaternion q2(0,0,0);
		//btQuaternion q2(btVehictor3(1,0,0), -0.3*sin(m_Time));
		btQuaternion q2(btVector3(1,0,0), -1.49f*btSin(1.5f*m_Time));
        
		// compose cone + twist and set target
		q1 = q1 * q2;
		m_ctc->enableMotor(true);
		m_ctc->setMotorTargetInConstraintSpace(q1);
	}
    
	{
		static bool once = true;
		if ( m_dynamicsWorld->getDebugDrawer() && once)
		{
			m_dynamicsWorld->getDebugDrawer()->setDebugMode(btIDebugDraw::DBG_DrawConstraints+btIDebugDraw::DBG_DrawConstraintLimits);
			once=false;
		}
	}
    
	
	{
	 	//during idle mode, just run 1 simulation step maximum
        int maxSimSubSteps = 1;
        //		int maxSimSubSteps = m_idle ? 1 : 1;
        //		if (m_idle)
        //			dt = 1.0f/420.f;
        
		int numSimSteps = m_dynamicsWorld->stepSimulation(dt,maxSimSubSteps);
        
		//optional but useful: debug drawing
		m_dynamicsWorld->debugDrawWorld();
        
		bool verbose = false;
		if (verbose)
		{
			if (!numSimSteps)
				printf("Interpolated transforms\n");
			else
			{
				if (numSimSteps > maxSimSubSteps)
				{
					//detect dropping frames
					printf("Dropped (%i) simulation steps out of %i\n",numSimSteps - maxSimSubSteps,numSimSteps);
				} else
				{
					printf("Simulated (%i) steps\n",numSimSteps);
				}
			}
		}
	}
}

void GearScene::draw()
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

void GearScene::keyPressed(int key)
{
}

void GearScene::exitPhysics()
{
    m_p2ps.clear();
    
    int i;
    
	//removed/delete constraints
	for (i=m_dynamicsWorld->getNumConstraints()-1; i>=0 ;i--)
	{
		btTypedConstraint* constraint = m_dynamicsWorld->getConstraint(i);
		m_dynamicsWorld->removeConstraint(constraint);
		delete constraint;
	}
	m_ctc = NULL;
    
	//remove the rigidbodies from the dynamics world and delete them
	for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}
    
    
    
    
	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
    
	m_collisionShapes.clear();
    
	//delete dynamics world
	delete m_dynamicsWorld;
    
	//delete solver
	delete m_constraintSolver;
    
	//delete broadphase
	delete m_overlappingPairCache;
    
	//delete dispatcher
	delete m_dispatcher;
    
	delete m_collisionConfiguration;
    
}

/// Basic staffs
const int maxNumObjects = 16384;
btTransform startTransforms[maxNumObjects];
btCollisionShape* gShapePtr[maxNumObjects];//1 rigidbody has 1 shape (no re-use of shapes)

GearScene::GearScene()
//see btIDebugDraw.h for modes
:
m_dynamicsWorld(0),
m_sundirection(btVector3(1,-2,1)*1000),
m_defaultContactProcessingThreshold(BT_LARGE_FLOAT)
{
	m_shapeDrawer = new GearShapeDrawer();
	m_shapeDrawer->enableTexture(true);
}

GearScene::~GearScene()
{
    exitPhysics();
    
	if (m_shapeDrawer)
		delete m_shapeDrawer;
}

void GearScene::setup(void)
{
    initPhysics();
}


//#define NUM_SPHERES_ON_DIAGONAL 9

btRigidBody* GearScene::localCreateRigidBody(float mass,
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
void GearScene::renderscene(int pass)
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
		//btVector3 wireColor(1.f,1.0f,0.5f); //wants deactivation
        btVector3 wireColor(0.55f,0.55f,0.5f); //wants deactivation
		//if(i&1) wireColor=btVector3(0.f,0.0f,1.f);
        if(i&1) wireColor=btVector3(0.5f,0.55f,0.55f);
		///color differently for active, sleeping, wantsdeactivation states
        //active
		if (colObj->getActivationState() == 1) {
			if (i & 1)
				//wireColor += btVector3 (1.f,0.f,0.f);
                wireColor -= btVector3 (0.05f,0.05f,0.f);
			else
				//wireColor += btVector3 (.5f,0.f,0.f);
                wireColor -= btVector3 (0.0f,0.05f,0.05f);
		}
        //ISLAND_SLEEPING
		if(colObj->getActivationState()==2) {
			if(i&1)
				//wireColor += btVector3 (0.f,1.f, 0.f);
                wireColor -= btVector3 (0.05f, 0.0f, 0.05f);
			else
				//wireColor += btVector3 (0.f,0.5f,0.f);
                wireColor -= btVector3 (0.0f, 0.05f, 0.0f);
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


void GearScene::clientResetScene()
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


