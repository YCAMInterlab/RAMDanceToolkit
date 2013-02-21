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
	
	void setup();
	void update();
	void draw();

	// node
	inline size_t getNumNodeArray() { return nodearrays.size(); }
	inline const vector<string>& getNodeArrayNames() { return nodearrays.keys(); }
	inline ramNodeArray& getNodeArray(int index) { return nodearrays[index]; }
	inline ramNodeArray& getNodeArray(const string& name) { return nodearrays[name]; }
	inline bool hasNodeArray(const string &key) { return nodearrays.hasKey(key); }

	// for mouse picked node
	const ramNodeIdentifer& getLastSelectedNodeIdentifer();
	const ramNode* getLastSelectedNode();
	const ramNodeArray* getLastSelectedNodeArray();
	
	// Freeze all actor
	inline bool isFreezed() { return bFreeze; }
	inline void setFreezed(bool v) { bFreeze = v; }
	inline void toggleFreeze() { bFreeze ^= true; }

	// bus
	bool hasBus(const string& bus_name) { return bus.find(bus_name) != bus.end(); }
	void setBus(const string& bus_name, const ramNodeArray &arr) { bus[bus_name] = arr; }
	const ramNodeArray& getBus(const string& bus_name) { return bus[bus_name]; }
	map<string, ramNodeArray>& getAllBus() {return bus;};
	
	// for internal use
	void updateWithOscMessage(const ofxOscMessage &m);
	void setupOscReceiver(int port) { oscReceiver.setup(port); }
	
	void onSelectStateChanged(ramNodeIdentifer &e);
	
private:

	static ramActorManager *_instance;
	
	ofxOscReceiver oscReceiver;
	
	// noncopyable
	ramActorManager() {};
	ramActorManager(const ramActorManager&) {}
	ramActorManager& operator=(const ramActorManager&) { return *this; }
	~ramActorManager() {};

	ramCompoundContainer<ramNodeArray> nodearrays;
	
	ofxInteractivePrimitives::RootNode rootNode;
	
	bool bFreeze;
	
	class NodeSelector;
	friend class NodeSelector;
	NodeSelector *nodeSelector;
	
	map<string, ramNodeArray> bus;
};

