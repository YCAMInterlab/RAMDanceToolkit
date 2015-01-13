// 
// ramPhysics.h - RAMDanceToolkit
// 
// Copyright 2012-2013 YCAM InterLab, Yoshito Onishi, Satoru Higa, Motoi Shimizu, and Kyle McDonald
// 
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
// 
//    http://www.apache.org/licenses/LICENSE-2.0
// 
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
    const ofxBt::World& getWorld() const { return const_cast<ofxBt::World&>(getWorld()); }
	ofxBt::SoftBodyWorld& getSoftBodyWorld() { return world; }
    const ofxBt::SoftBodyWorld& getSoftBodyWorld() const { return const_cast<ofxBt::SoftBodyWorld&>(getSoftBodyWorld()); }

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