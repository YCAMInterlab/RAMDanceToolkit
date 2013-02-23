#include "testApp.h"


ramExpansion expantion;

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


	/// register ramExpansion instance on GUI
	// ------------------
	ramGetGUI().addPanel(&expantion);
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
	ofSetColor(ramColor::GREEN_LIGHT);
	ramDrawBasicActor(actor);

	ramActor &expandedActor = (ramActor&)expantion.update(actor);

	ofPushStyle();
	ofSetColor(255);

	ofNoFill();
	for (int i=0; i<expandedActor.getNumNode(); i++)
	{
		ramNode &node = expandedActor.getNode(i);

		node.beginTransform();
		ofBox(10);
		node.endTransform();
		node.drawNodeName();
	}
	ofPopStyle();
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

