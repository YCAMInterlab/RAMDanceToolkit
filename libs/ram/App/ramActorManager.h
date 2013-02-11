#pragma once

#include "ofMain.h"

#include "ofxOsc.h"
#include "ofxInteractivePrimitives.h"

#include "ramConstants.h"
#include "ramUtils.h"
#include "ramActor.h"
#include "ramNodeIdentifer.h"

class ramActorManager
{
public:
	
	ofEvent<ramActor> actorEntered;
	ofEvent<ramActor> actorExited;
	ofEvent<ramRigidBody> rigidEntered;
	ofEvent<ramRigidBody> rigidExited;
	
	ofEvent<ramNodeIdentifer> selectStateChanged;
	
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
	
	// node
	inline size_t getNumNodeArray() { return nodearray.size(); }
	inline const vector<string>& getNodeArrayNames() { return nodearray.keys(); }
	inline ramNodeArray& getNodeArray(int index) { return nodearray[index]; }
	inline ramNodeArray& getNodeArray(const string& name) { return nodearray[name]; }

	void setup();
	void update();
	void draw();
	
	void updateWithOscMessage(const ofxOscMessage &m);
	
	const ramNodeIdentifer& getSelectedNode();
	
	void onSelectStateChanged(ramNodeIdentifer &e);
//	void setActorToBus(const string& bus_name, );

private:

	static ramActorManager *_instance;

	// noncopyable
	ramActorManager() {};
	ramActorManager(const ramActorManager&) {}
	ramActorManager& operator=(const ramActorManager&) { return *this; }
	~ramActorManager() {};

	ramCompoundContainer<ramNodeArray> nodearray;
	ramCompoundContainer<ramActor> actors;
	ramCompoundContainer<ramRigidBody> rigids;
	
	ofxInteractivePrimitives::RootNode rootNode;
	
	class NodeSelector;
	friend class NodeSelector;
	NodeSelector *nodeSelector;
};

