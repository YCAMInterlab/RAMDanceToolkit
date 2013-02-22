#pragma once

#include "ofMain.h"

#include "ramActorManager.h"
#include "ramCameraManager.h"
#include "ramSharedData.h"

struct ramGlobalShortcut
{
	
#pragma mark -
#pragma mark Shortcut to RamActorManager
	
	
	inline static ramActorManager& getActorManager() { return ramActorManager::instance(); }
	
	inline static bool hasNodeArray(const string &key) { return ramActorManager::instance().hasNodeArray(key); }
	
	inline static ramNodeArray& getNodeArray(string name) { return ramActorManager::instance().getNodeArray(name); }
	
	inline static size_t getNumNodeArray() { return ramActorManager::instance().getNumNodeArray(); }
	
	inline static ramNodeArray& getNodeArray(int index) { return ramActorManager::instance().getNodeArray(index); }

	inline static vector<ramNodeArray> getAllNodeArrays() { return ramActorManager::instance().getAllNodeArrays(); }
	
#pragma mark -
#pragma mark Shortcut to RamCameraManager
	
	inline static ramCameraManager& getCameraManager() { return ramCameraManager::instance(); }

	inline static ofCamera& getActiveCamera() { return ramCameraManager::instance().getActiveCamera(); }
	
};