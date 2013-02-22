#include "testApp.h"

ramNodeLine node_line;

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

	// setup ramNodeLine class
	node_line.from.set(ramActor::JOINT_LEFT_HAND);
	node_line.control0.set(ramActor::JOINT_LEFT_WRIST);
	node_line.control1.set(ramActor::JOINT_RIGHT_WRIST);
	node_line.to.set(ramActor::JOINT_RIGHT_HAND);
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
void testApp::drawActor(ramActor &actor)
{
	ramDrawBasicActor(actor);

	// draw line strip with some effects
	node_line.curve();
	node_line.extend();
	node_line.draw();

	// or oneliner
	// node_line.curve(200).resampling(0.1).noise(10, 10, ofGetElapsedTimef()).spiral(50, 6).draw();
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

