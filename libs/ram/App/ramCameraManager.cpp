#include "ramCameraManager.h"

ramCameraManager* ramCameraManager::_instance = NULL;

ramCameraManager& ramCameraManager::instance()
{
	if (_instance == NULL)
		_instance = new ramCameraManager;
	return *_instance;
}

ramCameraManager::ramCameraManager()
{
	active_camera = createCamera<ofEasyCam>();
}

void ramCameraManager::setEnableInteractiveCamera(bool v)
{
	if (typeid(*active_camera) == typeid(ofEasyCam))
	{
		ofEasyCam *cam = (ofEasyCam*)active_camera;
		
		if (v) cam->enableMouseInput();
		else cam->disableMouseInput();
	}
}

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