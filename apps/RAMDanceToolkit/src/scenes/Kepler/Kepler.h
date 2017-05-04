// 
// Kepler.h - RAMDanceToolkit
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

//
//  Kepler.h
//  Kepler
//
//  Created by Onishi Yoshito on 2/18/13.
//
//

#ifndef Kepler_Kepler_h
#define Kepler_Kepler_h

#include "ramBaseScene.h"

#include "btPicker.h"
#include "KeplerBtDynamics.h"
#include "KeplerCube.h"
#include "btBulletDynamicsCommon.h"

class Kepler : public rdtk::BaseScene
{
public:
    // ------------------------
    void setupControlPanel();
	void setup();
	void update();
	void draw();
    
	// ------------------------
	void drawActor(const rdtk::Actor &actor);
    
    string getName() const { return "Kepler"; }
    
private:
    void onValueChanged(ofxUIEventArgs& e);
    
    // ------------------------
    void spawnKepler();
    void attach();
        
    KeplerBtDynamics mKeplerDynamics;
    
    btPicker mPickerA;
    btPicker mPickerB;
    
    int mNodeA,mNodeB;
    int mEdgeA,mEdgeB;
    
    map<string, int> mNamesNodeA;
    map<string, int> mNamesNodeB;
    map<string, int> mNamesEdgeA;
    map<string, int> mNamesEdgeB;
};

#endif
