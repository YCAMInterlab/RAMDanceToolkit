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


ramMovementAnalyser movement;
ramTimerdMovementAnalyser timerdMovement;

ramNode nodeMov;
ramNode nodeTMov;
float mov;
float tMov;

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);

	/// ram setup
	// ------------------
	ramInitialize(10000);
	
	movement.setJointID(ramActor::JOINT_LEFT_ANKLE);
	timerdMovement.setJointID(ramActor::JOINT_HEAD);
	
	ramGetGUI().addPanel( &movement );
	ramGetGUI().addPanel( &timerdMovement );
}

//--------------------------------------------------------------
void testApp::update()
{
	if (getNumNodeArray() > 0)
	{
		const string name = getNodeArray(0).getName();
		movement.setTargetName( name );
		timerdMovement.setTargetName( name );
	}

	mov = movement.update();
	tMov = timerdMovement.update();
}	

//--------------------------------------------------------------
void testApp::draw()
{
	
	ramBeginCamera();
	
	// debugDraw
	ofSetColor(100);
	movement.draw();
	timerdMovement.draw();
	
	// see also ramNodeFinder
	movement.findOne(nodeMov);
	timerdMovement.findOne(nodeTMov);
	
	// draw value
	ofSetColor(255);
	ofDrawBitmapString("mov: "+ofToString(mov), nodeMov.getGlobalPosition()+30);
	ofDrawBitmapString("tMov: "+ofToString(tMov), nodeTMov.getGlobalPosition()+30);
	ramEndCamera();
}


#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(const ramActor &actor)
{
	ofSetColor(100);
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
