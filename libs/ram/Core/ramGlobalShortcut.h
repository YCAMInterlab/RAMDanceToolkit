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
	
	//	inline bool hasActor(const string &key) { return ramActorManager::instance().hasActor(key); }
	//	inline bool hasRigidBody(const string &key) { return ramActorManager::instance().hasRigidBody(key); }
	inline static bool hasNodeArray(const string &key) { return ramActorManager::instance().hasNodeArray(key); }
	
	//	inline ramActor& getActor(string name) { return ramActorManager::instance().getActor(name); }
	//	inline ramRigidBody& getRigidBody(string name) { return ramActorManager::instance().getRigidBody(name); }
	inline static ramNodeArray& getNodeArray(string name) { return ramActorManager::instance().getNodeArray(name); }
	
	//	inline size_t getNumActor() { return ramActorManager::instance().getNumActor(); }
	//	inline size_t getNumRigidBody() { return ramActorManager::instance().getNumRigidBody(); }
	inline static size_t getNumNodeArray() { return ramActorManager::instance().getNumNodeArray(); }
	
	//	inline ramActor& getActor(int index) { return ramActorManager::instance().getActor(index); }
	//	inline ramRigidBody& getRigidBody(int index) { return ramActorManager::instance().getRigidBody(index); }
	inline static ramNodeArray& getNodeArray(int index) { return ramActorManager::instance().getNodeArray(index); }

	
#pragma mark -
#pragma mark Shortcut to RamCameraManager
	
	inline static ramCameraManager& getCameraManager() { return ramCameraManager::instance(); }

	inline static ofCamera& getActiveCamera() { return ramCameraManager::instance().getActiveCamera(); }
	
};