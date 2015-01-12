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
const ramActorManager& ramGlobalShortcut::getActorManager() const { return ramActorManager::instance(); }

ramCommunicationManager& ramGlobalShortcut::getCommunicationManager() { return ramCommunicationManager::instance(); }
const ramCommunicationManager& ramGlobalShortcut::getCommunicationManager() const { return ramCommunicationManager::instance(); }

ramOscManager& ramGlobalShortcut::getOscManager() {return ramOscManager::instance(); }
const ramOscManager& ramGlobalShortcut::getOscManager() const {return ramOscManager::instance(); }

const vector<string>& ramGlobalShortcut::getNodeArrayNames() const { return ramActorManager::instance().getNodeArrayNames(); }

bool ramGlobalShortcut::hasNodeArray(const string &key) const { return ramActorManager::instance().hasNodeArray(key); }

ramNodeArray& ramGlobalShortcut::getNodeArray(const string &name) { return ramActorManager::instance().getNodeArray(name); }

const ramNodeArray& ramGlobalShortcut::getNodeArray(const string &name) const { return ramActorManager::instance().getNodeArray(name); }

size_t ramGlobalShortcut::getNumNodeArray() const { return ramActorManager::instance().getNumNodeArray(); }

ramNodeArray& ramGlobalShortcut::getNodeArray(int index) { return ramActorManager::instance().getNodeArray(index); }

const ramNodeArray& ramGlobalShortcut::getNodeArray(int index) const { return ramActorManager::instance().getNodeArray(index); }

vector<ramNodeArray> ramGlobalShortcut::getAllNodeArrays() const { return ramActorManager::instance().getAllNodeArrays(); }

ramCameraManager& ramGlobalShortcut::getCameraManager() { return ramCameraManager::instance(); }

ofCamera& ramGlobalShortcut::getActiveCamera() { return ramCameraManager::instance().getActiveCamera(); }

//ramSceneManager& ramGlobalShortcut::getSceneManager() { return ramSceneManager::instance(); }



#pragma mark - core
void ramInitialize(int oscPort, bool usePresetScenes)
{
	static bool inited = false;
	if (inited) return;

	inited = true;

	ram_simple_shadow.setup();

	ramOscManager::instance().setup(oscPort);

	ramActorManager::instance().setup();
	ramActorManager::instance().setupOscReceiver(&ramOscManager::instance());
	ramSceneManager::instance().setup();
	ramPhysics::instance();
	ramGetGUI().setup(usePresetScenes);

	ramCommunicationManager::instance().setup(&ramOscManager::instance());

}

string ramToResourcePath(const string& path)
{
	string path_prefix;

#if defined WIN32
	path_prefix = "../../../..";
#else
	path_prefix = "../..";
#endif
	return ofFilePath::join(ofToDataPath(path_prefix+"/resources"), path);
}


#pragma mark - actors
void ramEnableShowActors(bool v)
{
	ramSceneManager::instance().setShowAllActors(v);
}

bool ramShowActorsEnabled()
{
	return ramSceneManager::instance().getShowAllActors();	
}

ramNode _evilNode;
const ramNode& ramGetNode(unsigned int actorId, unsigned int jointId){
	
	const int numNA = ramActorManager::instance().getNumNodeArray();
	
	// if the actor does not exist...
	if (!(0 < numNA) || (numNA-1 <= actorId))
	{
		ofLogError("getRamNode()") << "the actor id " << actorId << " is not found. retruned evil node.";
		return _evilNode;
	}
	
	ramNodeArray &NA = ramActorManager::instance().getNodeArray(actorId);
	
	// if the joint does not exist...
	if (NA.getNumNode() >= jointId)
	{
		ofLogError("getRamNode()") << "the joint id " << jointId << " is greater than " << NA.getName() << "'s number of joints. retruned evil node.";
		return _evilNode;
	}
	
	return NA.getNode(jointId);
}

#pragma mark - camera

static ofRectangle _viewport;

void ramSetViewPort(ofRectangle viewport)
{
    _viewport = viewport;
}

ofRectangle ramGetViewPort()
{
    return _viewport;
}

void ramBeginCamera(ofRectangle viewport)
{
    ofRectangle v = viewport;
    if (v.isEmpty()) v = ofGetCurrentViewport();
	ramCameraManager::instance().getActiveCamera().begin(v);
}

void ramEndCamera()
{
	ramCameraManager::instance().getActiveCamera().end();
}

void ramEnableInteractiveCamera(bool v)
{
	ramCameraManager::instance().setEnableInteractiveCamera(v);
}



#pragma mark - shadows
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



#pragma mark - physics
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


#pragma mark - error
void ramNotImplementedError()
{
	ofLogWarning("RAM Dance Toolkit") << "not implemented yet";
}