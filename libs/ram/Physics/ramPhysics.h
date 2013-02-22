#pragma once

#include "ofMain.h"

#pragma managed(push, off)
#include "ofxBt.h"
#pragma managed(pop)

#include "ramActor.h"
#include "ramPrimitive.h"

#include <set>

class ramBasePrimitive;
class ramPrimitive;
class ramSoftBodyPrimitive;

class ramPhysics
{
public:

	static ramPhysics& instance();

	void setup();
	void clear();

	void debugDraw();

	ofxBt::World& getWorld() { return world; }
	ofxBt::SoftBodyWorld& getSoftBodyWorld() { return world; }

	void onUpdate(ofEventArgs&);

	void registerRigidBodyPrimitive(ramPrimitive *o);
	void unregisterRigidBodyPrimitive(ramPrimitive *o);
	void registerTempraryPrimitive(ramPrimitive *o);

	void registerSoftBodyPrimitive(ramSoftBodyPrimitive *o);
	void unregisterSoftBodyPrimitive(ramSoftBodyPrimitive *o);

	bool checkAndUpdateNodeCache(const ramNode *node);

private:

	static ramPhysics *_instance;

	bool inited;
	ofxBt::SoftBodyWorld world;

	vector<ramBasePrimitive*> primitives;
	vector<ramBasePrimitive*> temporary_primitives;

	set<const ramNode*> cache_index;

	ramPhysics() : inited(false) {}
	~ramPhysics() {}
	ramPhysics(const ramPhysics&);
	ramPhysics& operator=(const ramPhysics&);

};