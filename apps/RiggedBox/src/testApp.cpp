#include "testApp.h"
#include "ofxAutoControlPanel.h"

#include "btBulletDynamicsCommon.h"

#include "RiggedBoxScene.h"
#include "RiggedBox.h"

#include "ofxBtHelper.h"

ofxAutoControlPanel gui;

RiggedBoxScene scene;

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
	
	ramInit();
	
	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	// gui setup
	ofxControlPanel::setTextColor(simpleColor(255,255,255,255));
	ofxControlPanel::setBackgroundColor(simpleColor(0,0,0,127));
    
    scene.setup();
    
    ofEasyCam *cam = (ofEasyCam *)&ramCameraManager::instance().getActiveCamera();
    cam->setDistance(500);
    
    m_picker0.setWorld(scene.getDynamicsWorld());
    m_picker1.setWorld(scene.getDynamicsWorld());
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
    
    ramPushAll();
    ramBeginCamera();
    scene.draw();
    ramEndCamera();
    ramPopAll();
    
	ramPushAll();
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    ramPopAll();
}




#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{

}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
    ramPushAll();
    
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    
    for (int i=0; i<actor.getNumNode(); i++)
	{
		ramNode &node = actor.getNode(i);
		float jointSize = (i==ramActor::JOINT_HEAD) ? 6.0 : 3.0;
		
		node.beginTransform();
		ofSetColor(ramColor::BLUE_LIGHT);
        ofNoFill();
		//ofBox(jointSize);
		node.endTransform();
		
		if (node.hasParent())
		{
			ofSetColor(ramColor::RED_LIGHT);
			//ofLine(node, *node.getParent());
		}
	}
    
    glDisable(GL_DEPTH_TEST);
    ramPushAll();
    {
        glTranslated(0.0f, 10.0f, 0.0f);
        
        ofMultMatrix(m_shadowMat);
        ofEnableAlphaBlending();
        
        ofSetColor(0, 120);
        
        for (int i=0; i<actor.getNumNode(); i++)
        {
            ramNode &node = actor.getNode(i);
            float jointSize = (i==ramActor::JOINT_HEAD) ? 6.0 : 3.0;
            
            node.beginTransform();
            ofNoFill();
            ofBox(jointSize);
            node.endTransform();
            
            if (node.hasParent())
            {
                ofLine(node, *node.getParent());
            }
        }
    }
    ramPopAll();

    
    
    
    
    const ofVec3f lhp = actor.getNode(ramActor::JOINT_LEFT_HAND).getGlobalPosition();
    const ofVec3f rhp = actor.getNode(ramActor::JOINT_RIGHT_HAND).getGlobalPosition();
    m_picker0.updatePosition(btVector3(lhp.x, lhp.y, lhp.z));
    m_picker1.updatePosition(btVector3(rhp.x, rhp.y, rhp.z));
    
    ramPopAll();
}

#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    scene.keyPressed(key);
    
    if (key == ' ') {
        if (scene.getRiggedBoxes().size()) {
            btRigidBody *bd0 = scene.getRiggedBoxes().at(0)->m_bodies.at(9);
            btRigidBody *bd1 = scene.getRiggedBoxes().at(0)->m_bodies.at(11);
            
            m_picker0.attatchRigidBody(bd0);
            m_picker1.attatchRigidBody(bd1);
        }
    }
}











