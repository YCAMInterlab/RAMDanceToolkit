// 
// ramGlobal.h - RAMDanceToolkit
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

#include "ramActorManager.h"
#include "ramCameraManager.h"
#include "ramCommunicationManager.h"
#include "ramOscManager.h"
//#include "ramSceneManager.h"

class ramGlobalShortcut
{
public:
	
#pragma mark - Shortcut to ramActorManager

	ramActorManager& getActorManager();
    const ramActorManager& getActorManager() const;
	ramCommunicationManager& getCommunicationManager();
    const ramCommunicationManager& getCommunicationManager() const;
	ramOscManager& getOscManager();
    const ramOscManager& getOscManager() const;

	const vector<string>& getNodeArrayNames() const;
	bool hasNodeArray(const string& key) const;
	ramNodeArray& getNodeArray(const string& name);
    const ramNodeArray& getNodeArray(const string& name) const;
	size_t getNumNodeArray() const;
	ramNodeArray& getNodeArray(int index);
    const ramNodeArray& getNodeArray(int index) const;
	vector<ramNodeArray> getAllNodeArrays() const;
	
#pragma mark - Shortcut to ramCameraManager

	ramCameraManager& getCameraManager();
    inline const ramCameraManager& getCameraManager() const {
        return const_cast<ramCameraManager&>(getCameraManager());
    }
	ofCamera& getActiveCamera();
    inline const ofCamera& getActiveCamera() const {
        return const_cast<ofCamera&>(getActiveCamera());
    }
	
#pragma mark - Shortcut to ramSceneManager

	//ramSceneManager& getSceneManager();
	
};

#pragma mark - core
void ramInitialize(int oscPort = 10000, bool usePresetScenes = false);
string ramToResourcePath(const string& path);

#pragma mark - actors
void ramEnableShowActors(bool v);
bool ramShowActorsEnabled();
const ramNode& ramGetNode(unsigned int actorId, unsigned int jointId);

#pragma mark - camera
void ramSetViewPort(ofRectangle viewport);
ofRectangle ramGetViewPort();
void ramBeginCamera(ofRectangle viewport = ramGetViewPort());
void ramEndCamera();
void ramEnableInteractiveCamera(bool v);

#pragma mark - shadows
void ramEnableShadow(bool v = true);
void ramDisableShadow();
bool ramShadowEnabled();

void ramBeginShadow();
void ramEndShadow();
void ramSetShadowAlpha(float alpha);

#pragma mark - physics
void ramEnablePhysicsPrimitive(bool v = true);
void ramDisablePhysicsPrimitive();
bool ramGetEnablePhysicsPrimitive();

#pragma mark - error
void ramNotImplementedError();
