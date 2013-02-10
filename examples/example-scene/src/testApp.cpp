#include "testApp.h"


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
	
	
	/// Scene setup
	// ------------------
	myScene.setup();
	ramGetGUI().addPanel(&myScene);
}

//--------------------------------------------------------------
void testApp::update()
{
	/// Entities update
	// ------------------
	oscReceiver.update();
	
	
	/// Scene update
	// ------------------
	myScene.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	/// Scene draw
	// ------------------
	myScene.draw();
}



#pragma mark - ram methods

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	/// Scene drawActor
	// ------------------
	myScene.drawActor(actor);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
{
	/// Scene drawRigid
	// ------------------
	myScene.drawRigid(rigid);
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

