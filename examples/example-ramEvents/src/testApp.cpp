// 
// testApp.cpp - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
	
	
	/// collision event
	
	float size = 100.0;

	// create shape which has collision detection
	primitive = new ramBoxPrimitive(size);
	// primitive = new ramSpherePrimitive(size);
	// primitive = new ramPyramidPrimitive(size);
	
	// set some variables
	primitive->setPosition(ofVec3f(0.0, size, 0.0));
	primitive->setOrientation(ofVec3f(0.0, 1.0, 0.0));
	primitive->updatePhysicsTransform();
	primitive->getRigidBody().setStatic(true);
	
	// register primitive to event as observed object
	collision.setPrimitive(primitive);
	
	// set trigger timing
	collision.setTrigger(RAM_TRIGGER_BOTH);
}

//--------------------------------------------------------------
void testApp::update()
{
	
	if (timer.update())
	{
		// change box size if timer is fired
		size = ofRandom(1, 50);
	}
	
	if (randomTimer.update())
	{
		// change box size if randome timer is fired
		jointColor = ofColor(ofRandom(20, 240), ofRandom(20, 240), ofRandom(20, 240));
	}
	
	if (collision.update())
	{
		// change box color if actor drawn with ramBox touches primitive
		if (collision.getState())
			primitiveColor = ofColor(255);
		else
			primitiveColor = ofColor(100);
	}
}

//--------------------------------------------------------------
void testApp::draw()
{
	ramBeginCamera();
	ofNoFill();
	ofSetColor(primitiveColor);
	primitive->draw();
	ramEndCamera();
}


#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(const ramActor &actor)
{
	ofSetColor(jointColor);
	for(int i=0; i<actor.getNumNode(); i++)
	{
		const ramNode& node = actor.getNode(i);
		
		ofNoFill();
		
		// note that ofBox doesn't have collision detection.
		// use ramBox, ramSphere, ramPrimitive insted of general oF methods
		ramBox(node, size);
	}
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
