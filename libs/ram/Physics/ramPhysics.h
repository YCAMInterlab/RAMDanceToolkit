#pragma once

#include "ofMain.h"

#pragma managed(push, off)
#include "ofxBt.h"
#pragma managed(pop)

#include "ramActor.h"
#include "ramPrimitive.h"

class ramPrimitive;

void ramSetEnablePhysicsPrimitive(bool v);
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
	
private:
	
	static ramPhysics *_instance;
	
	bool inited;
	ofxBt::World world;
	vector<ramPrimitive*> primitives;
	vector<ramPrimitive*> temporary_primitives;
	
	ramPhysics() : inited(false) {}
	~ramPhysics() {}
	ramPhysics(const ramPhysics&);
	ramPhysics& operator=(const ramPhysics&);
	
};