// 
// ramGlobal.cpp - RAMDanceToolkit
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

#include "ramGlobal.h"

#include "ramSimpleShadow.h"
#include "ramPhysics.h"
#include "ramControlPanel.h"
#include "ramSceneManager.h"

static ramSimpleShadow ram_simple_shadow;

ramActorManager& ramGlobalShortcut::getActorManager() { return ramActorManager::instance(); }

const vector<string>& ramGlobalShortcut::getNodeArrayNames() { return ramActorManager::instance().getNodeArrayNames(); }

bool ramGlobalShortcut::hasNodeArray(const string &key) { return ramActorManager::instance().hasNodeArray(key); }

ramNodeArray& ramGlobalShortcut::getNodeArray(string name) { return ramActorManager::instance().getNodeArray(name); }

size_t ramGlobalShortcut::getNumNodeArray() { return ramActorManager::instance().getNumNodeArray(); }

ramNodeArray& ramGlobalShortcut::getNodeArray(int index) { return ramActorManager::instance().getNodeArray(index); }

vector<ramNodeArray> ramGlobalShortcut::getAllNodeArrays() { return ramActorManager::instance().getAllNodeArrays(); }

ramCameraManager& ramGlobalShortcut::getCameraManager() { return ramCameraManager::instance(); }

ofCamera& ramGlobalShortcut::getActiveCamera() { return ramCameraManager::instance().getActiveCamera(); }

//ramSceneManager& ramGlobalShortcut::getSceneManager() { return ramSceneManager::instance(); }


void ramInitialize(int oscPort, bool usePresetScenes)
{
	static bool inited = false;
	if (inited) return;

	inited = true;

	ram_simple_shadow.setup();
	ramActorManager::instance().setup();
	ramActorManager::instance().setupOscReceiver(oscPort);
	ramSceneManager::instance().setup();
	ramPhysics::instance();
	ramGetGUI().setup(usePresetScenes);
}

string ramToResourcePath(string path)
{
	return ofFilePath::join(ofToDataPath("../../../../resources"), path);
}

//

void ramEnableShowActors(bool v)
{
	ramSceneManager::instance().setShowAllActors(v);
}

bool ramShowActorsEnabled()
{
	return ramSceneManager::instance().getShowAllActors();	
}

//

void ramBeginCamera()
{
	ramCameraManager::instance().getActiveCamera().begin();
}

void ramEndCamera()
{
	ramCameraManager::instance().getActiveCamera().end();
}

void ramEnableInteractiveCamera(bool v)
{
	ramCameraManager::instance().setEnableInteractiveCamera(v);
}

//

// shadow

void ramEnableShadow(bool v)
{
	ram_simple_shadow.setEnable(v);
}

void ramDisableShadow()
{
	ram_simple_shadow.setEnable(false);
}

bool ramShadowEnabled()
{
	return ram_simple_shadow.getEnable();
}

void ramBeginShadow()
{
	ram_simple_shadow.begin();
}

void ramEndShadow()
{
	ram_simple_shadow.end();
}

void ramSetShadowAlpha(float alpha)
{
	ram_simple_shadow.setShadowAlpha(alpha);
}

// physics

static bool ram_enable_physics_primitive = true;

void ramEnablePhysicsPrimitive(bool v)
{
	ram_enable_physics_primitive = v;
}

void ramDisablePhysicsPrimitive()
{
	ram_enable_physics_primitive = false;
}

bool ramGetEnablePhysicsPrimitive()
{
	return ram_enable_physics_primitive;
}

//

void ramNotImplementedError()
{
	ofLogWarning("RAM Dance Toolkit") << "not implemented yet";
}