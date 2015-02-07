#include "testApp.h"

#include "btBulletDynamicsCommon.h"

#include "RagdollScene.h"
#include "RagDoll.h"

#include "ofxBtHelper.h"

RagdollScene ragdollScene;

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);

	ramInitialize(10000);
	
    
    ragdollScene.setup();
    
    ofEasyCam *cam = (ofEasyCam *)&ramCameraManager::instance().getActiveCamera();
    cam->setDistance(500);
    
    for (int i=0; i<COUNT; i++)
        m_pickConstraint[i] = NULL;
    
    keyPressed(' ');
}

//--------------------------------------------------------------
void testApp::update()
{
	
    ragdollScene.update();
    
    for (int i=0; i<COUNT; i++) {
        
        if (m_pickConstraint[i]) {
            if (m_pickConstraint[i]->getConstraintType() == D6_CONSTRAINT_TYPE) {
                btGeneric6DofConstraint* pickCon = static_cast<btGeneric6DofConstraint*>(m_pickConstraint[i]);
                if (pickCon)
                    pickCon->getFrameOffsetA().setOrigin(m_handPos[i]);
            }
        }
        
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
    
    //ofSetupScreen();
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    ramBeginCamera();
    ragdollScene.draw();
    ramEndCamera();
    glPopAttrib();
}




#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
//    ramBasicFloor(0);
}

//--------------------------------------------------------------
void testApp::drawActor(const ramActor &actor)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glPushMatrix();
    ofPushStyle();
    
    for (int i=0; i<actor.getNumNode(); i++)
	{
		const ramNode &node = actor.getNode(i);
		float jointSize = (i==ramActor::JOINT_HEAD) ? 6.0 : 3.0;
		
		node.beginTransform();
		ofSetColor(ramColor::BLUE_NORMAL);
        ofNoFill();
		ofBox(jointSize);
		node.endTransform();
		
		if (node.hasParent())
		{
			ofSetColor(ramColor::RED_NORMAL);
			ofLine(node, *node.getParent());
		}
		
		
        //ofColor shadowColor = getRamColor(ramColor::GRAY);
        //shadowColor.a = 50;
        //glPushMatrix();
        //glDisable(GL_DEPTH_TEST);
        //glMultMatrixf(matrixPtr);
        //ofEnableAlphaBlending();
        //ofSetColor(shadowColor);
        //
        //ofBox(node, jointSize);
        //if (node.hasParent())
        //    ofLine(node, *node.getParent());
        //glPopMatrix();
	}
    
    ofVec3f vl = actor.getNode(ramActor::JOINT_LEFT_HAND).getGlobalPosition();
    m_handPos[L].setValue(vl.x, vl.y, vl.z);
    
    ofVec3f vr = actor.getNode(ramActor::JOINT_RIGHT_HAND).getGlobalPosition();
    m_handPos[R].setValue(vr.x, vr.y, vr.z);
    
    ofVec3f vh = actor.getNode(ramActor::JOINT_LEFT_ANKLE).getGlobalPosition();
    m_handPos[H].setValue(vh.x, vh.y, vh.z);
    
    ofPopStyle();
    glPopMatrix();
    glDisable(GL_DEPTH_TEST);
    glPopAttrib();
}

#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    ragdollScene.keyPressed(key);
    
    if (key == ' ') {
        
        for (int i=0; i<COUNT; i++) {
            
            if (m_pickConstraint[i] && ragdollScene.getDynamicsWorld())
            {
                ragdollScene.getDynamicsWorld()->removeConstraint(m_pickConstraint[i]);
                delete m_pickConstraint[i];
                m_pickConstraint[i] = NULL;
                //printf("removed constraint %i",gPickingConstraintId);
                m_pickedBody[i]->forceActivationState(ACTIVE_TAG);
                m_pickedBody[i]->setDeactivationTime( 0.f );
                m_pickedBody[i] = NULL;
            }
            
            
            btAlignedObjectArray<RagDoll *> &ragdolls = ragdollScene.getRagdolls();
            if (ragdolls.size() != 0) {
                
                for (int j=0; j<ragdolls.size(); j++) {
                    RagDoll *dall = ragdolls.at(j);
                    
                    //enum
                    //{
                    //    BODYPART_PELVIS = 0,
                    //    BODYPART_SPINE,
                    //    BODYPART_HEAD,
                    //
                    //    BODYPART_LEFT_UPPER_LEG,
                    //    BODYPART_LEFT_LOWER_LEG,
                    //
                    //    BODYPART_RIGHT_UPPER_LEG,
                    //    BODYPART_RIGHT_LOWER_LEG,
                    //
                    //    BODYPART_LEFT_UPPER_ARM,
                    //    BODYPART_LEFT_LOWER_ARM,
                    //
                    //    BODYPART_RIGHT_UPPER_ARM,
                    //    BODYPART_RIGHT_LOWER_ARM,
                    //
                    //    BODYPART_COUNT
                    //};
                    
                    //int part = 0;
                    int part = RagDoll::BODYPART_HEAD;

                    switch (i) {
                        case L:
                            part = RagDoll::BODYPART_LEFT_LOWER_ARM;
                            break;
                        case R:
                            part = RagDoll::BODYPART_RIGHT_LOWER_ARM;
                            break;
                        case H:
                            part = RagDoll::BODYPART_PELVIS;
                            break;

                    }
                    
                    
                    m_pickedBody[i] = dall->m_bodies[part];
                    
                    if (m_pickedBody) {
                        //other exclusions?
                        if (!(m_pickedBody[i]->isStaticObject() || m_pickedBody[i]->isKinematicObject())) {
                            btRigidBody *body = m_pickedBody[i];
                            body->setActivationState(DISABLE_DEACTIVATION);
                            
                            
                            //btVector3 pickPos = rayCallback.m_hitPointWorld;
                            btVector3 pickPos = body->getCenterOfMassTransform().getOrigin();
                            
                            //printf("pickPos=%f,%f,%f\n",pickPos.getX(),pickPos.getY(),pickPos.getZ());
                            
                            btVector3 localPivot = body->getCenterOfMassTransform().inverse() * pickPos;
                            
                            btTransform tr;
                            tr.setIdentity();
                            tr.setOrigin(localPivot);
                            btGeneric6DofConstraint* dof6 = new btGeneric6DofConstraint(*body, tr,false);
                            dof6->setLinearLowerLimit(btVector3(0,0,0));
                            dof6->setLinearUpperLimit(btVector3(0,0,0));
                            dof6->setAngularLowerLimit(btVector3(0,0,0));
                            dof6->setAngularUpperLimit(btVector3(0,0,0));
                            
                            ragdollScene.getDynamicsWorld()->addConstraint(dof6,true);
                            m_pickConstraint[i] = dof6;
                            
                            dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,0);
                            dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,1);
                            dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,2);
                            dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,3);
                            dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,4);
                            dof6->setParam(BT_CONSTRAINT_STOP_CFM,0.8,5);
                            
                            dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,0);
                            dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,1);
                            dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,2);
                            dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,3);
                            dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,4);
                            dof6->setParam(BT_CONSTRAINT_STOP_ERP,0.1,5);
                            
                            //                        PickingPos = rayTo;
                            //                        gHitPos = pickPos;
                            //
                            //                        gOldPickingDist  = (pickPos-rayFrom).length();
                        }
                    }
                    
                }
            }
        }
    }
}
