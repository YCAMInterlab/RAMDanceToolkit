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
/*!
 Scenes
 */
#include "HakoniwaOscTest.h"
#include "HakoniwaColorOfWater.h"
#include "HakovisLaser.h"

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
    
    HakoniwaOscTest hakoniwaOscTest;
    HakoniwaColorOfWater hakoniwaColorOfWater;
    HakovisLaser hakovisLaser;
    
    struct : public ramBaseScene {
        void draw()
        {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(ofColor::cyan, 128);
            ofDrawBox(ofVec3f::zero(), 100.f);
            ofPopStyle();
        }
        
        void drawActor(const ramActor &actor)
        {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(ofColor::cyan, 128);
            ramDrawBasicActor(actor);
            ofPopStyle();
        }
        string getName() const { return "TestSceneA"; }
    } testSceneA;
    
    struct : public ramBaseScene {
        void draw()
        {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(ofColor::magenta, 128);
            ofDrawBox(ofVec3f::zero(), 100.f);
            ofPopStyle();
        }
        
        void drawActor(const ramActor &actor)
        {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(ofColor::magenta, 128);
            ramDrawBasicActor(actor);
            ofPopStyle();
        }
        string getName() const { return "TestSceneB"; }
    } testSceneB;
    
    struct : public ramBaseScene {
        void draw()
        {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(ofColor::yellow, 128);
            ofDrawBox(ofVec3f::zero(), 100.f);
            ofPopStyle();
        }
        
        void drawActor(const ramActor &actor)
        {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(ofColor::yellow, 128);
            ramDrawBasicActor(actor);
            ofPopStyle();
        }
        string getName() const { return "TestSceneC"; }
    } testSceneC;
    
    struct : public ramBaseScene {
        
        void draw()
        {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(ofColor::white, 128);
            ofDrawBox(ofVec3f::zero(), 100.f);
            ofPopStyle();
        }
        
        void drawActor(const ramActor &actor)
        {
            ofPushStyle();
            ofEnableAlphaBlending();
            ofSetColor(ofColor::white, 128);
            ramDrawBasicActor(actor);
            ofPopStyle();
        }
        string getName() const { return "TestSceneD"; }
    } testSceneD;
};