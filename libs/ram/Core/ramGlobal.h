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

namespace rdtk{
	class GlobalShortcut
	{
	public:
		
#pragma mark - Shortcut to ActorManager
		
		ActorManager&				getActorManager();
		const ActorManager&			getActorManager() const;
		CommunicationManager&		getCommunicationManager();
		const CommunicationManager& getCommunicationManager() const;
		OscManager&					getOscManager();
		const OscManager&			getOscManager() const;
		
		const vector<string>& getNodeArrayNames() const;
		bool hasNodeArray(const string& key) const;
		NodeArray& getNodeArray(const string& name);
		const NodeArray& getNodeArray(const string& name) const;
		size_t getNumNodeArray() const;
		NodeArray& getNodeArray(int index);
		const NodeArray& getNodeArray(int index) const;
		vector<NodeArray> getAllNodeArrays() const;
		
#pragma mark - Shortcut to CameraManager
		
		CameraManager& getCameraManager();
		inline const CameraManager& getCameraManager() const {
			return const_cast<CameraManager&>(getCameraManager());
		}
		ofCamera& getActiveCamera();
		inline const ofCamera& getActiveCamera() const {
			return const_cast<ofCamera&>(getActiveCamera());
		}
		
#pragma mark - Shortcut to SceneManager
		
		//SceneManager& getSceneManager();
		
	};
	
#pragma mark - core
	void Initialize(int oscPort = 10000, bool usePresetScenes = false);
	string ToResourcePath(const string& path);
	
#pragma mark - actors
	void EnableShowActors(bool v);
	bool ShowActorsEnabled();
	const Node& GetNode(unsigned int actorId, unsigned int jointId);
	
#pragma mark - camera
	void SetViewPort(ofRectangle viewport);
	ofRectangle GetViewPort();
	void BeginCamera(ofRectangle viewport = GetViewPort());
	void EndCamera();
	void EnableInteractiveCamera(bool v);
	
#pragma mark - shadows
	void EnableShadow(bool v = true);
	void DisableShadow();
	bool ShadowEnabled();
	
	void BeginShadow();
	void EndShadow();
	void SetShadowAlpha(float alpha);
	
#pragma mark - physics
	void EnablePhysicsPrimitive(bool v = true);
	void DisablePhysicsPrimitive();
	bool GetEnablePhysicsPrimitive();
	
#pragma mark - error
	void NotImplementedError();

}

typedef rdtk::GlobalShortcut RAMDEPRECATED(ramGlobalShortcut);

OF_DEPRECATED(void ramInitialize(int oscPort = 10000, bool usePresetScenes = false));
OF_DEPRECATED(string ramToResourcePath(const string& path));
OF_DEPRECATED(void ramEnableShowActors(bool v));
OF_DEPRECATED(bool ramShowActorsEnabled());
OF_DEPRECATED(const ramNode& ramGetNode(unsigned int actorId, unsigned int jointId));

OF_DEPRECATED(void ramSetViewPort(ofRectangle viewport));
OF_DEPRECATED(ofRectangle ramGetViewPort());
OF_DEPRECATED(void ramBeginCamera(ofRectangle viewport = ramGetViewPort()));
OF_DEPRECATED(void ramEndCamera());
OF_DEPRECATED(void ramEnableInteractiveCamera(bool v));

OF_DEPRECATED(void ramEnableShadow(bool v = true));
OF_DEPRECATED(void ramDisableShadow());
OF_DEPRECATED(bool ramShadowEnabled());

OF_DEPRECATED(void ramBeginShadow());
OF_DEPRECATED(void ramEndShadow());
OF_DEPRECATED(void ramSetShadowAlpha(float alpha));

OF_DEPRECATED(void ramEnablePhysicsPrimitive(bool v = true));
OF_DEPRECATED(void ramDisablePhysicsPrimitive());
OF_DEPRECATED(bool ramGetEnablePhysicsPrimitive());

OF_DEPRECATED(void ramNotImplementedError());
