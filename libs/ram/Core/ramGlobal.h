#pragma once

#include "ofMain.h"

#include "ramActorManager.h"
#include "ramCameraManager.h"
//#include "ramSceneManager.h"

class ramGlobalShortcut
{
public:
	
#pragma mark - Shortcut to ramActorManager

	ramActorManager& getActorManager();
	const vector<string>& getNodeArrayNames();
	bool hasNodeArray(const string &key);
	ramNodeArray& getNodeArray(string name);
	size_t getNumNodeArray();
	ramNodeArray& getNodeArray(int index);
	vector<ramNodeArray> getAllNodeArrays();
	
#pragma mark - Shortcut to ramCameraManager

	ramCameraManager& getCameraManager();
	ofCamera& getActiveCamera();
	
#pragma mark - Shortcut to ramSceneManager

	//ramSceneManager& getSceneManager();
	
};

#pragma mark - core
void ramInitialize(int oscPort = 10000);
string ramToResourcePath(string path);

#pragma mark - actors
void ramEnableShowActors(bool v);
bool ramShowActorsEnabled();

#pragma mark - camera
void ramBeginCamera();
void ramEndCamera();
void ramEnableInteractiveCamera(bool v);

#pragma mark - shoadws
void ramEnableShadow(bool v = true);
void ramDisableShadow();
bool ramShadowEnabled();

void ramBeginShadow();
void ramEndShadow();
void ramSetShadowAlpha(float alpha);

#pragma mark - physics
void ramEnablePhysicsPrimitive(bool v = true);
void ramDisablePhysicsPrimitive();
bool ramGetEnablePhysicsPrimitive();

#pragma mark - error
void ramNotImplementedError();
