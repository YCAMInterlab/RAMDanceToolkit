// 
// ofApp.cpp - RAMDanceToolkit
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

#include "ofApp.h"

#pragma mark - oF methods
//--------------------------------------------------------------
void ofApp::setup()
{
	ofSetFrameRate(60);
	ofSetVerticalSync(true);
	
	/// ram setup
	// ------------------
	rdtk::Initialize(10000, true);
	
	/// scenes setup
	// ------------------
	rdtk::SceneManager& sceneManager = rdtk::SceneManager::instance();

	sceneManager.addScene<Extractor>();
	sceneManager.addScene<MovingCam>();
	sceneManager.addScene<LineDrawing>();
	sceneManager.addScene<BigBox>();
	sceneManager.addScene<Future>();
	sceneManager.addScene<Donuts>();
	sceneManager.addScene<Stamp>();
	sceneManager.addScene<Expansion>();
// ignore win32
#ifndef TARGET_WIN32
	sceneManager.addScene<Particles>();
#endif
	sceneManager.addScene<Abacus>();
	sceneManager.addScene<SoundCube>();
	sceneManager.addScene<UpsideDown>();
	sceneManager.addScene<HastyChase>();
	sceneManager.addScene<ColorGrid>();
	sceneManager.addScene<ThreePoints>();
	sceneManager.addScene<FourPoints>();
	sceneManager.addScene<Monster>();
	sceneManager.addScene<Laban>();
	sceneManager.addScene<Notation>();
#if !defined (DEBUG) && !defined (_DEBUG) // exclude from debug build
    sceneManager.addScene<Kepler>();
    sceneManager.addScene<Chain>();
#endif
	
}

//--------------------------------------------------------------
void ofApp::update()
{
	
}

//--------------------------------------------------------------
void ofApp::draw()
{
	
}



#pragma mark - ram methods
//--------------------------------------------------------------
void ofApp::drawActor(const rdtk::Actor &actor)
{
	
}

//--------------------------------------------------------------
void ofApp::drawRigid(const rdtk::RigidBody &rigid)
{
	
}

#pragma mark - ram Events

//--------------------------------------------------------------
void ofApp::onActorSetup(const rdtk::Actor &actor)
{
	
}

//--------------------------------------------------------------
void ofApp::onActorExit(const rdtk::Actor &actor)
{

}

//--------------------------------------------------------------
void ofApp::onRigidSetup(const rdtk::RigidBody &rigid)
{

}

//--------------------------------------------------------------
void ofApp::onRigidExit(const rdtk::RigidBody &rigid)
{

}



#pragma mark - of Event
//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
	
}
