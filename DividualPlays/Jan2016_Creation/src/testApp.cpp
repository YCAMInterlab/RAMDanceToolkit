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

//#include "ramEventMain.h"
//#include "ramFilterMain.h"
//#include "ramGraphicsMain.h"
//#include "ramFilterMain.h"
//#include "ramRecognizerMain.h"

/*====== Legacy ======*/
#include "LineDrawing.h"
LineDrawing drawLines;

#include "BigBox.h"
BigBox bigbox;

#include "Future.h"
Future future;

#include "Donuts.h"
Donuts donuts;

#include "Expansion.h"
Expansion expansion;

#include "Particles.h"
Particles particles;

#include "UpsideDown.h"
UpsideDown upsideDown;

#include "Kepler.h"
Kepler kepler;

#include "HastyChase.h"
HastyChase hastyChase;

#include "ThreePoints.h"
ThreePoints threePoints;

#include "FourPoints.h"
FourPoints fourPoints;

#include "Chain.h"
Chain chain;

#include "Monster.h"
Monster monster;

#include "Notation.h"
Notation notation;


/* ==== 2014 Creation ==== */
#include "FixedBody.h"
FixedBody mFixed;

#include "BrokenBody.h"
BrokenBody mBroken;

#include "BurstBody.h"
BurstBody mBurst;

#include "AtractSphere.h"
AtractSphereScene mSphere;

#include "MoveSeparatedObjects.h"
MoveSeparatedObjects mSep;

#include "WeiredSpace.h"
WeiredSpace mWeierd;

#include "FloorLine.h"
FloorLine mFloorLine;

#include "ThreePointFlow.h"
ThreePointFlow mThree;

#include "VectorFieldScene.h"
VectorFieldScene mVector;

#include "WeiredSpace.h"
WeiredSpace mWeired;

#include "MoveSeparatedObjects.h"
MoveSeparatedObjects mMoveSeparatedObject;

#include "distanceMetaball.h"
distanceMetaball mDistanceMetaball;

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


	ramSceneManager& sceneManager = ramSceneManager::instance();
	
	/* Legacy */
	sceneManager.addScene( drawLines.getPtr() );
	sceneManager.addScene( bigbox.getPtr() );
	sceneManager.addScene( future.getPtr() );
	sceneManager.addScene( donuts.getPtr() );
	sceneManager.addScene( expansion.getPtr() );
	// ignore win32
#ifndef TARGET_WIN32
	sceneManager.addScene( particles.getPtr() );
#endif
	sceneManager.addScene( upsideDown.getPtr() );
	sceneManager.addScene( kepler.getPtr() );
	sceneManager.addScene( hastyChase.getPtr() );
	sceneManager.addScene( threePoints.getPtr() );
	sceneManager.addScene( fourPoints.getPtr() );
	sceneManager.addScene( chain.getPtr() );
	sceneManager.addScene( monster.getPtr() );
	sceneManager.addScene( notation.getPtr() );
	
    sceneManager.addScene(mFixed.getPtr());
    sceneManager.addScene(mBroken.getPtr());
    sceneManager.addScene(mBurst.getPtr());
    sceneManager.addScene(mFloorLine.getPtr());
    sceneManager.addScene(mThree.getPtr());
    sceneManager.addScene(mVector.getPtr());
    sceneManager.addScene(mWeired.getPtr());
    sceneManager.addScene(mMoveSeparatedObject.getPtr());

	//Sheep
	sceneManager.addScene(mDistanceMetaball.getPtr());

	/*
	sceneManager.addScene(mCube.getPtr());
	sceneManager.addScene(mMucous.getPtr());
	sceneManager.addScene(mStick.getPtr());
	sceneManager.addScene(mUnity.getPtr());
	sceneManager.addScene(mSphere.getPtr());
	sceneManager.addScene(mSep.getPtr());
	sceneManager.addScene(mWeierd.getPtr());
	 */
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
