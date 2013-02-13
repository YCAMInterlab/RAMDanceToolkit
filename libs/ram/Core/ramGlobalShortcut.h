#pragma once

#include "ofMain.h"

#include "ramActorManager.h"
#include "ramCameraManager.h"

struct ramGlobalShortcut
{
	
	inline ramActorManager& getActorManager() { return ramActorManager::instance(); }
	
	inline bool hasActor(const string &key) { return ramActorManager::instance().hasActor(key); }
	
	inline ramActor& getActor(string name) { return ramActorManager::instance().getActor(name); }
	
	
	inline size_t getNumActor() { return ramActorManager::instance().getNumActor(); }
	inline ramActor& getActor(int index) { return ramActorManager::instance().getActor(index); }
	
	
	inline bool hasRigidBody(const string &key) { return ramActorManager::instance().hasRigidBody(key); }
	
	inline ramRigidBody& getRigidBody(string name) { return ramActorManager::instance().getRigidBody(name); }
	
	
	inline size_t getNumRigidBody() { return ramActorManager::instance().getNumRigidBody(); }
	
	inline ramRigidBody& getRigidBody(int index) { return ramActorManager::instance().getRigidBody(index); }

	
	inline bool hasNodeArray(const string &key) { return ramActorManager::instance().hasNodeArray(key); }
	
	inline ramNodeArray& getNodeArray(string name) { return ramActorManager::instance().getNodeArray(name); }
	
	
	inline size_t getNumNodeArray() { return ramActorManager::instance().getNumNodeArray(); }
	
	inline ramNodeArray& getNodeArray(int index) { return ramActorManager::instance().getNodeArray(index); }

	//
	
	inline ramCameraManager& getCameraManager() { return ramCameraManager::instance(); }

	inline ofCamera& getActiveCamera() { return ramCameraManager::instance().getActiveCamera(); }

};