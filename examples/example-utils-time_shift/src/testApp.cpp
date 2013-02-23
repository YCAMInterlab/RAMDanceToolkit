#include "testApp.h"

ramTimeShifter timeShifter;

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

	timeShifter.setup();
	timeShifter.setNumBufferFrame(20);

	ramGetGUI().addPanel(&timeShifter);
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
	ramActor shifted_actor = timeShifter.update(actor);
	
	ofSetColor(ramColor::GREEN_NORMAL, 32);
	ramDrawBasicActor(shifted_actor);
	
	ofSetColor(ramColor::BLUE_NORMAL);
	ramDrawBasicActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(const ramRigidBody &rigid)
{

}




#pragma mark - oF Events
//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	if (key == 'c')
	{
		timeShifter.clear();
	}
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