//
// EmptyScene.h - RAMDanceToolkit
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

#include "EmptyScene.h"

void EmptyScene::setupControlPanel()
{
    ramGetGUI().addSlider("Slider", 0.0, 10.0, &mySlider);
    ramGetGUI().addToggle("Toggle", &myToggle);
    
    ofAddListener(ramGetGUI().getCurrentUIContext()->newGUIEvent, this, &EmptyScene::onPanelChanged);
}

void EmptyScene::setup()
{
    
}

void EmptyScene::update()
{
    
}

void EmptyScene::draw()
{
    ofColor color( myToggle ? 150 : 50 );
    ofSetColor( color );
    
    ramBeginCamera();
    ofDrawBitmapString( "Hello, "+getName()+ "!" + " Slider value is " + ofToString(mySlider), ofVec3f(0,200,0) );
    ramEndCamera();
}

void EmptyScene::onPanelChanged(ofxUIEventArgs &e)
{
    const string name = e.widget->getName();
    
    // do something...
}


void EmptyScene::drawActor(const ramActor& actor)
{
    
}

void EmptyScene::drawRigid(const ramRigidBody &rigid)
{
    
}

void EmptyScene::onActorSetup(const ramActor &actor)
{
    
}

void EmptyScene::onActorExit(const ramActor &actor)
{
    
}

void EmptyScene::onRigidSetup(const ramRigidBody &rigid)
{
    
}

void EmptyScene::onRigidExit(const ramRigidBody &rigid)
{
    
}
