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

//#include "Kepler.h"
//Kepler kepler;

#include "HastyChase.h"
HastyChase hastyChase;

#include "ThreePoints.h"
ThreePoints threePoints;

#include "FourPoints.h"
FourPoints fourPoints;

//#include "Chain.h"
//Chain chain;

#include "Monster.h"
Monster monster;

//#include "Notation.h"
//Notation notation;


/* ==== 2014 Creation ==== */

#include "BurstBody_ext.h"
BurstBody_ext mBurst_ext;

#include "distanceMetaball.h"
distanceMetaball mDistanceMetaball;

/* ==== Legacy external ==== */
#include "lineDrawing_ext.h"
LineDrawing_ext mLine_ext;

#include "BrokenBody_ext.h"
BrokenBody_ext mBrokenBody_ext;

#include "BigBox_ext.h"
BigBox_ext mBigBox_ext;

#include "FourPoints_ext.h"
FourPoints_ext mFourPoints_ext;

#include "UpsideDown_ext.h"
UpsideDown_ext mUpsideDown_ext;

#include "HastyChase_ext.h"
HastyChase_ext mHastyChase_ext;

#include "Monster_ext.h"
Monster_ext mMonster_ext;
Monster_ext_2 mMonster_ext2;

#include "ThreePoints_ext.h"
ThreePoints_ext mThreePoints_ext;

#include "Donuts_ext.h"
Donuts_ext mDonuts_ext;

#include "dp16_camera_controller.h"
dp16_camera_controller mCameraController;

#include "dp16_ramActorTranslator.h"
dp16_ramActorTranslator mRamActorTranslator;

#include "dp16_actorDrawer.h"
dp16_actorDrawer mActorDrawer;

//ito Scenes
#include "dpHakoniwaSand.h"
#include "dpHakoniwaMagnetPendulum.h"

#include "dpSyphonClientManager.h"

#include "magnetLooper.h"

#include "dpHakoVisBug.h"

// sand
dpHakoniwaSand hakoniwaSand;
dpHakoVisSandStorm visSandStorm;

// struggle
dpHakoniwaStruggle hakoniwaStruggle;
dpHakoVisStruggle visStruggle;

//MagPendulum
dpHakoniwaMagnetPendulum	magPendulum;
dpHakoVisMagnetPendulum visMagnetPendulum;

// servo pendulum
dpHakoVisServoPendulum      visServoPendulum;

// tornado
dpHakoniwaTornado hakoniwaTornado;
dpHakoVisTornado  visTornado;

dpRawCamera rawCamera;
dpSyphonClientManager *mSyphonClientManager;

// stage
dpHakoVisStageBlob                visStage;

//magnet looper
magnetLooper looper;

#include "dpHakoniwaGearMove.h"

dpHakoniwaGearMove hakoniwaGearMove;

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
    
    mSyphonClientManager = &dpSyphonClientManager::instance();
    mSyphonClientManager->setup();
	
    sceneManager.addScene(rawCamera.getPtr());
    
	sceneManager.addScene(mCameraController.getPtr());
	sceneManager.addScene(mRamActorTranslator.getPtr());
	
	sceneManager.addScene(mLine_ext.getPtr());
	sceneManager.addScene(mBrokenBody_ext.getPtr());
	sceneManager.addScene(mBigBox_ext.getPtr());
	sceneManager.addScene(mFourPoints_ext.getPtr());
    sceneManager.addScene(mUpsideDown_ext.getPtr());
    sceneManager.addScene(mHastyChase_ext.getPtr());
    sceneManager.addScene(mMonster_ext.getPtr());
	sceneManager.addScene(mMonster_ext2.getPtr());

    sceneManager.addScene(mThreePoints_ext.getPtr());
    sceneManager.addScene(mDonuts_ext.getPtr());

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
	sceneManager.addScene( hastyChase.getPtr() );
	sceneManager.addScene( threePoints.getPtr() );
	sceneManager.addScene( fourPoints.getPtr() );
	sceneManager.addScene( monster.getPtr() );

    sceneManager.addScene(mBurst_ext.getPtr());

	//Sheep
	sceneManager.addScene(mDistanceMetaball.getPtr());
    
    sceneManager.addScene(hakoniwaServoPendulum.getPtr());
    sceneManager.addScene(visServoPendulum.getPtr());
    
    sceneManager.addScene(visStage.getPtr());
    
    sceneManager.addScene(hakoniwaStruggle.getPtr());
    sceneManager.addScene(visStruggle.getPtr());
    
    sceneManager.addScene(hakoniwaSand.getPtr());
    sceneManager.addScene(visSandStorm.getPtr());
    
    sceneManager.addScene(magPendulum.getPtr());
    sceneManager.addScene(visMagnetPendulum.getPtr());
    
    //tornado
    sceneManager.addScene(hakoniwaTornado.getPtr());
    sceneManager.addScene(visTornado.getPtr());
    
    sceneManager.addScene(looper.getPtr());
    sceneManager.addScene(allHakoniwaMove.getPtr());
    
    sceneManager.addScene(hakoniwaGearMove.getPtr());
	
    sceneManager.allocateFbos(SINGLE_SCREEN_WIDTH, SINGLE_SCREEN_HEIGHT);
    sceneManager.setShowAllActors(false);
    
    //TO DO make draw call order manager.
    // do something, dancer cue, something draw called after sceneManager draw method
    afterBaseScene.setup();

}

//--------------------------------------------------------------
void testApp::update()
{
    
    mSyphonClientManager->update();
    ramGetGUI().getSceneTabs().setPosition(0, -mouseY);
    
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
    if(key == 'f')ofToggleFullscreen();
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
