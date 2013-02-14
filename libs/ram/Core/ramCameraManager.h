#pragma once

#include "ofMain.h"

#include "ofxXmlSettings.h"
#include "ramCameraSettings.h"

class ramCameraManager
{
public:
	
	static ramCameraManager& instance();
	
	void loadDefaults();
	
	inline size_t getNumCameras() { return cameras.size(); }
	ofCamera& getActiveCamera() { return *active_camera; }
	inline void setActiveCamera(int index) { active_camera = cameras.at(index); }
	
	template <typename T>
	T* createCamera()
	{
		T *o = new T;
		cameras.push_back(o);
		return o;
	}
	
	void setEnableInteractiveCamera(bool v);
	
	
	// defaults
	vector<string> getDefaultCameraNames();
	void rollbackDefaultCameraSetting(int camera_id);
	
protected:
	
	static ramCameraManager *_instance;
	
	ofCamera *active_camera;
	vector<ofCamera*> cameras;
	
	ramCameraManager();
	ramCameraManager(const ramCameraManager&) {}
	ramCameraManager& operator=(const ramCameraManager&) { return *this; }
	
	vector<ramCameraSettings> settings;
	
};

void ramBeginCamera();
void ramEndCamera();

void ramEnableInteractiveCamera(bool v);