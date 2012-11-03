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
	static ramActorManager& instance();

	ramActorManager() {};
	~ramActorManager() {};

	void update();
	void draw();

	// actor
	size_t getNumActor() { return actors.size(); }
	const vector<string>& getActorNames() { return actors.keys(); }
	ramActor& getActor(int index) { return actors[index]; }
	ramActor& getActor(const string& name) { return actors[name]; }

	// rigidbody
	size_t getNumRigidBody() { return rigids.size(); }
	const vector<string>& getRigidBodyNames() { return rigids.keys(); }
	ramRigidBody& getRigidBody(int index) { return rigids[index]; }
	ramRigidBody& getRigidBody(const string& name) { return rigids[name]; }

	
	// update actors, rigids with bvh data
	void updateWithOscMessage(const ofxOscMessage &m);

private:

	static ramActorManager *_instance;

	// noncopyable
	ramActorManager(const ramActorManager&) {}
	ramActorManager& operator=(const ramActorManager&) {}

	ramCompoundContainer<ramActor> actors;
	ramCompoundContainer<ramRigidBody> rigids;
};