#include "ramGlobal.h"

#include "ramSimpleShadow.h"
#include "ramPhysics.h"
#include "ramControlPanel.h"
#include "ramSceneManager.h"

static ramSimpleShadow ram_simple_shadow;

ramActorManager& ramGlobalShortcut::getActorManager() { return ramActorManager::instance(); }

const vector<string>& ramGlobalShortcut::getNodeArrayNames() { return ramActorManager::instance().getNodeArrayNames(); }

bool ramGlobalShortcut::hasNodeArray(const string &key) { return ramActorManager::instance().hasNodeArray(key); }

ramNodeArray& ramGlobalShortcut::getNodeArray(string name) { return ramActorManager::instance().getNodeArray(name); }

size_t ramGlobalShortcut::getNumNodeArray() { return ramActorManager::instance().getNumNodeArray(); }

ramNodeArray& ramGlobalShortcut::getNodeArray(int index) { return ramActorManager::instance().getNodeArray(index); }

vector<ramNodeArray> ramGlobalShortcut::getAllNodeArrays() { return ramActorManager::instance().getAllNodeArrays(); }

ramCameraManager& ramGlobalShortcut::getCameraManager() { return ramCameraManager::instance(); }

ofCamera& ramGlobalShortcut::getActiveCamera() { return ramCameraManager::instance().getActiveCamera(); }

//ramSceneManager& ramGlobalShortcut::getSceneManager() { return ramSceneManager::instance(); }


void ramInitialize(int oscPort)
{
	static bool inited = false;
	if (inited) return;

	inited = true;

	// !!!:
	ofLogWarning("ramInitialize(10000) in ramSharedData.cpp: ofSetLogLevel(OF_LOG_SILENT) will be deleted when Fonts probrem in ofxUI is fixed.");
	ofLogWarning("see addons/ofxUI.patch");
	// to apply `$ patch -p0 < ../ofxUI.patch` at ofxUI directory

	ram_simple_shadow.setup();
	
	ramActorManager::instance().setup();
	ramActorManager::instance().setupOscReceiver(oscPort);

	ramSceneManager::instance().setup();
	
	ramPhysics::instance();
	ramGetGUI().setup();
}

string ramToResourcePath(string path)
{
	return ofFilePath::join(ofToDataPath("../../../../resources"), path);
}

//

void ramEnableShowActors(bool v)
{
	ramSceneManager::instance().setShowAllActors(v);
}

bool ramShowActorsEnabled()
{
	return ramSceneManager::instance().getShowAllActors();	
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

//

// shadow

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

// physics

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

//

void ramNotImplementedError()
{
	ofLogWarning("RAM Dance Toolkit") << "not implemented yet";
}