#pragma once

#include "ofMain.h"

class ramCameraManager
{
public:
	
	static ramCameraManager& instance();
	
	inline size_t getNumCameras() { return cameras.size(); }
	ofCamera& getActiveCamera() { return *active_camera; }
	inline void setActiveCamera(int index) { active_camera = cameras.at(index); }
	
	template <typename T>
	T* createCamera()
	{
		T *o = new T;
		cameras.push_back(o);
		active_camera = o;
		return o;
	}
	
	
	
	
	
protected:
	
	static ramCameraManager *_instance;
	
	ofCamera *active_camera;
	vector<ofCamera*> cameras;
	
	ramCameraManager();
	ramCameraManager(const ramCameraManager&) {}
	ramCameraManager& operator=(const ramCameraManager&) { return *this; }
	
};

void ramCameraBegin();
void ramCameraEnd();