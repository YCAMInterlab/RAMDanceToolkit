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
#include "MovingCam.h"
MovingCam movingCam;

#include "LineDrawing.h"
LineDrawing drawLines;

#include "BigBox.h"
BigBox bigbox;

#include "Future.h"
Future future;

#include "Donuts.h"
Donuts donuts;

#include "Stamp.h"
Stamp stamp;

#include "Expansion.h"
Expansion expansion;

#include "Particles.h"
Particles particles;

#include "Abacus.h"
Abacus abacus;

#include "SoundCube.h"
SoundCube soundcube;

#include "UpsideDown.h"
UpsideDown upsideDown;

#include "Kepler.h"
Kepler kepler;

#include "HastyChase.h"
HastyChase hastyChase;

#include "ColorGrid.h"
ColorGrid colorGrid;

#include "ThreePoints.h"
ThreePoints threePoints;

#include "FourPoints.h"
FourPoints fourPoints;

#include "Chain.h"
Chain chain;    

#include "Monster.h"
Monster monster;

#include "Laban.h"
Laban laban;

#include "Notation.h"
Notation notation;


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
	sceneManager.addScene( movingCam.getPtr() );
	sceneManager.addScene( drawLines.getPtr() );
	sceneManager.addScene( bigbox.getPtr() );
	sceneManager.addScene( future.getPtr() );
	sceneManager.addScene( donuts.getPtr() );
	sceneManager.addScene( stamp.getPtr() );
	sceneManager.addScene( expansion.getPtr() );
// ignore win32
#ifndef TARGET_WIN32
	sceneManager.addScene( particles.getPtr() );
#endif
	sceneManager.addScene( abacus.getPtr() );
	sceneManager.addScene( soundcube.getPtr() );
	sceneManager.addScene( upsideDown.getPtr() );
	sceneManager.addScene( kepler.getPtr() );
	sceneManager.addScene( hastyChase.getPtr() );
	sceneManager.addScene( colorGrid.getPtr() );
	sceneManager.addScene( threePoints.getPtr() );
	sceneManager.addScene( fourPoints.getPtr() );
	sceneManager.addScene( chain.getPtr() );
	sceneManager.addScene( monster.getPtr() );
	sceneManager.addScene( laban.getPtr() );
	sceneManager.addScene( notation.getPtr() );
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
