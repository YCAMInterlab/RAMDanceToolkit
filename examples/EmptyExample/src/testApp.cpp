#include "testApp.h"





#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground( ramColor::WHITE );
	
	
	ofSetLogLevel(OF_LOG_FATAL_ERROR);
	
	/*!
	 ramBaseApp setup
	 */
	ramEnableAllEvents();
	oscReceiver.setup(10000);
	
	
	/*!
	 GUI setup
	 */
	gui.setup();
//	gui.loadFont("Fonts/din-webfont.ttf", 11);
//	gui.loadCameraSettings("settings.camera.xml");
}

//--------------------------------------------------------------
void testApp::update()
{
	/* Entities update */
	oscReceiver.update();
}

//--------------------------------------------------------------
void testApp::draw()
{
	
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
//	ramBasicFloor(gui.getValueI("Floor pattern"),
//				  gui.getValueF("Floor size"),
//				  gui.getValueF("Grid size"),
//				  ramColor::BLUE_LIGHT,
//				  ramColor::BLUE_LIGHT-20);
}

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	ramBasicActor(actor, NULL);
}

//--------------------------------------------------------------
void testApp::drawRigid(ramRigidBody &rigid)
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

