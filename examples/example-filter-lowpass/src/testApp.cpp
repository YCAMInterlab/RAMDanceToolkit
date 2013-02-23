#include "testApp.h"


ramLowPassFilter lowpass;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(ramColor::WHITE);


	/// ram setup
	// ------------------
	ramInitialize(10000);

	ramGetGUI().addPanel(&lowpass);
}

//--------------------------------------------------------------
void testApp::update()
{

}

//--------------------------------------------------------------
void testApp::draw()
{

}


#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(const ramActor &actor)
{
	// normal
	ofSetColor(ramColor::GREEN_LIGHT);
	ramDrawBasicActor(actor);

	// lowpassed
	ramActor &a = (ramActor&)lowpass.filter(actor);
	ofSetColor(ramColor::RED_LIGHT);
	ramDrawBasicActor(a);

	// draw line
	ofSetColor(ramColor::BLUE_NORMAL);
	ramDrawNodeCorresponds(actor, a);
}

//--------------------------------------------------------------
void testApp::drawRigid(const ramRigidBody &rigid)
{

}




#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{

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

