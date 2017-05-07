// 
// ramCameraManager.cpp - RAMDanceToolkit
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

#include "ramCameraManager.h"

#include "ramGlobal.h"

using namespace rdtk;

CameraManager* CameraManager::_instance = NULL;

CameraManager& CameraManager::instance()
{
	if (_instance == NULL)
		_instance = new CameraManager;
	return *_instance;
}

CameraManager::CameraManager()
{
	last_camera_id = 0;
	
	active_camera = createCamera<ofEasyCam>();

	loadDefaults();
	
	ofAddListener(ofEvents().update, this, &CameraManager::update);
}

void CameraManager::loadDefaults()
{
	const string &kCamSettingFile = ToResourcePath("Settings/camera/cam.default_positions.xml");
	ofxXmlSettings xml(kCamSettingFile);
	settings = CameraSettings::loadSettings(xml);
}

vector<string> CameraManager::getDefaultCameraNames() const
{
	vector<string> names;
	for (int i = 0; i < settings.size(); i++)
	{
		names.push_back(settings[i].name);
	}
	return names;
}

void CameraManager::setEnableInteractiveCamera(bool v)
{
	if (typeid(*active_camera) == typeid(ofEasyCam))
	{
		ofEasyCam *cam = (ofEasyCam*)active_camera;

		if (v) cam->enableMouseInput();
		else cam->disableMouseInput();
	}
}

bool CameraManager::getEnableInteractiveCamera()
{
	if (typeid(*active_camera) == typeid(ofEasyCam))
	{
		ofEasyCam *cam = (ofEasyCam*)active_camera;
		
		return cam->getMouseInputEnabled();
	}
	return false;
}

void CameraManager::rollbackDefaultCameraSetting(int camera_id)
{
	if(camera_id == -1) camera_id = last_camera_id;
	const CameraSettings &setting = settings.at(camera_id);
	active_camera->setPosition(setting.pos);
	active_camera->lookAt(setting.look_at);
	active_camera->setFov(setting.fov);
	last_camera_id = camera_id;
}

void CameraManager::update(ofEventArgs& args)
{
    if (ofGetFrameNum() < 2) rollbackDefaultCameraSetting();
    
	if (typeid(*active_camera) == typeid(ofEasyCam))
	{
		ofEasyCam *cam = (ofEasyCam*)active_camera;
		ofVec3f position = cam->getPosition();
		ofVec3f basePosition = ofVec3f(0, 0, cam->getDistance());
		if(position == basePosition) {
			rollbackDefaultCameraSetting();
		}
	}
}
