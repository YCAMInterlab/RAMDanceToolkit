#include "ramCameraManager.h"

#include "ramSharedData.h"
#include "ramUtils.h"

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
	
	loadDefaults();
}

void ramCameraManager::loadDefaults()
{
	const string &kCamSettingFile = ramToResourcePath("Settings/cam.default_positions.xml");
	ofxXmlSettings xml( kCamSettingFile );
	settings = ramCameraSettings::loadSettings(xml);
}

vector<string> ramCameraManager::getDefaultCameraNames()
{
	vector<string> names;
	for (int i = 0; i < settings.size(); i++)
	{
		names.push_back(settings[i].name);
	}
	return names;
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

void ramCameraManager::rollbackDefaultCameraSetting(int camera_id)
{
	const ramCameraSettings &setting = settings.at(camera_id);
	active_camera->setPosition(setting.pos);
	active_camera->lookAt(setting.look_at);
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

