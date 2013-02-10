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

void ramBeginCamera()
{
	ramCameraManager::instance().getActiveCamera().begin();
}

void ramEndCamera()
{
	ramCameraManager::instance().getActiveCamera().end();
}