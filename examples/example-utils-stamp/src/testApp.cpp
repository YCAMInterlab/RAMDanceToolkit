#include "testApp.h"

ramStamp stamp;

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
	
	
	/// stamp setup
	// ------------------
	stamp.setup();
	ramGetGUI().addPanel(&stamp);
}

//--------------------------------------------------------------
void testApp::update()
{
	if (getNumNodeArray() > 0)
		stamp.update(getNodeArray(0));
}

//--------------------------------------------------------------
void testApp::draw()
{
	ramBeginCamera();
	
	for (int i=0; i<stamp.getSize(); i++)
	{
		ramActor& actor = (ramActor&)stamp.get(i);
		
		ramDrawBasicActor(actor);
		ramDrawActorCube(actor);
		
		ramBeginShadow();
		ramDrawBasicActor(actor);
		ramDrawActorCube(actor);
		ramEndShadow();
	}
	
	ramEndCamera();
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
		stamp.clear();
	}
	if (key == 'p')
	{
		stamp.createStamp( getNodeArray(0) );
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