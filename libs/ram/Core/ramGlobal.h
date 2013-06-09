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
//#include "ramSceneManager.h"

class ramGlobalShortcut
{
public:
	
#pragma mark - Shortcut to ramActorManager

	ramActorManager& getActorManager();
	const vector<string>& getNodeArrayNames();
	bool hasNodeArray(const string &key);
	ramNodeArray& getNodeArray(string name);
	size_t getNumNodeArray();
	ramNodeArray& getNodeArray(int index);
	vector<ramNodeArray> getAllNodeArrays();
	
#pragma mark - Shortcut to ramCameraManager

	ramCameraManager& getCameraManager();
	ofCamera& getActiveCamera();
	
#pragma mark - Shortcut to ramSceneManager

	//ramSceneManager& getSceneManager();
	
};

#pragma mark - core
void ramInitialize(int oscPort = 10000, bool usePresetScenes = false);
string ramToResourcePath(string path);

#pragma mark - actors
void ramEnableShowActors(bool v);
bool ramShowActorsEnabled();

#pragma mark - camera
void ramBeginCamera();
void ramEndCamera();
void ramEnableInteractiveCamera(bool v);

#pragma mark - shoadws
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
