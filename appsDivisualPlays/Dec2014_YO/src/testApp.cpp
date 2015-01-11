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

struct : public ramBaseScene {
    void drawActor(const ramActor &actor) { ramDrawBasicActor(actor); }
    string getName() const { return "TestSceneA"; }
} testSceneA;

struct : public ramBaseScene {
    void drawActor(const ramActor &actor) { ramDrawBasicActor(actor); }
    string getName() const { return "TestSceneB"; }
} testSceneB;

struct : public ramBaseScene {
    void drawActor(const ramActor &actor) { ramDrawBasicActor(actor); }
    string getName() const { return "TestSceneC"; }
} testSceneC;

struct : public ramBaseScene {
    void drawActor(const ramActor &actor) { ramDrawBasicActor(actor); }
    string getName() const { return "TestSceneD"; }
} testSceneD;

#pragma mark - oF methods
//--------------------------------------------------------------
void testApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	/// ram setup
	// ------------------
	ramInitialize(10000, true);
    
    ramSetViewPort(ofRectangle(0.f, 0.f, SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT));
    
	/// scenes setup
	// ------------------
	ramSceneManager& sceneManager = ramSceneManager::instance();
    sceneManager.addScene( hakoniwaOscTest.getPtr() );
	sceneManager.addScene( hakoniwaColorOfWater.getPtr() );
    sceneManager.addScene( hakovisLaser.getPtr() );
    
    sceneManager.addScene( testSceneA.getPtr() );
    sceneManager.addScene( testSceneB.getPtr() );
    sceneManager.addScene( testSceneC.getPtr() );
    sceneManager.addScene( testSceneD.getPtr() );
	
    sceneManager.allocateFbos(SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT);
    
    sceneManager.enableScreen("TestSceneA", 0);
    sceneManager.enableScreen("TestSceneB", 1);
    sceneManager.enableAllScreens("TestSceneC");
    sceneManager.disableScreens("TestSceneD");
}

//--------------------------------------------------------------
void testApp::update()
{
    //ramSetViewPort(ofRectangle(0.f, 0.f, ofGetWidth(), ofGetHeight() * 0.5f));
}

//--------------------------------------------------------------
void testApp::draw()
{
	
}



#pragma mark - ram methods
//--------------------------------------------------------------
void testApp::drawActor(const ramActor &actor)
{
	
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
