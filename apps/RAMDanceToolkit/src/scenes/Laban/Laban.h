// 
// Laban.h - RAMDanceToolkit
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

#include "ramMain.h"

class LabanMoment
{
public:
    ofVec3f start, direction;
    int choice;
    float birth;
    float _maxLabanMomentLife;
    
    LabanMoment(ofVec3f start, ofVec3f direction, int choice)
    :start(start)
    ,direction(direction)
    ,choice(choice)
    {
        birth = ofGetElapsedTimef();
    }
    float getLife(float & maxLabanMomentLife)
    {
        _maxLabanMomentLife = maxLabanMomentLife;
        return 1. - ((ofGetElapsedTimef() - birth) / maxLabanMomentLife);
    }
    bool isDead()
    {
        return getLife(_maxLabanMomentLife) < 0;
    }
};

class Laban : public rdtk::BaseScene
{
public:

    string getName() const { return "Laban"; }

    void setupControlPanel();
	void drawImGui();
    void setup();
    void update();
    void draw();
    void drawActor(const rdtk::Actor &actor);
    void drawRigid(rdtk::RigidBody &rigid);

    float threshold, lineWidth, lineLength, scale, ticks;
    bool showLines, showPlanes, onlyLimbs;
    
    vector<ofColor> labanColors;
    vector<ofVec3f> labanDirections;
    
    float maxLabanMomentLife;
    
    list<LabanMoment> moments;
	
};
