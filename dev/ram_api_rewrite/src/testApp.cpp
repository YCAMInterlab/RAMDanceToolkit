#include "testApp.h"

ofEasyCam cam;

//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	ofBackground(0);
	oscReceiver.setup(10000);

	// enable ramBaseApp::setup, update, draw, exit
	ramEnableAllEvents();
}

//--------------------------------------------------------------
void testApp::update()
{
	while (oscReceiver.hasWaitingMessages())
	{
		ofxOscMessage m;
		oscReceiver.getNextMessage(&m);
		updateWithOscMessage(m);
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
}

// ram methods
//--------------------------------------------------------------
void testApp::drawFloor()
{
}

void testApp::drawJoint(const ramNode& joint)
{
	cam.begin();

	ofSetColor(255, 0, 0);
	ofNoFill();
	joint.transformBegin();
	ofBox(10);
	joint.transformEnd();

	ofSetColor(0, 255, 0);
	ofBox(joint, 20);

	cam.end();
}

void testApp::drawBone(const ramNode& jointA, const ramNode& jointB)
{
	cam.begin();

	ofSetColor(255);
	ofLine(jointA, jointB);

	cam.end();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
	switch (key)
	{
	case 'r':
		record();
		break;

	case 'p':
		play();
		break;
	default:
		break;
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