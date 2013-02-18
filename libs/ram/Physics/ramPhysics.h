#pragma once

#include "ofMain.h"

#include "ofxBt.h"

#include "ramActor.h"
#include "ramPrimitive.h"

#include <set>

class ramPrimitive;

void ramEnablePhysicsPrimitive(bool v = true);
void ramDisablePhysicsPrimitive();
bool ramGetEnablePhysicsPrimitive();

class ramPhysics
{
public:
	
	static ramPhysics& instance();
	
	void setup();
	void clear();
	
	void debugDraw();
	
	ofxBt::World& getWorld() { return world; }
	
	void onUpdate(ofEventArgs&);
	
	void registerPrimitive(ramPrimitive *o);
	void unregisterPrimitive(ramPrimitive *o);
	
	void registerTempraryPrimitive(ramPrimitive *o);
	
	bool checkAndUpdateNodeCache(const ramNode *node);
	
private:
	
	static ramPhysics *_instance;
	
	bool inited;
	ofxBt::World world;
	vector<ramPrimitive*> primitives;
	vector<ramPrimitive*> temporary_primitives;
	
	set<const ramNode*> cache_index;
	
	ramPhysics() : inited(false) {}
	~ramPhysics() {}
	ramPhysics(const ramPhysics&);
	ramPhysics& operator=(const ramPhysics&);
	
};