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

using namespace rdtk;

static SimpleShadow ram_simple_shadow;

ActorManager& GlobalShortcut::getActorManager() { return ActorManager::instance(); }
const ActorManager& GlobalShortcut::getActorManager() const { return ActorManager::instance(); }

CommunicationManager& GlobalShortcut::getCommunicationManager() { return CommunicationManager::instance(); }
const CommunicationManager& GlobalShortcut::getCommunicationManager() const { return CommunicationManager::instance(); }

OscManager& GlobalShortcut::getOscManager() {return OscManager::instance(); }
const OscManager& GlobalShortcut::getOscManager() const {return OscManager::instance(); }

const vector<string>& GlobalShortcut::getNodeArrayNames() const { return ActorManager::instance().getNodeArrayNames(); }

bool GlobalShortcut::hasNodeArray(const string &key) const { return ActorManager::instance().hasNodeArray(key); }

rdtk::NodeArray& GlobalShortcut::getNodeArray(const string &name) { return ActorManager::instance().getNodeArray(name); }

const rdtk::NodeArray& GlobalShortcut::getNodeArray(const string &name) const { return ActorManager::instance().getNodeArray(name); }

size_t GlobalShortcut::getNumNodeArray() const { return ActorManager::instance().getNumNodeArray(); }

rdtk::NodeArray& GlobalShortcut::getNodeArray(int index) { return ActorManager::instance().getNodeArray(index); }

const rdtk::NodeArray& GlobalShortcut::getNodeArray(int index) const { return ActorManager::instance().getNodeArray(index); }

vector<rdtk::NodeArray> GlobalShortcut::getAllNodeArrays() const { return ActorManager::instance().getAllNodeArrays(); }

CameraManager& GlobalShortcut::getCameraManager() { return CameraManager::instance(); }

ofCamera& GlobalShortcut::getActiveCamera() { return CameraManager::instance().getActiveCamera(); }

//SceneManager& GlobalShortcut::getSceneManager() { return SceneManager::instance(); }


#pragma mark - core
void rdtk::Initialize(int oscPort, bool usePresetScenes)
{
	static bool inited = false;
	if (inited) return;

	inited = true;

	ram_simple_shadow.setup();

	OscManager::instance().setup(oscPort);

	ActorManager::instance().setup();
	ActorManager::instance().setupOscReceiver(&OscManager::instance());
	SceneManager::instance().setup();
	Physics::instance();
	GetGUI().setup(usePresetScenes);

	CommunicationManager::instance().setup(&OscManager::instance());

}

string rdtk::ToResourcePath(const string& path)
{
	string base_path = "resources";
    bool dirExists = false;
    
    const int maxDepth = 5;
    for (int i=0; i<maxDepth; i++) {
        ofDirectory dir(base_path);
        if (dir.exists()) {
            dirExists = true;
            break;
        }
        
        string tmpPath = ofToDataPath(base_path);
        ofDirectory dirFromDataPath(tmpPath);
        if (dirFromDataPath.exists()) {
            base_path = tmpPath;
            dirExists = true;
            break;
        }
        base_path = "../" + base_path;
    }
    
    if (!dirExists) {
        ofLogError() << "resources folder dosen't found";
    }
    
	return ofFilePath::join(base_path, path);
}


#pragma mark - actors
void rdtk::EnableShowActors(bool v)
{
	SceneManager::instance().setShowAllActors(v);
}

bool rdtk::ShowActorsEnabled()
{
	return SceneManager::instance().getShowAllActors();	
}

Node _evilNode;
const Node& rdtk:: GetNode(unsigned int actorId, unsigned int jointId) {
	
	const int numNA = ActorManager::instance().getNumNodeArray();
	
	// if the actor does not exist...
	if (!(0 < numNA) || (numNA-1 <= actorId))
	{
		ofLogError("getRamNode()") << "the actor id " << actorId << " is not found. retruned evil node.";
		return _evilNode;
	}
	
	rdtk::NodeArray &NA = ActorManager::instance().getNodeArray(actorId);
	
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

void rdtk::SetViewPort(ofRectangle viewport)
{
    _viewport = viewport;
}

ofRectangle rdtk::GetViewPort()
{
    return _viewport;
}

void rdtk::BeginCamera(ofRectangle viewport)
{
    ofRectangle v = viewport;
    if (v.isEmpty()) v = ofGetCurrentViewport();
	CameraManager::instance().getActiveCamera().begin(v);
}

void rdtk::EndCamera()
{
	CameraManager::instance().getActiveCamera().end();
}

void rdtk::EnableInteractiveCamera(bool v)
{
	CameraManager::instance().setEnableInteractiveCamera(v);
}


#pragma mark - shadows
void rdtk::EnableShadow(bool v)
{
	ram_simple_shadow.setEnable(v);
}

void rdtk::DisableShadow()
{
	ram_simple_shadow.setEnable(false);
}

bool rdtk::ShadowEnabled()
{
	return ram_simple_shadow.getEnable();
}

void rdtk::BeginShadow()
{
	ram_simple_shadow.begin();
}

void rdtk::EndShadow()
{
	ram_simple_shadow.end();
}

void rdtk::SetShadowAlpha(float alpha)
{
	ram_simple_shadow.setShadowAlpha(alpha);
}



#pragma mark - physics
static bool ram_enable_physics_primitive = true;

void rdtk::EnablePhysicsPrimitive(bool v)
{
	ram_enable_physics_primitive = v;
}

void rdtk::DisablePhysicsPrimitive()
{
	ram_enable_physics_primitive = false;
}

bool rdtk::GetEnablePhysicsPrimitive()
{
	return ram_enable_physics_primitive;
}


#pragma mark - error
void rdtk::NotImplementedError()
{
	ofLogWarning("RAM Dance Toolkit") << "not implemented yet";
}

void ramInitialize(int oscPort, bool usePresetScenes){
	rdtk::Initialize(oscPort, usePresetScenes);
}

string ramToResourcePath(const string& path){
	return rdtk::ToResourcePath(path);
}

void ramEnableShowActors(bool v){
	rdtk::EnableShowActors(v);
}
bool ramShowActorsEnabled(){
	return rdtk::ShowActorsEnabled();
}
const ramNode& ramGetNode(unsigned int actorId, unsigned int jointId){
	return rdtk::GetNode(actorId, jointId);
}

void ramSetViewPort(ofRectangle viewport){rdtk::SetViewPort(viewport);}
ofRectangle ramGetViewPort(){return rdtk::GetViewPort();}
void ramBeginCamera(ofRectangle viewport){rdtk::BeginCamera(viewport);}
void ramEndCamera(){rdtk::EndCamera();}
void ramEnableInteractiveCamera(bool v){rdtk::EnableInteractiveCamera(v);}

void ramEnableShadow(bool v){rdtk::EnableShadow(v);}
void ramDisableShadow(){rdtk::DisableShadow();}
bool ramShadowEnabled(){return rdtk::ShadowEnabled();}

void ramBeginShadow(){rdtk::BeginShadow();}
void ramEndShadow(){rdtk::EndShadow();}
void ramSetShadowAlpha(float alpha){rdtk::SetShadowAlpha(alpha);}

void ramEnablePhysicsPrimitive(bool v){rdtk::EnablePhysicsPrimitive(v);}
void ramDisablePhysicsPrimitive(){rdtk::DisablePhysicsPrimitive();}
bool ramGetEnablePhysicsPrimitive(){return rdtk::GetEnablePhysicsPrimitive();}

void ramNotImplementedError(){rdtk::NotImplementedError();}
