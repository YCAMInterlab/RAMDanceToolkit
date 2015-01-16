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
#include "dpConstants.h"

/*!
 Scenes
 */
#include "dpMarionette.h"
#include "ramMotionExtractorExampleScene.h"

#include "HakoniwaPLink_Laser.h"
#include "HakoniwaPLink_Prism.h"
#include "HakoniwaPlink_Oil.h"

#include "hakoVisPLink_Laser.h"
#include "hakoVisPLink_Oil.h"
#include "hakoVisPLink_Prism.h"

HakoniwaPLink_Laser plink_laser;
HakoniwaPLink_Oil	plink_oil;
HakoniwaPLink_Prism plink_prism;

ramMotionExtractorExampleScene motionExt;
hakoVisPLink_Laser	visLaser;
hakoVisPLink_Oil visOil;

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	/// ram setup
	// ------------------
	ramInitialize(10000, true);
    
	/// scenes setup
	// ------------------
//    ofSetWindowPosition(2000, 0);
//    ofSetFullscreen(true);
	ramSetViewPort(ofRectangle(0.f, 0.f, SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT));

	ramSceneManager& sceneManager = ramSceneManager::instance();

	sceneManager.addScene( plink_laser.getPtr());
	sceneManager.addScene( plink_oil.getPtr());
	sceneManager.addScene( plink_prism.getPtr());

	sceneManager.addScene( visLaser.getPtr());
	sceneManager.addScene( visOil.getPtr());
	sceneManager.addScene( motionExt.getPtr());

	sceneManager.allocateFbos(SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT);
}

//--------------------------------------------------------------
void testApp::update()
{
//    ramSetViewPort(ofRectangle(0.f, 0.f, ofGetWidth(), ofGetHeight() * 0.5f));
}

//--------------------------------------------------------------
void testApp::draw()
{

}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(const ramActor &actor)
{
//	dpMarionette::instance().drawActor(actor);
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



#pragma mark - of Event
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
