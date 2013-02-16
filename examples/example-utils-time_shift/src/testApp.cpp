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
	ramInit();
	oscReceiver.setup(10000);
	
	timeShifter.setup();
	timeShifter.setShiftTime(2.0);
	timeShifter.setShiftFrames(20);
	timeShifter.setShiftType(RAM_TIMESHIFT_BY_FRAMES);
	
	ramGetGUI().addPanel(&timeShifter);
}

//--------------------------------------------------------------
void testApp::update()
{
	/// Entities update
	// ------------------
	oscReceiver.update();
	
	timeShifter.update( getNodeArray(0) );
}

//--------------------------------------------------------------
void testApp::draw()
{
	if (timeShifter.isPlayable())
	{
		ramActor& actor = (ramActor &)timeShifter.getResult();
		
		ramBeginCamera();
		ramDrawBasicActor(actor);
		ramEndCamera();
	}
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	ramDrawBasicActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
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