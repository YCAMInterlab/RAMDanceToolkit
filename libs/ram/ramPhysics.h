#pragma once

#include "ofMain.h"

#include "ofxBt.h"

#include "ramActor.h"

class ramPhysics
{
public:
	
	static ramPhysics& instance();
	
	void setup();
	void clear();
	
	void debugDraw();
	
	ofxBt::World& getWorld() { return world; }
	
	void onUpdate(ofEventArgs&);
	
private:
	
	static ramPhysics *_instance;
	
	bool inited;
	ofxBt::World world;
	
	ramPhysics() : inited(false) {}
	~ramPhysics() {}
	ramPhysics(const ramPhysics&);
	ramPhysics& operator=(const ramPhysics&);
	
};