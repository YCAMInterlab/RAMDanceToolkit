#include "testApp.h"
#include "ofxBt.h"

static const string myActorName = "Ando_2012-09-01_18-49-10";

ramBoxPrimitive *cube;
ramSpherePrimitive *sphere;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10000);
	
	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
	
	
	cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
	sphere = new ramSpherePrimitive(ofVec3f(0, 0, 0), 50);
}

//--------------------------------------------------------------
void testApp::update()
{
	oscReceiver.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	ramCameraBegin();
	{
		ofNoFill();
		cube->draw();
		sphere->draw();
		ramPhysics::instance().debugDraw();
	}
	ramCameraEnd();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glPopAttrib();
}




#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
	ramBasicFloor(600., 50.);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glEnable(GL_DEPTH_TEST);
	glPushMatrix();
	ofPushStyle();
	for (int i=0; i<actor.getNumNode(); i++)
    {
        ramNode &node = actor.getNode(i);
		
		ofNoFill();
		ofSetColor(0, 255, 0);
		ramBox(node, (i==ramActor::JOINT_HEAD) ? 6 : 3);
    }
	ofPopStyle();
	glPopMatrix();
	glDisable(GL_DEPTH_TEST);
	glPopAttrib();
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
}




#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	cube = new ramBoxPrimitive(ofVec3f(0, 300, 0), 100);
}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}

