#pragma once

#include "ofMain.h"

#include "ofxOsc.h"

#include "ramConstants.h"
#include "ramUtils.h"
#include "ramActor.h"

class ramActorManager
{
public:

	// singleton
	inline static ramActorManager& instance()
    {
        if (_instance == NULL)
            _instance = new ramActorManager;
        return *_instance;
    }

	// actor
	inline size_t getNumActor() { return actors.size(); }
	inline const vector<string>& getActorNames() { return actors.keys(); }
	inline ramActor& getActor(int index) { return actors[index]; }
	inline ramActor& getActor(const string& name) { return actors[name]; }

	// rigidbody
	inline size_t getNumRigidBody() { return rigids.size(); }
	inline const vector<string>& getRigidBodyNames() { return rigids.keys(); }
	inline ramRigidBody& getRigidBody(int index) { return rigids[index]; }
	inline ramRigidBody& getRigidBody(const string& name) { return rigids[name]; }

	void update();
	void draw();
	void updateWithOscMessage(const ofxOscMessage &m);

private:

    
	static ramActorManager *_instance;

	// noncopyable
	ramActorManager() {};
	ramActorManager(const ramActorManager&) {}
	ramActorManager& operator=(const ramActorManager&) { return *this; }
	~ramActorManager() {};

	ramCompoundContainer<ramActor> actors;
	ramCompoundContainer<ramRigidBody> rigids;
};

ramActorManager* ramActorManager::_instance = NULL;
