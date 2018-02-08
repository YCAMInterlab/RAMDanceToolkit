// 
// ofApp.h - RAMDanceToolkit
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

// Include scenes
#include "MovingCam.h"
#include "LineDrawing.h"
#include "BigBox.h"
#include "Future.h"

#include "Donuts.h"
#include "Stamp.h"
#include "Expansion.h"
#include "Particles.h"

#include "Abacus.h"
#include "SoundCube.h"
#include "UpsideDown.h"
#include "Kepler.h"

#include "HastyChase.h"
#include "ColorGrid.h"
#include "ThreePoints.h"
#include "FourPoints.h"

#include "Chain.h"
#include "Monster.h"
#include "Laban.h"
#include "Notation.h"

#include "Extractor.h"

class ofApp : public rdtk::BaseApp
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
	void drawActor(const rdtk::Actor &actor);
	void drawRigid(const rdtk::RigidBody &rigid);
	void onActorSetup(const rdtk::Actor &actor);
	void onActorExit(const rdtk::Actor &actor);
	void onRigidSetup(const rdtk::RigidBody &rigid);
	void onRigidExit(const rdtk::RigidBody &rigid);

};
