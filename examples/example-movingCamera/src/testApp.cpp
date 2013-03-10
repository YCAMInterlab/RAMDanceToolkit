#include "testApp.h"


#include "CameraMover.h"
CameraMover mover;


#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);


	/// ram setup
	// ------------------
	ramInitialize(10000);

	ramSceneManager &sceneManager = ramSceneManager::instance();
	sceneManager.addScene(mover.getPtr());
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
}

//--------------------------------------------------------------
void testApp::drawRigid(const ramRigidBody &rigid)
{

}


#pragma mark - ram Events
//--------------------------------------------------------------
void testApp::onActorSetup(const ramActor &actor)
{
	
}

//--------------------------------------------------------------
void testApp::onActorExit(const ramActor &actor)
{
	
}

//--------------------------------------------------------------
void testApp::onRigidSetup(const ramRigidBody &rigid)
{
	
}

//--------------------------------------------------------------
void testApp::onRigidExit(const ramRigidBody &rigid)
{
	
}


#pragma mark - of Events
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