#pragma once

#include "ofMain.h"

#include "ramActorManager.h"
#include "ramCameraManager.h"

struct ramGlobalShortcut
{

#pragma mark Shortcut to RamActorManager

	inline static ramActorManager& getActorManager() { return ramActorManager::instance(); }

	inline const vector<string>& getNodeArrayNames() { return ramActorManager::instance().getNodeArrayNames(); }

	inline static bool hasNodeArray(const string &key) { return ramActorManager::instance().hasNodeArray(key); }

	inline static ramNodeArray& getNodeArray(string name) { return ramActorManager::instance().getNodeArray(name); }

	inline static size_t getNumNodeArray() { return ramActorManager::instance().getNumNodeArray(); }

	inline static ramNodeArray& getNodeArray(int index) { return ramActorManager::instance().getNodeArray(index); }

	inline static vector<ramNodeArray> getAllNodeArrays() { return ramActorManager::instance().getAllNodeArrays(); }

#pragma mark Shortcut to RamCameraManager

	inline static ramCameraManager& getCameraManager() { return ramCameraManager::instance(); }

	inline static ofCamera& getActiveCamera() { return ramCameraManager::instance().getActiveCamera(); }

};

// core

void ramInitialize(int oscPort = 10000);
string ramToResourcePath(string path);

// camera

void ramBeginCamera();
void ramEndCamera();

void ramEnableInteractiveCamera(bool v);

// shadow

void ramEnableShadow(bool v = true);
void ramDisableShadow();
bool ramShadowEnabled();

void ramBeginShadow();
void ramEndShadow();
void ramSetShadowAlpha(float alpha);

// physics

void ramEnablePhysicsPrimitive(bool v = true);
void ramDisablePhysicsPrimitive();
bool ramGetEnablePhysicsPrimitive();

//

void ramNotImplementedError();