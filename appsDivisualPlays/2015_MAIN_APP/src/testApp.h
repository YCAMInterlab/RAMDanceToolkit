// 
// testApp.h - RAMDanceToolkit
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

#pragma once

#include "ofMain.h"
#include "ramMain.h"

//ito Scenes
#include "dpHakoniwaSand.h"
#include "dpHakoniwaMagnetPendulum.h"

//Miura Scenes
#include "HakoniwaTheta.h"

//Kezzar Scenes
#include "dpHakoniwaServoPendulum.h"
#include "dpHakoVisStageCircle.h"
#include "dpHakoVisServoPendulum.h"
#include "dpHakoVisWorm.h"
#include "dp3DDanceMap.h"
#include "dpHakoVisVecLineCircle.h"
#include "dpHakoniwaStruggle.h"
#include "dpHakoVisStruggle.h"
#include "dpHakoVisSandStorm.h"
#include "dpHakoVisMagnetPendulum.h"
#include "dpHakoVisTornado.h"
#include "dpHakoniwaTornado.h"


//SHEEP Scenes
#include "testScenes.h"
#include "HakoniwaPlink_Oil.h"
#include "HakoniwaPLink_Laser.h"
#include "HakoniwaPLink_Prism.h"
#include "hakoVisPLink_Laser.h"
#include "hakoVisPLink_Oil.h"
#include "hakoVisPLink_Prism.h"

//konno Scenes
#include "dpHakoniwaIce.h"


class testApp : public ramBaseApp
{
public:

	// of methods
	// ------------------------
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	// ram methods
	// ------------------------
	void drawActor(const ramActor &actor);
	void drawRigid(const ramRigidBody &rigid);
	void onActorSetup(const ramActor &actor);
	void onActorExit(const ramActor &actor);
	void onRigidSetup(const ramRigidBody &rigid);
	void onRigidExit(const ramRigidBody &rigid);

	//test Scenes;
	

	//Parallel link
	HakoniwaPLink_Oil			hakoniwaPlinkOil;
	HakoniwaPLink_Laser			hakoniwaPlinkLaser;
	HakoniwaPLink_Prism			hakoniwaPlinkPrism;

	hakoVisPLink_Laser			hakoVisPlinkLaser;
	hakoVisPLink_Oil			hakoVisPlinkOil;
	hakoVisPLink_Prism			hakoVisPlinkPrism;

	//Theta
	HakoniwaTheta				hakoVisTheta;

    // servo pendulum
    dpHakoniwaServoPendulum     hakoniwaServoPendulum;
    dpHakoVisServoPendulum      visServoPendulum;
    
    // stage
    dpHakoVisStageCircle                visStage;
    
    // worm
    dpVisWorm             visWorm;

	//MagPendulum
	dpHakoniwaMagnetPendulum	magPendulum;
    dpHakoVisMagnetPendulum visMagnetPendulum;

    // struggle
    dpHakoniwaStruggle hakoniwaStruggle;
    dpHakoVisStruggle visStruggle;
    
    // sandstorm
    dpHakoniwaSand hakoniwaSand;
    dpHakoVisSandStorm visSandStorm;
    dpHakoVisVecLineCircle vecLineCircle;

    // ice
    dpHakoniwaIce hakoniwaIce;
    dp3DDanceMap danceMap;
    
    // tornado
    dpHakoniwaTornado hakoniwaTornado;
    dpHakoVisTornado  visTornado;

};