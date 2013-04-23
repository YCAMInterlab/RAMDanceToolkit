// 
// ramPhysics.cpp - RAMDanceToolkit
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

#include "ramPhysics.h"

#include "ramPrimitive.h"
#include "ramSoftBodyPrimitive.h"

ramPhysics* ramPhysics::_instance = NULL;

ramPhysics& ramPhysics::instance()
{
	if (!_instance)
	{
		_instance = new ramPhysics;
		_instance->setup();
	}
	return *_instance;
}

void ramPhysics::setup()
{
	if (inited) return;

	inited = true;

	world.setup(ofVec3f(0, -980, 0));
	world.addWorldBox(ofVec3f(-1000, 0, -1000), ofVec3f(1000, 1000, 1000));

	ofAddListener(ofEvents().update, this, &ramPhysics::onUpdate);
}

void ramPhysics::clear()
{
	world.clear();
}

void ramPhysics::debugDraw()
{
	world.draw();
}

void ramPhysics::onUpdate(ofEventArgs&)
{
	for (int i = 0; i < primitives.size(); i++)
	{
		primitives[i]->internal_update();
	}

	world.update();

	// remove all temporary primitives

	for (int i = 0; i < temporary_primitives.size(); i++)
	{
		ramBasePrimitive *p = temporary_primitives[i];
		delete p;
	}

	temporary_primitives.clear();
	cache_index.clear();
}

void ramPhysics::registerRigidBodyPrimitive(ramPrimitive *o)
{
	if (find(primitives.begin(), primitives.end(), o) != primitives.end()) return;

	primitives.push_back(o);
}

void ramPhysics::unregisterRigidBodyPrimitive(ramPrimitive *o)
{
	vector<ramBasePrimitive*>::iterator it = find(primitives.begin(), primitives.end(), o);
	if (it == primitives.end()) return;

	world.removeRigidBody(o->body);

	primitives.erase(it);
}

void ramPhysics::registerTempraryPrimitive(ramPrimitive *o)
{
	temporary_primitives.push_back(o);
}

void ramPhysics::registerSoftBodyPrimitive(ramSoftBodyPrimitive *o)
{
	if (find(primitives.begin(), primitives.end(), o) != primitives.end()) return;

	primitives.push_back(o);
}

void ramPhysics::unregisterSoftBodyPrimitive(ramSoftBodyPrimitive *o)
{
	vector<ramBasePrimitive*>::iterator it = find(primitives.begin(), primitives.end(), o);
	if (it == primitives.end()) return;

	world.removeSoftBody(o->body);

	primitives.erase(it);
}

bool ramPhysics::checkAndUpdateNodeCache(const ramNode *node)
{
	if (cache_index.find(node) == cache_index.end())
	{
		cache_index.insert(node);
		return true;
	}
	return false;
}