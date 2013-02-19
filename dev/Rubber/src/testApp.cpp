#include "testApp.h"
#include "ofxAutoControlPanel.h"

#include "btBulletDynamicsCommon.h"

#include "GearScene.h"

ofxAutoControlPanel gui;

GearScene scene;

//--------------------------------------------------------------
void ramPushAll()
{
    ofPushView();
    
    glPushAttrib(GL_ALL_ATTRIB_BITS);
    
    GLint matrixMode;
    glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
    
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glMatrixMode(GL_TEXTURE);
    glPushMatrix();
    glMatrixMode(GL_COLOR);
    glPushMatrix();
    
    glMatrixMode(matrixMode);
    
    ofPushStyle();
}

//--------------------------------------------------------------
void ramPopAll()
{
    ofPopStyle();
    
    GLint matrixMode;
    glGetIntegerv(GL_MATRIX_MODE, &matrixMode);
    
    glMatrixMode(GL_COLOR);
    glPopMatrix();
    glMatrixMode(GL_TEXTURE);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    glMatrixMode(matrixMode);
    
    glPopAttrib();
    
    ofPopView();
}

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10000);
	
	ramInitialize();
	
	// gui setup
	ofxControlPanel::setTextColor(simpleColor(255,255,255,255));
	ofxControlPanel::setBackgroundColor(simpleColor(0,0,0,127));
    
    scene.setup();
    
    ofEasyCam *cam = (ofEasyCam *)&ramCameraManager::instance().getActiveCamera();
    cam->setDistance(500);
    
    m_picker.clear();
    
    for (int i=0; i<scene.getP2Ps().size(); i++) {
        btPicker *picker = new btPicker;
        picker->setWorld(scene.getDynamicsWorld());
        picker->attatchRigidBody(scene.getP2Ps().at(i).bodyB);
        m_picker.push_back(picker);
    }
    
    bShowLine= false;
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
    scene.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
    ofBackgroundGradient(ofColor(120), ofColor(60));
    
    //ofSetupScreen();
    ramPushAll();
    ramBeginCamera();
    scene.draw();
    
    ofDisableLighting();
    ofSetColor(ramColor::GREEN_NORMAL);
    for (int i=0; i<scene.getP2Ps().size(); i++) {

        btVector3 posA = scene.getP2Ps().at(i).bodyA->getCenterOfMassPosition();
        btVector3 posB = scene.getP2Ps().at(i).bodyB->getCenterOfMassPosition();

        if (bShowLine)
            ofLine(posA.x(), posA.y(), posA.z(), posB.x(), posB.y(), posB.z());
        
    }
    
    
    ramEndCamera();
    ramPopAll();
}

#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
    //ramPushAll();
    //glEnable(GL_DEPTH_TEST);
    //ramBasicFloor(ramFloor::FLOOR_CHECKER_PATTERN);
    //ramPopAll();
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	ramPushAll();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    for (int i=0; i<actor.getNumNode(); i++) {
		ramNode &node = actor.getNode(i);
		float jointSize = (i==ramActor::JOINT_HEAD) ? 6.0 : 3.0;
		
		node.beginTransform();
		ofSetColor(ramColor::BLUE_NORMAL);
        ofNoFill();
		ofBox(jointSize);
		node.endTransform();
		
		if (node.hasParent()) {
			ofSetColor(ramColor::RED_NORMAL);
			ofLine(node, *node.getParent());
		}
	}
    
    for (int i=0; i<m_picker.size(); i++) {
        ofVec3f pos;
        switch (i) {
            case 0:
                pos = actor.getNode(ramActor::JOINT_LEFT_HAND).getGlobalPosition();
                break;
            case 1:
                pos = actor.getNode(ramActor::JOINT_RIGHT_HAND).getGlobalPosition();
                break;
        }
        
        m_picker.at(i)->updatePosition(btVector3(pos.x, pos.y, pos.z));
    }
    
    glDisable(GL_DEPTH_TEST);
    ramPopAll();
}

#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    scene.keyPressed(key);
    
    if (key == 'l') bShowLine ^= true;
}
