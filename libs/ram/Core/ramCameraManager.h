// 
// ramCameraManager.h - RAMDanceToolkit
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

#include "ofxXmlSettings.h"
#include "ramCameraSettings.h"
#include "ramConstants.h"

namespace rdtk{

	class CameraManager
	{
	public:
		
		static CameraManager& instance();
		
		inline size_t getNumCameras() const { return cameras.size(); }
		ofCamera* getCamera(size_t index) { return cameras[index]; }
		const ofCamera* getCamera(size_t index) const { return const_cast<ofCamera*>(cameras[index]); }
		
		ofCamera& getActiveCamera() { return *active_camera; }
		const ofCamera& getActiveCamera() const { return const_cast<ofCamera&>(*active_camera); }
		inline void setActiveCamera(int index) { active_camera = cameras.at(index); }
		
		template <typename T>
		T* createCamera()
		{
			T *o = new T;
			cameras.push_back(o);
			return o;
		}
		
		void setEnableInteractiveCamera(bool v);
		bool getEnableInteractiveCamera();
		
		// defaults
		void loadDefaults();
		
		vector<string> getDefaultCameraNames() const;
		void rollbackDefaultCameraSetting(int camera_id = -1);
		
	protected:
		
		static CameraManager *_instance;
		CameraManager();
		CameraManager(const CameraManager&) {}
		CameraManager& operator=(const CameraManager&) { return *this; }
		
		ofCamera *active_camera;
		vector<ofCamera*> cameras;
		
		void update(ofEventArgs& args);
		
		vector<CameraSettings> settings;
		
		int last_camera_id;
	};
}

typedef rdtk::CameraManager RAMDEPRECATED(ramCameraManager);
