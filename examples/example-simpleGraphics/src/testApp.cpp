#include "testApp.h"



#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);


	/// ram setup
	// ------------------
	ramInitialize(10000);
	ramEnableShowActors(false);
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
	ramNode n1 = actor.getNode(ramActor::JOINT_HEAD);
	ramNode n2 = actor.getNode(ramActor::JOINT_RIGHT_HAND);
	ramNode n3 = actor.getNode(ramActor::JOINT_LEFT_HAND);
	ramNode n4 = actor.getNode(ramActor::JOINT_RIGHT_TOE);
	ramNode n5 = actor.getNode(ramActor::JOINT_LEFT_TOE);

	ofPushStyle();
	{
		// draw line betweem two nodes
		ofNoFill();
		ofSetColor( ramColor::RED_LIGHT );
		ofSetLineWidth( 3 );
		ofLine( n2, n3 );


		// draw triangle using three nodes
		ofFill();
		ofSetColor( ramColor::BLUE_LIGHT );
		ofSetLineWidth( 3 );
		ofTriangle( n1, n4, n5 );


		// ramNode::beginTransform() ~ endTransform()
		n1.beginTransform();
		{
			// draw cube at JOINT_HEAD
			ofNoFill();
			ofSetColor( ramColor::YELLOW_DEEP );
			ofSetLineWidth( 3 );
			ofBox( 20 );

			// draw cone at JOINT_HEAD
			ofNoFill();
			ofSetColor( ramColor::GRAY );
			ofSetLineWidth( 2 );
			ofCone(10, 100);
		}
		n1.endTransform();


		// draw plane using several nodes
		ofNoFill();
		ofSetColor( ramColor::YELLOW_LIGHT );
		ofSetLineWidth( 2 );
		ofPushMatrix();
		{
			ofScale( 3, 3, 3 );
			ramStripe( n1, n2, n3, n4, n5 );
		}
		ofPopMatrix();
	}
	ofPopStyle();
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

