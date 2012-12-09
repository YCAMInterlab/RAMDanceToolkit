#pragma once

#include "ofMain.h"

#include "ofxBt.h"

#include "ramActor.h"
#include "ramPrimitive.h"

class ramPrimitive;

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
	
private:
	
	static ramPhysics *_instance;
	
	bool inited;
	ofxBt::World world;
	vector<ramPrimitive*> primitives;
	
	ramPhysics() : inited(false) {}
	~ramPhysics() {}
	ramPhysics(const ramPhysics&);
	ramPhysics& operator=(const ramPhysics&);
	
};