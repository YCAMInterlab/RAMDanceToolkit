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
	
	
}

//--------------------------------------------------------------
void testApp::update()
{
	/// Entities update
	// ------------------
	oscReceiver.update();
	
}

//--------------------------------------------------------------
void testApp::draw()
{
	ramBasicFloor(0, 1000, 30, ramColor::WHITE, ramColor::BLACK);
}



#pragma mark - ram methods

//--------------------------------------------------------------
void testApp::drawActor(ramActor &actor)
{
	
	
	for(int i=0; i<actor.getNumNode(); i++)
	{
		ramNode &node = actor.getNode(i);
		
		
		if( i == ramActor::JOINT_RIGHT_ELBOW )
		{
			node.setPosition(0,0,0);
		}
		
		ofBox(node, (i== ramActor::JOINT_HEAD) ? 10 : 5 );
		
		
		if( node.hasParent() )
		{
			ofSetLineWidth(2);
			ofLine( node, *node.getParent() );
		}
	}
	
	
	
	
	
	
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

