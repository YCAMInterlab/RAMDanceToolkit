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


/*!
 Scenes
 */

#include "dpVisMemeA.h"
dpVisMemeA memeA;

#include "dpVisMemeB.h"
dpVisMemeB memeB;

#include "dpVisBlinkEffect.h"
BlinkEffect blinkeffect;

#include "dpVisMEME.h"
MEME meme;

#include "dpVisConcentrate.h"
Concentrate concentrate;

#include "dpVisBlinkSync.h"
BlinkSync blinksync;

#include "dpVisBlinkFrequency.h"
BlinkFrequency blinkfrequency;

#include "dpVisFrequency2.h"
Frequency2 frequency2;

#include "dpVisAccEffect.h"
AccEffect acceffect;



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
	ramSceneManager& sceneManager = ramSceneManager::instance();

    sceneManager.addScene( memeA.getPtr());
    sceneManager.addScene( memeB.getPtr());
    
    sceneManager.addScene(meme.getPtr());
    sceneManager.addScene(blinksync.getPtr());
    sceneManager.addScene(blinkfrequency.getPtr());
    sceneManager.addScene(frequency2.getPtr());
    
    
//    sceneManager.addScene(blinkeffect.getPtr());
//    sceneManager.addScene(concentrate.getPtr());
//    sceneManager.addScene(acceffect.getPtr());
    
        
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
